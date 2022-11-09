#include "stdafx.h"
#include "Servant_Quest.h"

#include "Export_Function.h"

CServant_Quest::CServant_Quest(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	m_eOwnerObjectID = Engine::OBJECTID::OBJECT_SERVANT_QUEST;
	m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_SERVANT_QUEST;
}

CServant_Quest::~CServant_Quest(void)
{

}

HRESULT CServant_Quest::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_AnimationSet(68);

	return S_OK;
}

_int CServant_Quest::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	/*  Coll False Init  */
	m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);

	return UPDATE_OK;
}

_int CServant_Quest::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	/*  KeyInput  */
	KeyInput();

	/*  Update State  */
	Update_State();

	return UPDATE_OK;
}

void CServant_Quest::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CServant_Quest::Render_Object(void)
{
	/*  Frustum Culling  */
	if (false == m_pOptimizationCom->Is_InFrustum_ForObject(&m_pTransCom->m_vInfo[Engine::INFO::INFO_POS], 2.f))
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

		pEffect->SetBool("g_bIamCatsle", true);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(0);

		m_pMeshCom->Play_Animation(m_fTimeDelta);
		m_pMeshCom->Render_Meshe(pEffect);

		pEffect->SetBool("g_bIamCatsle", false);
		pEffect->EndPass();
		pEffect->End();
	}

	m_pSphereColliderCom->Render_Collider(&m_pTransCom->m_matWorld);

	PostRender_Object();
}

void CServant_Quest::PostRender_Object(void)
{

}

_int CServant_Quest::CollisionUpdate_Object(const _float& fTimeDelta, 
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

			if (SERVANT_STATE::SERVANT_CLEANING_LOOP == m_eState)
				static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"대화 걸기");
			else if (SERVANT_STATE::SERVANT_SCARY_LOOP == m_eState)
			{
				static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"살해한다.");
				static_cast<CButton*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_C, L"살려준다.");
			}
			else if (SERVANT_STATE::SERVANT_IDLE == m_eState)
			{
				if (FALSE == m_bQuestAcceptKeyInput)
				{
					static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"수락한다.");
					static_cast<CButton*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_C, L"거절한다.");
				}
			}
			else if (SERVANT_STATE::SERVANT_REQUEST_LOOP == m_eState)
			{
				static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"수락한다.");
				static_cast<CButton*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_C, L"거절한다.");
			}
		}
	}

	return UPDATE_OK;
}

HRESULT CServant_Quest::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void CServant_Quest::Set_CameraPurpose(const _bool& _bState)
{
	if (TRUE == _bState)
		m_eState = SERVANT_STATE::SERVANT_LTURN;
	else
		m_eState = SERVANT_STATE::SERVANT_RTURN;
}

HRESULT CServant_Quest::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	
	/*  Mesh  */
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, m_pMeshTag));
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

