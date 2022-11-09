#ifndef CameraPoint_h__
#define CameraPoint_h__

#include "Engine_Include.h"
#include "Include.h"
#include "Struct.h"

#include "GameObject.h"


BEGIN(Engine)

class CTransform;
class CRenderer;
class CSphereCollider;

END

class CCameraPoint : public Engine::CGameObject
{
public:
	enum CAMERA_POINT_TYPE { CAMERA_POINT_EYE, CAMERA_POINT_AT };

private:
	/*  Structor  */
	explicit CCameraPoint(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CCameraPoint(void);

public:
	/*  General  */
	virtual	HRESULT					Ready_Object(void) override;
	virtual	_int					Update_Object(const _float& fTimeDelta) override;
	virtual	_int					LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void					PreRender_Object(void) override;
	virtual	void					Render_Object(void) override;
	virtual void					PostRender_Object(void) override;

public:
	/*  Set  */

public:
	/*  Get  */
						
private:
	HRESULT							Add_Component(void); 
	HRESULT							SetUp_Material(void);

public:
	Engine::CTransform*				m_pTransCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CSphereCollider*		m_pColliderCom = nullptr;

public:
	CAMERA_POINT_TYPE				m_ePointType;
	SPLINE							m_tSpline;



	/*  Creation and destruction  */
public:
	static CCameraPoint*			Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void					Free(void);
};


#endif // CameraPoint_h__
