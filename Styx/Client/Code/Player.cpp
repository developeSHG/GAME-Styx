#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"

#include "EliteBehavior.h"
#include "PlayerBehavior.h"
#include "PlayerState.inl"
#include "PlayerRender.inl"

#include "Spark.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	m_eOwnerObjectID = Engine::OBJECTID::OBJECT_PLAYER;
	m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_PLAYER;
}

CPlayer::~CPlayer(void)
{

}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

HRESULT CPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pNaviMeshCom->Set_Index(0);
	
	m_pMeshCom->Set_AnimationSet(185);
	m_pMeshCom_Elite->Set_AnimationSet(101);

//	m_pTransCom->Move_Pos(&_vec3(TERRAIN_NUM_X >> 1, 2.f, TERRAIN_NUM_Z / 2.9f - 8.f));
	m_pTransCom->Move_Pos(&_vec3((TERRAIN_NUM_X >> 1) - 0.5f, 4.1f, TERRAIN_NUM_Z / 2.9f - 4.f));
	m_vNewPos = m_pTransCom->Get_InfoPos();
	m_pTransCom->Set_Scale(0.01f, 0.01f, 0.01f);

	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	/*  Light Point Init  */
	{
		ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
		tLightInfo.Type = D3DLIGHT_POINT;

		tLightInfo.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
		tLightInfo.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
		tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
			
		tLightInfo.Range = 9.f;												
		//tLightInfo.Position = static_cast<Engine::CTransform*>(m_pTorchFire->Get_Component(Engine::COM_TRANSFORM))->Get_InfoPos();
		FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1), E_FAIL);
	}

	return S_OK;
}

_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Get_LightInfo(1)->Position = m_vNewPos;
	Engine::Get_LightInfo(1)->Position.y += 1.f;

	Engine::Set_LightActive(0);
	Engine::Set_LightActive(1);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);
	    
	/*  Update Graviry  */
	Update_Gravity(fTimeDelta);	
	
	DWORD dwJumpDownFlag = 0x00000001;
	if(PLAYER_JUMP == m_eCurState)
		dwJumpDownFlag = 0x00000002;
	if(PLAYER_DOWN == m_eCurState)
		dwJumpDownFlag = 0x00000004;

	/*  Compare, IsOnLand Option  */
	if (PLAYER_STATE::PLAYER_TRUNK_IN == m_eCurState
		|| PLAYER_STATE::PLAYER_CUPBOARD_IN == m_eCurState
		|| PLAYER_STATE::PLAYER_TRUNK_OUT == m_eCurState
		|| PLAYER_STATE::PLAYER_CUPBOARD_OUT == m_eCurState
		|| PLAYER_STATE::PLAYER_TRUNK_CATCH == m_eCurState
		|| PLAYER_STATE::PLAYER_CUPBOARD_CATCH == m_eCurState
		|| PLAYER_STATE::PLAYER_CATCH_HURT == m_eCurState)
	{
		m_bCompare = TRUE;
		m_bIsOnLand = TRUE;
		m_bRoadCliff = TRUE;
	}
	else
	{
		/*  MoveOn NaviMesh Check  */
		m_bIsOnLand = m_pNaviMeshCom->MoveOn_NaviMeshPlayer(&m_pTransCom->m_vInfo[Engine::INFO_POS],
			&m_vNewPos,
			m_bCompare,
			dwJumpDownFlag,
			m_bRoadCliff);
	}
	
	/*  Update IsOnLand  */
	{
		/*  Compare Move  */
		if (TRUE == m_bCompare)
			Update_IsOnLand(fTimeDelta);
		/*  Compare Stop / Sliding  */
		else if(FALSE == m_bCompare)
		{
			/*  Compare Stop and State Is Jump or Down  */
			if (TRUE == m_bIsOnLand)
			{
				if (PLAYER_STATE::PLAYER_JUMP == m_eCurState
					|| PLAYER_STATE::PLAYER_DOWN == m_eCurState)
				{
					if (FALSE == m_pPlayerBehavior->Get_Moving())
						m_eNextState = PLAYER_STATE::PLAYER_IDLE;
					else if (TRUE == m_pPlayerBehavior->Get_Moving())
					{
						/*  Run  */
						if (Engine::Get_DIKeyPressing(DIK_LSHIFT))
							m_eNextState = PLAYER_STATE::PLAYER_RUN;
						else
							m_eNextState = PLAYER_STATE::PLAYER_WALK;
					}
					m_pPlayerBehavior->Reset_JumpAccel();
				}
			}
		}
	}

	/*  CollisionMgr AddObject  */
	DWORD dwFlag = COL_FLAG_AABB_DEST | COL_FLAG_SPHERE_DEST;
	Engine::AddObject_CollisionMgr(dwFlag, this);

	/*  Coll False Init  */
	{
		m_pAABBColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);
		m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);
		m_bAABBCol = FALSE;
		m_eColTo = PLAYER_COL_END;
		//m_pCollToElite = nullptr;
	}

	return UPDATE_OK;
}

