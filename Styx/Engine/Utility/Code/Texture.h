#ifndef Texture_h__
#define Texture_h__

#include "Engine_Include.h"

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CResources
{
protected:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture(void);


public:
	HRESULT		Ready_Texture(const _tchar* pPath,
								TEXTURETYPE eType,
								const _uint& iCnt);

	void		Render_Texture(const _uint& iIndex = 0);
	void		Render_Texture(LPD3DXEFFECT pEffect, const char* pShaderName, const _uint& iIndex = 0);

public:
	/*  Get  */
	vector<IDirect3DBaseTexture9*>		Get_Texture() { return m_vecTexture; }

private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;


public:
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const _tchar* pPath,
									TEXTURETYPE eType,
									const _uint& iCnt = 1);
public:
	virtual CResources*		Clone(void);
public:
	virtual void			Free(void);
};

END

#endif // Texture_h__
