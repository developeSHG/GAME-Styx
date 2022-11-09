#include "stdafx.h"
#include "Logo.h"

#include "SceneSelector.h"
#include "Layer.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CScene(_pGraphicDev)
{

}

CLogo::~CLogo(void)
{

}

HRESULT CLogo::Ready_Scene()
{
	/*  If Logo First Start  */
	//FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev, RESOURCEID::RESOURCE_END), E_FAIL);

	Engine::PlayBGM(L"LogoBGM.mp3");

	FAILED_CHECK_RETURN(Ready_GameLogicLayer(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UILayer(L"UI"), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_DefaultSetting(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Loading Create (Multi Thread)  */
	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int CLogo::Update_Scene(const _float & fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{	
		m_pMeshLoading->Set_LoadingComplete();
		m_pPressEnter->Set_LoadingComplete();

		CSceneAlpha* pSceneAlpha = SCAST<CSceneAlpha*>(Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_SCENEALPHA)->back());
		if (1.9f <= dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_At().y)
			pSceneAlpha->Set_Fade(FALSE);

		if (FALSE == pSceneAlpha->Get_Fade()
			&& 1.f == pSceneAlpha->Get_Material().Diffuse.a)
		{
			Engine::CManagement* ppManagementClass = nullptr;
			FAILED_CHECK_RETURN(Engine::Create_Management(&ppManagementClass), UPDATE_NO);

			(ppManagementClass)->AddRef();

			/************************ (SongSong) ************************/
			/*  Scene Change is ObjectMgr Ref Clear  */
			Engine::Free_ObjectList();

			Engine::CScene*		pScene = this;
			pScene = CSceneSelector(SCENEID::SCENE_STAGE)(&pScene, m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, UPDATE_NO);

			FAILED_CHECK_RETURN((ppManagementClass)->SetUp_Scene(pScene, _ushort(SCENEID::SCENE_STAGE)), UPDATE_NO);

			return UPDATE_OK;
		}
	}

	return UPDATE_OK;
}

_int CLogo::LateUpdate_Scene(const _float& fTimeDelta)
{
	return Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CLogo::Render_Scene(void)
{
	/*  Debug Mode  */
#ifdef _DEBUG
	//Engine::Render_Font(L"Font_Default", m_pLoading->Get_String(), &_vec2(710.f, 500.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
#endif // _DEBUG
}

HRESULT CLogo::Ready_GameLogicLayer(const _tchar* pLayerTag)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;
	Engine::CTransform* pTransform = nullptr;

	/*  BackGround Create  */
	//pGameObject = CAbstractFactoryPattern<CBackGround>::CreateObject(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_Object(L"BackGround", pGameObject);
	//Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_BACKGROUND, pGameObject);

	/*  Styx Create  */
	pGameObject = CAbstractFactoryPattern<CStyx>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Styx", pGameObject);

	/*  DrawBridge Create  */
	pGameObject = CAbstractFactoryPattern<CDrawBridge>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"DrawBridge", pGameObject);

	/*  Title Create  */
	pGameObject = CAbstractFactoryPattern<CTitle>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Title", pGameObject);

	/*  TitleLight Create  */
	pGameObject = CAbstractFactoryPattern<CTitleLight>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"TitleLight", pGameObject);

	/*  TitleLight Create  */
	pGameObject = CAbstractFactoryPattern<CTitleLight>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"TitleLight", pGameObject);

	/*  MeshLoading Create  */
	pGameObject = CAbstractFactoryPattern<CMeshLoading>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_pMeshLoading = dynamic_cast<CMeshLoading*>(pGameObject);
	pLayer->Add_Object(L"MeshLoading", pGameObject);

	/*  PressEnter  */
	pGameObject = CAbstractFactoryPattern<CPressEnter>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_pPressEnter = dynamic_cast<CPressEnter*>(pGameObject);
	pLayer->Add_Object(L"PressEnter", pGameObject);

	/*  Lamp Create  */
	pGameObject = CAbstractFactoryPattern<CFeature>::CreateFeatureObject(m_pGraphicDev, L"Mesh_Lamp_00");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pTransform = dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));
	pTransform->Set_Pos(0.f, -1.27f, 1.f);
	pTransform->Set_Scale(0.01f, 0.01f, 0.01f);
	pTransform->Update_Component();
	pLayer->Add_Object(L"Lamp_00", pGameObject);

	/*  SyBoxL Create  */
	pGameObject = CAbstractFactoryPattern<CSkyBoxL>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"SkyBoxL", pGameObject);

	/*  SyBoxWhiteL Create  */
	pGameObject = CAbstractFactoryPattern<CSkyBoxWhiteL>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"SkyBoxWhiteL", pGameObject);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CLogo::Ready_UILayer(const _tchar* pLayerTag)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	/*  CopyRight Create  */
	pGameObject = CAbstractFactoryPattern<CCopyRight>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"CopyRight", pGameObject);

	/*  Cyanide Create  */
	pGameObject = CAbstractFactoryPattern<CCyanide>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Cyanide", pGameObject);

	/*  Enreal Create  */
	pGameObject = CAbstractFactoryPattern<CEnreal>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Enreal", pGameObject);

	/*  Focus Create  */
	pGameObject = CAbstractFactoryPattern<CFocus>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Focus", pGameObject);

	/*  Jusin Create  */
	pGameObject = CAbstractFactoryPattern<CJusin>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Jusin", pGameObject);

	/*  Pegi Create  */
	pGameObject = CAbstractFactoryPattern<CPegi>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Pegi", pGameObject);

	/*  Ps4 Create  */
	pGameObject = CAbstractFactoryPattern<CPs4>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Ps4", pGameObject);

	/*  SceneApha Create  */
	pGameObject = CAbstractFactoryPattern<CSceneAlpha>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"SceneAlpha", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_SCENEALPHA, pGameObject);

	/*  DynamicCamera Create  */
	pGameObject = CAbstractFactoryPattern<CDynamicCamera>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"DynamicCamera", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_DYNAMICCAMERA, pGameObject);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CLogo::Ready_Resource(LPDIRECT3DDEVICE9 _pGraphicDev, RESOURCEID _eMax)
{
	_pGraphicDev->AddRef();

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(_pGraphicDev, RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_LogoBG", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/StyxLogoBG_00%d.png", 1), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_LogoFocus", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Logo_Focus.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_LogoCyanide", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Logo_Cyanide.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_LogoEnreal", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Logo_Enreal.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_LogoJusin", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Logo_Jusin.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_LogoPegi", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Logo_Pegi.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_LogoPs4", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Logo_Ps4.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_LogoText", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Text.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_StyxTitle", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/StyxTitle_00%d.png", 2), E_FAIL);

	Engine::Safe_Release(_pGraphicDev);

	return S_OK;
}

