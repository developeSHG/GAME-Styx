#include "stdafx.h"
#include "PlayerBehavior.h"

#include "Player.h"
#include "EliteBehavior.h"

#include "PlayerKeyInput.inl"
#include "PlayerMoving.inl"
#include "PlayerFightStateKeyInput.inl"
#include "PlayerUpdateSink.inl"
#include "PlayerOnePlayAni.inl"

USING(Engine)

CPlayerBehavior::CPlayerBehavior()
{
}

CPlayerBehavior::CPlayerBehavior(const CPlayerBehavior& rhs)
	: CComponent(rhs)
{

}

CPlayerBehavior::~CPlayerBehavior(void)
{

}

HRESULT CPlayerBehavior::Ready_PlayerBehavior(void)
{
	m_fSpeed = 10.f;
	m_fJumpAccel = JUMP_ACCEL;

	return S_OK;
}

Engine::_int CPlayerBehavior::Update_Component(const Engine::_float & fTimeDelta)
{
	if (true == m_bRunSound)
	{
		m_fRunSound += 1.f * fTimeDelta;

		if (1.f <= m_fRunSound)
		{
			Engine::StopSound(Engine::CHANNELID::RUN);
			m_bRunSound = false;
			m_fRunSound = 0.f;
		}

		if (FALSE == m_pPlayer->Get_FightState())
		{
			if (FALSE == m_pPlayer->Get_CarryState())
			{
				if (PLAYER_STATE::PLAYER_RUN != m_pPlayer->Get_CurState())
				{
					Engine::StopSound(Engine::CHANNELID::RUN);
					m_bRunSound = false;
					m_fRunSound = 0.f;
				}
			}
			else if (TRUE == m_pPlayer->Get_CarryState())
			{
				if (PLAYER_STATE::PLAYER_CARRY_RUN != m_pPlayer->Get_CurState())
				{
					Engine::StopSound(Engine::CHANNELID::RUN);
					m_bRunSound = false;
					m_fRunSound = 0.f;
				}
			}
		}
		else
		{
			if (PLAYER_STATE::PLAYER_FIGHT_IDLE == m_pPlayer->Get_CurState())
			{
				Engine::StopSound(Engine::CHANNELID::RUN);
				m_bRunSound = false;
				m_fRunSound = 0.f;
			}
		}
	}

	if (CAMERA_PURPOSE::PURPOSE_BEHAVIOR != m_pDynamicCamera->Get_CameraPurpose())
	{
		KeyInput(fTimeDelta);
		FightStateKeyInput(fTimeDelta);
	}
	else if (CAMERA_PURPOSE::PURPOSE_BEHAVIOR == m_pDynamicCamera->Get_CameraPurpose())
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);

	/*  Update Jump  */
	Update_Jump(fTimeDelta);

	/*  Update Turn  */
	Update_Turn();

	/*  Update Roll  */
	Update_Roll(fTimeDelta);

	/*  Update Sink  */
	Update_Sink(fTimeDelta);

	/*  Init OnePlayAnimation  */
	Init_OnePlayAnimation();

	/*  CheatKey  */
	On_CheatKey(fTimeDelta);
	
	return UPDATE_OK;
}

void CPlayerBehavior::Set_Player(CPlayer * pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayerMeshCom = SCAST<CDynamicMesh*>(m_pPlayer->Get_Component(COMPONENTTYPE::COM_MESH));
	m_pPlayerTransCom = SCAST<CTransform*>(m_pPlayer->Get_Component(COMPONENTTYPE::COM_TRANSFORM));
}

void CPlayerBehavior::Set_CameraPurpose(const Engine::_bool& _bState)
{
	if (TRUE == _bState)
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE_TURN_R90);
		m_bTurn = TRUE;
		m_dOnePlayAnimationSetEnd = 0.98;
	}
	else
	{
		m_pPlayer->Set_CurState(PLAYER_STATE::PLAYER_IDLE_TURN_L90);
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE_TURN_L90);
		m_pPlayerMeshCom->Set_AnimationSet(216);
		m_bTurn = TRUE;
		m_dOnePlayAnimationSetEnd = 0.98;
	}
}

