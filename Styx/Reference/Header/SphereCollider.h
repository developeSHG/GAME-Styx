#ifndef SphereCollider_h__
#define SphereCollider_h__

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CSphereCollider : public CCollider
{
protected:
	/*  Structor  */
	explicit CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSphereCollider(void);

public:
	/*  General  */
	HRESULT						Ready_Collider(const _float fRadius,
												const _int iSlices,
												const _int iStacks);
	void						Render_Collider(const _matrix* pColliderMatrix, const _bool bRadiusUp = TRUE);

public:
	/*  Set  */


public:
	/*  Get  */
	const _float				Get_Radius() const { return m_fRadius; }

private:
	_float						m_fRadius;


	/*  Creation and destruction  */
public:
	static CSphereCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _float fRadius,
										const _int iSlices,
										const _int iStacks);
public:
	virtual CComponent*			Clone_Component(void) { return nullptr; }
public:
	virtual void				Free(void);
};

END
#endif // SphereCollider_h__
