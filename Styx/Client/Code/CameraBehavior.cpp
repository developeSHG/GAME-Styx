#include "stdafx.h"
#include "CameraBehavior.h"

#include "CameraHierarchy.h"
#include "CameraPoint.h"

CCameraBehavior::CCameraBehavior(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pCameraHierarchy = dynamic_cast<CCameraHierarchy*>(Engine::Find_ObjectListBack(Engine::OBJECT_CAMERAHIERARCHY));
	m_pStaticCamera = dynamic_cast<CStaticCamera*>(Engine::Find_StaticCameraObject());
	Set_Initialize();
}

CCameraBehavior::CCameraBehavior(const CCameraBehavior& rhs)
	: CComponent(rhs)
{

}

CCameraBehavior::~CCameraBehavior(void)
{

}

Engine::_int CCameraBehavior::Update_Component(const Engine::_float& fTimeDelta /*= 0.f*/)
{
	if (0 == m_pCameraHierarchy->m_vecCameraEyePoint.size()
		|| 1 == m_pCameraHierarchy->m_vecCameraEyePoint.size()
		|| m_pCameraHierarchy->m_vecCameraEyePoint.size() == m_iNum)
	{
		Set_Initialize();
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_END);
		return FALSE;
	}

	if (Engine::Get_DIKeyDown(DIK_RETURN))
	{
		Delete_Info();
		Set_Initialize();
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_END);
		return FALSE;
	}
	//Delete_Info();
	//Set_Initialize();
	//static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_END);
	//return FALSE;

	if (TRUE == m_bStart)
	{
		Start_Camera(fTimeDelta);
		return TRUE;
	}

	if (TRUE == m_bExit)
	{
		//Exit_Camera(fTimeDelta);
		return Exit_Camera(fTimeDelta);
	}

	CCameraPoint* pPointStart = m_pCameraHierarchy->m_vecCameraEyePoint[m_iNum];
	CCameraPoint* pPointEnd = m_pCameraHierarchy->m_vecCameraEyePoint[m_iNum + 1];

	D3DXVec3CatmullRom(&m_vEye,
						&pPointStart->m_tSpline.vVirtualStartPos, &pPointStart->m_tSpline.vPos,
						&pPointEnd->m_tSpline.vPos, &pPointStart->m_tSpline.vVirtualEndPos,
						m_fItv);

	m_vEye.y += 1.f;

	pPointStart = m_pCameraHierarchy->m_vecCameraAtPoint[m_iNum];
	pPointEnd = m_pCameraHierarchy->m_vecCameraAtPoint[m_iNum + 1];

	D3DXVec3CatmullRom(&m_vAt,
						&pPointStart->m_tSpline.vVirtualStartPos, &pPointStart->m_tSpline.vPos,
						&pPointEnd->m_tSpline.vPos, &pPointStart->m_tSpline.vVirtualEndPos,
						m_fItv);

	m_fItv += 0.005f * fTimeDelta * m_fSpeed;
	m_fSpeed += pPointStart->m_tSpline.fIncSpeed * fTimeDelta;

	if (1.f < m_fItv)
	{
		m_fItv = 0.f;
		m_iNum += 1;
		m_fSpeed = pPointEnd->m_tSpline.fSpeed;
	}
	
	if (m_pCameraHierarchy->m_vecCameraEyePoint.size() - 1.f <= m_iNum)
	{
		m_bExit = TRUE;
	}

	_matrix matView;
	D3DXMatrixLookAtLH(&matView,
		&m_vEye,
		&m_vAt,
		&_vec3(0.f, 1.f, 0.f));

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	return TRUE;
}

void CCameraBehavior::Set_Initialize()
{
	m_fItv = 0.f;
	m_iNum = 0;
	m_bStart = TRUE;
	m_bExit = FALSE;

	if(0 != m_pCameraHierarchy->m_vecCameraEyePoint.size())
		m_fSpeed = m_pCameraHierarchy->m_vecCameraEyePoint[0]->m_tSpline.fSpeed;
}

