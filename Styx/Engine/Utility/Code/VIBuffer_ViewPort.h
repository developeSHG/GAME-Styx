#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_ViewPort final : public CVIBuffer
{
private:
	explicit CVIBuffer_ViewPort(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_ViewPort(const CVIBuffer_ViewPort& rhs);
	virtual ~CVIBuffer_ViewPort() = default;
public:
	virtual HRESULT Ready_Buffer(_float fX, _float fY, _float fSizeX, _float fSizeY);
	virtual HRESULT Ready_Buffer_Clone(void* pArg);
	virtual void Render_Buffer(void);
public:
	static CVIBuffer_ViewPort* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fX, _float fY, _float fSizeX, _float fSizeY);
	virtual CResources*	Clone() { return nullptr; }
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

END