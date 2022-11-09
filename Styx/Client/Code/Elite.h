#ifndef Elite_h__
#define Elite_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

#include "DynamicMesh.h"
#include "SphereCollider.h"
#include "NaviMesh.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CSphereCollider;
class COptimization;
class CNaviMesh;
class CShader;

END

class CEliteBehavior;
class CSword;
class CBroom;
class CCup;
class CPlayer;
class CEliteMarker;

class CElite : public Engine::CGameObject
{
public:
	enum ELITE_PATTERN { PATTERN_END };

private:
	/*  Structor  */
	explicit CElite(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CElite(void);

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
	virtual HRESULT				LateInit_Object(void);

public:
	/*  Set  */
	void						Set_NewPos(const _vec3& _vPos) { m_vNewPos = _vPos; }
	void						Set_EliteID(const _ushort& _uID) { m_uEliteID = _uID; }
	void						Update_CurIndex(void) const { m_pNaviMeshCom->Update_CurIndex(m_vNewPos); }
	void						Set_IdleOrder(const _ushort& _uID) { m_uIdleOrder = _uID; }
	void						Set_OnMoveWay(const _bool& _bWay) { m_bOnMoveWay = _bWay; }
	void						Set_CurState(const ELITE_STATE& _eState) { m_eCurState = _eState; }
	void						Set_TrackPosition(const _double& _bTrack) { m_pMeshCom->Set_TrackPosition(_bTrack); }
	void						Set_Live(const _bool& _bLive) { m_bLive = _bLive; }
	void						Set_CarryState(const _bool& _bState) { m_bCarryState = _bState; }
	void						Set_SwordActive(const _bool& _bState) { m_bSwordActive = _bState; }
	void						Set_FightState(const _bool& _bState) { m_bFightState = _bState; }
	void						Set_ReturnState(const _bool& _bState) { m_bReturnState = _bState; }
	void						Set_Hide(const _bool& _bState) { m_bHide = _bState; }
	void						Set_CounterKey(const _bool& _bState) { m_bCounterKey = _bState; }
	void						Set_WorkChange(const _bool& _bState) { m_bWorkChange = _bState; }
	void						Set_PlayerAniOverlap(const _bool& _bState) { m_bPlayerAniOverlap = _bState; }
	void						Set_MakeSoundPattern(const _bool& _bState) { m_bMakeSoundPattern = _bState; }
	void						Set_PlayerCatchOriginY(const _float& _fY) { m_fPlayerOriginCatchY = _fY; }
	void						Set_FrontAss(const _bool& _bState) { m_bFrontAss = _bState; }
	void						Set_InitEliteLook(const _vec3* _vPos);
	
public:
	/*  Get  */
	CEliteBehavior*				Get_EliteBehavior() const { return m_pEliteBehavior; }
	const _ushort&				Get_EliteID() const { return m_uEliteID; }
	const _bool&				Get_AniRateEnd(const _double& _dRate) const { return m_pMeshCom->Is_AnimationSetRateEnd(_dRate); }
	const _bool&				Get_OnMoveWay() const { return m_bOnMoveWay; }
	const ELITE_STATE&			Get_CurState() const { return m_eCurState; }
	const _bool&				Get_SwordActive() const { return m_bSwordActive; }
	const _bool&				Get_Live() const { return m_bLive; }
	const _bool&				Get_CarryState() const { return m_bCarryState; }
	const _bool&				Get_FightState() const { return m_bFightState; }
	const _bool&				Get_ReturnState() const { return m_bReturnState; }
	const _bool&				Get_CounterKey() const { return m_bCounterKey; }
	const _bool&				Get_Hide() const { return m_bHide; }
	const _bool&				Get_WorkChange() const { return m_bWorkChange; }
	const _float&				Get_PlayerCatchOriginY() const { return m_fPlayerOriginCatchY; }
	const _bool&				Get_FrontAss() const { return m_bFrontAss; }
	const _bool&				Get_MakeSoundPattern() const { return m_bMakeSoundPattern; }
	CCup*						Get_Cup() const { return m_pCup; }

private:
	HRESULT						Add_Component(void);
	void						StaticTransform_Update(void);
	void						Update_State(void);
	void						Sink_PlayerCatch(void);

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CSphereCollider*	m_pSphereColliderCom = nullptr;
	Engine::COptimization*		m_pOptimizationCom = nullptr;
	Engine::CNaviMesh*			m_pNaviMeshCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_tchar*						m_pMeshTag = nullptr;
	CEliteBehavior*				m_pEliteBehavior = nullptr;
	CSword*						m_pSword = nullptr;
	CBroom*						m_pBroom = nullptr;
	CCup*						m_pCup = nullptr;
	CEliteMarker*				m_pEliteMarker = nullptr;

private:
	CPlayer*					m_pPlayer = nullptr;
	Engine::CTransform*			m_pPlayerTransCom = nullptr;
	Engine::CDynamicMesh*		m_pPlayerMeshCom = nullptr;

private:
	_vec3						m_vNewPos;
	ELITE_STATE					m_eCurState;
	ELITE_PATTERN				m_ePattern = ELITE_PATTERN::PATTERN_END;

private:
	_bool						m_bIsOnLand = TRUE;
	_bool						m_bCompare;
	_bool						m_bLive = TRUE;
	_bool						m_bOnMoveWay = TRUE;
	_bool						m_bSwordActive = FALSE;
	_bool						m_bFightState = FALSE;
	_bool						m_bCarryState = FALSE;
	_bool						m_bReturnState = FALSE;
	_ushort						m_uIdleOrder = 0;
	_ushort						m_uEliteID;
	_bool						m_bCounterKey = FALSE;
	_bool						m_bPlayerAniOverlap = FALSE;
	_float						m_fPlayerOriginCatchY = 0.f;
	_bool						m_bFrontAss = FALSE;
	_bool						m_bHide = FALSE;
	_bool						m_bWorkChange = FALSE;
	_bool						m_bMakeSoundPattern = FALSE;

public:
	_bool						m_bFightStateSound = FALSE;
	_bool						m_bDieSound = FALSE;


	/*  Creation and destruction  */
public:
	static CElite*				Create(LPDIRECT3DDEVICE9 _pGraphicDev, const _ushort& _uID);
private:
	virtual void				Free(void);
};


#endif // Elite_h__
