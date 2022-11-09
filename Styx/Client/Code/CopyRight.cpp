#include "stdafx.h"
#include "CopyRight.h"

CCopyRight::CCopyRight(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CCopyRight::~CCopyRight(void)
{
}

HRESULT CCopyRight::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].x = 0.f;
	m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].y = -560.f;
	m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].z = 1.f;
	m_pTransCom->m_vScale.x = 2000.f;
	m_pTransCom->m_vScale.y = 90.f;

	m_tMtrl = InitMtrl(WHITE, WHITE, WHITE, BLACK, 1.f);

	return S_OK;
}

_int CCopyRight::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_UI, this);

	if (FALSE == SCAST<CSceneAlpha*>(Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_SCENEALPHA)->back())->Get_Fade())
	{
		if (0.f < m_fAlpha)
			m_fAlpha += -170.f * fTimeDelta;
		else
			m_fAlpha = 0.f;
	}
	else
	{
		if (1.25f <= dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_At().y)
		{
			if (255.f > m_fAlpha)
				m_fAlpha += 170.f * fTimeDelta;
			else
				m_fAlpha = 255.f;
		}
	}

	return UPDATE_OK;
}

_int CCopyRight::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CCopyRight::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetMaterial(&m_tMtrl);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	/*  Alpha Blending On  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(DWORD(m_fAlpha), 255, 255, 255));

	/*  OrthoView Init */
	D3DXMATRIX matView, matProj;
	D3DXMatrixIdentity(&matView);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 1.f, 10.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CCopyRight::Render_Object(void)
{
	if (0.5f >= dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_At().y)
		return;

	PreRender_Object();

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	PostRender_Object();
}

void CCopyRight::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	/*  Alpha Blending Off  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	/*  PerspectiveView Init */
	SCAST<Engine::CCamera*>(Engine::Find_DynamicCameraObject())->TransformViewSpaceMatrix();
	SCAST<Engine::CCamera*>(Engine::Find_DynamicCameraObject())->TransformProjectionMatrix();
}

HRESULT CCopyRight::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_LOGO, L"Texture_CopyRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TEXTURE, pComponent);

	/*  Transform  */
	pComponent = m_pTransCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM, pComponent);

	/*  Renderer  */
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_RENDERER, pComponent);


	return S_OK;
}

CCopyRight * CCopyRight::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCopyRight *	pInstance = new CCopyRight(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);
	
	return pInstance;
}

void CCopyRight::Free(void)
{
	Engine::CGameObject::Free();
}

