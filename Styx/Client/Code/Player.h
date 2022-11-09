#ifndef Player_h__
#define Player_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

#include "DynamicMesh.h"
#include "SphereCollider.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCubeCollider;
class CSphereCollider;
class CNaviMesh;
class CShader;

END

class CPlayerBehavior;
class CElite;
class CSpark;
class CPlayerSword;

class CPlayer : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CPlayer(void);

public:
	/*  General  */
	virtual	HRESULT				Ready_Object(void) override;
	virtual	_int				Update_Object(const _float& fTimeDelta) override;
	virtual	_int				LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void				PreRender_Object(void) override;
	virtual	void				Render_Object(void) override;
	virtual void				PostRender_Object(void) override;

public:
	/*  Choice  */
	virtual _int				CollisionUpdate_Object(const _float& fTimeDelta, 
														const DWORD _dwFlag, 
														const Engine::COL_OBJECTID _eColObjectID,
														const _float& _fColValue = 0.f,
														Engine::CGameObject* _pObject = nullptr);
	virtual HRESULT				SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	/*  Set  */
	void						Set_NewPos(const _vec3& _vPos) { m_vNewPos = _vPos; }
	void						Set_IsOnLand(const _bool& _bIsOnLand) { m_bIsOnLand = _bIsOnLand; }
	void						Set_CurState(const PLAYER_STATE& _ePlayerState) { m_eCurState = _ePlayerState; }
	void						Set_NextState(const PLAYER_STATE& _ePlayerState) { m_eNextState = _ePlayerState; }
	void						Reset_FallAccel(void) { m_fFallAccel = 0.01f; }
	void						Set_AABBCol(const _bool& _bAABBCol) { m_bAABBCol = _bAABBCol; }
	void						Set_SneakState(const _bool& _bState) { m_bSneakState = _bState; }
	void						Set_SitState(const _bool& _bState) { m_bSitState = _bState; }
	void						Set_FightState(const _bool& _bState) { m_bFightState = _bState; }
	void						Set_CheerState(const _bool& _bState) { m_bCheerState = _bState; }
	void						Set_CarryState(const _bool& _bState) { m_bCarryState = _bState; }
	void						Set_FirstPersonState(const _bool& _bState) { m_bFirstPersonState = _bState; }
	void						Set_DeadState(const _bool& _bState) { m_bDeadState = _bState; }
	void						Set_SparkRender(const _bool& _bState) { m_bSparkRender = _bState; }
	void						Set_EliteState(const _bool& _bState) { m_bEliteState = _bState; }
	void						Set_EliteFightState(const _bool& _bState) { m_bEliteFightState = _bState; }
	void						Set_StyxEliteRender(const _bool& _bState) { m_bStyxEliteRender = _bState; }
	void						Set_Translucent(const _bool& _bState) { m_bTranslucent = _bState; }
	void						Set_TrackPosition(const _double& _bTrack) { m_pMeshCom->Set_TrackPosition(_bTrack); }
	void						Set_AnimationSet(const _uint& _iIndex) { m_pMeshCom->Set_AnimationSet(_iIndex); }
	void						Set_InitPlayerLook(const _vec3* _vPos);

public:
	/*  Get  */
	CPlayerBehavior*			Get_PlayerBehavior() const { return m_pPlayerBehavior; }
	Engine::CDynamicMesh*		Get_EliteMeshCom() const { return m_pMeshCom_Elite; }
	CElite*						Get_CollToElite() const { return m_pCollToElite; }
	Engine::CGameObject*		Get_HideObject() const { return m_pHideObject; }
	const PLAYER_STATE&			Get_CurState() const { return m_eCurState; }
	const PLAYER_STATE&			Get_NextState() const { return m_eNextState; }
	const _bool&				Get_IsOnLand() const { return m_bIsOnLand; }
	const _bool&				Get_AABBCol() const { return m_bAABBCol; }
	const DWORD&				Get_ColTo() const { return m_eColTo; }
	const _bool&				Get_AniRateEnd(const _double& _dRate) const { return m_pMeshCom->Is_AnimationSetRateEnd(_dRate); }
	const _vec3&				Get_NewPos() const { return m_vNewPos; }
	const _bool&				Get_SneakState() const { return m_bSneakState; }
	const _bool&				Get_SitState() const { return m_bSitState; }
	const _bool&				Get_FightState() const { return m_bFightState; }
	const _bool&				Get_CheerState() const { return m_bCheerState; }
	const _bool&				Get_CarryState() const { return m_bCarryState; }
	const _bool&				Get_DeadState() const { return m_bDeadState; }
	const _bool&				Get_EliteState() const { return m_bEliteState; }
	const _bool&				Get_Translucent() const { return m_bTranslucent; }
	const _bool&				Get_EliteFightState() const { return m_bEliteFightState; }
	const _bool&				Get_StyxEliteRender() const { return m_bStyxEliteRender; }
	const _bool&				Get_FirstPersonState() const { return m_bFirstPersonState; }
	const Engine::COLLTYPE&		Get_IsCollide(void) const { return m_pSphereColliderCom->Get_IsCollide(); }
	

private:
	HRESULT						Add_Component();

public:
	void						StaticTransform_Update(void);
	void						Update_Gravity(const float& fTimeDelta);
	void						Update_IsOnLand(const _float& fTimeDelta);
	void						Update_State(void);

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CDynamicMesh*		m_pMeshCom_Elite = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CTransform*			m_pTransCom_Elite = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CCubeCollider*		m_pAABBColliderCom = nullptr;
	Engine::CSphereCollider*	m_pSphereColliderCom = nullptr;
	Engine::CNaviMesh*			m_pNaviMeshCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	CPlayerBehavior*			m_pPlayerBehavior = nullptr;
	CElite*						m_pCollToElite = nullptr;
	CSpark*						m_pSpark = nullptr;
	CPlayerSword*				m_pPlayerSword = nullptr;
	Engine::CGameObject*		m_pHideObject = nullptr;

private:
	_vec3						m_vNewPos;
	PLAYER_STATE				m_eCurState = PLAYER_STATE::PLAYER_STATE_END;
	PLAYER_STATE				m_eNextState = PLAYER_STATE::PLAYER_SLEEP;

private:
	_bool						m_bIsOnLand = TRUE;
	_float						m_fFallAccel = 0.01f;
	_bool						m_bCompare;
	_bool						m_bAABBCol = FALSE;
	DWORD						m_eColTo = Engine::COL_OBJECTID::COL_OBJECT_END;
	_bool						m_bRoadCliff = TRUE;
	_bool						m_bSneakState = FALSE;
	_bool						m_bSitState = FALSE;
	_bool						m_bFightState = FALSE;
	_bool						m_bCheerState = FALSE;
	_bool						m_bCarryState = FALSE;
	_bool						m_bSparkRender = FALSE;
	_bool						m_bFirstPersonState = FALSE;
	_bool						m_bDeadState = FALSE;
	_bool						m_bEliteState = FALSE;
	_bool						m_bStyxEliteRender = FALSE;
	_bool						m_bTranslucent = FALSE;
	_bool						m_bEliteFightState = FALSE;



	/*  Creation and destruction  */
public:
	static CPlayer*				Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // Player_h__
