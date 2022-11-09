void CEliteBehavior::Update_ReturnWay(const Engine::_float& fTimeDelta)
{
	if (TRUE == m_bOnePlayAnimation
		|| TRUE == m_pElite->Get_SwordActive()
		|| TRUE == m_pElite->Get_OnMoveWay()
		|| TRUE == m_pElite->Get_CarryState()
		|| TRUE == m_pElite->Get_FightState()
		|| FALSE == m_pElite->Get_Live())
		return;

	if (FALSE == m_pElite->Get_ReturnState())
		return;

	m_fSpeed = 2.5f;
	m_pElite->Set_CurState(ELITE_STATE::ELITE_RUN);

	_vec3 vDist = m_vWayReturnPos - m_vNewElitePos;
	vDist.y = 0.f;

	m_vNewElitePos = m_pEliteTransCom->m_vInfo[Engine::INFO_POS] + m_vNewEliteLook * m_fSpeed * fTimeDelta;

	if (1.f > D3DXVec3Length(&vDist))
	{
		m_pElite->Set_ReturnState(FALSE);
		m_pElite->Set_OnMoveWay(TRUE);

		m_pElite->Set_CurState(ELITE_STATE::ELITE_WALK);
	}

	m_vNewEliteLook = vDist;
	D3DXVec3Normalize(&m_vNewEliteLook, &m_vNewEliteLook);
	m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook;

	D3DXVec3Cross(&m_pEliteTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_vNewEliteLook);
	D3DXVec3Cross(&m_pEliteTransCom->m_vInfo[Engine::INFO_UP], &m_vNewEliteLook, &m_pEliteTransCom->m_vInfo[Engine::INFO_RIGHT]);
	D3DXVec3Normalize(&m_pEliteTransCom->m_vInfo[Engine::INFO_UP], &m_pEliteTransCom->m_vInfo[Engine::INFO_UP]);

	m_pElite->Set_NewPos(m_vNewElitePos);
}
