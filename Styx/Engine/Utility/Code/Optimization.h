#ifndef Optimization_h__
#define Optimization_h__

#include "Component.h"

#include "Frustum.h"
#include "QuadTree.h"

BEGIN(Engine)

class ENGINE_DLL COptimization : public CComponent
{
private:
	/*  Structor  */
	explicit COptimization(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit COptimization(const COptimization& rhs);
	virtual ~COptimization(void);

public:
	/*  General  */
	HRESULT						Ready_Optimization(void);

public:
	_bool						Is_InFrustum_ForObject(const _vec3* pPos, 
														const _float fRadius = 0.f,
														_uint* uDir = nullptr);
	void						Is_InFrustum_ForTerrain(const _vec3* pVtxPos,
														const _ulong& dwCntX,
														const _ulong& dwCntZ,
														Engine::INDEX32* pIndex,
														_ulong* pTriCnt,
														CQuadTree* pQuadTree);

	

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	_bool						m_bClone;
	CFrustum*					m_pFrustum;



	/*  Creation and destruction  */
public:
	static COptimization*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual CComponent*			Clone_Component(void);
public:
	virtual void				Free(void);

};

END
#endif // Optimization_h__
