#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Include.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class CTransform;
class CDynamicMesh;
class CCubeCollider;
class CSphereCollider;


class ENGINE_DLL CGameObject : public CBase
{
protected:
	/*  Structor  */
	explicit	CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CGameObject(void);

public:
	/*  General  */
	virtual	HRESULT			Ready_Object(void);
	virtual	_int			Update_Object(const _float& fTimeDelta);
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta);
	virtual void			PreRender_Object(void);
	virtual	void			Render_Object(void);
	virtual void			PostRender_Object(void);

public:
	/*  Choice  */
	virtual _int			CollisionUpdate_Object(const _float& fTimeDelta, 
													const DWORD _dwFlag,
													const COL_OBJECTID _eColObjectID,
													const _float& _fColValue = 0.f,
													CGameObject* _pObject = nullptr) { return UPDATE_OK; }
	virtual HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect) { return S_OK; }
	virtual HRESULT			LateInit_Object(void);

public:
	/*  Set  */

public:
	/*  Get  */
	CComponent*				Get_Component(COMPONENTTYPE _eType);
	const OBJECTID			Get_OwnerObjectID() const { return m_eOwnerObjectID; }
	const COL_OBJECTID&		Get_ColObjectID() const { return m_eColObjectID; }
	const D3DMATERIAL9		Get_Material() const { return m_tMtrl; }
	const _float			Get_ViewZ(void) const { return m_fViewZ; }

protected:
	void					Compare_ViewZ(const _vec3* pPos);

protected:
	LPDIRECT3DDEVICE9									m_pGraphicDev = nullptr;
	map<const Engine::COMPONENTTYPE, CComponent*>		m_mapComponent[Engine::COMPONENTID::ID_END];

protected:
	OBJECTID				m_eOwnerObjectID = Engine::OBJECTID::OBJECT_END;
	COL_OBJECTID			m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_END;
	D3DMATERIAL9			m_tMtrl;
	_float					m_fTimeDelta;
	_float					m_fViewZ = 0.f;
	_bool					m_bLateInit = FALSE;


	/*  Creation and destruction  */
public:
	virtual CGameObject*	Clone_GameObject(void) { return nullptr; }
public:
	virtual void			Free(void);
};

END
#endif // GameObject_h__
