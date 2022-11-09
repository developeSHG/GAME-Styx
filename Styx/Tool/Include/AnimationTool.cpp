// AnimationTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "AnimationTool.h"
#include "afxdialogex.h"

#include "DynamicMesh.h"

// CAnimationTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimationTool, CDialog)

CAnimationTool::CAnimationTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ANIMATIONTOOL, pParent)
{

}

CAnimationTool::~CAnimationTool()
{
}

void CAnimationTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE4, m_ctrlDynamicMeshListTree);
	DDX_Control(pDX, IDC_LIST1, m_ctrlCurrentAniList);
	DDX_Control(pDX, IDC_LIST2, m_ctrlNewAniList);
}


BEGIN_MESSAGE_MAP(CAnimationTool, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CAnimationTool::OnSpin_CurrentWeight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN32, &CAnimationTool::OnSpin_CurrentSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN33, &CAnimationTool::OnSpin_CurrentDuration)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN34, &CAnimationTool::OnSpin_New)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN35, &CAnimationTool::OnSpin_NewSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN36, &CAnimationTool::OnSpin_NewDuration)
	ON_EN_CHANGE(IDC_EDIT1, &CAnimationTool::OnEdit_CurrentWeight)
	ON_EN_CHANGE(IDC_EDIT2, &CAnimationTool::OnEdit_CurrentSpeed)
	ON_EN_CHANGE(IDC_EDIT32, &CAnimationTool::OnEdit_CurrentDuration)
	ON_EN_CHANGE(IDC_EDIT33, &CAnimationTool::OnEdit_NewWeight)
	ON_EN_CHANGE(IDC_EDIT34, &CAnimationTool::OnEdit_NewSpeed)
	ON_EN_CHANGE(IDC_EDIT35, &CAnimationTool::OnEdit_NewDuration)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE4, &CAnimationTool::OnTvnSelchangedTreeDynamicMeshList)
	ON_LBN_SELCHANGE(IDC_LIST1, &CAnimationTool::OnLbnSelchangeCurrentAniList)
	ON_LBN_SELCHANGE(IDC_LIST2, &CAnimationTool::OnLbnSelchangeNewAniList)
END_MESSAGE_MAP()


// CAnimationTool 메시지 처리기입니다.

BOOL CAnimationTool::PreTranslateMessage(MSG* pMsg)
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

HRESULT CAnimationTool::Ready_DynamicMeshList(CString strPathTag)
{
	HTREEITEM hMeshRoot = m_ctrlDynamicMeshListTree.InsertItem(L"Dynamic_Mesh", 0, 0, TVI_ROOT, TVI_LAST);

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
			hChild_Folider = m_ctrlDynamicMeshListTree.InsertItem(strDirectoryName, 0, 0, hMeshRoot, TVI_LAST);

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

BOOL CAnimationTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_pToolMgr = CToolMgr::GetInstance();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAnimationTool::OnTvnSelchangedTreeDynamicMeshList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;

	/*  XFile FInd  */
	{
		CString cstrMeshFileTag = m_ctrlDynamicMeshListTree.GetItemText(hTreeItem);

		if (-1 != cstrMeshFileTag.Find(L"X"))
		{
			if (nullptr != m_pDynamicFeature)
			{
				Engine::Safe_Release(m_pDynamicFeature);
				m_pToolMgr->m_pSpace->Get_Layer(L"GameLogic")->Tool_DeleteAll();
				m_ctrlCurrentAniList.ResetContent();
				m_ctrlNewAniList.ResetContent();
			}

			cstrMeshFileTag.Replace(L".X", L"");
			CString  cstrMeshNameTag = L"Mesh_";
			cstrMeshNameTag += cstrMeshFileTag;
			m_pDynamicFeature = dynamic_cast<CDynamicFeature*>(m_pToolMgr->m_pSpace->Create_DynamicMeshObject((Engine::_tchar*)(cstrMeshNameTag.GetString())));

			Engine::CDynamicMesh* pDynamicMesh = dynamic_cast<Engine::CDynamicMesh*>(m_pDynamicFeature->Get_Component(Engine::COM_MESH));
			LPD3DXANIMATIONCONTROLLER d3dxAniCtrl = pDynamicMesh->Get_AniCtrl()->Get_AniCtrl();

			const _uint uMaxNumAniSet = d3dxAniCtrl->GetMaxNumAnimationSets();

			for (_uint i = 0; i < uMaxNumAniSet; i++)
			{
				LPD3DXANIMATIONSET d3dxAniSet;
				d3dxAniCtrl->GetAnimationSet(i, &d3dxAniSet);

				Engine::_tchar szTemp[64];
				swprintf_s(szTemp, L"%d", i);
				CString cstr = szTemp;
				cstr += L". ";
				cstr += (CA2W)(d3dxAniSet->GetName());

				m_ctrlCurrentAniList.AddString(cstr);
				m_ctrlNewAniList.AddString(cstr);
			}

			m_ctrlCurrentAniList.SetCurSel(0);
			m_ctrlNewAniList.SetCurSel(0);

			_tchar szTemp[6][32];
			swprintf_s(szTemp[0], L"%.2f", m_pDynamicFeature->m_tCurrentAniCtrlInfo.fTrackSpeed);
			swprintf_s(szTemp[1], L"%.2f", m_pDynamicFeature->m_tCurrentAniCtrlInfo.fWeight);
			swprintf_s(szTemp[2], L"%.2f", m_pDynamicFeature->m_tCurrentAniCtrlInfo.fDuration);
			swprintf_s(szTemp[3], L"%.2f", m_pDynamicFeature->m_tNewAniCtrlInfo.fTrackSpeed);
			swprintf_s(szTemp[4], L"%.2f", m_pDynamicFeature->m_tNewAniCtrlInfo.fWeight);
			swprintf_s(szTemp[5], L"%.2f", m_pDynamicFeature->m_tNewAniCtrlInfo.fDuration);

			SetDlgItemText(IDC_EDIT1, szTemp[0]);
			SetDlgItemText(IDC_EDIT2, szTemp[1]);
			SetDlgItemText(IDC_EDIT32, szTemp[2]);
			SetDlgItemText(IDC_EDIT33, szTemp[3]);
			SetDlgItemText(IDC_EDIT34, szTemp[4]);
			SetDlgItemText(IDC_EDIT35, szTemp[5]);
		}
	}
}

