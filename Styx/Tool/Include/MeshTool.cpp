// StaticMeshTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MeshTool.h"
#include "afxdialogex.h"

// CMeshTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMeshTool, CDialog)

USING(Engine)

CMeshTool::CMeshTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_STATICMESHTOOL, pParent)
{

}

CMeshTool::~CMeshTool()
{
}

void CMeshTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE3, m_ctrlStaticMeshListTree);
	DDX_Control(pDX, IDC_RADIO1, m_ctrlFillModeRadio[FILLMODE::FILL_WIREFRAME]);
	DDX_Control(pDX, IDC_RADIO2, m_ctrlFillModeRadio[FILLMODE::FILL_SOLID]);
	DDX_Control(pDX, IDC_TREE5, m_ctrlDynamicMeshListTree);
	DDX_Control(pDX, IDC_TREE4, m_ctrlStaticObjectListTree);
	DDX_Control(pDX, IDC_TREE6, m_ctrlDynamicObjectListTree);
	/*  User Value  */
	DDX_Text(pDX, IDC_EDIT1, m_vTransScale.x);
	DDX_Text(pDX, IDC_EDIT2, m_vTransScale.y);
	DDX_Text(pDX, IDC_EDIT3, m_vTransScale.z);
	DDX_Text(pDX, IDC_EDIT4, m_vTransRotation.x);
	DDX_Text(pDX, IDC_EDIT5, m_vTransRotation.y);
	DDX_Text(pDX, IDC_EDIT6, m_vTransRotation.z);
	DDX_Text(pDX, IDC_EDIT7, m_vTransPosition.x);
	DDX_Text(pDX, IDC_EDIT8, m_vTransPosition.y);
	DDX_Text(pDX, IDC_EDIT9, m_vTransPosition.z);
	DDX_Control(pDX, IDC_RADIO3, m_ctrlCatsleIn);
	DDX_Control(pDX, IDC_RADIO4, m_ctrlCatsleOut);
}


BEGIN_MESSAGE_MAP(CMeshTool, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE3, &CMeshTool::OnTvnSelchangedTreeStaticMeshList)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE5, &CMeshTool::OnTvnSelchangedTreeDynamicMeshList)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE4, &CMeshTool::OnTvnSelchangedTreeStaticObjectList)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE6, &CMeshTool::OnTvnSelchangedTreeDynamicObjectList)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CMeshTool::OnSpin_TransScaleX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CMeshTool::OnSpin_TransScaleY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CMeshTool::OnSpin_TransScaleZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CMeshTool::OnSpin_TransRotationX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CMeshTool::OnSpin_TransRotationY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CMeshTool::OnSpin_TransRotationZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CMeshTool::OnSpin_TransPositionX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CMeshTool::OnSpin_TransPositionY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CMeshTool::OnSpin_TransPositionZ)
	ON_EN_CHANGE(IDC_EDIT1, &CMeshTool::OnEdit_TransScaleX)
	ON_EN_CHANGE(IDC_EDIT2, &CMeshTool::OnEdit_TransScaleY)
	ON_EN_CHANGE(IDC_EDIT3, &CMeshTool::OnEdit_TransScaleZ)
	ON_EN_CHANGE(IDC_EDIT4, &CMeshTool::OnEdit_TransRotationX)
	ON_EN_CHANGE(IDC_EDIT5, &CMeshTool::OnEdit_TransRotationY)
	ON_EN_CHANGE(IDC_EDIT6, &CMeshTool::OnEdit_TransRotationZ)
	ON_EN_CHANGE(IDC_EDIT7, &CMeshTool::OnEdit_TransPositionX)
	ON_EN_CHANGE(IDC_EDIT8, &CMeshTool::OnEdit_TransPositionY)
	ON_EN_CHANGE(IDC_EDIT9, &CMeshTool::OnEdit_TransPositionZ)
	ON_BN_CLICKED(IDC_BUTTON1, &CMeshTool::OnButton_Delete)
	ON_BN_CLICKED(IDC_BUTTON11, &CMeshTool::OnButton_Save)
	ON_BN_CLICKED(IDC_BUTTON2, &CMeshTool::OnButton_Load)
	ON_BN_CLICKED(IDC_RADIO3, &CMeshTool::OnBnClickedRadioCatsleIn)
	ON_BN_CLICKED(IDC_RADIO4, &CMeshTool::OnBnClickedRadioCatsleOut)
END_MESSAGE_MAP()


// CMeshTool 메시지 처리기입니다.

