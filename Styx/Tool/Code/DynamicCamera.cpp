#include "stdafx.h"
#include "DynamicCamera.h"

#include "CameraBehavior.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{

}

CDynamicCamera::~CDynamicCamera(void)
{

}

HRESULT CDynamicCamera::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	m_vEye = _vec3(TERRAIN_NUM_X >> 1, 10.f, TERRAIN_NUM_Z / 2.9f - 8.f);
	//m_vEye = _vec3(0.f, 15.f, -5.5f);
	//m_vAt = _vec3(0.f, 15.f, 1.f);
	m_vUp = _vec3(0.f, 1.f, 0.f);

	/*  CameraBehavior Create  */
	m_pCameraBehavior = CCameraBehavior::Create(m_pGraphicDev);

	/*  VIEW Transform  */
	CCamera::TransformViewSpaceMatrix();

	/*  PROJECTION Set, Transform  */
	CCamera::SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 0.1f, 1000.f);
	CCamera::TransformProjectionMatrix();

	ShowCursor(false);

	return S_OK;
}

_int CDynamicCamera::Update_Object(const _float& fTimeDelta)
{
	if (TRUE == m_bPlayBehavior)
		m_bPlayBehavior = _bool(m_pCameraBehavior->Update_Component(fTimeDelta));
	else
	{
		KeyInput(fTimeDelta);
		TargetRenewal();

		/*  VIEW Transform  */
		CCamera::TransformViewSpaceMatrix();
	}

	return UPDATE_OK;
}

_int CDynamicCamera::LateUpdate_Object(const _float& fTimeDelta)
{
	//Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CDynamicCamera::PreRender_Object(void)
{

}

void CDynamicCamera::Render_Object(void)
{
	PreRender_Object();

	PostRender_Object();
}

void CDynamicCamera::PostRender_Object(void)
{

}

void CDynamicCamera::KeyInput(const _float& fTimeDelta)
{
	if (Engine::Get_DIKeyDown(DIK_Q))
	{
		m_bFixMouse = !(m_bFixMouse);

		if (FALSE == m_bFixMouse)
			ShowCursor(true);
		else
			ShowCursor(false);
	}
	if (Engine::Get_DIKeyDown(DIK_1))
		ShowCursor(false);
	if (Engine::Get_DIKeyDown(DIK_2))
		ShowCursor(true);

	if (TRUE == m_bFixMouse)
	{
		_long	dwMouse = 0;

		if (TOOL_NAVIMESH != CToolMgr::GetInstance()->m_eCurTool)
		{
			/*  Mouse LeftRight  */
			if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_X))
				//m_vAngle.y += dwMouse / 6.f * fTimeDelta;
				m_vAngle.y += dwMouse * fTimeDelta * 2.f;
			/*  Mouse UpDown  */		
			if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_Y))
				//m_vAngle.x += dwMouse * 8.f * fTimeDelta;
				m_vAngle.x += dwMouse * 80.f * fTimeDelta;
		}
		else
		{
			/*  Mouse LeftRight  */
			if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_X))
				m_vAngle.y += dwMouse / 3.f * fTimeDelta;
			/*  Mouse UpDown  */
			if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_Y))
				m_vAngle.x += dwMouse * 10.f * fTimeDelta;
		}
	}

	_vec3 vDir = m_vAt - m_vEye;
	D3DXVec3Normalize(&vDir, &vDir);

	if (Engine::Get_DIKeyPressing(DIK_W))
	{
		m_vEye += 25.f * vDir * fTimeDelta;
	}
	if (Engine::Get_DIKeyPressing(DIK_S))
	{
		m_vEye -= 25.f * vDir * fTimeDelta;
	}
	if (Engine::Get_DIKeyPressing(DIK_A))
	{
		/*  외적을 이용한 이동방식  */
		_vec3 vRight = *D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vDir);

		D3DXVec3Normalize(&vRight, &vRight);
		m_vEye += 25.f * -vRight * fTimeDelta;
	}
	if (Engine::Get_DIKeyPressing(DIK_D))
	{
		/*  행렬의 역행렬을 통해 Right벡터를 이용한 이동방식  */
		_matrix matView = *D3DXMatrixInverse(&matView, nullptr, &m_matView);

		memcpy(&vDir, &matView.m[0][0], sizeof(_vec3));
		m_vEye += 25.f * vDir * fTimeDelta;
	}
}

void CDynamicCamera::TargetRenewal(void)
{
	m_vAt = _vec3(0.f, 0.f, 1.f);
	D3DXVec3Normalize(&m_vAt, &m_vAt);

	/*  Rot  */
	{
		D3DXVECTOR3	vRight, vUp;
		D3DXMATRIX	matRotAxis;
		memcpy(&vRight, &_vec3(1.f, 0.f, 0.f), sizeof(float) * 3);
		memcpy(&vUp, &_vec3(0.f, 1.f, 0.f), sizeof(float) * 3);
		//memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(float) * 3);

		/*  RotUpdate OrderLine Important!!  */
		{
			/*  Rot UpDown  */
			D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(m_vAngle.x));
			D3DXVec3TransformNormal(&m_vAt, &m_vAt, &matRotAxis);

			/*  Rot LeftRight  */
			D3DXMatrixRotationAxis(&matRotAxis, &vUp, m_vAngle.y);
			D3DXVec3TransformNormal(&m_vAt, &m_vAt, &matRotAxis);
		}
	}

	//m_vEye = m_pTargetInfo->m_vInfo[Engine::INFO::INFO_POS];
	//m_vAt += m_pTargetInfo->m_vInfo[Engine::INFO::INFO_POS];
	m_vAt += m_vEye;
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamicCamera*	pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDynamicCamera::Free(void)
{
	Engine::Safe_Release(m_pCameraBehavior);

	Engine::CGameObject::Free();
}

