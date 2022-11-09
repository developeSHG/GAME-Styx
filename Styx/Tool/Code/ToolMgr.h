#ifndef ToolMgr_h__
#define ToolMgr_h__

#include "Engine_Include.h"
#include "Export_Function.h"
#include "Base.h"
#include "Include.h"

#include "ToolView.h"
#include "World.h"
#include "Space.h"
#include "MeshTool.h"
#include "AnimationTool.h"
#include "NaviMeshTool.h"
#include "CameraTool.h"
#include "LightTool.h"

class CToolView;
class CWorld;
class CSpace;
class CMeshTool;
class CAnimationTool;
class CNaviMeshTool;
class CCameraTool;
class CLightTool;

BEGIN(Engine)

class CGraphicDev;
class CManagement;

END

class CToolMgr : public CBase
{
	DECLARE_SINGLETON(CToolMgr)

private:
	explicit CToolMgr(void);
	virtual ~CToolMgr(void);

public:
	HRESULT						Ready_ToolMgr(HWND _hWnd = nullptr);
	_int						Update_ToolMgr(const _float& fTimeDelta);
	void						Render_ToolMgr(void);

public:
	HRESULT						SetUp_DefaultSetting(LPDIRECT3DDEVICE9* _ppGraphicDev);
	HRESULT						Ready_Scene(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::CManagement** _ppManagementClass);
	HRESULT						Raady_Resources(void);
	HRESULT						Ready_Component(void);
	HRESULT						Ready_LightInfo(void);
	
public:
	Engine::CGraphicDev*		m_pDeviceClass			= nullptr;
	Engine::CManagement*		m_pManagementClass		= nullptr;
	LPDIRECT3DDEVICE9			m_pGraphicDev			= nullptr;
	HWND						m_hWnd;

public:
	CToolView*					m_pToolView				= nullptr;
	CWorld*						m_pWorld				= nullptr;
	CSpace*						m_pSpace				= nullptr;
	CMeshTool*					m_pMeshTool				= nullptr;
	CAnimationTool*				m_pAnimationTool		= nullptr;
	CNaviMeshTool*				m_pNaviMeshTool			= nullptr;
	CCameraTool*				m_pCameraTool			= nullptr;
	CLightTool*					m_pLightTool			= nullptr;

public:
	/*  MeshTool  */
	typedef map<wstring, vector<Engine::CGameObject*>>	MAPMESHTYPE_MAPFEATUREVEC;
	MAPMESHTYPE_MAPFEATUREVEC				m_mapFeatureVector;


public:
	TOOLID						m_eCurTool;


	/*  Creation and destruction  */
public:
	virtual void				Free(void);		
};

#endif // ToolMgr_h__
