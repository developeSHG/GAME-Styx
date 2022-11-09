// LightTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "LightTool.h"
#include "afxdialogex.h"

// CLightTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLightTool, CDialog)

CLightTool::CLightTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_LIGHTTOOL, pParent)
{

}

CLightTool::~CLightTool()
{
}

void CLightTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ctrlLightList);
}


BEGIN_MESSAGE_MAP(CLightTool, CDialog)
	ON_EN_CHANGE(IDC_EDIT7, &CLightTool::OnEdit_PositionX)
	ON_EN_CHANGE(IDC_EDIT8, &CLightTool::OnEdit_PositionY)
	ON_EN_CHANGE(IDC_EDIT9, &CLightTool::OnEdit_PositionZ)
	ON_LBN_SELCHANGE(IDC_LIST2, &CLightTool::OnList_Light)
	ON_BN_CLICKED(IDC_BUTTON9, &CLightTool::OnButton_Delete)
	ON_BN_CLICKED(IDC_BUTTON1, &CLightTool::OnButton_Save)
	ON_BN_CLICKED(IDC_BUTTON2, &CLightTool::OnButton_Load)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CLightTool::OnSpin_PositionX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CLightTool::OnSpin_PositionY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CLightTool::OnSpin_PositionZ)
	ON_EN_CHANGE(IDC_EDIT18, &CLightTool::OnEdit_DiffuseR)
	ON_EN_CHANGE(IDC_EDIT19, &CLightTool::OnEdit_DiffuseG)
	ON_EN_CHANGE(IDC_EDIT20, &CLightTool::OnEdit_DiffuseB)
	ON_EN_CHANGE(IDC_EDIT21, &CLightTool::OnEdit_DiffuseA)
	ON_EN_CHANGE(IDC_EDIT22, &CLightTool::OnEdit_AmbientR)
	ON_EN_CHANGE(IDC_EDIT23, &CLightTool::OnEdit_AmbientG)
	ON_EN_CHANGE(IDC_EDIT24, &CLightTool::OnEdit_AmbientB)
	ON_EN_CHANGE(IDC_EDIT25, &CLightTool::OnEdit_AmbientA)
	ON_EN_CHANGE(IDC_EDIT26, &CLightTool::OnEdit_SpecularR)
	ON_EN_CHANGE(IDC_EDIT27, &CLightTool::OnEdit_SpecularG)
	ON_EN_CHANGE(IDC_EDIT28, &CLightTool::OnEdit_SpecularB)
	ON_EN_CHANGE(IDC_EDIT29, &CLightTool::OnEdit_SpecularA)
	ON_EN_CHANGE(IDC_EDIT30, &CLightTool::OnEdit_Range)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN19, &CLightTool::OnSpin_DiffuseR)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN20, &CLightTool::OnSpin_DiffuseG)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN21, &CLightTool::OnSpin_DiffuseB)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN22, &CLightTool::OnSpin_DiffuseA)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN23, &CLightTool::OnSpin_AmbientR)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN24, &CLightTool::OnSpin_AmbientG)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN25, &CLightTool::OnSpin_AmbientB)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN26, &CLightTool::OnSpin_AmbientA)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN27, &CLightTool::OnSpin_SpecularR)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN28, &CLightTool::OnSpin_SpecularG)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN29, &CLightTool::OnSpin_SpecularB)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN30, &CLightTool::OnSpin_SpecularA)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN31, &CLightTool::OnSpin_Range)
END_MESSAGE_MAP()


// CLightTool 메시지 처리기입니다.

BOOL CLightTool::PreTranslateMessage(MSG* pMsg)
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

BOOL CLightTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_pToolMgr = CToolMgr::GetInstance();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLightTool::OnList_Light()
{

}

void CLightTool::OnSpin_PositionX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

}

void CLightTool::OnSpin_PositionY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
		
}

void CLightTool::OnSpin_PositionZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

}

void CLightTool::OnSpin_DiffuseR(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnSpin_DiffuseG(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnSpin_DiffuseB(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnSpin_DiffuseA(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnSpin_AmbientR(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnSpin_AmbientG(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnSpin_AmbientB(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnSpin_AmbientA(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnSpin_SpecularR(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnSpin_SpecularG(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnSpin_SpecularB(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnSpin_SpecularA(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnSpin_Range(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLightTool::OnEdit_PositionX()
{

}

void CLightTool::OnEdit_PositionY()
{
	
}

void CLightTool::OnEdit_PositionZ()
{

}

void CLightTool::OnEdit_DiffuseR()
{

}


void CLightTool::OnEdit_DiffuseG()
{

}


void CLightTool::OnEdit_DiffuseB()
{

}


void CLightTool::OnEdit_DiffuseA()
{

}


void CLightTool::OnEdit_AmbientR()
{

}


void CLightTool::OnEdit_AmbientG()
{

}


void CLightTool::OnEdit_AmbientB()
{

}


void CLightTool::OnEdit_AmbientA()
{

}


void CLightTool::OnEdit_SpecularR()
{

}


void CLightTool::OnEdit_SpecularG()
{

}


void CLightTool::OnEdit_SpecularB()
{

}


void CLightTool::OnEdit_SpecularA()
{

}


void CLightTool::OnEdit_Range()
{

}

void CLightTool::OnButton_Delete()
{
	
}

void CLightTool::OnButton_Save()
{

}

void CLightTool::OnButton_Load()
{

}