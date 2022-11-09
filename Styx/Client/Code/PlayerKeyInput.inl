void CPlayerBehavior::KeyInput(const float & fDeltaTime)
{
	if (TRUE == m_bTurn
		|| TRUE == m_bOnePlayAnimation
		|| TRUE == m_bSinkTime
		|| TRUE == m_pPlayer->Get_FightState())
		return;

	Camera_Setting(fDeltaTime);

	/*  StaticCamera OwnerShip TRUE  */
	if (TRUE == m_pStaticCamera->Get_OwnerShip())
	{
		/*  Moving  */
		{
			if (Engine::Get_DIKeyPressing(DIK_W) && Engine::Get_DIKeyPressing(DIK_D))
				Update_WalkRightFront(fDeltaTime);
			else if (Engine::Get_DIKeyPressing(DIK_W) && Engine::Get_DIKeyPressing(DIK_A))
				Update_WalkLeftFront(fDeltaTime);
			else if (Engine::Get_DIKeyPressing(DIK_S) && Engine::Get_DIKeyPressing(DIK_D))
				Update_WalkRightBack(fDeltaTime);
			else if (Engine::Get_DIKeyPressing(DIK_S) && Engine::Get_DIKeyPressing(DIK_A))
				Update_WalkLeftBack(fDeltaTime);
			else if (Engine::Get_DIKeyPressing(DIK_W))
				Update_WalkFront(fDeltaTime);
			else if (Engine::Get_DIKeyPressing(DIK_S))
				Update_WalkBack(fDeltaTime);
			else if (Engine::Get_DIKeyPressing(DIK_D))
				Update_WalkRight(fDeltaTime);
			else if (Engine::Get_DIKeyPressing(DIK_A))
				Update_WalkLeft(fDeltaTime);
			else if (Engine::Get_DIKeyDown(DIK_0) && PLAYER_STATE::PLAYER_SLEEP == m_pPlayer->Get_CurState())
			{
				//Engine::MyPlaySound(L"Alwayskeeptotheshadows.mp3", Engine::CHANNELID::PLAYER);
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
			}
			/*  No KeyInput  */
			else if (FALSE == m_pPlayer->Get_CarryState())
			{
				if (PLAYER_STATE::PLAYER_JUMP != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_DOWN != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_SLEEP != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_SIT != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_CHEER != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_FIGHT_IDLE != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_TRUNK_IN != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_CUPBOARD_IN != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_TRUNK_OUT != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_CUPBOARD_OUT != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_DEAD_IN != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_DEAD != m_pPlayer->Get_CurState())
				{
					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);

					/*  Blind Render False  */
					if (CAMERA_PURPOSE::PURPOSE_FIRST_PERSON != static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Get_CameraPurpose())
						static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(FALSE);
				}

				/*  Moving Init  */
				m_bMoving = FALSE;

				if (true == m_bRunSound)
				{
					Engine::StopSound(Engine::CHANNELID::RUN);
					m_bRunSound = false;
					m_fRunSound = 0.f;
				}
			}
			else if (TRUE == m_pPlayer->Get_CarryState())
			{
				if (PLAYER_STATE::PLAYER_CARRY_PICK != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_CARRY_DROP != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_ELITE_HIDE_TRUNK != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_ELITE_HIDE_CUPBOARD != m_pPlayer->Get_CurState()
					&& PLAYER_STATE::PLAYER_CARRY_DROP != m_pPlayer->Get_CurState())
				{
					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CARRY_IDLE);
				}

				/*  Moving Init  */
				m_bMoving = FALSE;

				if (true == m_bRunSound)
				{
					Engine::StopSound(Engine::CHANNELID::RUN);
					m_bRunSound = false;
					m_fRunSound = 0.f;
				}
			}
		}

		/*  Roll  */
		if (Engine::Get_DIKeyDown(DIK_R)
			&& TRUE == m_bMoving)
		{
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_ROLL);
			Set_OnePlayAni(0.75);
			static_cast<CMp*>(Find_ObjectListFront(Engine::OBJECT_MP))->m_fuvX -= 0.15f;
		}

		/*  Sit  */
		if (Engine::Get_DIKeyDown(DIK_T) && PLAYER_STATE::PLAYER_IDLE == m_pPlayer->Get_CurState()
			|| Engine::Get_DIKeyDown(DIK_T) && PLAYER_STATE::PLAYER_SIT == m_pPlayer->Get_CurState())
		{
			if (PLAYER_STATE::PLAYER_SIT != m_pPlayer->Get_CurState())
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_SIT);
				m_pPlayer->Set_SitState(TRUE);
			}
			else
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
				m_pPlayer->Set_SitState(FALSE);
			}
		}

		/*  Item DropIn  */
		if (Engine::Get_DIKeyDown(DIK_E))
		{
			if (PLAYER_COL_MAP & m_pPlayer->Get_ColTo()
				|| PLAYER_COL_DAGGER & m_pPlayer->Get_ColTo()
				|| PLAYER_COL_KEY & m_pPlayer->Get_ColTo()
				|| PLAYER_COL_CUTLASS & m_pPlayer->Get_ColTo()
				|| PLAYER_COL_POSITION & m_pPlayer->Get_ColTo()
				|| PLAYER_COL_MYSTERY & m_pPlayer->Get_ColTo()
				|| PLAYER_COL_POISON & m_pPlayer->Get_ColTo())
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_DROPIN);
				Set_OnePlayAni(0.95);
				return;
			}
		}

		/*  Item DropOut  */
		if (Engine::Get_DIKeyDown(DIK_C)
			&& Engine::COL_FALSE == m_pPlayer->Get_IsCollide())
		{
			if (0 < static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_CurItemNum())
			{
				_ushort uItem = static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemSelect();

				/*  FirstPerosn Dagger End  */
				if (CAMERA_PURPOSE::PURPOSE_FIRST_PERSON == static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Get_CameraPurpose())
				{
					if (2 == uItem)
						return;
				}

				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_DROPOUT);
				Set_OnePlayAni(0.95);

				for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_ITEM))
				{
					if (FALSE == static_cast<CFeature*>(iter)->Get_Hide())
						continue;

					if (uItem == static_cast<CFeature*>(iter)->Get_ItemIndex())
					{
						m_pDropItem = static_cast<CFeature*>(iter);
						static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ITEMLIST))->Set_MiusCurItem();
						break;
					}
					/*switch (uItem)
					{
					case 0:
						break;
					case 1:
						break;
					case 2:
						if (Engine::COL_OBJECTID::COL_OBJECT_CUTLASS == iter->Get_ColObjectID())
							m_pDropItem = static_cast<CFeature*>(iter);
						break;
					case 3:
						break;
					case 4:
						if (Engine::COL_OBJECTID::COL_OBJECT_MAP == iter->Get_ColObjectID())
							m_pDropItem = static_cast<CFeature*>(iter);
						break;
					case 5:
						break;
					}*/
				}
			}
		}

		/*  Set SneakState  */
		if (Engine::Get_DIKeyDown(DIK_9))
		{
			if (FALSE == m_pPlayer->Get_SneakState())
				m_pPlayer->Set_SneakState(TRUE);
			else if (TRUE == m_pPlayer->Get_SneakState())
				m_pPlayer->Set_SneakState(FALSE);
		}

		/*  Jump  */
		if (Engine::Get_DIKeyDown(DIK_SPACE)
			&& PLAYER_STATE::PLAYER_JUMP != m_pPlayer->Get_CurState()
			&& PLAYER_STATE::PLAYER_DOWN != m_pPlayer->Get_CurState()
			&& FALSE == m_pPlayer->Get_CarryState()
			&& FALSE == m_pPlayer->Get_FightState()
			/*	&& TRUE == m_pPlayer->Get_IsOnLand()*/)
		{
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_JUMP);
		}

		/*  Turn  */
		{
			if (PLAYER_STATE::PLAYER_IDLE == m_pPlayer->Get_CurState()
				|| PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_CurState())
			{
				if (Engine::Get_DIKeyDown(DIK_1))
				{
					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE_TURN_L180);
					m_bTurn = TRUE;
					m_dOnePlayAnimationSetEnd = 0.98;
				}
				else if (Engine::Get_DIKeyDown(DIK_2))
				{
					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE_TURN_L90);
					m_bTurn = TRUE;
					m_dOnePlayAnimationSetEnd = 0.98;
				}
				else if (Engine::Get_DIKeyDown(DIK_3))
				{
					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE_TURN_R90);
					m_bTurn = TRUE;
					m_dOnePlayAnimationSetEnd = 0.98;
				}
				else if (Engine::Get_DIKeyDown(DIK_4))
				{
					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE_TURN_R180);
					m_bTurn = TRUE;
					m_dOnePlayAnimationSetEnd = 0.98;
				}
			}
		}

		/*  Find  */
		if (45.f > m_pPlayerTransCom->m_vInfo[Engine::INFO::INFO_POS].z)
		{
			if (Engine::Get_DIKeyDown(DIK_F) && PLAYER_STATE::PLAYER_IDLE == m_pPlayer->Get_CurState()
				|| Engine::Get_DIKeyDown(DIK_F) && PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_CurState())
			{
				static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CheerState(TRUE);

				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIND);
				Set_OnePlayAni(0.98);

				if (64.f < m_pPlayerTransCom->m_vInfo[Engine::INFO::INFO_POS].x
					&& 42.f > m_pPlayerTransCom->m_vInfo[Engine::INFO::INFO_POS].z)
				{
					m_pPlayer->Set_CheerState(TRUE);
				}
			}
		}

		/*  MakeSound  */
		if (Engine::Get_DIMouseDown(Engine::DIM_LB)
			&& Engine::COL_FALSE == m_pPlayer->Get_IsCollide())
		{
			//if (TRUE == static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Get_MakeSoundActive())
			if(12 == Engine::Get_EliteWave()
				&& FALSE == static_cast<CElite*>(Engine::Find_ObjectListBack(Engine::OBJECT_ELITE))->Get_MakeSoundPattern())
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_MAKE_SOUND);
				Engine::MyPlaySound(L"MakeSound.mp3", Engine::CHANNELID::PLAYER);
				Set_OnePlayAni(0.9);
				static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_MakeSoundActive(FALSE);
				return;
			}
		}

	}

	/*  Cup ColTo  */
	if (PLAYER_COL_CUP & m_pPlayer->Get_ColTo())
	{
		if (Engine::Get_DIKeyDown(DIK_E) && PLAYER_STATE::PLAYER_IDLE == m_pPlayer->Get_CurState())
		{
			if (0 < static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_CurItemNum())
			{
				if (3 == static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemSelect())
				{
					m_bSinkTime = TRUE;
				}
			}
		}
	}

	/*  Door ColTo  */
	if (PLAYER_COL_DOOR & m_pPlayer->Get_ColTo())
	{
		if (Engine::Get_DIKeyDown(DIK_E) && PLAYER_STATE::PLAYER_IDLE == m_pPlayer->Get_CurState()
			|| Engine::Get_DIKeyDown(DIK_E) && PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_CurState())
		{
			//m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_DOOR_OPEN);
			//m_bOnePlayAnimation = TRUE;
			//m_dOnePlayAnimationSetEnd = 0.95;

			//if (0 == static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemNum(5))
			//	static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(0);
			//else if (1 == static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemNum(5))
			//	static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(2);

			m_bSinkTime = TRUE;
		}
		else if (Engine::Get_DIKeyDown(DIK_C) && PLAYER_STATE::PLAYER_IDLE == m_pPlayer->Get_CurState()
			|| Engine::Get_DIKeyDown(DIK_C) && PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_CurState()
			|| Engine::Get_DIKeyDown(DIK_C) && PLAYER_STATE::PLAYER_DOOR_LOOP == m_pPlayer->Get_CurState())
		{
			if (CAMERA_PURPOSE::PURPOSE_DOOR != m_pDynamicCamera->Get_CameraPurpose())
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_DOOR_IN);
				Set_OnePlayAni(0.95);
				g_bLightPointActive = TRUE;
			}
			else if (CAMERA_PURPOSE::PURPOSE_DOOR == m_pDynamicCamera->Get_CameraPurpose())
			{
				m_pDynamicCamera->Set_CameraPurposeStart(FALSE);
				m_pDynamicCamera->Set_CameraPurposeExit(TRUE);
			}
		}
	}

	/*  Trunk ColTo  */
	else if (PLAYER_COL_TRUNK & m_pPlayer->Get_ColTo())
	{
		if (FALSE == m_pPlayer->Get_CarryState())
		{
			if (Engine::Get_DIKeyDown(DIK_E) && PLAYER_STATE::PLAYER_IDLE == m_pPlayer->Get_CurState()
				|| Engine::Get_DIKeyDown(DIK_E) && PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_CurState()
				|| Engine::Get_DIKeyDown(DIK_E) && PLAYER_STATE::PLAYER_TRUNK_IN == m_pPlayer->Get_CurState())
			{
				Engine::MyPlaySound(L"Trunk.mp3", Engine::CHANNELID::TRUNK);

				if (CAMERA_PURPOSE::PURPOSE_TRUNK != m_pDynamicCamera->Get_CameraPurpose())
				{
					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_TRUNK_IN);
					Set_OnePlayAni(0.95);

					for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_TRUNK))
					{
						if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
						{
							static_cast<CTrunk*>(iter)->Set_State(CTrunk::STATE::PLAYER_IN);
							break;
						}
					}
				}
				else if (CAMERA_PURPOSE::PURPOSE_TRUNK == m_pDynamicCamera->Get_CameraPurpose())
				{
					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_TRUNK_OUT);
					m_pDynamicCamera->Set_CameraPurposeStart(FALSE);
					m_pDynamicCamera->Set_CameraPurposeExit(TRUE);

					for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_TRUNK))
					{
						if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
						{
							static_cast<CTrunk*>(iter)->Set_State(CTrunk::STATE::PLAYER_OUT);
							break;
						}
					}
				}
			}
		}
		else if (TRUE == m_pPlayer->Get_CarryState())
		{
			if (Engine::Get_DIMouseDown(Engine::DIM_LB) && PLAYER_STATE::PLAYER_CARRY_IDLE == m_pPlayer->Get_CurState()
				|| Engine::Get_DIMouseDown(Engine::DIM_LB) && PLAYER_STATE::PLAYER_CARRY_WALK == m_pPlayer->Get_CurState()
				|| Engine::Get_DIMouseDown(Engine::DIM_LB) && PLAYER_STATE::PLAYER_CARRY_RUN == m_pPlayer->Get_CurState())
			{
				Engine::MyPlaySound(L"Trunk.mp3", Engine::CHANNELID::TRUNK);

				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_ELITE_HIDE_TRUNK);
				m_pPlayer->Get_CollToElite()->Set_CurState(ELITE_STATE::ELITE_HIDE_TRUNK_IN);
				m_pPlayer->Get_CollToElite()->Set_CarryState(FALSE);
				Set_OnePlayAni(0.95);

				for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_TRUNK))
				{
					if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
					{
						static_cast<CTrunk*>(iter)->Set_State(CTrunk::STATE::ELITE_HIDE);
						break;
					}
				}
			}
		}
	}

	/*  Cupboard ColTo  */
	else if (PLAYER_COL_CUPBOARD & m_pPlayer->Get_ColTo())
	{
		if (FALSE == m_pPlayer->Get_CarryState())
		{
			if (Engine::Get_DIKeyDown(DIK_E) && PLAYER_STATE::PLAYER_IDLE == m_pPlayer->Get_CurState()
				|| Engine::Get_DIKeyDown(DIK_E) && PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_CurState()
				|| Engine::Get_DIKeyDown(DIK_E) && PLAYER_STATE::PLAYER_CUPBOARD_IN == m_pPlayer->Get_CurState())
			{
				if (CAMERA_PURPOSE::PURPOSE_CUPBOARD != m_pDynamicCamera->Get_CameraPurpose())
				{
					/*m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CUPBOARD_IN);
					m_bOnePlayAnimation = TRUE;
					m_dOnePlayAnimationSetEnd = 0.95;

					for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_CUPBOARD))
					{
						if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
						{
							static_cast<CCupboard*>(iter)->Set_State(CCupboard::STATE::PLAYER_IN);
							break;
						}
					}*/

					m_bSinkTime = TRUE;
				}
				else if (CAMERA_PURPOSE::PURPOSE_CUPBOARD == m_pDynamicCamera->Get_CameraPurpose())
				{
					Engine::MyPlaySound(L"Trunk.mp3", Engine::CHANNELID::TRUNK);

					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CUPBOARD_OUT);
					m_pDynamicCamera->Set_CameraPurposeStart(FALSE);
					m_pDynamicCamera->Set_CameraPurposeExit(TRUE);

					for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_CUPBOARD))
					{
						if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
						{
							static_cast<CCupboard*>(iter)->Set_State(CCupboard::STATE::PLAYER_OUT);
							break;
						}
					}
				}
			}
		}
		else if (TRUE == m_pPlayer->Get_CarryState())
		{
			if (Engine::Get_DIMouseDown(Engine::DIM_LB) && PLAYER_STATE::PLAYER_CARRY_IDLE == m_pPlayer->Get_CurState()
				|| Engine::Get_DIMouseDown(Engine::DIM_LB) && PLAYER_STATE::PLAYER_CARRY_WALK == m_pPlayer->Get_CurState()
				|| Engine::Get_DIMouseDown(Engine::DIM_LB) && PLAYER_STATE::PLAYER_CARRY_RUN == m_pPlayer->Get_CurState())
			{
				m_bSinkTime = TRUE;
				
				for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_CUPBOARD))
				{
					if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
					{
						static_cast<CCupboard*>(iter)->Set_State(CCupboard::STATE::ELITE_HIDE);
						break;
					}
				}
			}
		}
	}

	/*  Torch ColTo  */
	else if (PLAYER_COL_TORCH & m_pPlayer->Get_ColTo())
	{
		if (FALSE == m_pPlayer->Get_CarryState())
		{
			if (Engine::Get_DIKeyDown(DIK_E) && PLAYER_STATE::PLAYER_IDLE == m_pPlayer->Get_CurState()
				|| Engine::Get_DIKeyDown(DIK_E) && PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_CurState())
			{
				//m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_TORCH_ON);
				//m_bOnePlayAnimation = TRUE;
				//m_dOnePlayAnimationSetEnd = 0.95;

				//for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_TORCH))
				//{
				//	if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
				//	{
				//		static_cast<CTorch*>(iter)->Set_LightOnOff();
				//		break;
				//	}
				//}

				m_bSinkTime = TRUE;
			}
		}
	}

	/*  Lever ColTo  */
	else if (PLAYER_COL_LEVER & m_pPlayer->Get_ColTo())
	{
		if (FALSE == m_pPlayer->Get_CarryState())
		{
			if (Engine::Get_DIKeyDown(DIK_E) && PLAYER_STATE::PLAYER_IDLE == m_pPlayer->Get_CurState())
			{
				//if (0 < static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECT_ITEMLIST))->Get_ItemNum(5))
					//m_bSinkTime = TRUE;
				//else
					//static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(7);

				m_bSinkTime = TRUE;

				Engine::CGameObject* pLever = (Engine::Find_ObjectListFront(Engine::OBJECT_LEVER));
				if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(pLever->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
				{
					if (0 == static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECT_ITEMLIST))->Get_ItemNum(5))
						static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(7);
				}
				else
				{

				}
			}
		}
	}

	/*  Elite ColTo  */
	else if (PLAYER_COL_ELITE & m_pPlayer->Get_ColTo())
	{
		if (nullptr != m_pPlayer->Get_CollToElite())
		{
			if (Engine::Get_DIKeyDown(DIK_E))
			{
				if (9 != m_pPlayer->Get_CollToElite()->Get_EliteID()
					&& 10 != m_pPlayer->Get_CollToElite()->Get_EliteID()
					&& 11 != m_pPlayer->Get_CollToElite()->Get_EliteID())
				{
					if (TRUE == m_pPlayer->Get_CollToElite()->Get_Live()
						&& FALSE == m_pPlayer->Get_CarryState())
					{
						/*  Assass Backstab  */
						if (FALSE == m_pPlayer->Get_CollToElite()->Get_SwordActive())
						{
							//m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_ASSASS_IN);
							//m_bOnePlayAnimation = TRUE;
							m_pPlayer->Get_CollToElite()->Set_CurState(ELITE_STATE::ELITE_IDLE);

							m_bSinkTime = TRUE;
						}
						/*  Assass Frontstab  */
						else if (TRUE == m_pPlayer->Get_CollToElite()->Get_SwordActive())
						{

						}
					}
					else if (FALSE == m_pPlayer->Get_CollToElite()->Get_Live())
					{
						if (0 < static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_CurItemNum())
						{
							/*  Mystery  */
							if (1 == static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemSelect())
							{
								m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_DRINK_POSITION);
								Set_OnePlayAni(0.95);
							}
						}
					}
				}
				else if (11 == m_pPlayer->Get_CollToElite()->Get_EliteID())
				{
					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_ATTACK_UP);
					Set_OnePlayAni(0.85);
				}
			}
			else if (Engine::Get_DIMouseDown(Engine::DIM_LB))
			{
				if (FALSE == m_pPlayer->Get_CollToElite()->Get_Live()
					&& ELITE_STATE::ELITE_HIDE != m_pPlayer->Get_CollToElite()->Get_CurState())
				{
					/*  Carry Pick  */
					if (FALSE == m_pPlayer->Get_CarryState())
					{
						m_bSinkTime = TRUE;
					}
					/*  Carry Drop  */
					else if (TRUE == m_pPlayer->Get_CarryState())
					{
						m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CARRY_DROP);
						Set_OnePlayAni(0.95);
						m_pPlayer->Get_CollToElite()->Set_CurState(ELITE_STATE::ELITE_CARRY_DROP);
					}
				}
			}
		}
	}

	/*  StyxElite True  */
	if (TRUE == m_pPlayer->Get_EliteState()
		&& FALSE == m_pPlayer->Get_Translucent())
	{
		if (Engine::Get_DIKeyDown(DIK_F)
			&& Engine::COL_FALSE == m_pPlayer->Get_IsCollide())
		{
			m_pPlayer->Set_EliteState(FALSE);
			m_pPlayer->Set_StyxEliteRender(TRUE);
			m_pPlayer->Set_FightState(TRUE);
			m_pPlayer->Set_SneakState(TRUE);

			m_pPlayer->Get_EliteMeshCom()->Set_AnimationSet(80);
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_IDLE);
			static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_FightState(TRUE);

			//m_pPlayer->Set_EliteState(FALSE);
			//m_pPlayer->Set_StyxEliteRender(TRUE);
			//m_pPlayer->Set_SneakState(FALSE);

			//m_pPlayer->Get_EliteMeshCom()->Set_AnimationSet(80);
			//m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
		}
	}

}

