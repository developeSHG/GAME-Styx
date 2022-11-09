#include "stdafx.h"
#include "TorchFire.h"

#include "Export_Function.h"

CTorchFire::CTorchFire(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	
}

CTorchFire::~CTorchFire(void)
{
	
}

HRESULT CTorchFire::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);
 
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Engine::CTransform*	pParentTransCom = dynamic_cast<Engine::CTransform*>(m_pParentObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));
	NULL_CHECK_RETURN(pParentTransCom, E_FAIL);

	m_pTransCom->m_vInfo[Engine::INFO::INFO_POS] = pParentTransCom->m_vInfo[Engine::INFO::INFO_POS];
	m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].y += 1.85f;

	_uint uAngle = _uint(D3DXToDegree(pParentTransCom->m_vAngle.y));

	if (0 == uAngle)
	{
		m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].x -= 0.03f;
		m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].z += 0.27f;
	}
	else if (90 == uAngle)
	{
		m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].x += 0.27f;
		m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].z += 0.02f;
	}
	else if (180 == uAngle)
	{
		m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].x += 0.02f;
		m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].z -= 0.27f;
	}
	else if (270 == uAngle)
	{
		m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].x -= 0.27f;
		m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].z -= 0.03f;
	}

	m_pTransCom->m_vScale = { 1.2f, 1.2f, 1.2f };

	return S_OK;
}

_int CTorchFire::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	//m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_BLENDTEST, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	m_fFrame += 36.f * fTimeDelta;

	if (36.f < m_fFrame)
		m_fFrame = 0.f;

	return UPDATE_OK;
}

_int CTorchFire::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CTorchFire::PreRender_Object(void)
{
	BillBoard();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CTorchFire::Render_Object(void)
{
	PreRender_Object();

	/*  RcTex Buffer, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(2);

		m_pBufferCom->Render_Buffer();
		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}

	//m_pTextureCom->Render_Texture(DWORD(m_fFrame));
	//m_pBufferCom->Render_Buffer();

	PostRender_Object();
}

void CTorchFire::PostRender_Object(void)
{

}

HRESULT CTorchFire::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Render_Texture(pEffect, "g_BaseTexture", DWORD(m_fFrame));

	Safe_Release(pEffect);

	return S_OK;
}

HRESULT CTorchFire::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_TorchFire"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TEXTURE, pComponent);

	/*  Transform  */
	pComponent = m_pTransCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_DYNAMIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM, pComponent);

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

void CTorchFire::BillBoard(void)
{
	_matrix matWorld, matView, matBill;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pTransCom->m_matWorld = (matBill * matWorld);
}

CTorchFire* CTorchFire::Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pParentObject)
{
	CTorchFire*	pInstance = new CTorchFire(_pGraphicDev);

	pInstance->Set_ParentObejct(_pParentObject);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTorchFire::Free(void)
{
	Engine::CGameObject::Free();
}