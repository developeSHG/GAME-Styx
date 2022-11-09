#ifndef CubeCollider_h__
#define CubeCollider_h__

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCubeCollider : public CCollider
{
protected:
	/*  Structor  */
	explicit CCubeCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCubeCollider(void);

public:
	/*  General  */
	HRESULT						Ready_Collider(const _vec3* pPos,
												const _ulong& dwNumVtx, 
												const _ulong& dwStride);
	HRESULT						Ready_Collider(const _vec3& vMin,
												const _vec3& vMax);

public:
	/*  Set  */
	void						Set_Scale(const _vec3 _vecScale);

public:
	/*  Get  */
	const	_vec3*				Get_Min(void) const { return &m_vMin; }
	const	_vec3*				Get_Max(void) const { return &m_vMax; }

private:
	_vec3						m_vMin, m_vMax;



	/*  Creation and destruction  */
public:
	static CCubeCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev,	
										const _vec3* pPos,
										const _ulong& dwNumVtx,
										const _ulong& dwStride);
	static CCubeCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3& vMin,
										const _vec3& vMax);
public:
	virtual CComponent*			Clone_Component(void) { return nullptr; }
public:
	virtual void				Free(void);
};

END
#endif // CubeCollider_h__
