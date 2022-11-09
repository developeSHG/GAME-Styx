void CPlayerBehavior::FightStateKeyInput(const Engine::_float& fDeltaTime)
{
	/*  Fight  */
	if (Engine::Get_DIKeyDown(DIK_CAPSLOCK)
		&& FALSE == m_pPlayer->Get_CarryState())
	{
		if (FALSE == m_pPlayer->Get_EliteState())
		{
			if (PLAYER_STATE::PLAYER_FIGHT_IDLE != m_pPlayer->Get_CurState())
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_IDLE);
				static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_FightState(TRUE);
				m_pPlayer->Set_FightState(TRUE);
			}
			else
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
				static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_FightState(FALSE);
				m_pPlayer->Set_FightState(FALSE);
			}
		}
		else
		{
			if (FALSE == m_pPlayer->Get_EliteFightState())
			{
				m_pPlayer->Set_EliteFightState(TRUE);

				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_SWORD_ACTIVEIN);
				Set_OnePlayAni(0.85);
				static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_FightState(TRUE);
			}
			else
			{
				m_pPlayer->Set_EliteFightState(FALSE);

				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_SWORD_ACTIVEOUT);
				Set_OnePlayAni(0.85);
				static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_FightState(FALSE);
			}
		}

	}

	if (TRUE == m_bTurn
		|| TRUE == m_bOnePlayAnimation
		|| TRUE == m_bSinkTime
		|| FALSE == m_pPlayer->Get_FightState()
		/*|| TRUE == g_bSlowActive
		|| PLAYER_STATE::PLAYER_FIGHT_COUNTER == m_pPlayer->Get_CurState()*/)
		return;

	if (true == m_bRunSound)
	{
		Engine::StopSound(Engine::CHANNELID::RUN);
		m_bRunSound = false;
		m_fRunSound = 0.f;
	}

	Camera_Setting(fDeltaTime);

	/*  StaticCamera OwnerShip TRUE  */
	if (TRUE == m_pStaticCamera->Get_OwnerShip())
	{
		/*  Moving  */
		{
			if (Engine::Get_DIKeyPressing(DIK_W) && Engine::Get_DIKeyPressing(DIK_D))
			{
				Update_WalkRightFront(fDeltaTime);
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_FORWARD);

				if (false == m_bRunSound)
				{
					Engine::MyPlaySound(L"Run.mp3", Engine::CHANNELID::RUN);
					m_bRunSound = true;
				}
			}
			else if (Engine::Get_DIKeyPressing(DIK_W) && Engine::Get_DIKeyPressing(DIK_A))
			{
				Update_WalkLeftFront(fDeltaTime);
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_FORWARD);

				if (false == m_bRunSound)
				{
					Engine::MyPlaySound(L"Run.mp3", Engine::CHANNELID::RUN);
					m_bRunSound = true;
				}
			}
			else if (Engine::Get_DIKeyPressing(DIK_S) && Engine::Get_DIKeyPressing(DIK_D))
			{
				Update_WalkRightBack(fDeltaTime);
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_BACKWARD);
			}
			else if (Engine::Get_DIKeyPressing(DIK_S) && Engine::Get_DIKeyPressing(DIK_A))
			{
				Update_WalkLeftBack(fDeltaTime);
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_BACKWARD);
			}
			else if (Engine::Get_DIKeyPressing(DIK_W))
			{
				Update_WalkFront(fDeltaTime);
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_FORWARD);
			}
			else if (Engine::Get_DIKeyPressing(DIK_S))
			{
				Update_WalkBack(fDeltaTime);
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_BACKWARD);
			}
			else if (Engine::Get_DIKeyPressing(DIK_D))
			{
				Update_WalkRight(fDeltaTime);
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_RIGHT);

				if (false == m_bRunSound)
				{
					Engine::MyPlaySound(L"Run.mp3", Engine::CHANNELID::RUN);
					m_bRunSound = true;
				}
			}
			else if (Engine::Get_DIKeyPressing(DIK_A))
			{
				Update_WalkLeft(fDeltaTime);
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_LEFT);

				if (false == m_bRunSound)
				{
					Engine::MyPlaySound(L"Run.mp3", Engine::CHANNELID::RUN);
					m_bRunSound = true;
				}
			}
			/*  No KeyInput  */
			else if(PLAYER_STATE::PLAYER_FIGHT_COUNTER != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_FIGHT_HURT != m_pPlayer->Get_CurState())
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_IDLE);

				/*  Moving Init  */
				m_bMoving = FALSE;
			}
		}
		/*if (Engine::Get_DIMouseDown(Engine::MOUSEKEYSTATE::DIM_LB)
			&& FALSE == m_bMoving
			&& PLAYER_STATE::PLAYER_FIGHT_COUNTER != m_pPlayer->Get_CurState())
		{
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_COUNTER);

		}*/
	}
}