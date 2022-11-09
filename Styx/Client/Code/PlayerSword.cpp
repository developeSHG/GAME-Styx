#include "stdafx.h"
#include "PlayerSword.h"

CPlayerSword::CPlayerSword(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	m_eOwnerObjectID = Engine::OBJECTID::OBJECT_ELITE;
	m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_SWORD;
}

CPlayerSword::~CPlayerSword(void)
{

}

HRESULT CPlayerSword::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (nullptr == m_pParentBoneMatrix)
	{
		m_pPlayerDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(m_pPlayer->Get_Component(Engine::COMPONENTTYPE::COM_MESH_ELITE));
		NULL_CHECK_RETURN(m_pPlayerDynamicMeshCom, E_FAIL);

		//const Engine::D3DXFRAME_DERIVED*		pFrame = m_pPlayerDynamicMeshCom->Get_FrameByName("Bip001-R-Finger1");
		const Engine::D3DXFRAME_DERIVED*		pFrame = m_pPlayerDynamicMeshCom->Get_FrameByName("Bip001-sheath_L");
		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		m_pPlayerTransCom = dynamic_cast<Engine::CTransform*>(m_pPlayer->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM_ELITE));
		NULL_CHECK_RETURN(m_pPlayerTransCom, E_FAIL);

		m_pParentWorldMatrix = m_pPlayerTransCom->Get_WorldMatrixPointer();

		m_pTransCom->m_vScale = _vec3(-1.f, -1.f, -1.f);
		
		/*  Sheath  */
		m_pTransCom->m_vAngle.z = 30.f;
		m_pTransCom->m_vAngle.y = 30.f;
	}

	return S_OK;
}

_int CPlayerSword::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	_matrix matWorld = (*m_pParentBoneMatrix) * (*m_pParentWorldMatrix);
	
	m_pTransCom->Set_ParentMatrix(&(matWorld));

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	/*  CollisionMgr AddObject  */
	//DWORD dwFlag = COL_FLAG_SPHERE_SOUR;
	//Engine::AddObject_CollisionMgr(dwFlag, this);

	/*  Coll False Init  */
	m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);

	return UPDATE_OK;
}

_int CPlayerSword::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CPlayerSword::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CPlayerSword::Render_Object(void)
{
	PreRender_Object();

	/*  Static Mesh, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		
		m_pMeshCom->Render_Meshe(pEffect);

		pEffect->End();
	}

	m_pSphereColliderCom->Render_Collider(&m_pTransCom->m_matWorld, FALSE);

	PostRender_Object();
}

void CPlayerSword::PostRender_Object(void)
{

}

_int CPlayerSword::CollisionUpdate_Object(const _float& fTimeDelta, 
										const DWORD _dwFlag, 
										const Engine::COL_OBJECTID _eColObjectID, 
										const _float& _fColValue /*= 0.f*/, 
										Engine::CGameObject* _pObject /*= nullptr*/)
{
	if (_dwFlag & COL_FLAG_SPHERE_SOUR)
	{
		m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);
	}

	return UPDATE_OK;
}

HRESULT CPlayerSword::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo();
	NULL_CHECK_RETURN(pLightInfo, E_FAIL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetFloat("g_fAlpha", 1.f);
	pEffect->SetBool("g_bSpecular", true);
	pEffect->SetBool("g_bCatsleIn", true);

	D3DMATERIAL9			tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 1.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	Safe_Release(pEffect);

	return S_OK;
}

void CPlayerSword::Set_Born(const BORN& _eBorn)
{
	if (_eBorn != m_eBorn)
	{
		m_eBorn = _eBorn;

		const Engine::D3DXFRAME_DERIVED* pFrame = nullptr;

		switch (m_eBorn)
		{
		case BORN::FINGER:
			pFrame = m_pPlayerDynamicMeshCom->Get_FrameByName("Bip001-R-Finger1");
			m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

			m_pTransCom->m_vAngle.z = 0.f;
			m_pTransCom->m_vAngle.y = 0.f;
			break;

		case BORN::SHEATH:
			pFrame = m_pPlayerDynamicMeshCom->Get_FrameByName("Bip001-sheath_L");
			m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

			m_pTransCom->m_vAngle.z = 30.f;
			m_pTransCom->m_vAngle.y = 30.f;
			break;

		}
	}
}

HRESULT CPlayerSword::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	
	/*  Mesh  */
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Sword"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_MESH, pComponent);

	/*  Transform  */
	pComponent = m_pTransCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_DYNAMIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM, pComponent);
	
	/*  Renderer  */
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_RENDERER, pComponent);

	/*  Sphere Collider  */
	//pComponent = m_pSphereColliderCom = Engine::CCubeCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), m_pMeshCom->Get_Stride());
	pComponent = m_pSphereColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, 0.12f, 25, 24);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SPHERE_COLLIDER, pComponent);

	/*  Shader  */
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"NewShader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_NEWSHADER_MESH, pComponent);

	return S_OK;
}

CPlayerSword* CPlayerSword::Create(LPDIRECT3DDEVICE9 _pGraphicDev, CPlayer* _pPlayer)
{
	CPlayerSword*	pInstance = new CPlayerSword(_pGraphicDev);

	pInstance->Set_ParentPlayer(_pPlayer);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayerSword::Free(void)
{
	Engine::CGameObject::Free();
}