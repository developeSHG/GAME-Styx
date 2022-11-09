#ifndef Monster_h__
#define Monster_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CTriCol;
class CTexture;
class CTransform;
class CRenderer;

END

class CMonster : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CMonster(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMonster(void);

public:
	/*  General  */
	virtual	HRESULT				Ready_Object(void) override;
	virtual	_int				Update_Object(const _float& fTimeDelta) override;
	virtual	_int				LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void				PreRender_Object(void) override;
	virtual	void				Render_Object(void) override;
	virtual void				PostRender_Object(void) override;

private:
	HRESULT						Add_Component();

private:
	void						TargetMove(const _float& fTimeDelta);

private:
	Engine::CTriCol*			m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;

private:
	Engine::CTransform*			m_pTargetCom = nullptr;

private:
	_float						m_fSpeed = 3.5f;



	/*  Creation and destruction  */
public:
	static CMonster*			Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // Monster_h__
