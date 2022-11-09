#include "stdafx.h"
#include "DynamicCamera.h"

#include "CameraBehavior.h"
#include "PlayerBehavior.h"

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

	m_eSceneID = SCENEID(Engine::Get_CurScene());

	/*  Create After GetCurSceneCheck.. and -1  */
	switch (m_eSceneID)
	{
	case SCENEID::SCENE_LOGO - 1:
		//m_vEye = _vec3(0.f, 0.f, -4.5f);
		m_vEye = _vec3(0.f, 0.f, -2.5f);
		m_vAt = _vec3(0.f, 0.f, 1.f);
		m_vUp = _vec3(0.f, 1.f, 0.f);
		m_bOwnerShip = TRUE;
		break;

	case SCENEID::SCENE_STAGE - 1:
		m_vEye = _vec3(0.f, 1.f, -10.f);
		m_vAt = _vec3(0.f, 0.f, 1.f);
		m_vUp = _vec3(0.f, 1.f, 0.f);
		m_bOwnerShip = FALSE;

		/*  Player  */
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Find_PlayerObject());
		dynamic_cast<CPlayerBehavior*>(pPlayer->Get_PlayerBehavior())->Set_DynamicCamera(this);

		/*  CameraBehavior Create  */
		m_pCameraBehavior = CCameraBehavior::Create(m_pGraphicDev);

		/*  StaticCamera Pointer  */
		m_pStaticCamera = dynamic_cast<CStaticCamera*>(Engine::Find_StaticCameraObject());

		/*  Light Point Init  */
		{
			D3DLIGHT9			tLightInfo;
			ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

			ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
			tLightInfo.Type = D3DLIGHT_POINT;

			tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
			tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
			tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

			tLightInfo.Range = 15.f;
			FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, Engine::CLightMgr::GetInstance()->Get_LightSize()), E_FAIL);
			Engine::Set_LightActive(2, FALSE);
		}

		break;
	}


	/*  VIEW Transform  */
	CCamera::TransformViewSpaceMatrix();

	/*  PROJECTION Set, Transform  */
	CCamera::SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 0.1f, 1000.f);
	CCamera::TransformProjectionMatrix();

	return S_OK;
}

_int CDynamicCamera::Update_Object(const _float& fTimeDelta)
{
	switch(m_eSceneID)
	{
	case SCENEID::SCENE_STAGE - 1:

		switch (m_eCameraPurpose)
		{
		case CAMERA_PURPOSE::PURPOSE_BEHAVIOR:
			_vec3 vDir = m_pCameraBehavior->m_vAt - m_pCameraBehavior->m_vEye;
			vDir.y = 0.f;
			D3DXVec3Normalize(&vDir, &vDir);
			Engine::Get_LightInfo(2)->Position = m_pCameraBehavior->m_vEye + vDir * 3.5f;
			Engine::Get_LightInfo(2)->Position.y += 1.f;
			break;
		}
	}

	return UPDATE_OK;
}

