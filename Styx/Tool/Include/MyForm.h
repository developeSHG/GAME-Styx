#pragma once
#include "afxcmn.h"


// CMyForm �� ���Դϴ�.

#include "MeshTool.h"
#include "NaviMeshTool.h"
#include "AnimationTool.h"
#include "CameraTool.h"
#include "LightTool.h"

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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


