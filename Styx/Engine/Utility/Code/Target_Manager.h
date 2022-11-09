#pragma once

#include "Base.h"

#include "Engine_Include.h"

BEGIN(Engine)

class CRenderTarget;

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	explicit CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	LPDIRECT3DTEXTURE9 Get_TargetTexture(const _tchar* pTargetTag);
public:
	HRESULT Add_RenderTarget(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTargetTag, _uint iWidth, _uint iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
public:
	HRESULT Begin_MRT(const _tchar* pMRTTag);
	HRESULT End_MRT(const _tchar* pMRTTag);

//#ifdef _DEBUG
public:
	HRESULT Ready_Debug_Buffer(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer(const _tchar* pMRTTag);
//#endif // _DEBUG

private:
	unordered_map<const _tchar*, CRenderTarget*>			m_RenderTargets;
	typedef unordered_map<const _tchar*, CRenderTarget*>	RENDERTARGETS;
private:
	unordered_map<const _tchar*, list<CRenderTarget*>>			m_MRT;
	typedef unordered_map<const _tchar*, list<CRenderTarget*>>	MRT;


private:
	CRenderTarget* Find_Target(const _tchar* pTargetTag);
	list<CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);
public:
	virtual void Free();	
};

END