#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "Engine_Include.h"
#include "Include.h"	

// CMeshTool 대화 상자입니다.

class CMeshTool : public CDialog
{
	DECLARE_DYNAMIC(CMeshTool)
public:
	CMeshTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMeshTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATICMESHTOOL };
#endif

public:
	enum MESH_INFO { TRANS_SCALE, TRANS_ROTATION, TRANS_POSITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	/*  Event  */
	virtual BOOL			PreTranslateMessage(MSG* pMsg);
	virtual BOOL			OnInitDialog();

	afx_msg void			OnTvnSelchangedTreeStaticMeshList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnTvnSelchangedTreeDynamicMeshList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnTvnSelchangedTreeStaticObjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnTvnSelchangedTreeDynamicObjectList(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void			OnBnClickedRadioCatsleIn();
	afx_msg void			OnBnClickedRadioCatsleOut();

	afx_msg void			OnSpin_TransScaleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_TransScaleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_TransScaleZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_TransRotationX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_TransRotationY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_TransRotationZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_TransPositionX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_TransPositionY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnSpin_TransPositionZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnEdit_TransScaleX();
	afx_msg void			OnEdit_TransScaleY();
	afx_msg void			OnEdit_TransScaleZ();
	afx_msg void			OnEdit_TransRotationX();
	afx_msg void			OnEdit_TransRotationY();
	afx_msg void			OnEdit_TransRotationZ();
	afx_msg void			OnEdit_TransPositionX();
	afx_msg void			OnEdit_TransPositionY();
	afx_msg void			OnEdit_TransPositionZ();

	afx_msg void			OnButton_Delete();
	afx_msg void			OnButton_Save();
	afx_msg void			OnButton_Load();



public:
	HRESULT					Add_ObjectList(Engine::MESHTYPE _eMeshType);

public:
	HRESULT					Ready_StaticMeshList(CString strPathTag);
	HRESULT					Ready_DynamicMeshList(CString strPathTag);

public:
	void					OnSpin(LPNMUPDOWN _pNMUpdDown, _float* _fValue, DWORD _dwID, MESH_INFO _eMeshInfo);
	void					OnEdit(void);


public:
	/*  Control Value  */
	CTreeCtrl				m_ctrlStaticMeshListTree;
	CTreeCtrl				m_ctrlDynamicMeshListTree;
	CTreeCtrl				m_ctrlStaticObjectListTree;
	CTreeCtrl				m_ctrlDynamicObjectListTree;
	CButton					m_ctrlFillModeRadio[FILLMODE::FILL_END];
	CButton					m_ctrlCatsleIn;
	CButton					m_ctrlCatsleOut;

public:
	/*  User Value  */
	CToolMgr*				m_pToolMgr = nullptr;
	HTREEITEM				m_hMeshRoot;
	HTREEITEM				m_hStaticObjectRoot;
	HTREEITEM				m_hDynamicObjectRoot;
	DWORD					m_dwNum;
	_bool					m_bXFileFind = FALSE;
	_bool					m_bMeshTree[MESHTREE::MESHTREE_END];
	CString					m_strMeshTag;
	_bool					m_bCatsleIn = TRUE;

public:
	_vec3					m_vTransScale;
	_vec3					m_vTransRotation;
	_vec3					m_vTransPosition;

};
