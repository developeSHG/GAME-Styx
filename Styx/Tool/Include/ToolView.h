
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

#include "Engine_Include.h"
#include "Include.h"	

class CToolDoc;
class CToolMgr;

BEGIN(Engine)

class CCalculator;
class CGameObject;
class CTransform;
class CStaticMesh;
class CTerrainTex;

END

class CToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	/*  Event  */
	virtual void				OnInitialUpdate();
	afx_msg void				OnLButtonDown(UINT nFlags, CPoint point);

public:
	HRESULT						MeshObjectCreate(void);
	HRESULT						NaviMeshCreate(void);
	HRESULT						CameraCreate(void);
	HRESULT						LightCreate(void);

public:
	HRESULT						Get_Pointer(void);

public:
	CToolMgr*					m_pToolMgr		= nullptr;
	Engine::CCalculator*		m_pCalculator	= nullptr;

public:
	Engine::CGameObject*		m_pCatsle				= nullptr;
	Engine::CStaticMesh*		m_pCatsleMeshCom		= nullptr;
	Engine::CTransform*			m_pCatsleTransCom		= nullptr;
	list<Engine::CGameObject*>*	m_pPrisonlist			= nullptr;
	Engine::CStaticMesh*		m_pPrisonMeshCom		= nullptr;
	Engine::CTransform*			m_pPrisonTransCom[4]	= {};
	Engine::CGameObject*		m_pTerrain				= nullptr;
	Engine::CTerrainTex*		m_pTerrainBufferCom		= nullptr;
	Engine::CTransform*			m_pTerrainTransCom		= nullptr;
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

