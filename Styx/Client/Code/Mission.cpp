#include "stdafx.h"
#include "Mission.h"

CMission::CMission(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	
}

CMission::~CMission(void)
{
	
}

HRESULT CMission::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);
 
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(980.f, 360.f, 1.f);
	m_pTransCom->m_vScale = { 310.f, 90.f, 1.f };

	m_pTransCom->Update_Component();

	m_vMissionChangePos = _vec3(0.f, 0.f, 1.f);

	return S_OK;
}

_int CMission::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (TRUE == m_bMissionChange
		&& m_pTransCom->m_vInfo[Engine::INFO_POS].y - 0.2f >= m_vMissionChangePos.y)
	{
		if (5.f <= m_fAlpha[0])
			m_fAlpha[0] += -180.f * fTimeDelta;
		else
			m_fAlpha[0] = 0.f;

		if (250.f >= m_fAlpha[1])
			m_fAlpha[1] += 180.f * fTimeDelta;
		else
			m_fAlpha[1] = 255.f;

		m_fTime += 1.f * fTimeDelta;

		if (3.f < m_fTime)
		{
			_vec3 vPos = m_pTransCom->m_vInfo[Engine::INFO_POS];
			vPos.z = 500.f;
			_vec3 vDir = vPos - m_vMissionChangePos;
			m_vMissionChangePos += *D3DXVec3Normalize(&vDir, &vDir) * 1750.f * fTimeDelta;
			m_vMissionChangePos.z = 1.f;
		}
	}

	return UPDATE_OK;
}

_int CMission::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_UI, this);

	return UPDATE_OK;
}

void CMission::PreRender_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMpBar_GREATER);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

	/*  Alpha Blending On  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(DWORD(255), 255, 255, 255));
}

void CMission::Render_Object(void)
{
	PreRender_Object();

	/*  RcTex Buffer, Shader Render  */
	{
		//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		//NULL_CHECK(pEffect);

		//SetUp_ConstantTable(pEffect);

		//_uint iPassCnt = 0;

		//pEffect->Begin(&iPassCnt, 0);
		//pEffect->BeginPass(1);

		//m_pBufferCom->Render_Buffer();

		//pEffect->EndPass();
		//pEffect->End();
	}
	if (0.f < m_fAlpha[0])
	{
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(DWORD(m_fAlpha[0]), 255, 255, 255));
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();
	}

	if (TRUE == m_bMissionChange)
	{
		_matrix matScale, matTrans;
		D3DXMatrixScaling(&matScale, m_pTransCom->m_vScale.x, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z);
		D3DXMatrixTranslation(&matTrans, m_vMissionChangePos.x, m_vMissionChangePos.y, m_vMissionChangePos.z);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(DWORD(m_fAlpha[1]), 255, 255, 255));
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &(matScale * matTrans));
		m_pTextureCom->Render_Texture(1);
		m_pBufferCom->Render_Buffer();
	}

	PostRender_Object();
}

void CMission::PostRender_Object(void)
{
	/*  Alpha Blending Off  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

HRESULT CMission::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Render_Texture(pEffect, "g_BaseTexture", 0);

	Safe_Release(pEffect);

	return S_OK;
}

HRESULT CMission::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Mission"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TEXTURE, pComponent);

	/*  Transform  */
	/*  Static Choice!!  */
	pComponent = m_pTransCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM, pComponent);

	/*  Renderer  */
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_RENDERER, pComponent);

	/*  Shader  */
	/*pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Alpha"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SHADER_ALPHA, pComponent);*/

	return S_OK;
}

CMission* CMission::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CMission*	pInstance = new CMission(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMission::Free(void)
{
	Engine::CGameObject::Free();
}