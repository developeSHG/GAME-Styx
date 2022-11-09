 #include "stdafx.h"
#include "StaticCamera.h"

#include "Player.h"
#include "PlayerBehavior.h"
#include "DynamicCamera.h"
#include "CameraBehavior.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{
	m_bOwnerShip = TRUE;
}

CStaticCamera::~CStaticCamera(void)
{

}

HRESULT CStaticCamera::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	Engine::CComponent*		pComponent = nullptr;

	/*  NaviMesh  */
	pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone(RESOURCEID::RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_NAVIMESH, pComponent);

	m_pNaviMeshCom->Set_Index(0);

	/*  VIEW Set, Transform  */
	CCamera::SetViewSpaceMatrix(&_vec3(0.f, 7.f, -1.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f));
	CCamera::TransformViewSpaceMatrix();

	/*  PROJECTION Set, Transform  */
	CCamera::SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 0.1f, 1000.f);
	CCamera::TransformProjectionMatrix();

	/************************ (SongSong) ************************/
	/*  ObjectMgr Get Player and Get Component  */
	m_pPlayer = dynamic_cast<CPlayer*>(Engine::Find_PlayerObject());
	m_pPlayer->Get_PlayerBehavior()->Set_StaticCamera(this);
	m_pTargetCom = dynamic_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));
	NULL_CHECK_RETURN(m_pTargetCom, E_FAIL);

	return S_OK;
}

_int CStaticCamera::Update_Object(const _float& fTimeDelta)
{
	if (FALSE == m_bOwnerShip)
		return UPDATE_OK;

	if (FALSE == m_bElitePerson)
	{
		KeyInput(fTimeDelta);

		/*  Dynamic Trigger  */
		Update_DynamicTrigger();

		/*  Camera Purpose  */
		Update_CameraPurpose();

		TargetRenewal(fTimeDelta);

		TargetDistCheckRenewal(fTimeDelta);

		FirstNarrativeRenewal();
	}
	else
	{
		m_vAt = m_pTargetCom->Get_InfoPos();
		m_vAt.y += 1.f;
	}

	/*  VIEW Transform  */
	CCamera::TransformViewSpaceMatrix();

	return UPDATE_OK;
}

_int CStaticCamera::LateUpdate_Object(const _float& fTimeDelta)
{
	return UPDATE_OK;
}

void CStaticCamera::PreRender_Object(void)
{

}

void CStaticCamera::Render_Object(void)
{
	PreRender_Object();
	
	PostRender_Object();
}

void CStaticCamera::PostRender_Object(void)
{

}

void CStaticCamera::Set_FightState(const _bool& _State)
{
	m_bFightState = _State;

	if (TRUE == m_bFightState)
	{
		m_vFightPrevAngle.x = m_vAngle.x;
		m_vAngle.x = 6.5f;
	}
	else
		m_vAngle.x = m_vFightPrevAngle.x;
}

