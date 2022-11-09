#include "Renderer.h"

#include "LightMgr.h"
#include "Target_Manager.h"
#include "VIBuffer_ViewPort.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

#include "Export_Function.h"
#include "Camera.h"

Engine::CRenderer::CRenderer(void)
	: m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLightMgr::GetInstance())
{

}

Engine::CRenderer::~CRenderer(void)
{
	Free();
}

HRESULT Engine::CRenderer::Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;

	D3DVIEWPORT9			ViewPortDesc;
	m_pGraphicDev->GetViewport(&ViewPortDesc);

	// 디퍼드를 위한 렌더타겟들을 생성하자.

	// For.Target_Diffuse
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphicDev, L"Target_Diffuse", ViewPortDesc.Width, ViewPortDesc.Height, D3DFMT_A8R8G8B8, D3DXCOLOR(1.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// For.Target_Normal
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphicDev, L"Target_Normal", ViewPortDesc.Width, ViewPortDesc.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Depth
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphicDev, L"Target_Depth", ViewPortDesc.Width, ViewPortDesc.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Shade
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphicDev, L"Target_Shade", ViewPortDesc.Width, ViewPortDesc.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Specular
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphicDev, L"Target_Specular", ViewPortDesc.Width, ViewPortDesc.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// 동시에 장치에 셋팅되는 타겟들을 묶어놓자(MRT)
	// For.MRT_Deferred
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Diffuse")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Normal")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Depth")))
		return E_FAIL;

	// For.MRT_LightAcc
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Shade")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Specular")))
		return E_FAIL;

	// For.Shader_LightAcc
	m_pShader_LightAcc = CShader::Create(m_pGraphicDev, L"../Bin/ShaderFiles/Shader_LightAcc.fx");
	if (nullptr == m_pShader_LightAcc)
		return E_FAIL;

	// For.Shader_Blend
	m_pShader_Blend = CShader::Create(m_pGraphicDev, L"../Bin/ShaderFiles/Shader_Blend.fx");
	if (nullptr == m_pShader_Blend)
		return E_FAIL;

	// For.VIBuffer_ViewPort
	m_pVIBuffer = CVIBuffer_ViewPort::Create(m_pGraphicDev, -0.5f, -0.5f, ViewPortDesc.Width, ViewPortDesc.Height);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

//#ifdef _DEBUG
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Diffuse", 0.f, 0.0f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Normal", 0.f, 200.0f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Depth", 0.f, 400.0f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Shade", 200.f, 0.0f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Specular", 200.f, 200.0f, 200.f, 200.f)))
		return E_FAIL;
//#endif // _DEBUG

	return NOERROR;
}

void Engine::CRenderer::Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	if (RENDERID::RENDER_END <= eGroup)
		return;

	m_RendererGroup[eGroup].push_back(pGameObject);

	//pGameObject->AddRef();
}

void Engine::CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
	//for (_uint i = 0; i < RENDER_END; ++i)
	//{
	//	for (auto& iter : m_RendererGroup[i])
	//	{
	//		iter->Render_Object();
	//		Safe_Release(iter);			// 레퍼런스 카운트 감수
	//	}
	//	m_RendererGroup[i].clear();
	//}
	m_pGraphicDev = pGraphicDev;
		
	if (m_RendererGroup[RENDERID::RENDER_PRIORITY].size())
		Render_Priority(pGraphicDev);

	Render_NonAlpha(pGraphicDev);

	if (2 == Engine::Get_CurScene())
	{
		if (FAILED(Render_LightAcc())) // Shade에 그린다.
			return;

		// 백버퍼에 찍는다.
		if (FAILED(Render_Blend()))
			return;
	}

	if (m_RendererGroup[RENDERID::RENDER_ALPHA].size())
		Render_Alpha(pGraphicDev);

	if (m_RendererGroup[RENDERID::RENDER_BLENDTEST].size())
		Render_BlendTest(pGraphicDev);

	if (m_RendererGroup[RENDERID::RENDER_LAST].size())
		Render_Last(pGraphicDev);

	if(m_RendererGroup[RENDERID::RENDER_UI].size())
		Render_UI(pGraphicDev);

	for (_uint i = 0; i < RENDERID::RENDER_END; ++i)
		m_RendererGroup[i].clear();


//#ifdef _DEBUG
	//if (2 == Engine::Get_CurScene())
	{
		if (nullptr == m_pTarget_Manager)
			return;

		if (Engine::Get_DIKeyDown(DIK_F2))
			m_bRenderTargetView = !m_bRenderTargetView;

		if (TRUE == m_bRenderTargetView)
		{
			if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(L"MRT_Deferred")))
				return;
			if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(L"MRT_LightAcc")))
				return;
		}
	}
//#endif // _DEBUG
}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for (_uint i = 0; i < RENDERID::RENDER_END; ++i)
	{
		//for_each(m_RendererGroup[i].begin(), m_RendererGroup[i].end(), CDeleteObj());
		m_RendererGroup[i].clear();
	}
}

