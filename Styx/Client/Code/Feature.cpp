#include "stdafx.h"
#include "Feature.h"

#include "Export_Function.h"

#include "CameraBehavior.h"
#include "PlayerBehavior.h"

CFeature::CFeature(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	m_eOwnerObjectID = Engine::OBJECTID::OBEJCT_FEATURE;
}

CFeature::~CFeature(void)
{

}

HRESULT CFeature::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!lstrcmpW(m_pMeshTag, L"Mesh_Ivy_00"))
		m_bAlphaTest = TRUE;

	/*  Item  */
	if (!lstrcmpW(m_pMeshTag, L"Mesh_Map_00"))
	{
		m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_MAP;
		m_bCollActive = TRUE;
		m_bHide = TRUE;
		m_uItemIndex = 4;
	}
	else if (!lstrcmpW(m_pMeshTag, L"Mesh_Dagger_00"))
	{
		m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_CUTLASS;
		m_bCollActive = TRUE;
		m_uItemIndex = 2;
	}
	else if (!lstrcmpW(m_pMeshTag, L"Mesh_Cup_00"))
	{
		m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_CUP;
		m_bCollActive = TRUE;
	}
	else if (!lstrcmpW(m_pMeshTag, L"Mesh_Position_Hp_00"))
	{
		m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_POSITION;
		m_bCollActive = TRUE;
		m_uItemIndex = 0;
	}
	else if (!lstrcmpW(m_pMeshTag, L"Mesh_Position_Mystery_00"))
	{
		m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_MYSTERY;
		m_bCollActive = TRUE;
		m_uItemIndex = 1;
	}
	else if (!lstrcmpW(m_pMeshTag, L"Mesh_Poison_00"))
	{
		m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_POISON;
		m_bCollActive = TRUE;
		m_uItemIndex = 3;
	}
	else if (!lstrcmpW(m_pMeshTag, L"Mesh_Key_00"))
	{
		m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_KEY;
		m_bCollActive = TRUE;
		m_uItemIndex = 5;
	}
	else if (!lstrcmpW(m_pMeshTag, L"Mesh_StyxDagger_000"))
	{
		m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_DAGGER;
		m_bCollActive = TRUE;
	}

	return S_OK;
}
 
_int CFeature::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (FALSE == m_bHide)
		m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	if (nullptr == m_pCameraBehavior && SCENEID::SCENE_STAGE == Engine::Get_CurScene())
		m_pCameraBehavior = static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_CameraBehavior();

	/*  KeyInput_Item  */
	KeyInput_Item();

	/*  Coll False Init  */
	if (TRUE == m_bCollActive)
		m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);

	return UPDATE_OK;
}

_int CFeature::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CFeature::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	/*  Find Mesh_Ivy_00  */
	if (TRUE == m_bAlphaTest)
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CFeature::Render_Object(void)
{
	/*  Culling  */
	{
		if (nullptr != m_pCameraBehavior && 2 == m_pCameraBehavior->Get_Trigger())
		{
			if (FALSE == m_bCatsleIn)
			{
				m_bHide = TRUE;
				return;
			}

			/*  Frustum Culling  */
			if (false == m_pOptimizationCom->Is_InFrustum_ForObject(&m_pTransCom->m_vInfo[Engine::INFO::INFO_POS], 3.f))
				return;
		}
	}

	/*  CollisionMgr AddObject  */
	if (TRUE == m_bCollActive)
	{
		DWORD dwFlag = COL_FLAG_SPHERE_SOUR;
		Engine::AddObject_CollisionMgr(dwFlag, this);
	}

	PreRender_Object();

	/*  Static Mesh, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect);

		_uint iPassCnt = 0;
		
		pEffect->Begin(&iPassCnt, 0);

		/*  Find Mesh_Ivy_00  */
		//if (/*TRUE == m_bAlphaTest || */TRUE == m_bSpecular)
		//	//m_pMeshCom->Render_Meshe(pEffect, 1);
		//	m_pMeshCom->Render_Meshe(pEffect);
		//else
			m_pMeshCom->Render_Meshe(pEffect);

		pEffect->End();
	}

	if(SCENEID::SCENE_STAGE == Engine::Get_CurScene())
		m_pSphereColliderCom->Render_Collider(&m_pTransCom->m_matWorld);

	PostRender_Object();
}

void CFeature::PostRender_Object(void)
{
	/*  Find Mesh_Ivy_00  */
	if (TRUE == m_bAlphaTest)
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_int CFeature::CollisionUpdate_Object(const _float& fTimeDelta, 
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

			if (Engine::COL_OBJECTID::COL_OBJECT_MAP == m_eColObjectID
				|| Engine::COL_OBJECTID::COL_OBJECT_KEY == m_eColObjectID
				|| Engine::COL_OBJECTID::COL_OBJECT_CUTLASS == m_eColObjectID
				|| Engine::COL_OBJECTID::COL_OBJECT_POSITION == m_eColObjectID
				|| Engine::COL_OBJECTID::COL_OBJECT_MYSTERY == m_eColObjectID
				|| Engine::COL_OBJECTID::COL_OBJECT_POISON == m_eColObjectID)
				static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"줍기");
			else if (Engine::COL_OBJECTID::COL_OBJECT_DAGGER == m_eColObjectID)
				static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"장착");
			else if (Engine::COL_OBJECTID::COL_OBJECT_CUP == m_eColObjectID)
			{
				if (0 < static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_CurItemNum())
				{
					if (3 == static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemSelect())
					{
						static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"약 타기");
					}
				}
			}
		}
	}

	return UPDATE_OK;
}

