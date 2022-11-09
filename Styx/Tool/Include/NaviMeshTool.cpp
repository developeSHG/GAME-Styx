// NaviMeshTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "NaviMeshTool.h"
#include "afxdialogex.h"

#include "NaviTri.h"
#include "NaviPoint.h"

// CNaviMeshTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNaviMeshTool, CDialog)

CNaviMeshTool::CNaviMeshTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_NAVIMESHTOOL, pParent)
{

}

CNaviMeshTool::~CNaviMeshTool()
{
}

void CNaviMeshTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE3, m_ctrlNaviMeshListTree);
	DDX_Control(pDX, IDC_RADIO1, m_ctrlPickingType[INSERT_POINT]);
	DDX_Control(pDX, IDC_RADIO2, m_ctrlPickingType[SELECT_POINT]);
	DDX_Control(pDX, IDC_RADIO6, m_ctrlPickingType[SELECT_CELL]);
	DDX_Control(pDX, IDC_RADIO3, m_ctrlNaviType[Engine::NAVI_TYPE::NAVI_ROAD]);
	DDX_Control(pDX, IDC_RADIO7, m_ctrlNaviType[Engine::NAVI_TYPE::NAVI_INSIDE]);
	DDX_Control(pDX, IDC_RADIO8, m_ctrlNaviType[Engine::NAVI_TYPE::NAVI_CLIFF]);
}


BEGIN_MESSAGE_MAP(CNaviMeshTool, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CNaviMeshTool::OnSpin_PositionX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CNaviMeshTool::OnSpin_PositionY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CNaviMeshTool::OnSpin_PositionZ)
	ON_EN_CHANGE(IDC_EDIT7, &CNaviMeshTool::OnEdit_PositionX)
	ON_EN_CHANGE(IDC_EDIT8, &CNaviMeshTool::OnEdit_PositionY)
	ON_EN_CHANGE(IDC_EDIT9, &CNaviMeshTool::OnEdit_PositionZ)
	ON_BN_CLICKED(IDC_BUTTON1, &CNaviMeshTool::OnEdit_Save)
	ON_BN_CLICKED(IDC_BUTTON2, &CNaviMeshTool::OnEdit_Load)
	ON_BN_CLICKED(IDC_BUTTON3, &CNaviMeshTool::OnEdit_Delete)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE3, &CNaviMeshTool::OnTvnSelchangedTreeNaviMeshList)
END_MESSAGE_MAP()


// CNaviMeshTool 메시지 처리기입니다.

BOOL CNaviMeshTool::PreTranslateMessage(MSG* pMsg)
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

