void CEliteBehavior::Update_FightState(const Engine::_float& fTimeDelta)
{
	if (TRUE == m_bOnePlayAnimation
		|| TRUE == m_pElite->Get_OnMoveWay()
		|| TRUE == m_pElite->Get_CarryState()
		|| FALSE == m_pElite->Get_Live()
		|| FALSE == m_pElite->Get_FightState()
		|| FALSE == m_pElite->Get_SwordActive())
		return;

	if (TRUE == Update_Story(fTimeDelta))
	{
		m_pElite->Set_NewPos(m_vNewElitePos);
		return;
	}

	if (TRUE == m_pElite->Get_FightState())
	{
		/*  Hide  */
		if (PLAYER_TRUNK_IN == m_pPlayer->Get_CurState()
			|| PLAYER_CUPBOARD_IN == m_pPlayer->Get_CurState())
		{
			Update_HideBehavior(fTimeDelta);
		}
		/*  No Hide  */
		else
		{
			Update_NoHideBehavior(fTimeDelta);
		}
	}

	m_pElite->Set_NewPos(m_vNewElitePos);
}

void CEliteBehavior::Update_HideBehavior(const Engine::_float& fTimeDelta)
{
	Engine::CTransform* pHideTransform = static_cast<Engine::CTransform*>(m_pPlayer->Get_HideObject()->Get_Component(Engine::COM_TRANSFORM));

	_vec3 vHideObjectPos = pHideTransform->Get_InfoPos();
	D3DXVec3Normalize(&pHideTransform->m_vInfo[Engine::INFO_LOOK], &pHideTransform->m_vInfo[Engine::INFO_LOOK]);
	vHideObjectPos += pHideTransform->m_vInfo[Engine::INFO_LOOK] * 1.5f;

	/*  Look  */
	m_vNewEliteLook = vHideObjectPos - m_vNewElitePos;
	m_vNewEliteLook.y = 0.f;
	D3DXVec3Normalize(&m_vNewEliteLook, &m_vNewEliteLook);

	if (4.f < D3DXVec3Length(&(vHideObjectPos - m_vNewElitePos)))
	{
		m_fSpeed = 2.5f;
		m_vNewElitePos = m_pEliteTransCom->m_vInfo[Engine::INFO_POS] + m_vNewEliteLook * m_fSpeed * fTimeDelta;
		m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
	}
	else if (0.05f < D3DXVec3Length(&(vHideObjectPos - m_vNewElitePos)))
	{
		m_fSpeed = 0.85f;
		m_vNewElitePos = m_pEliteTransCom->m_vInfo[Engine::INFO_POS] + m_vNewEliteLook * m_fSpeed * fTimeDelta;
		m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
	}
	else
	{
		m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);

		if (157 == m_pEliteMeshCom->Get_AnimationIndex())
		{
			if (m_pElite->Get_AniRateEnd(0.5))
			{
				DWORD uColTo = m_pPlayer->Get_ColTo();

				if (PLAYER_COL_TRUNK & uColTo)
				{
					m_pElite->Set_CurState(ELITE_STATE::ELITE_TRUNK_IN);
					m_pEliteMeshCom->Set_AnimationSet(164);
					Set_OnePlayAni(0.85);	

					static_cast<CTrunk*>(m_pPlayer->Get_HideObject())->Set_State(CTrunk::STATE::SEARCH_IN);

					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_TRUNK_IN);
					m_pPlayerMeshCom->Set_AnimationSet(257);
					m_pPlayer->Get_PlayerBehavior()->Set_OnePlayAni(9);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             

					Engine::MyPlaySound(L"Trunk.mp3", Engine::CHANNELID::TRUNK);
				}
				else if (PLAYER_COL_CUPBOARD & uColTo)
				{
					m_pElite->Set_CurState(ELITE_STATE::ELITE_CUPBOARD_IN);
					m_pEliteMeshCom->Set_AnimationSet(179);
					Set_OnePlayAni(0.85);

					static_cast<CCupboard*>(m_pPlayer->Get_HideObject())->Set_State(CCupboard::STATE::SEARCH_IN);

					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CUPBOARD_IN);
					m_pPlayerMeshCom->Set_AnimationSet(265);
					m_pPlayer->Get_PlayerBehavior()->Set_OnePlayAni(9);

					Engine::MyPlaySound(L"Trunk.mp3", Engine::CHANNELID::TRUNK);
				}
				m_vNewElitePos = pHideTransform->Get_InfoPos() + pHideTransform->m_vInfo[Engine::INFO_LOOK] * 1.f;

				_vec3 vPlayerPos = pHideTransform->Get_InfoPos();
				vPlayerPos.y = m_pPlayerTransCom->Get_InfoPos().y;
				
				_ushort uNum = 0;
				if (PLAYER_COL_TRUNK & uColTo)
				{
					for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_TRUNK))
					{
						if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
							break;
						else
							uNum++;
					}

					if (0 == uNum)
						vPlayerPos.x += 1.65f;
					else if (1 == uNum)
						vPlayerPos.z -= 1.65f;
				}
				else if (PLAYER_COL_CUPBOARD & uColTo)
				{
					for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_CUPBOARD))
					{
						if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
							break;
						else
							uNum++;
					}

					if (0 == uNum)
						vPlayerPos.x += 1.f;
					else if (1 == uNum)
						vPlayerPos.z -= 1.f;
					else if (2 == uNum)
						vPlayerPos.x += 1.f;
				}

				m_pPlayer->Set_NewPos(vPlayerPos);
				m_pPlayerTransCom->Set_Pos(&vPlayerPos);
				m_pPlayer->Get_PlayerBehavior()->Set_NewPlayerPos(vPlayerPos);

				m_pPlayer->Set_InitPlayerLook(&(-pHideTransform->m_vInfo[Engine::INFO_LOOK]));

				static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_CameraPurposeStart(FALSE);
				static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_CameraPurposeExit(TRUE);
			}
		}

		return;
	}

	/*  Elite Look  */
	if (3.f > D3DXVec3Length(&(vHideObjectPos - m_vNewElitePos)))
	{
		_vec3 vPlayerPos = vHideObjectPos - pHideTransform->m_vInfo[Engine::INFO_LOOK] * 0.8f;
		m_vNewEliteLook = vPlayerPos - m_vNewElitePos;
		m_vNewEliteLook.y = 0.f;
		D3DXVec3Normalize(&m_vNewEliteLook, &m_vNewEliteLook);
	}

	m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook;
	D3DXVec3Cross(&m_pEliteTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_vNewEliteLook);
	D3DXVec3Cross(&m_pEliteTransCom->m_vInfo[Engine::INFO_UP], &m_vNewEliteLook, &m_pEliteTransCom->m_vInfo[Engine::INFO_RIGHT]);
	D3DXVec3Normalize(&m_pEliteTransCom->m_vInfo[Engine::INFO_UP], &m_pEliteTransCom->m_vInfo[Engine::INFO_UP]);

	/*  SwordActive Out  */
	if (30.f < D3DXVec3Length(&(vHideObjectPos - m_vNewElitePos)))
	{
		Set_SwordActiveOut();
		return;
	}
}

