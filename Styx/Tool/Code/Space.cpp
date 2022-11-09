#include "stdafx.h"
#include "Space.h"

#include "CameraPoint.h"
#include "CameraBehavior.h"

CSpace::CSpace(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CScene(_pGraphicDev)
{

}

CSpace::~CSpace(void)
{
	
}

HRESULT CSpace::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_EnvironmentLayer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer(L"GameLogic"), E_FAIL);					/*  Only MeshTool  */
	FAILED_CHECK_RETURN(Ready_UILayer(L"UI"), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_DefaultSetting(), E_FAIL);

	CToolMgr::GetInstance()->m_pSpace = this;

	return S_OK;
}

_int CSpace::Update_Scene(const _float & fTimeDelta)
{
	return Engine::CScene::Update_Scene(fTimeDelta);
}

_int CSpace::LateUpdate_Scene(const _float& fTimeDelta)
{
	Engine::CScene::LateUpdate_Scene(fTimeDelta);

	Engine::LateUpdate_CollisionMgr(fTimeDelta);

	return UPDATE_OK;
}

void CSpace::Render_Scene(void)
{
	// DEBUG ���
#ifdef _DEBUG
	//Engine::Render_Font(L"Font_Default", L"�ε��Ϸ�", &_vec2(100.f, 100.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
#endif // _DEBUG
}

HRESULT CSpace::Ready_EnvironmentLayer(const _tchar* pLayerTag)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	/*  Terrain Create  */
	pGameObject = CAbstractFactoryPattern<CTerrain>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"Terrain", pGameObject);
	Engine::Add_ObjectMgr(Engine::OBJECTID::OBJECT_TERRAIN, pGameObject);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CSpace::Ready_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CSpace::Ready_UILayer(const _tchar* pLayerTag)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	/*  AniDynamicCamera Create  */
	pGameObject = CAbstractFactoryPattern<CDynamicCamera>::CreateObject(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_Object(L"DynamicCamera", pGameObject);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CSpace::SetUp_DefaultSetting()
{
	//FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	

	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);		// Z���ۿ� ����� ��, �ڵ� ������ �������� ���� �Լ� �ɼ�
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	// Z���ۿ� ����� �� �� �� ���� �����ϴ� �ɼ�

	/*  �亯ȯ���� ��� ������ ����ȭ ��Ų��. �ְ��� �������ؼ� ���� ����  */
	//m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	return S_OK;
}

HRESULT CSpace::Ready_LightInfo(void)
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

	//	tLightInfo.Attenuation0 = 0.000000001f;										/*  �������κ��� �־��� ���� ���Ⱚ  */
	//	tLightInfo.Range = 10.f;													/*  ������ ��ġ�� ����  */
	//	tLightInfo.Position = _vec3(TERRAIN_NUM_X >> 1, 0.f, TERRAIN_NUM_Z / 1.9f);
	//}

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, Engine::LIGHTINDEX::STAGE_POINT), E_FAIL);
	m_pGraphicDev->SetLight(Engine::LIGHTINDEX::LIGHT_POINT, &tLightInfo);
	m_pGraphicDev->LightEnable(Engine::LIGHTINDEX::LIGHT_POINT, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_AMBIENT, 0x00202020);

	/*  �亯ȯ���� ��� ������ ����ȭ ��Ų��. �ְ��� �������ؼ� ���� ����  */
	//m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	return S_OK;

}

Engine::CGameObject* CSpace::Create_StaticMeshObject(_tchar* pMeshTag)
{
	/*  Feature Create  */
	Engine::CGameObject* pGameObject = CAbstractFactoryPattern<CFeature>::CreateFeatureObject(m_pGraphicDev, pMeshTag);
	NULL_CHECK_RETURN(pGameObject, nullptr);
	FAILED_CHECK_RETURN(m_mapLayer[L"GameLogic"]->Add_Object(pMeshTag, pGameObject), nullptr);

	return pGameObject;
}

Engine::CGameObject* CSpace::Create_DynamicMeshObject(_tchar* pMeshTag)
{
	/*  DynamicFeature Create  */
	Engine::CGameObject* pGameObject = CAbstractFactoryPattern<CDynamicFeature>::CreateFeatureObject(m_pGraphicDev, pMeshTag);
	NULL_CHECK_RETURN(pGameObject, nullptr);
	FAILED_CHECK_RETURN(m_mapLayer[L"GameLogic"]->Add_Object(pMeshTag, pGameObject), nullptr);

	return pGameObject;
}

CSpace* CSpace::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CSpace* pInstance = new CSpace(_pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSpace::Free(void)
{
	Engine::CScene::Free();
}