#ifndef Button_h__
#define Button_h__

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

class CButton : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CButton(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CButton(void);

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
	void						Set_ButtonSetting(const BUTTON_UI _eButtonUI, const _tchar* _szButtonTag);
	//void						Set_ButtonUI(const BUTTON_UI _eButtonUI) { m_eButtonUI = _eButtonUI; }
	//void						Set_ButtonTag(const _tchar* _szButtonTag) { lstrcpy(m_szButtonTag, _szButtonTag); }
	void						Set_ButtonPosition(const _bool& _bPosition) { m_bPosition = _bPosition; }

private:
	HRESULT						Add_Component();

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_bool						m_bButtonRender = FALSE;
	_bool						m_bPosition;
	BUTTON_UI					m_eButtonUI;
	_tchar						m_szButtonTag[64];



	/*  Creation and destruction  */
public:
	static CButton*				Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // Button_h__
