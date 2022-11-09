#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Mesh.h"

#include "Cell.h"

BEGIN(Engine)

class ENGINE_DLL CNaviMesh : public CMesh
{
private:
	typedef struct tagNaviCell
	{
		Engine::NAVI_TYPE   eNaviType;
		_vec3				vPointA;
		_vec3				vPointB;
		_vec3				vPointC;
	}NAVICELL;

protected:
	/*  Structor  */
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public:
	HRESULT					Ready_NaviMesh(void);
	void					Render_NaviMesh(void);
//	_vec3					MoveOn_NaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);
	_bool					MoveOn_NaviMeshPlayer(_vec3 * pTargetPos, _vec3* pNewPos, _bool& _bCompare, const DWORD& _dwJumpDownFlag, _bool& _bRoadCliff);
	_bool					MoveOn_NaviMeshElite(_vec3 * pTargetPos, _vec3* pNewPos, _bool& _bCompare);
	_bool					MoveOn_NaviMeshCam(const _vec3 * pTargetPos, _vec3* pDir);
	_bool					MoveOn_NaviMeshCam2(_vec3 * pTargetPos, _vec3* pNewPos);

private:
	HRESULT					Link_Cell(void);
	CLine::COMPARE			Compare_Line(_vec3 vecStartPos, _vec3 vecEndPos, _vec3 vecPlayerPos);

public:
	/*  Set  */
	void					Set_Index(const _ulong& dwIndex) { m_dwIndex = dwIndex; }

public:
	/*  Get  */

public:
	/*  Tool  */
	HRESULT					Create_Cell(_vec3* _vPos);

public:
	_bool					Update_CurIndex(const _vec3& _vEndPos,
											const _vec3& _vPlusPos = _vec3(0.f, 0.f, 0.f));

public:
	vector<CCell*>			m_vecCell;
	vector<CCell*>			m_vecInsideCell;
	vector<CCell*>			m_vecCliffCell;
	_ulong					m_dwIndex = 0;
	CCell*					m_PrevCell = nullptr;
	CCell*					m_CurCell = nullptr;


	/*  Creation and destruction  */
public:
	static CNaviMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual CResources*		Clone(void);
public:
	virtual void			Free(void);
};

END

#endif // NaviMesh_h__
