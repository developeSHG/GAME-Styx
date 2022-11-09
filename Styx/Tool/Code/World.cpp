#include "stdafx.h"
#include "World.h"

#include "CameraPoint.h"
#include "CameraBehavior.h"

CWorld::CWorld(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CScene(_pGraphicDev)
{

}

CWorld::~CWorld(void)
{
	
}

HRESULT CWorld::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_EnvironmentLayer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer(L"GameLogic"), E_FAIL);					/*  Only MeshTool  */
	FAILED_CHECK_RETURN(Ready_UILayer(L"UI"), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_DefaultSetting(), E_FAIL);

	CToolMgr::GetInstance()->m_pWorld = this;

	return S_OK;
}

_int CWorld::Update_Scene(const _float & fTimeDelta)
{
	return Engine::CScene::Update_Scene(fTimeDelta);
}

_int CWorld::LateUpdate_Scene(const _float& fTimeDelta)
{
	Engine::CScene::LateUpdate_Scene(fTimeDelta);

	Engine::LateUpdate_CollisionMgr(fTimeDelta);

	return UPDATE_OK;
}

void CWorld::Render_Scene(void)
{
	// DEBUG 출력
#ifdef _DEBUG
	//Engine::Render_Font(L"Font_Default", L"로딩완료", &_vec2(100.f, 100.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
#endif // _DEBUG
}

HRESULT CWorld::Ready_EnvironmentLayer(const _tchar* pLayerTag)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	/*  Player Create  */
	//pGameObject = CAbstractFactoryPattern<CPlayer>::CreateObject(m_pGraphicDev);
	//pLayer->Add_Object(L"Player", pGameObject);
	//Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_PLAYER, pGameObject);

	/*  SkyBox Create  */
	pGameObject = CAbstractFactoryPattern<CSkyBox>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"SkyBox", pGameObject);

	/*  SkyBox Create  */
	pGameObject = CAbstractFactoryPattern<CSkyBox>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Rotation(Engine::ROTATION::ROT_X, D3DXToRadian(-180.f));
	dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(180.f));
	pLayer->Add_Object(L"SkyBox", pGameObject);

	/*  Terrain Create  */
	pGameObject = CAbstractFactoryPattern<CTerrain>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Terrain", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_TERRAIN, pGameObject);

	/*  Prison Create  */
	{
		pGameObject = CAbstractFactoryPattern<CPrison>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Set_Pos(-35.f, 0.f, 50.f);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Move_Pos(&_vec3(TERRAIN_NUM_X >> 1, 0.f, TERRAIN_NUM_Z / 2.9f));
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(135.f));
		pLayer->Add_Object(L"Prison1", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_PRISON, pGameObject);

		pGameObject = CAbstractFactoryPattern<CPrison>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Set_Pos(35.f, 0.f, 50.f);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Move_Pos(&_vec3(TERRAIN_NUM_X >> 1, 0.f, TERRAIN_NUM_Z / 2.9f));
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(225.f));
		pLayer->Add_Object(L"Prison2", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_PRISON, pGameObject);

		pGameObject = CAbstractFactoryPattern<CPrison>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Set_Pos(34.f, 0.f, -10.f);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Move_Pos(&_vec3(TERRAIN_NUM_X >> 1, 0.f, TERRAIN_NUM_Z / 2.9f));
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(315.f));
		pLayer->Add_Object(L"Prison3", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_PRISON, pGameObject);

		pGameObject = CAbstractFactoryPattern<CPrison>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Set_Pos(-34.f, 0.f, -10.f);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Move_Pos(&_vec3(TERRAIN_NUM_X >> 1, 0.f, TERRAIN_NUM_Z / 2.9f));
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(405.f));
		pLayer->Add_Object(L"Prison4", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_PRISON, pGameObject);
	}

	/*  Catsle Create  */
	pGameObject = CAbstractFactoryPattern<CCatsle>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Catsle", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_CATSLE, pGameObject);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CWorld::Ready_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CWorld::Ready_UILayer(const _tchar* pLayerTag)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	/*  SceneApha Create  */
	//pGameObject = CAbstractFactoryPattern<CSceneAlpha>::CreateObject(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_Object(L"SceneAlpha", pGameObject);
	//Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_SCENEALPHA, pGameObject);

	///*  StaticCamera Create  */
	//pGameObject = CAbstractFactoryPattern<CStaticCamera>::CreateObject(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_Object(L"StaticCamera", pGameObject);
	//Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_STATICCAMERA, pGameObject);

	/*  DynamicCamera Create  */
	pGameObject = CAbstractFactoryPattern<CDynamicCamera>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"DynamicCamera", pGameObject);
	m_pDynamicCamera = dynamic_cast<CDynamicCamera*>(pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_DYNAMICCAMERA, pGameObject);

	/*  CameraHierarchy Create  */
	pGameObject = CAbstractFactoryPattern<CCameraHierarchy>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_pCameraHierarchy = dynamic_cast<CCameraHierarchy*>(pGameObject);
	pLayer->Add_Object(L"CameraHierarchy", pGameObject);

	/*  NaviTri Create  */
	pGameObject = CAbstractFactoryPattern<CNaviTri>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_pNaviTri = dynamic_cast<CNaviTri*>(pGameObject);
	pLayer->Add_Object(L"NaviTri", pGameObject);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CWorld::SetUp_DefaultSetting()
{
	//FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	

	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);		// Z버퍼에 기록을 함, 자동 정렬을 수행할지 묻는 함수 옵션
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	// Z버퍼에 기록을 할 지 말 지를 결정하는 옵션

	/*  뷰변환에서 모든 법선을 정규화 시킨다. 왜곡을 막기위해서 쓰는 상태  */
	//m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	return S_OK;
}

