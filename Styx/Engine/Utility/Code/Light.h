#ifndef Light_h__
#define Light_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CShader;
class CVIBuffer_ViewPort;

class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight(void);
	
public:
	HRESULT					Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	HRESULT					Render_Light(CShader* pShader);

public:
	/*  Set  */
	void					Set_Active(const _bool& _bActive) { m_bActive = _bActive; }

public:
	/*  Get  */
	D3DLIGHT9*				Get_LightInfo(void) { return &m_tLightInfo; }
	const _bool&			Get_Active(void) { return m_bActive; }

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	D3DLIGHT9				m_tLightInfo;
	_uint					m_iIndex = 0;
	CVIBuffer_ViewPort*		m_pVIBuffer = nullptr;
	_bool					m_bActive = FALSE;



	/*  Creation and destruction  */
public:
	static CLight*			Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const D3DLIGHT9* pLightInfo,
									const _uint& iIndex);
private:
	virtual void			Free(void);

};

END
#endif // Light_h__
