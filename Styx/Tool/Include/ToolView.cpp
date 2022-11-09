
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
	Engine::Safe_Release(m_pCalculator);
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_pToolMgr = CToolMgr::GetInstance();
	m_pToolMgr->Ready_ToolMgr(m_hWnd);
	//m_pToolMgr->m_hWnd = m_hWnd;
	
	m_pToolMgr->m_pToolView = this;

	m_pCalculator = Engine::CCalculator::Create(m_pToolMgr->m_pGraphicDev);

	NULL_CHECK(m_pCalculator);
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);

	if (TOOL_MESH == m_pToolMgr->m_eCurTool)
		MeshObjectCreate();
	else if (TOOL_NAVIMESH == m_pToolMgr->m_eCurTool)
		NaviMeshCreate();
	else if (TOOL_CAMERA == m_pToolMgr->m_eCurTool)
		CameraCreate();
	else if (TOOL_LIGHT == m_pToolMgr->m_eCurTool)
		LightCreate();
}

HRESULT CToolView::MeshObjectCreate()
{
	Get_Pointer();

	if (TRUE == m_pToolMgr->m_pMeshTool->m_bXFileFind && TRUE == m_pToolMgr->m_pMeshTool->m_bMeshTree[MESHTREE::DYNAMIC_MESH]
		|| TRUE == m_pToolMgr->m_pMeshTool->m_bXFileFind && TRUE == m_pToolMgr->m_pMeshTool->m_bMeshTree[MESHTREE::STATIC_MESH])
	{ 
		_vec3 vPickPos;

		/*  Mesh First, Terrain Second  */ 
		if (m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pCatsleMeshCom, m_pCatsleTransCom)
			|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[0])
			|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[1])
			|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[2])
			|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[3])
			|| m_pCalculator->Pick_OnTerrain(m_hWnd, &vPickPos, m_pTerrainBufferCom, m_pTerrainTransCom))
		{
			auto& iter = m_pToolMgr->m_mapFeatureVector.find(m_pToolMgr->m_pMeshTool->m_strMeshTag.GetString());
			if (iter == m_pToolMgr->m_mapFeatureVector.end())
			{
				m_pToolMgr->m_mapFeatureVector.emplace(m_pToolMgr->m_pMeshTool->m_strMeshTag.GetString(), vector<Engine::CGameObject*>());
			}

			Engine::CGameObject* pGameFeature = nullptr;
			if (TRUE == m_pToolMgr->m_pMeshTool->m_bMeshTree[MESHTREE::STATIC_MESH])
				pGameFeature = m_pToolMgr->m_pWorld->Create_StaticMeshObject((_tchar*)(m_pToolMgr->m_pMeshTool->m_strMeshTag.GetString()));
			else if (TRUE == m_pToolMgr->m_pMeshTool->m_bMeshTree[MESHTREE::DYNAMIC_MESH])
				pGameFeature = m_pToolMgr->m_pWorld->Create_DynamicMeshObject((_tchar*)(m_pToolMgr->m_pMeshTool->m_strMeshTag.GetString()));
			NULL_CHECK_RETURN(pGameFeature, E_FAIL);

			dynamic_cast<Engine::CTransform*>(pGameFeature->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM))->Set_Pos(vPickPos.x, vPickPos.y + 0.f, vPickPos.z);
			m_pToolMgr->m_mapFeatureVector[m_pToolMgr->m_pMeshTool->m_strMeshTag.GetString()].push_back(pGameFeature);

			if (TRUE == m_pToolMgr->m_pMeshTool->m_bMeshTree[MESHTREE::STATIC_MESH])
				m_pToolMgr->m_pMeshTool->Add_ObjectList(Engine::MESHTYPE::TYPE_STATIC);
			else if (TRUE == m_pToolMgr->m_pMeshTool->m_bMeshTree[MESHTREE::DYNAMIC_MESH])
				m_pToolMgr->m_pMeshTool->Add_ObjectList(Engine::MESHTYPE::TYPE_DYNAMIC);

			for_each(m_pToolMgr->m_mapFeatureVector.begin(), m_pToolMgr->m_mapFeatureVector.end(), [&](auto& iter) {
				if (std::string::npos == iter.first.find(L"0"))
				{
					for (auto Veciter : iter.second)
						dynamic_cast<CDynamicFeature*>(Veciter)->m_bPick = FALSE;
				}
				else
				{
					for (auto Veciter : iter.second)
						dynamic_cast<CFeature*>(Veciter)->m_bPick = FALSE;
				}
			});
			if (TRUE == m_pToolMgr->m_pMeshTool->m_bMeshTree[MESHTREE::STATIC_MESH])
			{
				dynamic_cast<CFeature*>(pGameFeature)->m_bPick = TRUE;
				dynamic_cast<CFeature*>(pGameFeature)->m_bCatsleIn = m_pToolMgr->m_pMeshTool->m_bCatsleIn;
			}
			else if (TRUE == m_pToolMgr->m_pMeshTool->m_bMeshTree[MESHTREE::DYNAMIC_MESH])
				dynamic_cast<CDynamicFeature*>(pGameFeature)->m_bPick = TRUE;
		}
	}

	return S_OK;
}

HRESULT CToolView::NaviMeshCreate(void)
{
	Get_Pointer();

	_vec3 vPickPos;

	/*  Mesh First, Terrain Second  */
	if (m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pCatsleMeshCom, m_pCatsleTransCom)
		|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[0])
		|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[1])
		|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[2])
		|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[3])
		|| m_pCalculator->Pick_OnTerrain(m_hWnd, &vPickPos, m_pTerrainBufferCom, m_pTerrainTransCom))
	{
		m_pToolMgr->m_pNaviMeshTool->PointCheck(vPickPos);		
	}

	return S_OK;
}

