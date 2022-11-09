#ifndef Styx_h__
#define Styx_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;

END

class CStyx : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CStyx(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CStyx(void);

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
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;

private:
	_bool						m_bWalk = FALSE;


	/*  Creation and destruction  */
public:
	static CStyx*				Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // Styx_h__
