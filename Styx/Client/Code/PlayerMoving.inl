void CPlayerBehavior::Update_WalkFront(const Engine::_float & fDeltaTime)
{
	m_vNewPlayerLook = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] - m_vCameraPos;
	D3DXVec3Normalize(&m_vNewPlayerLook, &m_vNewPlayerLook);

	if (TRUE == m_pPlayer->Get_FirstPersonState())
		m_vNewPlayerLook *= -1.f;

	/*  Change Animation  */
	Change_MovingAnimation(fDeltaTime);

	if (PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_NextState())
	{
		if (m_pPlayer->Get_AniRateEnd(0.05))
			m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * m_fSpeed * fDeltaTime;
	}
	else
		m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * m_fSpeed * fDeltaTime;

	Update_PlayerAxis(fDeltaTime, m_fRotAngle);
}

void CPlayerBehavior::Update_WalkBack(const Engine::_float & fDeltaTime)
{
	m_vNewPlayerLook = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] - m_vCameraPos;
	D3DXVec3Normalize(&m_vNewPlayerLook, &m_vNewPlayerLook);
	m_vNewPlayerLook *= -1.f;

	if (TRUE == m_pPlayer->Get_FirstPersonState())
		m_vNewPlayerLook *= -1.f;

	/*  Change Animation  */
	Change_MovingAnimation(fDeltaTime);

	if (PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_NextState())
	{
		if (m_pPlayer->Get_AniRateEnd(0.05))
			m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * m_fSpeed * fDeltaTime;
	}
	/*  Run  */
	else
		m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * m_fSpeed * fDeltaTime;

	Update_PlayerAxis(fDeltaTime, m_fRotAngle);
}

void CPlayerBehavior::Update_WalkRight(const Engine::_float & fDeltaTime)
{
	Engine::_vec3 vecCameraRight;
	memcpy(&vecCameraRight, &m_matCameraWorld.m[0][0], sizeof(Engine::_float) * 3);

	Engine::_matrix matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(-1.f));
	D3DXVec3TransformNormal(&vecCameraRight, &vecCameraRight, &matRot);

	m_vNewPlayerLook = vecCameraRight;

	if (TRUE == m_pPlayer->Get_FirstPersonState())
		m_vNewPlayerLook *= -1.f;

	/*  Change Animation  */
	Change_MovingAnimation(fDeltaTime);

	if (PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_NextState())
	{
		if (m_pPlayer->Get_AniRateEnd(0.05))
			m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + vecCameraRight * m_fSpeed * fDeltaTime;
	}
	/*  Run  */
	else
		m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + vecCameraRight * m_fSpeed * fDeltaTime;

	//right, left각 맘에안들면 다른걸로 set, fps카메라에서 m_fSideRotAngle싱크
	Update_PlayerAxis(fDeltaTime, m_fRotAngle);
}

void CPlayerBehavior::Update_WalkLeft(const Engine::_float & fDeltaTime)
{
	Engine::_vec3 vecCameraLeft;
	memcpy(&vecCameraLeft, &m_matCameraWorld.m[0][0], sizeof(Engine::_float) * 3);
	vecCameraLeft *= -1.f;

	Engine::_matrix matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(1.f));
	D3DXVec3TransformNormal(&vecCameraLeft, &vecCameraLeft, &matRot);

	m_vNewPlayerLook = vecCameraLeft;

	if (TRUE == m_pPlayer->Get_FirstPersonState())
		m_vNewPlayerLook *= -1.f;

	/*  Change Animation  */
	Change_MovingAnimation(fDeltaTime);

	if (PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_NextState())
	{
		if (m_pPlayer->Get_AniRateEnd(0.05))
			m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + vecCameraLeft * m_fSpeed * fDeltaTime;
	}
	/*  Run  */
	else
		m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + vecCameraLeft * m_fSpeed * fDeltaTime;

	//right, left각 맘에안들면 다른걸로 set, fps카메라에서 m_fSideRotAngle싱크
	Update_PlayerAxis(fDeltaTime, m_fRotAngle);
}

/*  Corner Moving  */
void CPlayerBehavior::Update_WalkRightFront(const Engine::_float & fDeltaTime)
{
	m_vNewPlayerLook = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] - m_vCameraPos;

	if (TRUE == m_pPlayer->Get_FirstPersonState())
		m_vNewPlayerLook *= -1.f;

	Engine::_matrix matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(45.f));
	D3DXVec3TransformNormal(&m_vNewPlayerLook, &m_vNewPlayerLook, &matRot);

	D3DXVec3Normalize(&m_vNewPlayerLook, &m_vNewPlayerLook);

	/*  Change Animation  */
	Change_MovingAnimation(fDeltaTime);

	if (PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_NextState())
	{
		if (m_pPlayer->Get_AniRateEnd(0.05))
			m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * m_fSpeed * fDeltaTime;
	}
	/*  Run  */
	else
		m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * m_fSpeed * fDeltaTime;

	Update_PlayerAxis(fDeltaTime, m_fRotAngle);
}

