#ifndef ItemPortrait_h__
#define ItemPortrait_h__

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

class CItemPortrait : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CItemPortrait(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CItemPortrait(void);

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

private:
	HRESULT						Add_Component();

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_bool						m_bUpDown = TRUE;
	_float						m_fFrame = 0.f;



	/*  Creation and destruction  */
public:
	static CItemPortrait*		Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // ItemPortrait_h__
