#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Include.h"
#include "Base.h"

#include "VIBuffer.h"
#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "CubeTex.h"
#include "TerrainTex.h"
#include "TerrainRcTex.h"

#include "Texture.h"

#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"

BEGIN(Engine)

class CResources;

class ENGINE_DLL CResourcesMgr : public CBase
{
	DECLARE_SINGLETON(CResourcesMgr)

private:
	explicit CResourcesMgr(void);
	virtual ~CResourcesMgr(void);

public:
	HRESULT				Reserve_ContainerSize(const _ushort& _wSize);

public:
	HRESULT				Ready_Buffer(LPDIRECT3DDEVICE9 _pGraphicDev,
										const _ushort& _wContainerIdx,
										const _tchar* _pBufferTag,
										Engine::BUFFERID _eID,
										const _ulong& _dwCntX = 1,
										const _ulong& _dwCntZ = 1,
										const _ulong& _dwVtxItv = 1);
	HRESULT				Ready_Texture(LPDIRECT3DDEVICE9 _pGraphicDev,
										const _ushort& _wContainerIdx,
										const _tchar* _pTextureTag,
										Engine::TEXTURETYPE _eID,
										const _tchar* _pPath,
										const _uint& _iCnt = 1);

	HRESULT				Ready_Meshe(LPDIRECT3DDEVICE9 pGraphicDev,
										const _ushort& wContainerIdx,
										const _tchar* pMeshTag,
										MESHTYPE eType,
										const _tchar* pFilePath,
										const _tchar* pFileName);

public:
	void				Render_Buffer(const _ushort& _wContainerIdx,
										const _tchar* _pBufferTag);
	void				Render_Texture(const _ushort& _wContainerIdx,
										const _tchar* _pTextureTag,
										const _uint& _iIndex = 0);

private:
	CResources*					Find_Resources(const _ushort& _wContainerIdx,
												const _tchar* _pResourceTag);

private:
	map<wstring, CResources*>*		m_pMapResources = nullptr;
	_ushort									m_wContainerSize = 0;


public:
	CResources*					Clone(const _ushort& _wContainerIdx,
												const _tchar* _pResourceTag);
private:
	virtual void				Free(void);
};

END

#endif // ResourcesMgr_h__
