#include "stdafx.h"
#include "Button.h"

CButton::CButton(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{

}

CButton::~CButton(void)
{

}

HRESULT CButton::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vScale = { 80.f, 80.f, 1.f };

	m_pTransCom->Update_Component();

	return S_OK;
}

_int CButton::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_bButtonRender = FALSE;

	return UPDATE_OK;
}

_int CButton::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	if (TRUE == m_bButtonRender)
		m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_UI, this);

	return UPDATE_OK;
}

void CButton::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMpBar_GREATER);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);
}

void CButton::Render_Object(void)
{
	if (FALSE == m_bButtonRender)
		return;

	PreRender_Object();

	/*  RcTex Buffer, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(0);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}

	if (1 == m_bPosition)
	{
		_vec2 vPos = _vec2((m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].x / 2.f + WINCX / 2.f) + 25.f,
			(WINCY / 2.f - m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].y / 2.f) - 8.f);
		Engine::Render_Font(L"Font_UI", m_szButtonTag, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (2 == m_bPosition)
	{
		_vec2 vPos = _vec2((m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].x / 2.f + WINCX / 2.f) + 25.f,
			(WINCY / 2.f - m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].y / 2.f) + 1.5f);
		Engine::Render_Font(L"Font_UI", m_szButtonTag, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}

	PostRender_Object();
}

void CButton::PostRender_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CButton::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Render_Texture(pEffect, "g_BaseTexture", _uint(m_eButtonUI));

	Safe_Release(pEffect);

	return S_OK;
}

void CButton::Set_ButtonSetting(const BUTTON_UI _eButtonUI, const _tchar* _szButtonTag)
{
	m_bButtonRender = TRUE;
	m_eButtonUI = _eButtonUI;
	lstrcpy(m_szButtonTag, _szButtonTag);
}

HRESULT CButton::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Button"));
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
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Alpha"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SHADER_ALPHA, pComponent);

	return S_OK;
}

CButton* CButton::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CButton*	pInstance = new CButton(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CButton::Free(void)
{
	Engine::CGameObject::Free();
}