HRESULT CWorld::Ready_LightInfo(void)
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	/*  Light Directional Init  */
	{
		tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

		tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		//	tLightInfo.Ambient = D3DXCOLOR(0.12f, 0.12f, 0.12f, 1.f);
		tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
		tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);
	}

	/*  Light Point Init  */
	//{
	//	tLightInfo.Type = D3DLIGHT_POINT;

	//	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//	//	tLightInfo.Ambient = D3DXCOLOR(0.12f, 0.12f, 0.12f, 1.f);
	//	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);

	//	tLightInfo.Attenuation0 = 0.000000001f;										/*  광원으로부터 멀어질 떄의 감쇄값  */
	//	tLightInfo.Range = 10.f;													/*  조명이 미치는 범위  */
	//	tLightInfo.Position = _vec3(TERRAIN_NUM_X >> 1, 0.f, TERRAIN_NUM_Z / 1.9f);
	//}

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, Engine::LIGHTINDEX::STAGE_POINT), E_FAIL);
	m_pGraphicDev->SetLight(Engine::LIGHTINDEX::LIGHT_POINT, &tLightInfo);
	m_pGraphicDev->LightEnable(Engine::LIGHTINDEX::LIGHT_POINT, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_AMBIENT, 0x00202020);

	/*  뷰변환에서 모든 법선을 정규화 시킨다. 왜곡을 막기위해서 쓰는 상태  */
	//m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	return S_OK;

}

Engine::CGameObject* CWorld::Create_StaticMeshObject(_tchar* pMeshTag)
{
	/*  Feature Create  */
	Engine::CGameObject* pGameObject = CAbstractFactoryPattern<CFeature>::CreateFeatureObject(m_pGraphicDev, pMeshTag);
	NULL_CHECK_RETURN(pGameObject, nullptr);
	FAILED_CHECK_RETURN(m_mapLayer[L"GameLogic"]->Add_Object(pMeshTag, pGameObject), nullptr);

	return pGameObject;
}

Engine::CGameObject* CWorld::Create_DynamicMeshObject(_tchar* pMeshTag)
{
	/*  DynamicFeature Create  */
	Engine::CGameObject* pGameObject = CAbstractFactoryPattern<CDynamicFeature>::CreateFeatureObject(m_pGraphicDev, pMeshTag);
	NULL_CHECK_RETURN(pGameObject, nullptr);
	FAILED_CHECK_RETURN(m_mapLayer[L"GameLogic"]->Add_Object(pMeshTag, pGameObject), nullptr);

	return pGameObject;
}

CWorld* CWorld::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CWorld* pInstance = new CWorld(_pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CWorld::Free(void)
{
	Engine::CScene::Free();
}