_int CPlayer::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	/*  Update IsOnLand  */
	{
		/*  Compare Move  */
		if (TRUE == m_bCompare)
		{
			if (TRUE == m_bIsOnLand)
			{
				if (FALSE == m_bAABBCol)
				{
					m_pTransCom->Set_Pos(&m_vNewPos);
					m_pPlayerBehavior->Set_NewPlayerPos(m_pTransCom->m_vInfo[Engine::INFO::INFO_POS]);
				}
				else if(TRUE == m_bAABBCol)
				{
					m_vNewPos = m_pTransCom->m_vInfo[Engine::INFO::INFO_POS];
					m_pPlayerBehavior->Set_NewPlayerPos(m_pTransCom->m_vInfo[Engine::INFO::INFO_POS]);
				}
			}
			else if (FALSE == m_bIsOnLand)
			{
				/*  Is On Cliff Down  */
			/*	if (FALSE == m_bRoadCliff
					&& PLAYER_STATE::PLAYER_DOWN == m_eCurState)
				{
					m_pTransCom->Set_Pos(&m_vNewPos);
					m_bRoadCliff = TRUE;
				}*/

				if (PLAYER_STATE::PLAYER_JUMP == m_eCurState
					|| PLAYER_STATE::PLAYER_DOWN == m_eCurState)
				{
					if (FALSE == m_bAABBCol)
						m_pTransCom->Set_Pos(&m_vNewPos);
					else if (TRUE == m_bAABBCol)
						m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].y = m_vNewPos.y;
				}
			}
		}
		/*  Compare Stop / Sliding  */
		else if (FALSE == m_bCompare)
		{
			if (FALSE == m_bAABBCol)
			{
				m_pTransCom->Set_Pos(&m_vNewPos);
				m_pPlayerBehavior->Set_NewPlayerPos(m_pTransCom->m_vInfo[Engine::INFO::INFO_POS]);
			}
			else if (TRUE == m_bAABBCol)
			{
				m_vNewPos = m_pTransCom->m_vInfo[Engine::INFO::INFO_POS];
				m_pPlayerBehavior->Set_NewPlayerPos(m_pTransCom->m_vInfo[Engine::INFO::INFO_POS]);
			}
		}
	}

	//cout << "x : " << m_vNewPos.x << endl;
	//cout << "z : " << m_vNewPos.z << endl;
	//cout << "y : " << m_vNewPos.y << endl;

	/*  EliteState True  */
	if (TRUE == m_bEliteState
		&& FALSE == m_bTranslucent
		&& Engine::COL_FALSE == m_pSphereColliderCom->Get_IsCollide())
	{
		static_cast<CButton*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_F, L"해제하기");
	}

	/*  Translucent  */
	if (FALSE == m_bEliteState
		&& TRUE == m_bTranslucent)
	{
		_vec3 vDist = m_pTransCom_Elite->Get_InfoPos() - m_pTransCom->Get_InfoPos();
		vDist.y = 0.f;

		if (1.f > D3DXVec3Length(&vDist))
		{
			static_cast<CButton*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_F, L"들어가기");

			if (Engine::Get_DIKeyDown(DIK_F))
			{
				m_bEliteState = TRUE;
				m_bTranslucent = FALSE;
				m_pTransCom->Set_Pos(&m_pTransCom_Elite->Get_InfoPos());
				m_eNextState = PLAYER_STATE::PLAYER_IDLE;
				m_pMeshCom_Elite->Set_AnimationSet(106);
			}
		}
	}

	/*  StateCheck  */
	Update_State();

	/*  Static Transform Update  */
	StaticTransform_Update();

	/*  Spark Render  */
	if (m_pSpark && TRUE == m_bSparkRender)
		m_pSpark->Update_Object(m_fTimeDelta);
	else if (m_pSpark && FALSE == m_bSparkRender)
		m_pSpark->Set_InitScale();

	return UPDATE_OK;
}

