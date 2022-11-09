#include "stdafx.h"
#include "SkyBoxL.h"

#include "Export_Function.h"

CSkyBoxL::CSkyBoxL(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CSkyBoxL::~CSkyBoxL(void)
{
}

HRESULT CSkyBoxL::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(0.f, -28.f, 0.f);
	m_pTransCom->Set_Scale(0.0005f, 0.0005f, 0.0005f);

	m_pTransCom->Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(-45.f));

	return S_OK;
}

_int CSkyBoxL::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	if (0.5f <= dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_At().y)
	{
		if (0.f < m_fAlpha)
			m_fAlpha += -170.f * fTimeDelta;
		else
			m_fAlpha = 0.f;
	}

	return 0;
}

void CSkyBoxL::PreRender_Object(void)
{
	/*  Alpha Blending On  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(DWORD(m_fAlpha), 255, 255, 255));

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CSkyBoxL::Render_Object(void)
{
	PreRender_Object();

	m_pMeshCom->Render_Meshe();

	PostRender_Object();
}

void CSkyBoxL::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	/*  Alpha Blending Off  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

HRESULT CSkyBoxL::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Mesh  */
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_SkyBoxL"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_MESH, pComponent);

	/*  Transform  */
	pComponent = m_pTransCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_DYNAMIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM, pComponent);

	/*  Renderer  */
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_RENDERER, pComponent);

	return S_OK;
}

CSkyBoxL * CSkyBoxL::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBoxL *	pInstance = new CSkyBoxL(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);
	
	return pInstance;
}

void CSkyBoxL::Free(void)
{
	Engine::CGameObject::Free();
}

