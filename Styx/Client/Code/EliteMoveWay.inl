void CEliteBehavior::Update_MoveWay(const Engine::_float& fTimeDelta)
{
	if (TRUE == m_bOnePlayAnimation
		|| TRUE == m_pElite->Get_SwordActive()
		|| FALSE == m_pElite->Get_OnMoveWay()
		|| TRUE == m_pElite->Get_CarryState()
		|| TRUE == m_pElite->Get_FightState()
		|| FALSE == m_pElite->Get_Live())
		return;

	m_fSpeed = 1.f;
	if (ELITE_STATE::ELITE_RUN == m_pElite->Get_CurState()
		|| 12 == m_pElite->Get_EliteID())
	{
		m_fSpeed = 2.f;

		if (12 == m_pElite->Get_EliteID()
			&& ELITE_STATE::ELITE_RUN == m_pElite->Get_CurState())
			m_fSpeed = 4.f;
	}

	/*  Way  */
	if (0 < m_uWayPointNum)
	{
		if (ELITE_STATE::ELITE_WALK == m_pElite->Get_CurState()
			|| ELITE_STATE::ELITE_RUN == m_pElite->Get_CurState())
		{
			m_vNewEliteLook = m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK];
			D3DXVec3Normalize(&m_vNewEliteLook, &m_vNewEliteLook);
			m_vNewElitePos = m_pEliteTransCom->m_vInfo[Engine::INFO_POS] + m_vNewEliteLook * m_fSpeed * fTimeDelta;
		}
	}
	/*  No Way  */
	else
	{
		m_vNewElitePos = m_pEliteTransCom->m_vInfo[Engine::INFO_POS];
	}

	/*  Update Way  */
	Update_Way(fTimeDelta);

	/*  Update Scan  */
	Update_Scan(fTimeDelta);

	m_pElite->Set_NewPos(m_vNewElitePos);
}

void CEliteBehavior::Update_Way(const Engine::_float& fTimeDelta)
{
	_vec3 vDist;

	switch (m_pElite->Get_EliteID())
	{
	case 0:
		if (0 == m_uWayPointCurPattern)
		{
			vDist = m_vWayPointPos[1] - m_vNewElitePos;
			vDist.y = 0.f;

			/*  哭率 哎锭  */
			if (1.f > D3DXVec3Length(&vDist))
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_L90);
				Set_OnePlayAni(0.98);
				m_uWayPointCurPattern = 1;
				m_uWayPointDestiPattern = 2;
			}
		}
		else if (1 == m_uWayPointCurPattern)
		{
			/*  坷弗率  */
			if (2 == m_uWayPointDestiPattern)
			{
				vDist = m_vWayPointPos[2] - m_vNewElitePos;
				vDist.y = 0.f;

				if (1.f > D3DXVec3Length(&vDist))
				{
					m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_R180);
					Set_OnePlayAni(0.98);
					m_uWayPointCurPattern = 2;
					m_uWayPointDestiPattern = 1;
				}
			}
			/*  关率  */
			else if (0 == m_uWayPointDestiPattern)
			{
				vDist = m_vWayPointPos[0] - m_vNewElitePos;
				vDist.y = 0.f;

				if (1.f > D3DXVec3Length(&vDist))
				{
					m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_R180);
					Set_OnePlayAni(0.98);
					m_uWayPointCurPattern = 0;
					m_uWayPointDestiPattern = 1;
				}
			}
		}
		else if (2 == m_uWayPointCurPattern)
		{
			vDist = m_vWayPointPos[1] - m_vNewElitePos;
			vDist.y = 0.f;

			/*  关率 哎锭  */
			if (1.f > D3DXVec3Length(&vDist))
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_R90);
				Set_OnePlayAni(0.98);
				m_uWayPointCurPattern = 1;
				m_uWayPointDestiPattern = 0;
			}
		}

		m_pElite->Set_InitEliteLook(&vDist);
		break;

	case 1:
	case 2:
	case 3:
	case 4:
	case 12:
		if (TRUE == m_bPoison
			|| TRUE == m_pElite->Get_MakeSoundPattern())
			break;

		if (0 == m_uWayPointCurPattern)
		{
			vDist = m_vWayPointPos[1] - m_vNewElitePos;
			vDist.y = 0.f;

			if (1.f > D3DXVec3Length(&vDist))
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_R180);
				Set_OnePlayAni(0.98);
				m_uWayPointCurPattern = 1;
			}
		}
		else if (1 == m_uWayPointCurPattern)
		{
			vDist = m_vWayPointPos[0] - m_vNewElitePos;
			vDist.y = 0.f;

			if (1.f > D3DXVec3Length(&vDist))
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_R180);
				Set_OnePlayAni(0.98);
				m_uWayPointCurPattern = 0;
			}
		}

		m_pElite->Set_InitEliteLook(&vDist);
		break;

	case 5:
		break;

	case 6:
		break;

	case 7:
		break;

	case 8:
		break;

	case 9:
		break;

	case 10:
		break;

	case 11:
		break;

	}

}