_int CPlayer::CollisionUpdate_Object(const _float& fTimeDelta, 
										const DWORD _dwFlag,
										const Engine::COL_OBJECTID _eColObjectID,
										const _float& _fColValue /*= 0.f*/,
										Engine::CGameObject* _pObject /*= nullptr*/)
{
	if (_dwFlag & COL_FLAG_AABB_DEST)
	{
		m_pAABBColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);

		if (Engine::COL_OBJECTID::COL_OBJECT_DOOR == _eColObjectID)
		{
			if (5 == static_cast<Engine::CDynamicMesh*>(_pObject->Get_Component(Engine::COM_MESH))->Get_AnimationIndex())
				m_bAABBCol = TRUE;

			if (1 == static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemNum(5)
				&& PLAYER_STATE::PLAYER_DOOR_OPEN == m_eCurState
				&& TRUE == m_pMeshCom->Is_AnimationSetEnd(0.1f))
			{
				static_cast<Engine::CDynamicMesh*>(_pObject->Get_Component(Engine::COM_MESH))->Set_AnimationSet(0);
			}
		}
		else if (Engine::COL_OBJECTID::COL_OBJECT_GRID == _eColObjectID)
		{
			if(2 == static_cast<Engine::CDynamicMesh*>(_pObject->Get_Component(Engine::COM_MESH))->Get_AnimationIndex())
				m_bAABBCol = TRUE;
		}
	}
	else if (_dwFlag & COL_FLAG_SPHERE_DEST)
	{
		m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);

		if (Engine::COL_OBJECTID::COL_OBJECT_ELITE == _eColObjectID)
		{
			if(FALSE == m_bCarryState)
				m_pCollToElite = static_cast<CElite*>(_pObject);
		}
	}
	if(Engine::COL_OBJECTID::COL_OBJECT_DOOR == _eColObjectID)
		m_eColTo |= PLAYER_COL_DOOR;
	if (Engine::COL_OBJECTID::COL_OBJECT_TRUNK == _eColObjectID)
	{
		m_eColTo |= PLAYER_COL_TRUNK;
		m_pHideObject = _pObject;
	}
	if (Engine::COL_OBJECTID::COL_OBJECT_CUPBOARD == _eColObjectID)
	{
		m_eColTo |= PLAYER_COL_CUPBOARD;
		m_pHideObject = _pObject;
	}
	if (Engine::COL_OBJECTID::COL_OBJECT_TORCH == _eColObjectID)
		m_eColTo |= PLAYER_COL_TORCH;
	if (Engine::COL_OBJECTID::COL_OBJECT_LEVER == _eColObjectID)
		m_eColTo |= PLAYER_COL_LEVER;
	if (Engine::COL_OBJECTID::COL_OBJECT_GRID == _eColObjectID)
		m_eColTo |= PLAYER_COL_GRID;
	if (Engine::COL_OBJECTID::COL_OBJECT_ELITE == _eColObjectID)
		m_eColTo |= PLAYER_COL_ELITE;
	if (Engine::COL_OBJECTID::COL_OBJECT_SERVANT_QUEST == _eColObjectID)
		m_eColTo |= PLAYER_COL_SERVANT_QUEST;
	if (Engine::COL_OBJECTID::COL_OBJECT_SERVANT_RESCUE == _eColObjectID)
		m_eColTo |= PLAYER_COL_SERVANT_RESCUE;
	if (Engine::COL_OBJECTID::COL_OBJECT_MAP == _eColObjectID)
		m_eColTo |= PLAYER_COL_MAP;
	if (Engine::COL_OBJECTID::COL_OBJECT_DAGGER == _eColObjectID)
		m_eColTo |= PLAYER_COL_DAGGER;
	if (Engine::COL_OBJECTID::COL_OBJECT_KEY == _eColObjectID)
		m_eColTo |= PLAYER_COL_KEY;
	if (Engine::COL_OBJECTID::COL_OBJECT_CUTLASS == _eColObjectID)
		m_eColTo |= PLAYER_COL_CUTLASS;
	if (Engine::COL_OBJECTID::COL_OBJECT_POSITION == _eColObjectID)
		m_eColTo |= PLAYER_COL_POSITION;
	if (Engine::COL_OBJECTID::COL_OBJECT_MYSTERY == _eColObjectID)
		m_eColTo |= PLAYER_COL_MYSTERY;
	if (Engine::COL_OBJECTID::COL_OBJECT_POISON == _eColObjectID)
		m_eColTo |= PLAYER_COL_POISON;
	if (Engine::COL_OBJECTID::COL_OBJECT_CUP == _eColObjectID)
		m_eColTo |= PLAYER_COL_CUP;

	return UPDATE_OK;
}

