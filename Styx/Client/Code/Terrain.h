#ifndef Terrain_h__
#define Terrain_h__

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CTerrainTex;
class CTexture;
class CTransform;
class CRenderer;
class COptimization;
class CShader;

END

class CTerrain : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain(void);

public:
	/*  General  */
	virtual HRESULT				Ready_Object(void) override;
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual	_int				LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void				PreRender_Object(void) override;
	virtual	void				Render_Object(void) override;
	virtual void				PostRender_Object(void) override;

public:
	/*  Choice  */
	virtual HRESULT				SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	HRESULT						Add_Component(void);

private:
	Engine::CTerrainTex*		m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::COptimization*		m_pOptimizationCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	Engine::INDEX32*			m_pIndex = nullptr;
	_ulong						m_dwTriCnt = 0;


	/*  Creation and destruction  */
public:
	static CTerrain*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void				Free(void) override;
};

#endif // Terrain_h__
