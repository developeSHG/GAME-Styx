void CPlayerBehavior::Update_Sink(const Engine::_float& fDeltaTime)
{
	if (FALSE == m_bSinkTime)
		return;

	Engine::CTransform*	pColTransform = nullptr;
	Engine::_ushort uNum = 0;
	Engine::_vec3 vPos;
	Engine::_float fDist = 0.f;

	/*  Cup ColTo  */
	if (PLAYER_COL_CUP & m_pPlayer->Get_ColTo())
	{
		for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_ITEM))
		{
			if (Engine::COL_OBJECTID::COL_OBJECT_CUP == iter->Get_ColObjectID())
			{
				pColTransform = static_cast<Engine::CTransform*>(iter->Get_Component(Engine::COM_TRANSFORM));
				break;
			}
		}

		if (nullptr != pColTransform)
		{
			vPos = pColTransform->m_vInfo[Engine::INFO::INFO_POS];
			vPos.x -= 0.15f;
			vPos.z += 0.65f;

			m_vNewPlayerLook = Engine::_vec3(0.f, 0.f, -1.f);

			m_pPlayer->Set_InitPlayerLook(&m_vNewPlayerLook);

			Engine::_vec3 vDir = vPos - m_vNewPlayerPos;
			m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + *D3DXVec3Normalize(&vDir, &vDir) * 3.f * fDeltaTime;

			m_pPlayer->Set_NewPos(m_vNewPlayerPos);
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_WALK);
			m_bMoving = TRUE;

			vPos.y = m_vNewPlayerPos.y;
			fDist = 0.1f;
		}
	}

	/*  Door ColTo  */
	else if (PLAYER_COL_DOOR & m_pPlayer->Get_ColTo())
	{
		for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_DOOR))
		{
			if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_AABB_COLLIDER))->Get_IsCollide())
			{
				pColTransform = static_cast<Engine::CTransform*>(iter->Get_Component(Engine::COM_TRANSFORM));
				break;
			}
			else
				uNum++;
		}

		if (nullptr != pColTransform)
		{
			vPos = pColTransform->m_vInfo[Engine::INFO::INFO_POS];

			if (0 == uNum)
			{
				vPos.x = 63.6492f;
				vPos.z = 44.2066f;

				m_vNewPlayerLook = Engine::_vec3(0.f, 0.f, 1.f);

				m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewPlayerLook;
				D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_vNewPlayerLook);
				D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_vNewPlayerLook, &m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT]);
				D3DXVec3Normalize(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_pPlayerTransCom->m_vInfo[Engine::INFO_UP]);
			}

			Engine::_vec3 vDir = vPos - m_vNewPlayerPos;
			m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeed * fDeltaTime;

			m_pPlayer->Set_NewPos(m_vNewPlayerPos);
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_WALK);
			m_bMoving = TRUE;

			fDist = 0.5f;
		}
	}

	/*  Cupboard ColTo  */
	else if (PLAYER_COL_CUPBOARD & m_pPlayer->Get_ColTo())
	{
		for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_CUPBOARD))
		{
			if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
			{
				pColTransform = static_cast<Engine::CTransform*>(iter->Get_Component(Engine::COM_TRANSFORM));
				break;
			}
			else
				uNum++;
		}

		if (nullptr != pColTransform)
		{
			vPos = pColTransform->m_vInfo[Engine::INFO::INFO_POS];

			if (0 == uNum)
				vPos.x += 0.75f;
			else if (1 == uNum)
				vPos.z -= 0.75f;
			else if (2 == uNum)
			{
				vPos.x += 0.75f;

				/*  Look  */
				m_vNewPlayerLook = Engine::_vec3(-1.f, 0.f, 0.f);
				m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewPlayerLook;
				D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_vNewPlayerLook);
				D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_vNewPlayerLook, &m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT]);
				D3DXVec3Normalize(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_pPlayerTransCom->m_vInfo[Engine::INFO_UP]);
			}

			Engine::_vec3 vDir = vPos - m_vNewPlayerPos;

			if (FALSE == m_pPlayer->Get_CarryState())
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_WALK);
				m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeed * fDeltaTime;
			}
			else
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CARRY_WALK);
				m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeed * fDeltaTime;
			}
			m_bMoving = TRUE;

			m_pPlayer->Set_NewPos(m_vNewPlayerPos);

			fDist = 0.05f;
		}
	}

	/*  Torch ColTo  */
	else if (PLAYER_COL_TORCH & m_pPlayer->Get_ColTo())
	{
		Engine::_uint uAngle;

		for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_TORCH))
		{
			if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
			{
				pColTransform = static_cast<Engine::CTransform*>(iter->Get_Component(Engine::COM_TRANSFORM));
				uAngle = Engine::_uint(D3DXToDegree(pColTransform->m_vAngle.y));
				break;
			}
			else
				uNum++;
		}

		if (nullptr != pColTransform)
		{
			vPos = pColTransform->m_vInfo[Engine::INFO::INFO_POS];

			if (0 == uAngle)
			{
				vPos.x -= 0.03f;
				vPos.z += 0.95f;

				m_vNewPlayerLook = Engine::_vec3(0.f, 0.f, -1.f);
			}
			else if (90 == uAngle)
			{
				vPos.x += 0.95f;
				vPos.z += 0.02f;

				m_vNewPlayerLook = Engine::_vec3(-1.f, 0.f, 0.f);
			}
			else if (180 == uAngle)
			{
				vPos.x += 0.02f;
				vPos.z -= 0.95f;

				m_vNewPlayerLook = Engine::_vec3(0.f, 0.f, 1.f);
			}
			else if (270 == uAngle)
			{
				vPos.x -= 0.95f;
				vPos.z -= 0.03f;

				m_vNewPlayerLook = Engine::_vec3(1.f, 0.f, 0.f);
			}

			Engine::_vec3 vDir = vPos - m_vNewPlayerPos;
			m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeed * fDeltaTime;

			m_pPlayer->Set_NewPos(m_vNewPlayerPos);
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_WALK);
			m_bMoving = TRUE;

			/*  Look  */
			m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewPlayerLook;
			D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_vNewPlayerLook);
			D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_vNewPlayerLook, &m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT]);
			D3DXVec3Normalize(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_pPlayerTransCom->m_vInfo[Engine::INFO_UP]);

			fDist = 0.09f;
		}
	}

	/*  Lever ColTo  */
	else if (PLAYER_COL_LEVER & m_pPlayer->Get_ColTo())
	{
		for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_LEVER))
		{
			if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
			{
				pColTransform = static_cast<Engine::CTransform*>(iter->Get_Component(Engine::COM_TRANSFORM));
				break;
			}
			else
				uNum++;
		}

		if (nullptr != pColTransform)
		{
			vPos = pColTransform->m_vInfo[Engine::INFO::INFO_POS];

			if (0 == uNum)
			{
				vPos.x -= 1.f;

				m_vNewPlayerLook = Engine::_vec3(1.f, 0.f, 0.f);
				fDist = 0.2f;
			}
			else if (1 == uNum)
			{
				vPos.z += 1.f;

				m_vNewPlayerLook = Engine::_vec3(0.f, 0.f, -1.f);
				fDist = 0.05f;
			}

			Engine::_vec3 vDir = vPos - m_vNewPlayerPos;
			m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeed * fDeltaTime;

			m_pPlayer->Set_NewPos(m_vNewPlayerPos);
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_WALK);
			m_bMoving = TRUE;

			/*  Look  */
			m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewPlayerLook;
			D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_vNewPlayerLook);
			D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_vNewPlayerLook, &m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT]);
			D3DXVec3Normalize(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_pPlayerTransCom->m_vInfo[Engine::INFO_UP]);
		}
	}

	/*  Elite ColTo  */
	else if (PLAYER_COL_ELITE & m_pPlayer->Get_ColTo())
	{
		if (nullptr != m_pPlayer->Get_CollToElite())
		{
			pColTransform = static_cast<Engine::CTransform*>(m_pPlayer->Get_CollToElite()->Get_Component(Engine::COM_TRANSFORM));

			if (TRUE == m_pPlayer->Get_CollToElite()->Get_Live())
			{
				/*  Assass Backstab  */
				if (FALSE == m_pPlayer->Get_CollToElite()->Get_SwordActive())
				{
					vPos = pColTransform->m_vInfo[Engine::INFO_POS];
					D3DXVec3Normalize(&pColTransform->m_vInfo[Engine::INFO_LOOK], &pColTransform->m_vInfo[Engine::INFO_LOOK]);
					vPos -= pColTransform->m_vInfo[Engine::INFO_LOOK] * 1.4f;

					Engine::_vec3 vDir = vPos - m_vNewPlayerPos;
					m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeed * fDeltaTime;

					/*  Look  */
					m_vNewPlayerLook = pColTransform->m_vInfo[Engine::INFO_LOOK];

					fDist = 0.05f;
				}
				/*  Assass Frontstab  */
				else if (TRUE == m_pPlayer->Get_CollToElite()->Get_SwordActive())
				{
					vPos = pColTransform->m_vInfo[Engine::INFO_POS];
					D3DXVec3Normalize(&pColTransform->m_vInfo[Engine::INFO_LOOK], &pColTransform->m_vInfo[Engine::INFO_LOOK]);
					vPos += pColTransform->m_vInfo[Engine::INFO_LOOK] * 0.7f;

					Engine::_vec3 vDir = vPos - m_vNewPlayerPos;
					m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + *D3DXVec3Normalize(&vDir, &vDir) * 2.f * fDeltaTime;

					/*  Look  */
					m_vNewPlayerLook = -pColTransform->m_vInfo[Engine::INFO_LOOK];

					fDist = 0.05f;
				}
			}
			else if (FALSE == m_pPlayer->Get_CollToElite()->Get_Live())
			{
				/*  Carry Pick  */
				if (FALSE == m_pPlayer->Get_CarryState())
				{
					vPos = pColTransform->m_vInfo[Engine::INFO_POS];

					Engine::_vec3 vDir = vPos - m_vNewPlayerPos;

					if(FALSE == m_pPlayer->Get_SneakState())
						m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeed * fDeltaTime;
					else
						m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + *D3DXVec3Normalize(&vDir, &vDir) * m_fSpeed * fDeltaTime;

					/*  Look  */
					m_vNewPlayerLook = -pColTransform->m_vInfo[Engine::INFO_LOOK];
					
					
					if (ELITE_STATE::ELITE_ASSASS_OUT == m_pPlayer->Get_CollToElite()->Get_CurState()
						&& FALSE == m_pPlayer->Get_CollToElite()->Get_FrontAss())
					{
						_matrix matRot;
						D3DXMatrixRotationAxis(&matRot, &_vec3(0.f, 1.f, 0.f), D3DXToRadian(-45.f));
						D3DXVec3TransformNormal(&m_vNewPlayerLook, &m_vNewPlayerLook, &matRot);
					}

					fDist = 0.05f;
				}
				/*  Carry Drop  */
				//else if (TRUE == m_pPlayer->Get_CarryState())
				//{
				//}

			}

			/*  Moving  */
			m_pPlayer->Set_NewPos(m_vNewPlayerPos);
			if(FALSE == m_pPlayer->Get_FightState())
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_WALK);
			else if (TRUE == m_pPlayer->Get_FightState())
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_FORWARD);
			m_bMoving = TRUE;

			/*  Look  */
			m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewPlayerLook;
			D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_vNewPlayerLook);
			D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_vNewPlayerLook, &m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT]);
			D3DXVec3Normalize(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_pPlayerTransCom->m_vInfo[Engine::INFO_UP]);
		}
	}

	/*  Sink Time Moving  */
	if (nullptr != pColTransform)
	{
		if (fDist > D3DXVec3Length(&(vPos - m_vNewPlayerPos)))
		{
			/*  Cup  */
			if (PLAYER_COL_CUP & m_pPlayer->Get_ColTo())
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_SHAKE_POISON);
				Set_OnePlayAni(0.85);

				for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_ITEM))
				{
					if (Engine::COL_OBJECTID::COL_OBJECT_CUP == iter->Get_ColObjectID())
					{
						static_cast<CFeature*>(iter)->Set_Poison(TRUE);
						break;
					}
				}

				static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ITEMLIST))->Set_MiusCurItem();

				m_bSinkTime = FALSE;
				return;
			}

			/*  Door  */
			else if (PLAYER_COL_DOOR & m_pPlayer->Get_ColTo())
			{
				for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_DOOR))
				{
					if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_AABB_COLLIDER))->Get_IsCollide())
					{
						m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_DOOR_OPEN);
						Set_OnePlayAni(0.95);

						if (0 == static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemNum(5))
							static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(0);
						else if (1 == static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemNum(5))
							static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(2);

						m_bSinkTime = FALSE;
						return;
					}
				}
			}

			/*  Cupboard  */
			else if (PLAYER_COL_CUPBOARD & m_pPlayer->Get_ColTo())
			{
				for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_CUPBOARD))
				{
					if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
					{
						Engine::MyPlaySound(L"Trunk.mp3", Engine::CHANNELID::TRUNK);

						if (FALSE == m_pPlayer->Get_CarryState())
						{
							static_cast<CCupboard*>(iter)->Set_State(CCupboard::STATE::PLAYER_IN);
							m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CUPBOARD_IN);
							Set_OnePlayAni(0.95);
						}
						else
						{
							m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_ELITE_HIDE_CUPBOARD);
							m_pPlayer->Get_CollToElite()->Set_CurState(ELITE_STATE::ELITE_HIDE_CUPBOARD_IN);
							m_pPlayer->Get_CollToElite()->Set_CarryState(FALSE);
							Set_OnePlayAni(0.95);
						}

						m_bSinkTime = FALSE;
						return;
					}
				}
			}

			/*  Torch  */
			else if (PLAYER_COL_TORCH & m_pPlayer->Get_ColTo())
			{
				for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_TORCH))
				{
					if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
					{
						m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_TORCH_ON);
						Set_OnePlayAni(0.95);

						for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_TORCH))
						{
							if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
							{
								static_cast<CTorch*>(iter)->Set_LightOnOff();
								break;
							}
						}

						m_bSinkTime = FALSE;
						return;
					}
				}
			}

			/*  Lever  */
			else if (PLAYER_COL_LEVER & m_pPlayer->Get_ColTo())
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_LEVER_ON);
				Set_OnePlayAni(0.9);

				Engine::CGameObject* pLever = (Engine::Find_ObjectListFront(Engine::OBJECT_LEVER));
				if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(pLever->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
					static_cast<CLever*>(Engine::Find_ObjectListFront(Engine::OBJECT_LEVER))->Set_Active();
				else 
				{
					pLever = (Engine::Find_ObjectListBack(Engine::OBJECT_LEVER));
					if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(pLever->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
						static_cast<CLever*>(Engine::Find_ObjectListBack(Engine::OBJECT_LEVER))->Set_Active();
				}

				m_bSinkTime = FALSE;
				return;
			}

			/*  Elite  */
			else if (PLAYER_COL_ELITE & m_pPlayer->Get_ColTo())
			{
				if (nullptr != m_pPlayer->Get_CollToElite())
				{
					if (TRUE == m_pPlayer->Get_CollToElite()->Get_Live())
					{
						/*  Assass Backstab  */
						if (FALSE == m_pPlayer->Get_CollToElite()->Get_SwordActive())
						{
							m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_ASSASS_IN);
						}
						/*  Assass Frontstab  */
						else if (TRUE == m_pPlayer->Get_CollToElite()->Get_SwordActive())
						{

							//m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_ASSASS_IN);
							//m_pPlayer->Get_PlayerBehavior()->Set_OnePlayAni(0.85);

							//m_bSinkTime = FALSE;
							//return;
						}
					}
					else if (FALSE == m_pPlayer->Get_CollToElite()->Get_Live())
					{	
						/*  Carry Pick  */
						if (FALSE == m_pPlayer->Get_CarryState())
						{
							m_pPlayer->Set_CarryState(TRUE);
							m_pPlayer->Set_FightState(FALSE);
							m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CARRY_PICK);
							m_pPlayer->Get_CollToElite()->Set_CurState(ELITE_STATE::ELITE_CARRY_PICK);
							m_pPlayer->Get_CollToElite()->Set_CarryState(TRUE);
						}
					}

					m_bSinkTime = FALSE;
					return;
				}
			}

		}
	}
}