#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev : public CBase
{
	DECLARE_SINGLETON(CInputDev)

private:
	explicit CInputDev(void);
	virtual ~CInputDev(void);

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);

public:
	/*  Set  */
	void	Set_InputDev(void);

public:
	/*  Get  */
	_byte	Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	_bool   Get_DIKeyPressing(_ubyte byKeyID);
	_bool	Get_DIKeyDown(_ubyte byKeyID);
	_bool	Get_DIKeyUp(_ubyte byKeyID);

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) { return m_tMouseState.rgbButtons[eMouse]; }
	_bool   Get_DIMousePressing(Engine::MOUSEKEYSTATE eMouse);
	_bool	Get_DIMouseDown(Engine::MOUSEKEYSTATE eMouse);
	_bool	Get_DIMouseUp(Engine::MOUSEKEYSTATE eMouse);

	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tMouseState) + eMouseState);
	}

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_byte					m_byKeyState[256];
	_bool				    m_bKeyState[256];

	DIMOUSESTATE			m_tMouseState;
	_bool				    m_bMouseState[3];


public:
	virtual void	Free(void);
};
END



#endif // InputDev_h__
