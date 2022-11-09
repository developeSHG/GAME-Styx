// CameraTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "CameraTool.h"
#include "afxdialogex.h"

#include "CameraBehavior.h"

// CCameraTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCameraTool, CDialog)

CCameraTool::CCameraTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CAMERATOOL, pParent)
{

}

CCameraTool::~CCameraTool()
{
}

void CCameraTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ctrlEyePointListTree);
	DDX_Control(pDX, IDC_TREE2, m_ctrlAtPointListTree);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrlActionSlider);
}


BEGIN_MESSAGE_MAP(CCameraTool, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CCameraTool::OnSpin_PositionX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CCameraTool::OnSpin_PositionY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CCameraTool::OnSpin_PositionZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN17, &CCameraTool::OnSpin_SectionSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN18, &CCameraTool::OnSpin_TotalSpeed)
	ON_BN_CLICKED(IDC_BUTTON4, &CCameraTool::OnEdit_Delete)
	ON_EN_CHANGE(IDC_EDIT7, &CCameraTool::OnEdit_PositionX)
	ON_EN_CHANGE(IDC_EDIT8, &CCameraTool::OnEdit_PositionY)
	ON_EN_CHANGE(IDC_EDIT9, &CCameraTool::OnEdit_Positionz)
	ON_EN_CHANGE(IDC_EDIT17, &CCameraTool::OnEdit_SectionSpeed)
	ON_EN_CHANGE(IDC_EDIT31, &CCameraTool::OnEdit_TotalSpeed)
	ON_BN_CLICKED(IDC_BUTTON1, &CCameraTool::OnEdit_Save)
	ON_BN_CLICKED(IDC_BUTTON2, &CCameraTool::OnEdit_Load)
	ON_BN_CLICKED(IDC_BUTTON6, &CCameraTool::OnEdit_Play)
	ON_BN_CLICKED(IDC_BUTTON7, &CCameraTool::OnEdit_Pause)
	ON_BN_CLICKED(IDC_BUTTON8, &CCameraTool::OnEdit_Stop)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CCameraTool::OnTvnSelchangedTreeEyePointList)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, &CCameraTool::OnTvnSelchangedTreeAtPointList)
	ON_BN_CLICKED(IDC_BUTTON10, &CCameraTool::OnEdit_AllDelete)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CCameraTool::OnSlider)
END_MESSAGE_MAP()


// CCameraTool 메시지 처리기입니다.

BOOL CCameraTool::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;
		case 'A':
		case 'W':
		case 'S':
		case 'D':
		case 'Q':
		case 'E':
		case 'Z':
		case 'X':
		case 'C':
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CCameraTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_pToolMgr = CToolMgr::GetInstance();

	m_hEyePointListRoot = m_ctrlEyePointListTree.InsertItem(L"Eye_Camera", 0, 0, TVI_ROOT, TVI_LAST);
	m_hAtPointListRoot = m_ctrlAtPointListTree.InsertItem(L"At_Camera", 0, 0, TVI_ROOT, TVI_LAST);

	m_ctrlActionSlider.SetRange(0, CAMERA_ITV);
	m_ctrlActionSlider.SetRangeMin(0);
	m_ctrlActionSlider.SetRangeMax(CAMERA_ITV);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCameraTool::OnTvnSelchangedTreeEyePointList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	m_eCameraTree = CAMERATREE::TREE_EYE;

	HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;
	//CString strNumCheck;
	//AfxExtractSubString(strNumCheck, m_ctrlEyePointListTree.GetItemText(hTreeItem), 1, '.');

	for (auto& iter : m_pCameraHierarchy->m_vecCameraEyePoint)
		iter->m_bSectionPick = FALSE;

	for (auto& iter : m_pCameraHierarchy->m_vecCameraAtPoint)
		iter->m_bSectionPick = FALSE;

	/*  Section For N  */
	{
		m_strSectionTag = m_ctrlEyePointListTree.GetItemText(hTreeItem);

		if (-1 != m_strSectionTag.Find(L"N"))
		{
			CString strSectionNum;
			strSectionNum = m_strSectionTag[2];
			strSectionNum += m_strSectionTag[3];
			strSectionNum.Replace(L".", L"");

			m_dwNum = _ttoi(strSectionNum.GetString());

			m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_bSectionPick = TRUE;

			if (1 < m_pCameraHierarchy->m_vecCameraEyePoint.size())
				m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum]->m_bSectionPick = TRUE;

			/*  Section of Slider  */
			{
				dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_PlayBehavior(TRUE);
				dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_bPause = TRUE;

				dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_iNum = m_dwNum - 1;

				int iPos = int(float(m_dwNum - 1) / float(m_pCameraHierarchy->m_vecCameraEyePoint.size() - 1) * float(CAMERA_ITV));
				CToolMgr::GetInstance()->m_pCameraTool->m_ctrlActionSlider.SetPos(iPos);
			}
		}
	}

	/*  Pos(v) For  */
	{
		HTREEITEM hParentItem = m_ctrlEyePointListTree.GetNextItem(hTreeItem, TVGN_PARENT);
		CString strCameraPointTag = m_ctrlEyePointListTree.GetItemText(hParentItem);

		if (-1 != strCameraPointTag.Find(L"N"))
		{
			CString strSectionNum;
			strSectionNum = strCameraPointTag[2];
			strSectionNum += strCameraPointTag[3];
			strSectionNum.Replace(L".", L"");

			m_dwNum = _ttoi(strSectionNum.GetString());

			m_strSpline = m_ctrlEyePointListTree.GetItemText(hTreeItem);

			TCHAR szNum[5][32];
			if (-1 != m_strSpline.Find(L"1"))
			{
				swprintf_s(szNum[0], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vVirtualStartPos.x);
				swprintf_s(szNum[1], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vVirtualStartPos.y);
				swprintf_s(szNum[2], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vVirtualStartPos.z);
			}
			if (-1 != m_strSpline.Find(L"2"))
			{
				swprintf_s(szNum[0], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vPos.x);
				swprintf_s(szNum[1], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vPos.y);
				swprintf_s(szNum[2], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vPos.z);
			}
			if (-1 != m_strSpline.Find(L"3"))
			{
				swprintf_s(szNum[0], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum]->m_tSpline.vPos.x);
				swprintf_s(szNum[1], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum]->m_tSpline.vPos.y);
				swprintf_s(szNum[2], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum]->m_tSpline.vPos.z);
			}
			if (-1 != m_strSpline.Find(L"4"))
			{
				swprintf_s(szNum[0], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vVirtualEndPos.x);
				swprintf_s(szNum[1], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vVirtualEndPos.y);
				swprintf_s(szNum[2], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vVirtualEndPos.z);
			}
			swprintf_s(szNum[3], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.fSpeed);
			swprintf_s(szNum[4], L"%.2f", m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.fIncSpeed);

			m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_bSectionPick = TRUE;
			m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum]->m_bSectionPick = TRUE;

			SetDlgItemText(IDC_EDIT7, szNum[0]);
			SetDlgItemText(IDC_EDIT8, szNum[1]);
			SetDlgItemText(IDC_EDIT9, szNum[2]);
			SetDlgItemText(IDC_EDIT17, szNum[3]);
			SetDlgItemText(IDC_EDIT31, szNum[4]);
		}
	}
}

