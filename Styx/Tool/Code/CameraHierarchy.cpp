#include "stdafx.h"
#include "CameraHierarchy.h"

#include "Export_Function.h"

#include "CameraBehavior.h"

CCameraHierarchy::CCameraHierarchy(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	
}

CCameraHierarchy::~CCameraHierarchy(void)
{

}

HRESULT CCameraHierarchy::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	FAILED_CHECK_RETURN(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine), E_FAIL);

	CToolMgr::GetInstance()->m_pCameraTool->m_pCameraHierarchy = this;

	return S_OK;
}

_int CCameraHierarchy::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	for (auto& iter : m_vecCameraEyePoint)
	{
		if(nullptr != iter)
			iter->Update_Object(fTimeDelta);
	}
	for (auto& iter : m_vecCameraAtPoint)
	{
		if (nullptr != iter)
			iter->Update_Object(fTimeDelta);
	}

	return UPDATE_OK;
}

_int CCameraHierarchy::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CCameraHierarchy::PreRender_Object(void)
{

}

void CCameraHierarchy::Render_Object(void)
{
	if (TOOLID::TOOL_CAMERA == CToolMgr::GetInstance()->m_eCurTool)
	{
		/*  CameraEyePoint  */
		if (1 != m_vecCameraEyePoint.size())
		{
			int iNum = 0;
			for (auto& iter : m_vecCameraEyePoint)
			{
				if(nullptr == iter)
					continue;

				iter->Render_Object();

				if (iter == m_vecCameraEyePoint.back())
					continue;

				CCameraPoint* pPointStart = m_vecCameraEyePoint[iNum];
				CCameraPoint* pPointEnd = m_vecCameraEyePoint[iNum + 1];

				iNum++;	

				_vec3 vLine[200];
				float fNum = 0.f;

				for (int i = 0; i < CAMERA_ITV; ++i)
				{
					_vec3 vOut;
					D3DXVec3CatmullRom(&vOut,
						&pPointStart->m_tSpline.vVirtualStartPos, &pPointStart->m_tSpline.vPos,
						&pPointEnd->m_tSpline.vPos, &pPointStart->m_tSpline.vVirtualEndPos,
						fNum);

					vLine[i] = vOut;
					fNum += 1.f / float(CAMERA_ITV);
				}
				
				/*  Line Render  */
				{
					_matrix matView, matProj;
					m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
					m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

					for (int i = 0; i < CAMERA_ITV; ++i)
					{
						D3DXVec3TransformCoord(&vLine[i], &vLine[i], &matView);

						if (vLine[i].z <= 0.1f)
							vLine[i].z = 0.1f;

						//D3DXVec3TransformCoord(&vViewPoint[i], &vLine[i], &matProj);
					}

					m_pD3DXLine->SetWidth(5.f); // 라인의 굵기를 결정하는 함수

					m_pGraphicDev->EndScene();
					m_pGraphicDev->BeginScene();

					m_pD3DXLine->Begin();

					m_pD3DXLine->DrawTransform(vLine, CAMERA_ITV, &matProj, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));

					m_pD3DXLine->End();
				}
			}
		}

		/*  CameraAtPoint  */
		if (1 != m_vecCameraAtPoint.size())
		{
			int iNum = 0;
			for (auto& iter : m_vecCameraAtPoint)
			{
				if (nullptr == iter)
					continue;

				iter->Render_Object();

				if (iter == m_vecCameraAtPoint.back())
					continue;

				CCameraPoint* pPointStart = m_vecCameraAtPoint[iNum];
				CCameraPoint* pPointEnd = m_vecCameraAtPoint[iNum + 1];

				iNum++;

				_vec3 vLine[200];
				float fNum = 0.f;

				for (int i = 0; i < CAMERA_ITV; ++i)
				{
					_vec3 vOut;
					D3DXVec3CatmullRom(&vOut,
						&pPointStart->m_tSpline.vVirtualStartPos, &pPointStart->m_tSpline.vPos,
						&pPointEnd->m_tSpline.vPos, &pPointStart->m_tSpline.vVirtualEndPos,
						fNum);

					vLine[i] = vOut;
					fNum += 1.f / float(CAMERA_ITV);
				}

				/*  Line Render  */
				{
					_matrix matView, matProj;
					m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
					m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

					for (int i = 0; i < CAMERA_ITV; ++i)
					{
						D3DXVec3TransformCoord(&vLine[i], &vLine[i], &matView);

						if (vLine[i].z <= 0.1f)
							vLine[i].z = 0.1f;

						//D3DXVec3TransformCoord(&vViewPoint[i], &vLine[i], &matProj);
					}

					m_pD3DXLine->SetWidth(5.f); // 라인의 굵기를 결정하는 함수

					m_pGraphicDev->EndScene();
					m_pGraphicDev->BeginScene();

					m_pD3DXLine->Begin();

					m_pD3DXLine->DrawTransform(vLine, CAMERA_ITV, &matProj, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

					m_pD3DXLine->End();
				}
			}
		}


	}
}

void CCameraHierarchy::PostRender_Object(void)
{
	
}

