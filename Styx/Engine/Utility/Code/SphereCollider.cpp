#include "SphereCollider.h"

#include "SphereCol.h"

USING(Engine)

Engine::CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCollider(pGraphicDev)
{
	m_eColliderType = Engine::COLLIDERTYPE::COL_SPHERE;
}

Engine::CSphereCollider::~CSphereCollider(void)
{

}

HRESULT Engine::CSphereCollider::Ready_Collider(const _float fRadius, 
												const _int iSlices, 
												const _int iStacks)
{
	m_fRadius = fRadius;

//#ifdef _DEBUG
	m_pColBuffer = Engine::CSphereCol::Create(m_pGraphicDev, fRadius, iSlices, iStacks);

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

void Engine::CSphereCollider::Render_Collider(const _matrix* pColliderMatrix, const _bool bRadiusUp /*= TRUE*/)
{
	_matrix matColliderWorld = *pColliderMatrix;
	matColliderWorld.m[3][1] += m_fRadius;

	if(TRUE == bRadiusUp)
		Engine::CCollider::Render_Collider(&matColliderWorld);
	if(FALSE == bRadiusUp)
		Engine::CCollider::Render_Collider(pColliderMatrix);
}

Engine::CSphereCollider* Engine::CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev,
																		const _float fRadius, 
																		const _int iSlices, 
																		const _int iStacks)
{
	CSphereCollider*	pInstance = new CSphereCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(fRadius, iSlices, iStacks)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CSphereCollider::Free(void)
{
	Engine::CCollider::Free();
}



