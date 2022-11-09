#ifndef SkyBoxWhiteL_h__
#define SkyBoxWhiteL_h__

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;

END

class CSkyBoxWhiteL : public Engine::CGameObject
{
private:
	explicit CSkyBoxWhiteL(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkyBoxWhiteL(void);

public:
	virtual HRESULT				Ready_Object(void) override;
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual void				PreRender_Object(void) override;
	virtual	void				Render_Object(void) override;
	virtual void				PostRender_Object(void) override;

private:
	HRESULT						Add_Component(void);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	
private:
	_float						m_fAlpha = 0.f;


	/*  Creation and destruction  */
public:
	static CSkyBoxWhiteL*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void				Free(void) override;


};

#endif // SkyBoxWhiteL_h__
