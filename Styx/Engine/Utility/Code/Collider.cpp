#include "Collider.h"

USING(Engine)

#include "VIBuffer.h"
#include "Export_Function.h"

Engine::CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
//#ifdef _DEBUG
	: m_pGraphicDev(pGraphicDev)
//#endif
{
//#ifdef _DEBUG
	m_pGraphicDev->AddRef();
//#endif

#ifdef _DEBUG
	m_bCollRender = TRUE;
#endif
}

Engine::CCollider::~CCollider(void)
{

}

void Engine::CCollider::Render_Collider(const _matrix* pColliderMatrix)
{
	m_matWorld = *pColliderMatrix;

//#ifdef _DEBUG

	if (Engine::COLLIDERTYPE::COL_CUBE == m_eColliderType)
	{
		//m_matWorld.m[0][0] = 0.01f;
		//m_matWorld.m[1][1] = 0.01f;
		//m_matWorld.m[2][2] = 0.01f;

		m_matWorld.m[0][0] = 1.f;
		m_matWorld.m[1][1] = 1.f;
		m_matWorld.m[2][2] = 1.f;
	}
	else if (Engine::COLLIDERTYPE::COL_SPHERE == m_eColliderType)
	{
		m_matWorld.m[0][0] = 1.f;
		m_matWorld.m[1][1] = 1.f;
		m_matWorld.m[2][2] = 1.f;
	}

	if (FALSE == Engine::Get_NaviMgr()->m_bCollRender)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTexture(0, m_pTexture[m_eIsCollide]);

	m_pColBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//#endif
}

void Engine::CCollider::Free(void)
{
//#ifdef _DEBUG
	Safe_Release(m_pColBuffer);
	Safe_Release(m_pGraphicDev);

	for (_uint i = 0; i < COL_END; ++i)
		Safe_Release(m_pTexture[i]);
//#endif


}



