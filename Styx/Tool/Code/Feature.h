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
class CShader;

END

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
	virtual HRESULT				SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	/*  Set  */
	void						Set_MeshTag(Engine::_tchar* _pMeshTag) { m_pMeshTag = _pMeshTag; }

public:
	/*  Get  */
						
private:
	HRESULT						Add_Component(void);

public:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CSphereCollider*	m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_tchar*						m_pMeshTag = nullptr;

public:
	_bool						m_bPick = TRUE;
	_bool						m_bAlphaTest = FALSE;
	_bool						m_bCatsleIn;


	/*  Creation and destruction  */
public:
	static CFeature*			Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag);
private:
	virtual void				Free(void);
};


#endif // Feature_h__
