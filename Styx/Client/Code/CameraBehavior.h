#ifndef CameraBehavior_h__
#define CameraBehavior_h__

#include "Export_Function.h"
#include "Engine_Include.h"
#include "Include.h"

#include "Component.h"

BEGIN(Engine)

END

class CCameraHierarchy;

class CCameraBehavior : public Engine::CComponent
{
private:
	/*  Structor  */
	explicit CCameraBehavior(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCameraBehavior(const CCameraBehavior& rhs);
	virtual ~CCameraBehavior(void);

public:
	/*  General  */
	virtual Engine::_int			Update_Component(const Engine::_float& fTimeDelta = 0.f);
	virtual Engine::_int			LateUpdate_Component(const Engine::_float& fTimeDelta = 0.f) { return UPDATE_OK; }

public:
	/*  Set  */
	void							Set_Initialize(void);

public:
	/*  Get  */
	const WORD&						Get_Trigger() const { return m_wTrigger; }

public:
	void							Start_Camera(const Engine::_float& fTimeDelta = 0.f);
	_bool							Exit_Camera(const Engine::_float& fTimeDelta = 0.f);
	void							Delete_Info(void);

public:
	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;
	CCameraHierarchy*				m_pCameraHierarchy = nullptr;
	CStaticCamera*					m_pStaticCamera = nullptr;

public:
	_vec3							m_vEye, m_vAt;
	_bool							m_bStart = TRUE;
	_bool							m_bExit = FALSE;
	float							m_fItv = 0.f;
	int								m_iNum = 0;
	float							m_fSpeed;
	WORD							m_wTrigger = 0;


	/*  Creation and destruction  */
public:
	static CCameraBehavior*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual Engine::CComponent*		Clone(void);
public:
	virtual void					Free(void);
};

#endif // CameraBehavior_h__
