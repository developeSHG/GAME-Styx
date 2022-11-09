#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "Component.h"

#include "Shader.h"

BEGIN(Engine)

class CLightMgr;
class CTarget_Manager;
class CVIBuffer_ViewPort;

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer(void);
	virtual ~CRenderer(void);

public:
	HRESULT						Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	void						Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
	void						Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	void						Clear_RenderGroup(void);

public:
	list<CGameObject*>*			Get_RendererGroup() { return m_RendererGroup; }

private:
	void						Render_Priority(LPDIRECT3DDEVICE9 pGraphicDev);
	void						Render_NonAlpha(LPDIRECT3DDEVICE9 pGraphicDev);
	void						Render_Alpha(LPDIRECT3DDEVICE9 pGraphicDev);
	void						Render_BlendTest(LPDIRECT3DDEVICE9 pGraphicDev);
	void						Render_Last(LPDIRECT3DDEVICE9 pGraphicDev);
	void						Render_UI(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT						Render_LightAcc();
	HRESULT						Render_Blend();


private:
	list<CGameObject*>			m_RendererGroup[RENDERID::RENDER_END];
	LPDIRECT3DDEVICE9			m_pGraphicDev;

private:
	CTarget_Manager*			m_pTarget_Manager = nullptr;
	CLightMgr*					m_pLight_Manager = nullptr;
	CShader*					m_pShader_LightAcc = nullptr;
	CShader*					m_pShader_Blend = nullptr;
	CVIBuffer_ViewPort*			m_pVIBuffer = nullptr;
	_bool						m_bRenderTargetView = FALSE;

	/*  Creation and destruction  */
private:
	virtual void				Free(void);
};

END
#endif // Renderer_h__
