#ifndef NaviPoint_h__
#define NaviPoint_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CSphereCollider;

END

class CNaviPoint : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CNaviPoint(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CNaviPoint(void);

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
	Engine::NAVI_TYPE				m_eNaviType;
	_vec3							m_vPoint;
	_float							m_fRadius = 0.05f;
	_bool							m_bCreatePick = TRUE;
	_bool							m_bPick = FALSE;
	_bool							m_bCollTest = FALSE;
	_vec3							m_vCollSourPos;


	/*  Creation and destruction  */
public:
	static CNaviPoint*				Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void					Free(void);
};


#endif // NaviPoint_h__
