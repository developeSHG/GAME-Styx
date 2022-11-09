#include "stdafx.h"
#include "Monster.h"

#include "Export_Function.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	//m_eOwnerObjectID = Engine::OBJECTID::OBJECT_MONSTER;
}

CMonster::~CMonster(void)
{
	
}

HRESULT CMonster::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);
 
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  ObjectMgr Get Player and Get Component  */
	m_pTargetCom = dynamic_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));
	NULL_CHECK_RETURN(m_pTargetCom, E_FAIL);

	m_pTransCom->m_vInfo[Engine::INFO::INFO_POS] = _vec3(0.f, 0.f, 5.f);

	return S_OK;
}

_int CMonster::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_PRIORITY, this);

	TargetMove(fTimeDelta);

	return UPDATE_OK;
}

_int CMonster::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CMonster::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CMonster::Render_Object(void)
{
	PreRender_Object();

	m_pBufferCom->Render_Buffer();

	PostRender_Object();
}

void CMonster::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonster::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CTriCol*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_TriCol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	/*pComponent = Engine::Clone(RESOURCE_STAGE, L"Texture_Player");
	m_pTextureCom = dynamic_cast<Engine::CTexture*>(pComponent);
	m_mapComponent.emplace(L"Com_Texture", m_pTextureCom);*/

	/*  Transform  */
	pComponent = m_pTransCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	/*  If TargetCom True, COMPONENTID is ID_STATIC  */
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM, pComponent);

	/*  Renderer  */
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_RENDERER, pComponent);

	return S_OK;
}

void CMonster::TargetMove(const _float& fTimeDelta)
{
	_vec3 vLength = m_pTargetCom->m_vInfo[Engine::INFO::INFO_POS] - m_pTransCom->m_vInfo[Engine::INFO::INFO_POS];

	if (0.5f < D3DXVec3Length(&vLength))
		m_pTransCom->Chase_Target(&m_pTargetCom->m_vInfo[Engine::INFO::INFO_POS], m_fSpeed * fTimeDelta);

}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CMonster*	pInstance = new CMonster(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMonster::Free(void)
{
	Engine::CGameObject::Free();
}