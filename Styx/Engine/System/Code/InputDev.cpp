#include "InputDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)

Engine::CInputDev::CInputDev(void)
{

}

Engine::CInputDev::~CInputDev(void)
{
	Free();
}

HRESULT Engine::CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{

	// DInput 컴객체를 생성하는 함수
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
											DIRECTINPUT_VERSION,
											IID_IDirectInput8,
											(void**)&m_pInputSDK,
											NULL), E_FAIL);

	// 키보드 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();


	return S_OK;
}

void Engine::CInputDev::Set_InputDev(void)
{
	for (int i = 0; i < 256; ++i)
	{
		if (m_bKeyState[i] && !(Get_DIKeyState(i) & 0x80))
			m_bKeyState[i] = !m_bKeyState[i];

		if (!m_bKeyState[i] && (Get_DIKeyState(i) & 0x80))
			m_bKeyState[i] = !m_bKeyState[i];
	}

	for (int i = 0; i < 4; ++i)
	{
		Engine::MOUSEKEYSTATE eState = (Engine::MOUSEKEYSTATE)i;
		if (m_bMouseState[eState] && !(Get_DIMouseState(eState) & 0x80))
			m_bMouseState[eState] = !m_bMouseState[eState];

		if (!m_bMouseState[eState] && (Get_DIMouseState(eState) & 0x80))
			m_bMouseState[eState] = !m_bMouseState[eState];
	}

	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

_bool CInputDev::Get_DIKeyPressing(_ubyte byKeyID)
{
	if (Get_DIKeyState(byKeyID) & 0x80)
		return true;
	return false;
}

_bool CInputDev::Get_DIKeyDown(_ubyte byKeyID)
{
	if ((!m_bKeyState[byKeyID]) && (Get_DIKeyState(byKeyID) & 0x80))
		return true;

	return false;
}

_bool CInputDev::Get_DIKeyUp(_ubyte byKeyID)
{
	if ((m_bKeyState[byKeyID]) && !(Get_DIKeyState(byKeyID) & 0x80))
		return true;

	return false;
}

_bool CInputDev::Get_DIMousePressing(Engine::MOUSEKEYSTATE eMouse)
{
	if (Get_DIMouseState(eMouse) & 0x80)
		return true;

	return false;
}

_bool CInputDev::Get_DIMouseDown(Engine::MOUSEKEYSTATE eMouse)
{
	if ((!m_bMouseState[eMouse]) && (Get_DIMouseState(eMouse) & 0x80))
		return true;

	return false;
}

_bool CInputDev::Get_DIMouseUp(Engine::MOUSEKEYSTATE eMouse)
{
	if ((m_bMouseState[eMouse]) && !(Get_DIMouseState(eMouse) & 0x80))
		return true;

	return false;
}

void Engine::CInputDev::Free(void)
{
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}
