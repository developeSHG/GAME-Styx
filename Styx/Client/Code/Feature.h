#ifndef Feature_h__
#define Feature_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CSphereCollider;
class COptimization;
class CShader;

END

class CCameraBehavior;
class CElite;

class CFeature : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CFeature(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CFeature(void);

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
	/*  Choice  */
	virtual _int				CollisionUpdate_Object(const _float& fTimeDelta,
														const DWORD _dwFlag,
														const Engine::COL_OBJECTID _eColObjectID,
														const _float& _fColValue = 0.f,
														Engine::CGameObject* _pObject = nullptr);
	virtual HRESULT				SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	/*  Set  */
	void						Set_MeshTag(Engine::_tchar* _pMeshTag) { m_pMeshTag = _pMeshTag; }
	void						Set_CatsleIn(const _bool _bIn) { m_bCatsleIn = _bIn; }
	void						Set_Hide(const _bool& _bHide) { m_bHide = _bHide; }
	void						Set_Poison(const _bool& _b) { m_bPoison = _b; }
	void						Set_Drink(const _bool& _b) { m_bDrink = _b; }

public:
	/*  Get  */
	const _bool&				Get_Hide(void) const { return m_bHide; }
	const _ushort&				Get_ItemIndex(void) const { return m_uItemIndex; }
	const _bool&				Get_Poison(void) const { return m_bPoison; }

private:
	HRESULT						Add_Component(void);
	void						KeyInput_Item(void);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CSphereCollider*	m_pSphereColliderCom = nullptr;
	Engine::COptimization*		m_pOptimizationCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	CCameraBehavior*			m_pCameraBehavior = nullptr;

private:
	_tchar*						m_pMeshTag = nullptr;
	_bool						m_bAlphaTest = FALSE;
	_bool						m_bCatsleIn;
	_bool						m_bCollActive = FALSE;
	_bool						m_bHide = FALSE;
	_bool						m_bDropIn = FALSE;
	_ushort						m_uItemIndex = 0;
	_bool						m_bPoison = FALSE;
	_bool						m_bDrink = FALSE;
	_bool						m_bOneLowerText = FALSE;
	_bool						m_bSpecular = FALSE;


	/*  Creation and destruction  */
public:
	static CFeature*			Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag);
private:
	virtual void				Free(void);
};


#endif // Feature_h__
