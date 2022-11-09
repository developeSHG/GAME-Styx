#ifndef EliteMarker_h__
#define EliteMarker_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class COptimization;
class CShader;

class CDynamicMesh;

END

class CElite;

class CEliteMarker : public Engine::CGameObject
{
private:
	#define				MARKER_NUM 4

private:
	/*  Structor  */
	explicit CEliteMarker(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEliteMarker(void);

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
	virtual HRESULT				SetUp_ConstantTable(LPD3DXEFFECT pEffect, _ushort _uIndex);

public:
	/*  Set  */
	void						Set_Elite(CElite* pElite);

private:
	HRESULT						Add_Component();

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::COptimization*		m_pOptimizationCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	CElite*						m_pElite = nullptr;
	Engine::CDynamicMesh*		m_pEliteMeshCom = nullptr;
	Engine::CTransform*			m_pEliteTransCom = nullptr;

private:
	_float						m_fSize = 0.12f;
	


	/*  Creation and destruction  */
public:
	static CEliteMarker*		Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // EliteMarker_h__
