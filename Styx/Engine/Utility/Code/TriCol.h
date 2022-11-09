#ifndef TriCol_h__
#define TriCol_h__

#include "Engine_Include.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol : public CVIBuffer
{
protected:
	explicit CTriCol(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CTriCol(const CTriCol& rhs);
	virtual ~CTriCol(void);

public:
	virtual HRESULT				Ready_Buffer(void);


public:
	static CTriCol*				Create(LPDIRECT3DDEVICE9 _pGraphicDev);
public:
	virtual CResources*			Clone(void);
protected:
	virtual void				Free(void);
};

END

#endif // TriCol_h__
