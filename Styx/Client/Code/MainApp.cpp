#include "stdafx.h"
#include "MainApp.h"

#include "SceneSelector.h"
#include "Management.h"
#include "Renderer.h"

CMainApp::CMainApp(void)
{

}

CMainApp::~CMainApp(void)
{

}

HRESULT CMainApp::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_ClineCloneMgr(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::CRenderer::GetInstance()->Ready_RenderTarget(m_pGraphicDev), E_FAIL);

	Engine::CRenderer::GetInstance()->Ready_RenderTarget(m_pGraphicDev);

	return S_OK;
}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	/*  DInput Update  */
	Engine::Set_InputDev();

	m_pManagementClass->Update_Scene(fTimeDelta);

	return UPDATE_OK;
}

_int CMainApp::LateUpdate_MainApp(const _float& fTimeDelta)
{
	if (0 == Engine::Get_CurScene())
		return UPDATE_OK;

	m_pManagementClass->LateUpdate_Scene(fTimeDelta);

	/************************ (SongSong) ************************/
	/*  FillMode Change  */
	if (Engine::Get_DIKeyDown(DIK_P))
	{
		g_eFillMode = (g_eFillMode == D3DFILL_SOLID) ? D3DFILL_WIREFRAME : D3DFILL_SOLID;
		Engine::CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_FILLMODE, g_eFillMode);
	}

	return UPDATE_OK;
}

void CMainApp::Render_MainApp(void)
{
	//if (_ushort(SCENEID::SCENE_INTRO) == Engine::Get_CurScene())
	//{
	//	Engine::Render_Scene();
	//	return;
	//}
		if (0 == Engine::Get_CurScene())
		return;

	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 0.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	Engine::Render_End();
}


HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* _ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd,
												Engine::MODE_FULL,
												WINCX,
												WINCY,
												&m_pDeviceClass),
												E_FAIL);

	m_pDeviceClass->AddRef();

	*_ppGraphicDev = m_pDeviceClass->GetDevice();
	(*_ppGraphicDev)->AddRef();

	/*  Filter Set  */
	(*_ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*_ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	/************************ (SongSong) ************************/
	/*  DInput  */
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Font  */
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"굴림", 10, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_UI", L"서울남산체 M", 8, 20, FW_NORMAL), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_UI", L"리디바탕", 12, 17, FW_NORMAL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Timer", L"서울남산체 M", 8, 19, FW_NORMAL), E_FAIL);
	
	/*  Screen CenterPt Init  */
	//POINT CenterPt = { WINCX >> 1, WINCY >> 1 };
	ShowCursor(FALSE);
	//SetCursorPos(CenterPt.x, CenterPt.y);
	//ClientToScreen(g_hWnd, &CenterPt);
	
	return S_OK; 
}

HRESULT CMainApp::Ready_Component()
{
	Engine::Set_ComponentMgrGraphicDev(m_pGraphicDev);

	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform", Engine::COMPONENTTYPE::COM_TRANSFORM), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Calculator", Engine::COMPONENTTYPE::COM_CALCULATOR), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Optimization", Engine::COMPONENTTYPE::COM_OPTIMIZATION), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Mesh", Engine::COMPONENTTYPE::COM_SHADER_MESH), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Terrain", Engine::COMPONENTTYPE::COM_SHADER_TERRAIN), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Alpha", Engine::COMPONENTTYPE::COM_SHADER_ALPHA), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_ClineCloneMgr(void)
{
	/*CClientCloneMgr::Set_ClientCloneMgrGraphicDev(m_pGraphicDev);

	FAILED_CHECK_RETURN(Add_CloneGameObject(Engine::OBJECTID::OBJECT_PLAYER), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform", Engine::COMPONENTTYPE::COM_TRANSFORM), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Calculator", Engine::COMPONENTTYPE::COM_CALCULATOR), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Optimization", Engine::COMPONENTTYPE::COM_OPTIMIZATION), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Mesh", Engine::COMPONENTTYPE::COM_SHADER_MESH), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Terrain", Engine::COMPONENTTYPE::COM_SHADER_TERRAIN), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Alpha", Engine::COMPONENTTYPE::COM_SHADER_ALPHA), E_FAIL);
*/
	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::CManagement** _ppManagementClass)
{
	Engine::CScene*		pScene = nullptr;

	pScene = CSceneSelector(SCENEID::SCENE_INTRO)(&pScene, _pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Create_Management(_ppManagementClass), E_FAIL);

	(*_ppManagementClass)->AddRef();

	FAILED_CHECK_RETURN((*_ppManagementClass)->SetUp_Scene(pScene, _ushort(SCENEID::SCENE_INTRO)), E_FAIL);

	return S_OK;
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMainApp::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagementClass);

	Engine::Release_Utility();
	Engine::Release_System();
}

