#ifndef Torch_h__
#define Torch_h__

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

class CPlayer;
class CTorchFire;

class CTorch : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CTorch(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CTorch(void);

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
	virtual HRESULT				LateInit_Object(void);

public:
	/*  Set  */
	void						Set_MeshTag(Engine::_tchar* _pMeshTag) { m_pMeshTag = _pMeshTag; }
	void						Set_LightOnOff();
public:
	/*  Get  */

private:
	HRESULT						Add_Component(void);

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CSphereCollider*	m_pSphereColliderCom = nullptr;
	Engine::COptimization*		m_pOptimizationCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	CPlayer*					m_pPlayer = nullptr;
	CTorchFire*					m_pTorchFire = nullptr;
	_tchar*						m_pMeshTag = nullptr;
	_bool						m_bLightOn = TRUE;
	_ushort						m_uLightIndex;
	_bool						m_bFireSound = FALSE;
	_float						m_fFireSoundTime = 0.f;


	/*  Creation and destruction  */
public:
	static CTorch*				Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag);
private:
	virtual void				Free(void);
};


#endif // Torch_h__
