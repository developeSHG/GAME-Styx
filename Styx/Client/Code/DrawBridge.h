#ifndef DrawBridge_h__
#define DrawBridge_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;

END

class CDrawBridge : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CDrawBridge(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CDrawBridge(void);

public:
	/*  General  */
	virtual	HRESULT				Ready_Object(void) override;
	virtual	_int				Update_Object(const _float& fTimeDelta) override;
	virtual	_int				LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void				PreRender_Object(void) override;
	virtual	void				Render_Object(void) override;
	virtual void				PostRender_Object(void) override;

public:
	/*  Set  */

public:
	/*  Get  */
						
private:
	HRESULT						Add_Component();
	HRESULT						SetUp_Material(void);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;


	/*  Creation and destruction  */
public:
	static CDrawBridge*			Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // DrawBridge_h__
