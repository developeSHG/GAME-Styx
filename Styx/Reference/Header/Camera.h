#ifndef Camera_h__
#define Camera_h__

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	/*  Structor  */
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera(void);

public:
	/*  General  */
	virtual	HRESULT			Ready_Object(void);
	virtual	_int			Update_Object(const _float& fTimeDelta);
	virtual void			PreRender_Object(void);
	virtual	void			Render_Object(void);
	virtual void			PostRender_Object(void);

public:
	/*  Set  */
	void					SetViewSpaceMatrix(const D3DXVECTOR3* pEye,
												const D3DXVECTOR3* pAt,
												const D3DXVECTOR3* pUp);
	void					SetProjectionMatrix(const float& fFovY, 
												const float& fAspect,
												const float& fNear, 
												const float& fFar);
	void					Set_OwnerShip(const _bool _bOwner) { m_bOwnerShip = _bOwner; }
	void					Set_Eye(const D3DXVECTOR3* pEye) { m_vEye = *pEye; }
	void					Set_At(const D3DXVECTOR3* pAt) { m_vAt = *pAt; }

public:
	/*  Get  */
	const _matrix			GetViewSpaceMatrix() const { return m_matView; }
	const _matrix			GetProjectionMatrix() const { return m_matProj; }
	const _vec3				Get_Eye() const { return m_vEye; }
	const _vec3				Get_At() const { return m_vAt; }
	const _bool				Get_OwnerShip() const { return m_bOwnerShip; }


public:
	void					TransformViewSpaceMatrix();
	void					TransformProjectionMatrix();

protected:
	_matrix					m_matView, m_matProj;

protected:
	_vec3					m_vEye, m_vAt, m_vUp;
	_float					m_fFovY, m_fAspect, m_fNear, m_fFar;
	_bool					m_bOwnerShip;


public:
	virtual void			Free(void);
};

END

#endif // Camera_h__
