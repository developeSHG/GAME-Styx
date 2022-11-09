#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading(void)
{

}

_uint	APIENTRY CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_LOGO:
		iFlag = pLoading->Loading_ForLogo();
		break;

	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	_endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_LoadingID = eLoadingID;
	
	return S_OK;
}

_uint CLoading::Loading_ForLogo(void)
{
	/*  ResourceMgr MapContainer Size Init  */
	Engine::Reserve_ContainerSize(RESOURCEID::RESOURCE_END);

	/*  Buffer  */
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);

	/*  Texture  */
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Alpha", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/StyxBG_Alpha_00%d.png", 1), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Focus", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Focus.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Cyanide", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Cyanide.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Enreal", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Enreal.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Jusin", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Jusin.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Pegi", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Pegi.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Ps4", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Ps4.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_CopyRight", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/CopyRight.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_StyxTitle", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/StyxTitle_00%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_MeshLoading", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/MeshLoading_00%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_PressEnter", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/PressEnter_00%d.png", 11), E_FAIL);

	/*  Static Mesh  */
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_SkyBoxL", Engine::TYPE_STATIC, L"../Bin/Resource/Mesh/StaticMesh/SkyBoxL/", L"Skydom.x"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_SkyBoxWhiteL", Engine::TYPE_STATIC, L"../Bin/Resource/Mesh/StaticMesh/SkyBoxL/", L"SkydomWhite.x"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_DrawBridge_00", Engine::TYPE_STATIC, L"../Bin/Resource/Mesh/StaticMesh/AllTGA/", L"DrawBridge_00.x"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_Lamp_00", Engine::TYPE_STATIC, L"../Bin/Resource/Mesh/StaticMesh/AllTGA/", L"Lamp_00.x"), E_FAIL);
	/*  Dynamic Mesh  */
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_Styx", Engine::TYPE_DYNAMIC, L"../Bin/Resource/Mesh/DynamicMesh/Styx/", L"Styx.x"), E_FAIL);

	/*  Shader  */
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"NewShader_Default", Engine::COMPONENTTYPE::COM_NEWSHADER_DEFAULT), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"NewShader_Mesh", Engine::COMPONENTTYPE::COM_NEWSHADER_MESH), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"NewShader_Sky", Engine::COMPONENTTYPE::COM_NEWSHADER_SKY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component(Engine::COMPONENTID::ID_STATIC, L"NewShader_Terrain", Engine::COMPONENTTYPE::COM_NEWSHADER_TERRAIN), E_FAIL);

	m_bFinish = true;

	return 0;
}

