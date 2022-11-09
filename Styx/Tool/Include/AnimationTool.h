#pragma once


// CAnimationTool 대화 상자입니다.

#include "Engine_Include.h"
#include "Include.h"	
#include "afxcmn.h"
#include "afxwin.h"

class CAnimationTool : public CDialog
{
	DECLARE_DYNAMIC(CAnimationTool)

public:
	CAnimationTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	/*  Event  */
	virtual BOOL			PreTranslateMessage(MSG* pMsg);
	virtual BOOL			OnInitDialog();

	afx_msg void			OnTvnSelchangedTreeDynamicMeshList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnLbnSelchangeCurrentAniList();
	afx_msg void			OnLbnSelchangeNewAniList();

	afx_msg void			OnSpin_CurrentWeight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_CurrentSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_CurrentDuration(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_New(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_NewSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_NewDuration(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnEdit_CurrentWeight();
	afx_msg void			OnEdit_CurrentSpeed();
	afx_msg void			OnEdit_CurrentDuration();
	afx_msg void			OnEdit_NewWeight();
	afx_msg void			OnEdit_NewSpeed();
	afx_msg void			OnEdit_NewDuration();

public:
	void					OnSpin(LPNMUPDOWN _pNMUpdDown, DWORD _dwID);
	void				    OnEdit(void);

public:
	HRESULT					Ready_DynamicMeshList(CString strPathTag);

public:
	/*  Control Value  */
	CTreeCtrl				m_ctrlDynamicMeshListTree;
	CListBox				m_ctrlCurrentAniList;
	CListBox				m_ctrlNewAniList;

public:
	/*  User Value  */
	CToolMgr*				m_pToolMgr = nullptr;
	HTREEITEM				m_hDynamicObjectRoot;
	CDynamicFeature*		m_pDynamicFeature = nullptr;

};
