#include "stdafx.h"
#include "CameraPoint.h"

#include "Export_Function.h"

CCameraPoint::CCameraPoint(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	ZeroMemory(&m_tSpline, sizeof(SPLINE));
}

CCameraPoint::~CCameraPoint(void)
{

}

HRESULT CCameraPoint::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransCom->Set_Scale(0.025f, 0.025f, 0.025f);
	m_pTransCom->Set_Scale(0.01f, 0.01f, 0.01f);
	return S_OK;
}

_int CCameraPoint::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	m_pTransCom->m_vInfo[Engine::INFO::INFO_POS] = m_tSpline.vPos;

	return UPDATE_OK;
}

_int CCameraPoint::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CCameraPoint::PreRender_Object(void)
{
	CToolMgr::GetInstance()->m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
	
	FAILED_CHECK_RETURN(SetUp_Material(), );
}

void CCameraPoint::Render_Object(void)
{
	if (TOOL_CAMERA != CToolMgr::GetInstance()->m_eCurTool)
		return;
	
	PreRender_Object();

	if (CAMERA_POINT_EYE == m_ePointType)
	{
		m_pColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);

		if (TRUE == m_bSectionPick)
		{
			m_pColliderSectionCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);
			m_pColliderSectionCom->Render_Collider(&m_pTransCom->m_matWorld, FALSE);
		}
	}
	else
	{
		m_pColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);

		if (TRUE == m_bSectionPick)
		{
			m_pColliderSectionCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);
			m_pColliderSectionCom->Render_Collider(&m_pTransCom->m_matWorld, FALSE);
		}
	}
	m_pColliderCom->Render_Collider(&m_pTransCom->m_matWorld, FALSE);

	PostRender_Object();
}

void CCameraPoint::PostRender_Object(void)
{

}

HRESULT CCameraPoint::Add_Component()
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

	/*  Collider  */
	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, 0.5f, 25, 24 );
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SPHERE_COLLIDER, pComponent);

	/*  ColliderSection, No Component map  */
	pComponent = m_pColliderSectionCom = Engine::CSphereCollider::Create(m_pGraphicDev, 1.5f, 25, 24);
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	return S_OK;
}

HRESULT CCameraPoint::SetUp_Material(void)
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

CCameraPoint* CCameraPoint::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CCameraPoint*	pInstance = new CCameraPoint(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCameraPoint::Free(void)
{
	Engine::Safe_Release(m_pColliderSectionCom);

	Engine::CGameObject::Free();
}