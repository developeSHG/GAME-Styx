#ifndef RcTex_h__
#define RcTex_h__

#include "Engine_Include.h"

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex(void);

public:
	virtual HRESULT			Ready_Buffer(void);


public:
	static CRcTex*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual CResources*		Clone(void);
public:
	virtual void			Free(void);
};

END

#endif // RcTex_h__
