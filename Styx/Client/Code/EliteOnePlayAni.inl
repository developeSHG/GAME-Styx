void CEliteBehavior::Init_OnePlayAnimation(const Engine::_float& fTimeDelta)
{
	if (FALSE == m_bOnePlayAnimation
		|| !(m_pEliteMeshCom->Is_AnimationSetRateEnd(m_dOnePlayAnimationSetEnd)))
		return;

	/*  Turn  */
	Update_Turn(fTimeDelta);

	switch (m_pElite->Get_CurState())
	{
	case ELITE_STATE::ELITE_SWORD_ACTIVEIN:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
		m_pEliteMeshCom->Set_AnimationSet(139);
		break;

	case ELITE_STATE::ELITE_SWORD_ACTIVEOUT:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
		m_pElite->Set_SwordActive(FALSE);
		m_pElite->Set_IdleOrder(4);
		break;

	case ELITE_STATE::ELITE_ATTACK_UP:
	case ELITE_STATE::ELITE_ATTACK_LEFT:
	case ELITE_STATE::ELITE_ATTACK_RIGHT:
		if (12 != m_pElite->Get_EliteID())
		{
			m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
			m_pElite->Set_CounterKey(FALSE);
			m_pElite->Set_PlayerAniOverlap(FALSE);
		}
		else
		{
			m_pElite->Set_CurState(ELITE_STATE::ELITE_ATTACK_COUNTER);
			Set_OnePlayAni(0.88);
			return;
		}

		//if (FALSE == m_pElite->Get_CounterKey())
		//	m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
		//else
		//{
		//	m_pElite->Set_CurState(ELITE_STATE::ELITE_ATTACK_COUNTER);
		//	Set_OnePlayAni(0.9);
		//	return;
		//}
		//m_pElite->Set_PlayerAniOverlap(FALSE);
		break;

	case ELITE_STATE::ELITE_RUN:
		/*if (12 == m_pElite->Get_EliteID()
			|| TRUE == m_pElite->Get_CounterKey())*/
		{
			m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
			m_pElite->Set_CounterKey(FALSE);
			m_pElite->Set_PlayerAniOverlap(FALSE);
		}
		break;
	case ELITE_STATE::ELITE_ATTACK_COUNTER:
		{
			if (12 == m_pElite->Get_EliteID())
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
				Set_OnePlayAni(0.35);
				return;
			}
			
			m_pElite->Set_CounterKey(FALSE);
			m_pElite->Set_PlayerAniOverlap(FALSE);
			
			if (FALSE == m_pElite->Get_FrontAss())
			{
				m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
				Set_OnePlayAni(0.35);
				return;
			}
			else if (TRUE == m_pElite->Get_FrontAss())
			{
				_vec3 vLook = m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK];
				D3DXVec3Normalize(&vLook, &vLook);

				m_vNewElitePos = m_pEliteTransCom->Get_InfoPos() - vLook * 0.49f;
				m_pElite->Set_NewPos(m_vNewElitePos);
				m_pEliteTransCom->m_vInfo[Engine::INFO_POS] = m_vNewElitePos;

				m_pElite->Set_CurState(ELITE_STATE::ELITE_ASSASS_IN);
				Set_OnePlayAni(9);

				m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_ASSASS_IN);
				m_pPlayer->Get_PlayerBehavior()->Set_OnePlayAni(9);
				return;
			}
		}
		break;

	case ELITE_STATE::ELITE_TRUNK_IN:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_TRUNK_CATCH);
		m_pElite->Set_PlayerCatchOriginY(m_pPlayerTransCom->Get_InfoPos().y);
		Set_OnePlayAni(0.85);
		return;

	case ELITE_STATE::ELITE_CUPBOARD_IN:
		if (PLAYER_TRUNK_IN == m_pPlayer->Get_CurState()
			|| PLAYER_CUPBOARD_IN == m_pPlayer->Get_CurState())
		{
			m_pElite->Set_CurState(ELITE_STATE::ELITE_CUPBOARD_CATCH);
			m_pElite->Set_PlayerCatchOriginY(m_pPlayerTransCom->Get_InfoPos().y);
			Set_OnePlayAni(0.85);
		}
		else
		{
			m_pElite->Set_CurState(ELITE_STATE::ELITE_CUPBOARD_OUT);
			Set_OnePlayAni(0.85);

			//Engine::CGameObject* iter = (*Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_CUPBOARD)->begin() + 1);
			//static_cast<CCupboard*>(iter)->Set_State(CCupboard::STATE::SEARCH_OUT);
		}
		return;

	case ELITE_STATE::ELITE_CUPBOARD_OUT:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
		break;

	case ELITE_STATE::ELITE_TRUNK_CATCH:
	case ELITE_STATE::ELITE_CUPBOARD_CATCH:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_CATCH_ATTACK);
		Set_OnePlayAni(0.85);

		m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_CATCH_HURT);
		m_pPlayerMeshCom->Set_AnimationSet(251);
		m_pPlayer->Get_PlayerBehavior()->Set_OnePlayAni(0.85);
		return;

	case ELITE_STATE::ELITE_CATCH_ATTACK:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
		break;

	case ELITE_STATE::ELITE_DRINK_IN:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_DRINK_LOOP);
		m_pElite->Set_TrackPosition(0.98);
		m_pEliteMeshCom->Set_AnimationSet(123, 0.f, 1.f, 0.f, 0.01f);
		Set_OnePlayAni(0.9);

		return;

	case ELITE_STATE::ELITE_DRINK_LOOP:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_DRINK_OUT);
		Set_OnePlayAni(0.9);
		return;

	case ELITE_STATE::ELITE_DRINK_OUT:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_DRINK_DEAD);

		for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_ITEM))
		{
			if (Engine::COL_OBJECTID::COL_OBJECT_CUP == iter->Get_ColObjectID())
			{
				static_cast<CFeature*>(iter)->Set_Hide(FALSE);
				m_pElite->Get_Cup()->Set_Drink(FALSE);
				break;
			}
		}
		break;

	case ELITE_STATE::ELITE_SLEEP_OUT:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_L90);
		Set_OnePlayAni(0.98);
		return;

	case ELITE_STATE::ELITE_CUTLASS_DEAD:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_DEAD);
		m_pElite->Set_Live(FALSE);

		if (m_pElite->Get_EliteID() == Engine::Get_EliteWave())
		{
			Engine::Set_EliteWave();

			if (3 == m_pElite->Get_EliteID() || 5 == m_pElite->Get_EliteID() || 8 == m_pElite->Get_EliteID() || 11 == m_pElite->Get_EliteID())
				static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_TipOneRender(TRUE);
		}
		break;

	case ELITE_STATE::ELITE_CLEAN_BROOM_OUT:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);
		break;

	case ELITE_STATE::ELITE_LEVER_ON:
		static_cast<CGrid*>(Engine::Find_ObjectListBack(Engine::OBJECT_GRID))->Set_Active2();
		m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE_TURN_L90);
		Set_OnePlayAni(0.98);
		return;

	}

	m_bOnePlayAnimation = FALSE;
}

