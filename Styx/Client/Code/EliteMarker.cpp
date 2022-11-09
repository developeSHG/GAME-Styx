#include "stdafx.h"
#include "EliteMarker.h"

CEliteMarker::CEliteMarker(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{

}

CEliteMarker::~CEliteMarker(void)
{

}

HRESULT CEliteMarker::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CEliteMarker::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pTransCom->m_vInfo[Engine::INFO_POS] = m_pEliteTransCom->Get_InfoPos();
	m_pTransCom->m_vInfo[Engine::INFO_POS].y += 1.95f;

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3 vCamera;
	memcpy(&vCamera, matView.m[3], sizeof(_vec3));

	_float fDist = fabsf(m_pTransCom->m_vInfo[Engine::INFO_POS].z - vCamera.z);

	_float fSize = 0.12f;
	if (3.f < fDist)
		fSize = 0.15f;
	if (4.5f < fDist)
		fSize = 0.2f;
	if (6.f < fDist)
		fSize = 0.25f;
	if (10.f < fDist)
		fSize = 0.3f;
	if (17.f < fDist)
		fSize = 0.35f;
	if (25.f < fDist)
		fSize = 0.4f;
	if (30.f < fDist)
		fSize = 0.45f;
	if (35.f < fDist)
		fSize = 0.5f;
	if (40.f < fDist)
		fSize = 0.6f;
	if (45.f < fDist)
		fSize = 0.65f;
	if (50.f < fDist)
		fSize = 0.7f;
	if (55.f < fDist)
		fSize = 0.5f;
	if (60.f < fDist)
		fSize = 0.8f;

	//fSize += 0.045f;

	m_fSize += (fSize - m_fSize) * 2.f * fTimeDelta;

	if (fabsf(m_fSize - fSize) < 0.001f)
		m_fSize = fSize;
		
	m_pTransCom->m_vScale = _vec3(m_fSize, m_fSize, m_fSize);
	
	m_pTransCom->Update_Component();

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	return UPDATE_OK;
}

_int CEliteMarker::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CEliteMarker::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CEliteMarker::Render_Object(void)
{
	PreRender_Object();

	/*  RcTex Buffer, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect, 0);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(1);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();	
	}

	PostRender_Object();
}

void CEliteMarker::PostRender_Object(void)
{

}

HRESULT CEliteMarker::SetUp_ConstantTable(LPD3DXEFFECT pEffect, _ushort _uIndex)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj, matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixIdentity(&matBill);
	/*  Y  */
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	/*  X  */
	matBill._22 = matView._22;
	matBill._23 = matView._23;
	matBill._32 = matView._32;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	matWorld = (matBill * matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Render_Texture(pEffect, "g_BaseTexture", _uIndex);

	Safe_Release(pEffect);

	return S_OK;
}

void CEliteMarker::Set_Elite(CElite* pElite)
{
	m_pElite = pElite;
	m_pEliteMeshCom = SCAST<Engine::CDynamicMesh*>(m_pElite->Get_Component(Engine::COMPONENTTYPE::COM_MESH));
	m_pEliteTransCom = SCAST<Engine::CTransform*>(m_pElite->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));
}

HRESULT CEliteMarker::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_EliteMarker"));
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

CEliteMarker* CEliteMarker::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CEliteMarker*	pInstance = new CEliteMarker(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CEliteMarker::Free(void)
{
	Engine::CGameObject::Free();
}