void CStaticCamera::KeyInput(const _float& fTimeDelta)
{
	if (CAMERA_PURPOSE::PURPOSE_QUEST == m_eCameraPurpose)
		return;

	if (Engine::Get_DIKeyDown(DIK_Q))
	{
		m_bFixMouse = !(m_bFixMouse);
		
		if (TRUE == m_bFixMouse)
		{
			ShowCursor(true);
			static_cast<CCursor*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_CURSOR))->Set_Active(FALSE);
		}
		else if (FALSE == m_bFixMouse)
		{
			ShowCursor(false);
			static_cast<CCursor*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_CURSOR))->Set_Active(TRUE);
		}
	}

	if (TRUE == m_bFixMouse)
	{
		POINT pt = { WINCX >> 1, WINCY >> 1 };

		ClientToScreen(g_hWnd, &pt);
		SetCursorPos(pt.x, pt.y);

		//현제 마우스 위치 , pt는 화면중앙 좌표
		POINT ptMouse;
		GetCursorPos(&ptMouse);

		_long	dwMouse = 0;

		/*  Mouse UpDown  */
		{
			if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_Y))
			{
				m_vAngle.x -= dwMouse * 10.f * fTimeDelta;

				if (m_vAngle.x >= MAX_ANGLE_X)
					m_vAngle.x = MAX_ANGLE_X;
				if (m_vAngle.x < MIN_ANGLE_X)
					m_vAngle.x = MIN_ANGLE_X;
			}
		}

		/*  Mouse LeftRight  */
		if (FALSE == m_bOnCamColl)
		{
			if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_X))
			{
				m_vAngle.y += dwMouse / 2.f * fTimeDelta;
				//cout << D3DXToDegree(m_vAngle.y) << endl;
			}
		}

		/*  Mouse Wheel  */	
		if (FALSE == m_bFightState)
		{
			if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_Z))
			{
				if (dwMouse > 0)		//위로 스크롤   = 가까워지기
					m_fNewDistance = m_fDistance - (dwMouse / 80.f);
				else                    //아래로 스크롤 = 멀어지기
					m_fNewDistance = m_fDistance - (dwMouse / 80.f);

				if (m_fNewDistance < MIN_DISTANCE)
					m_fNewDistance = MIN_DISTANCE;
				if (m_fNewDistance > MAX_DISTANCE)
					m_fNewDistance = MAX_DISTANCE;

				m_fOriginDistance = m_fNewDistance;
			}
			m_fDistance += ((m_fNewDistance - m_fDistance)) * 4.f * fTimeDelta;

			if (fabsf(m_fDistance - m_fNewDistance) < 0.001f)
			{
				m_fDistance = m_fNewDistance;
				m_bOnCamColl = FALSE;
			}
		}
	}
}

void CStaticCamera::TargetDistCheckRenewal(const _float& fTimeDelta)
{
	if (FALSE == m_bOwnerShip)
		return;

	switch (m_eCameraPurpose)
	{
	case CAMERA_PURPOSE::PURPOSE_QUEST:
		m_vNewEye = _vec3(65.0376f, 6.2359f, 50.15f);
		m_vNewAt = _vec3(63.4723f, 5.27722f, 47.17f);
		break;
	}

	_vec3 vEyeDist = m_vNewEye - m_vEye;
	_vec3 vAtDist = m_vNewAt - m_vAt;

	if (1.f < D3DXVec3Length(&vEyeDist)
		|| 1.f < D3DXVec3Length(&vAtDist))
		m_bCameraMoving = TRUE;

	if (FALSE == m_bCameraMoving)
	{
		m_vEye = m_vNewEye;
		m_vAt = m_vNewAt;
	}
	else if(TRUE == m_bCameraMoving)
	{
		_vec3 vEyeDir = m_vNewEye - m_vEye;
		_vec3 vAtDir = m_vNewAt - m_vAt;

		//if (0.015f < D3DXVec3Length(&vEyeDir) || 0.015f < D3DXVec3Length(&vAtDir))
		{
			float fSpeed = 6.5f;
			if (CAMERA_PURPOSE::PURPOSE_QUEST == m_eCameraPurpose)
				fSpeed = 3.f;
			else if (CAMERA_PURPOSE::PURPOSE_FIRST_PERSON == m_eCameraPurpose)
				fSpeed = 4.f;
			else if (TRUE == m_bDeadState && FALSE == m_pPlayer->Get_EliteState())
				fSpeed = 5.5f;
			else if (TRUE == m_bDeadState && TRUE == m_pPlayer->Get_EliteState())
				fSpeed = 4.f;
			float fEyePower = D3DXVec3Length(&vEyeDir) * fSpeed;
			float fAtPower = D3DXVec3Length(&vAtDir) * fSpeed;
			m_vEye += fEyePower * (*D3DXVec3Normalize(&vEyeDir, &vEyeDir)) * fTimeDelta;
			m_vAt += fAtPower * (*D3DXVec3Normalize(&vAtDir, &vAtDir)) * fTimeDelta;
		}
		//else
		{
			//m_bCameraMoving = FALSE;
		}
	}
}

