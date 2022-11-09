#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_vEye(0.f, 0.f, 0.f)
	, m_vAt(0.f, 0.f, 0.f)
	, m_vUp(0.f, 1.f, 0.f)
	, m_fFovY(0.f)
	, m_fAspect(0.f)
	, m_fNear(0.f)
	, m_fFar(0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

Engine::CCamera::~CCamera(void)
{

}


HRESULT Engine::CCamera::Ready_Object(void)
{
	return S_OK;
}

_int Engine::CCamera::Update_Object(const _float& fTimeDelta)
{
	return UPDATE_OK;
}

void Engine::CCamera::PreRender_Object(void)
{

}

void Engine::CCamera::Render_Object(void)
{

}

void Engine::CCamera::PostRender_Object(void)
{

}

void Engine::CCamera::SetViewSpaceMatrix(const D3DXVECTOR3* pEye,
											const D3DXVECTOR3* pAt,
											const D3DXVECTOR3* pUp)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;
}

void Engine::CCamera::SetProjectionMatrix(const float& fFovY,
											const float& fAspect,
											const float& fNear,
											const float& fFar)
{
	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
}

void Engine::CCamera::TransformViewSpaceMatrix()
{
	D3DXMatrixLookAtLH(&m_matView,
		&m_vEye,
		&m_vAt,
		&m_vUp);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void Engine::CCamera::TransformProjectionMatrix()
{
	D3DXMatrixPerspectiveFovLH(&m_matProj,
		m_fFovY,
		m_fAspect,
		m_fNear,
		m_fFar);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void Engine::CCamera::Free(void)
{

}


