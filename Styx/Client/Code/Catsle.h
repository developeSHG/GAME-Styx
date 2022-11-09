#ifndef Catsle_h__
#define Catsle_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CShader;

END

class CCameraBehavior;

class CCatsle : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CCatsle(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CCatsle(void);

public:
	/*  General  */
	virtual	HRESULT					Ready_Object(void) override;
	virtual	_int					Update_Object(const _float& fTimeDelta) override;
	virtual	_int					LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void					PreRender_Object(void) override;
	virtual	void					Render_Object(void) override;
	virtual void					PostRender_Object(void) override;

public:
	/*  Choice  */
	virtual HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	/*  Set  */

public:
	/*  Get  */

private:
	HRESULT							Add_Component(void);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CTransform*				m_pTransCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private:
	CCameraBehavior*				m_pCameraBehavior = nullptr;


	/*  Creation and destruction  */
public:
	static CCatsle*					Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void					Free(void);
};


#endif // Catsle_h__
