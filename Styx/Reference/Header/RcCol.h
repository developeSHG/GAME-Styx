#ifndef RcCol_h__
#define RcCol_h__

#include "Engine_Include.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCol : public CVIBuffer
{
protected:
	explicit CRcCol(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CRcCol(const CRcCol& rhs);
	virtual ~CRcCol(void);

public:
	virtual HRESULT				Ready_Buffer(void);



public:
	static CRcCol*				Create(LPDIRECT3DDEVICE9 _pGraphicDev);
public:
	virtual CResources*			Clone(void);
protected:
	virtual void				Free(void);
};

END

#endif // RcCol_h__