/*  StaticMesh List Load  */
HRESULT CMeshTool::Ready_StaticMeshList(CString strPathTag)
{
	m_hMeshRoot = m_ctrlStaticMeshListTree.InsertItem(L"Static_Mesh", 0, 0, TVI_ROOT, TVI_LAST);

	CFileFind	find;

	BOOL bWorking = find.FindFile(strPathTag + L"*.*");

	while (bWorking)
	{
		bWorking = find.FindNextFile();

		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
		{
			CString strDirectoryName = find.GetFileName();

			HTREEITEM  hChild_Folider;
			hChild_Folider = m_ctrlStaticMeshListTree.InsertItem(strDirectoryName, 0, 0, m_hMeshRoot, TVI_LAST);

			CString strDirectoryPath = strPathTag + strDirectoryName;

			CString strFilePath = strDirectoryPath + L"/*.x";
			CFileFind fileFind;
			fileFind.FindFile(strFilePath);

			while (TRUE)
			{
				BOOL bIsExist = fileFind.FindNextFile();

				if (fileFind.IsDots() || fileFind.IsDirectory() || fileFind.IsSystem())
					continue;
				if (!bIsExist)
					break;

				CString strFileName = fileFind.GetFileName();
				m_ctrlStaticMeshListTree.InsertItem(strFileName, 0, 0, hChild_Folider, TVI_LAST);

				CString strTempName = strFileName;
				strTempName.Replace(L".X", L"");
				CString strMeshName = L"Mesh_" + strTempName;
				CString strDirectoryPathTGA = L"../../Client/Bin/Resource/Mesh/StaticMesh/AllTGA/";

				if (strFileName == L"zzz.X")
					continue;

				//_tchar* pNewMeshName = new _tchar[128];
				//_tchar* pNewDirectoryPath = new _tchar[128];
				//_tchar* pNewFileName = new _tchar[128];

				//lstrcpy(pNewMeshName, strMeshName.GetString());
				//lstrcpy(pNewDirectoryPath, strDirectoryPathTGA.GetString());
				//lstrcpy(pNewFileName, strFileName.GetString());

				//Engine::Ready_Meshe(m_pToolMgr->m_pGraphicDev, 
				//						RESOURCE_STAGE, 
				//						pNewMeshName,
				//						Engine::TYPE_STATIC, 
				//						pNewDirectoryPath,
				//						pNewFileName);

				Engine::Ready_Meshe(m_pToolMgr->m_pGraphicDev,
					RESOURCE_STAGE,
					strMeshName.GetString(),
					Engine::TYPE_STATIC,
					strDirectoryPathTGA.GetString(),
					strFileName.GetString());
			}
		}
	}

	return S_OK;
}

/*  DynamicMesh List Load  */
HRESULT CMeshTool::Ready_DynamicMeshList(CString strPathTag)
{
	m_hMeshRoot = m_ctrlDynamicMeshListTree.InsertItem(L"Dynamic_Mesh", 0, 0, TVI_ROOT, TVI_LAST);

	CFileFind	find;

	BOOL bWorking = find.FindFile(strPathTag + L"*.*");

	while (bWorking)
	{
		bWorking = find.FindNextFile();

		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
		{
			CString strDirectoryName = find.GetFileName();

			HTREEITEM  hChild_Folider;
			hChild_Folider = m_ctrlDynamicMeshListTree.InsertItem(strDirectoryName, 0, 0, m_hMeshRoot, TVI_LAST);

			CString strDirectoryPath = strPathTag + strDirectoryName;

			CString strFilePath = strDirectoryPath + L"/*.x";
			CFileFind fileFind;
			fileFind.FindFile(strFilePath);

			while (TRUE)
			{
				BOOL bIsExist = fileFind.FindNextFile();

				if (fileFind.IsDots() || fileFind.IsDirectory() || fileFind.IsSystem())
					continue;
				if (!bIsExist)
					break;

				CString strFileName = fileFind.GetFileName();
				m_ctrlDynamicMeshListTree.InsertItem(strFileName, 0, 0, hChild_Folider, TVI_LAST);

				CString strTempName = strFileName;
				strTempName.Replace(L".X", L"");
				CString strMeshName = L"Mesh_" + strTempName;
				CString strDirectoryPath = L"../../Client/Bin/Resource/Mesh/DynamicMesh/" + strTempName + "/";

				if (strFileName == L"zzz.X")
					continue;

				Engine::Ready_Meshe(m_pToolMgr->m_pGraphicDev,
					RESOURCE_STAGE,
					(Engine::_tchar*)(strMeshName.GetString()),
					Engine::TYPE_DYNAMIC,
					(Engine::_tchar*)(strDirectoryPath.GetString()),
					(Engine::_tchar*)(strFileName.GetString()));
			}
		}
	}

	return S_OK;
}

BOOL CMeshTool::PreTranslateMessage(MSG* pMsg)
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