void CPlayerBehavior::Camera_Setting(const float& fDeltaTime)
{
	/*  RotSpeed  */
	m_fRotAngle = 200.f * fDeltaTime;

	m_matCameraWorld = m_pStaticCamera->GetViewSpaceMatrix();
	D3DXMatrixInverse(&m_matCameraWorld, nullptr, &m_matCameraWorld);

	memcpy(&m_vCameraPos, &m_matCameraWorld.m[3][0], sizeof(Engine::_float) * 3);
	m_vCameraPos = m_pStaticCamera->Get_Eye();
	m_vCameraPos.y = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS].y;

	if (TRUE == m_pPlayer->Get_FirstPersonState())
	{
		m_vCameraPos = m_pStaticCamera->Get_At();
		m_vCameraPos.y = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS].y;
	}

	//m_vecCameraLook은 카메라 룩벡터가 아닌 플레이어가 앞으로 갈 방향
}

void CPlayerBehavior::Change_MovingAnimation(const float& fDeltaTime)
{
	/*  Sit State False  */
	m_pPlayer->Set_SitState(FALSE);

	/*  Fight State  */
	if (TRUE == m_pPlayer->Get_FightState())
	{
		if (false == m_bRunSound)
		{
			Engine::MyPlaySound(L"Run.mp3", Engine::CHANNELID::RUN);
			m_bRunSound = true;
		}

		m_fSpeed = 3.f;
		return;
	}
	
	if (FALSE == m_pPlayer->Get_CarryState())
	{
		/*  Run  */
		if (Engine::Get_DIKeyPressing(DIK_LSHIFT)
		/*	&& 0.f < static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX*/)
			m_fSpeed = 4.5f;
		/*  Walk  */
		else
		{
			if (FALSE == m_pPlayer->Get_SneakState())
				m_fSpeed = 2.f;
			else
				m_fSpeed = 2.f;
		}
	}
	else if (TRUE == m_pPlayer->Get_CarryState())
	{
		/*  Run  */
		if (Engine::Get_DIKeyPressing(DIK_LSHIFT)
			/*&& 0.f < static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX*/)
			m_fSpeed = 3.f;
		/*  Walk  */
		else
			m_fSpeed = 1.5f;
	}

	/*  Blind Render False  */
	if (FALSE == m_pPlayer->Get_FirstPersonState())
		static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(FALSE);

	if (PLAYER_STATE::PLAYER_JUMP == m_pPlayer->Get_CurState()
		|| PLAYER_STATE::PLAYER_DOWN == m_pPlayer->Get_CurState())
		return;

	if (FALSE == m_pPlayer->Get_CarryState())
	{
		/*  Run  */
		if (Engine::Get_DIKeyPressing(DIK_LSHIFT)
			/*&& 0.f < static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX*/)
		{
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_RUN);
			static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX -= 0.1f * fDeltaTime;

			if (false == m_bRunSound)
			{
				Engine::MyPlaySound(L"Run.mp3", Engine::CHANNELID::RUN);
				m_bRunSound = true;
			}
		}
		/*  Walk  */
		else
		{
			if (0.f > static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX)
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
				m_fSpeed = 0.f;
				m_bMoving = false;
				return;
			}

			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_WALK);

			if (TRUE == m_pPlayer->Get_SneakState())
			{
				/*  Blind Render True  */
				static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(TRUE);
				static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Index(3);
			}
		}
	}
	else if (TRUE == m_pPlayer->Get_CarryState())
	{
		/*  Run  */
		if (Engine::Get_DIKeyPressing(DIK_LSHIFT)
		/*	&& 0.f < static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX*/)
		{
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CARRY_RUN);
			static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX -= 0.1f * fDeltaTime;

			if (false == m_bRunSound)
			{
				Engine::MyPlaySound(L"Run.mp3", Engine::CHANNELID::RUN);
				m_bRunSound = true;
			}
		}
		/*  Walk  */
		else
		{
			if (0.f > static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX)
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CARRY_IDLE);
				m_fSpeed = 0.f;
				m_bMoving = false;
				return;
			}

			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CARRY_WALK);
		}
	}
}

