// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

	

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ctrlToolTab);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnTcnSelchangeTabTool)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CString tab1 = L"Mesh";
	CString tab2 = L"Navi Mesh";
	CString tab3 = L"Animation";
	CString tab4 = L"Camera";
	CString tab5 = L"Light";
	CString tab6 = L"Trigger";

	m_ctrlToolTab.SetItemSize(CSize(40, 20));

	m_ctrlToolTab.InsertItem(1, tab1);
	m_ctrlToolTab.InsertItem(2, tab2);
	m_ctrlToolTab.InsertItem(3, tab3);
	m_ctrlToolTab.InsertItem(4, tab4);
	m_ctrlToolTab.InsertItem(5, tab5);
	//m_ctrlToolTab.InsertItem(6, tab6);

	CRect rect;
	m_ctrlToolTab.GetWindowRect(&rect);

	if (nullptr == m_MeshTool.GetSafeHwnd())
		m_MeshTool.Create(IDD_STATICMESHTOOL, &m_ctrlToolTab);
	if (nullptr == m_NaviMeshTool.GetSafeHwnd())
		m_NaviMeshTool.Create(IDD_NAVIMESHTOOL, &m_ctrlToolTab);
	if (nullptr == m_AnimationTool.GetSafeHwnd())
		m_AnimationTool.Create(IDD_ANIMATIONTOOL, &m_ctrlToolTab);
	if (nullptr == m_CameraTool.GetSafeHwnd())
		m_CameraTool.Create(IDD_CAMERATOOL, &m_ctrlToolTab);
	if (nullptr == m_LightTool.GetSafeHwnd())
		m_LightTool.Create(IDD_LIGHTTOOL, &m_ctrlToolTab);

	m_MeshTool.ShowWindow(SW_SHOW);
	CToolMgr::GetInstance()->m_eCurTool = TOOLID::TOOL_MESH;

	CToolMgr::GetInstance()->m_pMeshTool = &m_MeshTool;
	CToolMgr::GetInstance()->m_pNaviMeshTool = &m_NaviMeshTool;
	CToolMgr::GetInstance()->m_pAnimationTool = &m_AnimationTool;
	CToolMgr::GetInstance()->m_pCameraTool = &m_CameraTool;
	CToolMgr::GetInstance()->m_pLightTool = &m_LightTool;
}


void CMyForm::OnTcnSelchangeTabTool(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	int iTabIndex = m_ctrlToolTab.GetCurSel();

	switch (iTabIndex)
	{

	case 0:
		m_MeshTool.ShowWindow(SW_SHOW);
		m_NaviMeshTool.ShowWindow(SW_HIDE);
		m_AnimationTool.ShowWindow(SW_HIDE);
		m_CameraTool.ShowWindow(SW_HIDE);
		m_LightTool.ShowWindow(SW_HIDE);

		CToolMgr::GetInstance()->m_eCurTool = TOOLID::TOOL_MESH;
		CToolMgr::GetInstance()->m_pManagementClass->SetUp_Scene(CToolMgr::GetInstance()->m_pWorld, TOOLSCENEID::TOOLSCENE_WORLD);
		break;

	case 1:
		m_MeshTool.ShowWindow(SW_HIDE);
		m_NaviMeshTool.ShowWindow(SW_SHOW);
		m_AnimationTool.ShowWindow(SW_HIDE);
		m_CameraTool.ShowWindow(SW_HIDE);
		m_LightTool.ShowWindow(SW_HIDE);

		CToolMgr::GetInstance()->m_eCurTool = TOOLID::TOOL_NAVIMESH;
		CToolMgr::GetInstance()->m_pManagementClass->SetUp_Scene(CToolMgr::GetInstance()->m_pWorld, TOOLSCENEID::TOOLSCENE_WORLD);
		break;

	case 2:
		m_MeshTool.ShowWindow(SW_HIDE);
		m_NaviMeshTool.ShowWindow(SW_HIDE);
		m_AnimationTool.ShowWindow(SW_SHOW);
		m_CameraTool.ShowWindow(SW_HIDE);
		m_LightTool.ShowWindow(SW_HIDE);

		CToolMgr::GetInstance()->m_eCurTool = TOOLID::TOOL_ANIMATION;
		CToolMgr::GetInstance()->m_pManagementClass->SetUp_Scene(CToolMgr::GetInstance()->m_pSpace, TOOLSCENEID::TOOLSCENE_SPACE);
		break;
	
	case 3:
		m_MeshTool.ShowWindow(SW_HIDE);
		m_NaviMeshTool.ShowWindow(SW_HIDE);
		m_AnimationTool.ShowWindow(SW_HIDE);
		m_CameraTool.ShowWindow(SW_SHOW);
		m_LightTool.ShowWindow(SW_HIDE);

		CToolMgr::GetInstance()->m_eCurTool = TOOLID::TOOL_CAMERA;
		CToolMgr::GetInstance()->m_pManagementClass->SetUp_Scene(CToolMgr::GetInstance()->m_pWorld, TOOLSCENEID::TOOLSCENE_WORLD);
		break;

	case 4:
		m_MeshTool.ShowWindow(SW_HIDE);
		m_NaviMeshTool.ShowWindow(SW_HIDE);
		m_AnimationTool.ShowWindow(SW_HIDE);
		m_CameraTool.ShowWindow(SW_HIDE);
		m_LightTool.ShowWindow(SW_SHOW);

		CToolMgr::GetInstance()->m_eCurTool = TOOLID::TOOL_LIGHT;
		CToolMgr::GetInstance()->m_pManagementClass->SetUp_Scene(CToolMgr::GetInstance()->m_pWorld, TOOLSCENEID::TOOLSCENE_WORLD);
		break;

	}
}
