#include "stdafx.h"
#include "Cupboard.h"

#include "Export_Function.h"

CCupboard::CCupboard(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	m_eOwnerObjectID = Engine::OBJECTID::OBJECT_CUPBOARD;
	m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_CUPBOARD;
}

CCupboard::~CCupboard(void)
{

}

HRESULT CCupboard::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_AnimationSet(0);

	return S_OK;
}

_int CCupboard::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	/*  Coll False Init  */
	m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);

	return UPDATE_OK;
}

_int CCupboard::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	/*  Update State  */
	Update_State();

	return UPDATE_OK;
}

void CCupboard::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CCupboard::Render_Object(void)
{
	m_pMeshCom->Play_Animation(m_fTimeDelta);

	/*  Frustum Culling  */
	if (false == m_pOptimizationCom->Is_InFrustum_ForObject(&m_pTransCom->m_vInfo[Engine::INFO::INFO_POS], 1.5f))
		return;

	/*  CollisionMgr AddObject  */
	DWORD dwFlag = COL_FLAG_SPHERE_SOUR;
	Engine::AddObject_CollisionMgr(dwFlag, this);

	PreRender_Object();

	/*  Dynamic Mesh, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(0);

		m_pMeshCom->Render_Meshe(pEffect);

		pEffect->EndPass();
		pEffect->End();
	}

	m_pSphereColliderCom->Render_Collider(&m_pTransCom->m_matWorld);

	PostRender_Object();
}

void CCupboard::PostRender_Object(void)
{

}

_int CCupboard::CollisionUpdate_Object(const _float& fTimeDelta, 
										const DWORD _dwFlag, 
										const Engine::COL_OBJECTID _eColObjectID, 
										const _float& _fColValue /*= 0.f*/, 
										Engine::CGameObject* _pObject /*= nullptr*/)
{
	if (_dwFlag & COL_FLAG_SPHERE_SOUR)
	{
		if (Engine::COL_OBJECTID::COL_OBJECT_PLAYER == _eColObjectID)
		{
			m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);

			if (0 == m_pMeshCom->Get_AnimationIndex())
			{
				if (FALSE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CarryState())
				{
					if (CAMERA_PURPOSE::PURPOSE_CUPBOARD != static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_CameraPurpose())
					{
						static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"¼û±â");
					}
					else if (CAMERA_PURPOSE::PURPOSE_CUPBOARD == static_cast<CDynamicCamera*>(Engine::Find_DynamicCameraObject())->Get_CameraPurpose())
					{
						static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"³ª°¡±â");
					}
				}
				else if (TRUE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CarryState())
				{
					static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_MOUSE, L"½ÃÃ¼ ¼û±â±â");
				}
			}
		}
	}

	return UPDATE_OK;
}

HRESULT CCupboard::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	pEffect->SetBool("g_bSpecular", false);
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

HRESULT CCupboard::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	
	/*  Mesh  */
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, m_pMeshTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_MESH, pComponent);

	/*  Transform  */
	/*  Static Choice!!  */
	pComponent = m_pTransCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM, pComponent);
	
	/*  Renderer  */
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_RENDERER, pComponent);

	/*  Sphere Collider  */
	//pComponent = m_pSphereColliderCom = Engine::CCubeCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), m_pMeshCom->Get_Stride());
	pComponent = m_pSphereColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, 1.f, 25, 24 );
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SPHERE_COLLIDER, pComponent);

	/*  Optimization  */
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_OPTIMIZATION, pComponent);

	/*  Shader  */
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"NewShader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_NEWSHADER_MESH, pComponent);

	return S_OK;
}

void CCupboard::Update_State(void)
{
	switch (m_eState)
	{
	case STATE::IDLE:
		m_pMeshCom->Set_AnimationSet(0);
		break;

	case STATE::PLAYER_IN:
		m_pMeshCom->Set_AnimationSet(11);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
		{
			m_eState = STATE::IDLE;
			m_pMeshCom->Set_TrackPosition(0);
			m_pMeshCom->Set_AnimationSet(0, 0.f, 1.f, 0.f, 0.01f);
		}
		break;

	case STATE::PLAYER_OUT:
		m_pMeshCom->Set_AnimationSet(10);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
		{
			m_eState = STATE::IDLE;
			m_pMeshCom->Set_TrackPosition(0);
			m_pMeshCom->Set_AnimationSet(0, 0.f, 1.f, 0.f, 0.01f);
		}
		break;

	case STATE::ELITE_HIDE:
		m_pMeshCom->Set_AnimationSet(12);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
		{
			m_eState = STATE::IDLE;
			m_pMeshCom->Set_TrackPosition(0);
			m_pMeshCom->Set_AnimationSet(0, 0.f, 1.f, 0.f, 0.01f);
		}
		break;

	case STATE::SEARCH_IN:
		m_pMeshCom->Set_AnimationSet(8);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.85))
		{
			if (PLAYER_STATE::PLAYER_CUPBOARD_IN == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState()
				|| PLAYER_STATE::PLAYER_CUPBOARD_CATCH == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState())
				m_eState = STATE::CATCH;
			else
				m_eState = STATE::SEARCH_OUT;
		}
		break;

	case STATE::SEARCH_OUT:
		m_pMeshCom->Set_AnimationSet(7);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
		{
			m_eState = STATE::IDLE;
			m_pMeshCom->Set_TrackPosition(0);
			m_pMeshCom->Set_AnimationSet(0, 0.f, 1.f, 0.f, 0.01f);
		}
		break;

	case STATE::CATCH:
		m_pMeshCom->Set_AnimationSet(9);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
		{
			m_eState = STATE::IDLE;
			m_pMeshCom->Set_TrackPosition(0);
			m_pMeshCom->Set_AnimationSet(0, 0.f, 1.f, 0.f, 0.01f);
		}
		break;

	}
}

CCupboard* CCupboard::Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag)
{
	CCupboard*	pInstance = new CCupboard(_pGraphicDev);

	pInstance->Set_MeshTag(_pMeshTag);
	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCupboard::Free(void)
{
	Engine::CGameObject::Free();
}