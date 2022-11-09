#ifndef Space_h__
#define Space_h__

#include "Engine_Include.h"
#include "Export_Function.h"
#include "Include.h"
#include "Scene.h"

class CNaviTri;
class CCameraHierarchy;

class CSpace : public Engine::CScene
{
private:
	/*  Structor  */
	explicit CSpace(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CSpace(void);

public:
	/*  General  */
	virtual HRESULT				Ready_Scene(void) override;
	virtual _int				Update_Scene(const _float& fTimeDelta) override;
	virtual _int				LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void				Render_Scene(void) override;

public:
	HRESULT						Ready_EnvironmentLayer(const _tchar* pLayerTag);
	HRESULT						Ready_Layer(const _tchar* pLayerTag);
	HRESULT						Ready_UILayer(const _tchar* pLayerTag);

public:
	HRESULT						SetUp_DefaultSetting();
	HRESULT						Ready_LightInfo(void);

public:
	

public:
	/*  Animation Tool  */
	Engine::CGameObject*		Create_StaticMeshObject(_tchar* pMeshTag);
	Engine::CGameObject*		Create_DynamicMeshObject(_tchar* pMeshTag);

public:
	CNaviTri*					m_pNaviTri = nullptr;
	CCameraHierarchy*			m_pCameraHierarchy = nullptr;




	/*  Creation and destruction  */
public:
	static CSpace*				Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};

#endif // Space_h__
