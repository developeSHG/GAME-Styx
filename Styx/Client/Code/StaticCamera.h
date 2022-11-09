#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Export_Function.h"
#include "Include.h"

#include "Camera.h"

BEGIN(Engine)

class CTransform;
class CNaviMesh;

END

class CPlayer;

class CStaticCamera : public Engine::CCamera
{
#define	 MAX_DISTANCE	10.f
#define  MIN_DISTANCE	2.f

#define	 MAX_ANGLE_X	60.f
#define  MIN_ANGLE_X	-17.f

private:
	/*  Structor  */
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticCamera(void);

public:
	/*  General  */
	virtual	HRESULT					Ready_Object(void);
	virtual	_int					Update_Object(const _float& fTimeDelta);
	virtual	_int					LateUpdate_Object(const _float& fTimeDelta);
	virtual void					PreRender_Object(void);
	virtual	void					Render_Object(void);
	virtual void					PostRender_Object(void);

public:
	/*  Set  */
	void							Set_CameraPurpose(const CAMERA_PURPOSE& _ePurpose);
	void							Set_FightState(const _bool& _State);
	void							Set_CheerState(const _bool& _State) { m_bCheerState = _State; };
	void							Set_DeadState(const _bool& _State) { m_bDeadState = _State; };
	void							Set_DynamicQuestTrigger(const _bool& _State) { m_bDynamicQuestTrigger = _State; };
	void							Set_NewEye(_vec3* _vPos) { m_vNewEye = *_vPos; }
	void							Set_NewAt(_vec3* _vPos) { m_vNewAt = *_vPos; }
	void							Set_AngleX(_float _fAngle) { m_vAngle.x = _fAngle; }
	void							Set_AngleY(_float _fAngle) { m_vAngle.y = _fAngle; }
	void							Set_ElitePerson(_bool _bState) { m_bElitePerson = _bState; }

public:
	/*  Get	  */
	const _vec3						Get_Angle() const { return m_vAngle; }
	const _vec3						Get_NewEye() const { return m_vNewEye; }
	const _vec3						Get_NewAt() const { return m_vNewAt; }
	CAMERA_PURPOSE&					Get_CameraPurpose() { return m_eCameraPurpose; }
	const _bool						Get_ElitePerson() { return m_bElitePerson; }

private:
	void							KeyInput(const _float& fTimeDelta);
	void							TargetDistCheckRenewal(const _float& fTimeDelta);
	void							TargetRenewal(const _float& fTimeDelta);
	void							FirstNarrativeRenewal(void);

private:
	void							Update_DynamicTrigger(void);
	void							Update_CameraPurpose(void);

private:
	const Engine::CTransform*		m_pTargetCom	= nullptr;						/*  Player Transform Component  */
	Engine::CNaviMesh*				m_pNaviMeshCom = nullptr;
	CPlayer*						m_pPlayer		= nullptr;

private:
	_vec3							m_vNewEye;
	_vec3							m_vNewAt;
	_vec3							m_vAngle		= { 15.f, 0.f, 0.f };
	_float							m_fDistance		= 3.5f;
	_float							m_fNewDistance	= 3.5f;
	_float							m_fOriginDistance = 3.5f;
	_bool							m_bFixMouse		= true;
	_bool							m_bOnCamColl	= false;
	_bool							m_bDynamicTrigger[2] = { FALSE };
	CAMERA_PURPOSE					m_eCameraPurpose = CAMERA_PURPOSE::PURPOSE_END;
	_bool							m_bCameraMoving = FALSE;
	_bool							m_bFightState = FALSE;
	_vec3							m_vFightPrevAngle;
	_bool							m_bCheerState = FALSE;
	_bool							m_bDeadState = FALSE;
	_bool							m_bDynamicQuestTrigger = FALSE;
	_vec3							m_vOriginNewEye;
	_vec3							m_vOriginNewAt;
	_bool							m_bElitePerson = FALSE;

public:
	static CStaticCamera*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void					Free(void);

};

#endif // StaticCamera_h__
