#pragma once

#include "Base.h"

#include "Engine_Include.h"

BEGIN(Engine)

class CVIBuffer_ViewPort;
class CRenderTarget final : public CBase
{
private:
	explicit CRenderTarget(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderTarget() = default;
public:
	LPDIRECT3DTEXTURE9 Get_TargetTexture() const {
		return m_pTexture; }
public:
	HRESULT Ready_RenderTarget(_uint iWidth, _uint iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor);
	HRESULT SetUp_OnGraphicDev(_uint iIndex);
	HRESULT Release_OnGraphicDev(_uint iIndex);
	HRESULT Clear_RenderTarget();
//#ifdef _DEBUG
public:
	HRESULT Ready_Debug_Buffer(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer();
//#endif // _DEBUG
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	LPDIRECT3DTEXTURE9			m_pTexture = nullptr;
	LPDIRECT3DSURFACE9			m_pSurface = nullptr;
	LPDIRECT3DSURFACE9			m_pOldSurface = nullptr;
	D3DXCOLOR					m_ClearColor;
//#ifdef _DEBUG
private:
	CVIBuffer_ViewPort*			m_pVIBuffer = nullptr;
//#endif // _DEBUG
public:
	static CRenderTarget* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWidth, _uint iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor);
	virtual void Free();
};

END