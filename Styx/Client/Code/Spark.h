#ifndef Spark_h__
#define Spark_h__

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

class CPlayer;

class CSpark : public Engine::CGameObject
{
private:
	#define				SPARK_NUM 2

private:
	/*  Structor  */
	explicit CSpark(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CSpark(void);

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
	void						Set_Player(CPlayer* pPlayer);
	void						Set_InitScale(void);

private:
	HRESULT						Add_Component(void);

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::COptimization*		m_pOptimizationCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	CPlayer*					m_pPlayer = nullptr;
	Engine::CDynamicMesh*		m_pPlayerMeshCom = nullptr;
	Engine::CTransform*			m_pPlayerTransCom = nullptr;

private:
	_float						m_fSize = 0.12f;
	


	/*  Creation and destruction  */
public:
	static CSpark*		Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // Spark_h__