BOOL CMeshTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pToolMgr = CToolMgr::GetInstance();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_ctrlFillModeRadio[FILLMODE::FILL_SOLID].SetCheck(TRUE);

	m_ctrlCatsleIn.SetCheck(TRUE);
	m_bCatsleIn = TRUE;

	m_hStaticObjectRoot = m_ctrlStaticObjectListTree.InsertItem(L"Static_Object", 0, 0, TVI_ROOT, TVI_LAST);
	m_hDynamicObjectRoot = m_ctrlDynamicObjectListTree.InsertItem(L"Dynamic_Object", 0, 0, TVI_ROOT, TVI_LAST);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMeshTool::OnTvnSelchangedTreeStaticMeshList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	ZeroMemory(m_bMeshTree, sizeof(m_bMeshTree));
	m_bMeshTree[MESHTREE::STATIC_MESH] = true;
	m_bXFileFind = FALSE;

	/*  X File Find  */
	{
		HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;
		CString strXFileCheck;
		AfxExtractSubString(strXFileCheck, m_ctrlStaticMeshListTree.GetItemText(hTreeItem), 1, '.');

		if (strXFileCheck == L"X")
		{
			m_bXFileFind = TRUE;
			CString strTempMeshTag = m_ctrlStaticMeshListTree.GetItemText(hTreeItem);
			strTempMeshTag.Replace(L".X", L"");
			m_strMeshTag = L"Mesh_" + strTempMeshTag;
		}
	}
}

void CMeshTool::OnTvnSelchangedTreeDynamicMeshList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	
	ZeroMemory(m_bMeshTree, sizeof(m_bMeshTree));
	m_bMeshTree[MESHTREE::DYNAMIC_MESH] = true;
	m_bXFileFind = FALSE;

	/*  X File Find  */
	{
		HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;
		CString strXFileCheck;
		AfxExtractSubString(strXFileCheck, m_ctrlDynamicMeshListTree.GetItemText(hTreeItem), 1, '.');

		if (strXFileCheck == L"X")
		{
			m_bXFileFind = TRUE;
			CString strTempMeshTag = m_ctrlDynamicMeshListTree.GetItemText(hTreeItem);
			strTempMeshTag.Replace(L".X", L"");
			m_strMeshTag = L"Mesh_" + strTempMeshTag;
		}
	}
}

void CMeshTool::OnTvnSelchangedTreeStaticObjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	ZeroMemory(m_bMeshTree, sizeof(m_bMeshTree));
	m_bMeshTree[MESHTREE::STATIC_OBJECT] = true;
	m_bXFileFind = FALSE;

	/*  X File Find  */
	{
		HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;
		//CString strXFileCheck;
		//AfxExtractSubString(strXFileCheck, m_ctrlStaticObjectListTree.GetItemText(hTreeItem), 1, '.');

		CString strCheck = m_ctrlStaticObjectListTree.GetItemText(hTreeItem).Left(2);

		if (L"No" == strCheck)
		{
			m_bXFileFind = TRUE;
			m_strMeshTag = m_ctrlStaticObjectListTree.GetItemText(hTreeItem).Mid(5);

			/*  10 <=  */
			if (0 == m_strMeshTag.FindOneOf(L" "))
			{
				m_strMeshTag = m_ctrlStaticObjectListTree.GetItemText(hTreeItem).Mid(6);
				m_dwNum = _wtoi(m_ctrlStaticObjectListTree.GetItemText(hTreeItem).Mid(2, 2)) - 1;
			}
			/*  10 >  */
			else
				m_dwNum = _wtoi(m_ctrlStaticObjectListTree.GetItemText(hTreeItem).Mid(2, 1)) - 1;

			m_strMeshTag = L"Mesh_" + m_strMeshTag;

			CTransform* pTransform = dynamic_cast<CTransform*>(m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()][m_dwNum]->Get_Component(COM_TRANSFORM));

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
			dynamic_cast<CFeature*>(m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()][m_dwNum])->m_bPick = TRUE;

			m_ctrlCatsleIn.SetCheck(FALSE);
			m_ctrlCatsleOut.SetCheck(FALSE);

			if (TRUE == dynamic_cast<CFeature*>(m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()][m_dwNum])->m_bCatsleIn)
				m_ctrlCatsleIn.SetCheck(TRUE);
			else
				m_ctrlCatsleOut.SetCheck(TRUE);

			UpdateData(TRUE);

			m_vTransScale = pTransform->m_vScale;
			m_vTransRotation = D3DXToDegree(pTransform->m_vAngle);
			m_vTransPosition = pTransform->m_vInfo[INFO::INFO_POS];

			UpdateData(FALSE);

			SetDlgItemText(IDC_EDIT10, m_ctrlStaticObjectListTree.GetItemText(hTreeItem));

			Invalidate(TRUE);
		}
	}
}

