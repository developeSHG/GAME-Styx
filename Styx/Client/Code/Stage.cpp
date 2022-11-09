#include "stdafx.h"
#include "Stage.h"

CStage::CStage(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CScene(_pGraphicDev)
{

}

CStage::~CStage(void)
{
	
}

HRESULT CStage::Ready_Scene()
{
	Engine::PlayBGM(L"StageBGM.mp3");

	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	/*  Light Directional Init  */
	{
		tLightInfo.Type = D3DLIGHT_DIRECTIONAL; 

		tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		//tLightInfo.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
		//	tLightInfo.Ambient = D3DXCOLOR(0.12f, 0.12f, 0.12f, 1.f);
		tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
		tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);
	}
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	FAILED_CHECK_RETURN(Ready_GameLogicLayer(L"00.GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UILayer(L"01.UI"), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_DefaultSetting(), E_FAIL);

	return S_OK;
}

_int CStage::Update_Scene(const _float & fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);

	Engine::LateUpdate_CollisionMgr(fTimeDelta);

	return UPDATE_OK;
}

_int CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
	Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return UPDATE_OK;
}

void CStage::Render_Scene(void)
{
	// DEBUG 출력
#ifdef _DEBUG
	//Engine::Render_Font(L"Font_Default", L"로딩완료", &_vec2(100.f, 100.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
#endif // _DEBUG

	//Engine::Get_NaviMgr()->Get_NaviMesh()->Render_NaviMesh();
}

