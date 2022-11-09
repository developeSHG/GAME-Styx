#include "CubeCollider.h"

#include "CubeCol.h"

USING(Engine)

Engine::CCubeCollider::CCubeCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCollider(pGraphicDev)
{
	m_eColliderType = Engine::COLLIDERTYPE::COL_CUBE;
}

Engine::CCubeCollider::~CCubeCollider(void)
{

}

HRESULT Engine::CCubeCollider::Ready_Collider(const _vec3* pPos,	
											const _ulong& dwNumVtx, 
											const _ulong& dwStride)
{
	/*  pPos : 전체 정점들 중 첫번째 정점의 포지션 값  */
	D3DXComputeBoundingBox(pPos, dwNumVtx, sizeof(_vec3), &m_vMin, &m_vMax);

//ssssssssswwwwwwwwqasssssssssssssswwwwwwwwwwwwwwwwwwwwwwdsssssswwwwwwwwqsssssssssssssssq

	m_pColBuffer = Engine::CCubeCol::Create(m_pGraphicDev, m_vMin, m_vMax);

	for (_uint i = 0; i < COL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT	tLockRect;
		ZeroMemory(&tLockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &tLockRect, NULL, 0);

		*((_ulong*)tLockRect.pBits) = D3DXCOLOR(1.f * i, 1.f * (1.f - i), 0.f, 1.f);
		
		m_pTexture[i]->UnlockRect(0);
	}

//#endif

	return S_OK;
}

HRESULT Engine::CCubeCollider::Ready_Collider(const _vec3& vMin, const _vec3& vMax)
{
	m_vMin = vMin;
	m_vMax = vMax;

	m_pColBuffer = Engine::CCubeCol::Create(m_pGraphicDev, m_vMin, m_vMax);

	for (_uint i = 0; i < COL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT	tLockRect;
		ZeroMemory(&tLockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &tLockRect, NULL, 0);

		*((_ulong*)tLockRect.pBits) = D3DXCOLOR(1.f * i, 1.f * (1.f - i), 0.f, 1.f);

		m_pTexture[i]->UnlockRect(0);
	}

	return S_OK;
}

void Engine::CCubeCollider::Set_Scale(const _vec3 _vecScale)
{
	m_vMax.x *= _vecScale.x;
	m_vMax.y *= _vecScale.y;
	m_vMax.z *= _vecScale.z;

	m_vMin.x *= _vecScale.x;
	m_vMin.y *= _vecScale.y;
	m_vMin.z *= _vecScale.z;
}

Engine::CCubeCollider* Engine::CCubeCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
													const _vec3* pPos, 
													const _ulong& dwNumVtx, 
													const _ulong& dwStride)
{
	CCubeCollider*	pInstance = new CCubeCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(pPos, dwNumVtx, dwStride)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CCubeCollider* Engine::CCubeCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
													const _vec3& vMin, 
													const _vec3& vMax)
{
	CCubeCollider*	pInstance = new CCubeCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(vMin, vMax)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCubeCollider::Free(void)
{
	Engine::CCollider::Free();
}



