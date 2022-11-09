#include "Transform.h"

Engine::CTransform::CTransform(const _vec3& vLook)
	: m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
	, m_vDir(vLook)
{

}

Engine::CTransform::CTransform(const CTransform& rhs)
	: m_vScale(rhs.m_vScale)
	, m_vAngle(rhs.m_vAngle)
	, m_vDir(rhs.m_vDir)
{
	Ready_Transform();
}

Engine::CTransform::~CTransform(void)
{

}

HRESULT Engine::CTransform::Ready_Transform(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

Engine::_int Engine::CTransform::Update_Component(const _float& fTimeDelta /*= 0.f*/)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matNRotWorld);

	/*  Pos 전 Look 까지만  */
	for(_uint i = 0; i < Engine::INFO::INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	/*  Scale  */
	for (_uint i = 0; i < Engine::INFO::INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	/*  회전을 안먹이는 월드행렬은 여기서 회전변환 전에 복사해준다.  */
	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matNRotWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	/*  Rot  */
	_matrix		matRot[ROT_END];
	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	/*  World Set  */
	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	return UPDATE_OK;
}

Engine::_int Engine::CTransform::LateUpdate_Component(const _float& fTimeDelta /*= 0.f*/)
{
	return UPDATE_OK;
}

void Engine::CTransform::Set_Direction()
{
	Get_Info(INFO_LOOK, &m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}

void Engine::CTransform::Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
{
	m_vScale.x = fX;
	m_vScale.y = fY;
	m_vScale.z = fZ;
}

void Engine::CTransform::Set_Pos(const _float& fX, const _float& fY, const _float& fZ)
{
	m_vInfo[INFO_POS].x = fX;
	m_vInfo[INFO_POS].y = fY;
	m_vInfo[INFO_POS].z = fZ;
}

void Engine::CTransform::Set_AABBWorld(const _vec3& _vNewPos)
{
	/*  World Set  */
	for (Engine::_uint i = 0; i < Engine::INFO::INFO_END; ++i)
		memcpy(&m_matAABBWorld.m[i][0], &m_matWorld.m[i][0], sizeof(Engine::_vec4));

	m_matAABBWorld.m[3][0] = _vNewPos.x;
	m_matAABBWorld.m[3][1] = _vNewPos.y;
	m_matAABBWorld.m[3][2] = _vNewPos.z;
}

void Engine::CTransform::Chase_Target(const _vec3* _pTargetPos, const _float& _fSpeed)
{
	m_vDir = *_pTargetPos - m_vInfo[Engine::INFO::INFO_POS];
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	Move_Pos(&(m_vDir * _fSpeed));

	_matrix		matScale = *D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	_matrix		matRot	 = *Compute_LookAtTarget(_pTargetPos);
	_matrix		matTrans = *D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

const Engine::_matrix* Engine::CTransform::Compute_LookAtTarget(const _vec3* _pTargetPos)
{
	_float fAngle = D3DXVec3Dot(&m_vDir, D3DXVec3Normalize(&m_vInfo[INFO::INFO_UP], &m_vInfo[INFO::INFO_UP]));
	
	/*   Standard Rot Vector  */
	_vec3 vStandardRot = *D3DXVec3Cross(&vStandardRot, &m_vInfo[INFO::INFO_UP], &m_vDir);

	return D3DXMatrixRotationAxis(&_matrix(), &vStandardRot, acosf(fAngle));
}

Engine::CTransform* Engine::CTransform::Create(const _vec3& vLook)
{
	CTransform*		pInstance = new CTransform(vLook);

	if (FAILED(pInstance->Ready_Transform()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* Engine::CTransform::Clone_Component(void)
{
	return new CTransform(*this);
}

void Engine::CTransform::Free(void)
{

}