void CEliteBehavior::Update_Turn(const Engine::_float& fTimeDelta)
{
	if (ELITE_STATE::ELITE_IDLE_TURN_L180 != m_pElite->Get_CurState()
		&& ELITE_STATE::ELITE_IDLE_TURN_L90 != m_pElite->Get_CurState()
		&& ELITE_STATE::ELITE_IDLE_TURN_R90 != m_pElite->Get_CurState()
		&& ELITE_STATE::ELITE_IDLE_TURN_R180 != m_pElite->Get_CurState())
		return;

	Engine::_matrix matRot;

	m_vNewEliteLook = m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK];

	switch (m_pElite->Get_CurState())
	{
	case ELITE_STATE::ELITE_IDLE_TURN_L180:
		D3DXMatrixRotationY(&matRot, D3DXToRadian(-180.f));
		D3DXVec3TransformNormal(&m_vNewEliteLook, &m_vNewEliteLook, &matRot);
		break;

	case ELITE_STATE::ELITE_IDLE_TURN_L90:
		D3DXMatrixRotationY(&matRot, D3DXToRadian(-90.f));
		D3DXVec3TransformNormal(&m_vNewEliteLook, &m_vNewEliteLook, &matRot);
		break;

	case ELITE_STATE::ELITE_IDLE_TURN_R90:
		D3DXMatrixRotationY(&matRot, D3DXToRadian(90.f));
		D3DXVec3TransformNormal(&m_vNewEliteLook, &m_vNewEliteLook, &matRot);
		break;

	case ELITE_STATE::ELITE_IDLE_TURN_R180:
		D3DXMatrixRotationY(&matRot, D3DXToRadian(180.f));
		D3DXVec3TransformNormal(&m_vNewEliteLook, &m_vNewEliteLook, &matRot);
		break;
	}

	m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook;
	D3DXVec3Cross(&m_pEliteTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_vNewEliteLook);
	D3DXVec3Cross(&m_pEliteTransCom->m_vInfo[Engine::INFO_UP], &m_vNewEliteLook, &m_pEliteTransCom->m_vInfo[Engine::INFO_RIGHT]);
	D3DXVec3Normalize(&m_pEliteTransCom->m_vInfo[Engine::INFO_UP], &m_pEliteTransCom->m_vInfo[Engine::INFO_UP]);

	m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
	m_pEliteMeshCom->Set_AnimationSet(89, 0.f, 1.f, 0.f, 0.001f);
}