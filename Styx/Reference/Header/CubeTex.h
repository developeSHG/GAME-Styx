#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex : public CVIBuffer
{
private:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex(void);

public:
	virtual HRESULT		Ready_Buffer(void);

public:
	static CCubeTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual CResources*		Clone(void);
public:
	virtual void			Free(void);
};

END


#endif // CubeTex_h__