_int CDynamicCamera::LateUpdate_Object(const _float& fTimeDelta)
{
	if (FALSE == m_bOwnerShip)
		return UPDATE_OK;

	switch (m_eSceneID)
	{
	case SCENEID::SCENE_LOGO - 1:
		LogoSceneUpdate(fTimeDelta);

		/*  VIEW Transform  */
		CCamera::TransformViewSpaceMatrix();
		break;

	case SCENEID::SCENE_STAGE - 1:

		switch (m_eCameraPurpose)
		{
		case CAMERA_PURPOSE::PURPOSE_BEHAVIOR:
			Update_PurposeBehavior(fTimeDelta);

			if (TRUE == m_bCameraPurposeExit)
				Engine::Set_LightActive(2, false);
			break;

		case CAMERA_PURPOSE::PURPOSE_QUEST:
			Update_PurposeQuest(fTimeDelta);
			break;

		case CAMERA_PURPOSE::PURPOSE_DOOR:
			Update_PurposeDoor(fTimeDelta);
			break;

		case CAMERA_PURPOSE::PURPOSE_TRUNK:
			Update_PurposeTrunk(fTimeDelta);
			break;

		case CAMERA_PURPOSE::PURPOSE_CUPBOARD:
			Update_PurposeCupboard(fTimeDelta);
			break;

		case CAMERA_PURPOSE::PURPOSE_GRID:
			Update_PurposeGrid(fTimeDelta);
			break;

		}

		if (TRUE == m_bCameraPurposeExit)
			Exit_CameraPurpose(fTimeDelta);

		if (FALSE == m_bCameraPurposeStart
			&& FALSE == m_bCameraPurposeExit)
			TargetRenewal(fTimeDelta);

		break;
	}

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

void CDynamicCamera::TargetRenewal(const _float& fTimeDelta)
{
	if (DIR::DIR_FRONT == m_eDir)
		m_vAt = _vec3(0.f, 0.f, 1.f);
	if (DIR::DIR_BACK == m_eDir)
		m_vAt = _vec3(0.f, 0.f, -1.f);
	if (DIR::DIR_LEFT == m_eDir)
		m_vAt = _vec3(-1.f, 0.f, 0.f);
	if (DIR::DIR_RIGHT == m_eDir)
		m_vAt = _vec3(1.f, 0.f, 0.f);

	_long	dwMouse = 0;

	/*  Mouse LeftRight  */
	if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_X))
		m_vAngle.y += dwMouse / 9.f * fTimeDelta;

	/*  Mouse UpDown  */
	if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_Y))
	{
		m_vAngle.x += dwMouse * 2.f * fTimeDelta;
	}

	/*  Rot  */
	{
		D3DXVECTOR3	vRight, vUp;
		D3DXMATRIX	matRotAxis;

		memcpy(&vRight, &_vec3(1.f, 0.f, 0.f), sizeof(float) * 3);
		memcpy(&vUp, &_vec3(0.f, 1.f, 0.f), sizeof(float) * 3);

		if (CAMERA_PURPOSE::PURPOSE_TRUNK == m_eCameraPurpose)
		{
			if (0 == m_uColNum)
				memcpy(&vRight, &_vec3(0.f, 0.f, -1.f), sizeof(float) * 3);
			else if (1 == m_uColNum)
				memcpy(&vRight, &_vec3(-1.f, 0.f, 0.f), sizeof(float) * 3);
		}
		else if (CAMERA_PURPOSE::PURPOSE_CUPBOARD == m_eCameraPurpose)
		{
			if (0 == m_uColNum)
				memcpy(&vRight, &_vec3(0.f, 0.f, -1.f), sizeof(float) * 3);
			else if (1 == m_uColNum)
				memcpy(&vRight, &_vec3(-1.f, 0.f, 0.f), sizeof(float) * 3);
			else if (2 == m_uColNum)
				memcpy(&vRight, &_vec3(0.f, 0.f, -1.f), sizeof(float) * 3);
		}
		
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

	m_vAt += m_vEye;

	/*  VIEW Transform  */
	CCamera::TransformViewSpaceMatrix();
}

void CDynamicCamera::LogoSceneUpdate(const _float& fTimeDelta)
{
	_vec3 vDestZ(0.f, 1.5f, -4.5f);

	_vec3 vDirZ = vDestZ - m_vEye;
	D3DXVec3Normalize(&vDirZ, &vDirZ);

	if (-4.5f < m_vEye.z)
		m_vEye.z += vDirZ.z * fTimeDelta;
	else
		m_vEye.z = -4.5f;

	if (TRUE == m_bTrailer)
	{
		_vec3 vDestY(0.f, 2.f, 5.f);

		_vec3 vDirY = vDestY - m_vAt;
		D3DXVec3Normalize(&vDirY, &vDirY);

		m_vEye.y = m_vAt.y += vDirY.y * 1.65f * fTimeDelta;
	}
}

void CDynamicCamera::Start_CameraPurpose(const DIR& _eDir, _vec3& _vPos, const Engine::_float& fTimeDelta /*= 0.f*/)
{
	_vec3 vEyeDir = _vPos - m_vEye;

	m_eDir = _eDir;

	if (DIR::DIR_FRONT == m_eDir)
		_vPos.z += 1.f;
	if (DIR::DIR_BACK == m_eDir)
		_vPos.z -= 1.f;
	if (DIR::DIR_LEFT == m_eDir)
		_vPos.x -= 1.f;
	if (DIR::DIR_RIGHT == m_eDir)
		_vPos.x += 1.f;

	_vec3 vAtDir = _vPos - m_vAt;

	if (0.015f < D3DXVec3Length(&vEyeDir) || 0.015f < D3DXVec3Length(&vAtDir))
	{
		float fEyePower = D3DXVec3Length(&vEyeDir) * 4.5f;
		float fAtPower = D3DXVec3Length(&vAtDir) * 4.5f;
		m_vEye += fEyePower * (*D3DXVec3Normalize(&vEyeDir, &vEyeDir)) * fTimeDelta;
		m_vAt += fAtPower * (*D3DXVec3Normalize(&vAtDir, &vAtDir)) * fTimeDelta;
	}
	else
		m_bCameraPurposeStart = FALSE;

	/*  VIEW Transform  */
	CCamera::TransformViewSpaceMatrix();
}

