#ifndef Stone_h__
#define Stone_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCollider;
class COptimization;

END

class CStone : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CStone(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CStone(void);

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
	virtual _int				CollisionUpdate_Object(const _float& fTimeDelta, const DWORD _dwFlag, Engine::CGameObject* _pObject = nullptr);

public:
	/*  Set  */

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_Material(void);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::COptimization*		m_pOptimizationCom = nullptr;


	/*  Creation and destruction  */
public:
	static CStone*				Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // Player_h__