void CCameraTool::OnTvnSelchangedTreeAtPointList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	m_eCameraTree = CAMERATREE::TREE_AT;

	HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;
	//CString strNumCheck;
	//AfxExtractSubString(strNumCheck, m_ctrlEyePointListTree.GetItemText(hTreeItem), 1, '.');

	for (auto& iter : m_pCameraHierarchy->m_vecCameraEyePoint)
		iter->m_bSectionPick = FALSE;

	for (auto& iter : m_pCameraHierarchy->m_vecCameraAtPoint)
		iter->m_bSectionPick = FALSE;

	/*  Section For N  */
	{
		m_strSectionTag = m_ctrlAtPointListTree.GetItemText(hTreeItem);

		if (-1 != m_strSectionTag.Find(L"N"))
		{
			CString strSectionNum;
			strSectionNum = m_strSectionTag[2];
			strSectionNum += m_strSectionTag[3];
			strSectionNum.Replace(L".", L"");

			m_dwNum = _ttoi(strSectionNum.GetString());

			/*  Section of Slider  */
			{
				dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_PlayBehavior(TRUE);
				dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_bPause = TRUE;

				dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_iNum = m_dwNum - 1;

				int iPos = int(float(m_dwNum - 1) / float(m_pCameraHierarchy->m_vecCameraAtPoint.size() - 1) * float(CAMERA_ITV));
				CToolMgr::GetInstance()->m_pCameraTool->m_ctrlActionSlider.SetPos(iPos);
			}
		}
	}

	/*  Pos(v) For  */
	{
		HTREEITEM hParentItem = m_ctrlAtPointListTree.GetNextItem(hTreeItem, TVGN_PARENT);
		CString strCameraPointTag = m_ctrlAtPointListTree.GetItemText(hParentItem);

		if (-1 != strCameraPointTag.Find(L"N"))
		{
			CString strSectionNum;
			strSectionNum = strCameraPointTag[2];
			strSectionNum += strCameraPointTag[3];
			strSectionNum.Replace(L".", L"");

			m_dwNum = _ttoi(strSectionNum.GetString());

			m_strSpline = m_ctrlAtPointListTree.GetItemText(hTreeItem);

			TCHAR szNum[5][32];
			if (-1 != m_strSpline.Find(L"1"))
			{
				swprintf_s(szNum[0], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vVirtualStartPos.x);
				swprintf_s(szNum[1], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vVirtualStartPos.y);
				swprintf_s(szNum[2], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vVirtualStartPos.z);
			}
			if (-1 != m_strSpline.Find(L"2"))
			{
				swprintf_s(szNum[0], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vPos.x);
				swprintf_s(szNum[1], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vPos.y);
				swprintf_s(szNum[2], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vPos.z);
			}
			if (-1 != m_strSpline.Find(L"3"))
			{
				swprintf_s(szNum[0], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum]->m_tSpline.vPos.x);
				swprintf_s(szNum[1], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum]->m_tSpline.vPos.y);
				swprintf_s(szNum[2], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum]->m_tSpline.vPos.z);
			}
			if (-1 != m_strSpline.Find(L"4"))
			{
				swprintf_s(szNum[0], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vVirtualEndPos.x);
				swprintf_s(szNum[1], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vVirtualEndPos.y);
				swprintf_s(szNum[2], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vVirtualEndPos.z);
			}
			swprintf_s(szNum[3], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.fSpeed);
			swprintf_s(szNum[4], L"%.2f", m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.fIncSpeed);

			m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_bSectionPick = TRUE;
			m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum]->m_bSectionPick = TRUE;

			SetDlgItemText(IDC_EDIT7, szNum[0]);
			SetDlgItemText(IDC_EDIT8, szNum[1]);
			SetDlgItemText(IDC_EDIT9, szNum[2]);
			SetDlgItemText(IDC_EDIT17, szNum[3]);
			SetDlgItemText(IDC_EDIT31, szNum[4]);
		}
	}
}

