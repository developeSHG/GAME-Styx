#include "stdafx.h"
#include "HpBar.h"

CHpBar::CHpBar(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{

}

CHpBar::~CHpBar(void)
{

}

HRESULT CHpBar::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(976.f, -528.f, 1.f);
	m_pTransCom->m_vScale = { 170.f, 90.f, 1.f };

	m_pTransCom->Update_Component();

	return S_OK;
}

_int CHpBar::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	return UPDATE_OK;
}

_int CHpBar::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_UI, this);

	return UPDATE_OK;
}

void CHpBar::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CHpBar::Render_Object(void)
{
	PreRender_Object();

	/*  RcTex Buffer, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(1);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}

	PostRender_Object();
}

void CHpBar::PostRender_Object(void)
{
	
}

HRESULT CHpBar::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Render_Texture(pEffect, "g_BaseTexture");

	Safe_Release(pEffect);

	return S_OK;
}

HRESULT CHpBar::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HpBar"));
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

CHpBar* CHpBar::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CHpBar*	pInstance = new CHpBar(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CHpBar::Free(void)
{
	Engine::CGameObject::Free();
}