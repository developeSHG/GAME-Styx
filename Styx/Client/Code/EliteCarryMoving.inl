void CEliteBehavior::Update_CarryMoving(const Engine::_float& fTimeDelta)
{
	if (TRUE == m_pElite->Get_OnMoveWay()
		|| FALSE == m_pElite->Get_CarryState()
		|| TRUE == m_pElite->Get_Live())
		return;

	m_vNewElitePos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS];

	m_pElite->Set_NewPos(m_vNewElitePos);

	/*  Look  */
	m_vNewEliteLook = -m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK];
	m_pEliteTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewEliteLook;
	D3DXVec3Cross(&m_pEliteTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_vNewEliteLook);
	D3DXVec3Cross(&m_pEliteTransCom->m_vInfo[Engine::INFO_UP], &m_vNewEliteLook, &m_pEliteTransCom->m_vInfo[Engine::INFO_RIGHT]);
	D3DXVec3Normalize(&m_pEliteTransCom->m_vInfo[Engine::INFO_UP], &m_pEliteTransCom->m_vInfo[Engine::INFO_UP]);
}