void CCameraTool::OnSpin_PositionX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN7);
}

void CCameraTool::OnSpin_PositionY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN8);
}

void CCameraTool::OnSpin_PositionZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN9);
}

void CCameraTool::OnSpin_SectionSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN17);
}

void CCameraTool::OnSpin_TotalSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN18);
}

void CCameraTool::OnEdit_PositionX()
{
	OnEdit();
}

void CCameraTool::OnEdit_PositionY()
{
	OnEdit();
}

void CCameraTool::OnEdit_Positionz()
{
	OnEdit();
}

void CCameraTool::OnEdit_SectionSpeed()
{
	OnEdit();
}

void CCameraTool::OnEdit_TotalSpeed()
{
	OnEdit();
}

void CCameraTool::OnEdit_Delete()
{
	if (-1 == m_strSectionTag.Find(L"N"))
		return;

	list<Engine::CGameObject*>& listRender = Engine::Get_Renderer()->Get_RendererGroup()[Engine::RENDERID::RENDER_NONALPHA];

	/*  Size 1  */
	if (1 == m_dwNum)
	{
		/*  Camera Eye  */
		{
			if (2 == m_pCameraHierarchy->m_vecCameraEyePoint.size())
			{
				for (auto& pEyePoint : m_pCameraHierarchy->m_vecCameraEyePoint)
				{
					auto& iter = find(listRender.begin(), listRender.end(), pEyePoint);
					if (iter != listRender.end())
						listRender.erase(iter);
				}

				for_each(m_pCameraHierarchy->m_vecCameraEyePoint.begin(),
					m_pCameraHierarchy->m_vecCameraEyePoint.end(),
					Engine::CDeleteObj());

				m_pCameraHierarchy->m_vecCameraEyePoint.shrink_to_fit();
				m_pCameraHierarchy->m_vecCameraEyePoint.clear();
			}
			else
			{
				auto& iter = find(listRender.begin(), listRender.end(), m_pCameraHierarchy->m_vecCameraEyePoint[0]);
				if (iter != listRender.end())
					listRender.erase(iter);

				Engine::Safe_Release(m_pCameraHierarchy->m_vecCameraEyePoint[0]);

				auto& iter2 = find(m_pCameraHierarchy->m_vecCameraEyePoint.begin(),
					m_pCameraHierarchy->m_vecCameraEyePoint.end(),
					m_pCameraHierarchy->m_vecCameraEyePoint[0]);

				m_pCameraHierarchy->m_vecCameraEyePoint.erase(iter2);
			}
		}
		/*  Camera At  */
		{
			if (2 == m_pCameraHierarchy->m_vecCameraAtPoint.size())
			{
				for (auto& pAtPoint : m_pCameraHierarchy->m_vecCameraAtPoint)
				{
					auto& iter = find(listRender.begin(), listRender.end(), pAtPoint);
					if (iter != listRender.end())
						listRender.erase(iter);
				}

				for_each(m_pCameraHierarchy->m_vecCameraAtPoint.begin(),
					m_pCameraHierarchy->m_vecCameraAtPoint.end(),
					Engine::CDeleteObj());

				m_pCameraHierarchy->m_vecCameraAtPoint.shrink_to_fit();
				m_pCameraHierarchy->m_vecCameraAtPoint.clear();
			}
			else
			{
				auto& iter = find(listRender.begin(), listRender.end(), m_pCameraHierarchy->m_vecCameraAtPoint[0]);
				if (iter != listRender.end())
					listRender.erase(iter);

				Engine::Safe_Release(m_pCameraHierarchy->m_vecCameraAtPoint[0]);

				auto& iter2 = find(m_pCameraHierarchy->m_vecCameraAtPoint.begin(),
					m_pCameraHierarchy->m_vecCameraAtPoint.end(),
					m_pCameraHierarchy->m_vecCameraAtPoint[0]);

				m_pCameraHierarchy->m_vecCameraAtPoint.erase(iter2);
			}
		}
	}
	else if(0 != m_dwNum)
	{
		/*  Camera Eye  */
		{
			auto& iter = find(listRender.begin(), listRender.end(), m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum]);
			if (iter != listRender.end())
				listRender.erase(iter);

			Engine::Safe_Release(m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum]);

			auto& iter2 = find(m_pCameraHierarchy->m_vecCameraEyePoint.begin(),
				m_pCameraHierarchy->m_vecCameraEyePoint.end(),
				m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum]);

			m_pCameraHierarchy->m_vecCameraEyePoint.erase(iter2);

			if(m_dwNum != m_pCameraHierarchy->m_vecCameraEyePoint.size())
				m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vVirtualEndPos = m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum]->m_tSpline.vPos;
		}
		/*  Camera At  */
		{
			auto& iter = find(listRender.begin(), listRender.end(), m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum]);
			if (iter != listRender.end())
				listRender.erase(iter);

			Engine::Safe_Release(m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum]);

			auto& iter2 = find(m_pCameraHierarchy->m_vecCameraAtPoint.begin(),
				m_pCameraHierarchy->m_vecCameraAtPoint.end(),
				m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum]);

			m_pCameraHierarchy->m_vecCameraAtPoint.erase(iter2);

			if (m_dwNum != m_pCameraHierarchy->m_vecCameraAtPoint.size())
				m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vVirtualEndPos = m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum]->m_tSpline.vPos;
		}
	}

	m_ctrlEyePointListTree.DeleteAllItems();
	m_hEyePointListRoot = m_ctrlEyePointListTree.InsertItem(L"Eye_Camera", 0, 0, TVI_ROOT, TVI_LAST);
	m_ctrlAtPointListTree.DeleteAllItems();
	m_hAtPointListRoot = m_ctrlAtPointListTree.InsertItem(L"At_Camera", 0, 0, TVI_ROOT, TVI_LAST);

	int iNum = 1;
	for (auto& iter : m_pCameraHierarchy->m_vecCameraEyePoint)
	{
		if (iNum == m_pCameraHierarchy->m_vecCameraEyePoint.size())
			break;

		CString strCamera;
		strCamera = L"No";
		TCHAR szNum[32] = L"";
		swprintf_s(szNum, L"%d", iNum);
		strCamera += szNum;
		strCamera += L". Section";

		HTREEITEM hChild = m_ctrlEyePointListTree.InsertItem(strCamera, 0, 0, m_hEyePointListRoot, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v1(Spline)", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v2", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v3", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v4(Spline)", 0, 0, hChild, TVI_LAST);

		hChild = m_ctrlAtPointListTree.InsertItem(strCamera, 0, 0, m_hAtPointListRoot, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v1(Spline)", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v2", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v3", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v4(Spline)", 0, 0, hChild, TVI_LAST);

		iNum++;
	}

	dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_PlayBehavior(FALSE);

	//_tchar szTemp[3][32];
	//swprintf_s(szTemp[0], L"%.1f", 0.f);
	//swprintf_s(szTemp[1], L"%.1f", 0.f);
	//swprintf_s(szTemp[2], L"%.1f", 0.f);sssssq

	//SetDlgItemText(IDC_EDIT7, szTemp[0]);
	//SetDlgItemText(IDC_EDIT8, szTemp[1]);
	//SetDlgItemText(IDC_EDIT9, szTemp[2]);
}

