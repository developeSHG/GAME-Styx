#ifndef Servant_Quest_h__
#define Servant_Quest_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CSphereCollider;
class COptimization;
class CShader;

END

class CServant_Quest : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CServant_Quest(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CServant_Quest(void);

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
	void						Set_MeshTag(Engine::_tchar* _pMeshTag) { m_pMeshTag = _pMeshTag; }
	void						Set_State(const SERVANT_STATE& _eState) { m_eState = _eState; }
	void						Set_CameraPurpose(const _bool& _bState);

public:
	/*  Get  */
	const SERVANT_STATE&		Get_State(void) const { return m_eState; }

private:
	HRESULT						Add_Component(void);
	void						Update_State(void);
	void						KeyInput(void);

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CSphereCollider*	m_pSphereColliderCom = nullptr;
	Engine::COptimization*		m_pOptimizationCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_tchar*						m_pMeshTag = nullptr;
	SERVANT_STATE				m_eState = SERVANT_STATE::SERVANT_CLEANING_LOOP;
	_ushort						m_uIdleOrder = 0;
	_bool						m_bQuestAccept = FALSE;
	_bool						m_bQuestAcceptKeyInput = FALSE;
	_ushort						m_uThankOrder = 0;



	/*  Creation and destruction  */
public:
	static CServant_Quest*		Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag);
private:
	virtual void				Free(void);
};


#endif // Servant_Quest_h__
