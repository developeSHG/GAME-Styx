#ifndef Calculator_h__
#define Calculator_h__

#include "Component.h"

#include "Transform.h"
#include "TerrainTex.h"
#include "StaticMesh.h"

BEGIN(Engine)

class ENGINE_DLL CCalculator : public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator(void);

public:
	HRESULT						Ready_Calculator(void);
	virtual _int				Update_Component(const _float& fTimeDelta = 0.f) { return UPDATE_OK; }
	virtual _int				LateUpdate_Component(const _float& fTimeDelta = 0.f) { return UPDATE_OK; }

public:
	_float						Compute_HeightTerrain(const _vec3* pPos,
														const _vec3* pTerrainVtxPos,
														const _ulong& dwCntX,
														const _ulong& dwCntZ);
	_bool						Pick_OnTerrain(HWND hWnd,
												_vec3* pOutPos,
												const CTerrainTex* pTerrainBufferCom,
												const CTransform* pTerrainTransform);
	_bool						Pick_OnMesh(HWND hWnd,
											_vec3* pOutPos,
											CStaticMesh* pMeshBufferCom,
											CTransform* pTransformCom);


private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;



	/*  Creation and destruction  */
public:
	static CCalculator*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual CComponent*			Clone_Component(void);
private:
	virtual void				Free(void);

};

END
#endif // Calculator_h__
