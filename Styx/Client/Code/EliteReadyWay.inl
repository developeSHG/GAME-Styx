void CEliteBehavior::Ready_WayPoint()
{
	switch (m_pElite->Get_EliteID())
	{
	case 0:
		m_uWayPointNum = 3;
		break;

	case 1:
	case 2:
	case 3:
	case 4:
	case 12:
		m_uWayPointNum = 2;
		break;

		/*  Not Moving  */
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		m_uWayPointNum = 0;
		break;
	}

	m_vWayPointPos[0] = m_pEliteTransCom->m_vInfo[Engine::INFO::INFO_POS];
	
	m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook = _vec3(0.f, 0.f, 1.f);

	switch (m_pElite->Get_EliteID())
	{
		/*  중앙 세 방향 애  */
	case 0:
		m_vWayPointPos[1] = m_vWayPointPos[0];
		m_vWayPointPos[1].z += 12.f;
		m_vWayPointPos[2] = m_vWayPointPos[1];
		m_vWayPointPos[2].x -= 6.f;

		m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
		break;

		/*  중앙 오른쪽 애  */
	case 1:
		m_vWayPointPos[1] = m_vWayPointPos[0];
		m_vWayPointPos[1].z += 13.f;

		m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
		break;

		/*  중앙 왼쪽 애  */
	case 2:
		m_vWayPointPos[1] = m_vWayPointPos[0];
		m_vWayPointPos[1].z -= 13.f;

		m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook = _vec3(0.f, 0.f, -1.f);
		m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
		break;

		/*  중앙 왼쪽통로 테이블 순찰하는 애  */
	case 3:
		m_vWayPointPos[1] = m_vWayPointPos[0];
		m_vWayPointPos[1].z -= 13.f;

		m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook = _vec3(0.f, 0.f, -1.f);
		m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
		break;

		/*  왼쪽 통로 움직이는 애  */
	case 4:
		m_vWayPointPos[1] = m_vWayPointPos[0];
		m_vWayPointPos[1].x += 15.f;

		m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook = _vec3(1.f, 0.f, 0.f);
		m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
		break;

		/*  왼쪽 통로 청소하는 애  */
	case 5:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_CLEAN_BROOM);

		m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook = _vec3(1.f, 0.f, 0.f);
		break;

		/*  왼쪽 2층 졸고있는 애  */
	case 6:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_SLEEP_LOOP);
		break;

		/*  왼쪽 2층 의자쪽에 있는 애  */
	case 7:
		m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook = _vec3(-1.f, 0.f, 0.f);
		m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
		break;

		/*  오른쪽 1층 벽닦고 있는 애  */
	case 8:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_CLEAN_WALL);
		break;

		/*  오른쪽 2층 보초서는 왼쪽 후임  */
	case 9:
		m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
		break;

		/*  오른쪽 2층 자고있는 오른쪽 선임  */
	case 10:
		m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook = _vec3(0.f, 0.f, -1.f);
		m_pElite->Set_CurState(ELITE_STATE::ELITE_SLEEP_LOOP);
		break;

		/*  오른쪽 2층 문안에 보초서는 애  */
	case 11:
		m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook = _vec3(1.f, 0.f, 0.f);
		m_pElite->Set_CurState(ELITE_STATE::ELITE_IDLE);
		break;

		/*  최홍만  */
	case 12:
		m_vWayPointPos[1] = m_vWayPointPos[0];
		m_vWayPointPos[1].z -= 12.f;

		m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook = _vec3(0.f, 0.f, -1.f);
		m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
		break;

	}

	D3DXVec3Cross(&m_pEliteTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK]);
	D3DXVec3Cross(&m_pEliteTransCom->m_vInfo[Engine::INFO_UP], &m_vNewEliteLook, &m_pEliteTransCom->m_vInfo[Engine::INFO_RIGHT]);
	D3DXVec3Normalize(&m_pEliteTransCom->m_vInfo[Engine::INFO_UP], &m_pEliteTransCom->m_vInfo[Engine::INFO_UP]);
}