HRESULT CToolView::CameraCreate(void)
{
	Get_Pointer();

	_vec3 vPickPos;

	/*  Mesh First, Terrain Second  */
	if (m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pCatsleMeshCom, m_pCatsleTransCom)
		|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[0])
		|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[1])
		|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[2])
		|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[3])
		|| m_pCalculator->Pick_OnTerrain(m_hWnd, &vPickPos, m_pTerrainBufferCom, m_pTerrainTransCom))
	{
		m_pToolMgr->m_pWorld->m_pCameraHierarchy->Create_CameraPoint(vPickPos);

		m_pToolMgr->m_pCameraTool->TreeUpdate();
	}

	return S_OK;
}

HRESULT CToolView::LightCreate(void)
{
	Get_Pointer();

	_vec3 vPickPos;

	/*  Mesh First, Terrain Second  */
	if (m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pCatsleMeshCom, m_pCatsleTransCom)
		|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[0])
		|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[1])
		|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[2])
		|| m_pCalculator->Pick_OnMesh(m_hWnd, &vPickPos, m_pPrisonMeshCom, m_pPrisonTransCom[3])
		|| m_pCalculator->Pick_OnTerrain(m_hWnd, &vPickPos, m_pTerrainBufferCom, m_pTerrainTransCom))
	{
		D3DLIGHT9			tLightInfo;
		ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

		/*  Light Init  */
		tLightInfo.Type = D3DLIGHT_POINT;

		tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

		tLightInfo.Attenuation0 = 0.000000001f;			/*  광원으로부터 멀어질 떄의 감쇄값  */
		tLightInfo.Range = 5.f;							/*  조명이 미치는 범위  */
		tLightInfo.Position = vPickPos;

		DWORD dwIndex = Engine::LIGHTINDEX::LIGHT_POINT + m_pToolMgr->m_pLightTool->m_vecLight.size();
		m_pToolMgr->m_pGraphicDev->SetLight(dwIndex, &tLightInfo);
		m_pToolMgr->m_pGraphicDev->LightEnable(dwIndex, TRUE);

		m_pToolMgr->m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_pToolMgr->m_pGraphicDev->SetRenderState(D3DRS_AMBIENT, 0x00202020);

		LIGHT tLight;
		m_pToolMgr->m_pLightTool->m_vecLight.push_back(&tLight);
		/*if (TRUE == m_pToolMgr->m_pMeshTool->m_bMeshTree[MESHTREE::STATIC_MESH])
			m_pToolMgr->m_pMeshTool->Add_ObjectList(Engine::MESHTYPE::TYPE_STATIC);
		else if (TRUE == m_pToolMgr->m_pMeshTool->m_bMeshTree[MESHTREE::DYNAMIC_MESH])
			m_pToolMgr->m_pMeshTool->Add_ObjectList(Engine::MESHTYPE::TYPE_DYNAMIC);*/

			/*		if (TRUE == m_pToolMgr->m_pMeshTool->m_bMeshTree[MESHTREE::STATIC_MESH])
						dynamic_cast<CFeature*>(pGameFeature)->m_bPick = TRUE;
					else if (TRUE == m_pToolMgr->m_pMeshTool->m_bMeshTree[MESHTREE::DYNAMIC_MESH])
						dynamic_cast<CDynamicFeature*>(pGameFeature)->m_bPick = TRUE;*/
	}
	return S_OK;
}

HRESULT CToolView::Get_Pointer(void)
{
	/*  Catsle  */
	m_pCatsle = Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_CATSLE);

	m_pCatsleMeshCom = dynamic_cast<Engine::CStaticMesh*>
		(m_pCatsle->Get_Component(Engine::COMPONENTTYPE::COM_MESH));
	NULL_CHECK_RETURN(m_pCatsleMeshCom, E_FAIL);

	m_pCatsleTransCom = dynamic_cast<Engine::CTransform*>
		(m_pCatsle->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));
	NULL_CHECK_RETURN(m_pCatsleTransCom, E_FAIL);
	/*  Catsle  */

	/*  Prison  */
	m_pPrisonlist = Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_PRISON);

	m_pPrisonMeshCom = dynamic_cast<Engine::CStaticMesh*>
		(m_pPrisonlist->back()->Get_Component(Engine::COMPONENTTYPE::COM_MESH));
	NULL_CHECK_RETURN(m_pPrisonMeshCom, E_FAIL);

	WORD wNum = 0;
	for (auto& iter : *m_pPrisonlist)
	{
		m_pPrisonTransCom[wNum] = dynamic_cast<Engine::CTransform*>
			(iter->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));
		NULL_CHECK_RETURN(m_pPrisonTransCom[wNum], E_FAIL);
		wNum++;
	}
	/*  Prison  */

	/*  Terrain  */
	m_pTerrain = Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_TERRAIN);

	m_pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
		(m_pTerrain->Get_Component(Engine::COMPONENTTYPE::COM_BUFFER));
	NULL_CHECK_RETURN(m_pTerrainBufferCom, E_FAIL);

	m_pTerrainTransCom = dynamic_cast<Engine::CTransform*>
		(m_pTerrain->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));
	NULL_CHECK_RETURN(m_pTerrainTransCom, E_FAIL);
	/*  Terrain  */

	return S_OK;
}
