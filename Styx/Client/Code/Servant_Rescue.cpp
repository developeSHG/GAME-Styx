#include "stdafx.h"
#include "Servant_Rescue.h"

#include "Export_Function.h"

CServant_Rescue::CServant_Rescue(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	m_eOwnerObjectID = Engine::OBJECTID::OBJECT_SERVANT_RESCUE;
	m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_SERVANT_RESCUE;
}

CServant_Rescue::~CServant_Rescue(void)
{

}

HRESULT CServant_Rescue::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_AnimationSet(84);

	return S_OK;
}

_int CServant_Rescue::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	//m_pMeshCom->Set_AnimationSet(0);

	//m_pMeshCom->Set_CurAnimationPos(0.f);

	/*  Coll False Init  */
	m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);

	return UPDATE_OK;
}

_int CServant_Rescue::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CServant_Rescue::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CServant_Rescue::Render_Object(void)
{
	/*  Frustum Culling  */
	if (false == m_pOptimizationCom->Is_InFrustum_ForObject(&m_pTransCom->m_vInfo[Engine::INFO::INFO_POS], 2.f))
		return;

	/*  CollisionMgr AddObject  */
	DWORD dwFlag = COL_FLAG_SPHERE_SOUR;
	Engine::AddObject_CollisionMgr(dwFlag, this);

	PreRender_Object();

	/*  Dynamic Mesh, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect);

		pEffect->SetBool("g_bIamCatsle", true);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(0);

		m_pMeshCom->Play_Animation(m_fTimeDelta);
		m_pMeshCom->Render_Meshe(pEffect);

		pEffect->SetBool("g_bIamCatsle", false);
		pEffect->EndPass();
		pEffect->End();
	}

	m_pSphereColliderCom->Render_Collider(&m_pTransCom->m_matWorld);

	PostRender_Object();
}

void CServant_Rescue::PostRender_Object(void)
{

}

_int CServant_Rescue::CollisionUpdate_Object(const _float& fTimeDelta, 
												const DWORD _dwFlag, 
												const Engine::COL_OBJECTID _eColObjectID, 
												const _float& _fColValue /*= 0.f*/, 
												Engine::CGameObject* _pObject /*= nullptr*/)
{
	if (_dwFlag & COL_FLAG_SPHERE_SOUR)
	{
		if (Engine::COL_OBJECTID::COL_OBJECT_PLAYER == _eColObjectID)
		{
			m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);
		}
	}

	return UPDATE_OK;
}

HRESULT CServant_Rescue::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo();
	NULL_CHECK_RETURN(pLightInfo, E_FAIL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetFloat("g_fAlpha", 1.f);

	D3DMATERIAL9			tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 1.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	Safe_Release(pEffect);

	return S_OK;
}

HRESULT CServant_Rescue::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	
	/*  Mesh  */
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, m_pMeshTag));
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

	/*  Sphere Collider  */
	pComponent = m_pSphereColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, 1.f, 25, 24 );
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SPHERE_COLLIDER, pComponent);

	/*  Optimization  */
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_OPTIMIZATION, pComponent);

	/*  Shader  */
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"NewShader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_NEWSHADER_MESH, pComponent);

	return S_OK;
}

void CServant_Rescue::Update_State(void)
{
	switch (m_eState)
	{
	case SERVANT_STATE::SERVANT_IDLE:
		break;
	}
}

CServant_Rescue* CServant_Rescue::Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag)
{
	CServant_Rescue*	pInstance = new CServant_Rescue(_pGraphicDev);

	pInstance->Set_MeshTag(_pMeshTag);
	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CServant_Rescue::Free(void)
{
	Engine::CGameObject::Free();
}