void CDynamicCamera::Exit_CameraPurpose(const Engine::_float& fTimeDelta /*= 0.f*/)
{
	/*  PROJECTION Set, Transform  */
	CCamera::SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 0.1f, 1000.f);
	CCamera::TransformProjectionMatrix();

	/*  Blind Render False  */
	static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(FALSE);

	ZeroMemory(&m_vAngle, sizeof(_vec3));

	_vec3 vEyeDir = m_pStaticCamera->Get_Eye() - m_vEye;
	_vec3 vAtDir = m_pStaticCamera->Get_At() - m_vAt;

	if (0.015f < D3DXVec3Length(&vEyeDir) || 0.015f < D3DXVec3Length(&vAtDir))
	{
		float fEyePower = D3DXVec3Length(&vEyeDir) * 4.f;
		float fAtPower = D3DXVec3Length(&vAtDir) * 4.f;
		m_vEye += fEyePower * (*D3DXVec3Normalize(&vEyeDir, &vEyeDir)) * fTimeDelta;
		m_vAt += fAtPower * (*D3DXVec3Normalize(&vAtDir, &vAtDir)) * fTimeDelta;
	}
	else
	{
		m_bOwnerShip = FALSE;

		if (CAMERA_PURPOSE::PURPOSE_DOOR == m_eCameraPurpose)
			static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_NextState(PLAYER_STATE::PLAYER_IDLE);
		m_eCameraPurpose = CAMERA_PURPOSE::PURPOSE_END;

		m_bCameraPurposeStart = TRUE;
		m_bCameraPurposeExit = FALSE;

		m_pStaticCamera->Set_OwnerShip(TRUE);
		m_pStaticCamera->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_END);

		m_pColTransform = nullptr;
		m_uColNum = 0;
	}

	/*  VIEW Transform  */
	CCamera::TransformViewSpaceMatrix();
}

void CDynamicCamera::Update_PurposeBehavior(const _float& fTimeDelta)
{
	m_bPlayBehavior = _bool(m_pCameraBehavior->Update_Component(fTimeDelta));

	if (FALSE == m_bPlayBehavior)
	{
		static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_OwnerShip(TRUE);
		static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_END);
		m_bOwnerShip = FALSE;

		if (SERVANT_STATE::SERVANT_VIEW == static_cast<CServant_Quest*>(Engine::Find_ObjectListBack(Engine::OBJECT_SERVANT_QUEST))->Get_State())
		{
			static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_PlayerBehavior()->Set_CameraPurpose(FALSE);
			static_cast<CServant_Quest*>(Engine::Find_ObjectListBack(Engine::OBJECT_SERVANT_QUEST))->Set_CameraPurpose(FALSE);
			static_cast<CTimer*>(Engine::Find_ObjectListBack(Engine::OBJECT_TIMER))->Set_TimerActive();
			Engine::Set_LightActive(2, false);
		}
	}
}

void CDynamicCamera::Update_PurposeQuest(const _float& fTimeDelta)
{
	if (TRUE == m_bCameraPurposeStart)
	{
		_vec3 vPos = m_pCameraBehavior->m_pCameraHierarchy->m_vecCameraEyePoint[0]->m_tSpline.vPos;

		_vec3 vEyeDir = vPos - m_vEye;

		if (0.155f < D3DXVec3Length(&vEyeDir))
		{
			float fEyePower = D3DXVec3Length(&vEyeDir) * 1.3f;
			m_vEye += fEyePower * (*D3DXVec3Normalize(&vEyeDir, &vEyeDir)) * fTimeDelta;
		}
		else
		{
			m_eCameraPurpose = CAMERA_PURPOSE::PURPOSE_BEHAVIOR;
			m_pCameraBehavior->m_vEye = m_vEye;
			m_pCameraBehavior->m_vAt = m_vAt;
			static_cast<CLowerTextTuto*>(Engine::Find_ObjectListBack(Engine::OBJECT_LOWERTEXTTUTO))->Set_Render();
		}

		/*  VIEW Transform  */
		CCamera::TransformViewSpaceMatrix();
	}
}

