#include "Management.h"

#include "Export_Utility.h"
#include "Export_System.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement(void)
{
	m_pRenderer = Get_Renderer();
}

Engine::CManagement::~CManagement(void)
{
	Free();
}

HRESULT Engine::CManagement::SetUp_Scene(CScene* pScene, _ushort _wSceneID)
{
	if (nullptr == m_pRenderer)
		Get_Renderer()->Clear_RenderGroup();
	else
		m_pRenderer->Clear_RenderGroup();

	m_pScene = pScene;
	m_wCurSceneID = _wSceneID;

	return S_OK;
}

_int Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	_int iEnd = UPDATE_OK;

	if (nullptr != m_pScene)
	{
		iEnd = m_pScene->Update_Scene(fTimeDelta);

		if (iEnd & 0x80000000)
			return iEnd;
	}

	return iEnd;
}

_int Engine::CManagement::LateUpdate_Scene(const float& fTimeDelta)
{
	_int iEnd = UPDATE_OK;

	if (nullptr != m_pScene)
	{
		iEnd = m_pScene->LateUpdate_Scene(fTimeDelta);

		if (iEnd & 0x80000000)
			return iEnd;
	}

	return iEnd;
}

void Engine::CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (nullptr == m_pRenderer)
		Get_Renderer()->Render_GameObject(pGraphicDev);
	else
		m_pRenderer->Render_GameObject(pGraphicDev);


	if (nullptr != m_pScene)
		m_pScene->Render_Scene();

	// DEBUG Ãâ·Â
//#ifdef _DEBUG
	//Engine::Render_FPS(L"Frame60");
//#endif // _DEBUG
}

void Engine::CManagement::Free(void)
{
	Safe_Release(m_pScene);
}