void CMeshTool::OnTvnSelchangedTreeDynamicObjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	ZeroMemory(m_bMeshTree, sizeof(m_bMeshTree));
	m_bMeshTree[MESHTREE::DYNAMIC_OBJECT] = true;
	m_bXFileFind = FALSE;

	/*  X File Find  */
	{
		HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;
		//CString strXFileCheck;
		//AfxExtractSubString(strXFileCheck, m_ctrlStaticObjectListTree.GetItemText(hTreeItem), 1, '.');

		CString strCheck = m_ctrlDynamicObjectListTree.GetItemText(hTreeItem).Left(2);

		if (L"No" == strCheck)
		{
			m_bXFileFind = TRUE;
			m_strMeshTag = m_ctrlDynamicObjectListTree.GetItemText(hTreeItem).Mid(5);

			/*  10 <=  */
			if (0 == m_strMeshTag.FindOneOf(L" "))
			{
				m_strMeshTag = m_ctrlDynamicObjectListTree.GetItemText(hTreeItem).Mid(6);
				m_dwNum = _wtoi(m_ctrlDynamicObjectListTree.GetItemText(hTreeItem).Mid(2, 2)) - 1;
			}
			/*  10 >  */
			else
				m_dwNum = _wtoi(m_ctrlDynamicObjectListTree.GetItemText(hTreeItem).Mid(2, 1)) - 1;

			m_strMeshTag = L"Mesh_" + m_strMeshTag;

			CTransform* pTransform = dynamic_cast<CTransform*>(m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()][m_dwNum]->Get_Component(COM_TRANSFORM));

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
			dynamic_cast<CDynamicFeature*>(m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()][m_dwNum])->m_bPick = TRUE;

			UpdateData(TRUE);

			m_vTransScale = pTransform->m_vScale;
			m_vTransRotation = D3DXToDegree(pTransform->m_vAngle);
			m_vTransPosition = pTransform->m_vInfo[INFO::INFO_POS];

			UpdateData(FALSE);

			SetDlgItemText(IDC_EDIT10, m_ctrlDynamicObjectListTree.GetItemText(hTreeItem));

			Invalidate(TRUE);
		}
	}
}

HRESULT CMeshTool::Add_ObjectList(Engine::MESHTYPE _eMeshType)
{
	switch (_eMeshType)
	{
	case Engine::MESHTYPE::TYPE_STATIC:
	{
		m_ctrlStaticObjectListTree.DeleteAllItems();
		m_hStaticObjectRoot = m_ctrlStaticObjectListTree.InsertItem(L"Static_Object", 0, 0, TVI_ROOT, TVI_LAST);
		
		for (auto& iter : m_pToolMgr->m_mapFeatureVector)
		{
			if (std::string::npos == iter.first.find(L"0"))
				continue;

			CString strDirectory = iter.first.c_str();
			strDirectory.Replace(L"Mesh_", L"");
			//AfxExtractSubString(strDirectory, iter.first.c_str(), 1, '_'); // Mesh_Arnchor_00 -> Arnchor
			HTREEITEM hChild_Folider;
			hChild_Folider = m_ctrlStaticObjectListTree.InsertItem(strDirectory, 0, 0, m_hStaticObjectRoot, TVI_LAST);
			
			int iNum = 1;

			for (auto& veciter : iter.second)
			{
				CString strMeshTag = iter.first.c_str();
				strMeshTag.Replace(L"Mesh_", L"");

				TCHAR szNum[32] = L"";
				swprintf_s(szNum, L"%d", iNum);
				CString strFullMeshTag;
				strFullMeshTag = L"No";
				strFullMeshTag += szNum;
				strFullMeshTag += L". " + strMeshTag;

				m_ctrlStaticObjectListTree.InsertItem(strFullMeshTag, 0, 0, hChild_Folider, TVI_LAST);
				iNum++;

				SetDlgItemText(IDC_EDIT10, strFullMeshTag);
			}
		}

		/*  Expand  */
		HTREEITEM hItem;
		hItem = m_ctrlStaticObjectListTree.GetFirstVisibleItem();
		while (NULL != hItem)
		{
			m_ctrlStaticObjectListTree.Expand(hItem, TVE_EXPAND);
			hItem = m_ctrlStaticObjectListTree.GetNextItem(hItem, TVGN_NEXTVISIBLE);
		}
		break;
	}
	case Engine::MESHTYPE::TYPE_DYNAMIC:
		m_ctrlDynamicObjectListTree.DeleteAllItems();
		m_hDynamicObjectRoot = m_ctrlDynamicObjectListTree.InsertItem(L"Dynamic_Object", 0, 0, TVI_ROOT, TVI_LAST);

		for (auto& iter : m_pToolMgr->m_mapFeatureVector)
		{
			if (std::string::npos != iter.first.find(L"0"))
				continue;

			CString strDirectory = iter.first.c_str();
			strDirectory.Replace(L"Mesh_", L"");
			//AfxExtractSubString(strDirectory, iter.first.c_str(), 1, '_'); // Mesh_Styx -> Styx
			HTREEITEM hChild_Folider;
			hChild_Folider = m_ctrlDynamicObjectListTree.InsertItem(strDirectory, 0, 0, m_hDynamicObjectRoot, TVI_LAST);

			int iNum = 1;

			for (auto& veciter : iter.second)
			{
				CString strMeshTag = iter.first.c_str();
				strMeshTag.Replace(L"Mesh_", L"");

				TCHAR szNum[32] = L"";
				swprintf_s(szNum, L"%d", iNum);
				CString strFullMeshTag;
				strFullMeshTag = L"No";
				strFullMeshTag += szNum;
				strFullMeshTag += L". " + strMeshTag;

				m_ctrlDynamicObjectListTree.InsertItem(strFullMeshTag, 0, 0, hChild_Folider, TVI_LAST);
				iNum++;

				SetDlgItemText(IDC_EDIT10, strFullMeshTag);
			}
		}

		/*  Expand  */
		HTREEITEM hItem;
		hItem = m_ctrlDynamicObjectListTree.GetFirstVisibleItem();
		while (NULL != hItem)
		{
			m_ctrlDynamicObjectListTree.Expand(hItem, TVE_EXPAND);
			hItem = m_ctrlDynamicObjectListTree.GetNextItem(hItem, TVGN_NEXTVISIBLE);
		}
		break;
	}

	m_dwNum = m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()].size() - 1;
	CTransform* pTransform = dynamic_cast<CTransform*>(m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()][m_dwNum]->Get_Component(COM_TRANSFORM));

	UpdateData(TRUE);

	m_vTransScale = pTransform->m_vScale;
	m_vTransRotation = pTransform->m_vAngle;
	m_vTransPosition = pTransform->m_vInfo[INFO::INFO_POS];

	UpdateData(FALSE);

	Invalidate(TRUE);

	return S_OK;
}