void CPlayer::Set_InitPlayerLook(const _vec3* _vPos)
{
	m_pTransCom->m_vInfo[Engine::INFO_LOOK] = *_vPos;
	D3DXVec3Normalize(&m_pTransCom->m_vInfo[Engine::INFO_LOOK], &m_pTransCom->m_vInfo[Engine::INFO_LOOK]);
	m_pPlayerBehavior->Set_NewPlayerLook(m_pTransCom->m_vInfo[Engine::INFO_LOOK]);
	D3DXVec3Cross(&m_pTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_pTransCom->m_vInfo[Engine::INFO_LOOK]);
	D3DXVec3Cross(&m_pTransCom->m_vInfo[Engine::INFO_UP], &m_pTransCom->m_vInfo[Engine::INFO_LOOK], &m_pTransCom->m_vInfo[Engine::INFO_RIGHT]);
	D3DXVec3Normalize(&m_pTransCom->m_vInfo[Engine::INFO_UP], &m_pTransCom->m_vInfo[Engine::INFO_UP]);

	StaticTransform_Update();
}

HRESULT CPlayer::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Mesh  */
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Styx"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_MESH, pComponent);

	pComponent = m_pMeshCom_Elite = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Monster_Elite"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_MESH_ELITE, pComponent);

	/*  Transform  */
	/*  Static Choice!!  */
	pComponent = m_pTransCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pTransCom_Elite = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM_ELITE, pComponent);

	/*  Renderer  */
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_RENDERER, pComponent);

	/*  AABB Collider  */
	pComponent = m_pAABBColliderCom = Engine::CCubeCollider::Create(m_pGraphicDev, _vec3(-0.3f, 0.f, -0.3f), _vec3(0.3f, 1.15f, 0.3f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_AABB_COLLIDER, pComponent);

	/*  Sphere Collider  */
	//pComponent = m_pColliderCom = Engine::CCubeCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), m_pMeshCom->Get_Stride());
	pComponent = m_pSphereColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, 0.6f, 25, 24);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SPHERE_COLLIDER, pComponent);

	/*  NaviMesh  */
	pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone(RESOURCEID::RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_NAVIMESH, pComponent);

	/*  Shader  */
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"NewShader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_NEWSHADER_MESH, pComponent);

	/*  Player Component  */
	{
		/*  PlayerBehavior  */
		pComponent = m_pPlayerBehavior = CPlayerBehavior::Create();
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::COMPONENTID::ID_DYNAMIC].emplace(Engine::COMPONENTTYPE::COM_PLAYERBEHAVIOR, pComponent);
		m_pPlayerBehavior->Set_Player(this);
	}

	/*  Spark  */
	m_pSpark = CSpark::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pSpark, E_FAIL);
	m_pSpark->Set_Player(this);

	/*  PlayerSword  */
	m_pPlayerSword = CPlayerSword::Create(m_pGraphicDev, this);
	NULL_CHECK_RETURN(m_pPlayerSword, E_FAIL);

	return S_OK;
}

