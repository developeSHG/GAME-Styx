#include "ResourcesMgr.h"

IMPLEMENT_SINGLETON(Engine::CResourcesMgr)

Engine::CResourcesMgr::CResourcesMgr(void)
{
	
}

Engine::CResourcesMgr::~CResourcesMgr(void)
{
	Free();
}

HRESULT Engine::CResourcesMgr::Reserve_ContainerSize(const _ushort& _wSize)
{
	if (nullptr != m_pMapResources)
		return E_FAIL;

	m_pMapResources = new map<wstring, Engine::CResources*>[_wSize];

	m_wContainerSize = _wSize;

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 _pGraphicDev, 
												const _ushort& _wContainerIdx, 
												const _tchar* _pBufferTag, 
												Engine::BUFFERID _eID, 
												const _ulong& _dwCntX /*= 1*/, 
												const _ulong& _dwCntZ /*= 1*/, 
												const _ulong& _dwVtxItv /*= 1*/)
{
	if (nullptr == m_pMapResources)
	{
		MSG_BOX("Resources Container not Reserved");
		return E_FAIL;
	}

	Engine::CResources*		pResources = Find_Resources(_wContainerIdx, _pBufferTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (_eID)
	{
	case Engine::BUFFERID::BUFFER_TRICOL:
		pResources = CTriCol::Create(_pGraphicDev);
		break;

	case Engine::BUFFERID::BUFFER_RCCOL:
		pResources = CRcCol::Create(_pGraphicDev);
		break;

	case Engine::BUFFERID::BUFFER_RCTEX:
		pResources = CRcTex::Create(_pGraphicDev);
		break;

	case Engine::BUFFERID::BUFFER_CUBETEX:
		pResources = CCubeTex::Create(_pGraphicDev);
		break;

	case Engine::BUFFERID::BUFFER_TERRAINTEX:
		pResources = CTerrainTex::Create(_pGraphicDev, _dwCntX, _dwCntZ, _dwVtxItv);
		break;

	case Engine::BUFFERID::BUFFER_TERRAINRCTEX:
		pResources = CTerrainRcTex::Create(_pGraphicDev);
		break;
	}

	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[_wContainerIdx].emplace(_pBufferTag, pResources);

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Texture(LPDIRECT3DDEVICE9 _pGraphicDev, 
												const _ushort& _wContainerIdx, 
												const _tchar* _pTextureTag, 
												Engine::TEXTURETYPE _eID, 
												const _tchar* _pPath, 
												const _uint& _iCnt /*= 1*/)
{
	if (nullptr == m_pMapResources)
	{
		MSG_BOX("Resources Container not Reserved");
		return E_FAIL;
	}

	Engine::CResources*		pResources = Find_Resources(_wContainerIdx, _pTextureTag);

	if (nullptr != pResources)
		return E_FAIL;

	pResources = Engine::CTexture::Create(_pGraphicDev, _pPath, _eID, _iCnt);
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[_wContainerIdx].emplace(_pTextureTag, pResources);

	return S_OK;

}

HRESULT Engine::CResourcesMgr::Ready_Meshe(LPDIRECT3DDEVICE9 pGraphicDev,
											const _ushort& wContainerIdx,
											const _tchar* pMeshTag,
											MESHTYPE eType,
											const _tchar* pFilePath,
											const _tchar* pFileName)
{
	if (nullptr == m_pMapResources)
	{
		MSG_BOX("Resources Container not Reserved");
		return E_FAIL;
	}

	CResources*		pResources = Find_Resources(wContainerIdx, pMeshTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (eType)
	{
	case TYPE_STATIC:
		pResources = CStaticMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;

	case TYPE_DYNAMIC:
		pResources = CDynamicMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;

	case TYPE_NAVI:
		pResources = CNaviMesh::Create(pGraphicDev);
		break;
	}

	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[wContainerIdx].emplace(pMeshTag, pResources);

	return S_OK;
}

void Engine::CResourcesMgr::Render_Buffer(const _ushort& _wContainerIdx, 
											const _tchar* _pBufferTag)
{
	Engine::CResources*	pResources = Find_Resources(_wContainerIdx, _pBufferTag);

	if (nullptr == pResources)
		return;

	dynamic_cast<Engine::CVIBuffer*>(pResources)->Render_Buffer();
}

void Engine::CResourcesMgr::Render_Texture(const _ushort& _wContainerIdx, 
											const _tchar* _pTextureTag, 
											const _uint& _iIndex /*= 0*/)
{
	Engine::CResources* pResources = Find_Resources(_wContainerIdx, _pTextureTag);

	if (nullptr == pResources)
		return;

	dynamic_cast<Engine::CTexture*>(pResources)->Render_Texture();
}

Engine::CResources* Engine::CResourcesMgr::Find_Resources(const _ushort& _wContainerIdx, 
															const _tchar* _pResourceTag)
{
	//auto iter = find_if(m_pMapResources[_wContainerIdx].begin(), m_pMapResources[_wContainerIdx].end(), Engine::CTag_Finder(_pResourceTag));

	auto iter =m_pMapResources[_wContainerIdx].find(_pResourceTag);

	if (iter == m_pMapResources[_wContainerIdx].end())
		return nullptr;

	return iter->second;
}

Engine::CResources* Engine::CResourcesMgr::Clone(const _ushort& _wContainerIdx,
															const _tchar* _pResourceTag)
{
	if (nullptr == m_pMapResources)
		return nullptr;

	Engine::CResources*		pProtoType = Find_Resources(_wContainerIdx, _pResourceTag);

	return pProtoType->Clone();
}

void Engine::CResourcesMgr::Free(void)
{
	for (_ushort i = 0; i < m_wContainerSize; ++i)
	{
		for_each(m_pMapResources[i].begin(), m_pMapResources[i].end(), Engine::CDeleteMap());
#ifndef _TOOL
		//if (2 == i)
		//{
		//	int a = 0;
		//	for (auto& iter : m_pMapResources[i])
		//	{
		//		a++;
		//		if (a < 5)
		//			continue;
		//		if (nullptr != iter.first)
		//			delete[] iter.first;
		//	}
		//}
#endif // 
		m_pMapResources[i].clear();
	}

	Engine::Safe_Delete_Array(m_pMapResources);
}