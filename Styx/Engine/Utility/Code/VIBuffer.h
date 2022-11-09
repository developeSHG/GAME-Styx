#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Engine_Include.h"
#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CResources
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer(void);

public:
	virtual HRESULT				Ready_Buffer(void);
	virtual HRESULT				Ready_Buffer_Clone(void* pArg) { return S_OK; }
	virtual void				Render_Buffer(void);

public:
	/*  Get  */
	_ulong		Get_TriCnt(void) { return m_dwTriCnt; }

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

protected:
	DWORD						m_dwVtxSize;
	DWORD						m_dwVtxCnt;
	DWORD						m_dwVtxFVF;
	DWORD						m_dwTriCnt;
protected:
	DWORD						m_dwIdxSize;
	D3DFORMAT					m_IdxFmt;


public:
	virtual CResources*			Clone() PURE;
	    virtual CComponent*			Clone(void* pArg) { return nullptr; };
protected:
	virtual void				Free(void);
};

END

#endif // VIBuffer_h__
