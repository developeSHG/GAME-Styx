#ifndef SceneAlpha_h__
#define SceneAlpha_h__

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CVIBuffer;
class CTexture;
class CTransform;
class CRenderer;

END

class CSceneAlpha : public Engine::CGameObject
{
private:
	explicit CSceneAlpha(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSceneAlpha(void);

public:
	virtual HRESULT				Ready_Object(void) override;
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual	_int				LateUpdate_Object(const _float& fTimeDelta);
	virtual void				PreRender_Object(void);
	virtual	void				Render_Object(void);
	virtual void				PostRender_Object(void);

public:
	/*  Set  */
	void						Set_Fade(const _bool _b) { m_bFade = _b; }
	void						Set_Dead(const _bool _b) { m_bDead = _b; }

public:
	/*  Get  */
	const _bool					Get_Fade() const { return m_bFade; }

private:
	HRESULT						Add_Component(void);

private:
	Engine::CVIBuffer*			m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;

private:
	/*  Scene Go = TRUE, Scene Out = FALSE  */
	_bool						m_bFade = true;
	_bool						m_bDead = FALSE;
	_float						m_fDeadTime = 0.f;


public:
	static CSceneAlpha*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void				Free(void) override;


};

#endif // SceneAlpha_h__
