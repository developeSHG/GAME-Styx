#pragma once
#include "afxwin.h"


// CLightTool 대화 상자입니다.

#include "Engine_Include.h"
#include "Include.h"	

class CLightTool : public CDialog
{
	DECLARE_DYNAMIC(CLightTool)

public:
	CLightTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLightTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIGHTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	/*  Event  */
	virtual BOOL			PreTranslateMessage(MSG* pMsg);
	virtual BOOL			OnInitDialog();

	afx_msg void OnList_Light();

	afx_msg void OnSpin_PositionX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_PositionY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_PositionZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_DiffuseR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_DiffuseG(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_DiffuseB(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_DiffuseA(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_AmbientR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_AmbientG(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_AmbientB(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_AmbientA(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_SpecularR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_SpecularG(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_SpecularB(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_SpecularA(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Range(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEdit_PositionX();
	afx_msg void OnEdit_PositionY();
	afx_msg void OnEdit_PositionZ();
	afx_msg void OnEdit_DiffuseR();
	afx_msg void OnEdit_DiffuseG();
	afx_msg void OnEdit_DiffuseB();
	afx_msg void OnEdit_DiffuseA();
	afx_msg void OnEdit_AmbientR();
	afx_msg void OnEdit_AmbientG();
	afx_msg void OnEdit_AmbientB();
	afx_msg void OnEdit_AmbientA();
	afx_msg void OnEdit_SpecularR();
	afx_msg void OnEdit_SpecularG();
	afx_msg void OnEdit_SpecularB();
	afx_msg void OnEdit_SpecularA();
	afx_msg void OnEdit_Range();

	afx_msg void OnButton_Delete();
	afx_msg void OnButton_Save();
	afx_msg void OnButton_Load();

public:
	/*  Control Value  */
	CListBox m_ctrlLightList;
	
public:
	/*  User Value  */
	CToolMgr*				m_pToolMgr = nullptr;
	vector<LIGHT*>			m_vecLight;
};