BOOL CNaviMeshTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_pToolMgr = CToolMgr::GetInstance();

	m_hNaviMeshListRoot = m_ctrlNaviMeshListTree.InsertItem(L"Navigation_Mesh_Cell", 0, 0, TVI_ROOT, TVI_LAST);

	m_ctrlPickingType[INSERT_POINT].SetCheck(TRUE);
	m_ctrlNaviType[Engine::NAVI_TYPE::NAVI_ROAD].SetCheck(TRUE);
	Update_PickingType();
	Update_NaviType();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNaviMeshTool::OnTvnSelchangedTreeNaviMeshList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;
	
	for (auto& iter : m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri)
	{
		for (auto& pointiter : iter)
			pointiter->m_bPick = FALSE;
	}
	m_uPointTri = -1;
	m_uPointIndex = -1;
	m_bFindDelete = FALSE;

	/*  Cell  */
	{
		CString cstrCell;
		cstrCell = m_ctrlNaviMeshListTree.GetItemText(hTreeItem);

		/*  Cell  */
		if (-1 == cstrCell.Find(L"N") && -1 != cstrCell.Find(L"C"))
		{
			SetDlgItemText(IDC_EDIT10, cstrCell);

			cstrCell.Replace(L"Cell_", L"");
			m_uPointTri = _ttoi(cstrCell.GetString());
			m_bFindDelete = TRUE;

			m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri][0]->m_bPick = TRUE;
			m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri][1]->m_bPick = TRUE;
			m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri][2]->m_bPick = TRUE;
		}

		HTREEITEM hParentItem = m_ctrlNaviMeshListTree.GetNextItem(hTreeItem, TVGN_PARENT);
		cstrCell = m_ctrlNaviMeshListTree.GetItemText(hParentItem);

		/*  Cell Index  */
		if (-1 == cstrCell.Find(L"N") && -1 != cstrCell.Find(L"C"))
		{
			CString cstrCellIndexTag = cstrCell;
			cstrCellIndexTag += L"_Index_";

			cstrCell.Replace(L"Cell_", L"");
			m_uPointTri = _ttoi(cstrCell.GetString());
			m_bFindDelete = TRUE;

			CString cstrIndex = m_ctrlNaviMeshListTree.GetItemText(hTreeItem);
			cstrIndex.Replace(L"Index_", L"");
			m_uPointIndex = _ttoi(cstrIndex.GetString());

			/*  Text  */
			cstrCellIndexTag += cstrIndex;
			SetDlgItemText(IDC_EDIT10, cstrCellIndexTag);

			/*  Edit  */
			{
				_tchar szTemp[3][32];
				swprintf_s(szTemp[0], L"%.2f", m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri][m_uPointIndex]->m_vPoint.x);
				swprintf_s(szTemp[1], L"%.2f", m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri][m_uPointIndex]->m_vPoint.y);
				swprintf_s(szTemp[2], L"%.2f", m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri][m_uPointIndex]->m_vPoint.z);

				SetDlgItemText(IDC_EDIT7, szTemp[0]);
				SetDlgItemText(IDC_EDIT8, szTemp[1]);
				SetDlgItemText(IDC_EDIT9, szTemp[2]);
			}
			m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri][m_uPointIndex]->m_bPick = TRUE;
		}
	}
}

void CNaviMeshTool::OnSpin_PositionX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN7);
}

void CNaviMeshTool::OnSpin_PositionY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN8);
}

void CNaviMeshTool::OnSpin_PositionZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN9);
}

void CNaviMeshTool::OnEdit_PositionX()
{
	OnEdit();
}

void CNaviMeshTool::OnEdit_PositionY()
{
	OnEdit();
}

void CNaviMeshTool::OnEdit_PositionZ()
{
	OnEdit();
}

void CNaviMeshTool::OnEdit_Delete()
{
	if (0 != m_iPointNumCheck)
	{
		AfxMessageBox(L"Waring! Click To Point Index Finish");
		return;
	}
	if (-1 == m_uPointTri || FALSE == m_bFindDelete)
	{
		AfxMessageBox(L"Waring! Find To Cell or Cell Index");
		return;
	}

	for (auto& iter : m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri])
	{
		Engine::Safe_Release(iter);
	}
	m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri].clear();
	m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri].shrink_to_fit();
	
	auto& iter = m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.begin() + m_uPointTri;

	m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.erase(iter);

	m_ctrlNaviMeshListTree.DeleteAllItems();
	m_hNaviMeshListRoot = m_ctrlNaviMeshListTree.InsertItem(L"Navigation_Mesh_Cell", 0, 0, TVI_ROOT, TVI_LAST);

	_ushort iNum = 0;
	for (auto& iter : m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri)
	{
		if(iter == m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.back())
			continue;

		_tchar szTemp[32];
		swprintf_s(szTemp, L"%d", iNum);

		CString cstrNum = L"Cell_";
		cstrNum += szTemp;
		HTREEITEM hChild = m_ctrlNaviMeshListTree.InsertItem(cstrNum, 0, 0, m_hNaviMeshListRoot, TVI_LAST);
		m_ctrlNaviMeshListTree.InsertItem(L"Index_0", 0, 0, hChild, TVI_LAST);
		m_ctrlNaviMeshListTree.InsertItem(L"Index_1", 0, 0, hChild, TVI_LAST);
		m_ctrlNaviMeshListTree.InsertItem(L"Index_2", 0, 0, hChild, TVI_LAST);

		iNum++;
	}
}