HRESULT CCameraHierarchy::Create_CameraPoint(_vec3 _vPickPos)
{
	/*  CameraEyePoint Create  */
	{
		CCameraPoint* pCameraPoint = nullptr;
		pCameraPoint = dynamic_cast<CCameraPoint*>(CAbstractFactoryPattern<CCameraPoint>::CreateObject(m_pGraphicDev));
		NULL_CHECK_RETURN(pCameraPoint, E_FAIL);

		pCameraPoint->m_ePointType = CCameraPoint::CAMERA_POINT_TYPE::CAMERA_POINT_EYE;
		pCameraPoint->m_tSpline.vPos = _vPickPos;
		pCameraPoint->m_tSpline.vPos.y += 1.f;

		_vec3 vRand;
		vRand = _vec3(float(rand() % 50 - 25.f), float(rand() % 10), float(rand() % 50 - 25.f));
		vRand = _vec3(0.f, 0.f, 0.f);
		pCameraPoint->m_tSpline.vVirtualStartPos = pCameraPoint->m_tSpline.vPos + vRand;
		vRand = _vec3(float(rand() % 50 - 25.f), float(rand() % 10), float(rand() % 50 - 25.f));
		vRand = _vec3(0.f, 0.f, 0.f);
		pCameraPoint->m_tSpline.vVirtualEndPos = pCameraPoint->m_tSpline.vPos + vRand;

		pCameraPoint->m_tSpline.fSpeed = 80.f;
		pCameraPoint->m_tSpline.fIncSpeed = 10.f;

		m_vecCameraEyePoint.push_back(pCameraPoint);

		/*  VirtualEndpos Init  */
		if (1 != m_vecCameraEyePoint.size())
		{
			int iBackNum = m_vecCameraEyePoint.size() - 2;
			vRand = _vec3(float(rand() % 50 - 25.f), float(rand() % 10), float(rand() % 50 - 25.f));
			vRand = _vec3(0.f, 0.f, 0.f);
			m_vecCameraEyePoint[iBackNum]->m_tSpline.vVirtualEndPos = pCameraPoint->m_tSpline.vVirtualStartPos + vRand;
		}

		/*  First CameraBehavior Speed Init  */
		if (1 == m_vecCameraEyePoint.size())
		{
			dynamic_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->m_pCameraBehavior->m_fSpeed = pCameraPoint->m_tSpline.fSpeed;
		}
	}

	/*  CameraAtPoint Create  */
	{
		_matrix matView;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

		D3DXMatrixInverse(&matView, nullptr, &matView);

		CCameraPoint* pCameraPoint = nullptr;
		pCameraPoint = dynamic_cast<CCameraPoint*>(CAbstractFactoryPattern<CCameraPoint>::CreateObject(m_pGraphicDev));
		NULL_CHECK_RETURN(pCameraPoint, E_FAIL);

		pCameraPoint->m_ePointType = CCameraPoint::CAMERA_POINT_TYPE::CAMERA_POINT_AT;
		pCameraPoint->m_tSpline.vPos = _vPickPos;
		pCameraPoint->m_tSpline.vPos.y += 1.f;

		/*  CameraLook Pos Plus  */
		_vec3 vCameraLook;
		memcpy(vCameraLook, &matView.m[2], sizeof(float) * 3);
		vCameraLook.y = 0.f;
		D3DXVec3Normalize(&vCameraLook, &vCameraLook);
		pCameraPoint->m_tSpline.vPos += vCameraLook * 10.f;

		_vec3 vRand;
		vRand = _vec3(float(rand() % 50 - 25.f), float(rand() % 10), float(rand() % 50 - 25.f));
		vRand = _vec3(0.f, 0.f, 0.f);
		pCameraPoint->m_tSpline.vVirtualStartPos = pCameraPoint->m_tSpline.vPos + vRand;
		vRand = _vec3(float(rand() % 50 - 25.f), float(rand() % 10), float(rand() % 50 - 25.f));
		vRand = _vec3(0.f, 0.f, 0.f);
		pCameraPoint->m_tSpline.vVirtualEndPos = pCameraPoint->m_tSpline.vPos + vRand;

		pCameraPoint->m_tSpline.fSpeed = 80.f;
		pCameraPoint->m_tSpline.fIncSpeed = 10.f;

		m_vecCameraAtPoint.push_back(pCameraPoint);

		/*  VirtualEndpos Init  */
		if (1 != m_vecCameraAtPoint.size())
		{
			int iBackNum = m_vecCameraAtPoint.size() - 2;
			vRand = _vec3(float(rand() % 50 - 25.f), float(rand() % 10), float(rand() % 50 - 25.f));
			vRand = _vec3(0.f, 0.f, 0.f);
			m_vecCameraAtPoint[iBackNum]->m_tSpline.vVirtualEndPos = pCameraPoint->m_tSpline.vVirtualStartPos + vRand;
		}
	}

	return S_OK;
}

HRESULT CCameraHierarchy::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Renderer  */
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_RENDERER, pComponent);

	return S_OK;
}

HRESULT CCameraHierarchy::SetUp_Material(void)
{
	return S_OK;
}

CCameraHierarchy* CCameraHierarchy::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CCameraHierarchy*	pInstance = new CCameraHierarchy(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCameraHierarchy::Free(void)
{
	Safe_Release(m_pD3DXLine);

	for_each(m_vecCameraEyePoint.begin(), m_vecCameraEyePoint.end(), Engine::CDeleteObj());
	m_vecCameraEyePoint.clear();

	for_each(m_vecCameraAtPoint.begin(), m_vecCameraAtPoint.end(), Engine::CDeleteObj());
	m_vecCameraAtPoint.clear();

	Engine::CGameObject::Free();
}