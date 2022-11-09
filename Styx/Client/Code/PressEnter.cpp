#include "stdafx.h"
#include "PressEnter.h"

CPressEnter::CPressEnter(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CPressEnter::~CPressEnter(void)
{
}

HRESULT CPressEnter::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vInfo[Engine::INFO_POS].x = 1.45f;
	m_pTransCom->m_vInfo[Engine::INFO_POS].y = -0.8f;
	m_pTransCom->m_vScale.x = 0.75f;
	m_pTransCom->m_vScale.y = 0.1f;

	m_tMtrl = InitMtrl(WHITE, WHITE, WHITE, BLACK, 1.f);

	return S_OK;
}

_int CPressEnter::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	if (TRUE == m_bLoading && Engine::Get_DIKeyDown(DIK_RETURN) || TRUE == m_bEnter)
	{
		m_fCount += 12.f * fTimeDelta;

		if (11.f < m_fCount)
			m_fCount = 10.f;

		m_bAlpha = TRUE;
		m_bEnter = TRUE;
	}
	else if(TRUE == m_bLoading && FALSE == m_bEnter)
	{

		if (TRUE == m_bAlpha)
		{
			m_fCount += 12.f * fTimeDelta;

			if (8.5f < m_fCount)
			{
				m_fCount = 8.5f;
				m_bAlpha = FALSE;
			}
		}
		else
		{
			m_fCount -= 12.f * fTimeDelta;

			if (0.f > m_fCount)
			{
				m_fCount = 0.f;
				m_bAlpha = TRUE;
			}
		}
	}
	

	return UPDATE_OK;
}

_int CPressEnter::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CPressEnter::PreRender_Object(void)
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

	/*  Alpha Test On  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0X00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	/*  Z Write Buffer On  */
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

void CPressEnter::Render_Object(void)
{
	if (FALSE == m_bLoading)
		return;

	PreRender_Object();

	m_pTextureCom->Render_Texture(_uint(m_fCount));
	m_pBufferCom->Render_Buffer();

	PostRender_Object();
}

void CPressEnter::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	/*  Alpha Blending Off  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	/*  Alpha Test Off  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, false);

}

HRESULT CPressEnter::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_LOGO, L"Texture_PressEnter"));
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

CPressEnter * CPressEnter::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPressEnter *	pInstance = new CPressEnter(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);
	
	return pInstance;
}

void CPressEnter::Free(void)
{
	Engine::CGameObject::Free();
}