void CCameraTool::OnEdit_AllDelete()
{
	for_each(m_pCameraHierarchy->m_vecCameraEyePoint.begin(),
		m_pCameraHierarchy->m_vecCameraEyePoint.end(),
		Engine::CDeleteObj());

	m_pCameraHierarchy->m_vecCameraEyePoint.shrink_to_fit();
	m_pCameraHierarchy->m_vecCameraEyePoint.clear();

	for_each(m_pCameraHierarchy->m_vecCameraAtPoint.begin(),
		m_pCameraHierarchy->m_vecCameraAtPoint.end(),
		Engine::CDeleteObj());

	m_pCameraHierarchy->m_vecCameraAtPoint.shrink_to_fit();
	m_pCameraHierarchy->m_vecCameraAtPoint.clear();

	m_ctrlEyePointListTree.DeleteAllItems();
	m_hEyePointListRoot = m_ctrlEyePointListTree.InsertItem(L"Eye_Camera", 0, 0, TVI_ROOT, TVI_LAST);
	m_ctrlAtPointListTree.DeleteAllItems();
	m_hAtPointListRoot = m_ctrlAtPointListTree.InsertItem(L"At_Camera", 0, 0, TVI_ROOT, TVI_LAST);

	_tchar szTemp[5][32];
	swprintf_s(szTemp[0], L"%.1f", 0.f);
	swprintf_s(szTemp[1], L"%.1f", 0.f);
	swprintf_s(szTemp[2], L"%.1f", 0.f);
	swprintf_s(szTemp[3], L"%.1f", 0.f);
	swprintf_s(szTemp[4], L"%.1f", 0.f);

	SetDlgItemText(IDC_EDIT7, szTemp[0]);
	SetDlgItemText(IDC_EDIT8, szTemp[1]);
	SetDlgItemText(IDC_EDIT9, szTemp[2]);
	SetDlgItemText(IDC_EDIT17, szTemp[3]);
	SetDlgItemText(IDC_EDIT31, szTemp[4]);
}