void CNaviMeshTool::OnEdit_Save()
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

		NAVICELL tNaviCell;

		for (auto& iter : m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri)
		{
			for (auto& pointiter : iter)
			{
				tNaviCell.eNaviType = iter[0]->m_eNaviType;
				tNaviCell.vPointA = iter[0]->m_vPoint;
				tNaviCell.vPointB = iter[1]->m_vPoint;
				tNaviCell.vPointC = iter[2]->m_vPoint;

				WriteFile(hFile, &tNaviCell, sizeof(NAVICELL), &dwByte, nullptr);

				if (0 == dwByte)
					break;

				break;
			}
		}

		CloseHandle(hFile);
		AfxMessageBox(L"Save Complete");
	}

	m_pToolMgr->m_pToolView->Invalidate(FALSE);
	Invalidate(FALSE);
}

void CNaviMeshTool::OnEdit_Load()
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
		for_each(m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.begin(), m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.end(), [&](auto& iter){
			for_each(iter.begin(), iter.end(), Engine::CDeleteObj());
			iter.clear();
			iter.shrink_to_fit();
		});
		m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.clear();
		m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.shrink_to_fit();

		DWORD dwByte = 0;

		NAVICELL tNaviCell;

		Update_PickingType();

		while (TRUE)
		{
			ReadFile(hFile, &tNaviCell, sizeof(NAVICELL), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			/*  Tri Create  */
			{
				m_pToolMgr->m_pWorld->m_pNaviTri->m_uPointIndex = 0;
				m_pToolMgr->m_pWorld->m_pNaviTri->PointCreate(tNaviCell.vPointA, tNaviCell.eNaviType);
				m_pToolMgr->m_pWorld->m_pNaviTri->m_uPointIndex = 1;
				m_pToolMgr->m_pWorld->m_pNaviTri->PointCreate(tNaviCell.vPointB, tNaviCell.eNaviType);
				m_pToolMgr->m_pWorld->m_pNaviTri->m_uPointIndex = 2;
				m_pToolMgr->m_pWorld->m_pNaviTri->PointCreate(tNaviCell.vPointC, tNaviCell.eNaviType);
			}
		}

		m_ctrlNaviMeshListTree.DeleteAllItems();
		m_hNaviMeshListRoot = m_ctrlNaviMeshListTree.InsertItem(L"Navigation_Mesh_Cell", 0, 0, TVI_ROOT, TVI_LAST);

		_ushort iNum = 0;
		for (auto& iter : m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri)
		{
			if (iter == m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.back())
				continue;

			_tchar szTemp[32];
			swprintf_s(szTemp, L"%d", iNum);

			CString cstrNum = L"Cell_";
			cstrNum += szTemp;
			HTREEITEM hChild = m_ctrlNaviMeshListTree.InsertItem(cstrNum, 0, 0, m_hNaviMeshListRoot, TVI_LAST);
			m_ctrlNaviMeshListTree.InsertItem(L"Index_0", 0, 0, hChild, TVI_LAST);
			m_ctrlNaviMeshListTree.InsertItem(L"Index_1", 0, 0, hChild, TVI_LAST);
			m_ctrlNaviMeshListTree.InsertItem(L"Index_2", 0, 0, hChild, TVI_LAST);

			iNum++;
		}

		CloseHandle(hFile);
		AfxMessageBox(L"Load Complete");
	}

	Invalidate(FALSE);
}