void CCameraBehavior::Start_Camera(const Engine::_float& fTimeDelta /*= 0.f*/)
{
	CCameraPoint* pPointStart = m_pCameraHierarchy->m_vecCameraEyePoint[m_iNum];
	CCameraPoint* pPointEnd = m_pCameraHierarchy->m_vecCameraEyePoint[m_iNum + 1];

	_vec3 vEyeOut, vAtOut;
	D3DXVec3CatmullRom(&vEyeOut,
						&pPointStart->m_tSpline.vVirtualStartPos, &pPointStart->m_tSpline.vPos,
						&pPointEnd->m_tSpline.vPos, &pPointStart->m_tSpline.vVirtualEndPos,
						m_fItv);

	vEyeOut.y += 1.f;

	pPointStart = m_pCameraHierarchy->m_vecCameraAtPoint[m_iNum];
	pPointEnd = m_pCameraHierarchy->m_vecCameraAtPoint[m_iNum + 1];

	D3DXVec3CatmullRom(&vAtOut,
						&pPointStart->m_tSpline.vVirtualStartPos, &pPointStart->m_tSpline.vPos,
						&pPointEnd->m_tSpline.vPos, &pPointStart->m_tSpline.vVirtualEndPos,
						m_fItv);

	_vec3 vEyeDir = vEyeOut - m_vEye;
	_vec3 vAtDir = vAtOut - m_vAt;

	if (0.015f < D3DXVec3Length(&vEyeDir) || 0.015f < D3DXVec3Length(&vAtDir))
	{
		float fEyePower = D3DXVec3Length(&vEyeDir) * 4.f;
		float fAtPower = D3DXVec3Length(&vAtDir) * 4.f;
		m_vEye += fEyePower * (*D3DXVec3Normalize(&vEyeDir, &vEyeDir)) * fTimeDelta;
		m_vAt += fAtPower * (*D3DXVec3Normalize(&vAtDir, &vAtDir)) * fTimeDelta;
	}
	else
	{
		m_bStart = FALSE;

		if(1 == m_wTrigger)
			Engine::MyPlaySound(L"Rulenumberone.mp3", Engine::CHANNELID::PLAYER);
	}

	_matrix matView;
	D3DXMatrixLookAtLH(&matView,
		&m_vEye,
		&m_vAt,
		&_vec3(0.f, 1.f, 0.f));

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
}

_bool CCameraBehavior::Exit_Camera(const Engine::_float& fTimeDelta /*= 0.f*/)
{
	_vec3 vEyeDir = m_pStaticCamera->Get_Eye() - m_vEye;
	_vec3 vAtDir = m_pStaticCamera->Get_At() - m_vAt;

	if (0.015f < D3DXVec3Length(&vEyeDir) || 0.015f < D3DXVec3Length(&vAtDir))
	{
		float fEyePower = D3DXVec3Length(&vEyeDir) * 4.f;
		float fAtPower = D3DXVec3Length(&vAtDir) * 4.f;
		m_vEye += fEyePower * (*D3DXVec3Normalize(&vEyeDir, &vEyeDir)) * fTimeDelta;
		m_vAt += fAtPower * (*D3DXVec3Normalize(&vAtDir, &vAtDir)) * fTimeDelta;

		_matrix matView;
		D3DXMatrixLookAtLH(&matView,
							&m_vEye,
							&m_vAt,
							&_vec3(0.f, 1.f, 0.f));

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	}
	else
	{
		_matrix matView;
		D3DXMatrixLookAtLH(&matView,
			&m_vEye,
			&m_vAt,
			&_vec3(0.f, 1.f, 0.f));

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

		Delete_Info();
		Set_Initialize();
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_END);
		return FALSE;
	}
	return TRUE;
}

void CCameraBehavior::Delete_Info(void)
{
	list<Engine::CGameObject*>& listRender = Engine::Get_Renderer()->Get_RendererGroup()[Engine::RENDERID::RENDER_NONALPHA];

	for (auto& pEyePoint : m_pCameraHierarchy->m_vecCameraEyePoint)
	{
		auto& iter = find(listRender.begin(), listRender.end(), pEyePoint);
		if (iter != listRender.end())
			listRender.erase(iter);
	}

	for (auto& pAtPoint : m_pCameraHierarchy->m_vecCameraAtPoint)
	{
		auto& iter = find(listRender.begin(), listRender.end(), pAtPoint);
		if (iter != listRender.end())
			listRender.erase(iter);
	}

	for_each(m_pCameraHierarchy->m_vecCameraEyePoint.begin(), m_pCameraHierarchy->m_vecCameraEyePoint.end(), Engine::CDeleteObj());
	m_pCameraHierarchy->m_vecCameraEyePoint.shrink_to_fit();
	m_pCameraHierarchy->m_vecCameraEyePoint.clear();

	for_each(m_pCameraHierarchy->m_vecCameraAtPoint.begin(), m_pCameraHierarchy->m_vecCameraAtPoint.end(), Engine::CDeleteObj());
	m_pCameraHierarchy->m_vecCameraAtPoint.shrink_to_fit();
	m_pCameraHierarchy->m_vecCameraAtPoint.clear();

	m_wTrigger++;
}

CCameraBehavior* CCameraBehavior::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return new CCameraBehavior(pGraphicDev);
}

Engine::CComponent* CCameraBehavior::Clone(void)
{
	return nullptr;
}

void CCameraBehavior::Free(void)
{
	
}

