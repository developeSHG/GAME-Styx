#ifndef Marker_h__
#define Marker_h__

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

END

class CMarker : public Engine::CGameObject
{
private:
	#define				MARKER_NUM 4

private:
	/*  Structor  */
	explicit CMarker(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMarker(void);

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
	const _vec3*				m_pPos[MARKER_NUM];
	_matrix						m_matTrans[MARKER_NUM];
	_matrix						m_matScale[MARKER_NUM];
	_float						m_fSize[MARKER_NUM] = { 0 };
	


	/*  Creation and destruction  */
public:
	static CMarker*				Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // Marker_h__
