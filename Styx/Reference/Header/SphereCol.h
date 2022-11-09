#ifndef SphereCol_h__
#define SphereCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSphereCol : public CVIBuffer
{
	struct INDEX
	{
		WORD _0, _1, _2;
	};


private:
	explicit CSphereCol(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSphereCol(void);

public:
	virtual HRESULT				Ready_Buffer(const _float fRadius,
											const _int iSlices,
											const _int iStacks);

private:
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];

private:
	LPD3DXMESH					m_pMesh = nullptr;


public:
	static CSphereCol*			Create(LPDIRECT3DDEVICE9 pGraphicDev,
											const _float fRadius,
											const _int iSlices,
											const _int iStacks);
public:
	virtual CResources*			Clone(void) { return nullptr; }
public:
	virtual void				Free(void);
};

END


#endif // SphereCol_h__
