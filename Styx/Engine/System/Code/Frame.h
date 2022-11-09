#ifndef Frame_h__
#define Frame_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFrame : public CBase
{
private:
	explicit CFrame(void);
	virtual ~CFrame(void);

public:
	_bool		IsPermit_Call(const _float& fTimeDelta);

public:
	HRESULT		Ready_Frame(const _float& fCallLimit);
	void		Render_FPS();

private:
	_float		m_fCallLimit = 0.f;
	_float		m_fAccTimeDelta = 0.f;

private:
	_float		m_fTimeCnt = 0.f;
	TCHAR		m_szFrameInfo[128] = L"";
	_int		m_iFPSCnt = 0;

public:
	static CFrame*		Create(const _float& fCallLimit);
	virtual void		Free(void);

};

END
#endif // Frame_h__