void CStaticCamera::TargetRenewal(const _float& fTimeDelta)
{
	if (FALSE == m_bOwnerShip
		|| CAMERA_PURPOSE::PURPOSE_FIRST_PERSON == m_eCameraPurpose)
		return;

	/*  Eye Distance Init  */
	m_vNewEye = D3DXVECTOR3(0.f, 0.f, -1.f);
	if (FALSE == m_bFightState && FALSE == m_bCheerState && FALSE == m_bDeadState)
		m_vNewEye *= m_fDistance;
	else if (TRUE == m_bFightState)
		m_vNewEye *= 2.75f;
	else if (TRUE == m_bCheerState)
		m_vNewEye *= -1.5f;
	else if (TRUE == m_bDeadState)
		m_vNewEye *= 1.f;

	/*  Rot  */
	{
		_vec3 vRight, vUp;
		_matrix	matRotAxis;
		memcpy(&vRight, &_vec3(1.f, 0.f, 0.f), sizeof(float) * 3);
		memcpy(&vUp, &_vec3(0.f, 1.f, 0.f), sizeof(float) * 3);

		/*  RotUpdate OrderLine Important!!  */
		{
			/*  Rot UpDown  */
			D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(m_vAngle.x));
			D3DXVec3TransformNormal(&m_vNewEye, &m_vNewEye, &matRotAxis);

			/*  Rot LeftRight  */
			D3DXMatrixRotationAxis(&matRotAxis, &vUp, m_vAngle.y);
			D3DXVec3TransformNormal(&m_vNewEye, &m_vNewEye, &matRotAxis);
		}
	}

	/*  World  */
	m_vNewEye += m_pTargetCom->m_vInfo[Engine::INFO::INFO_POS];
	//m_vEye.y += 1.7f;
	m_vNewEye.y += 1.35f;
	m_vNewAt = m_pTargetCom->m_vInfo[Engine::INFO::INFO_POS];
	m_vNewAt.y += 1.35f;

	/*  Cheer State  */
	if (TRUE == m_bCheerState)
	{
		m_vNewEye = m_pTargetCom->m_vInfo[Engine::INFO::INFO_POS];
		_vec3 vDir = m_pTargetCom->m_vInfo[Engine::INFO::INFO_LOOK];
		D3DXVec3Normalize(&vDir, &vDir);
		m_vNewEye += vDir * 1.75f;
		m_vNewEye.y += 0.5f;

		m_vNewAt = m_pTargetCom->m_vInfo[Engine::INFO::INFO_POS];
		m_vNewAt.y += 0.5f;
	}
	/*  Dead State  */
	else if (TRUE == m_bDeadState)
	{
		m_vNewAt = m_pTargetCom->m_vInfo[Engine::INFO::INFO_POS];

		if (TRUE == m_pPlayer->Get_EliteState())
			m_vNewAt.y += 1.f;
	}
}

void CStaticCamera::FirstNarrativeRenewal(void)
{
	if (CAMERA_PURPOSE::PURPOSE_FIRST_PERSON != m_eCameraPurpose)
		return;

	_vec3 vEyeDist = m_vEye - m_pTargetCom->m_vInfo[Engine::INFO::INFO_POS];
	vEyeDist.y = 0.f;
	if (0.7f >= D3DXVec3Length(&vEyeDist))
	{
		if(m_pPlayer)
			m_pPlayer->Set_FirstPersonState(TRUE);
	}

	m_vNewEye = m_pTargetCom->m_vInfo[Engine::INFO::INFO_POS];
	_vec3 vDir = m_pTargetCom->m_vInfo[Engine::INFO::INFO_LOOK];
	D3DXVec3Normalize(&vDir, &vDir);
	m_vNewEye += vDir * 0.7f;
	m_vNewEye.y += 0.55f;

	m_vNewAt = _vec3(0.f, 0.f, 1.f);

	/*  Rot  */
	{
		_vec3 vRight, vUp;
		_matrix	matRotAxis;
		memcpy(&vRight, &_vec3(1.f, 0.f, 0.f), sizeof(float) * 3);
		memcpy(&vUp, &_vec3(0.f, 1.f, 0.f), sizeof(float) * 3);

		/*  RotUpdate OrderLine Important!!  */
		{
			/*  Rot UpDown  */
			D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(m_vAngle.x));
			D3DXVec3TransformNormal(&m_vNewAt, &m_vNewAt, &matRotAxis);

			/*  Rot LeftRight  */
			D3DXMatrixRotationAxis(&matRotAxis, &vUp, m_vAngle.y);
			D3DXVec3TransformNormal(&m_vNewAt, &m_vNewAt, &matRotAxis);
		}
	}

	m_vNewAt += m_vNewEye;
}

