#ifndef Timer1_h__
#define Timer1_h__

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

class CTimer : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CTimer(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CTimer(void);

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
	void						Set_TimerActive() { m_bTimerActive = TRUE; }

private:
	HRESULT						Add_Component();

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_bool						m_bTimerActive = FALSE;
	_bool						m_bTimerStart = FALSE;
	_float						m_fAlpha = 0.f;
	_uint						m_uMinTime = 10;
	_float						m_fSecTime = 0.f;




	/*  Creation and destruction  */
public:
	static CTimer*			Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // Timer1_h__