void CEliteBehavior::Update_Scan(const Engine::_float& fTimeDelta)
{
	if (9 != m_pElite->Get_EliteID())
	{
		if (m_pElite->Get_EliteID() != Engine::Get_EliteWave())
			return;
	}

	switch (m_pElite->Get_EliteID())
	{
	case 0:
		break;

	case 1:
		Scan_Info(0.3f, 6.f, 45.f);
		break;

	case 2:
		break;

	case 3:
		break;

	case 4:
		if (FALSE == m_bPoison)
		{
			for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_ITEM))
			{
				if (Engine::COL_OBJECTID::COL_OBJECT_CUP == iter->Get_ColObjectID())
				{
					if (TRUE == static_cast<CFeature*>(iter)->Get_Poison())
					{
						if (0 == m_uWayPointCurPattern
							&& ELITE_STATE::ELITE_WALK == m_pElite->Get_CurState())
						{
							m_bPoison = TRUE;
						}
					}
				}
			}
		}
		else if (TRUE == m_bPoison)
		{
			_vec3 vPos;
			_vec3 vDist;
			if (0 == m_uWayPointCurPattern)
			{
				vPos = _vec3(48.5342f, m_vNewElitePos.y, 66.4959f);
				vDist = vPos - m_vNewElitePos;
				vDist.y = 0.f;

				if (0.5f > D3DXVec3Length(&vDist))
				{
					m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_R90);
					Set_OnePlayAni(0.98);
					m_uWayPointCurPattern = 1;
				}
			}
			else if (1 == m_uWayPointCurPattern)
			{
				vPos = _vec3(48.5272f, m_vNewElitePos.y, 64.9f);
				vDist = vPos - m_vNewElitePos;
				vDist.y = 0.f;

				if (0.5f > D3DXVec3Length(&vDist))
				{
					if (ELITE_STATE::ELITE_DRINK_DEAD != m_pElite->Get_CurState())
					{
						m_pElite->Set_CurState(ELITE_STATE::ELITE_DRINK_IN);
						Set_OnePlayAni(0.98);
					}
				}
			}

			m_pElite->Set_InitEliteLook(&vDist);
		}
		break;

	case 5:
		{
			if (ELITE_STATE::ELITE_CLEAN_BROOM == m_pElite->Get_CurState())
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_CLEAN_BROOM_OUT);
				Set_OnePlayAni(0.85);
				m_uWayPointNum = 1;
			}

			_vec3 vPos;
			_vec3 vDist;
			if (0 == m_uWayPointCurPattern)
			{
				vPos = _vec3(48.5272f, m_vNewElitePos.y, 65.2f);
				vDist = vPos - m_vNewElitePos;
				vDist.y = 0.f;

				if (1.f > D3DXVec3Length(&vDist))
				{
					if (ELITE_STATE::ELITE_RUN == m_pElite->Get_CurState())
					{
						m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
						m_pElite->Set_IdleOrder(1);
					}
					else if (ELITE_STATE::ELITE_IDLE == m_pElite->Get_CurState())
					{
						if (m_pEliteMeshCom->Is_AnimationSetRateEnd(0.9))
						{
							m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
							m_uWayPointCurPattern = 1;
						}
					}
				}
			}
			else if (1 == m_uWayPointCurPattern)
			{
				vPos = _vec3(52.f, m_vNewElitePos.y, 66.9468f);
				vDist = vPos - m_vNewElitePos;
				vDist.y = 0.f;

				if (1.f > D3DXVec3Length(&vDist))
				{
					m_pElite->Set_CurState(ELITE_STATE::ELITE_SWORD_ACTIVEIN);
					m_pElite->Set_SwordActive(TRUE);
					Set_OnePlayAni(0.85);
				}
			}

			m_pElite->Set_InitEliteLook(&vDist);
		}
		break;

	case 6:
		break;

	case 7:
		{
			if (TRUE == m_bFullStory)
			{
				m_pElite->Set_InitEliteLook(&_vec3(-1.f, 0.f, 0.f));
				m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
				Scan_Info(0.3f, 4.5f, 30.f);
				break;
			}

			if ( ELITE_STATE::ELITE_IDLE == m_pElite->Get_CurState())
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
				m_uWayPointNum = 1;
			}

			_vec3 vPos;
			_vec3 vDist;

			if (FALSE == m_bMoveStory)
			{
				if (0 == m_uWayPointCurPattern)
				{
					//vPos = _vec3(50.0383f, m_vNewElitePos.y, 72.5915f);
					vPos = _vec3(49.5f, m_vNewElitePos.y, 71.3225f);
					vDist = vPos - m_vNewElitePos;
					vDist.y = 0.f;

					if (1.f > D3DXVec3Length(&vDist))
					{
						if (ELITE_STATE::ELITE_RUN == m_pElite->Get_CurState())
						{
							m_pElite->Set_CurState(ELITE_STATE::ELITE_SWORD_ACTIVEIN);
							m_pElite->Set_SwordActive(TRUE);
							Set_OnePlayAni(0.85);
							m_bFightStory = TRUE;
							m_pElite->Set_OnMoveWay(FALSE);
						}
					}
				}
			}
			else if (TRUE == m_bMoveStory)
			{
				if (4 == m_uWayPointCurPattern)
				{
					if (140 == m_pEliteMeshCom->Get_AnimationIndex()
						&& m_pEliteMeshCom->Is_AnimationSetRateEnd(0.85))
					{
						m_pElite->Set_SwordActive(FALSE);
						m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_L180);
						Set_OnePlayAni(0.98);
						m_uWayPointCurPattern = 5;
					}

					vDist = m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK];
				}
				else if (5 == m_uWayPointCurPattern)
				{
					vPos = _vec3(41.5634f, m_vNewElitePos.y, 77.5179f);

					vDist = vPos - m_vNewElitePos;

					if (0.5f > D3DXVec3Length(&vDist))
						m_uWayPointCurPattern = 6;
					else
						m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
				}
				else if (6 == m_uWayPointCurPattern)
				{
					vPos = _vec3(47.9283f, m_vNewElitePos.y, 77.f);

					vDist = vPos - m_vNewElitePos;

					if (0.5f > D3DXVec3Length(&vDist))
						m_uWayPointCurPattern = 8;
					else
						m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
				}
		/*		else if (7 == m_uWayPointCurPattern)
				{
					vPos = _vec3(48.4569f, m_vNewElitePos.y, 75.0737f);

					vDist = vPos - m_vNewElitePos;

					if (0.5f > D3DXVec3Length(&vDist))
						m_uWayPointCurPattern = 8;
					else
						m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
				}*/
				else if (8 == m_uWayPointCurPattern)
				{
					vDist = m_vWayPointPos[0] - m_vNewElitePos;

					if (0.5f > D3DXVec3Length(&vDist))
						m_uWayPointCurPattern = 9;
					else
						m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
				}
				else if (9 == m_uWayPointCurPattern)
				{
					m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_L180);
					Set_OnePlayAni(0.98);
					m_bFullStory = TRUE;

					vDist = m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK];
				}
			}
			m_pElite->Set_InitEliteLook(&vDist);
			
		}
		break;

	case 8:
		break;

	case 9:
		if (TRUE == m_pElite->Get_WorkChange())
		{
			if (ELITE_STATE::ELITE_IDLE == m_pElite->Get_CurState())
			{ 
				m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
				m_uWayPointNum = 1;
			}

			_vec3 vPos;
			_vec3 vDist;

			if (0 == m_uWayPointCurPattern)
			{
				vPos = _vec3(77.851f, m_vNewElitePos.y, 56.29f);
				vDist = vPos - m_vNewElitePos;
				vDist.y = 0.f;

				if (0.5f > D3DXVec3Length(&vDist))
				{
					m_fTime += 1.f * fTimeDelta;
					m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);

					if (m_pElite->Get_EliteID() == Engine::Get_EliteWave())
					{
						Engine::Set_EliteWave();

						USHORT m_uEliteID = m_pElite->Get_EliteID();
						if (3 == m_uEliteID || 5 == m_uEliteID || 8 == m_uEliteID || 11 == m_uEliteID)
							static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_TipOneRender(TRUE);
					}

					if (2.55f < m_fTime)
					{
						m_uWayPointCurPattern = 1;

						m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_R90);
						Set_OnePlayAni(0.98);
					}
				}
				else
					m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
			}
			else if (1 == m_uWayPointCurPattern)
			{
				vPos = _vec3(84.8226f, m_vNewElitePos.y, 55.608f);
				vDist = vPos - m_vNewElitePos;
				vDist.y = 0.f;

				if (0.5f > D3DXVec3Length(&vDist))
				{
					m_uWayPointCurPattern = 2;

					m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_L90);
					Set_OnePlayAni(0.98);
				}
				else
					m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
			}
			else if (2 == m_uWayPointCurPattern)
			{
				vPos = _vec3(84.8226f, m_vNewElitePos.y, 60.7f);
				vDist = vPos - m_vNewElitePos;
				vDist.y = 0.f;

				if (0.5f > D3DXVec3Length(&vDist))
				{
					m_uWayPointCurPattern = 3;
					m_pElite->Set_Hide(TRUE);
				}
				else
					m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
			}

			m_pElite->Set_InitEliteLook(&vDist);
		}
		break;

	case 10:
		{
			if (ELITE_STATE::ELITE_SLEEP_LOOP == m_pElite->Get_CurState())
			{
				m_uWayPointNum = 1;
				m_pElite->Set_CurState(ELITE_STATE::ELITE_SLEEP_OUT);
				Set_OnePlayAni(0.95);
				return;
			}

			_vec3 vPos;
			_vec3 vDist;

			if (0 == m_uWayPointCurPattern)
			{
				vPos = _vec3(84.1226f, m_vNewElitePos.y, 55.608f);
				vDist = vPos - m_vNewElitePos;
				vDist.y = 0.f;

				if (0.5f > D3DXVec3Length(&vDist))
				{
					m_uWayPointCurPattern = 1;

					m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_L90);
					Set_OnePlayAni(0.98);
				}
				else
					m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
			}
			else if (1 == m_uWayPointCurPattern)
			{
				vPos = _vec3(84.1226f, m_vNewElitePos.y, 60.7f);
				vDist = vPos - m_vNewElitePos;
				vDist.y = 0.f;

				if (2.f > D3DXVec3Length(&vDist)
					&& 1.5f < D3DXVec3Length(&vDist))
				{
					static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(18);
				}
				if (0.5f > D3DXVec3Length(&vDist))
				{
					m_uWayPointCurPattern = 2;
					m_pElite->Set_Hide(TRUE);

					if (m_pElite->Get_EliteID() == Engine::Get_EliteWave())
					{
						Engine::Set_EliteWave();

						USHORT m_uEliteID = m_pElite->Get_EliteID();
						if (3 == m_uEliteID || 5 == m_uEliteID || 8 == m_uEliteID || 11 == m_uEliteID)
							static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_TipOneRender(TRUE);
					}
				}
				else
					m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
			}

			m_pElite->Set_InitEliteLook(&vDist);
		}
		break; 

	case 11:
		if (
			TRUE == m_pPlayer->Get_Translucent()
			&& TRUE == m_pPlayer->Get_CarryState()
			&&
			10.05f <= m_pPlayerTransCom->Get_InfoPos().y)
		{
			if (TRUE == static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Get_ElitePerson())
			{
				m_fTime += 1.f * fTimeDelta;

				if (4.f < m_fTime)
				{
					m_uWayPointNum = 1;
					m_uWayPointCurPattern = 0;
					m_pElite->Set_SwordActive(TRUE);
					m_pElite->Set_OnMoveWay(FALSE);
					m_pElite->Set_CurState(ELITE_STATE::ELITE_SWORD_ACTIVEIN);
					Set_OnePlayAni(0.85);
					m_bFightStory = TRUE;
					m_bMoveStory = FALSE;
					static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_ElitePerson(FALSE);
				}
			}
		}
		break;

	case 12:
		if (FALSE == m_pElite->Get_MakeSoundPattern())
			return;

		if (ELITE_STATE::ELITE_WALK == m_pElite->Get_CurState()
			&& 2 > m_uWayPointCurPattern)
		{
			m_uWayPointCurPattern = 2;
			m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_R180);
			Set_OnePlayAni(0.98);
			return;
		}

		_vec3 vPos;
		_vec3 vDist;

		if (2 == m_uWayPointCurPattern)
		{
			vPos = _vec3(73.1392f, m_vNewElitePos.y, 55.6716f);
			vDist = vPos - m_vNewElitePos;
			vDist.y = 0.f;

			if (0.5f > D3DXVec3Length(&vDist))
			{
				m_uWayPointCurPattern = 3;

				m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_L90);
				Set_OnePlayAni(0.98);
			}
			else
				m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
		}
		else if (3 == m_uWayPointCurPattern)
		{
			m_pElite->Set_SwordActive(TRUE);
			m_pElite->Set_OnMoveWay(FALSE);
			m_pElite->Set_CurState(ELITE_STATE::ELITE_SWORD_ACTIVEIN);
			Set_OnePlayAni(0.85);
			m_bFightStory = TRUE;
			m_bMoveStory = FALSE;

			vDist = m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK];
		}

		m_pElite->Set_InitEliteLook(&vDist);
		break;
	}
}

void CEliteBehavior::Scan_Info(_float _fY, _float _fDist, _float _fAngle)
{
	_vec3 vDist = m_pPlayerTransCom->Get_InfoPos() - m_vNewElitePos;

	if (m_pPlayerTransCom->Get_InfoPos().y > m_vNewElitePos.y - _fY)
	{
		if (_fDist > D3DXVec3Length(&vDist))
		{
			_vec3 vPlayerDir = vDist;
			D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);
			D3DXVec3Normalize(&m_vNewEliteLook, &m_vNewEliteLook);
			float fAngle = D3DXVec3Dot(&m_vNewEliteLook, &vPlayerDir);

			//cout << "Angle : " << D3DXToDegree(acosf(fAngle)) << endl;
			if (D3DXToRadian(_fAngle) > acosf(fAngle))
			{
				m_pElite->Set_SwordActive(TRUE);
				m_pElite->Set_OnMoveWay(FALSE);
				m_pElite->Set_CurState(ELITE_STATE::ELITE_SWORD_ACTIVEIN);
				Set_OnePlayAni(0.85);
				m_vWayReturnPos = m_vNewElitePos;
			}

		}
		/*	else
				cout << "Dist : " << D3DXVec3Length(&vDist) << endl;*/
	}
}