void CPlayerBehavior::On_CheatKey(const Engine::_float& fDeltaTime)
{
	if (Engine::Get_DIKeyDown(DIK_6))
	{
		if (CAMERA_PURPOSE::PURPOSE_FIRST_PERSON != static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Get_CameraPurpose())
			static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_FIRST_PERSON);
		else if (CAMERA_PURPOSE::PURPOSE_FIRST_PERSON == static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Get_CameraPurpose())
			static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_END);
	}

	if (Engine::Get_DIKeyDown(DIK_7))
	{
		m_bCheatJump = !(m_bCheatJump);
		Reset_JumpAccel();
	}


	/*  Back ChiteKey  */
	if (Engine::Get_DIKeyPressing(DIK_TAB))
	{
		m_vNewPlayerLook = m_vCameraPos - m_pPlayerTransCom->m_vInfo[Engine::INFO_POS];
		m_vNewPlayerLook.y = 0.f;
		D3DXVec3Normalize(&m_vNewPlayerLook, &m_vNewPlayerLook);

		m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * fDeltaTime * 1.3f;

		m_pPlayer->Set_NewPos(m_vNewPlayerPos);
		m_pPlayerTransCom->m_vInfo[Engine::INFO::INFO_POS] = m_vNewPlayerPos;

		/*  Look  */
		m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewPlayerLook;
		D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_vNewPlayerLook);
		D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_vNewPlayerLook, &m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT]);
		D3DXVec3Normalize(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_pPlayerTransCom->m_vInfo[Engine::INFO_UP]);

		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_WALK);
		m_bMoving = TRUE;

		/*  AABB NewWorld  */
		m_pPlayerTransCom->Set_AABBWorld(m_vNewPlayerPos);
	}

	/*  Grid Open  */
	//else if (Engine::Get_DIKeyDown(DIK_NUMPAD9))
	//{
	//	static_cast<CGrid*>(Engine::Find_ObjectListFront(Engine::OBJECT_GRID))->Set_Active();
	//	static_cast<CGrid*>(Engine::Find_ObjectListFront(Engine::OBJECT_GRID))->Set_Open();
	//}

	/*  SlowActive False  */
	else if (Engine::Get_DIKeyDown(DIK_NUMPAD9))
	{
		g_bSlowActive = FALSE;
	}

	/*  Elite SwordActive Out  */
	else if (Engine::Get_DIKeyDown(DIK_NUMPAD7))
	{
		/*if(nullptr != m_pPlayer->Get_CollToElite())
			m_pPlayer->Get_CollToElite()->Get_EliteBehavior()->Set_SwordActiveOut();
		else*/
		{
			for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECT_ELITE))
			{
				if (TRUE == static_cast<CElite*>(iter)->Get_SwordActive()
					&& TRUE == static_cast<CElite*>(iter)->Get_Live())
				{
					static_cast<CElite*>(iter)->Get_EliteBehavior()->Set_SwordActiveOut();
					return;
				}
			}
		}
	}

	/*  SinkTime False  */
	else if (Engine::Get_DIKeyDown(DIK_NUMPAD6))
	{
		m_bSinkTime = FALSE;
	}
}