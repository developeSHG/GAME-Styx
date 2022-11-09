#ifndef LowerTextTuto_h__
#define LowerTextTuto_h__

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

class CLowerTextTuto : public Engine::CGameObject
{
private:
	#define				TEXT_NUM 4

private:
	/*  Structor  */
	explicit CLowerTextTuto(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CLowerTextTuto(void);

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
	void						Set_Render() { m_bRender = TRUE; }

private:
	HRESULT						Add_Component();

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_bool						m_bRender = FALSE;
	_float						m_fTime = 0.f;
	_bool						m_bAlphaUpDown[TEXT_NUM] = { FALSE };
	_ushort						m_uWave = 0;
	_float						m_fAlpha[TEXT_NUM] = { 0.f, 0.f, 0.f, 0.f };
	_matrix						m_matWorld[TEXT_NUM];



	/*  Creation and destruction  */
public:
	static CLowerTextTuto*	Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // LowerTextTuto_h__