void CCameraTool::OnSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if (1 >= m_pCameraHierarchy->m_vecCameraAtPoint.size()
		|| TRUE == dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_bPlay)
		return;

	dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_PlayBehavior(TRUE);
	dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_bPause = TRUE;

	float fPos = m_ctrlActionSlider.GetPos() / float(CAMERA_ITV) * (m_pCameraHierarchy->m_vecCameraAtPoint.size() - 1);
	if (m_pCameraHierarchy->m_vecCameraAtPoint.size() - 1 != int(fPos))
	{
		dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_fItv = fPos - int(fPos);
		dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_iNum = int(fPos);
	}
}

void CCameraTool::OnEdit_Play()
{
	dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_PlayBehavior(TRUE);
	dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_bPlay = TRUE;
	dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_bPause = FALSE;
}

void CCameraTool::OnEdit_Pause()
{
	dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_bPlay = FALSE;
	dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_bPause = TRUE;
}

void CCameraTool::OnEdit_Stop()
{
	dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->Set_Initialize();
	dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_PlayBehavior(FALSE);
	m_ctrlActionSlider.SetPos(0);
}

void CCameraTool::OnEdit_Save()
{
	CFileDialog Dlg(
		FALSE,   /* 열기 TRUE, 저장 FALSE */
		L"dat", /* 디폴트 확장자 */
		L"제목없음.dat", /* 디폴트 파일이름 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", /* 파일 형식 */
		this /* 부모 윈도우 */);

	TCHAR szCurDir[256] = L"";

	//현재 작업 경로를 얻어오는 함수
	GetCurrentDirectory(256, szCurDir);
	//경로상에 파일명을 제거하는 함수. 제거할 파일명이 없다면 말단 폴더명을 제거함.
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurDir; // 절대경로

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Save Failed");
			return;
		}

		DWORD dwByte = 0;
		
		_tchar szCameraTag[128];

		/*  Camera Eye  */
		{
			lstrcpy(szCameraTag, L"Camera_Eye");
			WriteFile(hFile, szCameraTag, sizeof(szCameraTag), &dwByte, nullptr);

			size_t uSize = m_pCameraHierarchy->m_vecCameraEyePoint.size();
			WriteFile(hFile, &uSize, sizeof(size_t), &dwByte, nullptr);

			for (auto& iter : m_pCameraHierarchy->m_vecCameraEyePoint)
			{
				SPLINE tSpline;

				tSpline = iter->m_tSpline;
				WriteFile(hFile, &tSpline, sizeof(tSpline), &dwByte, nullptr);

				if (0 == dwByte)
					break;
			}
		}

		/*  Camera At  */
		{
			lstrcpy(szCameraTag, L"Camera_At");
			WriteFile(hFile, szCameraTag, sizeof(szCameraTag), &dwByte, nullptr);

			size_t uSize = m_pCameraHierarchy->m_vecCameraAtPoint.size();
			WriteFile(hFile, &uSize, sizeof(size_t), &dwByte, nullptr);

			for (auto& iter : m_pCameraHierarchy->m_vecCameraAtPoint)
			{
				SPLINE tSpline;

				tSpline = iter->m_tSpline;
				WriteFile(hFile, &tSpline, sizeof(tSpline), &dwByte, nullptr);

				if (0 == dwByte)
					break;
			}
		}
		CloseHandle(hFile);
		AfxMessageBox(L"Save Complete");
	}

	Invalidate(FALSE);
}

