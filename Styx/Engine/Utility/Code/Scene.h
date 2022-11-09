#ifndef Scene_h__
#define Scene_h__

#include "Engine_Include.h"
#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	/*  Structor  */
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene(void);

public:
	/*  General  */
	virtual HRESULT				Ready_Scene(void);
	virtual _int				Update_Scene(const _float& fTimeDelta);
	virtual _int				LateUpdate_Scene(const float& fTimeDelta);
	virtual void				Render_Scene(void) PURE;

public:
	/*  Get  */
	CLayer*						Get_Layer(const _tchar* _pLayerTag);

protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;
	map<wstring, CLayer*>			m_mapLayer;


	/*  Creation and destruction  */
public:
	virtual void Free(void);
};
END
#endif // Scene_h__
