#ifndef Mission_h__
#define Mission_h__

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

class CMission : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CMission(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMission(void);

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
	void						Set_MissionChange() { m_bMissionChange = TRUE; }

private:
	HRESULT						Add_Component();

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_bool						m_bMissionChange = FALSE;
	_float						m_fAlpha[2] = { 255.f, 0.f };
	_vec3						m_vMissionChangePos;
	_float						m_fTime = 0.f;




	/*  Creation and destruction  */
public:
	static CMission*			Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // Mission_h__
