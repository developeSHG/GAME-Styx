#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Include.h"
#include "Base.h"
#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)

private:
	explicit CLightMgr(void);
	virtual ~CLightMgr(void);

public:
	HRESULT					Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
										const D3DLIGHT9* pLightInfo,
										const _uint& iIndex);
	HRESULT					Render_Light(CShader* pShader);

public:
	/*  Set  */
	void					Set_LightActive(const _uint iIndex = 0, bool _bActive = TRUE);

public:
	/*  Get  */
	D3DLIGHT9*				Get_LightInfo(const _uint iIndex = 0);
	_ushort					Get_LightSize() { return m_listLight.size(); }
	_bool					Get_LightActive(const _uint iIndex = 0);

private:
	list<CLight*>				m_listLight;
	typedef list<CLight*>		LIGHTLIST;


	/*  Creation and destruction  */
public:
	virtual void			Free(void);

};
END
#endif // LightMgr_h__
