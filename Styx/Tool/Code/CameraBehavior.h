#ifndef CameraBehavior_h__
#define CameraBehavior_h__

#include "Export_Function.h"
#include "Engine_Include.h"
#include "Include.h"

#include "Component.h"

BEGIN(Engine)

END

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

private:
	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;

public:
	_vec3							m_vEye, m_vAt;
	float							m_fItv = 0.f;
	int								m_iNum = 0;
	float							m_fSpeed;
	_bool							m_bPlay = FALSE;
	_bool							m_bPause = FALSE;


	/*  Creation and destruction  */
public:
	static CCameraBehavior*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual Engine::CComponent*		Clone(void);
public:
	virtual void					Free(void);
};

#endif // CameraBehavior_h__
