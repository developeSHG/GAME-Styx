#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex(void);

public:
	virtual HRESULT		Ready_Buffer(void);
	virtual HRESULT		Ready_Buffer(const _ulong& wCntX,
									const _ulong& wCntZ,
									const _ulong& wItv);

public:
	/*  Set  */
	void				Set_VtxPosWorld(const _matrix* _matWolrd);

public:
	/*  Get  */
	const _vec3*		Get_VtxPos(void) const { return m_pPos; }
	const _vec3*		Get_VtxPosWorld(void) const { return m_pPosWorld; }
	_ulong				Get_VtxCntX(void) const { return m_dwCntX; }
	_ulong				Get_VtxCntZ(void) const { return m_dwCntZ; }
	//_ulong				Get_VtxCntX(void) const { return m_iH.biWidth; }
	//_ulong				Get_VtxCntZ(void) const { return m_iH.biHeight; }

public:
	DWORD*				LoadImages(void);
	void				Copy_Index(Engine::INDEX32* pIndex, const _ulong& dwTriCnt);

private:
	HANDLE				m_hFile;
	DWORD				m_dwCntX;
	DWORD				m_dwCntZ;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

private:
	_vec3*				m_pPos;
	_vec3*				m_pPosWorld;
	_bool				m_bClone;

public:
	static CTerrainTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const _ulong& wCntX, 
								const _ulong& wCntZ, 
								const _ulong& wItv);
public:
	virtual CResources*		Clone(void);
public:
	virtual void			Free(void);
};

END
#endif // TerrainTex_h__
