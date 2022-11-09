#pragma once
#include "afxcmn.h"


// CMyForm 폼 뷰입니다.

#include "MeshTool.h"
#include "NaviMeshTool.h"
#include "AnimationTool.h"
#include "CameraTool.h"
#include "LightTool.h"

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	/*  Event  */
	virtual void			 OnInitialUpdate();
	afx_msg void			 OnTcnSelchangeTabTool(NMHDR *pNMHDR, LRESULT *pResult);

public:
	/*  Dialog Value  */
	CTabCtrl				m_ctrlToolTab;

public:
	CMeshTool				m_MeshTool;
	CNaviMeshTool			m_NaviMeshTool;
	CAnimationTool			m_AnimationTool;
	CCameraTool				m_CameraTool;
	CLightTool				m_LightTool;
};


