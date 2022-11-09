#include "stdafx.h"
#include "ToolMgr.h"

#include "SceneSelector.h"

IMPLEMENT_SINGLETON(CToolMgr)

CToolMgr::CToolMgr(void)
{

}

CToolMgr::~CToolMgr(void)
{
	Free();
}

HRESULT CToolMgr::Ready_ToolMgr(HWND _hWnd /*= nullptr*/)
{
	m_hWnd = _hWnd;

	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Raady_Resources(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	return S_OK;
}

_int CToolMgr::Update_ToolMgr(const _float& fTimeDelta)
{
	/*  DInput Update  */
	Engine::Set_InputDev();

	m_pManagementClass->Update_Scene(fTimeDelta);
	m_pManagementClass->LateUpdate_Scene(fTimeDelta);

	return UPDATE_OK;
}

void CToolMgr::Render_ToolMgr(void)
{
	Engine::Render_Begin(D3DXCOLOR(42.f/255.f, 41.f / 255.f, 45.f / 255.f, 0.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	if (TOOL_NAVIMESH == m_eCurTool)
	{
		/*  NaviMesh Render  */
		//Engine::Get_NaviMgr()->Get_NaviMesh()->Render_NaviMesh();

		m_pNaviMeshTool->Update_PickingType();
		/*  Picking Index Font Render  */
		if (CNaviMeshTool::PICKING_TYPE::INSERT_POINT == m_pNaviMeshTool->m_ePickingType)
		{
			{
				_tchar szTemp[32];
				swprintf_s(szTemp, L"%d", m_pNaviMeshTool->m_iPointNumCheck);
				CString strPoint = L"Index No.";
				strPoint += szTemp;

				POINT		ptMouse{};
				GetCursorPos(&ptMouse);
				ScreenToClient(m_hWnd, &ptMouse);
				if (FALSE == m_pWorld->m_pDynamicCamera->m_bFixMouse)
				{
					Engine::Render_Font(L"Font_Default", strPoint, &_vec2(ptMouse.x + 15.f, ptMouse.y + 20.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					Engine::Render_Font(L"Font_Default", L"Fix Camera", &_vec2(100.f, 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
				}
				else
				{
					Engine::Render_Font(L"Font_Default", L"NoFix Camera", &_vec2(100.f, 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
				}
			}
		}
	}
	if (TOOL_CAMERA == m_eCurTool)
	{
		/*  Picking Index Font Render  */
		{
			_tchar szTemp[32];
			swprintf_s(szTemp, L"%d", m_pWorld->m_pCameraHierarchy->m_vecCameraEyePoint.size() + 1);
			CString strPoint = L"Section Point No.";
			strPoint += szTemp;

			POINT		ptMouse{};
			GetCursorPos(&ptMouse);
			ScreenToClient(m_hWnd, &ptMouse);

			if (FALSE == m_pWorld->m_pDynamicCamera->m_bFixMouse)
			{
				Engine::Render_Font(L"Font_Default", strPoint, &_vec2(ptMouse.x + 15.f, ptMouse.y + 20.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
				Engine::Render_Font(L"Font_Default", L"Fix Camera", &_vec2(100.f, 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			}
			else
			{
				Engine::Render_Font(L"Font_Default", L"NoFix Camera", &_vec2(100.f, 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			}
		}
	}

	Engine::Render_End();
}

HRESULT CToolMgr::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* _ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(m_hWnd,
		Engine::MODE_WIN,
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
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(AfxGetInstanceHandle(), m_hWnd), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Font  */
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"¹ÙÅÁ", 15, 20, FW_HEAVY), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Light  */
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	return S_OK;
}

HRESULT CToolMgr::Ready_Scene(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::CManagement** _ppManagementClass)
{
	Engine::CScene*		pScene = nullptr;

	pScene = CSceneSelector(TOOLSCENEID::TOOLSCENE_WORLD)(&pScene, _pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Create_Management(_ppManagementClass), E_FAIL);

	(*_ppManagementClass)->AddRef();

	FAILED_CHECK_RETURN((*_ppManagementClass)->SetUp_Scene(pScene, _ushort(TOOLSCENEID::TOOLSCENE_WORLD)), E_FAIL);

	m_pSpace = CSpace::Create(_pGraphicDev);
	NULL_CHECK_RETURN(m_pSpace, E_FAIL);

	return S_OK;
}

HRESULT CToolMgr::Raady_Resources(void)
{
	/*  ResourceMgr MapContainer Size Init  */
	Engine::Reserve_ContainerSize(RESOURCEID::RESOURCE_END);

	/*  Buffer  */
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TerrainTex", Engine::BUFFER_TERRAINTEX, TERRAIN_NUM_X, TERRAIN_NUM_Z, 1), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TerrainRcTex", Engine::BUFFER_TERRAINRCTEX), E_FAIL);

	/*  Texture  */
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Terrain", Engine::TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Stage/Terrain/Terrain.tga"), E_FAIL);

	/*  Static Mesh  */
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_SkyBox", Engine::TYPE_STATIC, L"../../Client/Bin/Resource/Mesh/StaticMesh/SkyBox/", L"Sky_Night.x"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_Prison", Engine::TYPE_STATIC, L"../../Client/Bin/Resource/Mesh/StaticMesh/Prison/", L"Base.x"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_Catsle", Engine::TYPE_STATIC, L"../../Client/Bin/Resource/Mesh/StaticMesh/Catsle/", L"Dungeon2.x"), E_FAIL);
	m_pMeshTool->Ready_StaticMeshList(L"../../Client/Bin/Resource/Mesh/StaticMesh/All/");
	m_pMeshTool->Ready_DynamicMeshList(L"../../Client/Bin/Resource/Mesh/DynamicMesh/");
	m_pAnimationTool->Ready_DynamicMeshList(L"../../Client/Bin/Resource/Mesh/DynamicMesh/");

	/*  Navi Mesh  */
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_Navi", Engine::TYPE_NAVI, nullptr, nullptr), E_FAIL);

	return S_OK;
}

HRESULT CToolMgr::Ready_Component(void)
{
	Engine::Set_ComponentMgrGraphicDev(m_pGraphicDev);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform", Engine::COMPONENTTYPE::COM_TRANSFORM), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Calculator", Engine::COMPONENTTYPE::COM_CALCULATOR), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Mesh", Engine::COMPONENTTYPE::COM_SHADER_MESH), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Terrain", Engine::COMPONENTTYPE::COM_SHADER_TERRAIN), E_FAIL);

	return S_OK;
}

HRESULT CToolMgr::Ready_LightInfo(void)
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//	tLightInfo.Ambient = D3DXCOLOR(0.12f, 0.12f, 0.12f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, Engine::LIGHTINDEX::LIGHT_POINT), E_FAIL);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

void CToolMgr::Free(void)
{
	for (auto& iter : m_mapFeatureVector)
	{
		for (auto Veciter : iter.second)
			Engine::Safe_Release(Veciter);
		iter.second.clear();
		iter.second.shrink_to_fit();
	}
	m_mapFeatureVector.clear();

	//for_each(m_mapFeatureVector.begin(), m_mapFeatureVector.end(), [&](auto& iter){
	//	for (auto Veciter : iter.second)
	//		Engine::Safe_Release(Veciter);
	//	iter.second.clear();
	//	iter.second.shrink_to_fit();
	//});
	//m_mapFeatureVector.clear();

	Engine::Safe_Release(m_pSpace);
	Engine::Safe_Release(m_pWorld);

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagementClass);	

	Engine::Release_Utility();
	Engine::Release_System();

	int a = 5;
}

