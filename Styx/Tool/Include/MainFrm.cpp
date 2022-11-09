
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"

#include "Include.h"
#include "ToolView.h"
#include "MyForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/************************ (SongSong) ************************/
	/*  Delete Tool menu window  */
	SetMenu(NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	/************************ (SongSong) ************************/
	/*  Tool Window Style Settings  */
	cs.style &= ~FWS_ADDTOTITLE;
	cs.lpszName = L"Styx Tool";
	cs.cx = WINCX + 400;  // �ڽ��� ���ϴ� ���� �������� x�� ũ��
	cs.cy = WINCY - 20;  // �ڽ��� ���ϴ� ���� �������� y�� ũ��
	//GetClientRect()

	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	m_MainSplitt.CreateStatic(this, 1, 2);			//this�� ������������
	int id = m_MainSplitt.IdFromRowCol(0, 0);

	//������ id�� �θ��� ��� �Ҽӵ��� ���ϴ°�.
	//���� ���ν����� ũ������Ʈ���� WS_CHILD | WS_VISIBLE�� ����Ʈ�� ����.
	//�ٸ� �̴Ͻ����� ���� �� id���� ����ϹǷ� �Ű������� ����ϴ� ��.
	//m_MiniSplitt.CreateStatic(&m_MainSplitt, 2, 1, WS_CHILD | WS_VISIBLE, id);		//�̴Ͻ������� �θ�� ���ν������̴�.

	//���ҵ� ������ view�� ��ġ (2�� ��ġ)
	//�� Ŭ�������� �����ڰ� protected�� �Ǿ� �����Ƿ� ������� �����Ҵ� �Ұ����ϴ�.
	//-> RUNTIME_CLASS ��ũ�θ� ���.

	m_MainSplitt.SetColumnInfo(0, WINCX, 0);		//���� �ʺ� ���� (3��° ���ڴ� �ּ� ũ��)
	m_MainSplitt.SetRowInfo(0, WINCY - 20, 0);
	//m_MainSplitt.SetColumnInfo(1, TOOLCX, TOOLCX);		//���� �ʺ� ���� (3��° ���ڴ� �ּ� ũ��)

	m_MainSplitt.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(400, WINCY - 20), pContext);
	m_MainSplitt.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY - 20), pContext);

	srand((unsigned)time(NULL));

	return TRUE;		/*CFrameWnd::OnCreateClient(lpcs, pContext);*/  // ���⼭ �並 �����ϹǷ� �츮�� �����ؾ��Ѵ�.

}