void CServant_Quest::Update_State(void)
{
	switch (m_eState)
	{
	case SERVANT_STATE::SERVANT_IDLE:
		/*  RTurn  */
		if (98 == m_pMeshCom->Get_AnimationIndex())
		{
			m_pMeshCom->Set_AnimationSet(102, 0.f, 1.f, 0.f, 0.01f);
			m_uIdleOrder = 0;
			return;
		}
		if(0 == m_uIdleOrder)
			m_pMeshCom->Set_AnimationSet(102);
		else if (1 == m_uIdleOrder)
			m_pMeshCom->Set_AnimationSet(103);
		else if (2 == m_uIdleOrder)
			m_pMeshCom->Set_AnimationSet(104);
		else if (3 == m_uIdleOrder)
			m_pMeshCom->Set_AnimationSet(105);
		else if (4 == m_uIdleOrder)
			m_pMeshCom->Set_AnimationSet(106);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.98))
			m_uIdleOrder++;

		if (5 <= m_uIdleOrder)
			m_uIdleOrder = 0;

		if (FALSE == m_bQuestAccept)
		{
			static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(4);
			m_bQuestAccept = TRUE;
		}

		break;

	case SERVANT_STATE::SERVANT_CLEANING_LOOP:
		m_pMeshCom->Set_AnimationSet(68);
		break;

	case SERVANT_STATE::SERVANT_CLEANING_OUT:
		m_pMeshCom->Set_AnimationSet(66);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_pMeshCom->Set_TrackPosition(0.83);
			m_eState = SERVANT_SCARY_IN;
			Engine::MyPlaySound(L"Scream.mp3", Engine::CHANNELID::SCREAM);
		}
		break;

	case SERVANT_STATE::SERVANT_SCARY_IN:
		m_pMeshCom->Set_AnimationSet(110, 1.f, 1.f, 0.1f, 0.25f);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.8))
		{
			m_pMeshCom->Set_TrackPosition(0.7);
			m_eState = SERVANT_SCARY_LOOP;
			static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(3);
		}
		break;

	case SERVANT_STATE::SERVANT_SCARY_LOOP:
		m_pMeshCom->Set_AnimationSet(109, 0.5f, 1.f);

		if (Engine::COL_TRUE == m_pSphereColliderCom->Get_IsCollide())
		{
			if (Engine::Get_DIKeyDown(DIK_C))
				m_eState = SERVANT_SCARY_OUT;
		}
		break;

	case SERVANT_STATE::SERVANT_SCARY_OUT:
		m_pMeshCom->Set_AnimationSet(108);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_pMeshCom->Set_TrackPosition(0.8);
			m_eState = SERVANT_IDLE;
		}
		break;

	case SERVANT_STATE::SERVANT_THANK_IN:
		m_pMeshCom->Set_AnimationSet(67, 0.95f, 0.95f);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_pMeshCom->Set_TrackPosition(0.8);
			m_eState = SERVANT_THANK_OUT;

			if (2 <= m_uThankOrder)
			{
				static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(6);

				for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_ITEM))
				{
					if (Engine::COL_OBJECTID::COL_OBJECT_MAP == iter->Get_ColObjectID())
						static_cast<CFeature*>(iter)->Set_Hide(FALSE);
				}
			}
		}
		break;

	case SERVANT_STATE::SERVANT_THANK_OUT:
		m_pMeshCom->Set_AnimationSet(66, 0.95f, 0.95f);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.8))
		{
			if (2 > m_uThankOrder)
			{
				m_uThankOrder++;
				m_pMeshCom->Set_TrackPosition(0.8);
				m_eState = SERVANT_THANK_IN;
			}
			else
			{
				m_pMeshCom->Set_TrackPosition(0.8);
				m_eState = SERVANT_IDLE;
			}
		}
		break;

	case SERVANT_STATE::SERVANT_REQUEST_IN:
		m_pMeshCom->Set_AnimationSet(18);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_pMeshCom->Set_TrackPosition(0.8);
			m_eState = SERVANT_REQUEST_LOOP;
			static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_Index(5);
		}
		break;

	case SERVANT_STATE::SERVANT_REQUEST_LOOP:
		m_pMeshCom->Set_AnimationSet(17);
		break;

	case SERVANT_STATE::SERVANT_LTURN:
		m_pMeshCom->Set_AnimationSet(100);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_pMeshCom->Set_TrackPosition(0);
			m_eState = SERVANT_VIEW;

			m_pTransCom->m_vAngle.y += D3DXToRadian(-90.f);
			m_pTransCom->Update_Component();
		}
		break;

	case SERVANT_STATE::SERVANT_RTURN:
		m_pMeshCom->Set_AnimationSet(98);

		if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
		{
			m_pMeshCom->Set_TrackPosition(0);
			m_eState = SERVANT_IDLE;

			m_pTransCom->m_vAngle.y += D3DXToRadian(90.f);
			m_pTransCom->Update_Component();
		}
		break;

	case SERVANT_STATE::SERVANT_VIEW:
		m_pMeshCom->Set_AnimationSet(101, 0.f, 1.f, 0.f, 0.01f);
		break;

	case SERVANT_STATE::SERVANT_CRAZY:
		m_pMeshCom->Set_AnimationSet(36);
		break;
	}
}

void CServant_Quest::KeyInput(void)
{
	if (Engine::COLLTYPE::COL_TRUE == m_pSphereColliderCom->Get_IsCollide())
	{
		if (SERVANT_STATE::SERVANT_CLEANING_LOOP == m_eState)
		{
			if (Engine::Get_DIKeyDown(DIK_E))
			{
				m_eState = SERVANT_CLEANING_OUT;
				static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_QUEST);
			}
		}
		else if (SERVANT_STATE::SERVANT_IDLE == m_eState)
		{
			if (FALSE == m_bQuestAcceptKeyInput)
			{
				if (Engine::Get_DIKeyDown(DIK_C))
				{
					m_eState = SERVANT_REQUEST_IN;
					m_bQuestAcceptKeyInput = TRUE;
				}
			}
		}
		else if (SERVANT_STATE::SERVANT_REQUEST_LOOP == m_eState)
		{
			if (Engine::Get_DIKeyDown(DIK_E))
			{
				m_eState = SERVANT_THANK_IN;
				static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Set_RenderExit();
				static_cast<CMission*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_MISSION))->Set_MissionChange();
			}
		}
	}
}

CServant_Quest* CServant_Quest::Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag)
{
	CServant_Quest*	pInstance = new CServant_Quest(_pGraphicDev);

	pInstance->Set_MeshTag(_pMeshTag);
	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CServant_Quest::Free(void)
{
	Engine::CGameObject::Free();
}