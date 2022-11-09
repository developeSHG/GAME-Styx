#pragma once

#include "Engine_Include.h"
#include "Include.h"	
#include "afxcmn.h"
#include "afxwin.h"

// CNaviMeshTool 대화 상자입니다.

class CNaviMeshTool : public CDialog
{
	DECLARE_DYNAMIC(CNaviMeshTool)

public:
	CNaviMeshTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNaviMeshTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVIMESHTOOL };
#endif

public:
	enum PICKING_TYPE { INSERT_POINT, SELECT_POINT, SELECT_CELL, PICKING_TYPE_END };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	/*  Event  */
	virtual BOOL			PreTranslateMessage(MSG* pMsg);
	virtual BOOL			OnInitDialog();

	afx_msg void			OnTvnSelchangedTreeNaviMeshList(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void			OnSpin_PositionX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_PositionY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_PositionZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnEdit_PositionX();
	afx_msg void			OnEdit_PositionY();
	afx_msg void			OnEdit_PositionZ();

	afx_msg void			OnEdit_Delete();
	afx_msg void			OnEdit_Save();
	afx_msg void			OnEdit_Load();

public:
	void					OnSpin(LPNMUPDOWN _pNMUpdDown, DWORD _dwID);
	void					OnEdit(void);
	void					PointCheck(const _vec3& _vPickPos);
	void					Update_PickingType();
	void					Update_NaviType();
	_bool					Picking_Point();

public:
	/*  Control Value  */
	CTreeCtrl				m_ctrlNaviMeshListTree;
	CButton					m_ctrlPickingType[PICKING_TYPE::PICKING_TYPE_END];
	CButton					m_ctrlNaviType[Engine::NAVI_TYPE::NAVI_TYPE_END];
	
public:
	/*  User Value  */
	CToolMgr*				m_pToolMgr = nullptr;
	HTREEITEM				m_hNaviMeshListRoot;
	PICKING_TYPE			m_ePickingType = PICKING_TYPE::INSERT_POINT;
	Engine::NAVI_TYPE		m_eNaviType = Engine::NAVI_TYPE::NAVI_ROAD;
	_int					m_iPointNumCheck = 0;
	_bool					m_bFindDelete = FALSE;
	_vec3					m_vPoint[POINT_CHECK::POINT_END];
	_ushort					m_uPointTri = -1;
	_ushort					m_uPointIndex = -1;
};
