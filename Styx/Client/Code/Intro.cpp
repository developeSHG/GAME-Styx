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

	/*��DirectShow ���ø����̼� ���α׷����� ���䡹�� �����ϰ� �ֵ���,
	DirectShow ���ø����̼��� �����ϴ� �⺻���� ������ �׻� ����.
	1.���� �׷��� �Ŵ����� �ν��Ͻ��� ���� �Ѵ�.
	2.���� �׷��� �Ŵ����� ����� ���� �׷����� ���� �Ѵ�.
	3.�׷����� ������, �����͸� ���ͷ� ó���Ѵ�.
	�켱 ���ʷ�,CoInitialize �� ȣ���� COM ���̺귯���� �ʱ�ȭ�Ѵ�. */

	// COM ���̺귯���� �ʱ�ȭ�Ѵ�.
	HRESULT hr = CoInitialize(NULL); 

	//1. ���� �׷��� �Ŵ����� ���� ��, �������̽��� �����Ѵ�.
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_SERVER, IID_IGraphBuilder, (void**)&pGraph);
	if (FAILED(hr))
	{
		MSG_BOX("1Error");
		return E_FAIL;
	}

	//2. �̰�����, ���� �׷����� ���� �� �� �ִ�. ������ ����� ���ؼ���, ������ �޼����� ȣ��� ��������
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
		// �׷����� �����Ѵ�.
		if (SUCCEEDED(pControl->Run()))
		{
			// �Ϸ��� ������ ����Ѵ�.
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
	// ������� â ���°Ͱ� ���谡 ����.
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