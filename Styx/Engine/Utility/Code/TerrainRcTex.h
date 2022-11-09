#ifndef TerrainRcTex_h__
#define TerrainRcTex_h__

#include "Engine_Include.h"

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainRcTex : public CVIBuffer
{
private:
	explicit CTerrainRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainRcTex(const CTerrainRcTex& rhs);
	virtual ~CTerrainRcTex(void);

public:
	virtual HRESULT					Ready_Buffer(void);


public:
	static CTerrainRcTex*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual CResources*				Clone(void);
public:
	virtual void					Free(void);
};

END

#endif // TerrainRcTex_h__
