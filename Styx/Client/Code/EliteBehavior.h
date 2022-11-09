#ifndef EliteBehavior_h__
#define EliteBehavior_h__

#include "Export_Function.h"
#include "Include.h"

#include "Component.h"

BEGIN(Engine)


END

class CElite;
class CStaticCamera;
class CDynamicCamera;
class CPlayer;

class  CEliteBehavior : public Engine::CComponent
{
private:
	//enum TURN_DIR { TURN_RIGHT180, TRUN_RIGHT90,};

private:
	/*  Structor  */
	explicit CEliteBehavior();
	explicit CEliteBehavior(const CEliteBehavior& rhs);
	virtual ~CEliteBehavior(void);

private:
	/*  General  */
	HRESULT					Ready_EliteBehavior(void);
	virtual Engine::_int	Update_Component(const Engine::_float& fTimeDelta = 0.f);
	virtual Engine::_int	LateUpdate_Component(const Engine::_float& fTimeDelta = 0.f) { return UPDATE_OK; }

public:
	/*  Set  */
	void					Set_Elite(CElite* pElite);
	void					Set_Player(CPlayer* pPlayer);
	void					Set_StaticCamera(CStaticCamera* _pStaticCamera) { m_pStaticCamera = _pStaticCamera; }
	void					Set_DynamicCamera(CDynamicCamera* _pDynamicCamera) { m_pDynamicCamera = _pDynamicCamera; }
	void					Set_NewElitePos(const _vec3& _vPos) { m_vNewElitePos = _vPos; }
	void					Set_NewEliteLook(const _vec3& _vPos) { m_vNewEliteLook = _vPos; }
	void					Set_OnePlayAni(const _double& _dTrack) { m_bOnePlayAnimation = TRUE; m_dOnePlayAnimationSetEnd = _dTrack; }
	void					Set_SwordActiveOut();

public:
	/*  Get  */
	const _bool				Get_Moving() const { return m_bMoving; }

private:
	void					Ready_WayPoint();
	void					Update_ReturnWay(const Engine::_float& fTimeDelta);
	void					Update_Way(const Engine::_float& fTimeDelta);
	void					Init_OnePlayAnimation(const Engine::_float& fTimeDelta);
	void					Update_Turn(const Engine::_float& fTimeDelta);
	void					Update_MoveWay(const Engine::_float& fTimeDelta);
	void					Update_Scan(const Engine::_float& fTimeDelta);
	void					Scan_Info(_float _fY, _float _fDist, _float _fAngle);
	void					Update_CarryMoving(const Engine::_float& fTimeDelta);
	void					Update_FightState(const Engine::_float& fTimeDelta);
	void					Update_HideBehavior(const Engine::_float& fTimeDelta);
	void					Update_NoHideBehavior(const Engine::_float& fTimeDelta);
	_bool					Update_Story(const Engine::_float& fTimeDelta);


private:
	CElite*					m_pElite = nullptr;
	Engine::CDynamicMesh*	m_pEliteMeshCom = nullptr;
	Engine::CTransform*		m_pEliteTransCom = nullptr;
	CPlayer*				m_pPlayer = nullptr;
	Engine::CDynamicMesh*	m_pPlayerMeshCom = nullptr;
	Engine::CTransform*		m_pPlayerTransCom = nullptr;
	CStaticCamera*			m_pStaticCamera = nullptr;
	CDynamicCamera*			m_pDynamicCamera = nullptr;

private:
	_vec3					m_vNewElitePos;
	_vec3					m_vNewEliteLook;
	
private:
	_bool					m_bMoving = FALSE;
	_float					m_fSpeed;
	_float					m_fRotAngle; //회전각, 회전속도
	_bool					m_bOnePlayAnimation = FALSE;
	_double					m_dOnePlayAnimationSetEnd = 1.f;
	_bool					m_bSinkTime = FALSE;
	_ushort					m_uWayPointNum;
	_ushort					m_uWayPointCurPattern = 0;
	_ushort					m_uWayPointDestiPattern = 0;
	_vec3					m_vWayPointPos[3];
	_vec3					m_vWayReturnPos;
	_float					m_fAttackCoolTime = 0.f;
	_ushort					m_uAttackPattern = 0;
	_bool					m_bPoison = FALSE;
	_bool					m_bMoveStory = FALSE;
	_bool					m_bFightStory = FALSE;
	_bool					m_bFullStory = FALSE;
	_float					m_fTime = 0.f;


	/*  Creation and destruction  */
public:
	static CEliteBehavior*			Create();
public:
	virtual Engine::CComponent*		Clone(void);
public:
	virtual void					Free(void);
};


#endif // EliteBehavior_h__