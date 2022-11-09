#include "stdafx.h"
#include "Lever.h"

#include "Export_Function.h"

CLever::CLever(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	m_eOwnerObjectID = Engine::OBJECTID::OBJECT_LEVER;
	m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_LEVER;
}

CLever::~CLever(void)
{

}

HRESULT CLever::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_AnimationSet(0);

	return S_OK;
}

_int CLever::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	if (TRUE == m_bActive)
	{
		/*if(PLAYER_STATE::PLAYER == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState()
			&& TRUE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_AniRateEnd(0.5))*/
		m_pMeshCom->Set_AnimationSet(1);

		if (1 == m_pMeshCom->Get_AnimationIndex())
		{
			if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
			{
				m_pMeshCom->Set_AnimationSet(0);
				m_bActive = FALSE;
			}
		}
	}

	/*  Coll False Init  */
	m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);

	return UPDATE_OK;
}

_int CLever::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CLever::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CLever::Render_Object(void)
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

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(0);

		m_pMeshCom->Play_Animation(m_fTimeDelta);
		m_pMeshCom->Render_Meshe(pEffect);

		pEffect->EndPass();
		pEffect->End();
	}

	m_pSphereColliderCom->Render_Collider(&m_pTransCom->m_matWorld);

	PostRender_Object();
}

void CLever::PostRender_Object(void)
{

}

_int CLever::CollisionUpdate_Object(const _float& fTimeDelta, 
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

			if (0 == m_pMeshCom->Get_AnimationIndex())
			{
				static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"가동하기");
			}
		}
	}

	return UPDATE_OK;
}

HRESULT CLever::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	pEffect->SetBool("g_bSpecular", true);
	pEffect->SetBool("g_bCatsleIn", true);

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

void CLever::Set_Active()
{
	m_bActive = TRUE; 
	Engine::MyPlaySound(L"PullLever.mp3", Engine::CHANNELID::PULL_LEVER);
}

HRESULT CLever::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	
	/*  Mesh  */
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, m_pMeshTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_MESH, pComponent);

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

	/*  Sphere Collider  */
	//pComponent = m_pSphereColliderCom = Engine::CCubeCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), m_pMeshCom->Get_Stride());
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

CLever* CLever::Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag)
{
	CLever*	pInstance = new CLever(_pGraphicDev);

	pInstance->Set_MeshTag(_pMeshTag);
	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLever::Free(void)
{
	Engine::CGameObject::Free();
}