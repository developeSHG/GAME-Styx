void CPlayerBehavior::Init_OnePlayAnimation(void)
{
	if (FALSE == m_pPlayer->Get_EliteState())
	{
		if (FALSE == m_bOnePlayAnimation
			|| !(m_pPlayerMeshCom->Is_AnimationSetRateEnd(m_dOnePlayAnimationSetEnd)))
			return;
	}
	else
	{
		if (FALSE == m_bOnePlayAnimation
			|| !(m_pPlayer->Get_EliteMeshCom()->Is_AnimationSetRateEnd(m_dOnePlayAnimationSetEnd)))
			return;
	}


	/*  Door In  */
	if (PLAYER_STATE::PLAYER_DOOR_IN == m_pPlayer->Get_CurState())
	{
		m_pStaticCamera->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_DOOR);
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_DOOR_LOOP);
	}
	/*  Door Open  */
	else if (PLAYER_STATE::PLAYER_DOOR_OPEN == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);

		if (1 == static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemNum(5))
			static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Set_MiusItem(5);
	}

	/*  Trunk In  */
	else if (PLAYER_STATE::PLAYER_TRUNK_IN == m_pPlayer->Get_CurState())
		m_pStaticCamera->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_TRUNK);

	/*  Cupboard In  */
	else if (PLAYER_STATE::PLAYER_CUPBOARD_IN == m_pPlayer->Get_CurState())
		m_pStaticCamera->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_CUPBOARD);

	/*  Find  */
	else if (PLAYER_STATE::PLAYER_FIND == m_pPlayer->Get_CurState())
	{
		if (FALSE == m_pPlayer->Get_CheerState())
		{
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
			static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CheerState(FALSE);
		}
		else if (TRUE == m_pPlayer->Get_CheerState())
		{
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CHEER);

			if (0 == static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemNum(5))
			{
				static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(1);
				static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_ITEMLIST))->Set_AddItem(5);
			}
		}
	}

	/*  Roll  */
	else if (PLAYER_STATE::PLAYER_ROLL == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
		m_pPlayerMeshCom->Set_TrackPosition(0.75);

		if (TRUE == m_bMoving)
		{
			if (Engine::Get_DIKeyPressing(DIK_W) || Engine::Get_DIKeyPressing(DIK_S)
				|| Engine::Get_DIKeyPressing(DIK_A) || Engine::Get_DIKeyPressing(DIK_D))
			{
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_WALK);

				if (Engine::Get_DIKeyPressing(DIK_LSHIFT))
					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_RUN);
			}
		}
	}

	/*  DropIn, DropOut  */
	else if (PLAYER_STATE::PLAYER_DROPIN == m_pPlayer->Get_CurState()
		|| PLAYER_STATE::PLAYER_DROPOUT == m_pPlayer->Get_CurState())
	{
		if (FALSE == m_pPlayer->Get_SneakState())
		{
			if(FALSE == m_pPlayer->Get_SitState())
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
			else
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_SIT);
		}
		/*  Basic Idle is Sit  */
		else if (TRUE == m_pPlayer->Get_SneakState())
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
	}

	/*  Lever On  */
	else if (PLAYER_STATE::PLAYER_LEVER_ON == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
		m_pPlayer->Set_TrackPosition(0.9);

		if(FALSE == m_pPlayer->Get_SneakState())
			m_pPlayerMeshCom->Set_AnimationSet(217, 0.55f);
		else
			m_pPlayerMeshCom->Set_AnimationSet(180, 0.55f);

		Engine::CGameObject* pLever = (Engine::Find_ObjectListFront(Engine::OBJECT_LEVER));
		if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(pLever->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
		{
			/*  Key  */
			if (0 < static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECT_ITEMLIST))->Get_ItemNum(5))
			{
				static_cast<CItemList*>(Engine::Find_ObjectListBack(Engine::OBJECT_ITEMLIST))->Set_MiusItem(5);
				static_cast<CGrid*>(Engine::Find_ObjectListFront(Engine::OBJECT_GRID))->Set_Active();
			}
		}
		else
		{
			pLever = (Engine::Find_ObjectListBack(Engine::OBJECT_LEVER));
			if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(pLever->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
				static_cast<CGrid*>(Engine::Find_ObjectListBack(Engine::OBJECT_GRID))->Set_Active2();
		}
	}

	/*  Elite ColTo  */
	else if (PLAYER_STATE::PLAYER_CARRY_DROP == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
		m_pPlayer->Set_CarryState(FALSE);

		m_pPlayerMeshCom->Set_TrackPosition(0.85);
	}

	/*  Elite Trunk Hide  */
	else if (PLAYER_STATE::PLAYER_ELITE_HIDE_TRUNK == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
		m_pPlayer->Set_CarryState(FALSE);

		m_pPlayerMeshCom->Set_TrackPosition(0.85);
	}

	/*  Elite Cupboard Hide  */
	else if (PLAYER_STATE::PLAYER_ELITE_HIDE_CUPBOARD == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
		m_pPlayer->Set_CarryState(FALSE);

		m_pPlayerMeshCom->Set_TrackPosition(0.85);
	}

	/*  Player Catch Hurt  */
	else if (PLAYER_STATE::PLAYER_CATCH_HURT == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
		m_pPlayer->Set_FightState(TRUE);

		Engine::CTransform* pHideTransCom = static_cast<Engine::CTransform*>(m_pPlayer->Get_HideObject()->Get_Component(Engine::COM_TRANSFORM));
		/*  Look  */
		{
			m_vNewPlayerLook = pHideTransCom->m_vInfo[Engine::INFO_LOOK];
			D3DXVec3Normalize(&m_vNewPlayerLook, &(-m_vNewPlayerLook));

			m_pPlayer->Set_InitPlayerLook(&m_vNewPlayerLook);
		}

		static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_FightState(TRUE);

		_ushort uNum = 0;
		_bool bCheck = FALSE;
		
		for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_TRUNK))
		{
			//if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
			if (iter == m_pPlayer->Get_HideObject())
			{
				bCheck = TRUE;
				break;
			}
			else
				uNum++;
		}

		if (0 == uNum)
			m_pStaticCamera->Set_AngleY(D3DXToRadian(-90.f));
		else if (1 == uNum)
			m_pStaticCamera->Set_AngleY(D3DXToRadian(0.f));
		
		if(FALSE == bCheck)
		{
			uNum = 0;

			for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_CUPBOARD))
			{
				//if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
				if (iter == m_pPlayer->Get_HideObject())
					break;
				else
					uNum++;
			}

			if(0 == uNum)
				m_pStaticCamera->Set_AngleY(D3DXToRadian(-90.f));
			else if(1 == uNum)
				m_pStaticCamera->Set_AngleY(D3DXToRadian(0.f));
			else if (2 == uNum)
				m_pStaticCamera->Set_AngleY(D3DXToRadian(-90.f));
		}
	}

	/*  Cutlass Throw  */
	else if (PLAYER_STATE::PLAYER_THROW_CUTLASS == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
		m_pPlayerMeshCom->Set_TrackPosition(0.85);
	}

	/*  Cup  */
	else if (PLAYER_STATE::PLAYER_SHAKE_POISON == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
		m_pPlayerMeshCom->Set_TrackPosition(0.85);
	}

	/*  Drink Position  */
	else if (PLAYER_STATE::PLAYER_DRINK_POSITION == m_pPlayer->Get_CurState())
	{
		if (FALSE == m_pPlayer->Get_EliteState())
		{
			static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECT_ITEMLIST))->Set_MiusCurItem();

			/*  Hp  */
			if (0 == static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemSelect())
				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
			/*  Mystery  */
			else if (1 == static_cast<CItemList*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_ITEMLIST))->Get_ItemSelect())
			{
				if (PLAYER_COL_ELITE & m_pPlayer->Get_ColTo())
				{
					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_DRINK_AFTER);
					m_pPlayerMeshCom->Set_TrackPosition(0.95);
					Set_OnePlayAni(0.25);
					return;
					//m_pStaticCamera->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_TRUNK);
				}
				else
					m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
			}
		}
		/*  Translucent True  */
		else if (TRUE == m_pPlayer->Get_EliteState())
		{
			m_pPlayer->Set_EliteState(FALSE);
			m_pPlayer->Set_Translucent(TRUE);
			m_pPlayer->Set_SneakState(FALSE);
			m_pPlayer->Get_EliteMeshCom()->Set_AnimationSet(106);
			m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_TRANSLUCENT);
			Set_OnePlayAni(0.98);
			return;
		}
	}

	/*  Drink After  */
	else if (PLAYER_STATE::PLAYER_DRINK_AFTER == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_DEAD_IN);
	}

	/*  Sick Loop  */
	else if (PLAYER_STATE::PLAYER_SICK_LOOP == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_SICK_OUT);
		Set_OnePlayAni(0.85);
		return;
	}

	/*  Translucent  */
	else if (PLAYER_STATE::PLAYER_TRANSLUCENT == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
		m_pPlayer->Set_TrackPosition(0.98);
		m_pPlayerMeshCom->Set_AnimationSet(217, 0.f, 1.f, 0.f, 0.01f);

		_vec3 vLook = m_pPlayerTransCom->m_vInfo[Engine::INFO::INFO_LOOK];
		D3DXVec3Normalize(&vLook, &vLook);

		m_pPlayer->Set_NewPos(m_vNewPlayerPos + vLook);
	}

	/*  Attack Up  */
	else if (PLAYER_STATE::PLAYER_ATTACK_UP == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
	}

	/*  MakeSound  */
	else if (PLAYER_STATE::PLAYER_MAKE_SOUND == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
	
		for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_ELITE))
		{
			if (12 == static_cast<CElite*>(iter)->Get_EliteID())
			{
				static_cast<CElite*>(iter)->Set_MakeSoundPattern(TRUE);
				break;
			}
		}
	}

	/*  Sick Out  */
	else if (PLAYER_STATE::PLAYER_SICK_OUT == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
	}

	m_bOnePlayAnimation = FALSE;
}
