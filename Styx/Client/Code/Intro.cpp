#include "stdafx.h"
#include "Intro.h"

#include "SceneSelector.h"
#include "Layer.h"


CIntro::CIntro(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CScene(_pGraphicDev)
{

}

CIntro::~CIntro(void)
{

}

HRESULT CIntro::Ready_Scene()
{
	/************************ (SongSong) ************************/
	/*  Loading Create (Multi Thread)  */
	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_LOGO);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	Engine::MyPlaySound(L"Pegi.mp3", Engine::CHANNELID::UI);
	Engine::MyPlaySound(L"PassScene.mp3", Engine::CHANNELID::PASS_SCENE);
	
	FAILED_CHECK_RETURN(Video_Play(), E_FAIL);

	return S_OK;
}

_int CIntro::Update_Scene(const _float & fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);

	if (false == m_bSound)
	{
		if (2.f < m_fTime)
		{
			m_bSound = true;
			Engine::MyPlaySound(L"IntroBGM.mp3", Engine::CHANNELID::PLAYER);
		}
		else
			m_fTime += 1.f *fTimeDelta;
	}


	if (true == m_pLoading->Get_Finish())
	{
		if (Engine::Get_DIKeyDown(DIK_SPACE))
		{
			Engine::CManagement* ppManagementClass = nullptr;
			FAILED_CHECK_RETURN(Engine::Create_Management(&ppManagementClass), UPDATE_NO);

			(ppManagementClass)->AddRef();

			Engine::CScene*		pScene = this;
			pScene = CSceneSelector(SCENEID::SCENE_LOGO)(&pScene, m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, UPDATE_NO);

			FAILED_CHECK_RETURN((ppManagementClass)->SetUp_Scene(pScene, _ushort(SCENEID::SCENE_LOGO)), UPDATE_NO);
		}

		/*
		if (Engine::Get_DIKeyDown(DIK_RETURN))
		{
			Engine::CManagement* ppManagementClass = nullptr;
			FAILED_CHECK_RETURN(Engine::Create_Management(&ppManagementClass), UPDATE_NO);

			(ppManagementClass)->AddRef();

			Engine::CScene*		pScene = this;
			pScene = CSceneSelector(SCENEID::SCENE_LOGO)(&pScene, m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, UPDATE_NO);

			FAILED_CHECK_RETURN((ppManagementClass)->SetUp_Scene(pScene, _ushort(SCENEID::SCENE_LOGO)), UPDATE_NO);

			return UPDATE_OK;
		}
		*/

	}

	return UPDATE_OK;
}

_int CIntro::LateUpdate_Scene(const _float& fTimeDelta)
{
	return Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CIntro::Render_Scene(void)
{
	/*  Debug Mode  */
#ifdef _DEBUG

#endif // _DEBUG
}

HRESULT CIntro::Video_Play()
{
	m_bVideoPlay = true;

	/*「DirectShow 애플리케이션 프로그래밍의 개요」로 설명하고 있듯이,
	DirectShow 애플리케이션이 실행하는 기본적인 순서는 항상 같다.
	1.필터 그래프 매니저의 인스턴스를 생성 한다.
	2.필터 그래프 매니저를 사용해 필터 그래프를 생성 한다.
	3.그래프를 실행해, 데이터를 필터로 처리한다.
	우선 최초로,CoInitialize 를 호출해 COM 라이브러리를 초기화한다. */

	// COM 라이브러리를 초기화한다.
	HRESULT hr = CoInitialize(NULL); 

	//1. 필터 그래프 매니저를 생성 해, 인터페이스를 문의한다.
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_SERVER, IID_IGraphBuilder, (void**)&pGraph);
	if (FAILED(hr))
	{
		MSG_BOX("1Error");
		return E_FAIL;
	}

	//2. 이것으로, 필터 그래프를 생성 할 수 있다. 파일의 재생에 대해서는, 단일의 메서드의 호출로 행해진다
	hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
	if (FAILED(hr))
	{
		MSG_BOX("2Error");
		return E_FAIL;
	}
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
	if (FAILED(hr))
	{
		MSG_BOX("3Error");
		return E_FAIL;
	}
	pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVideoWindow);

	_tchar str[] = L"../Bin/Resource/Intro.wmv";
	hr = pGraph->RenderFile(str, NULL);

	pVideoWindow->put_WindowStyle(WS_CLIPSIBLINGS);
	pVideoWindow->HideCursor(OATRUE);
	pVideoWindow->put_Owner((OAHWND)g_hWnd);
	pVideoWindow->SetWindowPosition(0, 0, WINCX, WINCY);
	pVideoWindow->put_FullScreenMode(TRUE);

	if (SUCCEEDED(hr))
	{
		// 그래프를 실행한다.
		if (SUCCEEDED(pControl->Run()))
		{
			// 완료할 때까지 대기한다.
			/*long ev;
			pEvent->WaitForCompletion(INFINITE, &ev);*/
		}
	}
	else
	{
		MSG_BOX("4Error");
		return E_FAIL;
	}

	return S_OK;
}

CIntro* CIntro::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{ 
	CIntro* pInstance = new CIntro(_pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CIntro::Free(void)
{
	// 릴리즈는 창 끄는것과 관계가 없다.
	if (true == m_bVideoPlay)
	{
		pControl->Release();
		pEvent->Release();
		pGraph->Release();
		pVideoWindow->Release();
		CoUninitialize();
	}

	Safe_Release(m_pLoading);

	Engine::CScene::Free();
}