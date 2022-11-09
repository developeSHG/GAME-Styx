#ifndef Logo_h__
#define Logo_h__

#include "Engine_Include.h"
#include "Export_Function.h"
#include "Include.h"
#include "Scene.h"

#include "Loading.h"

class CMeshLoading;
class CPressEnter;

class CLogo : public Engine::CScene
{
private:
	/*  Structor  */
	explicit CLogo(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CLogo(void);

public:
	// General
	virtual HRESULT			Ready_Scene(void) override;
	virtual _int			Update_Scene(const _float& fTimeDelta) override;
	virtual _int			LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void			Render_Scene(void) override;

private:
	//HRESULT				Ready_EnvironmentLayer(const _tchar* pLayerTag);
	HRESULT					Ready_GameLogicLayer(const _tchar* pLayerTag);
	HRESULT					Ready_UILayer(const _tchar* pLayerTag);

private:
	HRESULT					Ready_Resource(LPDIRECT3DDEVICE9 _pGraphicDev, RESOURCEID _eMax);

public:
	HRESULT					SetUp_DefaultSetting();


private:
	CLoading*				m_pLoading = nullptr;

private:
	CMeshLoading*			m_pMeshLoading = nullptr;
	CPressEnter*			m_pPressEnter = nullptr;


public:
	static CLogo*			Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void			Free(void);
};


#endif // Logo_h__