void CAnimationTool::OnLbnSelchangeCurrentAniList()
{
	_uint uIndex = m_ctrlCurrentAniList.GetCurSel();

	Engine::CDynamicMesh* pDynamicMesh = dynamic_cast<Engine::CDynamicMesh*>(m_pDynamicFeature->Get_Component(Engine::COM_MESH));
	pDynamicMesh->Get_AniCtrl()->Set_AnimationSet(uIndex);

	m_pDynamicFeature->m_uCurrentIndex = uIndex;
}

void CAnimationTool::OnLbnSelchangeNewAniList()
{
	_uint uIndex = m_ctrlNewAniList.GetCurSel();

	Engine::CDynamicMesh* pDynamicMesh = dynamic_cast<Engine::CDynamicMesh*>(m_pDynamicFeature->Get_Component(Engine::COM_MESH));
	pDynamicMesh->Get_AniCtrl()->Set_AnimationSet(uIndex);

	m_pDynamicFeature->m_uNexIndex = uIndex;
}

void CAnimationTool::OnSpin_CurrentWeight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN3);
}

void CAnimationTool::OnSpin_CurrentSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN32);
}

void CAnimationTool::OnSpin_CurrentDuration(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN33);
}

void CAnimationTool::OnSpin_New(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN34);
}

void CAnimationTool::OnSpin_NewSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN35);
}

void CAnimationTool::OnSpin_NewDuration(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	OnSpin(pNMUpDown, IDC_SPIN36);
}

void CAnimationTool::OnEdit_CurrentWeight()
{
	OnEdit();
}

void CAnimationTool::OnEdit_CurrentSpeed()
{
	OnEdit();
}

void CAnimationTool::OnEdit_CurrentDuration()
{
	OnEdit();
}

void CAnimationTool::OnEdit_NewWeight()
{
	OnEdit();
}

void CAnimationTool::OnEdit_NewSpeed()
{
	OnEdit();
}

void CAnimationTool::OnEdit_NewDuration()
{
	OnEdit();
}

