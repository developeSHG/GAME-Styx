#include "NaviMgr.h"

#include "Export_Resources.h"
#include "NaviMesh.h"

IMPLEMENT_SINGLETON(Engine::CNaviMgr)

Engine::CNaviMgr::CNaviMgr()
{
	Create_NaviMesh();
}

Engine::CNaviMgr::~CNaviMgr(void)
{
	Free();
}

HRESULT Engine::CNaviMgr::Create_NaviMesh()
{
	m_pNaviMesh = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone(RESOURCEID::RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(m_pNaviMesh, E_FAIL);

	return S_OK;
}

void Engine::CNaviMgr::Free(void)
{
	Engine::Safe_Release(m_pNaviMesh);
}
