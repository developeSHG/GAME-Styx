#ifndef Sword_h__
#define Sword_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CSphereCollider;
class CShader;

class CDynamicMesh;

END

class CElite;

class CSword : public Engine::CGameObject
{
public:
	enum BORN { FINGER, SHEATH };

private:
	/*  Structor  */
	explicit CSword(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CSword(void);

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
	virtual _int				CollisionUpdate_Object(const _float& fTimeDelta, 
														const DWORD _dwFlag, 
														const Engine::COL_OBJECTID _eColObjectID,
														const _float& _fColValue = 0.f,
														Engine::CGameObject* _pObject = nullptr);
	virtual HRESULT				SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	/*  Set  */
	void						Set_ParentElite(CElite* _pElite) { m_pElite = _pElite; }
	void						Set_Born(const BORN& _eBorn);

private:
	HRESULT						Add_Component();

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CSphereCollider*	m_pSphereColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	CElite*						m_pElite = nullptr;
	Engine::CTransform*			m_pEliteTransCom = nullptr;
	Engine::CDynamicMesh*		m_pEliteDynamicMeshCom = nullptr;
	const _matrix*				m_pParentBoneMatrix = nullptr;
	const _matrix*				m_pParentWorldMatrix = nullptr;

private:
	BORN						m_eBorn = SHEATH;


	/*  Creation and destruction  */
public:
	static CSword*				Create(LPDIRECT3DDEVICE9 _pGraphicDev, CElite* _pElite);
private:
	virtual void				Free(void);
};


#endif // Sword_h__
