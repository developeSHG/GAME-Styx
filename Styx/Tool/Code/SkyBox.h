#ifndef SkyBox_h__
#define SkyBox_h__

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CShader;

END

class CSkyBox : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkyBox(void);

public:
	/*  General  */
	virtual HRESULT				Ready_Object(void) override;
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual void				PreRender_Object(void) override;
	virtual	void				Render_Object(void) override;
	virtual void				PostRender_Object(void) override;

public:
	/*  Choice  */
	virtual HRESULT				SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	HRESULT						Add_Component(void);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;
	

public:
	static CSkyBox*				Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void				Free(void) override;

};

#endif // SkyBox_h__
