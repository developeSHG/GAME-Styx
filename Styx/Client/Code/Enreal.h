#ifndef Enreal_h__
#define Enreal_h__

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CVIBuffer;
class CTexture;
class CTransform;
class CRenderer;

END

class CEnreal : public Engine::CGameObject
{
private:
	explicit CEnreal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEnreal(void);

public:
	virtual HRESULT				Ready_Object(void) override;
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual	_int				LateUpdate_Object(const _float& fTimeDelta);
	virtual void				PreRender_Object(void);
	virtual	void				Render_Object(void);
	virtual void				PostRender_Object(void);

private:
	HRESULT						Add_Component(void);

private:
	Engine::CVIBuffer*			m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;

private:
	_float						m_fAlpha = 0.f;


public:
	static CEnreal*				Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void				Free(void) override;


};

#endif // Enreal_h__