_uint CLoading::Loading_ForStage(void)
{
	/*  Buffer  */
	lstrcpy(m_szLoading, L"Buffer Loading.........................");
	//FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TriCol", Engine::BUFFER_TRICOL), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_CubeTex", Engine::BUFFER_CUBETEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TerrainTex", Engine::BUFFER_TERRAINTEX, 100, 95, 1), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TerrainRcTex", Engine::BUFFER_TERRAINRCTEX), E_FAIL);
	
	/*  Texture  */
	lstrcpy(m_szLoading, L"Texture Loading.........................");
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Button", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Button_00%d.tga", 5), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Mission", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Mission_00%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Timer", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Timer.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Circle", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Circle_00%d.tga", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Portrait", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/PortraitCircle/PortraitCircle_%d.png", 29), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Item", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Item_00%d.png", 6), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_HpBar", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/HpBar_00%d.tga", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_MpBar", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/MpBar_00%d.tga", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_LowerText", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/LowerText_00%d.png", 20), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_LowerTextTuto", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/LowerTextTuto_00%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Blind", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Blind_00%d.tga", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_BlindEk", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Blind_Ek_00%d.png", 3), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Cursor", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Cursor.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Marker", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Marker_00%d.tga", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_EliteMarker", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/EliteMarker.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Terrain", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Stage/Terrain/Terrain.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_TorchFire", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Stage/TorchFire/TorchFire%d.png", 36), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Spark", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Stage/Spark/Spark_00%d.tga", 2), E_FAIL);
	/*FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Potion", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Potion_00%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Sand", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Sand_00%d.png", 1), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_MissionStats", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/StyxMissionStatsRight_00%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_PauseChallenge", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/UI/StyxPauseChallenge_00%d.png", 42), E_FAIL);*/

	lstrcpy(m_szLoading, L"Mesh Loading.........................");
	/*  Static Mesh  */
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_Stone", Engine::TYPE_STATIC, L"../Bin/Resource/StudyMesh/StaticMesh/TombStone/", L"TombStone.x"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_SkyBox", Engine::TYPE_STATIC, L"../Bin/Resource/Mesh/StaticMesh/SkyBox/", L"Sky_Night.x"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_Prison", Engine::TYPE_STATIC, L"../Bin/Resource/Mesh/StaticMesh/Prison/", L"Base.x"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_Catsle", Engine::TYPE_STATIC, L"../Bin/Resource/Mesh/StaticMesh/Catsle/", L"Dungeon2.x"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_StyxDagger", Engine::TYPE_STATIC, L"../Bin/Resource/Mesh/StaticMesh/StyxDagger/", L"StyxDagger_000.x"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_Broom", Engine::TYPE_STATIC, L"../Bin/Resource/Mesh/StaticMesh/AllTGA/", L"Broom_00.x"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_Map", Engine::TYPE_STATIC, L"../Bin/Resource/Mesh/StaticMesh/AllTGA/", L"Map_00.x"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_Sword", Engine::TYPE_STATIC, L"../Bin/Resource/Mesh/StaticMesh/AllTGA/", L"Sword_00.x"), E_FAIL);
	/*  Navi Mesh  */
	FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, L"Mesh_Navi", Engine::TYPE_NAVI, nullptr, nullptr), E_FAIL);

	/*  Read Data  */
	FAILED_CHECK_RETURN(Read_Data(), E_FAIL);

	lstrcpy(m_szLoading, L"Loading Complete!");
	m_bFinish = true;

	Engine::MyPlaySound(L"AllrightLetsKeepgoing.mp3", Engine::CHANNELID::UI);

	return 0;
}

HRESULT CLoading::Read_Data(void)
{
	HANDLE hFile = CreateFile(L"../../Data/Client - StaticMeshLoadList.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(NULL, TEXT("Load Failed"), L"System Message", MB_OK);
		return E_FAIL;
	}

	DWORD dwByte = 0;

	while (TRUE)
	{
		Engine::_tchar szMeshTag[128];
		ReadFile(hFile, szMeshTag, sizeof(szMeshTag), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		
		wstring strXFile = szMeshTag;
		strXFile.replace(0, 5, L"");
		strXFile += L".x";

		if (L"Lamp_00.x" == strXFile
			|| L"DrawBridge_00.x" == strXFile)
			continue;

		FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, szMeshTag, Engine::TYPE_STATIC, L"../Bin/Resource/Mesh/StaticMesh/AllTGA/", strXFile.c_str()), E_FAIL);
	}

	hFile = CreateFile(L"../../Data/Client - DynamicMeshLoadList.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(NULL, TEXT("Load Failed"), L"System Message", MB_OK);
		return E_FAIL;
	}

	while (TRUE)
	{
		Engine::_tchar szMeshTag[128];
		ReadFile(hFile, szMeshTag, sizeof(szMeshTag), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		wstring strDirectory = szMeshTag;
		strDirectory.replace(0, 5, L"");
		strDirectory = L"../Bin/Resource/Mesh/DynamicMesh/" + strDirectory + L"/";
		wstring strXFile = szMeshTag;
		strXFile.replace(0, 5, L"");
		strXFile += L".x";

		if (L"Styx.x" == strXFile)
			continue;

		FAILED_CHECK_RETURN(Engine::Ready_Meshe(m_pGraphicDev, RESOURCE_STAGE, szMeshTag, Engine::TYPE_DYNAMIC, strDirectory.c_str(), strXFile.c_str()), E_FAIL);
	}

	CloseHandle(hFile);
	

	return S_OK;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoadingID)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}

