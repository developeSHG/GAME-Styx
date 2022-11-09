#ifndef CubeCol_h__
#define CubeCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeCol : public CVIBuffer
{
private:
	explicit CCubeCol(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCubeCol(void);

public:
	virtual HRESULT				Ready_Buffer(const _vec3& vMin, const _vec3& vMax);

private:
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];

private:
	_vec3						m_vMin, m_vMax;


public:
	static CCubeCol*			Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3& vMin, 
										const _vec3& vMax);
public:
	virtual CResources*			Clone(void) { return nullptr; }
public:
	virtual void				Free(void);
};

END


#endif // CubeCol_h__
