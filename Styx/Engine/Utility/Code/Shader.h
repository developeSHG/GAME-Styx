#ifndef Shader_h__
#define Shader_h__

#include "Engine_Include.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	/*  Structor  */
	explicit	CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	CShader(const CShader& rhs);
	virtual		~CShader(void);

public:
	/*  General  */
	HRESULT						Ready_Shader(const _tchar* pFilePath);

public:
	/*  Set  */
	HRESULT						Set_Value(D3DXHANDLE hParameter, void* pValue, size_t iSize);
	HRESULT						Set_Texture(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture);
	HRESULT						Set_Bool(D3DXHANDLE hParameter, _bool Value);

public:
	/*  Get  */
	LPD3DXEFFECT				Get_EffectHandle(void) { return m_pEffect; }

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPD3DXEFFECT				m_pEffect;
	LPD3DXBUFFER				m_pErrMsg;



	/*  Creation and destruction  */
public:
	static	CShader*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath);
public:
	virtual CComponent*			Clone_Component(void);
public:
	virtual void				Free(void);
};

END
#endif // Shader_h__
