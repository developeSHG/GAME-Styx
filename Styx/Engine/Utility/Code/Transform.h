#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
protected:
	/*  Structor  */
	explicit CTransform(const _vec3& vLook);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform(void);

public:
	/*  General  */
	HRESULT					Ready_Transform(void);
	virtual _int			Update_Component(const _float& fTimeDelta = 0.f);
	virtual _int			LateUpdate_Component(const _float& fTimeDelta = 0.f);

public:
	/*  Set  */
	void					Set_Direction();
	void					Set_Scale(const _float& fX, const _float& fY, const _float& fZ);
	void					Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void					Set_Pos(const _vec3* pPos) { m_vInfo[INFO_POS] = *pPos; }
	void					Set_ParentMatrix(const _matrix* pParentMatrix) { m_matWorld *= *pParentMatrix; }
	void					Set_AABBWorld(const _vec3& _vNewPos);

public:
	/*  Get  */
	void					Get_Info(INFO eType, _vec3* pInfo) { memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3)); }
	const _vec3				Get_InfoPos() const { return m_vInfo[INFO::INFO_POS]; }
	const _vec3*			Get_InfoPosPointer() const { return &m_vInfo[INFO::INFO_POS]; }
	void					Get_WorldMatrix(_matrix* pWorldMatrix) const { *pWorldMatrix = m_matWorld; }
	const _matrix*			Get_WorldMatrixPointer(void) const { return &m_matWorld; }
	void					Get_NRotWorldMatrix(_matrix* pWorldMatrix) const { *pWorldMatrix = m_matNRotWorld; }
	const _matrix&			Get_AABBWorld() const { return m_matAABBWorld; }

public:
	void					Move_Pos(const _vec3* pDir) { m_vInfo[Engine::INFO::INFO_POS] += *pDir; }
	void					Rotation(ROTATION eType, const _float& fAngle) { *((_float*)&m_vAngle + eType) += fAngle; }

public:
	void					Chase_Target(const _vec3* _pTargetPos, const _float& _fSpeed);
	const _matrix*			Compute_LookAtTarget(const _vec3* _pTargetPos);

public:
	_vec3					m_vInfo[Engine::INFO::INFO_END];
	_vec3					m_vScale;
	_vec3					m_vAngle;
	_vec3					m_vDir;
	_matrix					m_matWorld;
	_matrix					m_matNRotWorld;
	_matrix					m_matAABBWorld;


	/*  Creation and destruction  */
public:
	static CTransform*			Create(const _vec3& vLook);
public:
	virtual CComponent*			Clone_Component(void);
public:
	virtual void				Free(void);
};

END

#endif // Transform_h__
