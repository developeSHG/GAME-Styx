#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Export_Function.h"
#include "Engine_Include.h"
#include "Include.h"

#include "GameObject.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;

END

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
	void							Set_PlayBehavior(const _bool _bPlay) { m_bPlayBehavior = _bPlay; }

public:
	void							KeyInput(const _float& fTimeDelta);
	void							TargetRenewal(void);

public:
	const Engine::CTransform*		m_pTargetInfo = nullptr;
	
public:
	CCameraBehavior*				m_pCameraBehavior = nullptr;

public:
	_vec3							m_vAngle = { 0.f , 0.f, 0.f };
	_bool							m_bFixMouse = true;
	_bool							m_bPlayBehavior = false;



	/*  Creation and destruction  */
public:
	static CDynamicCamera*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void					Free(void);

};

#endif // DynamicCamera_h__