void CMeshTool::OnBnClickedRadioCatsleIn()
{
	m_bCatsleIn = TRUE;

	if (TRUE == m_bXFileFind 
		&& nullptr != m_strMeshTag.GetString() 
		&& TRUE == m_bMeshTree[MESHTREE::STATIC_OBJECT])
	{
		dynamic_cast<CFeature*>(m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()][m_dwNum])->m_bCatsleIn = m_bCatsleIn;
	}
}

void CMeshTool::OnBnClickedRadioCatsleOut()
{
	m_bCatsleIn = FALSE;

	if (TRUE == m_bXFileFind 
		&& nullptr != m_strMeshTag.GetString()
		&& TRUE == m_bMeshTree[MESHTREE::STATIC_OBJECT])
	{
		dynamic_cast<CFeature*>(m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()][m_dwNum])->m_bCatsleIn = m_bCatsleIn;
	}
}

void CMeshTool::OnSpin_TransScaleX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, &m_vTransScale.x, IDC_EDIT1, MESH_INFO::TRANS_SCALE);
}

void CMeshTool::OnSpin_TransScaleY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, &m_vTransScale.y, IDC_EDIT2, MESH_INFO::TRANS_SCALE);
}

void CMeshTool::OnSpin_TransScaleZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, &m_vTransScale.z, IDC_EDIT3, MESH_INFO::TRANS_SCALE);
}

void CMeshTool::OnSpin_TransRotationX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, &m_vTransRotation.x, IDC_EDIT4, MESH_INFO::TRANS_ROTATION);
}

void CMeshTool::OnSpin_TransRotationY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, &m_vTransRotation.y, IDC_EDIT5, MESH_INFO::TRANS_ROTATION);
}

void CMeshTool::OnSpin_TransRotationZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, &m_vTransRotation.z, IDC_EDIT6, MESH_INFO::TRANS_ROTATION);
}

void CMeshTool::OnSpin_TransPositionX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, &m_vTransPosition.x, IDC_EDIT7, MESH_INFO::TRANS_POSITION);
}

void CMeshTool::OnSpin_TransPositionY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, &m_vTransPosition.y, IDC_EDIT8, MESH_INFO::TRANS_POSITION);
}

void CMeshTool::OnSpin_TransPositionZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, &m_vTransPosition.z, IDC_EDIT9, MESH_INFO::TRANS_POSITION);
}

void CMeshTool::OnEdit_TransScaleX()
{
	OnEdit();
}

void CMeshTool::OnEdit_TransScaleY()
{
	OnEdit();
}

void CMeshTool::OnEdit_TransScaleZ()
{
	OnEdit();
}

void CMeshTool::OnEdit_TransRotationX()
{
	OnEdit();
}

void CMeshTool::OnEdit_TransRotationY()
{
	OnEdit();
}

void CMeshTool::OnEdit_TransRotationZ()
{
	OnEdit();
}

