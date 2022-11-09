#include "stdafx.h"
#include "CameraHierarchy.h"

#include "Export_Function.h"

#include "CameraBehavior.h"
#include "CameraPoint.h"

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

	FAILED_CHECK_RETURN(Load_CameraWave1(), E_FAIL);

	return S_OK;
}

_int CCameraHierarchy::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	for (auto& iter : m_vecCameraEyePoint)
		iter->Update_Object(fTimeDelta);
	for (auto& iter : m_vecCameraAtPoint)
		iter->Update_Object(fTimeDelta);

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
	return;

	/*  CameraEyePoint  */
	if (1 != m_vecCameraEyePoint.size())
	{
		int iNum = 0;
		for (auto& iter : m_vecCameraEyePoint)
		{
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

void CCameraHierarchy::PostRender_Object(void)
{
	
}

HRESULT CCameraHierarchy::Load_CameraWave1()
{
	HANDLE hFile = CreateFile(L"../../Data/CameraWave1.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(NULL, TEXT("Load Failed"), L"System Message", MB_OK);
		return E_FAIL;
	}

	DWORD dwByte = 0;

	_tchar szCameraTag[128];
	ReadFile(hFile, szCameraTag, sizeof(szCameraTag), &dwByte, nullptr);

	size_t uSize;
	ReadFile(hFile, &uSize, sizeof(size_t), &dwByte, nullptr);

	/*  CameraEye  */
	if (!lstrcmpW(szCameraTag, L"Camera_Eye"))
	{
		while (uSize)
		{
			SPLINE tSpline;
			ReadFile(hFile, &tSpline, sizeof(SPLINE), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CCameraPoint* pCameraPoint = nullptr;
			pCameraPoint = dynamic_cast<CCameraPoint*>(CAbstractFactoryPattern<CCameraPoint>::CreateObject(m_pGraphicDev));
			NULL_CHECK_RETURN(pCameraPoint, E_FAIL);

			pCameraPoint->m_ePointType = CCameraPoint::CAMERA_POINT_TYPE::CAMERA_POINT_EYE;
			memcpy(&pCameraPoint->m_tSpline, &tSpline, sizeof(SPLINE));

			m_vecCameraEyePoint.push_back(pCameraPoint);

			uSize--;
		}
	}

	ReadFile(hFile, szCameraTag, sizeof(szCameraTag), &dwByte, nullptr);
	ReadFile(hFile, &uSize, sizeof(size_t), &dwByte, nullptr);

	/*  CameraAt  */
	if (!lstrcmpW(szCameraTag, L"Camera_At"))
	{
		while (uSize)
		{
			SPLINE tSpline;
			ReadFile(hFile, &tSpline, sizeof(tSpline), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CCameraPoint* pCameraPoint = nullptr;
			pCameraPoint = dynamic_cast<CCameraPoint*>(CAbstractFactoryPattern<CCameraPoint>::CreateObject(m_pGraphicDev));
			NULL_CHECK_RETURN(pCameraPoint, E_FAIL);

			pCameraPoint->m_ePointType = CCameraPoint::CAMERA_POINT_TYPE::CAMERA_POINT_AT;
			memcpy(&pCameraPoint->m_tSpline, &tSpline, sizeof(SPLINE));

			m_vecCameraAtPoint.push_back(pCameraPoint);

			uSize--;
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CCameraHierarchy::Load_CameraWave2()
{
	if (TRUE == m_bCameraWave2)
		return E_FAIL;
	else if (FALSE == m_bCameraWave2)
		m_bCameraWave2 = TRUE;

	HANDLE hFile = CreateFile(L"../../Data/CameraWave2.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(NULL, TEXT("Load Failed"), L"System Message", MB_OK);
		return E_FAIL;
	}

	DWORD dwByte = 0;

	_tchar szCameraTag[128];
	ReadFile(hFile, szCameraTag, sizeof(szCameraTag), &dwByte, nullptr);

	size_t uSize;
	ReadFile(hFile, &uSize, sizeof(size_t), &dwByte, nullptr);

	/*  CameraEye  */
	if (!lstrcmpW(szCameraTag, L"Camera_Eye"))
	{
		while (uSize)
		{
			SPLINE tSpline;
			ReadFile(hFile, &tSpline, sizeof(SPLINE), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CCameraPoint* pCameraPoint = nullptr;
			pCameraPoint = dynamic_cast<CCameraPoint*>(CAbstractFactoryPattern<CCameraPoint>::CreateObject(m_pGraphicDev));
			NULL_CHECK_RETURN(pCameraPoint, E_FAIL);

			pCameraPoint->m_ePointType = CCameraPoint::CAMERA_POINT_TYPE::CAMERA_POINT_EYE;
			memcpy(&pCameraPoint->m_tSpline, &tSpline, sizeof(SPLINE));

			m_vecCameraEyePoint.push_back(pCameraPoint);

			uSize--;
		}
	}

	ReadFile(hFile, szCameraTag, sizeof(szCameraTag), &dwByte, nullptr);
	ReadFile(hFile, &uSize, sizeof(size_t), &dwByte, nullptr);

	/*  CameraAt  */
	if (!lstrcmpW(szCameraTag, L"Camera_At"))
	{
		while (uSize)
		{
			SPLINE tSpline;
			ReadFile(hFile, &tSpline, sizeof(tSpline), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CCameraPoint* pCameraPoint = nullptr;
			pCameraPoint = dynamic_cast<CCameraPoint*>(CAbstractFactoryPattern<CCameraPoint>::CreateObject(m_pGraphicDev));
			NULL_CHECK_RETURN(pCameraPoint, E_FAIL);

			pCameraPoint->m_ePointType = CCameraPoint::CAMERA_POINT_TYPE::CAMERA_POINT_AT;
			memcpy(&pCameraPoint->m_tSpline, &tSpline, sizeof(SPLINE));

			m_vecCameraAtPoint.push_back(pCameraPoint);

			uSize--;
		}
	}

	CloseHandle(hFile);

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
	m_vecCameraEyePoint.shrink_to_fit();
	m_vecCameraEyePoint.clear();

	for_each(m_vecCameraAtPoint.begin(), m_vecCameraAtPoint.end(), Engine::CDeleteObj());
	m_vecCameraAtPoint.shrink_to_fit();
	m_vecCameraAtPoint.clear();

	Engine::CGameObject::Free();
}