void CEliteBehavior::Update_NoHideBehavior(const Engine::_float& fTimeDelta)
{
	_vec3 vPlayerPos = m_pPlayerTransCom->Get_InfoPos();

	m_vNewEliteLook = vPlayerPos - m_vNewElitePos;
	m_vNewEliteLook.y = 0.f;
	D3DXVec3Normalize(&m_vNewEliteLook, &m_vNewEliteLook);
	m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook;

	D3DXVec3Cross(&m_pEliteTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_vNewEliteLook);
	D3DXVec3Cross(&m_pEliteTransCom->m_vInfo[Engine::INFO_UP], &m_vNewEliteLook, &m_pEliteTransCom->m_vInfo[Engine::INFO_RIGHT]);
	D3DXVec3Normalize(&m_pEliteTransCom->m_vInfo[Engine::INFO_UP], &m_pEliteTransCom->m_vInfo[Engine::INFO_UP]);

	if (1.4f < D3DXVec3Length(&(vPlayerPos - m_vNewElitePos)))
	{
		m_fSpeed = 2.5f;
		m_vNewElitePos = m_pEliteTransCom->m_vInfo[Engine::INFO_POS] + m_vNewEliteLook * m_fSpeed * fTimeDelta;
		m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
	}
	else
	{
		m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);

		m_fAttackCoolTime += 1.f * fTimeDelta;

		if (2.f <= m_fAttackCoolTime)
		{
			m_fAttackCoolTime = 0.f;

			if (0 == m_uAttackPattern)
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_ATTACK_UP);
				m_pEliteMeshCom->Set_AnimationSet(144);
			}
			else if (1 == m_uAttackPattern)
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_ATTACK_RIGHT);
				m_pEliteMeshCom->Set_AnimationSet(145);
			}
			else if (2 == m_uAttackPattern)
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_ATTACK_LEFT);
				m_pEliteMeshCom->Set_AnimationSet(146);
			}

			Set_OnePlayAni(0.85);

			m_uAttackPattern++;
			if (2 < m_uAttackPattern)
				m_uAttackPattern = 0;
		}
	}

	/*  SwordActive Out  */
	if (30.f < D3DXVec3Length(&(vPlayerPos - m_vNewElitePos)))
	{
		Set_SwordActiveOut();
		return;
	}
}