void CStaticCamera::Update_DynamicTrigger(void)
{
	if (TRUE == m_bDynamicTrigger[1])
		return;

	if (FALSE == m_bDynamicTrigger[0] 
		&& 41.f < m_pTargetCom->m_vInfo[Engine::INFO::INFO_POS].z)
	{
		m_bDynamicTrigger[0] = TRUE;

		m_bOwnerShip = FALSE;
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_OwnerShip(TRUE);
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_BEHAVIOR);
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_CameraBehavior()->m_vEye = m_vEye;
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_CameraBehavior()->m_vAt = m_vAt;
	}

	if (FALSE == m_bDynamicTrigger[1] 
		&& TRUE == m_bDynamicQuestTrigger)
	{
		m_bDynamicTrigger[1] = TRUE;

		static_cast<CCameraHierarchy*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_CAMERAHIERARCHY))->Load_CameraWave2();
		m_bOwnerShip = FALSE;
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_OwnerShip(TRUE);
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_QUEST);
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->SetViewSpaceMatrix(&m_vEye, &m_vAt, &m_vUp);

		Engine::Set_LightActive(2);
	}

	/*if (FALSE == m_bDynamicTrigger[1]
		&& 42.f < m_pTargetCom->m_vInfo[Engine::INFO::INFO_POS].z)
	{
		m_bDynamicTrigger[1] = TRUE;

		static_cast<CCameraHierarchy*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_CAMERAHIERARCHY))->Load_CameraWave2();
		m_bOwnerShip = FALSE;
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_OwnerShip(TRUE);
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_BEHAVIOR);
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_CameraBehavior()->m_vEye = m_vEye;
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_CameraBehavior()->m_vAt = m_vAt;

		static_cast<CLowerTextTuto*>(Engine::Find_ObjectListBack(Engine::OBJECT_LOWERTEXTTUTO))->Set_Render();
		Engine::Set_LightActive(2);
	}*/
}

void CStaticCamera::Update_CameraPurpose(void)
{
 	if (CAMERA_PURPOSE::PURPOSE_END == m_eCameraPurpose
		|| CAMERA_PURPOSE::PURPOSE_BEHAVIOR == m_eCameraPurpose
		|| CAMERA_PURPOSE::PURPOSE_QUEST == m_eCameraPurpose)
		return;

	switch (m_eCameraPurpose)
	{
	case CAMERA_PURPOSE::PURPOSE_DOOR:
	case CAMERA_PURPOSE::PURPOSE_TRUNK:
	case CAMERA_PURPOSE::PURPOSE_CUPBOARD:
	case CAMERA_PURPOSE::PURPOSE_GRID:
		m_bOwnerShip = FALSE;
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_OwnerShip(TRUE);
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Set_CameraPurpose(m_eCameraPurpose);
		static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->SetViewSpaceMatrix(&m_vEye, &m_vAt, &m_vUp);
		break;
	}
}

void CStaticCamera::Set_CameraPurpose(const CAMERA_PURPOSE& _ePurpose)
{
	if (CAMERA_PURPOSE::PURPOSE_FIRST_PERSON == m_eCameraPurpose)
	{
		if (m_pPlayer)
		{
			m_pPlayer->Set_FirstPersonState(FALSE);

			_vec3 vLook = m_vAt - m_vEye;
			vLook.y = 0.f;
			m_pPlayer->Set_InitPlayerLook(&vLook);
		}

		m_vNewEye = m_vOriginNewEye;
		m_vNewAt = m_vOriginNewAt;

	}

	m_eCameraPurpose = _ePurpose;

	if (CAMERA_PURPOSE::PURPOSE_FIRST_PERSON == m_eCameraPurpose)
	{
		m_vOriginNewEye = m_vNewEye;
		m_vOriginNewAt = m_vNewAt;
		//m_vAngle.x = 0.f;
		//m_vAngle.y = 0.f;
	}
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaticCamera*	pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStaticCamera::Free(void)
{
	Engine::CGameObject::Free();
}