void CMeshTool::OnEdit_TransPositionX()
{
	OnEdit();
}

void CMeshTool::OnEdit_TransPositionY()
{
	OnEdit();
}

void CMeshTool::OnEdit_TransPositionZ()
{
	OnEdit();
}

void CMeshTool::OnSpin(LPNMUPDOWN _pNMUpdDown, Engine::_float* _fValue, DWORD _dwID, MESH_INFO _eMeshInfo)
{
	//CString c;
	//GetDlgItemText(_dwID, c);
	//*_fValue = (Engine::_float)(_wtof(c.GetString()));

	if (FALSE == m_bXFileFind || nullptr == m_strMeshTag.GetString())
		return;

	if (_pNMUpdDown->iDelta < 0)
	{
		if (MESH_INFO::TRANS_SCALE == _eMeshInfo)
			*_fValue += 0.005f;
		else if (MESH_INFO::TRANS_ROTATION == _eMeshInfo)
			*_fValue += 5.f;
		else if (MESH_INFO::TRANS_POSITION == _eMeshInfo)
			*_fValue += 0.05f;
	}
	else
	{
		if (MESH_INFO::TRANS_SCALE == _eMeshInfo)
			*_fValue -= 0.005f;
		else if (MESH_INFO::TRANS_ROTATION == _eMeshInfo)
			*_fValue -= 5.f;
		else if (MESH_INFO::TRANS_POSITION == _eMeshInfo)
			*_fValue -= 0.05f;
	}

	CTransform* pTransform = dynamic_cast<CTransform*>(m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()][m_dwNum]->Get_Component(COM_TRANSFORM));

	UpdateData(FALSE);

	pTransform->m_vScale = m_vTransScale;
	pTransform->m_vAngle = D3DXToRadian(m_vTransRotation);
	pTransform->m_vInfo[INFO::INFO_POS] = m_vTransPosition;

	UpdateData(TRUE);

	Invalidate(TRUE);

	//TCHAR szTemp[32] = L"";
	//swprintf_s(szTemp, L"%.2f", *_fValue);
	//SetDlgItemText(_dwID, szTemp);
}

void CMeshTool::OnEdit()
{
	CTransform* pTransform = nullptr;
	if (TRUE == m_bXFileFind)
		pTransform = dynamic_cast<CTransform*>(m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()][m_dwNum]->Get_Component(COM_TRANSFORM));
	else
		return;

	CString strTransScaleX, strTransScaleY, strTransScaleZ;
	CString strTransRotationX, strTransRotationY, strTransRotationZ;
	CString strTransPositionX, strTransPositionY, strTransPositionZ;

	GetDlgItemText(IDC_EDIT1, strTransScaleX);
	GetDlgItemText(IDC_EDIT2, strTransScaleY);
	GetDlgItemText(IDC_EDIT3, strTransScaleZ);
	GetDlgItemText(IDC_EDIT4, strTransRotationX);
	GetDlgItemText(IDC_EDIT5, strTransRotationY);
	GetDlgItemText(IDC_EDIT6, strTransRotationZ);
	GetDlgItemText(IDC_EDIT7, strTransPositionX);
	GetDlgItemText(IDC_EDIT8, strTransPositionY);
	GetDlgItemText(IDC_EDIT9, strTransPositionZ);

	Engine::_vec3 vScale = { (float)_wtof(strTransScaleX), (float)_wtof(strTransScaleY), (float)_wtof(strTransScaleZ) };
	Engine::_vec3 vRot	 = { (float)_wtof(strTransRotationX), (float)_wtof(strTransRotationY), (float)_wtof(strTransRotationZ) };
	Engine::_vec3 vPos   = { (float)_wtof(strTransPositionX), (float)_wtof(strTransPositionY), (float)_wtof(strTransPositionZ) };

	pTransform->m_vScale				= vScale;
	pTransform->m_vAngle				= D3DXToRadian(vRot);
	pTransform->m_vInfo[INFO::INFO_POS] = vPos;

	m_vTransScale	 = pTransform->m_vScale;
	m_vTransRotation = D3DXToDegree(pTransform->m_vAngle);
	m_vTransPosition = pTransform->m_vInfo[INFO::INFO_POS];

	Invalidate(TRUE);
}