_bool CEliteBehavior::Update_Story(const Engine::_float& fTimeDelta)
{
	if (m_pElite->Get_EliteID() != Engine::Get_EliteWave()
		|| FALSE == m_bFightStory)
		return FALSE;

	_vec3 vPos;
	_vec3 vDist;

	switch (m_pElite->Get_EliteID())
	{
	case 7:
		{
			_bool bRun = FALSE;

			if (0 == m_uWayPointCurPattern)
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_R90);
				Set_OnePlayAni(0.98);
				m_uWayPointCurPattern = 1;
			}
			else if (1 == m_uWayPointCurPattern)
			{
				vPos = _vec3(50.0619f, m_vNewElitePos.y, 80.4855f);
				vPos.z -= 1.f;

				vDist = vPos - m_vNewElitePos;

				m_pElite->Set_InitEliteLook(&vDist);

				if (0.5f > D3DXVec3Length(&vDist))
				{
					if (ELITE_STATE::ELITE_RUN == m_pElite->Get_CurState())
					{
						m_pElite->Set_CurState(ELITE_STATE::ELITE_CUPBOARD_IN);
						Set_OnePlayAni(0.85);

						_ushort uNum = 0;
						for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_CUPBOARD))
						{
							if (1 == uNum)
							{
								static_cast<CCupboard*>(iter)->Set_State(CCupboard::STATE::SEARCH_IN);
								break;
							}
							uNum++;
						}
					}
					else if (ELITE_STATE::ELITE_IDLE == m_pElite->Get_CurState())
					{
						m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_L90);
						Set_OnePlayAni(0.98);
						m_uWayPointCurPattern = 2;
					}
				}
				else
					bRun = TRUE;
			}
			else if (2 == m_uWayPointCurPattern)
			{
				vPos = _vec3(41.5634f, m_vNewElitePos.y, 77.5179f);

				vDist = vPos - m_vNewElitePos;

				m_pElite->Set_InitEliteLook(&vDist);

				if (0.5f > D3DXVec3Length(&vDist))
					m_uWayPointCurPattern = 3;
				else
					bRun = TRUE;
			}
			else if (3 == m_uWayPointCurPattern)
			{
				vPos = _vec3(38.6f, m_vNewElitePos.y, 73.9935f);

				vDist = vPos - m_vNewElitePos;

				m_pElite->Set_InitEliteLook(&vDist);

				if (0.5f > D3DXVec3Length(&vDist))
				{
					//if (ELITE_STATE::ELITE_RUN == m_pElite->Get_CurState())
					{
						m_pElite->Set_FightState(FALSE);
						m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
						m_pEliteMeshCom->Set_AnimationSet(139);

						if (139 == m_pEliteMeshCom->Get_AnimationIndex()
							&& m_pEliteMeshCom->Is_AnimationSetRateEnd(1.9))
						{
							m_uWayPointCurPattern = 4;
							m_pElite->Set_CurState(ELITE_STATE::ELITE_SWORD_ACTIVEOUT);
							m_pEliteMeshCom->Set_AnimationSet(140);
							m_fAttackCoolTime = 0.f;
							m_bMoveStory = TRUE;
							m_bFightStory = FALSE;
							m_pElite->Set_OnMoveWay(TRUE);
						}
					}
				}
				else
					bRun = TRUE;
			}

			if (TRUE == bRun)
			{
				m_fSpeed = 2.25f;
				m_vNewElitePos = m_pEliteTransCom->m_vInfo[Engine::INFO_POS] + m_vNewEliteLook * m_fSpeed * fTimeDelta;
				m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
			}
		}
		break;

	case 11:
		{
			_bool bRun = FALSE;

			if (0 == m_uWayPointCurPattern)
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_R180);
				Set_OnePlayAni(0.98);
				m_uWayPointCurPattern = 1;
			}
			else if (1 == m_uWayPointCurPattern)
			{
				Engine::CTransform* pTransCom = static_cast<Engine::CTransform*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LEVER)->Get_Component(Engine::COM_TRANSFORM));
				vPos = pTransCom->Get_InfoPos();
				vPos.y = m_vNewElitePos.y;
				vPos.z += 1.f;

				vDist = vPos - m_vNewElitePos;

				m_pElite->Set_InitEliteLook(&vDist);

				if (0.05f > D3DXVec3Length(&vDist))
				{
					m_pElite->Set_InitEliteLook(&_vec3(0.f, 0.f, -1.f));
					m_pElite->Set_CurState(ELITE_STATE::ELITE_LEVER_ON);
					Set_OnePlayAni(0.9);
					m_uWayPointCurPattern = 2;
					static_cast<CLever*>(Engine::Find_ObjectListBack(Engine::OBJECT_LEVER))->Set_Active();
				}
				else
					bRun = TRUE;
			}
			else if (2 == m_uWayPointCurPattern)
			{
				Engine::CTransform* pTransCom = static_cast<Engine::CTransform*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ELITE)->Get_Component(Engine::COM_TRANSFORM));
				vPos = pTransCom->Get_InfoPos();
				vPos.y = m_vNewElitePos.y;
				vPos.x -= 1.f;

				vDist = vPos - m_vNewElitePos;

				m_pElite->Set_InitEliteLook(&vDist);

				if (0.05f > D3DXVec3Length(&vDist))
				{
					m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
					m_uWayPointCurPattern = 3;
				}
				else
					bRun = TRUE;
			}

			if (TRUE == bRun)
			{
				m_fSpeed = 2.25f;
				m_vNewElitePos = m_pEliteTransCom->m_vInfo[Engine::INFO_POS] + m_vNewEliteLook * m_fSpeed * fTimeDelta;
				m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
			}
		}
		break;

	case 12:
		{
			_bool bRun = FALSE;

			if (3 == m_uWayPointCurPattern)
			{
				vPos = _vec3(79.978f, m_vNewElitePos.y, 54.5315f);

				vDist = vPos - m_vNewElitePos;

				m_pElite->Set_InitEliteLook(&vDist);

				if (0.5f > D3DXVec3Length(&vDist))
				{
					m_uWayPointCurPattern = 4;
					m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
				}
				else
					bRun = TRUE;
			}
			else if (4 == m_uWayPointCurPattern)
			{
				if (FALSE == static_cast<CGrid*>(Engine::Find_ObjectListBack(Engine::OBJECT_GRID))->Get_CloseActive())
					return TRUE;

				m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_L180);
				Set_OnePlayAni(0.98);
				m_uWayPointCurPattern = 5;
			}
			else if (5 == m_uWayPointCurPattern)
			{
				vPos = _vec3(77.5762f, m_vNewElitePos.y, 54.5342f);

				vDist = vPos - m_vNewElitePos;

				m_pElite->Set_InitEliteLook(&vDist);

				if (0.5f > D3DXVec3Length(&vDist))
				{
					m_uWayPointCurPattern = 6;
					m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
				}
				else
					bRun = TRUE;
			}
			else if (6 == m_uWayPointCurPattern)
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);

				m_fAttackCoolTime += 1.f * fTimeDelta;

				if (1.5f <= m_fAttackCoolTime)
				{
					m_fAttackCoolTime = 0.f;

					if (0 == m_uAttackPattern)
					{
						m_pElite->Set_CurState(ELITE_STATE::ELITE_ATTACK_UP);
						m_pEliteMeshCom->Set_AnimationSet(144);
					}
					else if (1 == m_uAttackPattern)
					{
						m_pElite->Set_CurState(ELITE_STATE::ELITE_ATTACK_RIGHT);
						m_pEliteMeshCom->Set_AnimationSet(145);
					}
					else if (2 == m_uAttackPattern)
					{
						m_pElite->Set_CurState(ELITE_STATE::ELITE_ATTACK_LEFT);
						m_pEliteMeshCom->Set_AnimationSet(146);
					}

					Engine::MyPlaySound(L"SwordCrash.wav", Engine::CHANNELID::SWORD_CRASH);
					Set_OnePlayAni(0.35);

					m_uAttackPattern++;
					if (2 < m_uAttackPattern)
						m_uAttackPattern = 0;
				}
			}

			if (TRUE == bRun)
			{
				m_fSpeed = 4.f;
				m_vNewElitePos = m_pEliteTransCom->m_vInfo[Engine::INFO_POS] + m_vNewEliteLook * m_fSpeed * fTimeDelta;
				m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
			}
		}
		break;

	}

	return TRUE;
}

void CEliteBehavior::Set_SwordActiveOut()
{
	m_pElite->Set_FightState(FALSE);
	m_pElite->Set_OnMoveWay(FALSE);
	m_pElite->Set_CurState(ELITE_STATE::ELITE_SWORD_ACTIVEOUT);
	Set_OnePlayAni(0.85);
	m_fAttackCoolTime = 0.f;
}