HRESULT CStage::Ready_GameLogicLayer(const _tchar* pLayerTag)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	/*  Player Create  */
	pGameObject = CAbstractFactoryPattern<CPlayer>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"00.Player", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_PLAYER, pGameObject);

	/*  Dagger Create  */
	pGameObject = CAbstractFactoryPattern<CDagger>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Dagger", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_DAGGER, pGameObject);
	
	/*  Cutlass Create  */
	pGameObject = CAbstractFactoryPattern<CCutlass>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Cutlass", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_CUTLASS, pGameObject);

	/*  Stone Create  */
	//pGameObject = CAbstractFactoryPattern<CStone>::CreateObject(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_Object(L"Stone", pGameObject);

	/*  Monster Create  */
	//pGameObject = CAbstractFactoryPattern<CMonster>::CreateObject(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_Object(L"Monster", pGameObject);
	//Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_MONSTER, pGameObject);

	/*  SkyBox Create  */
	pGameObject = CAbstractFactoryPattern<CSkyBox>::CreateObject(m_pGraphicDev);
	dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Update_Component();
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"SkyBox", pGameObject);

	/*  SkyBox Create  */
	pGameObject = CAbstractFactoryPattern<CSkyBox>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Rotation(Engine::ROTATION::ROT_X, D3DXToRadian(-180.f));
	dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(180.f));
	dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Update_Component();
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
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Update_Component();
		pLayer->Add_Object(L"Prison1", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_PRISON, pGameObject);

		pGameObject = CAbstractFactoryPattern<CPrison>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Set_Pos(35.f, 0.f, 50.f);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Move_Pos(&_vec3(TERRAIN_NUM_X >> 1, 0.f, TERRAIN_NUM_Z / 2.9f));
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(225.f));
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Update_Component();
		pLayer->Add_Object(L"Prison2", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_PRISON, pGameObject);

		pGameObject = CAbstractFactoryPattern<CPrison>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Set_Pos(34.f, 0.f, -10.f);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Move_Pos(&_vec3(TERRAIN_NUM_X >> 1, 0.f, TERRAIN_NUM_Z / 2.9f));
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(315.f));
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Update_Component();
		pLayer->Add_Object(L"Prison3", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_PRISON, pGameObject);

		pGameObject = CAbstractFactoryPattern<CPrison>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Set_Pos(-34.f, 0.f, -10.f);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Move_Pos(&_vec3(TERRAIN_NUM_X >> 1, 0.f, TERRAIN_NUM_Z / 2.9f));
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(405.f));
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Update_Component();
		pLayer->Add_Object(L"Prison4", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_PRISON, pGameObject);
	}

	/*  Catsle Create  */
	pGameObject = CAbstractFactoryPattern<CCatsle>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Catsle", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_CATSLE, pGameObject);

	/************************ (SongSong) ************************/
	/*  Read Data Create  */
	Read_GameLogicData(pLayer);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_UILayer(const _tchar* pLayerTag)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	/*  UI  */
	{
		/*  Button1 Create  */
		pGameObject = CAbstractFactoryPattern<CButton>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Set_Pos(350.f, 150.f, 1.f);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Update_Component();
		static_cast<CButton*>(pGameObject)->Set_ButtonPosition(1);
		pLayer->Add_Object(L"Z.Button", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_BUTTON, pGameObject);

		/*  Button2 Create  */
		pGameObject = CAbstractFactoryPattern<CButton>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Set_Pos(350.f, 80.f, 1.f);
		dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Update_Component();
		static_cast<CButton*>(pGameObject)->Set_ButtonPosition(2);
		pLayer->Add_Object(L"Z.Button", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_BUTTON, pGameObject);
		
		/*  Mission Create  */
		pGameObject = CAbstractFactoryPattern<CMission>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"Mission", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_MISSION, pGameObject);

		/*  Timer Create  */
		pGameObject = CAbstractFactoryPattern<CTimer>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"Timer", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_TIMER, pGameObject);

		/*  ItemList Create  */
		pGameObject = CAbstractFactoryPattern<CItemList>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"ItemList", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_ITEMLIST, pGameObject);

		/*  ItemNumCircle Create  */
		pGameObject = CAbstractFactoryPattern<CItemNumCircle>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"ItemNumCircle", pGameObject);

		/*  ItemSelectCircle Create  */
		pGameObject = CAbstractFactoryPattern<CItemSelectCircle>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"ItemSelectCircle", pGameObject);

		/*  ItemPortrait Create  */
		pGameObject = CAbstractFactoryPattern<CItemPortrait>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"ItemPortrait", pGameObject);

		/*  HpBar Create  */
		pGameObject = CAbstractFactoryPattern<CHpBar>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"1.HpBar", pGameObject);

		/*  MpBar Create  */
		pGameObject = CAbstractFactoryPattern<CMpBar>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"1.MpBar", pGameObject);

		/*  Hp Create  */
		pGameObject = CAbstractFactoryPattern<CHp>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"Hp", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_HP, pGameObject);

		/*  Mp Create  */
		pGameObject = CAbstractFactoryPattern<CMp>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"Mp", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_MP, pGameObject);

		/*  LowerText Create  */
		pGameObject = CAbstractFactoryPattern<CLowerText>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"LowerText", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_LOWERTEXT, pGameObject);

		/*  LowerTextTuto Create  */
		pGameObject = CAbstractFactoryPattern<CLowerTextTuto>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"LowerTextTuto", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_LOWERTEXTTUTO, pGameObject);

		/*  Blind Create  */
		pGameObject = CAbstractFactoryPattern<CBlind>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_Object(L"0.Blind", pGameObject);
		Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_BLIND, pGameObject);

	}
	
	/*  Cursor Create  */
	pGameObject = CAbstractFactoryPattern<CCursor>::CreateObject(m_pGraphicDev);
	pLayer->Add_Object(L"Z.Cursor", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_CURSOR, pGameObject);

	/*  Marker Create  */
	pGameObject = CAbstractFactoryPattern<CMarker>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Z.Marker", pGameObject);

	/*  StaticCamera Create  */
	pGameObject = CAbstractFactoryPattern<CStaticCamera>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"StaticCamera", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_STATICCAMERA, pGameObject);
	
	/*  CameraHierarchy Create  */
	pGameObject = CAbstractFactoryPattern<CCameraHierarchy>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"CameraHierarchy", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_CAMERAHIERARCHY, pGameObject);

	/*  DynamicCamera Create  */
	pGameObject = CAbstractFactoryPattern<CDynamicCamera>::CreateObject(m_pGraphicDev);
	pLayer->Add_Object(L"DynamicCamera", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_DYNAMICCAMERA, pGameObject);

	/*  SceneApha Create  */
	pGameObject = CAbstractFactoryPattern<CSceneAlpha>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"ZZ.SceneAlpha", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_SCENEALPHA, pGameObject);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::SetUp_DefaultSetting()
{
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);		// Z버퍼에 기록을 함, 자동 정렬을 수행할지 묻는 함수 옵션
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	// Z버퍼에 기록을 할 지 말 지를 결정하는 옵션

	/*  뷰변환에서 모든 법선을 정규화 시킨다. 왜곡을 막기위해서 쓰는 상태  */
	//m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	return S_OK;
}