void CCameraTool::OnEdit_Load()
{
	CFileDialog Dlg(
		TRUE,   /* 열기 TRUE, 저장 FALSE */
		L"dat", /* 디폴트 확장자 */
		L"제목없음.dat", /* 디폴트 파일이름 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", /* 파일 형식 */
		this /* 부모 윈도우 */);

	TCHAR szCurDir[256] = L"";

	//현재 작업 경로를 얻어오는 함수
	GetCurrentDirectory(256, szCurDir);
	//경로상에 파일명을 제거하는 함수. 제거할 파일명이 없다면 말단 폴더명을 제거함.
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurDir; // 절대경로

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Load Failed");
			return;
		}

		/*  Delete  */
		OnEdit_AllDelete();

		DWORD dwByte = 0;

		_tchar szCameraTag[128];
		ReadFile(hFile, szCameraTag, sizeof(szCameraTag), &dwByte, nullptr);

		size_t uSize;
		ReadFile(hFile, &uSize, sizeof(size_t), &dwByte, nullptr);

		/*  CameraEye  */
		if (!lstrcmpW(szCameraTag, L"Camera_Eye"))
		{
			while (uSize)
			{
				SPLINE tSpline;
				ReadFile(hFile, &tSpline, sizeof(tSpline), &dwByte, nullptr);
				
				if (0 == dwByte)
					break;

				CCameraPoint* pCameraPoint = nullptr;
				pCameraPoint = dynamic_cast<CCameraPoint*>(CAbstractFactoryPattern<CCameraPoint>::CreateObject(CToolMgr::GetInstance()->m_pGraphicDev));
				NULL_CHECK_RETURN(pCameraPoint, );

				pCameraPoint->m_ePointType = CCameraPoint::CAMERA_POINT_TYPE::CAMERA_POINT_EYE;
				memcpy(&pCameraPoint->m_tSpline, &tSpline, sizeof(SPLINE));

				m_pCameraHierarchy->m_vecCameraEyePoint.push_back(pCameraPoint);

				uSize--;

				/*  Tree Update  */
				Load_EyeTreeUpdate();
			}
		}

		ReadFile(hFile, szCameraTag, sizeof(szCameraTag), &dwByte, nullptr);
		ReadFile(hFile, &uSize, sizeof(size_t), &dwByte, nullptr);

		/*  CameraAt  */
		if (!lstrcmpW(szCameraTag, L"Camera_At"))
		{
			while (uSize)
			{
				SPLINE tSpline;
				ReadFile(hFile, &tSpline, sizeof(tSpline), &dwByte, nullptr);

				if (0 == dwByte)
					break;

				CCameraPoint* pCameraPoint = nullptr;
				pCameraPoint = dynamic_cast<CCameraPoint*>(CAbstractFactoryPattern<CCameraPoint>::CreateObject(CToolMgr::GetInstance()->m_pGraphicDev));
				NULL_CHECK_RETURN(pCameraPoint, );

				pCameraPoint->m_ePointType = CCameraPoint::CAMERA_POINT_TYPE::CAMERA_POINT_AT;
				memcpy(&pCameraPoint->m_tSpline, &tSpline, sizeof(SPLINE));

				m_pCameraHierarchy->m_vecCameraAtPoint.push_back(pCameraPoint);

				uSize--;

				/*  Tree Update  */
				Load_AttreeUpdate();
			}
		}

		CloseHandle(hFile);
		AfxMessageBox(L"Load Complete");
	}

	Invalidate(FALSE);
}

void CCameraTool::TreeUpdate()
{
	int iNum = m_pCameraHierarchy->m_vecCameraEyePoint.size();

	CString strCamera;

	/*  TreeItem First  */
	if (2 == iNum)
	{
		m_ctrlEyePointListTree.DeleteAllItems();
		m_hEyePointListRoot = m_ctrlEyePointListTree.InsertItem(L"Eye_Camera", 0, 0, TVI_ROOT, TVI_LAST);
		m_ctrlAtPointListTree.DeleteAllItems();
		m_hAtPointListRoot = m_ctrlAtPointListTree.InsertItem(L"At_Camera", 0, 0, TVI_ROOT, TVI_LAST);

		strCamera = L"No";
		TCHAR szNum[32] = L"";
		swprintf_s(szNum, L"%d", iNum-1);
		strCamera += szNum;
		strCamera += L". Section";

		HTREEITEM hChild = m_ctrlEyePointListTree.InsertItem(strCamera, 0, 0, m_hEyePointListRoot, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v1(Spline)", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v2", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v3", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v4(Spline)", 0, 0, hChild, TVI_LAST);
		
		hChild = m_ctrlAtPointListTree.InsertItem(strCamera, 0, 0, m_hAtPointListRoot, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v1(Spline)", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v2", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v3", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v4(Spline)", 0, 0, hChild, TVI_LAST);
	}
	else if(2 < iNum)
	{
		strCamera = L"No";
		TCHAR szNum[32] = L"";
		swprintf_s(szNum, L"%d", iNum-1);
		strCamera += szNum;
		strCamera += L". Section";

		HTREEITEM hChild = m_ctrlEyePointListTree.InsertItem(strCamera, 0, 0, m_hEyePointListRoot, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v1(Spline)", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v2", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v3", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v4(Spline)", 0, 0, hChild, TVI_LAST);

		hChild = m_ctrlAtPointListTree.InsertItem(strCamera, 0, 0, m_hAtPointListRoot, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v1(Spline)", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v2", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v3", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v4(Spline)", 0, 0, hChild, TVI_LAST);
	}
}

