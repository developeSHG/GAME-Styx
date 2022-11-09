#ifndef LowerText_h__
#define LowerText_h__

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

class CLowerText : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CLowerText(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CLowerText(void);

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
	void						Set_Index(const _ushort& _uIndex);
	void						Set_RenderExit();
	void						Set_TipOneRender(const _bool& _bActive) { m_bTipOneRender = _bActive; }
	void						Set_MakeSoundActive(const _bool& _bActive) { m_bMakeSoundActive = _bActive; }

public:
	/*  Get  */
	const _bool&				Get_MakeSoundActive() const { return m_bMakeSoundActive; }

private:
	HRESULT						Add_Component();

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_bool						m_bRender = FALSE;
	_ushort						m_uIndex = 0;
	_float						m_fTime = 0.f;
	_bool						m_bAlphaUpDown = TRUE;
	_float						m_fAlpha = 0.f;
	_bool						m_bTipOneRender = TRUE;
	_bool						m_bMakeSoundActive = FALSE;



	/*  Creation and destruction  */
public:
	static CLowerText*	Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // LowerText_h__
