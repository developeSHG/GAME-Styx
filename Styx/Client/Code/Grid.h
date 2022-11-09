#ifndef Grid_h__
#define Grid_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCubeCollider;
class COptimization;
class CShader;

END

class CGrid : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CGrid(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CGrid(void);

public:
	/*  General  */
	virtual	HRESULT				Ready_Object(void) override;
	virtual	_int				Update_Object(const _float& fTimeDelta) override;
	virtual	_int				LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void				PreRender_Object(void) override;
	virtual	void				Render_Object(void) override;
	virtual void				PostRender_Object(void) override;

public:
	/*  Choice  */
	virtual HRESULT				SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	virtual _int				CollisionUpdate_Object(const _float& fTimeDelta,
														const DWORD _dwFlag,
														const Engine::COL_OBJECTID _eColObjectID,
														const _float& _fColValue = 0.f,
														Engine::CGameObject* _pObject = nullptr);

public:
	/*  Set  */
	void						Set_MeshTag(Engine::_tchar* _pMeshTag) { m_pMeshTag = _pMeshTag; }
	void						Set_Active() { m_bActive = TRUE; }
	void						Set_Active2() { m_bActive2 = TRUE; }
	void						Set_Open() { m_bOpen = TRUE; }

public:
	/*  Get  */
	const _bool&				Get_Active(void) const { return m_bActive; }
	const _bool&				Get_Active2(void) const { return m_bActive2; }
	const _bool&				Get_Open(void) const { return m_bOpen; }
	const _bool&				Get_CloseActive(void) const { return m_bCloseActive; }
						
private:
	HRESULT						Add_Component(void);

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CCubeCollider*		m_pAABBColliderCom = nullptr;
	Engine::COptimization*		m_pOptimizationCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_tchar*						m_pMeshTag = nullptr;
	_bool						m_bActive = FALSE;
	_bool						m_bActive2 = FALSE;
	_bool						m_bOpen = FALSE;
	_bool						m_bClose = FALSE;
	_bool						m_bCloseActive = FALSE;
	_double						m_dAccTime = 1;

	/*  Creation and destruction  */
public:
	static CGrid*				Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag);
private:
	virtual void				Free(void);
};


#endif // Grid_h__