void CCameraTool::OnSpin(LPNMUPDOWN _pNMUpdDown, DWORD _dwID)
{
	_bool bReturn = TRUE;
	if (CAMERATREE::TREE_EYE == m_eCameraTree)
	{
		for (auto& iter : m_pCameraHierarchy->m_vecCameraEyePoint)
			if (TRUE == iter->m_bSectionPick)
			{
				bReturn = FALSE;
				break;
			}
	}
	else if(CAMERATREE::TREE_AT == m_eCameraTree)
	{
		for (auto& iter : m_pCameraHierarchy->m_vecCameraAtPoint)
			if (TRUE == iter->m_bSectionPick)
			{
				bReturn = FALSE;
				break;
			}
	}
	if (TRUE == bReturn)
		return;

	CString strX, strY, strZ, strSectionSpeed, strIncreaseSpeed;
	GetDlgItemText(IDC_EDIT7, strX);
	GetDlgItemText(IDC_EDIT8, strY);
	GetDlgItemText(IDC_EDIT9, strZ);
	GetDlgItemText(IDC_EDIT17, strSectionSpeed);
	GetDlgItemText(IDC_EDIT31, strIncreaseSpeed);

	Engine::_vec3 vPosition = { (float)_wtof(strX), (float)_wtof(strY), (float)_wtof(strZ) };
	_float fSectionSpeed = (float)_wtof(strSectionSpeed);
	_float fIncreaseSpeed = (float)_wtof(strIncreaseSpeed);

	float fValue = 0.f;

	if (_pNMUpdDown->iDelta < 0)
		fValue += 1.f;
	else
		fValue -= 1.f;

	if (IDC_SPIN7 == _dwID)
		vPosition.x += fValue;
	else if (IDC_SPIN8 == _dwID)
		vPosition.y += fValue;
	else if (IDC_SPIN9 == _dwID)
		vPosition.z += fValue;
	else if (IDC_SPIN17 == _dwID)
		fSectionSpeed += fValue;
	else if (IDC_SPIN18 == _dwID)
		fIncreaseSpeed += fValue;

	if (CAMERATREE::TREE_EYE == m_eCameraTree)
	{
		if (-1 != m_strSpline.Find(L"1"))
			m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vVirtualStartPos = vPosition;
		if (-1 != m_strSpline.Find(L"2"))
			m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vPos = vPosition;
		if (-1 != m_strSpline.Find(L"3"))
			m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum]->m_tSpline.vVirtualStartPos = vPosition;
		if (-1 != m_strSpline.Find(L"4"))
			m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vVirtualEndPos = vPosition;
	}
	else if (CAMERATREE::TREE_AT == m_eCameraTree)
	{
		if (-1 != m_strSpline.Find(L"1"))
			m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vVirtualStartPos = vPosition;
		if (-1 != m_strSpline.Find(L"2"))
			m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vPos = vPosition;
		if (-1 != m_strSpline.Find(L"3"))
			m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum]->m_tSpline.vVirtualStartPos = vPosition;
		if (-1 != m_strSpline.Find(L"4"))
			m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vVirtualEndPos = vPosition;
	}
	m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.fSpeed = fSectionSpeed;
	m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.fIncSpeed = fIncreaseSpeed;
	m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.fSpeed = fSectionSpeed;
	m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.fIncSpeed = fIncreaseSpeed;

	_tchar szTemp[5][32];
	swprintf_s(szTemp[0], L"%.2f", vPosition.x);
	swprintf_s(szTemp[1], L"%.2f", vPosition.y);
	swprintf_s(szTemp[2], L"%.2f", vPosition.z);
	swprintf_s(szTemp[3], L"%.2f", fSectionSpeed);
	swprintf_s(szTemp[4], L"%.2f", fIncreaseSpeed);

	SetDlgItemText(IDC_EDIT7, szTemp[0]);
	SetDlgItemText(IDC_EDIT8, szTemp[1]);
	SetDlgItemText(IDC_EDIT9, szTemp[2]);
	SetDlgItemText(IDC_EDIT17, szTemp[3]);
	SetDlgItemText(IDC_EDIT31, szTemp[4]);
}

