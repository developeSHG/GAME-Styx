#include "stdafx.h"
#include "NaviPoint.h"

#include "Export_Function.h"

CNaviPoint::CNaviPoint(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{

}

CNaviPoint::~CNaviPoint(void)
{

}

HRESULT CNaviPoint::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CNaviPoint::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	m_pTransCom->m_vInfo[Engine::INFO::INFO_POS] = m_vPoint;

	return UPDATE_OK;
}

_int CNaviPoint::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	m_bCreatePick = FALSE;

	if (FALSE == m_bPick)
	{
		if (TRUE == m_bCollTest)
		{
			m_vPoint = m_vCollSourPos;
			m_bCollTest = FALSE;
		}
	}

	return UPDATE_OK;
}

void CNaviPoint::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
	
	FAILED_CHECK_RETURN(SetUp_Material(), );
}

void CNaviPoint::Render_Object(void)
{
	if (TOOL_NAVIMESH != CToolMgr::GetInstance()->m_eCurTool)
		return;
	
	PreRender_Object();

	if (FALSE == m_bPick)
	{
		m_pColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);
	}
	else
	{
		m_pColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);
	}

	m_pColliderCom->Render_Collider(&m_pTransCom->m_matWorld, TRUE);

	PostRender_Object();
}

void CNaviPoint::PostRender_Object(void)
{

}

HRESULT CNaviPoint::Add_Component()
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
	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, m_fRadius, 13, 13 );
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SPHERE_COLLIDER, pComponent);

	return S_OK;
}

HRESULT CNaviPoint::SetUp_Material(void)
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

CNaviPoint* CNaviPoint::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CNaviPoint*	pInstance = new CNaviPoint(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CNaviPoint::Free(void)
{
	Engine::CGameObject::Free();
}