void CMeshTool::OnButton_Delete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (FALSE == m_bXFileFind)
		return;

	if (FALSE == m_bMeshTree[MESHTREE::STATIC_OBJECT] && FALSE == m_bMeshTree[MESHTREE::DYNAMIC_OBJECT])
		return;

	m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()][m_dwNum];

	auto& iter = m_pToolMgr->m_mapFeatureVector.find(m_strMeshTag.GetString());

	if (iter == m_pToolMgr->m_mapFeatureVector.end())
		return;
	
	if (m_pToolMgr->m_pWorld->Get_Layer(L"GameLogic")->Tool_DeleteObject(m_strMeshTag.GetString(), iter->second[m_dwNum]))
	{
		Engine::Safe_Release(iter->second[m_dwNum]);

	}
	iter->second.erase(iter->second.begin() + m_dwNum);

	if (TRUE == m_bMeshTree[MESHTREE::STATIC_OBJECT])
	{
		m_ctrlStaticObjectListTree.DeleteAllItems();
		m_hStaticObjectRoot = m_ctrlStaticObjectListTree.InsertItem(L"Static_Object", 0, 0, TVI_ROOT, TVI_LAST);

		for (auto& iter : m_pToolMgr->m_mapFeatureVector)
		{
			if (std::string::npos == iter.first.find(L"0"))
				continue;

			CString strDirectory = iter.first.c_str();
			strDirectory.Replace(L"Mesh_", L"");
			//AfxExtractSubString(strDirectory, iter.first.c_str(), 1, '_'); // Mesh_Arnchor_00 -> Arnchor
			HTREEITEM hChild_Folider;
			hChild_Folider = m_ctrlStaticObjectListTree.InsertItem(strDirectory, 0, 0, m_hStaticObjectRoot, TVI_LAST);

			int iNum = 1;

			for (auto& veciter : iter.second)
			{
				CString strMeshTag = iter.first.c_str();
				strMeshTag.Replace(L"Mesh_", L"");

				TCHAR szNum[32] = L"";
				swprintf_s(szNum, L"%d", iNum);
				CString strFullMeshTag;
				strFullMeshTag = L"No";
				strFullMeshTag += szNum;
				strFullMeshTag += L". " + strMeshTag;

				m_ctrlStaticObjectListTree.InsertItem(strFullMeshTag, 0, 0, hChild_Folider, TVI_LAST);
				iNum++;

			}
		}

		/*  Expand  */
		HTREEITEM hItem;
		hItem = m_ctrlStaticObjectListTree.GetFirstVisibleItem();
		while (NULL != hItem)
		{
			m_ctrlStaticObjectListTree.Expand(hItem, TVE_EXPAND);
			hItem = m_ctrlStaticObjectListTree.GetNextItem(hItem, TVGN_NEXTVISIBLE);
		}
	}
	else if (TRUE == m_bMeshTree[MESHTREE::DYNAMIC_OBJECT])
	{
		m_ctrlDynamicObjectListTree.DeleteAllItems();
		m_hDynamicObjectRoot = m_ctrlDynamicObjectListTree.InsertItem(L"Dynamic_Object", 0, 0, TVI_ROOT, TVI_LAST);

		for (auto& iter : m_pToolMgr->m_mapFeatureVector)
		{
			if (std::string::npos != iter.first.find(L"0"))
				continue;

			CString strDirectory = iter.first.c_str();
			strDirectory.Replace(L"Mesh_", L"");
			//AfxExtractSubString(strDirectory, iter.first.c_str(), 1, '_'); // Mesh_Arnchor_00 -> Arnchor
			HTREEITEM hChild_Folider;
			hChild_Folider = m_ctrlDynamicObjectListTree.InsertItem(strDirectory, 0, 0, m_hDynamicObjectRoot, TVI_LAST);

			int iNum = 1;

			for (auto& veciter : iter.second)
			{
				CString strMeshTag = iter.first.c_str();
				strMeshTag.Replace(L"Mesh_", L"");

				TCHAR szNum[32] = L"";
				swprintf_s(szNum, L"%d", iNum);
				CString strFullMeshTag;
				strFullMeshTag = L"No";
				strFullMeshTag += szNum;
				strFullMeshTag += L". " + strMeshTag;

				m_ctrlDynamicObjectListTree.InsertItem(strFullMeshTag, 0, 0, hChild_Folider, TVI_LAST);
				iNum++;

			}
		}

		/*  Expand  */
		HTREEITEM hItem;
		hItem = m_ctrlDynamicObjectListTree.GetFirstVisibleItem();
		while (NULL != hItem)
		{
			m_ctrlDynamicObjectListTree.Expand(hItem, TVE_EXPAND);
			hItem = m_ctrlDynamicObjectListTree.GetNextItem(hItem, TVGN_NEXTVISIBLE);
		}
	}

	UpdateData(TRUE);

	ZeroMemory(m_vTransScale, sizeof(Engine::_vec3));
	ZeroMemory(m_vTransRotation, sizeof(Engine::_vec3));
	ZeroMemory(m_vTransPosition, sizeof(Engine::_vec3));

	UpdateData(FALSE);

	SetDlgItemText(IDC_EDIT10, L"");

	Invalidate(TRUE);
}