void CCameraTool::OnEdit(void)
{
	_bool bReturn = TRUE;
	if (CAMERATREE::TREE_EYE == m_eCameraTree)
	{
		for (auto& iter : m_pCameraHierarchy->m_vecCameraEyePoint)
			if (TRUE == iter->m_bSectionPick)
			{
				bReturn = FALSE;
				break;
			}
	}
	else if (CAMERATREE::TREE_AT == m_eCameraTree)
	{
		for (auto& iter : m_pCameraHierarchy->m_vecCameraAtPoint)
			if (TRUE == iter->m_bSectionPick)
			{
				bReturn = FALSE;
				break;
			}
	}
	if (TRUE == bReturn)
		return;

	CString strX, strY, strZ, strSectionSpeed, strIncreaseSpeed;
	GetDlgItemText(IDC_EDIT7, strX);
	GetDlgItemText(IDC_EDIT8, strY);
	GetDlgItemText(IDC_EDIT9, strZ);
	GetDlgItemText(IDC_EDIT17, strSectionSpeed);
	GetDlgItemText(IDC_EDIT31, strIncreaseSpeed);

	Engine::_vec3 vPosition = { (float)_wtof(strX), (float)_wtof(strY), (float)_wtof(strZ) };
	_float fSectionSpeed = (float)_wtof(strSectionSpeed);
	_float fIncreaseSpeed = (float)_wtof(strIncreaseSpeed);

	if (CAMERATREE::TREE_EYE == m_eCameraTree)
	{
		if (-1 != m_strSpline.Find(L"1"))
			m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vVirtualStartPos = vPosition;
		if (-1 != m_strSpline.Find(L"2"))
			m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vPos = vPosition;
		if (-1 != m_strSpline.Find(L"3"))
			m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum]->m_tSpline.vVirtualStartPos = vPosition;
		if (-1 != m_strSpline.Find(L"4"))
			m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.vVirtualEndPos = vPosition;
	}
	else if (CAMERATREE::TREE_AT == m_eCameraTree)
	{
		if (-1 != m_strSpline.Find(L"1"))
			m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vVirtualStartPos = vPosition;
		if (-1 != m_strSpline.Find(L"2"))
			m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vPos = vPosition;
		if (-1 != m_strSpline.Find(L"3"))
			m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum]->m_tSpline.vVirtualStartPos = vPosition;
		if (-1 != m_strSpline.Find(L"4"))
			m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.vVirtualEndPos = vPosition;
	}
	m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.fSpeed = fSectionSpeed;
	m_pCameraHierarchy->m_vecCameraEyePoint[m_dwNum - 1]->m_tSpline.fIncSpeed = fIncreaseSpeed;
	m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.fSpeed = fSectionSpeed;
	m_pCameraHierarchy->m_vecCameraAtPoint[m_dwNum - 1]->m_tSpline.fIncSpeed = fIncreaseSpeed;
}

void CCameraTool::Load_EyeTreeUpdate()
{
	/*  Tree Update  */
	int iNum = m_pCameraHierarchy->m_vecCameraEyePoint.size();

	CString strCamera;

	/*  TreeItem First  */
	if (2 == iNum)
	{
		m_ctrlEyePointListTree.DeleteAllItems();
		m_hEyePointListRoot = m_ctrlEyePointListTree.InsertItem(L"Eye_Camera", 0, 0, TVI_ROOT, TVI_LAST);

		strCamera = L"No";
		TCHAR szNum[32] = L"";
		swprintf_s(szNum, L"%d", iNum - 1);
		strCamera += szNum;
		strCamera += L". Section";

		HTREEITEM hChild = m_ctrlEyePointListTree.InsertItem(strCamera, 0, 0, m_hEyePointListRoot, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v1(Spline)", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v2", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v3", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v4(Spline)", 0, 0, hChild, TVI_LAST);
	}
	else if (2 < iNum)
	{
		strCamera = L"No";
		TCHAR szNum[32] = L"";
		swprintf_s(szNum, L"%d", iNum - 1);
		strCamera += szNum;
		strCamera += L". Section";

		HTREEITEM hChild = m_ctrlEyePointListTree.InsertItem(strCamera, 0, 0, m_hEyePointListRoot, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v1(Spline)", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v2", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v3", 0, 0, hChild, TVI_LAST);
		m_ctrlEyePointListTree.InsertItem(L"v4(Spline)", 0, 0, hChild, TVI_LAST);
	}
}

void CCameraTool::Load_AttreeUpdate()
{
	/*  Tree Update  */
	int iNum = m_pCameraHierarchy->m_vecCameraAtPoint.size();

	CString strCamera;

	/*  TreeItem First  */
	if (2 == iNum)
	{
		m_ctrlAtPointListTree.DeleteAllItems();
		m_hAtPointListRoot = m_ctrlAtPointListTree.InsertItem(L"At_Camera", 0, 0, TVI_ROOT, TVI_LAST);

		strCamera = L"No";
		TCHAR szNum[32] = L"";
		swprintf_s(szNum, L"%d", iNum - 1);
		strCamera += szNum;
		strCamera += L". Section";

		HTREEITEM hChild = m_ctrlAtPointListTree.InsertItem(strCamera, 0, 0, m_hAtPointListRoot, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v1(Spline)", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v2", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v3", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v4(Spline)", 0, 0, hChild, TVI_LAST);
	}
	else if (2 < iNum)
	{
		strCamera = L"No";
		TCHAR szNum[32] = L"";
		swprintf_s(szNum, L"%d", iNum - 1);
		strCamera += szNum;
		strCamera += L". Section";

		HTREEITEM hChild = m_ctrlAtPointListTree.InsertItem(strCamera, 0, 0, m_hAtPointListRoot, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v1(Spline)", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v2", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v3", 0, 0, hChild, TVI_LAST);
		m_ctrlAtPointListTree.InsertItem(L"v4(Spline)", 0, 0, hChild, TVI_LAST);
	}
}