/*  Corner Moving  */
void CPlayerBehavior::Update_WalkLeftFront(const Engine::_float & fDeltaTime)
{
	m_vNewPlayerLook = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] - m_vCameraPos;

	if (TRUE == m_pPlayer->Get_FirstPersonState())
		m_vNewPlayerLook *= -1.f;

	Engine::_matrix matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(-45.f));
	D3DXVec3TransformNormal(&m_vNewPlayerLook, &m_vNewPlayerLook, &matRot);

	D3DXVec3Normalize(&m_vNewPlayerLook, &m_vNewPlayerLook);

	/*  Change Animation  */
	Change_MovingAnimation(fDeltaTime);

	if (PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_NextState())
	{
		if (m_pPlayer->Get_AniRateEnd(0.05))
			m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * m_fSpeed * fDeltaTime;
	}
	/*  Run  */
	else
		m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * m_fSpeed * fDeltaTime;

	Update_PlayerAxis(fDeltaTime, m_fRotAngle);
}

/*  Corner Moving  */
void CPlayerBehavior::Update_WalkRightBack(const Engine::_float & fDeltaTime)
{
	m_vNewPlayerLook = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] - m_vCameraPos;

	if (TRUE == m_pPlayer->Get_FirstPersonState())
		m_vNewPlayerLook *= -1.f;

	Engine::_matrix matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(135.f));
	D3DXVec3TransformNormal(&m_vNewPlayerLook, &m_vNewPlayerLook, &matRot);

	D3DXVec3Normalize(&m_vNewPlayerLook, &m_vNewPlayerLook);

	/*  Change Animation  */
	Change_MovingAnimation(fDeltaTime);

	if (PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_NextState())
	{
		if (m_pPlayer->Get_AniRateEnd(0.05))
			m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * m_fSpeed * fDeltaTime;
	}
	/*  Run  */
	else
		m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * m_fSpeed * fDeltaTime;

	Update_PlayerAxis(fDeltaTime, m_fRotAngle);
}

/*  Corner Moving  */
void CPlayerBehavior::Update_WalkLeftBack(const Engine::_float & fDeltaTime)
{
	m_vNewPlayerLook = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] - m_vCameraPos;

	if (TRUE == m_pPlayer->Get_FirstPersonState())
		m_vNewPlayerLook *= -1.f;

	Engine::_matrix matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(-135.f));
	D3DXVec3TransformNormal(&m_vNewPlayerLook, &m_vNewPlayerLook, &matRot);

	D3DXVec3Normalize(&m_vNewPlayerLook, &m_vNewPlayerLook);

	/*  Change Animation  */
	Change_MovingAnimation(fDeltaTime);

	if (PLAYER_STATE::PLAYER_WALK == m_pPlayer->Get_NextState())
	{
		if (m_pPlayer->Get_AniRateEnd(0.05))
			m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * m_fSpeed * fDeltaTime;
	}
	/*  Run  */
	else
		m_vNewPlayerPos = m_pPlayerTransCom->m_vInfo[Engine::INFO_POS] + m_vNewPlayerLook * m_fSpeed * fDeltaTime;

	Update_PlayerAxis(fDeltaTime, m_fRotAngle);
}

void CPlayerBehavior::Update_PlayerAxis(const Engine::_float & fDeltaTime, Engine::_float fRotAngle)
{
	/*  Moving TRUE  */
	m_bMoving = TRUE;

	/*  NewPos  */
	//if(FALSE == m_pPlayer->Get_AABBCol())
	m_pPlayer->Set_NewPos(m_vNewPlayerPos);

	/*  AABB NewWorld  */
	m_pPlayerTransCom->Set_AABBWorld(m_vNewPlayerPos);
	//static_cast<Engine::CCollider*>(m_pPlayer->Get_Component(Engine::COM_AABB_COLLIDER))->Set_WorldMatrix(&m_pPlayerTransCom->m_matAABBWorld);

	/* FightState Rotate False */
	if (TRUE == m_pPlayer->Get_FightState())
	{
		if (PLAYER_STATE::PLAYER_FIGHT_FORWARD != m_pPlayer->Get_CurState()
			&& PLAYER_STATE::PLAYER_FIGHT_BACKWARD != m_pPlayer->Get_CurState())
			return;
		if (PLAYER_STATE::PLAYER_FIGHT_BACKWARD == m_pPlayer->Get_CurState())
			m_vNewPlayerLook *= -1.f;
	}

	Engine::_vec3 vecCurPlayerLook = m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK];
	D3DXVec3Normalize(&vecCurPlayerLook, &vecCurPlayerLook);
	float fAngle = D3DXVec3Dot(&vecCurPlayerLook, &m_vNewPlayerLook);
	fAngle = D3DXToDegree(acosf(fAngle));

	if (fAngle < 1.f)
		m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK] = m_vNewPlayerLook;
	else
	{
		Engine::_vec3 vecCrossForAngle;
		D3DXVec3Cross(&vecCrossForAngle, &vecCurPlayerLook, &m_vNewPlayerLook);
		Engine::_matrix matRotX;
		if (vecCrossForAngle.y >= 0.f)
		{
			D3DXMatrixRotationY(&matRotX, D3DXToRadian(fRotAngle));
			D3DXVec3TransformNormal(&vecCurPlayerLook, &vecCurPlayerLook, &matRotX);
		}
		else
		{
			D3DXMatrixRotationY(&matRotX, D3DXToRadian(-fRotAngle));
			D3DXVec3TransformNormal(&vecCurPlayerLook, &vecCurPlayerLook, &matRotX);
		}
	}
	m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK] = vecCurPlayerLook;
	D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &vecCurPlayerLook);
	D3DXVec3Cross(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &vecCurPlayerLook, &m_pPlayerTransCom->m_vInfo[Engine::INFO_RIGHT]);
	D3DXVec3Normalize(&m_pPlayerTransCom->m_vInfo[Engine::INFO_UP], &m_pPlayerTransCom->m_vInfo[Engine::INFO_UP]);
}