HRESULT CLogo::SetUp_DefaultSetting()
{
	/*  Light Init  */
	//D3DLIGHT9 dir;
	//ZeroMemory(&dir, sizeof(D3DLIGHT9));
	//dir.Type = D3DLIGHT_DIRECTIONAL;
	//dir.Ambient = WHITE * 1.f;
	//dir.Diffuse = WHITE;
	//dir.Specular = WHITE * 0.3f;
	//dir.Direction = D3DXVECTOR3(0.f, 0.f, 1.f);
	//m_pGraphicDev->SetLight(Engine::LIGHTINDEX::LOGO_DIRECTIONAL, &dir);				//★
	//m_pGraphicDev->LightEnable(Engine::LIGHTINDEX::LOGO_DIRECTIONAL, true);			//★

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	/*  Light Init  */
	tLightInfo.Type = D3DLIGHT_POINT;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(0.12f, 0.12f, 0.12f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);

	tLightInfo.Attenuation0 = 0.000000001f;			/*  광원으로부터 멀어질 떄의 감쇄값  */
	tLightInfo.Range = 6.1f;						/*  조명이 미치는 범위  */
	tLightInfo.Position = _vec3(0.f, 0.f, 1.4f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_AMBIENT, 0x00202020);

	/*  뷰변환에서 모든 법선을 정규화 시킨다. 왜곡을 막기위해서 쓰는 상태  */
	//m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{ 
	CLogo* pInstance = new CLogo(_pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLogo::Free(void)
{
	Engine::StopAll();

	Safe_Release(m_pLoading);

	Engine::CLightMgr::GetInstance()->Free();
	//Engine::CLightMgr::GetInstance()->DestroyInstance();
	//m_pGraphicDev->LightEnable(Engine::LIGHTINDEX::LIGHT_POINT, FALSE);

	Engine::CScene::Free();
}