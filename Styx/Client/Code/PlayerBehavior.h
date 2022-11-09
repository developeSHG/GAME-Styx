#ifndef PlayerBehavior_h__
#define PlayerBehavior_h__

#include "Export_Function.h"
#include "Include.h"

#include "Component.h"

BEGIN(Engine)


END

class CPlayer;
class CStaticCamera;
class CDynamicCamera;

class  CPlayerBehavior : public Engine::CComponent
{
private:
	//enum TURN_DIR { TURN_RIGHT180, TRUN_RIGHT90,};

private:
	/*  Structor  */
	explicit CPlayerBehavior();
	explicit CPlayerBehavior(const CPlayerBehavior& rhs);
	virtual ~CPlayerBehavior(void);

private:
	/*  General  */
	HRESULT					Ready_PlayerBehavior(void);
	virtual Engine::_int	Update_Component(const Engine::_float& fTimeDelta = 0.f);
	virtual Engine::_int	LateUpdate_Component(const Engine::_float& fTimeDelta = 0.f) { return UPDATE_OK; }

public:
	/*  Set  */
	void					Set_Player(CPlayer* pPlayer);
	void					Set_StaticCamera(CStaticCamera* _pStaticCamera) { m_pStaticCamera = _pStaticCamera; }
	void					Set_DynamicCamera(CDynamicCamera* _pDynamicCamera) { m_pDynamicCamera = _pDynamicCamera; }
	void					Set_NewPlayerPos(const _vec3& _vPos) { m_vNewPlayerPos = _vPos; }
	void					Set_NewPlayerLook(const _vec3& _vPos) { m_vNewPlayerLook = _vPos; }
	void					Reset_JumpAccel() { m_fJumpAccel = JUMP_ACCEL; if (m_bCheatJump) { m_fJumpAccel = 6.6f; } }
	void					Set_CameraPurpose(const _bool& _bState);
	void					Set_OnePlayAni(const _double& _dTrack) { m_bOnePlayAnimation = TRUE; m_dOnePlayAnimationSetEnd = _dTrack; }
	void					Set_OnePlayAniFalse() { m_bOnePlayAnimation = FALSE; m_dOnePlayAnimationSetEnd = 1; }
	void					Set_SinkTime(const _bool& _bActive) { m_bSinkTime = _bActive; }

public:
	/*  Get  */
	const _bool				Get_Moving() const { return m_bMoving; }

private:
	void					KeyInput(const float& fDeltaTime);
	void					Camera_Setting(const float& fDeltaTime);
	void					Change_MovingAnimation(const float& fDeltaTime);
	void					Update_Jump(const Engine::_float& fDeltaTime);
	void					FightStateKeyInput(const Engine::_float& fDeltaTime);

private:
	void					Update_WalkFront(const Engine::_float& fDeltaTime);
	void					Update_WalkBack(const Engine::_float& fDeltaTime);
	void					Update_WalkRight(const Engine::_float& fDeltaTime);
	void					Update_WalkLeft(const Engine::_float& fDeltaTime);

private:
	/*  Corner Moving  */
	void					Update_WalkRightFront(const Engine::_float& fDeltaTime);
	void					Update_WalkLeftFront(const Engine::_float& fDeltaTime);
	void					Update_WalkRightBack(const Engine::_float& fDeltaTime);
	void					Update_WalkLeftBack(const Engine::_float& fDeltaTime);

private:
	void					Update_PlayerAxis(const Engine::_float& fDeltaTime, Engine::_float fRotAngle);
	void					Update_Turn(void);
	void					Init_OnePlayAnimation(void);
	void					On_CheatKey(const Engine::_float& fDeltaTime);
	void					Update_Sink(const Engine::_float& fDeltaTime);
	void					Update_Roll(const Engine::_float& fDeltaTime);

public:
	void					Update_DropOut(void);

private:
	CStaticCamera*			m_pStaticCamera = nullptr;
	CDynamicCamera*			m_pDynamicCamera = nullptr;
	CPlayer*				m_pPlayer = nullptr;
	Engine::CDynamicMesh*	m_pPlayerMeshCom = nullptr;
	Engine::CTransform*		m_pPlayerTransCom = nullptr;

private:
	_matrix					m_matCameraWorld;
	_vec3					m_vNewPlayerPos;
	_vec3					m_vNewPlayerLook;
	_vec3					m_vCameraPos;
	
private:
	_bool					m_bMoving = FALSE;
	_float					m_fSpeed;
	_float					m_fRotAngle; //회전각, 회전속도
	_float					m_fJumpAccel;
	_bool					m_bTurn = FALSE;
	_bool					m_bOnePlayAnimation = FALSE;
	_double					m_dOnePlayAnimationSetEnd = 1.f;
	_bool					m_bSinkTime = FALSE;
	CFeature*				m_pDropItem = nullptr;
	_bool					m_bRunSound = false;
	_float					m_fRunSound = 0.f;
	_bool					m_bCheatJump = false;



	/*  Creation and destruction  */
public:
	static CPlayerBehavior*			Create();
public:
	virtual Engine::CComponent*		Clone(void);
public:
	virtual void					Free(void);
};


#endif // PlayerBehavior_h__