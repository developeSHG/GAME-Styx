#ifndef CameraHierarchy_h__
#define CameraHierarchy_h__

#include "Engine_Include.h"
#include "Include.h"

#include "GameObject.h"

BEGIN(Engine)

class CRenderer;

END

class CCameraPoint;

class CCameraHierarchy : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CCameraHierarchy(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CCameraHierarchy(void);

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

public:
	HRESULT							Load_CameraWave1();
	HRESULT							Load_CameraWave2();
					
private:
	HRESULT							Add_Component(void); 
	HRESULT							SetUp_Material(void);

public:
	Engine::CRenderer*				m_pRendererCom = nullptr;
	LPD3DXLINE						m_pD3DXLine = nullptr;

public:
	vector<CCameraPoint*>			m_vecCameraEyePoint;     
	vector<CCameraPoint*>			m_vecCameraAtPoint;
	
public:
	_bool							m_bCameraWave2 = FALSE;
	

	/*  Creation and destruction  */
public:
	static CCameraHierarchy*		Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void					Free(void);
};


#endif // CameraRoad_h__