HRESULT CStage::Ready_LightInfo(void)
{
	/*  Light Point Init  */
	//{
	//	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
	//	tLightInfo.Type = D3DLIGHT_POINT;

	//	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.6235294f, 0.207843137f, 1.f);
	//	tLightInfo.Specular = tLightInfo.Diffuse;
	//	tLightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);

	//	//tLightInfo.Attenuation0 = 0.000000001f;										/*  광원으로부터 멀어질 떄의 감쇄값  */
	//	tLightInfo.Range = 5.f;													/*  조명이 미치는 범위  */
	//	tLightInfo.Position = _vec3(TERRAIN_NUM_X >> 1, 1.f, TERRAIN_NUM_Z / 1.9f);

	//	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);
	//}

	///*  Light Point Init  */
	//{
	//	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
	//	tLightInfo.Type = D3DLIGHT_POINT;

	//	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.6235294f, 0.207843137f, 1.f);
	//	tLightInfo.Specular = tLightInfo.Diffuse;
	//	tLightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);

	//	//tLightInfo.Attenuation0 = 0.000000001f;										/*  광원으로부터 멀어질 떄의 감쇄값  */
	//	tLightInfo.Range = 5.f;													/*  조명이 미치는 범위  */
	//	tLightInfo.Position = _vec3(TERRAIN_NUM_X >> 1, 1.f, TERRAIN_NUM_Z / 1.9f);

	//	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);
	//}

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_AMBIENT, 0x00202020);

	return S_OK;

}

