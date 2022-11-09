#ifndef Cell_h__
#define Cell_h__

#include "Engine_Include.h"
#include "Base.h"

#include "Line.h"

BEGIN(Engine)

class ENGINE_DLL CCell : public CBase
{
public:
	enum POINT		{ POINT_A, POINT_B, POINT_C, POINT_END };
	enum NEIGHBOR	{ NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
	enum LINE		{ LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum COMPARE	{ COMPARE_MOVE, COMPARE_STOP };

private:
	/*  Structor  */
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCell(void);

public:
	HRESULT						Ready_Cell(const _ulong& dwIndex,
											const _vec3* pPointA,
											const _vec3* pPointB,
											const _vec3* pPointC);
	_bool						Compare_Point(const _vec3* pFirstPoint,
												const _vec3* pSecondPoint,
												CCell* pCell);
	COMPARE						Compare(const _vec3* pEndPos, 
											_ulong* pCellIndex);
	void						Render_Cell(void);

public:
	/*  Set  */
	void						Set_Neighbor(const NEIGHBOR eType, CCell* pNeighbor) { m_pNeighbor[eType] = pNeighbor; }

public:
	/*  Get  */
	const _vec3*				Get_Point(const POINT eType) const { return &m_vPoint[eType]; }
	CCell*						Get_Neighbor(const NEIGHBOR eType) const { return m_pNeighbor[eType]; }
	const _ulong*				Get_Index(void) const { return &m_dwIndex; }
	const Engine::NAVI_TYPE&	Get_NaviType(void) const { return m_eNaviType; }

public:
	CLine::COMPARE				Compare_Line(LINE eLine, const _vec3* pEndPos);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
	LPD3DXLINE					m_pD3DXLine = nullptr;

public:
	_vec3						m_vPoint[POINT_END];
	CCell*						m_pNeighbor[NEIGHBOR_END];
	CLine*						m_pLine[LINE_END];
	_ulong						m_dwIndex = 0;
	Engine::NAVI_TYPE			m_eNaviType;
	


	/*  Creation and destruction  */
public:
	static CCell*				Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _ulong& dwIndex,
										const _vec3* pPointA,
										const _vec3* pPointB,
										const _vec3* pPointC);
public:
	virtual void				Free(void);

};
END
#endif // Cell_h__
