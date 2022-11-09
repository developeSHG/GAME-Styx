#ifndef TorchFire_h__
#define TorchFire_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CShader;

END

class CTorchFire : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CTorchFire(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CTorchFire(void);

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
	void						Set_ParentObejct(CGameObject* _pGameObject) { m_pParentObject = _pGameObject; }

private:
	HRESULT						Add_Component();

private:
	void						BillBoard(void);

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	CGameObject*				m_pParentObject = nullptr;

private:
	_float						m_fFrame = 0.f;



	/*  Creation and destruction  */
public:
	static CTorchFire*			Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pParentObject);
private:
	virtual void				Free(void);
};


#endif // TorchFire_h__
