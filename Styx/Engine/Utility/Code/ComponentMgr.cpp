#include "ComponentMgr.h"

IMPLEMENT_SINGLETON(Engine::CComponentMgr)

Engine::CComponentMgr::CComponentMgr()
{

}

Engine::CComponentMgr::~CComponentMgr(void)
{
	Free();
}

HRESULT Engine::CComponentMgr::Add_Component(const _ushort& _wContainerIdx, const _tchar* _pComponentTag, Engine::COMPONENTTYPE _eType)
{
	Engine::CComponent*		pComponent = nullptr;

	auto iter = find_if(m_mapComponent[_wContainerIdx].begin(), m_mapComponent[_wContainerIdx].end(), Engine::CTag_Finder(_pComponentTag));

	if (iter != m_mapComponent[_wContainerIdx].end())
		return E_FAIL;

	switch (_eType)
	{
	case Engine::COMPONENTTYPE::COM_TRANSFORM:
		pComponent = Engine::CTransform::Create(_vec3(0.f, 0.f, 1.f));
		break;

	case Engine::COMPONENTTYPE::COM_CALCULATOR:
		pComponent = Engine::CCalculator::Create(m_pGraphicDev);
		break;

	case Engine::COMPONENTTYPE::COM_OPTIMIZATION:
		pComponent = Engine::COptimization::Create(m_pGraphicDev);
		break;

	case Engine::COMPONENTTYPE::COM_SHADER_MESH:
		pComponent = Engine::CShader::Create(m_pGraphicDev, L"../../Reference/Header/Shader_Mesh.hpp");
		break;

	case Engine::COMPONENTTYPE::COM_SHADER_TERRAIN:
		pComponent = Engine::CShader::Create(m_pGraphicDev, L"../../Reference/Header/Shader_Terrain.hpp");
		break;

	case Engine::COMPONENTTYPE::COM_SHADER_ALPHA:
		pComponent = Engine::CShader::Create(m_pGraphicDev, L"../../Reference/Header/Shader_Alpha.hpp");
		break;

	case Engine::COMPONENTTYPE::COM_NEWSHADER_DEFAULT:
		pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Bin/ShaderFiles/Shader_Default.fx");
		break;

	case Engine::COMPONENTTYPE::COM_NEWSHADER_MESH:
		pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Bin/ShaderFiles/Shader_Mesh.fx");
		break;

	case Engine::COMPONENTTYPE::COM_NEWSHADER_SKY:
		pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Bin/ShaderFiles/Shader_Sky.fx");
		break;

	case Engine::COMPONENTTYPE::COM_NEWSHADER_TERRAIN:
		pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Bin/ShaderFiles/Shader_Terrain.fx");
		break;
	}

	NULL_CHECK_RETURN(pComponent, E_FAIL);

	m_mapComponent[_wContainerIdx].emplace(_pComponentTag, pComponent);

	return S_OK;
}

Engine::CComponent* Engine::CComponentMgr::Find_Component(const _ushort& _wContainerIdx, const _tchar* _pComponentTag)
{
	auto iter = find_if(m_mapComponent[_wContainerIdx].begin(), m_mapComponent[_wContainerIdx].end(), Engine::CTag_Finder(_pComponentTag));

	if (iter == m_mapComponent[_wContainerIdx].end())
		return nullptr;

	return iter->second;
}

HRESULT Engine::CComponentMgr::Set_ObjectComponent(const _tchar* _pObjTag, const _tchar* _pComponentTag, CComponent* _pComponent)
{
	if (nullptr == _pComponent)
		return E_FAIL;

	auto iter = find_if(m_mapObjectmapComponent.begin(), m_mapObjectmapComponent.end(), Engine::CTag_Finder(_pObjTag));

	/*  If ObjTag is nullptr, create temp value  */
	if (iter == m_mapObjectmapComponent.end())
	{
		m_mapObjectmapComponent.emplace(_pObjTag, MAPCOMPONENT());
		m_mapObjectmapComponent[_pObjTag].emplace(_pComponentTag, _pComponent);
		return S_OK;
	}

	auto Componet_iter = find_if(iter->second.begin(), iter->second.end(), Engine::CTag_Finder(_pComponentTag));

	/*  If ComponentTag is not nullptr  */
	if (Componet_iter != iter->second.end())
		return E_FAIL;

	m_mapObjectmapComponent[_pObjTag].emplace(_pComponentTag, _pComponent);

	return S_OK;
}

Engine::CComponent* Engine::CComponentMgr::Get_ObjectComponent(const _tchar* _pObjTag, const _tchar* _pComponentTag)
{
	auto iter = find_if(m_mapObjectmapComponent.begin(), m_mapObjectmapComponent.end(), Engine::CTag_Finder(_pObjTag));

	if (iter == m_mapObjectmapComponent.end())
		return nullptr;

	auto mapComponet = find_if(iter->second.begin(), iter->second.end(), Engine::CTag_Finder(_pComponentTag));

	if (mapComponet == iter->second.end())
		return nullptr;

	return m_mapObjectmapComponent[_pObjTag][_pComponentTag];
	//return mapComponet->second;
}

Engine::CComponent* Engine::CComponentMgr::Clone_Component(const _ushort& _wContainerIdx, const _tchar* _pComponentTag)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = Find_Component(_wContainerIdx, _pComponentTag);

	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent->Clone_Component();
}

void Engine::CComponentMgr::Free(void)
{
	for (_ushort i = 0; i < Engine::COMPONENTID::ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), Engine::CDeleteMap());
		m_mapComponent[i].clear();
	}

	for (auto& iter : m_mapObjectmapComponent)
		iter.second.clear();

	m_mapObjectmapComponent.clear();
}