void CDynamicCamera::Update_PurposeDoor(const _float & fTimeDelta)
{
	/*  PROJECTION Set, Transform  */
	CCamera::SetProjectionMatrix(D3DXToRadian(25.f), float(WINCX) / WINCY, 0.1f, 1000.f);
	CCamera::TransformProjectionMatrix();

	/*  Blind Render True  */
	static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(TRUE);
	static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Index(0);

	if (TRUE == m_bCameraPurposeStart
		&& FALSE == m_bCameraPurposeExit)
	{
		//Engine::CTransform* pTransform = static_cast<Engine::CTransform*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_DOOR)->Get_Component(Engine::COM_TRANSFORM));
		Engine::CTransform* pTransform = static_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COM_TRANSFORM));

		_vec3 vPos = pTransform->m_vInfo[Engine::INFO::INFO_POS];
		vPos.x -= 0.05f;
		vPos.y += 1.3f;
		vPos.z += 1.f;

		m_vEye.x = vPos.x;
		m_vEye.y = vPos.y;

		Start_CameraPurpose(DIR::DIR_FRONT, vPos, fTimeDelta);
	}
}

void CDynamicCamera::Update_PurposeTrunk(const _float & fTimeDelta)
{
	/*  Blind Render True  */
	static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(TRUE);
	static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Index(1);

	if (TRUE == m_bCameraPurposeStart
		&& FALSE == m_bCameraPurposeExit)
	{
		if (nullptr == m_pColTransform)
		{
			for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_TRUNK))
			{
				if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
				{
					m_pColTransform = static_cast<Engine::CTransform*>(iter->Get_Component(Engine::COM_TRANSFORM));
					break;
				}
				else
					m_uColNum++;
			}
		}

		_vec3 vPos = m_pColTransform->m_vInfo[Engine::INFO::INFO_POS];

		if (0 == m_uColNum)
		{
			vPos.x += 1.f;
			vPos.y += 0.8f;

			Start_CameraPurpose(DIR::DIR_RIGHT, vPos, fTimeDelta);
		}
		else if (1 == m_uColNum)
		{
			vPos.y += 0.8f;
			vPos.z -= 1.f;

			Start_CameraPurpose(DIR::DIR_BACK, vPos, fTimeDelta);
		}

		_vec3 vCamPos = m_pColTransform->m_vInfo[Engine::INFO::INFO_POS];
		vCamPos.y += 2.f;
		m_pStaticCamera->Set_At(&m_pColTransform->m_vInfo[Engine::INFO::INFO_POS]);

		vCamPos = m_pStaticCamera->Get_Eye();
		vCamPos.y = m_pColTransform->m_vInfo[Engine::INFO::INFO_POS].y + 3.f;
		m_pStaticCamera->Set_Eye(&vCamPos);
	}

	if (TRUE == m_bCameraPurposeExit)
	{
		Engine::CTransform* pPlayerTransform = static_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COM_TRANSFORM));
		_vec3 vNewPos = m_pColTransform->m_vInfo[Engine::INFO::INFO_POS];

		if (PLAYER_STATE::PLAYER_TRUNK_OUT == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState()
			|| PLAYER_STATE::PLAYER_IDLE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState())
		{
			if (PLAYER_STATE::PLAYER_TRUNK_OUT == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState())
				vNewPos.y += 0.25f;

			if (0 == m_uColNum)
			{
				if (PLAYER_STATE::PLAYER_IDLE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState())
					vNewPos.x += 1.5f;

				pPlayerTransform->m_vInfo[Engine::INFO::INFO_LOOK] = _vec3(1.f, 0.f, 0.f);
			}
			else if (1 == m_uColNum)
			{
				if (PLAYER_STATE::PLAYER_IDLE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState())
					vNewPos.z -= 1.5f;

				pPlayerTransform->m_vInfo[Engine::INFO::INFO_LOOK] = _vec3(0.f, 0.f, -1.f);
			}

			pPlayerTransform->Set_Pos(&vNewPos);
			static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_NewPos(vNewPos);
			static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_InitPlayerLook(&pPlayerTransform->m_vInfo[Engine::INFO::INFO_LOOK]);
		}
	}
}

