#ifndef Collider_h__
#define Collider_h__

#include "Component.h"

BEGIN(Engine)

class CVIBuffer;

class ENGINE_DLL CCollider : public CComponent
{
protected:
	/*  Structor  */
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCollider(void);

public:
	/*  General  */
	void						Render_Collider(const _matrix* pColliderMatrix);

public:
	/*  Set  */
	void						Set_IsCollide(const COLLTYPE _eCheck) { m_eIsCollide = _eCheck; }
	void						Set_WorldMatrix(const _matrix* pColliderMatrix) { m_matWorld = *pColliderMatrix; }

public:
	/*  Get  */
	const _matrix*				Get_ColliderWorld(void) const { return &m_matWorld; }
	const COLLTYPE				Get_IsCollide(void) const { return m_eIsCollide; }
	const COLLIDERTYPE			Get_ColliderType(void) const { return m_eColliderType; }
	const _bool					Get_CollRender(void) const { return m_bCollRender; }

protected:
	_matrix						m_matWorld;
	COLLTYPE					m_eIsCollide = COLLTYPE::COL_FALSE;
	COLLIDERTYPE				m_eColliderType;
	_bool						m_bCollRender = FALSE;

//#ifdef _DEBUG
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	CVIBuffer*					m_pColBuffer;
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];
//#endif


	/*  Creation and destruction  */
public:
	virtual CComponent*			Clone_Component(void) { return nullptr; }
public:
	virtual void				Free(void) PURE;
};

END
#endif // Collider_h__
