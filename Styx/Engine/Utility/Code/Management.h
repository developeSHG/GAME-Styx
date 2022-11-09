#ifndef Management_h__
#define Management_h__

#include "Engine_Include.h"
#include "Base.h"
#include "Scene.h"

BEGIN(Engine)

class CRenderer;

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	/*  Structor  */
	explicit CManagement(void);
	virtual ~CManagement(void);

public:
	/*  General  */
	HRESULT				SetUp_Scene(CScene* pScene, _ushort _wSceneID);
	_int				Update_Scene(const _float& fTimeDelta);
	_int				LateUpdate_Scene(const float& fTimeDelta);
	void				Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void				Set_EliteWave() { m_uEliteWave++; }

public:
	/*  Get  */
	const _ushort		Get_CurScene() const { return m_wCurSceneID; }
	const _ushort		Get_EliteWave() const { return m_uEliteWave; }

private:
	CRenderer*			m_pRenderer = nullptr;

private:
	CScene*				m_pScene = nullptr;
	_ushort				m_wCurSceneID;
	_ushort				m_uEliteWave = 0;

public:
	virtual void	Free(void);
};

END
#endif // Management_h__