void CMeshTool::OnButton_Save()
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

		for (auto& iter : m_pToolMgr->m_mapFeatureVector)
		{
			for (auto& veciter : iter.second)
			{
				Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(veciter->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));

				FEATURE tFeature;

				if (std::string::npos != iter.first.find(L"0"))
				{
					tFeature.bCatsleIn = dynamic_cast<CFeature*>(veciter)->m_bCatsleIn;
					tFeature.eMeshType = MESHTYPE::TYPE_STATIC;
				}
				else if(std::string::npos == iter.first.find(L"0"))
					tFeature.eMeshType = MESHTYPE::TYPE_DYNAMIC;

				lstrcpy(tFeature.szMeshTag, iter.first.c_str());

				tFeature.vScale = pTransform->m_vScale;
				tFeature.vRotation = pTransform->m_vAngle;
				tFeature.vPosition = pTransform->m_vInfo[Engine::INFO_POS];

				WriteFile(hFile, &tFeature, sizeof(FEATURE), &dwByte, nullptr);
			
				if(0 == dwByte)
					break;
			}
		}

		/*  Static Mesh  */
		hFile = CreateFile(L"../../Data/Client - StaticMeshLoadList.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Save Failed");
			return;
		}

		for (auto& iter : m_pToolMgr->m_mapFeatureVector)
		{
			if (std::string::npos == iter.first.find(L"0"))
				continue;

			Engine::_tchar szMeshTag[128];
			lstrcpy(szMeshTag, iter.first.c_str());

			WriteFile(hFile, szMeshTag, sizeof(szMeshTag), &dwByte, nullptr);

			if (0 == dwByte)
				break;
		}

		/*  Dynamic Mesh  */
		hFile = CreateFile(L"../../Data/Client - DynamicMeshLoadList.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Save Failed");
			return;
		}

		for (auto& iter : m_pToolMgr->m_mapFeatureVector)
		{
			if (std::string::npos != iter.first.find(L"0"))
				continue;

			Engine::_tchar szMeshTag[128];
			lstrcpy(szMeshTag, iter.first.c_str());

			WriteFile(hFile, szMeshTag, sizeof(szMeshTag), &dwByte, nullptr);

			if (0 == dwByte)
				break;
		}


		CloseHandle(hFile);
		AfxMessageBox(L"Save Complete");
	}

	Invalidate(FALSE);
}

void CMeshTool::OnButton_Load()
{
	UpdateData(TRUE);

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

		DWORD dwByte = 0;

		if(m_pToolMgr->m_pWorld->Get_Layer(L"GameLogic"))
			m_pToolMgr->m_pWorld->Get_Layer(L"GameLogic")->Tool_DeleteAll();

		for_each(m_pToolMgr->m_mapFeatureVector.begin(), m_pToolMgr->m_mapFeatureVector.end(), [&](auto& iter){
			for(auto& veciter : iter.second)
				Engine::Safe_Release(veciter);
			iter.second.clear();
			iter.second.shrink_to_fit();
		});
		m_pToolMgr->m_mapFeatureVector.clear();

		while (TRUE)
		{
			FEATURE tFeature;
			ReadFile(hFile, &tFeature, sizeof(FEATURE), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			m_strMeshTag = tFeature.szMeshTag;
			m_dwNum = 0;

			Engine::CGameObject* pGameFeature = nullptr;
			if (MESHTYPE::TYPE_STATIC == tFeature.eMeshType)
			{
				pGameFeature = m_pToolMgr->m_pWorld->Create_StaticMeshObject((Engine::_tchar*)(m_strMeshTag.GetString()));
				dynamic_cast<CFeature*>(pGameFeature)->m_bCatsleIn = tFeature.bCatsleIn;
			}
			else if (MESHTYPE::TYPE_DYNAMIC == tFeature.eMeshType)
				pGameFeature = m_pToolMgr->m_pWorld->Create_DynamicMeshObject((Engine::_tchar*)(m_strMeshTag.GetString()));

			auto& iter = m_pToolMgr->m_mapFeatureVector.find(m_strMeshTag.GetString());
			if (iter == m_pToolMgr->m_mapFeatureVector.end())
			{
				m_pToolMgr->m_mapFeatureVector.emplace(m_strMeshTag.GetString(), vector<Engine::CGameObject*>());
			}
			m_pToolMgr->m_mapFeatureVector[m_strMeshTag.GetString()].push_back(pGameFeature);

			Add_ObjectList(tFeature.eMeshType);

			Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(pGameFeature->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));
			pTransform->m_vScale = tFeature.vScale;
			pTransform->m_vAngle = tFeature.vRotation;
			pTransform->m_vInfo[Engine::INFO_POS] = tFeature.vPosition;

		}
		
		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Load Failed");
			return;
		}

		CloseHandle(hFile);
		AfxMessageBox(L"Load Complete");
	}

	Invalidate(FALSE);
	UpdateData(FALSE);
}

