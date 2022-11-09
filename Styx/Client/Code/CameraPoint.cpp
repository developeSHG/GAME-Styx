#include "stdafx.h"
#include "CameraPoint.h"

#include "Export_Function.h"

using namespace Engine;

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

	m_pTransCom->Set_Scale(0.01f, 0.01f, 0.01f);

	return S_OK;
}

Engine::_int CCameraPoint::Update_Object(const Engine::_float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	m_pTransCom->m_vInfo[Engine::INFO::INFO_POS] = m_tSpline.vPos;

	return UPDATE_OK;
}

Engine::_int CCameraPoint::LateUpdate_Object(const Engine::_float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CCameraPoint::PreRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CCameraPoint::Render_Object(void)
{
	return;

	PreRender_Object();

	if (CAMERA_POINT_EYE == m_ePointType)
	{
		m_pColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);
	}
	else
	{
		m_pColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);
	}
	m_pColliderCom->Render_Collider(&m_pTransCom->m_matWorld, FALSE);

	PostRender_Object();
}

void CCameraPoint::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
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
	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, 1.f, 25, 24 );
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SPHERE_COLLIDER, pComponent);

	return S_OK;
}

HRESULT CCameraPoint::SetUp_Material(void)
{
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
	Engine::CGameObject::Free();
}