#include "stdafx.h"
#include "EliteBehavior.h"

#include "Elite.h"
#include "PlayerBehavior.h"
#include "Cup.h"

#include "EliteReadyWay.inl"
#include "EliteMoveWay.inl"
#include "EliteOnePlayAni.inl"
#include "EliteCarryMoving.inl"
#include "EliteFightState.inl"
#include "EliteReturnWay.inl"

USING(Engine)

CEliteBehavior::CEliteBehavior()
{
}

CEliteBehavior::CEliteBehavior(const CEliteBehavior& rhs)
	: CComponent(rhs)
{

}

CEliteBehavior::~CEliteBehavior(void)
{

}

HRESULT CEliteBehavior::Ready_EliteBehavior(void)
{
	return S_OK;
}

Engine::_int CEliteBehavior::Update_Component(const Engine::_float & fTimeDelta)
{	
	//Engine::_vec3 vDir = *m_pPlayerTransCom->Get_InfoPosPointer() - m_pEliteTransCom->m_vInfo[Engine::INFO_POS];
	//m_vNewElitePos = m_pEliteTransCom->m_vInfo[Engine::INFO_POS] + *D3DXVec3Normalize(&vDir, &vDir) * 3.f * fTimeDelta;

	if (TRUE == m_pElite->Get_Hide())
		return UPDATE_OK;

	/*  Init OnePlayAnimation  */
	Init_OnePlayAnimation(fTimeDelta);

	/*  Move Way  */
	Update_MoveWay(fTimeDelta);

	/*  Carry Moving  */
	Update_CarryMoving(fTimeDelta);

	/*  Fight State  */
	Update_FightState(fTimeDelta);

	/*  Return Way  */
	Update_ReturnWay(fTimeDelta);

	return UPDATE_OK;
}

void CEliteBehavior::Set_Elite(CElite * pElite)
{
	m_pElite = pElite;
	m_pEliteMeshCom = SCAST<CDynamicMesh*>(m_pElite->Get_Component(COMPONENTTYPE::COM_MESH));
	m_pEliteTransCom = SCAST<CTransform*>(m_pElite->Get_Component(COMPONENTTYPE::COM_TRANSFORM));

	Ready_WayPoint();
}

void CEliteBehavior::Set_Player(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayerMeshCom = SCAST<CDynamicMesh*>(m_pPlayer->Get_Component(COMPONENTTYPE::COM_MESH));
	m_pPlayerTransCom = SCAST<CTransform*>(m_pPlayer->Get_Component(COMPONENTTYPE::COM_TRANSFORM));
}

CEliteBehavior* CEliteBehavior::Create()
{
	CEliteBehavior* pInstance = new CEliteBehavior();

	if (FAILED(pInstance->Ready_EliteBehavior()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent * CEliteBehavior::Clone(void)
{
	return new CEliteBehavior(*this);
}

void CEliteBehavior::Free(void)
{

}