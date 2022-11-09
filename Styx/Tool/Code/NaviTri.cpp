#include "stdafx.h"
#include "NaviTri.h"

#include "Export_Function.h"

#include "NaviPoint.h"

CNaviTri::CNaviTri(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{

}

CNaviTri::~CNaviTri(void)
{

}

HRESULT CNaviTri::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	FAILED_CHECK_RETURN(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine), E_FAIL);

	return S_OK;
}

_int CNaviTri::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	for (auto& iter : m_vecvecNaviTri)
	{
		for (auto& pointiter : iter)
		{
			if (nullptr != pointiter)
			{
				pointiter->Update_Object(fTimeDelta);

				/*  Collision  */
				if (TRUE == pointiter->m_bCreatePick || TRUE == pointiter->m_bPick)
				{ 
					for (auto& souriter : m_vecvecNaviTri)
					{
						if (iter == souriter)
							continue;

						bool bColl = FALSE;
						for (auto& sourpointiter : souriter)
						{
							if (pointiter == sourpointiter)
								continue;

							_vec3 vDist = pointiter->m_vPoint - sourpointiter->m_vPoint;

							_float fRadiusSum = pointiter->m_fRadius + sourpointiter->m_fRadius;

							if (fRadiusSum > D3DXVec3Length(&vDist))
							{
								if (TRUE == pointiter->m_bCreatePick)
								{
									pointiter->m_vPoint = sourpointiter->m_vPoint;
									bColl = TRUE;
									break;
								}
								else if (TRUE == pointiter->m_bPick)
								{
									pointiter->m_bCollTest = TRUE;
									pointiter->m_vCollSourPos = sourpointiter->m_vPoint;
									bColl = TRUE;
									break;
								}
							}
							else
								pointiter->m_bCollTest = FALSE;
						}
						if (TRUE == bColl)
							break;
					}
				}
			}
		}
	}



	//m_pTransCom->m_vInfo[Engine::INFO::INFO_POS] = m_vIndexPoint;

	return UPDATE_OK;
}

_int CNaviTri::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	for (auto& iter : m_vecvecNaviTri)
	{
		for (auto& pointiter : iter)
		{
			if (nullptr != pointiter)
				pointiter->LateUpdate_Object(fTimeDelta);
		}
	}

	return UPDATE_OK;
}

void CNaviTri::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
	
	FAILED_CHECK_RETURN(SetUp_Material(), );
}

void CNaviTri::Render_Object(void)
{
	if (TOOL_NAVIMESH != CToolMgr::GetInstance()->m_eCurTool)
		return;
	
	PreRender_Object();

	/*  Cell Line Render  */
	CellLineRender();

	/*  Cell Index Render  */
	CellIndexRender();

	PostRender_Object();
}

void CNaviTri::PostRender_Object(void)
{

}

HRESULT CNaviTri::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Transform  */
	pComponent = m_pTransCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_DYNAMIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM, pComponent);
	
	/*  Renderer  */
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_RENDERER, pComponent);

	return S_OK;
}

HRESULT CNaviTri::SetUp_Material(void)
{
	D3DMATERIAL9			tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 1.f;

	m_pGraphicDev->SetMaterial(&tMtrlInfo);

	return S_OK;
}

void CNaviTri::CellLineRender(void)
{
	for (auto& iter : m_vecvecNaviTri)
	{
		if (3 != iter.size())
			return;

		_vec3	vPoint[4];
		vPoint[0] = iter[POINT_A]->m_vPoint;
		vPoint[1] = iter[POINT_B]->m_vPoint;
		vPoint[2] = iter[POINT_C]->m_vPoint;
		vPoint[3] = iter[POINT_A]->m_vPoint;

		_matrix		matView, matProj;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

		for (_ulong i = 0; i < 4; ++i)
		{
			D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

			if (vPoint[i].z <= 0.1f)
				vPoint[i].z = 0.1f;

			D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
		}

		m_pD3DXLine->SetWidth(3.f); // 라인의 굵기를 결정하는 함수

		m_pGraphicDev->EndScene();
		m_pGraphicDev->BeginScene();

		m_pD3DXLine->Begin();

		_matrix matTemp;

		if(Engine::NAVI_TYPE::NAVI_ROAD ==  iter[POINT_A]->m_eNaviType)
			m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		else if (Engine::NAVI_TYPE::NAVI_INSIDE == iter[POINT_A]->m_eNaviType)
			m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		else if (Engine::NAVI_TYPE::NAVI_CLIFF == iter[POINT_A]->m_eNaviType)
			m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

		m_pD3DXLine->End();
	}
}

