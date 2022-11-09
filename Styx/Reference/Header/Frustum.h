#ifndef Frustum_h__
#define Frustum_h__

#include "Engine_Include.h"
#include "Base.h"

#include "QuadTree.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum : public CBase
{
private:
	/*  Structor  */
	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFrustum(void);

public:
	HRESULT						Ready_Frustum(void);
	_bool						Is_InFrustum(const _vec3* pPos, 
											const _float fRadius = 0.f,
											_uint* uDir = nullptr);
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
	_vec3						m_vPoint[8];
	D3DXPLANE					m_Plane[6];


	/*  Creation and destruction  */
public:
	static CFrustum*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void				Free(void);
};

END
#endif // Frustum_h__