void CNaviMeshTool::OnSpin(LPNMUPDOWN _pNMUpdDown, DWORD _dwID)
{
	if (-1 == m_uPointTri || -1 == m_uPointIndex)
		return;

	CString strX, strY, strZ;
	GetDlgItemText(IDC_EDIT7, strX);
	GetDlgItemText(IDC_EDIT8, strY);
	GetDlgItemText(IDC_EDIT9, strZ);

	Engine::_vec3 vPosition = { (float)_wtof(strX), (float)_wtof(strY), (float)_wtof(strZ) };

	float fValue = 0.f;

	if (_pNMUpdDown->iDelta < 0)
		fValue += 0.05f;
	else
		fValue -= 0.05f;

	if (IDC_SPIN7 == _dwID)
		vPosition.x += fValue;
	else if (IDC_SPIN8 == _dwID)
		vPosition.y += fValue;
	else if (IDC_SPIN9 == _dwID)
		vPosition.z += fValue;

	m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri][m_uPointIndex]->m_vPoint = vPosition;

	_tchar szTemp[3][32];
	swprintf_s(szTemp[0], L"%.2f", vPosition.x);
	swprintf_s(szTemp[1], L"%.2f", vPosition.y);
	swprintf_s(szTemp[2], L"%.2f", vPosition.z);

	SetDlgItemText(IDC_EDIT7, szTemp[0]);
	SetDlgItemText(IDC_EDIT8, szTemp[1]);
	SetDlgItemText(IDC_EDIT9, szTemp[2]);

	//Engine::Get_NaviMgr()->Get_NaviMesh()->m_vecCell[m_uPointTri]->m = vPosition;
}

void CNaviMeshTool::OnEdit(void)
{
	if (-1 == m_uPointTri || -1 == m_uPointIndex)
		return;

	CString strX, strY, strZ;
	GetDlgItemText(IDC_EDIT7, strX);
	GetDlgItemText(IDC_EDIT8, strY);
	GetDlgItemText(IDC_EDIT9, strZ);

	Engine::_vec3 vPosition = { (float)_wtof(strX), (float)_wtof(strY), (float)_wtof(strZ) };

	m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri][m_uPointIndex]->m_vPoint = vPosition;
}

