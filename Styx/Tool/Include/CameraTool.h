#pragma once
#include "afxcmn.h"


// CCameraTool 대화 상자입니다.

#include "Engine_Include.h"
#include "Include.h"	

#include "CameraHierarchy.h"
#include "CameraPoint.h"

class CCameraTool : public CDialog
{
	DECLARE_DYNAMIC(CCameraTool)

public:
	CCameraTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCameraTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERATOOL };
#endif

public:
	enum CAMERATREE { TREE_EYE, TREE_AT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	/*  Event  */
	virtual BOOL			 PreTranslateMessage(MSG* pMsg);
	virtual BOOL			 OnInitDialog();

	afx_msg void			 OnTvnSelchangedTreeEyePointList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			 OnTvnSelchangedTreeAtPointList(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void			 OnSpin_PositionX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			 OnSpin_PositionY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			 OnSpin_PositionZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			 OnSpin_SectionSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			 OnSpin_TotalSpeed(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void			 OnEdit_PositionX();
	afx_msg void			 OnEdit_PositionY();
	afx_msg void			 OnEdit_Positionz();
	afx_msg void			 OnEdit_SectionSpeed();
	afx_msg void			 OnEdit_TotalSpeed();

	afx_msg void			 OnEdit_Delete();
	afx_msg void			 OnEdit_AllDelete();
	afx_msg void			 OnSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			 OnEdit_Play();
	afx_msg void			 OnEdit_Pause();
	afx_msg void			 OnEdit_Stop();
	afx_msg void			 OnEdit_Save();
	afx_msg void			 OnEdit_Load();

public:
	void					 TreeUpdate();
	void					 OnSpin(LPNMUPDOWN _pNMUpdDown, DWORD _dwID);
	void					 OnEdit(void);
	void					 Load_EyeTreeUpdate();
	void					 Load_AttreeUpdate();


public:
	/*  Control Value  */
	CTreeCtrl				 m_ctrlEyePointListTree;
	CTreeCtrl				 m_ctrlAtPointListTree;
	CSliderCtrl				 m_ctrlActionSlider;
	
public:
	/*  User Value  */
	CToolMgr*				 m_pToolMgr = nullptr;
	CCameraHierarchy*		 m_pCameraHierarchy = nullptr;
	HTREEITEM				 m_hEyePointListRoot;
	HTREEITEM				 m_hAtPointListRoot;
	DWORD					 m_dwNum = 0;
	CAMERATREE				 m_eCameraTree;
	CString					 m_strSectionTag;
	CString					 m_strSpline;
};
