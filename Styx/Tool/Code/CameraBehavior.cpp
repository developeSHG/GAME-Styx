#include "stdafx.h"
#include "CameraBehavior.h"

#include "ToolMgr.h"

#include "CameraPoint.h"

CCameraBehavior::CCameraBehavior(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{

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
	CCameraHierarchy* pCameraHierarchy = CToolMgr::GetInstance()->m_pWorld->m_pCameraHierarchy;

	if (0 == pCameraHierarchy->m_vecCameraEyePoint.size()
		|| 1 == pCameraHierarchy->m_vecCameraEyePoint.size())
	{
		Set_Initialize();
		return FALSE;
	}

	CCameraPoint* pPointStart = pCameraHierarchy->m_vecCameraEyePoint[m_iNum];
 	CCameraPoint* pPointEnd = pCameraHierarchy->m_vecCameraEyePoint[m_iNum + 1];

	D3DXVec3CatmullRom(&m_vEye,
		&pPointStart->m_tSpline.vVirtualStartPos, &pPointStart->m_tSpline.vPos,
		&pPointEnd->m_tSpline.vPos, &pPointStart->m_tSpline.vVirtualEndPos,
		m_fItv);

	m_vEye.y += 1.f;

	pPointStart = pCameraHierarchy->m_vecCameraAtPoint[m_iNum];
	pPointEnd = pCameraHierarchy->m_vecCameraAtPoint[m_iNum + 1];

	D3DXVec3CatmullRom(&m_vAt,
		&pPointStart->m_tSpline.vVirtualStartPos, &pPointStart->m_tSpline.vPos,
		&pPointEnd->m_tSpline.vPos, &pPointStart->m_tSpline.vVirtualEndPos,
		m_fItv);

	if (TRUE == m_bPlay || FALSE == m_bPause)
	{
		m_fItv += 0.005f * fTimeDelta * m_fSpeed;
		m_fSpeed += pPointStart->m_tSpline.fIncSpeed * fTimeDelta;

		if (1.f < m_fItv)
		{
			m_fItv = 0.f;
			m_iNum += 1;
			m_fSpeed = pPointEnd->m_tSpline.fSpeed;
		}

		int iPos = int(((m_iNum + m_fItv) * CAMERA_ITV) / (pCameraHierarchy->m_vecCameraAtPoint.size() - 1));
		CToolMgr::GetInstance()->m_pCameraTool->m_ctrlActionSlider.SetPos(iPos);
	}

	if (pCameraHierarchy->m_vecCameraEyePoint.size() - 1.f <= m_iNum)
	{
		Set_Initialize();
		return FALSE;
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

	if(0 != CToolMgr::GetInstance()->m_pWorld->m_pCameraHierarchy->m_vecCameraEyePoint.size())
		m_fSpeed = CToolMgr::GetInstance()->m_pWorld->m_pCameraHierarchy->m_vecCameraEyePoint[0]->m_tSpline.fSpeed;

	m_bPlay = FALSE;
	m_bPause = FALSE;
	CToolMgr::GetInstance()->m_pCameraTool->m_ctrlActionSlider.SetPos(0);
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

