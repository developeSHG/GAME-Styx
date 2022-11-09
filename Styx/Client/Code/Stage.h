#ifndef Stage_h__
#define Stage_h__

#include "Engine_Include.h"
#include "Export_Function.h"
#include "Include.h"
#include "Scene.h"


class CStage : public Engine::CScene
{
private:
	/*  Structor  */
	explicit CStage(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CStage(void);

public:
	// General
	virtual HRESULT				Ready_Scene(void) override;
	virtual _int				Update_Scene(const _float& fTimeDelta) override;
	virtual _int				LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void				Render_Scene(void) override;

private:
	//HRESULT					Ready_EnvironmentLayer(const _tchar* pLayerTag);
	HRESULT						Ready_GameLogicLayer(const _tchar* pLayerTag);
	HRESULT						Ready_UILayer(const _tchar* pLayerTag);

public:
	HRESULT						SetUp_DefaultSetting();
	HRESULT						Ready_LightInfo(void);

public:
	HRESULT						Read_GameLogicData(Engine::CLayer* _pLayer);

private:
	_matrix						m_matWorld, m_matView, m_matProj;
	_ushort						m_uServantNum = 0;


public:
	static CStage*				Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};

#endif // Stage_h__
