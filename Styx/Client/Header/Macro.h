#ifndef Macro_h__
#define Macro_h__

#define  WINCX 1280
#define  WINCY 720

#define  CAMERA_ITV 200

#define	 TERRAIN_NUM_X 129
#define	 TERRAIN_NUM_Z 129

#define	GRAVITY	9.8f
#define JUMP_ACCEL 6.f

#define			PLAYER_COL_DOOR				0x00000001
#define			PLAYER_COL_CUPBOARD			0x00000002
#define			PLAYER_COL_LEVER			0x00000004
#define			PLAYER_COL_TORCH			0x00000008
#define			PLAYER_COL_TRUNK			0x00000010
#define			PLAYER_COL_GRID				0x00000020
#define			PLAYER_COL_ELITE			0x00000040
#define			PLAYER_COL_SERVANT_QUEST	0x00000080
#define			PLAYER_COL_SERVANT_RESCUE	0x00000100
#define			PLAYER_COL_MAP				0x00000200
#define			PLAYER_COL_DAGGER			0x00000400
#define			PLAYER_COL_KEY				0x00000800
#define			PLAYER_COL_CUTLASS			0x00001000
#define			PLAYER_COL_POSITION			0x00002000
#define			PLAYER_COL_POISON			0x00004000
#define			PLAYER_COL_CUP				0x00008000
#define			PLAYER_COL_MYSTERY			0x00010000
#define			PLAYER_COL_END				0x00000000

#ifndef MSG_BOX
#define	MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define SCAST static_cast

#define NULL_CHECK( _ptr)	\
{if( _ptr == 0){__asm { int 3 };return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
{if( _ptr == 0){__asm { int 3 };return _return;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };return _return;}}


#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return _return;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return _return;}



#define NO_COPY(CLASSNAME)							\
	private:										\
	CLASSNAME(const CLASSNAME&);					\
	CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)				\
	NO_COPY(CLASSNAME)								\
	private:										\
	static CLASSNAME*	m_pInstance;				\
	public:											\
	static CLASSNAME*	GetInstance( void );		\
	static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
		if(NULL == m_pInstance) {					\
			m_pInstance = new CLASSNAME;			\
		}											\
		return m_pInstance;							\
	}												\
	void CLASSNAME::DestroyInstance( void ) {		\
		if(NULL != m_pInstance)	{					\
			delete m_pInstance;						\
			m_pInstance = NULL;						\
		}											\
	}


#endif // Macro_h__
