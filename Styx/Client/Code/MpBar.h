#ifndef MpBar_h__
#define MpBar_h__

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

class CMpBar : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CMpBar(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMpBar(void);

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




	/*  Creation and destruction  */
public:
	static CMpBar*	Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // MpBar_h__
