#ifndef Export_System_h__
#define Export_System_h__

#include "Engine_Include.h"

#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "SoundMgr.h"
#include "FontMgr.h"
#include "InputDev.h"

BEGIN(Engine)

/*  GraphicDev  */
// Set
// Get
// General
inline HRESULT		Ready_GraphicDev(HWND hWnd,	WINMODE eMode,	const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev);
inline void			Render_Begin(D3DXCOLOR Color);
inline void			Render_End(void);

/*  TimerMgr  */
// Set
inline void			Set_TimeDelta(const _tchar* pTimerTag);
// Get
inline _float		Get_TimeDelta(const _tchar* pTimerTag);
// General
inline HRESULT		Ready_Timer(const _tchar* pTimerTag);

/*  FrameMgr  */
// Set
// Get
inline	_bool		IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
// General
inline	HRESULT		Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);
inline  void		Render_FPS(const _tchar* pFrameTag);

/*  SoundMgr  */
// Set
// Get
// General
inline void			MyPlaySound(TCHAR* pSoundKey, Engine::CHANNELID eID);
inline void			MyPlaySound2(TCHAR* pSoundKey, Engine::CHANNELID eID);
inline void			PlayBGM(TCHAR* pSoundKey, bool bReduceVolume = false);
inline void			StopSound(Engine::CHANNELID eID);
inline void			StopAll();

/*  FontMgr  */
// Set
// Get
// General
inline HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
inline void		Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color);

/*  InputDev  */
// Set
inline void		Set_InputDev(void);
// Get
inline _bool    Get_DIKeyPressing(_ubyte byKeyID);
inline _bool    Get_DIKeyDown(_ubyte byKeyID);
inline _bool    Get_DIKeyUp(_ubyte byKeyID);
inline _byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
inline _bool    Get_DIMousePressing(Engine::MOUSEKEYSTATE eMouse);
inline _bool    Get_DIMouseDown(Engine::MOUSEKEYSTATE eMouse);
inline _bool    Get_DIMouseUp(Engine::MOUSEKEYSTATE eMouse);
inline _long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
// General
inline HRESULT	Ready_InputDev(HINSTANCE hInst, HWND hWnd);



/*  Release System  */
inline void Release_System(void);

#include "Export_System.inl"

END
#endif // Export_System_h__