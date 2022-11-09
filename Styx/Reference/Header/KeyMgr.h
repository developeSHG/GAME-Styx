#ifndef KeyMgr_h__
#define KeyMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)


const DWORD KEY_UP	    = 0x00000001;	// 0000 0000 0000 0000 0000 0000 0000 0001
const DWORD KEY_DOWN    = 0x00000002;	// 0000 0000 0000 0000 0000 0000 0000 0010
const DWORD KEY_LEFT	= 0x00000004;	// 0000 0000 0000 0000 0000 0000 0000 0100
const DWORD KEY_RIGHT   = 0x00000008;	// 0000 0000 0000 0000 0000 0000 0000 1000
const DWORD KEY_RETURN	= 0x00000010;	// 0000 0000 0000 0000 0000 0000 0001 0000
const DWORD KEY_SPACE	= 0x00000020;	// 0000 0000 0000 0000 0000 0000 0010 0000
const DWORD KEY_LBUTTON = 0x00000040;	// 0000 0000 0000 0000 0000 0000 0100 0000
const DWORD KEY_RBUTTON = 0x00000080;	// 0000 0000 0000 0000 0000 0000 1000 0000

const DWORD KEY_A		= 0x00000100;	// 0000 0000 0000 0000 0000 0001 0000 0000
const DWORD KEY_D		= 0x00000200;	// 0000 0000 0000 0000 0000 0010 0000 0000
const DWORD KEY_W	    = 0x00000400;	// 0000 0000 0000 0000 0000 0101 0000 0000
const DWORD KEY_S       = 0x00000800;	// 0000 0000 0000 0000 0000 1000 0000 0000
const DWORD KEY_Q		= 0x00001000;	// 0000 0000 0000 0000 0001 0000 0000 0000
const DWORD KEY_E		= 0x00002000;	// 0000 0000 0000 0000 0010 0000 0000 0000

const DWORD KEY_1		= 0x00004000;	// 0000 0000 0000 0000 0100 0000 0000 0000
const DWORD KEY_2		= 0x00008000;	// 0000 0000 0000 0000 1000 0000 0000 0000
const DWORD KEY_3		= 0x00010000;	// 0000 0000 0000 0001 0000 0000 0000 0000
const DWORD KEY_4		= 0x00020000;	// 0000 0000 0000 0010 0000 0000 0000 0000
const DWORD KEY_5		= 0x00040000;	// 0000 0000 0000 0100 0000 0000 0000 0000
const DWORD KEY_6		= 0x00080000;	// 0000 0000 0000 1000 0000 0000 0000 0000

const DWORD KEY_P		= 0x00100000;	// 0000 0000 0001 0000 0000 0000 0000 0000
const DWORD KEY_SHIFT	= 0x00200000;	// 0000 0000 0010 0000 0000 0000 0000 0000
const DWORD KEY_TAB		= 0x00400000;	// 0000 0000 0100 0000 0000 0000 0000 0000


class ENGINE_DLL CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)

private:
	CKeyMgr(void);
	~CKeyMgr(void);


public:
	void		 Update(void);
	bool		 KeyDown(DWORD dwKey);
	bool		 KeyUp(DWORD dwKey);
	bool		 KeyPressing(DWORD dwKey);
	bool		 KeyCombined(DWORD dwFirst, DWORD dwSecond);

private:
	void		Release(void);

private:
	DWORD m_dwKey;
	DWORD m_dwKeyDown;
	DWORD m_dwKeyPressed;

};

END
#endif // KeyMgr_h__
