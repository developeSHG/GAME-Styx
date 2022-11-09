void CPlayer::Update_State(void)
{
	if (m_eNextState != m_eCurState)
	{
		m_eCurState = m_eNextState;
	}

	switch (m_eCurState)
	{
	case PLAYER_STATE::PLAYER_IDLE:
		static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX += 0.3f * m_fTimeDelta;

		/*  Sleep  */
		if (185 == m_pMeshCom->Get_AnimationIndex())
			Engine::MyPlaySound(L"Alwayskeeptotheshadows.mp3", Engine::CHANNELID::PLAYER);

		/*  Down  */
		if (189 == m_pMeshCom->Get_AnimationIndex()
			|| 95 == m_pMeshCom_Elite->Get_AnimationIndex())
			Engine::MyPlaySound(L"JumpEnd.wav", Engine::CHANNELID::JUMP);

		if (FALSE == m_bSneakState)
		{
			/*  Down  */
			if (189 == m_pMeshCom->Get_AnimationIndex())
			{                                   
				if (TRUE == m_pMeshCom->Is_AnimationSetEnd(2.5f))
					m_pMeshCom->Set_AnimationSet(217, 1.f, 1.f, 0.005f);
			}
			/*  Door_Open  */
			else if (155 == m_pMeshCom->Get_AnimationIndex())
			{
				m_pMeshCom->Set_AnimationSet(217, 0.f, 1.f, 0.f, 0.1f);
			}
			/*  Trunk Out  */
			else if (256 == m_pMeshCom->Get_AnimationIndex())
			{
				m_pMeshCom->Set_AnimationSet(217, 0.f, 1.f, 0.f, 0.01f);
			}
			/*  Torch_On  */
			else if (141 == m_pMeshCom->Get_AnimationIndex())
			{
				m_pMeshCom->Set_AnimationSet(217, 0.f, 1.f, 0.f, 0.1f);
			}
			/*  Sleep  */
			else if (185 == m_pMeshCom->Get_AnimationIndex())
			{
				if (TRUE == m_pMeshCom->Is_AnimationSetEnd(2.5f))
					m_pMeshCom->Set_AnimationSet(217, 0.1f, 1.f, 0.f, 0.6f);
			}
			else
			{
				m_pMeshCom->Set_AnimationSet(217);
			}
		}
		else if (TRUE == m_bSneakState)
		{
			/*  Basic Sit  */
			//m_pMeshCom->Set_AnimationSet(180);

			/*  Assass Out  */
			//if (18 == m_pMeshCom->Get_AnimationIndex())
			//{
			//	m_pMeshCom->Set_TrackPosition(0.9);
			//	//m_pMeshCom->Set_AnimationSet(180, 0.6f, 1.f, 0.3f, 0.7f);
			//	m_pMeshCom->Set_AnimationSet(180);
			//}
			//else
				m_pMeshCom->Set_AnimationSet(180);
		}
		if (TRUE == m_bEliteState
			&& FALSE == m_bTranslucent)
		{
			if(FALSE == m_bEliteFightState)
				m_pMeshCom_Elite->Set_AnimationSet(106);
			else
				m_pMeshCom_Elite->Set_AnimationSet(157);
		}

		break;

	case PLAYER_STATE::PLAYER_WALK:
		static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX += 0.3f * m_fTimeDelta;

		/*  Sleep  */
		if (185 == m_pMeshCom->Get_AnimationIndex())
			Engine::MyPlaySound(L"Alwayskeeptotheshadows.mp3", Engine::CHANNELID::PLAYER);
		
		/*  Down  */
		if (189 == m_pMeshCom->Get_AnimationIndex()
			|| 95 == m_pMeshCom_Elite->Get_AnimationIndex())
			Engine::MyPlaySound(L"JumpEnd.wav", Engine::CHANNELID::JUMP);

		if (TRUE == m_pPlayerBehavior->Get_Moving())
		{
			if (FALSE == m_bSneakState)
			{
				m_pMeshCom->Set_AnimationSet(211);
			}
			else
			{
				m_pMeshCom->Set_AnimationSet(172, 1.f, 1.2f);

				static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(TRUE);
				static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Index(3);
			}
		}
		/*  Down  */
		if (189 == m_pMeshCom->Get_AnimationIndex())
		{
			if (TRUE == m_pMeshCom->Is_AnimationSetEnd(2.5f))
				m_pMeshCom->Set_AnimationSet(211, 1.f, 1.f, 0.005f);
		}
		if (TRUE == m_bEliteState
			&& FALSE == m_bTranslucent)
		{
			if (FALSE == m_bEliteFightState)
				m_pMeshCom_Elite->Set_AnimationSet(89);
			else
				m_pMeshCom_Elite->Set_AnimationSet(136);
		}

		break;

	case PLAYER_STATE::PLAYER_RUN:
		if (189 == m_pMeshCom->Get_AnimationIndex()
			|| 95 == m_pMeshCom_Elite->Get_AnimationIndex())
			Engine::MyPlaySound(L"JumpEnd.wav", Engine::CHANNELID::JUMP);

		/*  Down  */
		if (189 == m_pMeshCom->Get_AnimationIndex())
		{
			if (TRUE == m_pMeshCom->Is_AnimationSetEnd(2.5f))
				m_pMeshCom->Set_AnimationSet(212, 1.f, 1.f, 0.005f);
		}
		else if (TRUE == m_pPlayerBehavior->Get_Moving() && Engine::Get_DIKeyPressing(DIK_LSHIFT))
			m_pMeshCom->Set_AnimationSet(212);

		if (TRUE == m_bEliteState
			&& FALSE == m_bTranslucent)
		{
			if (FALSE == m_bEliteFightState)
				m_pMeshCom_Elite->Set_AnimationSet(92);
			else
				m_pMeshCom_Elite->Set_AnimationSet(141);
		}
		break;

	case PLAYER_STATE::PLAYER_JUMP:
		m_pMeshCom->Set_AnimationSet(187, 1.f, 0.75f);

		if (TRUE == m_bEliteState
			&& FALSE == m_bTranslucent)
			m_pMeshCom_Elite->Set_AnimationSet(95);
		break;

	case PLAYER_STATE::PLAYER_DOWN:
		if (m_pMeshCom->Is_AnimationSetEnd(0.35f))
		{
			//m_pMeshCom->Set_CurAnimationPos(0.f);
			m_pMeshCom->Set_AnimationSet(189, 1.f, 0.75f);
		}

		if (TRUE == m_bEliteState
			&& FALSE == m_bTranslucent)
			m_pMeshCom_Elite->Set_AnimationSet(96);
		break;

	case PLAYER_STATE::PLAYER_IDLE_TURN_L180:
		m_pMeshCom->Set_AnimationSet(215);
		break;

	case PLAYER_STATE::PLAYER_IDLE_TURN_L90:
		m_pMeshCom->Set_AnimationSet(216);
		break;

	case PLAYER_STATE::PLAYER_IDLE_TURN_R90:
		m_pMeshCom->Set_AnimationSet(214);
		break;

	case PLAYER_STATE::PLAYER_IDLE_TURN_R180:
		m_pMeshCom->Set_AnimationSet(213);
		break;

	case PLAYER_STATE::PLAYER_DOOR_IN:
		m_pMeshCom->Set_AnimationSet(154);
		break;

	case PLAYER_STATE::PLAYER_DOOR_LOOP:
		m_pMeshCom->Set_AnimationSet(153, 0.f, 1.f, 0.f, 0.1f);
		break;

	case PLAYER_STATE::PLAYER_DOOR_OUT:
		m_pMeshCom->Set_AnimationSet(152);
		break;

	case PLAYER_STATE::PLAYER_DOOR_OPEN:
		if (0 == static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemNum(5))
			m_pMeshCom->Set_AnimationSet(151);
		else if (1 == static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemNum(5))
			m_pMeshCom->Set_AnimationSet(155);
		break;

	case PLAYER_STATE::PLAYER_TRUNK_IN:
		m_pMeshCom->Set_AnimationSet(257);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_pMeshCom->Set_TrackPosition(0.95);
		}
		break;

	case PLAYER_STATE::PLAYER_TRUNK_OUT:
		m_pMeshCom->Set_AnimationSet(256);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
		{
			m_eCurState = PLAYER_STATE::PLAYER_IDLE;
			m_eNextState = PLAYER_STATE::PLAYER_IDLE;
			
			m_pMeshCom->Set_TrackPosition(0);
			m_pMeshCom->Set_AnimationSet(217, 0.f, 1.f, 0.f, 0.01f);
		}
		break;

	case PLAYER_STATE::PLAYER_CUPBOARD_IN:
		m_pMeshCom->Set_AnimationSet(265);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
			m_pMeshCom->Set_TrackPosition(0.95);
		break;

	case PLAYER_STATE::PLAYER_CUPBOARD_OUT:
		m_pMeshCom->Set_AnimationSet(264);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
		{
			m_eCurState = PLAYER_STATE::PLAYER_IDLE;
			m_eNextState = PLAYER_STATE::PLAYER_IDLE;

			m_pMeshCom->Set_TrackPosition(0);
			m_pMeshCom->Set_AnimationSet(217, 0.f, 1.f, 0.f, 0.01f);
		}
		break;

	case PLAYER_STATE::PLAYER_TORCH_ON:
		m_pMeshCom->Set_AnimationSet(141);
		break;

	case PLAYER_STATE::PLAYER_LEVER_ON:
		m_pMeshCom->Set_AnimationSet(159);
		break;

	case PLAYER_STATE::PLAYER_TRUNK_CATCH:
		m_pMeshCom->Set_AnimationSet(246);
		break;

	case PLAYER_STATE::PLAYER_CUPBOARD_CATCH:
		m_pMeshCom->Set_AnimationSet(252);
		break;

	case PLAYER_STATE::PLAYER_CATCH_HURT:
		m_pMeshCom->Set_AnimationSet(251);
		break;

	case PLAYER_STATE::PLAYER_ROLL:
		m_pMeshCom->Set_AnimationSet(181, 1.f, 1.3f, 0.1f);

		//if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
		//{
		//	if (TRUE == m_pPlayerBehavior->Get_Moving())
		//	{
		//		if (FALSE == m_bSneakState)
		//			m_pMeshCom->Set_AnimationSet(211);
		//		else
		//			m_pMeshCom->Set_AnimationSet(172);
		//	}
		//	else
		//		m_eNextState = PLAYER_STATE::PLAYER_IDLE;
		//}
		break;

	case PLAYER_STATE::PLAYER_SIT:
		m_pMeshCom->Set_AnimationSet(180);
		break;

	case PLAYER_STATE::PLAYER_DROPIN:
		if (TRUE == m_bSneakState
			|| TRUE == m_bSitState)
			m_pMeshCom->Set_AnimationSet(137, 1.f, 0.5f);
		else
			m_pMeshCom->Set_AnimationSet(136);
		break;

	case PLAYER_STATE::PLAYER_DROPOUT:
		if (TRUE == m_bSneakState
			|| TRUE == m_bSitState)
			m_pMeshCom->Set_AnimationSet(146);
		else
			m_pMeshCom->Set_AnimationSet(145);

		if (TRUE == m_bSneakState && m_pMeshCom->Is_AnimationSetRateEnd(0.6)
			|| TRUE == m_bSitState && m_pMeshCom->Is_AnimationSetRateEnd(0.6))
			m_pPlayerBehavior->Update_DropOut();
		else if (FALSE == m_bSneakState && FALSE == m_bSitState
				&& m_pMeshCom->Is_AnimationSetRateEnd(0.6))
			m_pPlayerBehavior->Update_DropOut();
		break;

	case PLAYER_STATE::PLAYER_FIGHT_IDLE:
		static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX += 0.3f * m_fTimeDelta;

		/*  Fight Counter  */
		if (120 == m_pMeshCom->Get_AnimationIndex())
			m_pMeshCom->Set_AnimationSet(127, 0.3f, 1.f, 0.01f);
		else
			m_pMeshCom->Set_AnimationSet(127);
		break;

	case PLAYER_STATE::PLAYER_FIGHT_LEFT:
		m_pMeshCom->Set_AnimationSet(126);
		break;

	case PLAYER_STATE::PLAYER_FIGHT_RIGHT:
		m_pMeshCom->Set_AnimationSet(125);
		break;

	case PLAYER_STATE::PLAYER_FIGHT_FORWARD:
		m_pMeshCom->Set_AnimationSet(128);
		break;

	case PLAYER_STATE::PLAYER_FIGHT_BACKWARD:
		m_pMeshCom->Set_AnimationSet(129);
		break;

	case PLAYER_STATE::PLAYER_FIGHT_COUNTER:
		m_pMeshCom->Set_AnimationSet(120);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
			m_eNextState = PLAYER_STATE::PLAYER_FIGHT_IDLE;

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.5))
			g_bSlowActive = FALSE;
		else if (m_pMeshCom->Is_AnimationSetRateEnd(0.15))
		{
			g_bSlowActive = TRUE;
			m_bSparkRender = TRUE;
		}
		break;

	case PLAYER_STATE::PLAYER_FIGHT_HURT:
		m_pMeshCom->Set_AnimationSet(105);
		
		if (m_pMeshCom->Is_AnimationSetRateEnd(0.9))
		{
			m_pMeshCom->Set_TrackPosition(0.85);
			m_eNextState = PLAYER_STATE::PLAYER_FIGHT_IDLE;
		}
		//m_pPlayerBehavior->Set_OnePlayAni(0.95);
		break;

	case PLAYER_STATE::PLAYER_FIND:
		m_pMeshCom->Set_AnimationSet(175);
		break;

	case PLAYER_STATE::PLAYER_CHEER:
		m_pMeshCom->Set_AnimationSet(178);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
		{
			m_eNextState = PLAYER_STATE::PLAYER_IDLE;
			static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CheerState(FALSE);
		}
		break;

	case PLAYER_STATE::PLAYER_SLEEP:
		m_pMeshCom->Set_AnimationSet(185);
		break;

	case PLAYER_STATE::PLAYER_CARRY_PICK:
		m_pMeshCom->Set_AnimationSet(66);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_pMeshCom->Set_TrackPosition(0.8);
			m_eNextState = PLAYER_STATE::PLAYER_CARRY_IDLE;
			static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CheerState(FALSE);
		}
		break;

	case PLAYER_STATE::PLAYER_CARRY_IDLE:
		static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX += 0.3f * m_fTimeDelta;

		m_pMeshCom->Set_AnimationSet(64);

		if (nullptr != m_pCollToElite)
			m_pCollToElite->Set_CurState(ELITE_STATE::ELITE_CARRY_IDLE);
		break;

	case PLAYER_STATE::PLAYER_CARRY_WALK:
		static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX += 0.3f * m_fTimeDelta;

		m_pMeshCom->Set_AnimationSet(62);

		if (nullptr != m_pCollToElite)
			m_pCollToElite->Set_CurState(ELITE_STATE::ELITE_CARRY_WALK);
		break;

	case PLAYER_STATE::PLAYER_CARRY_RUN:
		m_pMeshCom->Set_AnimationSet(63);

		if (nullptr != m_pCollToElite)
			m_pCollToElite->Set_CurState(ELITE_STATE::ELITE_CARRY_RUN);
		break;

	case PLAYER_STATE::PLAYER_CARRY_DROP:
		m_pMeshCom->Set_AnimationSet(72);
		break;

	case PLAYER_STATE::PLAYER_ELITE_HIDE_TRUNK:
		m_pMeshCom->Set_AnimationSet(58);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_pMeshCom->Set_TrackPosition(0.85);
			m_eNextState = PLAYER_STATE::PLAYER_IDLE;
			m_bCarryState = FALSE;
		}
		break;

	case PLAYER_STATE::PLAYER_ELITE_HIDE_CUPBOARD:
		m_pMeshCom->Set_AnimationSet(60);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_pMeshCom->Set_TrackPosition(0.85);
			m_eNextState = PLAYER_STATE::PLAYER_IDLE;
			m_bCarryState = FALSE;
		}
		break;

	case PLAYER_STATE::PLAYER_ASSASS_IN:
		m_pPlayerBehavior->Set_OnePlayAni(9);
		static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(TRUE);
		static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Index(3);

		if (FALSE == m_bFightState)
			m_pMeshCom->Set_AnimationSet(21);
		else if (TRUE == m_bFightState)
			//m_pMeshCom->Set_AnimationSet(8);
			m_pMeshCom->Set_AnimationSet(4);

		m_pCollToElite->Set_CurState(ELITE_STATE::ELITE_ASSASS_IN);
		
		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_eNextState = PLAYER_ASSASS_LOOP;
			if (FALSE == m_bFightState)
				m_vNewPos = m_pTransCom->m_vInfo[Engine::INFO_POS] + m_pTransCom->m_vInfo[Engine::INFO_LOOK] * 77.f;
			else if (TRUE == m_bFightState)
				m_vNewPos = m_pTransCom->m_vInfo[Engine::INFO_POS] + m_pTransCom->m_vInfo[Engine::INFO_LOOK] * 130.f;

			m_pPlayerBehavior->Set_NewPlayerPos(m_vNewPos);
			m_pTransCom->Set_Pos(&m_vNewPos);
			StaticTransform_Update();
			m_pMeshCom->Set_TrackPosition(0.95);

			m_pCollToElite->Set_CurState(ELITE_STATE::ELITE_ASSASS_LOOP);
			m_pCollToElite->Set_TrackPosition(0.85);

			if (FALSE == m_bFightState)
				m_pMeshCom->Set_AnimationSet(20, 0.f, 1.f, 0.f, 0.01f);
			else if (TRUE == m_bFightState)
				//m_pMeshCom->Set_AnimationSet(7);
				m_pMeshCom->Set_AnimationSet(3, 0.f, 1.f, 0.f, 0.01f);
		}
		break;

	case PLAYER_STATE::PLAYER_ASSASS_LOOP:
		m_pPlayerBehavior->Set_OnePlayAni(9);
		static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(TRUE);
		static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Index(3);

		if(FALSE == m_bFightState)
			m_pMeshCom->Set_AnimationSet(20, 0.f, 1.f, 0.f, 0.01f);
		else if(TRUE == m_bFightState)
			//m_pMeshCom->Set_AnimationSet(7);
			m_pMeshCom->Set_AnimationSet(3, 0.f, 1.f, 0.f, 0.01f);

		m_pCollToElite->Set_CurState(ELITE_STATE::ELITE_ASSASS_LOOP);
		
		if (m_pMeshCom->Is_AnimationSetRateEnd(0.5))
		{
			m_eNextState = PLAYER_ASSASS_OUT;

			m_pCollToElite->Set_CurState(ELITE_STATE::ELITE_ASSASS_OUT);

			//if (FALSE == m_bFightState)
				Engine::MyPlaySound(L"Assass.wav", Engine::CHANNELID::SPARK);
		}
		break;

	case PLAYER_STATE::PLAYER_ASSASS_OUT:
		m_pPlayerBehavior->Set_OnePlayAni(9);
		static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(TRUE);
		static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Index(3);

		if (FALSE == m_bFightState)
			m_pMeshCom->Set_AnimationSet(18);
		else if (TRUE == m_bFightState)
			//m_pMeshCom->Set_AnimationSet(5);
			m_pMeshCom->Set_AnimationSet(1);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			Set_NextState(PLAYER_STATE::PLAYER_IDLE);
			m_pMeshCom->Set_TrackPosition(0.85);
			m_pPlayerBehavior->Set_OnePlayAniFalse();

			if(FALSE == m_bFightState)
				m_pMeshCom->Set_AnimationSet(180, 1.f, 1.f, 0.1f, 0.5f);
			else
				m_pMeshCom->Set_AnimationSet(180);

			static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(FALSE);
		}
		break;

	case PLAYER_STATE::PLAYER_SHAKE_POISON:
		m_pMeshCom->Set_AnimationSet(145);
		break;

	case PLAYER_STATE::PLAYER_THROW_CUTLASS:
		m_pMeshCom->Set_AnimationSet(140);

		static_cast<CCutlass*>(Engine::Find_ObjectListFront(Engine::OBJECT_CUTLASS))->Set_Render(TRUE);
		if (m_pMeshCom->Is_AnimationSetRateEnd(0.4))
		{
			if(FALSE == static_cast<CCutlass*>(Engine::Find_ObjectListFront(Engine::OBJECT_CUTLASS))->Get_Throw())
				static_cast<CCutlass*>(Engine::Find_ObjectListFront(Engine::OBJECT_CUTLASS))->Set_Throw(TRUE);
		}
		break;

	case PLAYER_STATE::PLAYER_DRINK_POSITION:
		if (FALSE == m_bEliteState)
			m_pMeshCom->Set_AnimationSet(182);
		else if (TRUE == m_bEliteState
			&& FALSE == m_bTranslucent)
			m_pMeshCom_Elite->Set_AnimationSet(125);

		break;

	case PLAYER_STATE::PLAYER_DRINK_AFTER:
		m_pMeshCom->Set_AnimationSet(217);
		break;

	case PLAYER_STATE::PLAYER_MAKE_SOUND:
		m_pMeshCom->Set_AnimationSet(186);
		// L Sound 87
		break;

	case PLAYER_STATE::PLAYER_SICK_LOOP:
		if (TRUE == m_bEliteState
			&& FALSE == m_bTranslucent)
		{
			m_pMeshCom_Elite->Set_AnimationSet(214);

			/*	if (m_pMeshCom_Elite->Is_AnimationSetRateEnd(0.95))
				{
					m_eNextState = PLAYER_STATE::PLAYER_SICK_OUT;
					m_pPlayerBehavior->Set_OnePlayAni(9);
				}*/
		}
		break;

	case PLAYER_STATE::PLAYER_SICK_OUT:
		if (TRUE == m_bEliteState
			&& FALSE == m_bTranslucent)
		{
			m_pMeshCom_Elite->Set_AnimationSet(213);

		/*	if (m_pMeshCom_Elite->Is_AnimationSetRateEnd(0.95))
			{
				m_eNextState = PLAYER_STATE::PLAYER_IDLE;
				m_pPlayerBehavior->Set_OnePlayAniFalse();
			}*/
		}
		break;

	case PLAYER_STATE::PLAYER_TRANSLUCENT:
		m_pMeshCom->Set_AnimationSet(266);
		break;

	case PLAYER_STATE::PLAYER_DEAD_IN:
		m_pMeshCom->Set_AnimationSet(112);

		m_bDeadState = TRUE;

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
		{
			m_eNextState = PLAYER_STATE::PLAYER_DEAD;
			m_pMeshCom->Set_TrackPosition(0.98);
			m_pMeshCom->Set_AnimationSet(111, 0.f, 1.f, 0.f, 0.01f);
		}
		break;

	case PLAYER_STATE::PLAYER_SWORD_ACTIVEIN:
		if (TRUE == m_bEliteState)
		{
			m_pMeshCom_Elite->Set_AnimationSet(138, 1.f, 0.65f);

			if (m_pMeshCom_Elite->Is_AnimationSetRateEnd(0.25))
				m_pPlayerSword->Set_Born(CPlayerSword::BORN::FINGER);
		}
		break;

	case PLAYER_STATE::PLAYER_SWORD_ACTIVEOUT:
		if (TRUE == m_bEliteState)
		{
			m_pMeshCom_Elite->Set_AnimationSet(140);

			if (m_pMeshCom_Elite->Is_AnimationSetRateEnd(0.65))
			{
				m_pPlayerSword->Set_Born(CPlayerSword::BORN::SHEATH);
			}
		}
		break;

	case PLAYER_STATE::PLAYER_ATTACK_UP:
		if (TRUE == m_bEliteState)
		{
			m_pMeshCom_Elite->Set_AnimationSet(144);

			if (m_pMeshCom->Is_AnimationSetRateEnd(0.4))
				g_bSlowActive = FALSE;
			else if (m_pMeshCom->Is_AnimationSetRateEnd(0.25))
				g_bSlowActive = TRUE;

			if (m_pMeshCom_Elite->Is_AnimationSetRateEnd(0.45))
			{
				m_pCollToElite->Set_CurState(ELITE_STATE::ELITE_CUTLASS_DEAD);
				m_pCollToElite->Get_EliteBehavior()->Set_OnePlayAni(0.85);
			}
		}
		break;

	case PLAYER_STATE::PLAYER_DEAD:
		m_pMeshCom->Set_AnimationSet(111, 0.f, 1.f, 0.f, 0.01f);

		m_bDeadState = TRUE;

		static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_DeadState(TRUE);

		//if(m_pMeshCom->Is_AnimationSetRateEnd(1))
			static_cast<CSceneAlpha*>(Engine::Find_ObjectListBack(Engine::OBJECT_SCENEALPHA))->Set_Fade(FALSE);
			static_cast<CSceneAlpha*>(Engine::Find_ObjectListBack(Engine::OBJECT_SCENEALPHA))->Set_Dead(TRUE);
		//else
			//static_cast<CSceneAlpha*>(Engine::Find_ObjectListBack(Engine::OBJECT_SCENEALPHA))->Set_Fade(TRUE);

		break;

	}

	
	/*  Elite Finish  */
	if (FALSE == m_bEliteState
		&& TRUE == m_bStyxEliteRender
		&& FALSE == m_bTranslucent)
	{
		// 81
		if (80 == m_pMeshCom_Elite->Get_AnimationIndex())
		{
			if(m_pMeshCom_Elite->Is_AnimationSetRateEnd(0.98))
				m_pMeshCom_Elite->Set_AnimationSet(84, 0.f, 1.f, 0.f, 0.01f);
		}
	}
}