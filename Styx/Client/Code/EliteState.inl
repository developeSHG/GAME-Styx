void CElite::Update_State(void)
{
	/*  Basic  */
	switch (m_eCurState)
	{
	case ELITE_STATE::ELITE_HIDE_TRUNK_IN:
		m_pMeshCom->Set_AnimationSet(52);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_eCurState = ELITE_HIDE;
			m_vNewPos = m_pTransCom->m_vInfo[Engine::INFO_POS] + m_pTransCom->m_vInfo[Engine::INFO_LOOK] * -90.f;
		}
		break;

	case ELITE_STATE::ELITE_HIDE_CUPBOARD_IN:
		m_pMeshCom->Set_AnimationSet(54);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_eCurState = ELITE_HIDE;
			m_vNewPos = m_pTransCom->m_vInfo[Engine::INFO_POS] + m_pTransCom->m_vInfo[Engine::INFO_LOOK] * -75.f;
		}
		break;

	case ELITE_STATE::ELITE_HIDE:
		m_pMeshCom->Set_AnimationSet(53, 0.f, 1.f, 0.f, 0.01f);
		break;

	case ELITE_STATE::ELITE_CARRY_PICK:
		m_pMeshCom->Set_AnimationSet(59);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_eCurState = ELITE_STATE::ELITE_CARRY_IDLE;
			m_pMeshCom->Set_TrackPosition(0.9);
			m_pMeshCom->Set_AnimationSet(60);
		}
		break;

	case ELITE_STATE::ELITE_CARRY_IDLE:
		m_pMeshCom->Set_AnimationSet(60);
		break;

	case ELITE_STATE::ELITE_CARRY_WALK:
		m_pMeshCom->Set_AnimationSet(56);
		break;

	case ELITE_STATE::ELITE_CARRY_RUN:
		m_pMeshCom->Set_AnimationSet(57);
		break;

	case ELITE_STATE::ELITE_CARRY_DROP:
		m_pMeshCom->Set_AnimationSet(61);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_bCarryState = FALSE;
			m_eCurState = ELITE_STATE::ELITE_DEAD;
			m_pMeshCom->Set_TrackPosition(0.85);
			m_pMeshCom->Set_AnimationSet(84);
		}
		break;

	case ELITE_STATE::ELITE_SICK_LOOP:
		m_pMeshCom->Set_AnimationSet(214);
		break;

	case ELITE_STATE::ELITE_SICK_OUT:
		m_pMeshCom->Set_AnimationSet(213);
		break;

	case ELITE_STATE::ELITE_CUTLASS_DEAD:
		m_pMeshCom->Set_AnimationSet(80);
		break;

	case ELITE_STATE::ELITE_DEAD:
		m_pMeshCom->Set_AnimationSet(84);
		// 83 back

		m_bLive = FALSE;
		if (Get_EliteID() == Engine::Get_EliteWave())
			Engine::Set_EliteWave();

		if (TRUE == m_bFightStateSound)
		{
			Engine::StopSound(Engine::CHANNELID::BGM);
			Engine::PlayBGM(L"StageBGM.mp3");

			m_bFightStateSound = FALSE;
		}

		break;

	}

	/*  Sword Not Active  */
	if (FALSE == m_bSwordActive)
	{
		switch (m_eCurState)
		{
		case ELITE_STATE::ELITE_IDLE:
			if (0 == m_uIdleOrder)
				m_pMeshCom->Set_AnimationSet(101);
			else if (1 == m_uIdleOrder)
				m_pMeshCom->Set_AnimationSet(102);
			else if (2 == m_uIdleOrder)
				m_pMeshCom->Set_AnimationSet(103);
			else if (3 == m_uIdleOrder)
				m_pMeshCom->Set_AnimationSet(104);
			else if (4 == m_uIdleOrder)
				m_pMeshCom->Set_AnimationSet(105);
			else if (5 == m_uIdleOrder)
				m_pMeshCom->Set_AnimationSet(106);
			else if (6 == m_uIdleOrder)
				m_pMeshCom->Set_AnimationSet(107);

			if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
			{
				if(TRUE == m_bOnMoveWay)
					m_uIdleOrder++;
				else
					m_bReturnState = TRUE;
			}

			if (7 <= m_uIdleOrder)
				m_uIdleOrder = 0;

			break;

		case ELITE_STATE::ELITE_WALK:
			m_pMeshCom->Set_AnimationSet(89);
			break;

		case ELITE_STATE::ELITE_RUN:
			m_pMeshCom->Set_AnimationSet(92);
			break;

		case ELITE_STATE::ELITE_IDLE_TURN_L180:
			m_pMeshCom->Set_AnimationSet(99);
			break;

		case ELITE_STATE::ELITE_IDLE_TURN_L90:
			m_pMeshCom->Set_AnimationSet(100);
			break;

		case ELITE_STATE::ELITE_IDLE_TURN_R90:
			m_pMeshCom->Set_AnimationSet(98);
			break;

		case ELITE_STATE::ELITE_IDLE_TURN_R180:
			m_pMeshCom->Set_AnimationSet(97);
			break;

		case ELITE_STATE::ELITE_RUSH:
			m_pMeshCom->Set_AnimationSet(93);
			break;

		case ELITE_STATE::ELITE_ASSASS_IN:
			m_pMeshCom->Set_AnimationSet(22);

			m_bOnMoveWay = FALSE;

			if(FALSE == m_bDieSound)
			{
				m_bDieSound = TRUE;
				Engine::MyPlaySound(L"SilentKill.mp3", Engine::CHANNELID::SPARK);
			}

			//if(PLAYER_STATE::PLAYER_ASSASS_IN == m_pPlayer->Get_CurState()
			//	&& m_pPlayer->Get_AniRateEnd())
			//if (m_pMeshCom->Is_AnimationSetRateEnd(0.5))
			//{
			//	m_eNextState = PLAYER_ASSASS_OUT;
			//	m_pPlayerBehavior->Set_OnePlayAni(0.95);
			//}
			break;

		case ELITE_STATE::ELITE_ASSASS_LOOP:
			m_pMeshCom->Set_AnimationSet(21);
			break;

		case ELITE_STATE::ELITE_ASSASS_OUT:
			m_pMeshCom->Set_AnimationSet(19);

			if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
			{
				//m_eCurState = ELITE_DEAD;
				m_pMeshCom->Set_TrackPosition(0.95);
				m_bLive = FALSE;

				if (m_uEliteID == Engine::Get_EliteWave())
				{
					Engine::Set_EliteWave();

					if(3 == m_uEliteID || 5 == m_uEliteID || 8 == m_uEliteID || 11 == m_uEliteID)
						static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_TipOneRender(TRUE);
				}
			}
			break;

		case ELITE_STATE::ELITE_DEAD:
			m_pMeshCom->Set_AnimationSet(84, 0.f, 1.f, 0.f, 0.01f);

			m_bLive = FALSE;

			if (m_uEliteID == Engine::Get_EliteWave())
			{
				Engine::Set_EliteWave();

				if (3 == m_uEliteID || 5 == m_uEliteID || 8 == m_uEliteID || 11 == m_uEliteID)
					static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_TipOneRender(TRUE);
			}
			break;

		case ELITE_STATE::ELITE_SLEEP_IN:
			m_pMeshCom->Set_AnimationSet(208);
			break;

		case ELITE_STATE::ELITE_SLEEP_LOOP:
			m_pMeshCom->Set_AnimationSet(207);
			break;

		case ELITE_STATE::ELITE_SLEEP_OUT:
			m_pMeshCom->Set_AnimationSet(206);
			break;

		case ELITE_STATE::ELITE_CLEAN_BROOM:
			m_pMeshCom->Set_AnimationSet(65);
			break;

		case ELITE_STATE::ELITE_CLEAN_BROOM_OUT:
			m_pMeshCom->Set_AnimationSet(62);
			break;

		case ELITE_STATE::ELITE_CLEAN_FLOOR:
			m_pMeshCom->Set_AnimationSet(68);
			break;

		case ELITE_STATE::ELITE_CLEAN_WALL:
			m_pMeshCom->Set_AnimationSet(70);
			break;

		case ELITE_STATE::ELITE_CLEAN_TABLE:
			if (76 != m_pMeshCom->Get_AnimationIndex())
				m_pMeshCom->Set_AnimationSet(74);
			else if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
			{
				if (74 == m_pMeshCom->Get_AnimationIndex())
					m_pMeshCom->Set_AnimationSet(76);
				else
					m_pMeshCom->Set_AnimationSet(74);
			}
			break;

		case ELITE_STATE::ELITE_JOKE:
			m_pMeshCom->Set_AnimationSet(90);
			// 90 129 130 131
			break;

		case ELITE_STATE::ELITE_DRINK_IN:
			m_pMeshCom->Set_AnimationSet(124);

			if (m_pMeshCom->Is_AnimationSetRateEnd(0.3))
			{
				for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_ITEM))
				{
					if (Engine::COL_OBJECTID::COL_OBJECT_CUP == iter->Get_ColObjectID())
					{
						static_cast<CFeature*>(iter)->Set_Hide(TRUE);
						m_pCup->Set_Drink(TRUE);
						break;
					}
				}
			}
			break;

		case ELITE_STATE::ELITE_DRINK_LOOP:
			m_pMeshCom->Set_AnimationSet(123);
			break;

		case ELITE_STATE::ELITE_DRINK_OUT:
			m_pMeshCom->Set_AnimationSet(122);
			break;

		case ELITE_STATE::ELITE_DRINK_DEAD:
			m_pMeshCom->Set_AnimationSet(79);

			if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
			{
				m_pMeshCom->Set_TrackPosition(0.95);
				m_bLive = FALSE;

				if (Get_EliteID() == Engine::Get_EliteWave())
					Engine::Set_EliteWave();
			}
			break;

		}
	}
	/*  Sword Active  */
	else if (TRUE == m_bSwordActive)
	{
		switch (m_eCurState)
		{

		case ELITE_STATE::ELITE_IDLE:
			//if (FALSE == m_bFightState)
			//{
			//	if (0 == m_uIdleOrder)
			//		m_pMeshCom->Set_AnimationSet(151);
			//	else if (1 == m_uIdleOrder)
			//		m_pMeshCom->Set_AnimationSet(152);
			//	else if (2 == m_uIdleOrder)
			//		m_pMeshCom->Set_AnimationSet(153);

			//	if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
			//		m_uIdleOrder++;

			//	if (3 <= m_uIdleOrder)
			//		m_uIdleOrder = 0;
			//}
			//else if (TRUE == m_bFightState)
			//{
			//	m_pMeshCom->Set_AnimationSet(139);
			//}

			if (TRUE == m_bFightState)
				m_pMeshCom->Set_AnimationSet(157);
			else
			{
				m_pMeshCom->Set_AnimationSet(139);

				if (m_pMeshCom->Is_AnimationSetRateEnd(1.4))
				{
					m_bFightState = TRUE;
				}
			}
			break;

		case ELITE_STATE::ELITE_WALK:
			m_pMeshCom->Set_AnimationSet(136);
			break;

		case ELITE_STATE::ELITE_RUN:
			m_pMeshCom->Set_AnimationSet(141);
			break;

		case ELITE_STATE::ELITE_IDLE_TURN_L180:
			m_pMeshCom->Set_AnimationSet(149);
			break;

		case ELITE_STATE::ELITE_IDLE_TURN_L90:
			m_pMeshCom->Set_AnimationSet(150);
			break;

		case ELITE_STATE::ELITE_IDLE_TURN_R90:
			m_pMeshCom->Set_AnimationSet(148);
			break;

		case ELITE_STATE::ELITE_IDLE_TURN_R180:
			m_pMeshCom->Set_AnimationSet(147);
			break;

		case ELITE_STATE::ELITE_RUSH:
			m_pMeshCom->Set_AnimationSet(143);
			break;

		case ELITE_STATE::ELITE_ASSASS_IN:
			//m_pMeshCom->Set_AnimationSet(7);
			m_pMeshCom->Set_AnimationSet(3);

			m_bOnMoveWay = FALSE;

			if (FALSE == m_bDieSound)
			{
				m_bDieSound = TRUE;
				Engine::MyPlaySound(L"SilentKill.mp3", Engine::CHANNELID::SPARK);
			}
			break;

		case ELITE_STATE::ELITE_ASSASS_LOOP:
			//m_pMeshCom->Set_AnimationSet(6);
			m_pMeshCom->Set_AnimationSet(2);
			break;

		case ELITE_STATE::ELITE_ASSASS_OUT:
			//m_pMeshCom->Set_AnimationSet(4);
			m_pMeshCom->Set_AnimationSet(0);

			if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
			{
				//m_eCurState = ELITE_DEAD;
				m_pMeshCom->Set_TrackPosition(0.95);
				m_bLive = FALSE;

				if (Get_EliteID() == Engine::Get_EliteWave())
					Engine::Set_EliteWave();
			}

			if (TRUE == m_bFightStateSound)
			{
				Engine::StopSound(Engine::CHANNELID::BGM);
				Engine::PlayBGM(L"StageBGM.mp3");

				m_bFightStateSound = FALSE;
			}

			break;

		case ELITE_STATE::ELITE_SWORD_ACTIVEIN:
			m_pMeshCom->Set_AnimationSet(138, 1.f, 0.65f);

			if (5 != m_uEliteID)
			{
				if (m_pMeshCom->Is_AnimationSetRateEnd(0.25))
					m_pSword->Set_Born(CSword::BORN::FINGER);
			}

			if (FALSE == m_bFightStateSound)
			{
				Engine::StopSound(Engine::CHANNELID::BGM);
				Engine::PlayBGM(L"StageFightBGM.mp3");
				m_bFightStateSound = TRUE;
			}

			break;

		case ELITE_STATE::ELITE_SWORD_ACTIVEOUT:
			m_pMeshCom->Set_AnimationSet(140);

			if (m_pMeshCom->Is_AnimationSetRateEnd(0.65))
			{
				m_pSword->Set_Born(CSword::BORN::SHEATH);
				Set_SwordActive(FALSE);
			}
			break;

		case ELITE_STATE::ELITE_ATTACK_UP:
			m_pMeshCom->Set_AnimationSet(144);
			break;

		case ELITE_STATE::ELITE_ATTACK_RIGHT:
			m_pMeshCom->Set_AnimationSet(145);
			break;

		case ELITE_STATE::ELITE_ATTACK_LEFT:
			m_pMeshCom->Set_AnimationSet(146);
			break;

		case ELITE_STATE::ELITE_ATTACK_COUNTER:
			m_pMeshCom->Set_AnimationSet(134);
			break;

		case ELITE_STATE::ELITE_TRUNK_IN:
			m_pMeshCom->Set_AnimationSet(164);
			break;

		case ELITE_STATE::ELITE_TRUNK_CATCH:
			m_pMeshCom->Set_AnimationSet(165);

			if (m_pMeshCom->Is_AnimationSetRateEnd(0.2))
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_TRUNK_CATCH);
				m_pPlayerMeshCom->Set_AnimationSet(246);
				m_pPlayer->Get_PlayerBehavior()->Set_OnePlayAni(9);

				Sink_PlayerCatch();
			}

			if (!(m_pPlayerMeshCom->Is_AnimationSetRateEnd(0.5)))
			{
				m_pPlayerTransCom->m_vInfo[Engine::INFO_POS].y += 0.7f *m_fTimeDelta;
				m_pPlayer->Set_NewPos(m_pPlayerTransCom->m_vInfo[Engine::INFO_POS]);
				m_pPlayer->Get_PlayerBehavior()->Set_NewPlayerPos(m_pPlayerTransCom->m_vInfo[Engine::INFO_POS]);

				m_pPlayer->StaticTransform_Update();
			}
			break;

		case ELITE_STATE::ELITE_CUPBOARD_IN:
			m_pMeshCom->Set_AnimationSet(179);

			break;

		case ELITE_STATE::ELITE_CUPBOARD_OUT:
			m_pMeshCom->Set_AnimationSet(178);
			break;

		case ELITE_STATE::ELITE_CUPBOARD_CATCH:
			m_pMeshCom->Set_AnimationSet(180);

			if (m_pMeshCom->Is_AnimationSetRateEnd(0.2))
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CUPBOARD_CATCH);
				m_pPlayerMeshCom->Set_AnimationSet(252);
				m_pPlayer->Get_PlayerBehavior()->Set_OnePlayAni(9);

				Sink_PlayerCatch();
			}

			if (!(m_pPlayerMeshCom->Is_AnimationSetRateEnd(0.5)))
			{
				m_pPlayerTransCom->m_vInfo[Engine::INFO_POS].y += 0.7f *m_fTimeDelta;
				m_pPlayer->Set_NewPos(m_pPlayerTransCom->m_vInfo[Engine::INFO_POS]);
				m_pPlayer->Get_PlayerBehavior()->Set_NewPlayerPos(m_pPlayerTransCom->m_vInfo[Engine::INFO_POS]);

				m_pPlayer->StaticTransform_Update();
			}
			break;

		case ELITE_STATE::ELITE_CATCH_ATTACK:
			m_pMeshCom->Set_AnimationSet(174);

			if (!(m_pPlayerMeshCom->Is_AnimationSetRateEnd(0.5)))
			{
				D3DXVec3Normalize(&m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK], &m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK]);
				_vec3 vNewPos = m_pPlayer->Get_NewPos();
				m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] = vNewPos += m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK] * 2.1f * m_fTimeDelta;
				m_pPlayer->Set_NewPos(vNewPos);
				m_pPlayer->Get_PlayerBehavior()->Set_NewPlayerPos(m_vNewPos);
			}

			if (m_fPlayerOriginCatchY < m_pPlayerTransCom->m_vInfo[Engine::INFO_POS].y)
			{
				m_pPlayerTransCom->m_vInfo[Engine::INFO_POS].y -= 0.7f * m_fTimeDelta;
				m_pPlayer->Set_NewPos(m_pPlayerTransCom->m_vInfo[Engine::INFO_POS]);
				m_pPlayer->Get_PlayerBehavior()->Set_NewPlayerPos(m_pPlayerTransCom->m_vInfo[Engine::INFO_POS]);
			}
			else
				m_pPlayerTransCom->m_vInfo[Engine::INFO_POS].y = m_fPlayerOriginCatchY;

			m_pPlayer->StaticTransform_Update();

			break;

		case ELITE_STATE::ELITE_LEVER_ON:
			// 131 135 
			m_pMeshCom->Set_AnimationSet(135);
			break;
		}
	}
}