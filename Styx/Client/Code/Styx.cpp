#include "stdafx.h"
#include "Styx.h"

#include "Export_Function.h"

CStyx::CStyx(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{

}

CStyx::~CStyx(void)
{

}

HRESULT CStyx::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pMeshCom->Set_AnimationSet(78);
	m_pMeshCom->Set_AnimationSet(99);

	//m_pTransCom->Move_Pos(&_vec3(TERRAIN_NUM_X >> 1, 0.f, TERRAIN_NUM_Z / 2.9f - 8.f));
	//m_pTransCom->Set_Pos(-1.f, -1.25f, -0.85f);
	m_pTransCom->Set_Pos(-1.f, -1.1f, -0.85f);
	m_pTransCom->Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(180.f));
	m_pTransCom->Set_Scale(0.0195f, 0.0195f, 0.0195f);

	return S_OK;
}

_int CStyx::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	if (Engine::Get_DIKeyDown(DIK_RETURN))
	{
		m_pMeshCom->Set_AnimationSet(172);
		m_bWalk = TRUE;
	}
	//if (true == m_pMeshCom->Is_AnimationSetEnd())
	{
		//m_pMeshCom->Set_AnimationSet(217);
	}

	if (TRUE == m_bWalk)
	{
		m_pTransCom->Move_Pos(&(_vec3(0.f, 0.f, -1.4f) * fTimeDelta));

		if(-3.5f > m_pTransCom->m_vInfo[Engine::INFO_POS].z)
			SCAST<CDynamicCamera*>(Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_DYNAMICCAMERA)->back())->Set_Trailer(TRUE);
	}

	return UPDATE_OK;
}

_int CStyx::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CStyx::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	FAILED_CHECK_RETURN(SetUp_Material(), );
}

void CStyx::Render_Object(void)
{
	PreRender_Object();

	m_pMeshCom->Play_Animation(m_fTimeDelta);
	m_pMeshCom->Render_Meshe();

	PostRender_Object();
}

void CStyx::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CStyx::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Mesh  */
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Styx"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_MESH, pComponent);

	/*  Transform  */
	/*  Dynamic Choice!!  */
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

HRESULT CStyx::SetUp_Material(void)
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

CStyx* CStyx::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CStyx*	pInstance = new CStyx(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStyx::Free(void)
{
	Engine::CGameObject::Free();
}