#ifndef PressEnter_h__
#define PressEnter_h__

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CVIBuffer;
class CTexture;
class CTransform;
class CRenderer;

END

class CPressEnter : public Engine::CGameObject
{
private:
	explicit CPressEnter(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPressEnter(void);

public:
	virtual HRESULT				Ready_Object(void) override;
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual	_int				LateUpdate_Object(const _float& fTimeDelta);
	virtual void				PreRender_Object(void);
	virtual	void				Render_Object(void);
	virtual void				PostRender_Object(void);

private:
	HRESULT						Add_Component(void);

public:
	const void					Set_LoadingComplete() { m_bLoading = TRUE; }

private:
	Engine::CVIBuffer*			m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;

private:
	_float						m_fAlpha = 255.f;
	_float						m_fCount = 0.f;
	_bool						m_bAlpha = TRUE;
	_bool						m_bLoading = FALSE;
	_bool						m_bEnter = FALSE;


public:
	static CPressEnter*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void				Free(void) override;


};

#endif // PressEnter_h__
