#include "Frame.h"

#include "Export_System.h"

USING(Engine)

Engine::CFrame::CFrame(void)
{

}

Engine::CFrame::~CFrame(void)
{

}

_bool Engine::CFrame::IsPermit_Call(const _float& fTimeDelta)
{
	m_fAccTimeDelta += fTimeDelta;
	m_fTimeCnt += fTimeDelta;

	if (m_fAccTimeDelta >= m_fCallLimit)
	{
		++m_iFPSCnt;
		m_fAccTimeDelta = 0.f;
		return true;
	}

	return false;
}

HRESULT Engine::CFrame::Ready_Frame(const _float& fCallLimit)
{
	m_fCallLimit = 1.f / fCallLimit;
		

	return S_OK;
}

void CFrame::Render_FPS()
{
	if (1.f <= m_fTimeCnt)
	{
		swprintf_s(m_szFrameInfo, L"FPS: %d", m_iFPSCnt);
		m_iFPSCnt = 0;
		m_fTimeCnt = 0.f;
	}

	Engine::Render_Font(L"Font_Default", m_szFrameInfo, &_vec2(100.f, 600.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

CFrame* Engine::CFrame::Create(const _float& fCallLimit)
{
	CFrame*	pInstance = new CFrame;

	if (FAILED(pInstance->Ready_Frame(fCallLimit)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CFrame::Free(void)
{

}