void CDynamicCamera::Update_PurposeCupboard(const _float & fTimeDelta)
{
	/*  Blind Render True  */
	static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(TRUE);
	static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Index(2);

	if (TRUE == m_bCameraPurposeStart
		&& FALSE == m_bCameraPurposeExit)
	{
		if (nullptr == m_pColTransform)
		{
			for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_CUPBOARD))
			{
				if (Engine::COL_TRUE == static_cast<Engine::CCollider*>(iter->Get_Component(Engine::COM_SPHERE_COLLIDER))->Get_IsCollide())
				{
					m_pColTransform = static_cast<Engine::CTransform*>(iter->Get_Component(Engine::COM_TRANSFORM));
					break;
				}
				else
					m_uColNum++;
			}
		}

		_vec3 vPos = m_pColTransform->m_vInfo[Engine::INFO::INFO_POS];

		if (0 == m_uColNum)
		{
			vPos.x += 1.f;
			vPos.y += 1.1f;

			Start_CameraPurpose(DIR::DIR_RIGHT, vPos, fTimeDelta);
		}
		else if (1 == m_uColNum)
		{
			vPos.y += 1.1f;
			vPos.z -= 1.f;

			Start_CameraPurpose(DIR::DIR_BACK, vPos, fTimeDelta);
		}
		else if (2 == m_uColNum)
		{
			vPos.x += 1.f;
			vPos.y += 1.1f;

			Start_CameraPurpose(DIR::DIR_RIGHT, vPos, fTimeDelta);
		}

		_vec3 vCamPos = m_pColTransform->m_vInfo[Engine::INFO::INFO_POS];
		vCamPos.y += 2.6f;
		m_pStaticCamera->Set_At(&m_pColTransform->m_vInfo[Engine::INFO::INFO_POS]);

		vCamPos = m_pStaticCamera->Get_Eye();
		vCamPos.y = m_pColTransform->m_vInfo[Engine::INFO::INFO_POS].y + 3.f;
		m_pStaticCamera->Set_Eye(&vCamPos);
	}

	if (TRUE == m_bCameraPurposeExit)
	{
		Engine::CTransform* pPlayerTransform = static_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COM_TRANSFORM));
		_vec3 vNewPos = m_pColTransform->m_vInfo[Engine::INFO::INFO_POS];

		if (PLAYER_STATE::PLAYER_CUPBOARD_OUT == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState()
			|| PLAYER_STATE::PLAYER_IDLE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState())
		{
			if(PLAYER_STATE::PLAYER_CUPBOARD_OUT == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState())
				vNewPos.y += 0.25f;

			if (0 == m_uColNum)
			{
				if (PLAYER_STATE::PLAYER_IDLE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState())
					vNewPos.x += 1.f;

				pPlayerTransform->m_vInfo[Engine::INFO::INFO_LOOK] = _vec3(1.f, 0.f, 0.f);
			}
			else if (1 == m_uColNum)
			{
				if (PLAYER_STATE::PLAYER_IDLE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState())
					vNewPos.z -= 1.f;

				pPlayerTransform->m_vInfo[Engine::INFO::INFO_LOOK] = _vec3(0.f, 0.f, -1.f);
			}
			else if (2 == m_uColNum)
			{
				if (PLAYER_STATE::PLAYER_IDLE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState())
					vNewPos.x += 1.f;

				pPlayerTransform->m_vInfo[Engine::INFO::INFO_LOOK] = _vec3(1.f, 0.f, 0.f);
			}

			pPlayerTransform->Set_Pos(&vNewPos);
			static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_NewPos(vNewPos);
			static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_InitPlayerLook(&pPlayerTransform->m_vInfo[Engine::INFO::INFO_LOOK]);
		}
	}
}

void CDynamicCamera::Update_PurposeGrid(const _float& fTimeDelta)
{
	if (TRUE == m_bCameraPurposeStart)
	{
		_vec3 vPos = static_cast<Engine::CTransform*>(Engine::Find_ObjectListFront(Engine::OBJECT_GRID)->Get_Component(Engine::COM_TRANSFORM))->Get_InfoPos();
		vPos.y += 3.f;
		_vec3 vAtDir = vPos - m_vAt;

		vPos.x -= 8.f;
		vPos.y -= 1.5f;
		_vec3 vEyeDir = vPos - m_vEye;

		if (0.15f < D3DXVec3Length(&vEyeDir) || 0.015f < D3DXVec3Length(&vAtDir))
		{
			float fEyePower = D3DXVec3Length(&vEyeDir) * 3.f;
			float fAtPower = D3DXVec3Length(&vAtDir) * 3.f;
			m_vEye += fEyePower * (*D3DXVec3Normalize(&vEyeDir, &vEyeDir)) * fTimeDelta;
			m_vAt += fAtPower * (*D3DXVec3Normalize(&vAtDir, &vAtDir)) * fTimeDelta;
		}
		else
		{
			if (FALSE == static_cast<CGrid*>(Engine::Find_ObjectListFront(Engine::OBJECT_GRID))->Get_Active())
			{
				m_bCameraPurposeStart = FALSE;
				m_bCameraPurposeExit = TRUE;
			}
		}
	}

	/*  VIEW Transform  */
	CCamera::TransformViewSpaceMatrix();
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