void CNaviMeshTool::PointCheck(const _vec3& _vPickPos)
{
	for (auto& iter : m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri)
	{
		for (auto& pointiter : iter)
			pointiter->m_bPick = FALSE;
	}

	Update_PickingType();
	Update_NaviType();

	if (PICKING_TYPE::INSERT_POINT == m_ePickingType)
	{
		if (2 == m_iPointNumCheck)
		{
			Engine::_vec3 vecDir1 = m_vPoint[1] - m_vPoint[0];
			Engine::_vec3 vecDir2 = _vPickPos - m_vPoint[0];

			Engine::_vec3 vecCross = *D3DXVec3Cross(&vecCross, &vecDir1, &vecDir2);

			if (0.f > vecCross.y)
			{
				m_pToolMgr->m_pWorld->m_pNaviTri->BackDelete();

				ZeroMemory(m_vPoint, sizeof(m_vPoint));
				m_iPointNumCheck = 0;

				AfxMessageBox(L"Warning! Please Picking ClockWise");
				return;
			}
		}

		/*  Tri Create  */
		{
			m_pToolMgr->m_pWorld->m_pNaviTri->m_uPointIndex = m_iPointNumCheck;
			m_pToolMgr->m_pWorld->m_pNaviTri->PointCreate(_vPickPos, m_eNaviType);
		}

		m_vPoint[m_iPointNumCheck] = _vPickPos;
		m_iPointNumCheck++;

		/*  Create Cell  */
		if (3 == m_iPointNumCheck)
		{
			/*  NaviMesh Create Cell  */
			{
				/*m_pToolMgr->m_pWorld->m_pNaviTri->Update_Object(1.f);
				m_vPoint[0] = m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.size() - 2][0]->m_vPoint;
				m_vPoint[1] = m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.size() - 2][1]->m_vPoint;
				m_vPoint[2] = m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.size() - 2][2]->m_vPoint;
				Engine::Get_NaviMgr()->Get_NaviMesh()->Create_Cell(m_vPoint);*/
			}

			m_iPointNumCheck = 0;

			/*  Create First  */
			if (2 == m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.size())
			{
				m_ctrlNaviMeshListTree.DeleteAllItems();
				m_hNaviMeshListRoot = m_ctrlNaviMeshListTree.InsertItem(L"Navigation_Mesh_Cell", 0, 0, TVI_ROOT, TVI_LAST);
			}

			_ushort uSize = m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri.size() - 2;
			_tchar szTemp[32];
			swprintf_s(szTemp, L"%d", uSize);

			CString cstrNum = L"Cell_";
			cstrNum += szTemp;
			HTREEITEM hChild = m_ctrlNaviMeshListTree.InsertItem(cstrNum, 0, 0, m_hNaviMeshListRoot, TVI_LAST);
			m_ctrlNaviMeshListTree.InsertItem(L"Index_0", 0, 0, hChild, TVI_LAST);
			m_ctrlNaviMeshListTree.InsertItem(L"Index_1", 0, 0, hChild, TVI_LAST);
			m_ctrlNaviMeshListTree.InsertItem(L"Index_2", 0, 0, hChild, TVI_LAST);
		}
	}
	else if (PICKING_TYPE::SELECT_POINT == m_ePickingType)
	{
		//m_pToolMgr->m_pWorld->m_pNaviTri->SelectPoint(_vPickPos);

		_ushort uCellIndex = 0;
		for (auto& iter : m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri)
		{
			_ushort uPointIndex = 0;
			for (auto& pointiter : iter)
			{
				if (nullptr != pointiter)
				{
					_vec3 vDist = pointiter->m_vPoint - _vPickPos;

					_float fRadiusSum = pointiter->m_fRadius + pointiter->m_fRadius;

					if (fRadiusSum > D3DXVec3Length(&vDist))
					{
						m_uPointTri = uCellIndex;
						m_uPointIndex = uPointIndex;

						/*  Text  */
						CString cstrCellIndexTag = L"Cell_";

						_tchar szTemp[32];
						swprintf_s(szTemp, L"%d", m_uPointTri);
						cstrCellIndexTag += szTemp;
						cstrCellIndexTag += L"_Index_";
						swprintf_s(szTemp, L"%d", m_uPointIndex);
						cstrCellIndexTag += szTemp;
						SetDlgItemText(IDC_EDIT10, cstrCellIndexTag);

						/*  Edit  */
						{
							_tchar szTemp[3][32];
							swprintf_s(szTemp[0], L"%.2f", m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri][m_uPointIndex]->m_vPoint.x);
							swprintf_s(szTemp[1], L"%.2f", m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri][m_uPointIndex]->m_vPoint.y);
							swprintf_s(szTemp[2], L"%.2f", m_pToolMgr->m_pWorld->m_pNaviTri->m_vecvecNaviTri[m_uPointTri][m_uPointIndex]->m_vPoint.z);

							SetDlgItemText(IDC_EDIT7, szTemp[0]);
							SetDlgItemText(IDC_EDIT8, szTemp[1]);
							SetDlgItemText(IDC_EDIT9, szTemp[2]);
						}

						pointiter->m_bPick = TRUE;
						return;
					}
				}
				uPointIndex++;
			}
			uCellIndex++;
		}
	}
	else if (PICKING_TYPE::SELECT_CELL == m_ePickingType)
	{

	}
}

void CNaviMeshTool::Update_PickingType()
{
	for (_ushort i = 0; i < PICKING_TYPE::PICKING_TYPE_END; ++i)
	{
		if (TRUE == m_ctrlPickingType[i].GetCheck())
			m_ePickingType = PICKING_TYPE(i);
	}
}

void CNaviMeshTool::Update_NaviType()
{
	for (_ushort i = 0; i < Engine::NAVI_TYPE::NAVI_TYPE_END; ++i)
	{
		if (TRUE == m_ctrlNaviType[i].GetCheck())
			m_eNaviType = Engine::NAVI_TYPE(i);
	}
}

_bool CNaviMeshTool::Picking_Point()
{
	return TRUE;
}