void CPlayerBehavior::Update_Jump(const Engine::_float& fDeltaTime)
{
	if (PLAYER_STATE::PLAYER_JUMP == m_pPlayer->Get_CurState())
	{
		m_fJumpAccel += -GRAVITY * fDeltaTime;
		m_vNewPlayerPos.y = m_pPlayerTransCom->m_vInfo[Engine::INFO::INFO_POS].y;
		m_vNewPlayerPos.y += m_fJumpAccel * fDeltaTime;
		m_pPlayer->Set_NewPos(m_vNewPlayerPos);

		if (0.f >= m_fJumpAccel)
		{
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_DOWN);
			m_pPlayer->Reset_FallAccel();
		}
	}
}

void CPlayerBehavior::Update_Turn(void)
{
	if (FALSE == m_bTurn
		|| !(m_pPlayerMeshCom->Is_AnimationSetRateEnd(m_dOnePlayAnimationSetEnd)))
		return;

	Engine::_matrix matRot;

	m_vNewPlayerLook = m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK];

	switch (m_pPlayer->Get_CurState())
	{
	case PLAYER_STATE::PLAYER_IDLE_TURN_L180:
		D3DXMatrixRotationY(&matRot, D3DXToRadian(-180.f));
		D3DXVec3TransformNormal(&m_vNewPlayerLook, &m_vNewPlayerLook, &matRot);
		break;

	case PLAYER_STATE::PLAYER_IDLE_TURN_L90:
		D3DXMatrixRotationY(&matRot, D3DXToRadian(-90.f));
		D3DXVec3TransformNormal(&m_vNewPlayerLook, &m_vNewPlayerLook, &matRot);
		break;

	case PLAYER_STATE::PLAYER_IDLE_TURN_R90:
		D3DXMatrixRotationY(&matRot, D3DXToRadian(90.f));
		D3DXVec3TransformNormal(&m_vNewPlayerLook, &m_vNewPlayerLook, &matRot);
		break;

	case PLAYER_STATE::PLAYER_IDLE_TURN_R180:
		D3DXMatrixRotationY(&matRot, D3DXToRadian(180.f));
		D3DXVec3TransformNormal(&m_vNewPlayerLook, &m_vNewPlayerLook, &matRot);
		break;
	}

	m_bTurn = FALSE;

	m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewPlayerLook;
	D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_vNewPlayerLook);
	D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_vNewPlayerLook, &m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT]);
	D3DXVec3Normalize(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_pPlayerTransCom->m_vInfo[Engine::INFO_UP]);

	m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
	m_pPlayerMeshCom->Set_AnimationSet(217, 0.f, 1.f, 0.f, 0.001f);
}

void CPlayerBehavior::Update_Roll(const Engine::_float& fDeltaTime)
{
	if (PLAYER_STATE::PLAYER_ROLL != m_pPlayer->Get_CurState())
		return;

	Engine::_double dSpeed = 5.6;

	m_vNewPlayerLook = m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK];
	D3DXVec3Normalize(&m_vNewPlayerLook, &m_vNewPlayerLook);

	if(m_pPlayer->Get_AniRateEnd(0.3))
		m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * dSpeed * fDeltaTime;
	if (!m_pPlayer->Get_AniRateEnd(0.3) || m_pPlayer->Get_AniRateEnd(0.8))
		m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * (1.f) * fDeltaTime;

	m_pPlayer->Set_NewPos(m_vNewPlayerPos);
}

void CPlayerBehavior::Update_DropOut(void)
{
	if (nullptr != m_pDropItem)
	{
		Engine::CTransform* pTransform = static_cast<Engine::CTransform*>(m_pDropItem->Get_Component(Engine::COM_TRANSFORM));

		Engine::_vec3 vDir = *D3DXVec3Normalize(&vDir, &m_pPlayerTransCom->m_vInfo[Engine::INFO::INFO_LOOK]);
		Engine::_vec3 vPos = m_vNewPlayerPos + vDir * 0.53f;
		pTransform->Set_Pos(&vPos);
		pTransform->Update_Component();

		static_cast<CFeature*>(m_pDropItem)->Set_Hide(FALSE);
	}
	m_pDropItem = nullptr;
}

CPlayerBehavior* CPlayerBehavior::Create()
{
	CPlayerBehavior* pInstance = new CPlayerBehavior();

	if (FAILED(pInstance->Ready_PlayerBehavior()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent * CPlayerBehavior::Clone(void)
{
	return new CPlayerBehavior(*this);
}

void CPlayerBehavior::Free(void)
{
	
}