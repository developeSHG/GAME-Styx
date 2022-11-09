#include "stdafx.h"
#include "TitleLight.h"

CTitleLight::CTitleLight(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CTitleLight::~CTitleLight(void)
{
}

HRESULT CTitleLight::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vInfo[Engine::INFO_POS].x = 1.45f;
	m_pTransCom->m_vInfo[Engine::INFO_POS].y = 0.5f;
	m_pTransCom->m_vScale.x = 2.5f;
	m_pTransCom->m_vScale.y = 2.f;

	m_tMtrl = InitMtrl(WHITE, WHITE, WHITE, BLACK, 1.f);
	//m_tMtrl.Diffuse.a = 0.1f;

	return S_OK;
}

_int CTitleLight::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	//if (0.f < m_tMtrl.Diffuse.a)
	//	m_tMtrl.Diffuse.a += -0.7f * fTimeDelta;
	//else
	//	m_tMtrl.Diffuse.a = 0.f;

	if (0.5f <= dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_At().y)
	{
		if (0.f < m_fAlpha)
			m_fAlpha += -170.f * fTimeDelta;
		else
			m_fAlpha = 0.f;
	}
	else
	{
		if (TRUE == m_bLight)
		{
			m_fAlphaPower -= 55.f * fTimeDelta;
			m_fAlpha += -m_fAlphaPower * fTimeDelta;

			if (50.f > m_fAlpha)
			{
				m_bLight = !(m_bLight);
				m_fAlphaPower = 170.f;
			}
		}
		else
		{
			m_fAlphaPower -= 55.f * fTimeDelta;
			m_fAlpha += m_fAlphaPower * fTimeDelta;

			if (250.f < m_fAlpha)
			{
				m_bLight = !(m_bLight);
				m_fAlphaPower = 170.f;
			}
		}
	}

	return UPDATE_OK;
}

_int CTitleLight::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CTitleLight::PreRender_Object(void)
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

	/*  Alpha Blending On  */
	//m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	/*  Alpha Test On  */
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0X00000088);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMpBar_GREATER);

	/*  Z Write Buffer On  */
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

void CTitleLight::Render_Object(void)
{
	PreRender_Object();

	m_pTextureCom->Render_Texture(1);
	m_pBufferCom->Render_Buffer();

	PostRender_Object();
}

void CTitleLight::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	/*  Alpha Blending Off  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	/*  Alpha Test Off  */
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	/*  Z Write Buffer Off  */
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CTitleLight::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_LOGO, L"Texture_StyxTitle"));
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

CTitleLight * CTitleLight::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTitleLight *	pInstance = new CTitleLight(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);
	
	return pInstance;
}

void CTitleLight::Free(void)
{
	Engine::CGameObject::Free();
}