void CNaviTri::CellIndexRender(void)
{
	_ushort uIndex = 0;
	for (auto& iter : m_vecvecNaviTri)
	{
		for (auto& pointiter : iter)
		{
			if (nullptr != pointiter)
				pointiter->Render_Object();
		}

		/*  Center Index Render  */
		if (iter != m_vecvecNaviTri.back())
		{
			_bool bRenderCheck = true;
			_vec3 vecCenterPos = (iter[0]->m_vPoint + iter[1]->m_vPoint + iter[2]->m_vPoint) / 3.f;

			_matrix matView, matProj;
			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
			m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

			D3DXVec3TransformCoord(&vecCenterPos, &vecCenterPos, &matView);
			if (vecCenterPos.z < 0.f || vecCenterPos.z > 10.f)
				bRenderCheck = false;

			D3DXVec3TransformCoord(&vecCenterPos, &vecCenterPos, &matProj);

			Engine::_vec2	vecTextPos;

			if (bRenderCheck)
			{
				vecTextPos.x = vecCenterPos.x * (WINCX / 2.f) + (WINCX / 2.f);
				vecTextPos.y = -vecCenterPos.y * (WINCY / 2.f) + (WINCY / 2.f);

				TCHAR szNaviNum[32] = L"";
				swprintf_s(szNaviNum, L"%d", uIndex);

				//if (!m_bIsSelectCell)
				Engine::Render_Font(L"Font_Default", szNaviNum, &vecTextPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
				//else
				//	Engine::Render_Font(L"Font_Heavy", szNaviNum, &vecTextPos, D3DXCOLOR(0.32f, 0.15f, 0.03f, 1.f));
			}
			uIndex++;
		}
	}
}

HRESULT CNaviTri::PointCreate(const _vec3 _vPos, Engine::NAVI_TYPE _eType)
{
	if(0 == m_vecvecNaviTri.size())
		m_vecvecNaviTri.push_back(vector<CNaviPoint*>());

	/*  Point Create  */
	{
		Engine::CGameObject* pGameObject = CAbstractFactoryPattern<CNaviPoint>::CreateObject(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		m_vecvecNaviTri[m_vecvecNaviTri.size() - 1].push_back(static_cast<CNaviPoint*>(pGameObject));
		m_vecvecNaviTri[m_vecvecNaviTri.size() - 1][m_uPointIndex]->m_vPoint = _vPos;
		m_vecvecNaviTri[m_vecvecNaviTri.size() - 1][m_uPointIndex]->m_eNaviType = _eType;
		if(0 != m_uPointIndex)
			m_vecvecNaviTri[m_vecvecNaviTri.size() - 1][m_uPointIndex - 1]->m_bPick = FALSE;
		if(2 != m_uPointIndex)
			m_vecvecNaviTri[m_vecvecNaviTri.size() - 1][m_uPointIndex]->m_bPick = TRUE;
		else if (2 == m_uPointIndex)
			m_vecvecNaviTri.push_back(vector<CNaviPoint*>());
	}

	return S_OK;
}

HRESULT CNaviTri::BackDelete()
{
	for (auto& iter : m_vecvecNaviTri.back())
	{
		Engine::Safe_Release(iter);
	}
	m_vecvecNaviTri.back().clear();

	m_vecvecNaviTri.resize(m_vecvecNaviTri.size());

	return S_OK;
}

CNaviTri* CNaviTri::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CNaviTri*	pInstance = new CNaviTri(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CNaviTri::Free(void)
{
	Safe_Release(m_pD3DXLine);

	for (auto& iter : m_vecvecNaviTri)
	{
		for_each(iter.begin(), iter.end(), Engine::CDeleteObj());
		iter.clear();
		iter.shrink_to_fit();
	}
	m_vecvecNaviTri.clear();
	m_vecvecNaviTri.shrink_to_fit();

	Engine::CGameObject::Free();
}