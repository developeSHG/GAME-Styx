#ifndef Cutlass_h__
#define Cutlass_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CSphereCollider;
class CShader;

END

class CElite;

class CCutlass : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CCutlass(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CCutlass(void);

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
	void						Set_Render(const _bool& _bActive) { m_bRender = _bActive; }
	void						Set_Throw(const _bool& _bActive);

public:
	/*  Get  */
	const _bool&				Get_Throw(void) { return m_bThrow; }

private:
	HRESULT						Add_Component();

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CSphereCollider*	m_pSphereColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	const	_matrix*			m_pParentBoneMatrix = nullptr;
	const	_matrix*			m_pParentWorldMatrix = nullptr;

private:
	_bool						m_bRender = FALSE;
	_bool						m_bThrow = FALSE;
	_ushort						m_uPattern = 0;
	CElite*						m_pElite = nullptr;


	/*  Creation and destruction  */
public:
	static CCutlass*				Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // Cutlass_h__