void Engine::CRenderer::Render_Priority(LPDIRECT3DDEVICE9 pGraphicDev)
{
	/*  PreRender  */
	{
		pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
		pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	for (auto& iter : m_RendererGroup[RENDERID::RENDER_PRIORITY])
	{
		if (nullptr != iter)
			iter->Render_Object();
	}

	/*  PostRender  */
	{
		pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

void Engine::CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (nullptr == m_pTarget_Manager)
		return ;

	// Diffuse + Normal
	if (2 == Engine::Get_CurScene())
	{
		if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Deferred")))
			return;
	}

	for (auto& iter : m_RendererGroup[RENDER_NONALPHA])
	{
		if(nullptr != iter)
			iter->Render_Object();
	}

	if (2 == Engine::Get_CurScene())
	{
		if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Deferred")))
			return;
	}

	/*if (2 == Engine::Get_CurScene())
		pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);*/
}

_bool	Compare_Z(CGameObject* pDest, CGameObject* pSrc)
{
	return pDest->Get_ViewZ() > pSrc->Get_ViewZ();
}

void Engine::CRenderer::Render_Alpha(LPDIRECT3DDEVICE9 pGraphicDev)
{
	/*  PreRender  */
	{
		pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	m_RendererGroup[RENDERID::RENDER_ALPHA].sort(Compare_Z);

	for (auto& iter : m_RendererGroup[RENDERID::RENDER_ALPHA])
	{
		if (nullptr != iter)
			iter->Render_Object();
	}

	/*  PostRender  */
	{
		pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

void Engine::CRenderer::Render_BlendTest(LPDIRECT3DDEVICE9 pGraphicDev)
{
	/*  PreRender  */
	{
		pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pGraphicDev->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
		pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	}

	for (auto& iter : m_RendererGroup[RENDERID::RENDER_BLENDTEST])
	{
		if (nullptr != iter)
			iter->Render_Object();
	}

	/*  PostRender  */
	{
		pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}


void Engine::CRenderer::Render_Last(LPDIRECT3DDEVICE9 pGraphicDev)
{
	for (auto& iter : m_RendererGroup[RENDERID::RENDER_LAST])
	{
		if (nullptr != iter)
			iter->Render_Object();
	}
}

void Engine::CRenderer::Render_UI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	/*  PreRender  */
	{
		pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
		pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
		pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		/*  OrthoView Init */
		D3DXMATRIX matView, matProj;
		D3DXMatrixIdentity(&matView);
		pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixOrthoLH(&matProj, 1280.f * 2.f, 720.f * 2.f, 1.f, 10.f);
		pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	}

	for (auto& iter : m_RendererGroup[RENDERID::RENDER_UI])
	{
		if (nullptr != iter)
			iter->Render_Object();
	}

	/*  PostRender  */
	{
		//pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

		/*  PerspectiveView Init */
		static_cast<Engine::CCamera*>(Engine::Find_DynamicCameraObject())->TransformViewSpaceMatrix();
		static_cast<Engine::CCamera*>(Engine::Find_DynamicCameraObject())->TransformProjectionMatrix();
	}
}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();

	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pShader_Blend);
	Safe_Release(m_pShader_LightAcc);
}

HRESULT Engine::CRenderer::Render_LightAcc()
{
	if (nullptr == m_pTarget_Manager ||
		nullptr == m_pShader_LightAcc ||
		nullptr == m_pLight_Manager)
		return E_FAIL;

	// Shade
	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_LightAcc")))
		return E_FAIL;

	if (FAILED(m_pShader_LightAcc->Set_Texture("g_NormalTexture", m_pTarget_Manager->Get_TargetTexture(L"Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pShader_LightAcc->Set_Texture("g_DepthTexture", m_pTarget_Manager->Get_TargetTexture(L"Target_Depth"))))
		return E_FAIL;

	_matrix		matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_matrix			matViewInv, matProjInv;
	D3DXMatrixInverse(&matProjInv, nullptr, &matProj);
	D3DXMatrixInverse(&matViewInv, nullptr, &matView);

	_vec3 vEye;
	memcpy(&vEye, &matViewInv.m[3], sizeof(_vec3));

	if (FAILED(m_pShader_LightAcc->Set_Value("g_matProjInv", &matProjInv, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShader_LightAcc->Set_Value("g_matViewInv", &matViewInv, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShader_LightAcc->Set_Value("g_vCamPosition", &_vec4(vEye, 1.f), sizeof(_vec4))))
		return E_FAIL;

	m_pShader_LightAcc->Get_EffectHandle()->Begin(nullptr, 0);
	
	m_pLight_Manager->Render_Light(m_pShader_LightAcc);

	m_pShader_LightAcc->Get_EffectHandle()->End();

	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_LightAcc")))
		return E_FAIL;

	return NOERROR;
}

HRESULT Engine::CRenderer::Render_Blend()
{
		if (nullptr == m_pVIBuffer || 
			nullptr == m_pShader_Blend)
		return E_FAIL;

	if (FAILED(m_pShader_Blend->Set_Texture("g_DiffuseTexture", m_pTarget_Manager->Get_TargetTexture(L"Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pShader_Blend->Set_Texture("g_ShadeTexture", m_pTarget_Manager->Get_TargetTexture(L"Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pShader_Blend->Set_Texture("g_SpecularTexture", m_pTarget_Manager->Get_TargetTexture(L"Target_Specular"))))
		return E_FAIL;
	if (FAILED(m_pShader_Blend->Set_Texture("g_DepthTexture", m_pTarget_Manager->Get_TargetTexture(L"Target_Depth"))))
		return E_FAIL;
	//if (FAILED(m_pShader_Blend->Set_Texture("g_NormalTexture", m_pTarget_Manager->Get_TargetTexture(L"Target_Normal"))))
	//	return E_FAIL;

	m_pShader_Blend->Get_EffectHandle()->Begin(nullptr, 0);
	m_pShader_Blend->Get_EffectHandle()->BeginPass(0);

	m_pVIBuffer->Render_Buffer();

	m_pShader_Blend->Get_EffectHandle()->EndPass();
	m_pShader_Blend->Get_EffectHandle()->End();

	return NOERROR;
}