void CPlayer::StaticTransform_Update(void)
{
	/*  Scale  */
	for (Engine::_uint i = 0; i < Engine::INFO::INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_pTransCom->m_vInfo[i], &m_pTransCom->m_vInfo[i]);

		if(TRUE == m_bEliteState)
			m_pTransCom->m_vScale = _vec3(0.01f, 0.007f, 0.01f);
		else
			m_pTransCom->m_vScale = _vec3(0.01f, 0.01f, 0.01f);

		m_pTransCom->m_vInfo[i] *= *(((Engine::_float*)&m_pTransCom->m_vScale) + i);
	}

	/*  World Set  */
	for (Engine::_uint i = 0; i < Engine::INFO::INFO_END; ++i)
		memcpy(&m_pTransCom->m_matWorld.m[i][0], &m_pTransCom->m_vInfo[i], sizeof(Engine::_vec3));

	if (TRUE == m_bEliteState
		&& FALSE == m_bTranslucent)
	{
		memcpy(&m_pTransCom_Elite->m_matWorld, &m_pTransCom->m_matWorld, sizeof(Engine::_matrix));
		memcpy(&m_pTransCom_Elite->m_vInfo[Engine::INFO_POS], &m_pTransCom->m_vInfo[Engine::INFO_POS], sizeof(Engine::_vec3));
	}
}

void CPlayer::Update_Gravity(const float & fTimeDelta)
{
	if (PLAYER_STATE::PLAYER_JUMP != m_eCurState
		&& PLAYER_STATE::PLAYER_TRUNK_IN != m_eCurState && PLAYER_STATE::PLAYER_TRUNK_OUT != m_eCurState
		&& PLAYER_STATE::PLAYER_CUPBOARD_IN != m_eCurState && PLAYER_STATE::PLAYER_CUPBOARD_OUT != m_eCurState
		&& PLAYER_STATE::PLAYER_TRUNK_CATCH != m_eCurState && PLAYER_STATE::PLAYER_CUPBOARD_CATCH != m_eCurState
		&& PLAYER_STATE::PLAYER_CATCH_HURT != m_eCurState)
	{
		//m_fFallAccel += 0.15f * fTimeDelta;
		//m_vNewPos.y = m_vNewPos.y - GRAVITY * fTimeDelta * m_fFallAccel;

		m_fFallAccel += -GRAVITY * fTimeDelta;
		m_vNewPos.y += m_fFallAccel * fTimeDelta;

	}
}

void CPlayer::Update_IsOnLand(const _float & fTimeDelta)
{
	if (TRUE == m_bIsOnLand)
	{
		if (PLAYER_STATE::PLAYER_JUMP == m_eCurState
			|| PLAYER_STATE::PLAYER_DOWN == m_eCurState)
		{
			if (FALSE == m_pPlayerBehavior->Get_Moving())
				m_eNextState = PLAYER_STATE::PLAYER_IDLE;
			else if (TRUE == m_pPlayerBehavior->Get_Moving())
			{
				/*  Run  */
				if (Engine::Get_DIKeyPressing(DIK_LSHIFT))
					m_eNextState = PLAYER_STATE::PLAYER_RUN;
				else
					m_eNextState = PLAYER_STATE::PLAYER_WALK;
			}
			m_pPlayerBehavior->Reset_JumpAccel();
		}
	}
	else if (FALSE == m_bIsOnLand)
	{
		/*  Is On Cliff Down  */
		if (FALSE == m_bRoadCliff
			&& PLAYER_STATE::PLAYER_DOWN != m_eCurState)
		{
			m_eNextState = PLAYER_STATE::PLAYER_DOWN;
			m_pMeshCom->Set_AnimationSet(189, 1.f, 0.75f);
			Reset_FallAccel();
			m_bRoadCliff = TRUE;
		}
	}
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free(void)
{
	if (m_pSpark)
		Engine::Safe_Release(m_pSpark);

	if (m_pPlayerSword)
		Engine::Safe_Release(m_pPlayerSword);

	Engine::CGameObject::Free();
}