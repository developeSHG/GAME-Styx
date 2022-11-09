#ifndef Loading_h__
#define Loading_h__

#include "Include.h"
#include "Base.h"

class CLoading : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading(void);

public:
	LOADINGID					Get_LoadingID(void) const { return m_LoadingID; }
	CRITICAL_SECTION*			Get_Crt(void) { return &m_Crt; }
	_bool						Get_Finish(void) { return m_bFinish; }
	const _tchar*				Get_String(void) const { return m_szLoading; }

public:
	static _uint	APIENTRY	Thread_Main(void* pArg);

public:
	HRESULT						Ready_Loading(LOADINGID eLoadingID);

public:
	_uint						Loading_ForLogo(void);
	_uint						Loading_ForStage(void);

private:
	HRESULT						Read_Data(void);

private:
	HANDLE						m_hThread;
	CRITICAL_SECTION			m_Crt;
	LOADINGID					m_LoadingID;
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	_bool						m_bFinish = false;
	_tchar						m_szLoading[128];

public:	
	static CLoading*			Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID);
private:
	virtual void				Free(void);


};

#endif // Loading_h__