HRESULT CStage::Read_GameLogicData(Engine::CLayer* _pLayer)
{
	HANDLE hFile = CreateFile(L"../../Data/FeatureObject.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(NULL, TEXT("Load Failed"), L"System Message", MB_OK);
		CloseHandle(hFile);
		return E_FAIL;
	}

	DWORD dwByte = 0;
	_ushort uEliteID = 0;

	while (TRUE)
	{
		FEATURE tFeature;
		ReadFile(hFile, &tFeature, sizeof(FEATURE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			CloseHandle(hFile);
			return E_FAIL;
		}

		Engine::CGameObject* pGameObject = nullptr;

		/*  Static Mesh  */
		if (Engine::MESHTYPE::TYPE_STATIC == tFeature.eMeshType)
		{
			pGameObject = CAbstractFactoryPattern<CFeature>::CreateFeatureObject(m_pGraphicDev, tFeature.szMeshTag);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);

			Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));

			dynamic_cast<CFeature*>(pGameObject)->Set_CatsleIn(tFeature.bCatsleIn);
			pTransform->m_vInfo[Engine::INFO::INFO_POS] = tFeature.vPosition;
			pTransform->m_vScale = tFeature.vScale;
			pTransform->m_vAngle = tFeature.vRotation;

			pTransform->Update_Component();

			wstring wstrObjectTag = tFeature.szMeshTag;
			wstrObjectTag.replace(0, 5, L"");
			_pLayer->Add_Object(wstrObjectTag.c_str(), pGameObject);

			/*  Object Item  */
			if (!(lstrcmp(tFeature.szMeshTag, L"Mesh_Map_00"))
				|| !(lstrcmp(tFeature.szMeshTag, L"Mesh_Dagger_00"))
				|| !(lstrcmp(tFeature.szMeshTag, L"Mesh_Cup_00"))
				|| !(lstrcmp(tFeature.szMeshTag, L"Mesh_Position_Hp_00"))
				|| !(lstrcmp(tFeature.szMeshTag, L"Mesh_Position_Mystery_00"))
				|| !(lstrcmp(tFeature.szMeshTag, L"Mesh_Poison_00"))
				|| !(lstrcmp(tFeature.szMeshTag, L"Mesh_Key_00"))
				|| !(lstrcmp(tFeature.szMeshTag, L"Mesh_StyxDagger_000")))
				Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_ITEM, pGameObject);
	
		}
		/*  Dynamic Mesh  */
		else if (Engine::MESHTYPE::TYPE_DYNAMIC == tFeature.eMeshType)
		{
			if (!(lstrcmp(tFeature.szMeshTag, L"Mesh_Monster_Elite")))
			{
				if (4 == uEliteID)
					uEliteID = 5;
				else if (5 == uEliteID)
					uEliteID = 4;

				pGameObject = CElite::Create(m_pGraphicDev, uEliteID);
				Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_ELITE, pGameObject);
				static_cast<CElite*>(pGameObject)->Set_NewPos(tFeature.vPosition);
				static_cast<CElite*>(pGameObject)->Update_CurIndex();
				
				if(4 != uEliteID
					&& 5 != uEliteID)
					uEliteID++;
				else if (4 == uEliteID)
					uEliteID = 6;
			}
			else if (!(lstrcmp(tFeature.szMeshTag, L"Mesh_Monster_Servant")))
			{
				if (0 == m_uServantNum)
				{
					pGameObject = CAbstractFactoryPattern<CServant_Quest>::CreateFeatureObject(m_pGraphicDev, tFeature.szMeshTag);
					Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_SERVANT_QUEST, pGameObject);
				}
				else
				{
					pGameObject = CAbstractFactoryPattern<CServant_Rescue>::CreateFeatureObject(m_pGraphicDev, tFeature.szMeshTag);
					Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_SERVANT_RESCUE, pGameObject);
				}

				m_uServantNum++;
			}
			else if (!(lstrcmp(tFeature.szMeshTag, L"Mesh_Door")))
			{
				pGameObject = CAbstractFactoryPattern<CDoor>::CreateFeatureObject(m_pGraphicDev, tFeature.szMeshTag);
				Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_DOOR, pGameObject);
			}
			else if (!(lstrcmp(tFeature.szMeshTag, L"Mesh_Cupboard")))
			{
				pGameObject = CAbstractFactoryPattern<CCupboard>::CreateFeatureObject(m_pGraphicDev, tFeature.szMeshTag);
				Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_CUPBOARD, pGameObject);
			}
			else if (!(lstrcmp(tFeature.szMeshTag, L"Mesh_Lever")))
			{
				pGameObject = CAbstractFactoryPattern<CLever>::CreateFeatureObject(m_pGraphicDev, tFeature.szMeshTag);
				Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_LEVER, pGameObject);
			}
			else if (!(lstrcmp(tFeature.szMeshTag, L"Mesh_Torch")))
			{
				pGameObject = CAbstractFactoryPattern<CTorch>::CreateFeatureObject(m_pGraphicDev, tFeature.szMeshTag);
				Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_TORCH, pGameObject);
			}
			else if (!(lstrcmp(tFeature.szMeshTag, L"Mesh_Trunk")))
			{
				pGameObject = CAbstractFactoryPattern<CTrunk>::CreateFeatureObject(m_pGraphicDev, tFeature.szMeshTag);
				Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_TRUNK, pGameObject);
			}
			else if (!(lstrcmp(tFeature.szMeshTag, L"Mesh_Grid")))
			{
				pGameObject = CAbstractFactoryPattern<CGrid>::CreateFeatureObject(m_pGraphicDev, tFeature.szMeshTag);
				Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_GRID, pGameObject);
			}

			NULL_CHECK_RETURN(pGameObject, E_FAIL);

			Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));

 			pTransform->m_vInfo[Engine::INFO::INFO_POS] = tFeature.vPosition;
			pTransform->m_vScale = tFeature.vScale;
			pTransform->m_vAngle = tFeature.vRotation;

			pTransform->Update_Component();

			wstring wstrObjectTag = tFeature.szMeshTag;
			wstrObjectTag.replace(0, 5, L"");

			if (!(lstrcmp(tFeature.szMeshTag, L"Mesh_Monster_Elite")))
			{
				pGameObject->LateInit_Object();
				_pLayer->Add_Object(L"0.Mesh_Monster_Elite", pGameObject);
			}
			else
				_pLayer->Add_Object(wstrObjectTag.c_str(), pGameObject);
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CStage* pInstance = new CStage(_pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStage::Free(void)
{
	Engine::CScene::Free();
}