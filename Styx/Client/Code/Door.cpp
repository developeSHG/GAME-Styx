#include "stdafx.h"
#include "Door.h"

#include "Export_Function.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	m_eOwnerObjectID = Engine::OBJECTID::OBJECT_DOOR;
	m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_DOOR;
}

CDoor::~CDoor(void)
{

}

HRESULT CDoor::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_AnimationSet(5);

	if (5 != m_pMeshCom->Get_AnimationIndex())
		m_bFindKey = FALSE;

	return S_OK;
}

_int CDoor::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	//m_pMeshCom->Set_AnimationSet(0);

	//m_pMeshCom->Set_CurAnimationPos(0.f);

	/*  Unlock  */
	if (0 == m_pMeshCom->Get_AnimationIndex())
	{
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_pMeshCom->Set_AnimationSet(1, 0.f, 1.f, 0.f, 0.01f);
			Engine::MyPlaySound(L"DoorOpen.mp3", Engine::CHANNELID::DOOR);
		}
	}
	/*  Opening R  */
	if (1 == m_pMeshCom->Get_AnimationIndex())
	{
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_pMeshCom->Set_AnimationSet(3, 0.f, 1.f, 0.f, 0.01f);
		}
	}

	/*  Coll False Init  */
	m_pAABBColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);

	return UPDATE_OK;
}

_int CDoor::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	/*  FindKey Font  */
	if (TRUE == m_bFindKey
		&& Engine::COL_FALSE == m_pAABBColliderCom->Get_IsCollide())
	{
		if (5 == m_pMeshCom->Get_AnimationIndex())
			static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_F, L"찾기");
	}

	return UPDATE_OK;
}

void CDoor::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CDoor::Render_Object(void)
{
	/*  Frustum Culling  */
	if (false == m_pOptimizationCom->Is_InFrustum_ForObject(&m_pTransCom->m_vInfo[Engine::INFO::INFO_POS], 2.f))
	{
		m_bFindKey = FALSE;
		return;
	}

	/*  CollisionMgr AddObject  */
	DWORD dwFlag = COL_FLAG_AABB_SOUR;
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

	m_pAABBColliderCom->Render_Collider(&m_pTransCom->m_matNRotWorld);

	PostRender_Object();
}

void CDoor::PostRender_Object(void)
{

}

_int CDoor::CollisionUpdate_Object(const _float& fTimeDelta,
									const DWORD _dwFlag,
									const Engine::COL_OBJECTID _eColObjectID,
									const _float& _fColValue /*= 0.f*/,
									Engine::CGameObject* _pObject /*= nullptr*/)
{
	if (_dwFlag & COL_FLAG_AABB_SOUR)
	{
		if (Engine::COL_OBJECTID::COL_OBJECT_PLAYER == _eColObjectID)
		{
			m_pAABBColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);

			/*  Lock  */
			if (5 == m_pMeshCom->Get_AnimationIndex())
			{
				if (CAMERA_PURPOSE::PURPOSE_DOOR != static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_CameraPurpose())
				{
					_bool _bCheck = FALSE;
					if (PLAYER_STATE::PLAYER_DOOR_OPEN == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState())
					{
						if (0 == static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemNum(5))
						{
							m_bFindKey = TRUE;
							_bCheck = TRUE;
						}
					}
					if (FALSE == _bCheck)
					{
						static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"잠금 해제");
						static_cast<CButton*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_C, L"문틈 보기");
					}
				}
				else if (CAMERA_PURPOSE::PURPOSE_DOOR == static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_CameraPurpose())
				{
					static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_C, L"되돌아가기");
				}
			}			
		}
	}

	return UPDATE_OK;
}

HRESULT CDoor::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	pEffect->SetBool("g_bSpecular", false);
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

HRESULT CDoor::Add_Component()
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

	/*  AABB Collider  */
	//pComponent = m_pSphereColliderCom = Engine::CCubeCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), m_pMeshCom->Get_Stride());
	pComponent = m_pAABBColliderCom = Engine::CCubeCollider::Create(m_pGraphicDev, _vec3(-1.3f, 0.f, -0.35f), _vec3(1.3f, 2.f, 0.35f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_AABB_COLLIDER, pComponent);

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

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag)
{
	CDoor*	pInstance = new CDoor(_pGraphicDev);

	pInstance->Set_MeshTag(_pMeshTag);
	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDoor::Free(void)
{
	Engine::CGameObject::Free();
}