#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Export_Function.h"
#include "Include.h"

#include "Camera.h"

BEGIN(Engine)

class CTransform;

END

class CStaticCamera;
class CCameraBehavior;

class CDynamicCamera : public Engine::CCamera
{
private:
	/*  Structor  */
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera(void);

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
	void							Set_Trailer(const _bool& _bTrailer) { m_bTrailer = _bTrailer; }
	void							Set_CameraPurpose(const CAMERA_PURPOSE& _ePurpose) { m_eCameraPurpose = _ePurpose; }
	void							Set_CameraPurposeStart(const _bool& _bStart) { m_bCameraPurposeStart = _bStart; }
	void							Set_CameraPurposeExit(const _bool& _bExit) { m_bCameraPurposeExit = _bExit; }

public:
	/*  Get  */
	CCameraBehavior*				Get_CameraBehavior() { return m_pCameraBehavior; }
	const CAMERA_PURPOSE&			Get_CameraPurpose(void) { return m_eCameraPurpose; }
	const _bool&					Get_CameraPurposeStart(void) const { return m_bCameraPurposeStart; }
	const _bool&					Get_CameraPurposeExit(void) const { return m_bCameraPurposeExit; }

private:
	void							TargetRenewal(const _float& fTimeDelta);

private:
	void							LogoSceneUpdate(const _float& fTimeDelta);
	void							Start_CameraPurpose(const DIR& _eDir, _vec3& _vPos, const Engine::_float& fTimeDelta = 0.f);
	void							Exit_CameraPurpose(const Engine::_float& fTimeDelta = 0.f);

private:
	void							Update_PurposeBehavior(const _float& fTimeDelta);
	void							Update_PurposeQuest(const _float& fTimeDelta);
	void							Update_PurposeDoor(const _float& fTimeDelta);
	void							Update_PurposeTrunk(const _float& fTimeDelta);
	void							Update_PurposeCupboard(const _float& fTimeDelta);
	void							Update_PurposeGrid(const _float& fTimeDelta);

private:
	SCENEID							m_eSceneID;

private:
	/*  Logo  */
	const Engine::CTransform*		m_pTargetInfo = nullptr;
	_bool							m_bTrailer = FALSE;

private:
	/*  Stage  */
	CStaticCamera*					m_pStaticCamera = nullptr;
	CCameraBehavior*				m_pCameraBehavior = nullptr;
	_bool							m_bPlayBehavior = false;
	CAMERA_PURPOSE					m_eCameraPurpose = CAMERA_PURPOSE::PURPOSE_END;
	_bool							m_bCameraPurposeStart = TRUE;
	_bool							m_bCameraPurposeExit = FALSE;
	_vec3							m_vAngle = {0.f, 0.f, 0.f};
	DIR								m_eDir = DIR::DIR_END;
	Engine::CTransform*				m_pColTransform = nullptr;
	_ushort							m_uColNum = 0;

public:
	static CDynamicCamera*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void					Free(void);

};

#endif // DynamicCamera_h__