HRESULT CFeature::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	if (SCENEID::SCENE_STAGE == Engine::Get_CurScene())
		pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	else if (SCENEID::SCENE_LOGO == Engine::Get_CurScene())
		pEffect->SetVector("g_vLightAmbient", &_vec4(1.f, 1.f, 1.f, 1.f));

	pEffect->SetFloat("g_fAlpha", 1.f);

	if (!(lstrcmp(m_pMeshTag, L"Mesh_Elite_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Lamp_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Jar_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Jar_Amber_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Jar_Amber_01"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Chandelier_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Candle_Group_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Candle_Large_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Candle_Medium_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Statue_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Statue_01"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_StatueBlack_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Lantern_Attach_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Lantern_OFF_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Lantern_Wall_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Luster_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Grid_01"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Grid_02"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Grid_03"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Grid_04"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Position_Hp_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Position_Mystery_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Poison_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_Key_00"))
		|| !(lstrcmp(m_pMeshTag, L"Mesh_StyxDagger_000")))
	//if ((lstrcmp(m_pMeshTag, L"Mesh_StatueBlack_00")))
	{
		pEffect->SetBool("g_bSpecular", true);
		m_bSpecular = TRUE;
	}	
	else
		pEffect->SetBool("g_bSpecular", true);

	if(TRUE == m_bCatsleIn)
		pEffect->SetBool("g_bCatsleIn", true);
	else
		pEffect->SetBool("g_bCatsleIn", false);

	//D3DMATERIAL9			tMtrlInfo;
	//ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	//tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	//tMtrlInfo.Power = 1.f;

	//pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	//pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	Safe_Release(pEffect);

	return S_OK;
}

HRESULT CFeature::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Mesh  */
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, m_pMeshTag));
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

	/*  Collider  */
	pComponent = m_pSphereColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, 0.5f, 25, 24 );
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

void CFeature::KeyInput_Item(void)
{
	if (TRUE == m_bHide)
		return;

	if (Engine::COLLTYPE::COL_TRUE == m_pSphereColliderCom->Get_IsCollide())
	{
		if (Engine::COL_OBJECTID::COL_OBJECT_MAP == m_eColObjectID
			|| Engine::COL_OBJECTID::COL_OBJECT_DAGGER == m_eColObjectID
			|| Engine::COL_OBJECTID::COL_OBJECT_KEY == m_eColObjectID
			|| Engine::COL_OBJECTID::COL_OBJECT_CUTLASS == m_eColObjectID
			|| Engine::COL_OBJECTID::COL_OBJECT_POSITION == m_eColObjectID
			|| Engine::COL_OBJECTID::COL_OBJECT_MYSTERY == m_eColObjectID
			|| Engine::COL_OBJECTID::COL_OBJECT_POISON == m_eColObjectID)
		{
			if (Engine::Get_DIKeyDown(DIK_E))
			{
				m_bDropIn = TRUE;
			}
		}

		/*  Player DropIn  */
		if (TRUE == m_bDropIn 
			&& PLAYER_STATE::PLAYER_DROPIN == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState())
		{
			if (static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_AniRateEnd(0.3))
			{
				if (Engine::COL_OBJECTID::COL_OBJECT_DAGGER == m_eColObjectID)
				{
					m_bHide = TRUE;
					static_cast<CDagger*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_DAGGER))->Set_Hide(FALSE);
					static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(14);
					Engine::MyPlaySound(L"AhThereismydagger.mp3", Engine::CHANNELID::PLAYER);
				}
				else
				{
					if (FALSE == m_bOneLowerText)
					{
						if (Engine::COL_OBJECTID::COL_OBJECT_POSITION == m_eColObjectID)
							static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(8);
						else if (Engine::COL_OBJECTID::COL_OBJECT_MYSTERY == m_eColObjectID)
							static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(9);
						else if (Engine::COL_OBJECTID::COL_OBJECT_CUTLASS == m_eColObjectID)
							static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(10);
						else if (Engine::COL_OBJECTID::COL_OBJECT_POISON == m_eColObjectID)
							static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(11);
						else if (Engine::COL_OBJECTID::COL_OBJECT_MAP == m_eColObjectID)
							static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(12);
						else if (Engine::COL_OBJECTID::COL_OBJECT_KEY == m_eColObjectID)
						{
							static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(13);

							static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CheerState(TRUE);
							static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_NextState(PLAYER_STATE::PLAYER_CHEER);
							static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_PlayerBehavior()->Set_OnePlayAni(0.98);
						}
					}
					static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Set_AddItem(m_uItemIndex);
					m_bHide = TRUE;
					m_bDropIn = FALSE;
					m_bOneLowerText = TRUE;
				}
			}
		}
	}

}

CFeature* CFeature::Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag)
{
	CFeature*	pInstance = new CFeature(_pGraphicDev);

	pInstance->Set_MeshTag(_pMeshTag);
	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFeature::Free(void)
{
	Engine::CGameObject::Free();
}