void CAnimationTool::OnSpin(LPNMUPDOWN _pNMUpdDown, DWORD _dwID)
{
	if (nullptr == m_pDynamicFeature)
		return;

	CString cstrCurrentSpeed, cstrCurrentWeight, cstrCurrentDuration;
	CString cstrNewSpeed, cstrNewWeight, cstrNewDuration;

	GetDlgItemText(IDC_EDIT1, cstrCurrentSpeed);
	GetDlgItemText(IDC_EDIT2, cstrCurrentWeight);
	GetDlgItemText(IDC_EDIT32, cstrCurrentDuration);
	GetDlgItemText(IDC_EDIT33, cstrNewSpeed);
	GetDlgItemText(IDC_EDIT34, cstrNewWeight);
	GetDlgItemText(IDC_EDIT35, cstrNewDuration);

	_float fCurrentSpeed = (float)_wtof(cstrCurrentSpeed);
	_float fCurrentWeight = (float)_wtof(cstrCurrentWeight);
	_float fCurrentDuration = (float)_wtof(cstrCurrentDuration);
	_float fNewSpeed = (float)_wtof(cstrNewSpeed);
	_float fNewWeight = (float)_wtof(cstrNewWeight);
	_float fNewDuration = (float)_wtof(cstrNewDuration);

	float fValue = 0.f;

	if (_pNMUpdDown->iDelta < 0)
	{
		if (IDC_SPIN32 == _dwID || IDC_SPIN33 == _dwID
			|| IDC_SPIN35 == _dwID || IDC_SPIN36 == _dwID)
			fValue += 0.05f;
		else
			fValue += 0.1f;
	}
	else
	{
		if (IDC_SPIN32 == _dwID || IDC_SPIN33 == _dwID
			|| IDC_SPIN35 == _dwID || IDC_SPIN36 == _dwID)
			fValue -= 0.05f;
		else
			fValue -= 0.1f;
	}

	if (IDC_SPIN3 == _dwID)
		fCurrentSpeed += fValue;
	else if (IDC_SPIN32 == _dwID)
		fCurrentWeight += fValue;
	else if (IDC_SPIN33 == _dwID)
		fCurrentDuration += fValue;
	else if (IDC_SPIN34 == _dwID)
		fNewSpeed += fValue;
	else if (IDC_SPIN35 == _dwID)
		fNewWeight += fValue;
	else if (IDC_SPIN36 == _dwID)
		fNewDuration += fValue;

	m_pDynamicFeature->m_tCurrentAniCtrlInfo.fTrackSpeed = fCurrentSpeed;
	m_pDynamicFeature->m_tCurrentAniCtrlInfo.fWeight = fCurrentWeight;
	m_pDynamicFeature->m_tCurrentAniCtrlInfo.fDuration = fCurrentDuration;
	m_pDynamicFeature->m_tNewAniCtrlInfo.fTrackSpeed = fNewSpeed;
	m_pDynamicFeature->m_tNewAniCtrlInfo.fWeight = fNewWeight;
	m_pDynamicFeature->m_tNewAniCtrlInfo.fDuration = fNewDuration;

	_tchar szTemp[6][32];
	swprintf_s(szTemp[0], L"%.2f", fCurrentSpeed);
	swprintf_s(szTemp[1], L"%.2f", fCurrentWeight);
	swprintf_s(szTemp[2], L"%.2f", fCurrentDuration);
	swprintf_s(szTemp[3], L"%.2f", fNewSpeed);
	swprintf_s(szTemp[4], L"%.2f", fNewWeight);
	swprintf_s(szTemp[5], L"%.2f", fNewDuration);

	SetDlgItemText(IDC_EDIT1, szTemp[0]);
	SetDlgItemText(IDC_EDIT2, szTemp[1]);
	SetDlgItemText(IDC_EDIT32, szTemp[2]);
	SetDlgItemText(IDC_EDIT33, szTemp[3]);
	SetDlgItemText(IDC_EDIT34, szTemp[4]);
	SetDlgItemText(IDC_EDIT35, szTemp[5]);
}

void CAnimationTool::OnEdit(void)
{
	if (nullptr == m_pDynamicFeature)
		return;

	CString cstrCurrentSpeed, cstrCurrentWeight, cstrCurrentDuration;
	CString cstrNewSpeed, cstrNewWeight, cstrNewDuration;

	GetDlgItemText(IDC_EDIT1, cstrCurrentSpeed);
	GetDlgItemText(IDC_EDIT2, cstrCurrentWeight);
	GetDlgItemText(IDC_EDIT32, cstrCurrentDuration);
	GetDlgItemText(IDC_EDIT33, cstrNewSpeed);
	GetDlgItemText(IDC_EDIT34, cstrNewWeight);
	GetDlgItemText(IDC_EDIT35, cstrNewDuration);

	_float fCurrentSpeed = (float)_wtof(cstrCurrentSpeed);
	_float fCurrentWeight = (float)_wtof(cstrCurrentWeight);
	_float fCurrentDuration = (float)_wtof(cstrCurrentDuration);
	_float fNewSpeed = (float)_wtof(cstrNewSpeed);
	_float fNewWeight = (float)_wtof(cstrNewWeight);
	_float fNewDuration = (float)_wtof(cstrNewDuration);

	m_pDynamicFeature->m_tCurrentAniCtrlInfo.fTrackSpeed = fCurrentSpeed;
	m_pDynamicFeature->m_tCurrentAniCtrlInfo.fWeight = fCurrentWeight;
	m_pDynamicFeature->m_tCurrentAniCtrlInfo.fDuration = fCurrentDuration;
	m_pDynamicFeature->m_tNewAniCtrlInfo.fTrackSpeed = fNewSpeed;
	m_pDynamicFeature->m_tNewAniCtrlInfo.fWeight = fNewWeight;
	m_pDynamicFeature->m_tNewAniCtrlInfo.fDuration = fNewDuration;
}


