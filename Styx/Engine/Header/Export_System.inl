USING(Engine)

/*  GraphicDev  */
// Set
// Get
// General
HRESULT		Ready_GraphicDev(HWND hWnd, 
							WINMODE eMode, 
							const _uint& iSizeX, 
							const _uint& iSizeY, 
							Engine::CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}
void		Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void		Render_End(void)
{
	CGraphicDev::GetInstance()->Render_End();
}

/*  TimerMgr  */
// Set
void		Set_TimeDelta(const _tchar* pTimerTag)
{
	CTimerMgr::GetInstance()->Set_TimeDelta(pTimerTag);
}
// Get
_float		Get_TimeDelta(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
// General
HRESULT		Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}

/*  FrameMgr  */
// Set
// Get
_bool		IsPermit_Call(const _tchar* pFrameTag, 
							const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}
// General
HRESULT		Ready_Frame(const _tchar* pFrameTag, 
						const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}
void		Render_FPS(const _tchar* pFrameTag)
{
	return CFrameMgr::GetInstance()->Render_FPS(pFrameTag);
}

/*  SoundMgr  */
// Set
// Get
// General
void		MyPlaySound(TCHAR* pSoundKey, 
						Engine::CHANNELID eID)
{
	CSoundMgr::GetInstance()->MyPlaySound(pSoundKey, eID);
}
void		MyPlaySound2(TCHAR* pSoundKey,
	Engine::CHANNELID eID)
{
	CSoundMgr::GetInstance()->MyPlaySound2(pSoundKey, eID);
}
void		PlayBGM(TCHAR* pSoundKey, 
					bool bReduceVolume /*= false*/)
{
	CSoundMgr::GetInstance()->PlayBGM(pSoundKey, bReduceVolume);
}
void		StopSound(Engine::CHANNELID eID)
{
	CSoundMgr::GetInstance()->StopSound(eID);
}
void		StopAll()
{
	CSoundMgr::GetInstance()->StopAll();
}

/*  FontMgr  */
// Get
// Set
// General
HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, 
					const _tchar* pFontTag, 
					const _tchar* pFontType, 
					const _uint& iWidth, 
					const _uint& iHeight, 
					const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}
void	Render_Font(const _tchar* pFontTag, 
					const _tchar* pString, 
					const _vec2* pPos, 
					D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color);
}

/*  InputDev  */
// Set
void		Set_InputDev(void)
{
	CInputDev::GetInstance()->Set_InputDev();
}
// Get
_bool		Get_DIKeyPressing(Engine::_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyPressing(byKeyID);
}
_bool		Get_DIKeyDown(Engine::_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyDown(byKeyID);;
}
_bool		Get_DIKeyUp(Engine::_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyUp(byKeyID);;
}
_byte		Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Get_DIMouseState(eMouse);
}
_bool		Get_DIMousePressing(Engine::MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Get_DIMousePressing(eMouse);
}
_bool		Get_DIMouseDown(Engine::MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Get_DIMouseDown(eMouse);
}
_bool		Get_DIMouseUp(Engine::MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Get_DIMouseUp(eMouse);
}
_long		Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return CInputDev::GetInstance()->Get_DIMouseMove(eMouseState);
}
// General
HRESULT		Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}


/*  Release System  */
void Release_System(void)
{
	CInputDev::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
#ifndef _TOOL
		//CSoundMgr::GetInstance()->DestroyInstance();
#endif
	CGraphicDev::GetInstance()->DestroyInstance();
}