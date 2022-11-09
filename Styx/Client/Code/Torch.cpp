#include "stdafx.h"
#include "Torch.h"

#include "Export_Function.h"

#include "TorchFire.h"

CTorch::CTorch(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	m_eOwnerObjectID = Engine::OBJECTID::OBJECT_TORCH;
	m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_TORCH;
}

CTorch::~CTorch(void)
{

}

HRESULT CTorch::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Update_Component();

	m_pMeshCom->Set_AnimationSet(3);

	m_pPlayer = static_cast<CPlayer*>(Engine::Find_PlayerObject());
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);

	return S_OK;
}

_int CTorch::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	if (TRUE == m_bLightOn)
	{
		if (4 == m_pMeshCom->Get_AnimationIndex())
		{
			if (m_pPlayer->Get_AniRateEnd(0.3))
			{
				m_pMeshCom->Set_AnimationSet(1, 0.f, 0.3f, 0.f, 0.01f);

				Engine::MyPlaySound(L"FireOff.wav", Engine::CHANNELID::FIREOFF);
			}
		}
		else if (1 == m_pMeshCom->Get_AnimationIndex())
		{
			if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
				m_pMeshCom->Set_AnimationSet(3, 0.f, 1.f, 0.f, 0.01f);
			else if (m_pMeshCom->Is_AnimationSetRateEnd(0.3))
				m_pTorchFire->Update_Object(m_fTimeDelta);
		}

		if (FALSE == m_bFireSound)
		{
			_vec3 vDist = m_pTransCom->Get_InfoPos() - m_pPlayer->Get_NewPos();

			if (3.5f >= D3DXVec3Length(&vDist))
			{
				Engine::MyPlaySound(L"Fireplace.wav", Engine::CHANNELID::FIREPLACE);
				m_bFireSound = TRUE;
				m_fFireSoundTime = 0.f;
			}
		}
		else
		{
			m_fFireSoundTime += 1.f * fTimeDelta;

			if (6.f <= m_fFireSoundTime)
			{
				Engine::StopSound(Engine::CHANNELID::FIREPLACE);
				m_bFireSound = false;
				m_fFireSoundTime = 0.f;
			}

			_vec3 vDist = m_pTransCom->Get_InfoPos() - m_pPlayer->Get_NewPos();

			if (3.5f < D3DXVec3Length(&vDist))
			{
				Engine::StopSound(Engine::CHANNELID::FIREPLACE);
				m_bFireSound = false;
				m_fFireSoundTime = 0.f;
			}
		}
	}
	else if (FALSE == m_bLightOn)
	{
		if (3 == m_pMeshCom->Get_AnimationIndex())
		{
			//m_pMeshCom->Set_TrackPosition(0.f);
			//m_pMeshCom->Set_AnimationSet(0, 1.f, 1.f, 0.99f, 1.f);

			m_pMeshCom->Set_TrackPosition(0.f);
			if (m_pPlayer->Get_AniRateEnd(0.1))
			{
				m_pMeshCom->Set_AnimationSet(0, 1.f, 1.f, 0.1f, 0.75f);
				Engine::MyPlaySound(L"FireOff.wav", Engine::CHANNELID::FIREOFF);
			}
		}
		else if (0 == m_pMeshCom->Get_AnimationIndex())
		{
			if (m_pMeshCom->Is_AnimationSetRateEnd(0.95))
				m_pMeshCom->Set_AnimationSet(4, 0.f, 1.f, 0.f, 0.01f);

			if (TRUE == m_bFireSound)
			{
				Engine::StopSound(Engine::CHANNELID::FIREPLACE);
				m_bFireSound = false;
				m_fFireSoundTime = 0.f;
			}
		}
	}

	/*  TorchFire Create  */
	if(nullptr == m_pTorchFire)
		FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_Object(), UPDATE_NO);

	/*  Coll False Init  */
	m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);

	return UPDATE_OK;
}

_int CTorch::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CTorch::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CTorch::Render_Object(void)
{
	/*  Frustum Culling  */
	if (FALSE == m_pOptimizationCom->Is_InFrustum_ForObject(&m_pTransCom->m_vInfo[Engine::INFO::INFO_POS], 5.5f))
		return;

	if(TRUE == g_bLightPointActive 
		&& 4 != m_pMeshCom->Get_AnimationIndex()
		&& 1 != m_pMeshCom->Get_AnimationIndex())
		Engine::Set_LightActive(m_uLightIndex);

	if (1 == m_pMeshCom->Get_AnimationIndex())
	{
		if (m_pMeshCom->Is_AnimationSetRateEnd(0.3))
			Engine::Set_LightActive(m_uLightIndex);
	}

	/*  TorchFire Update  */
	if (4 != m_pMeshCom->Get_AnimationIndex()
		&& 1 != m_pMeshCom->Get_AnimationIndex())
		m_pTorchFire->Update_Object(m_fTimeDelta);

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

		m_pMeshCom->Play_Animation(m_fTimeDelta);
		m_pMeshCom->Render_Meshe(pEffect);

		pEffect->EndPass();
		pEffect->End();
	}

	m_pSphereColliderCom->Render_Collider(&m_pTransCom->m_matWorld);

	PostRender_Object();
}

void CTorch::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

_int CTorch::CollisionUpdate_Object(const _float& fTimeDelta, 
									const DWORD _dwFlag, 
									const Engine::COL_OBJECTID _eColObjectID, 
									const _float& _fColValue /*= 0.f*/, 
									Engine::CGameObject* _pObject /*= nullptr*/)
{
	if (_dwFlag & COL_FLAG_SPHERE_SOUR)
	{
		if (Engine::COL_OBJECTID::COL_OBJECT_PLAYER == _eColObjectID
			&& FALSE == m_pPlayer->Get_DeadState())
		{
			m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);

			if (3 == m_pMeshCom->Get_AnimationIndex())
			{
				static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"불 끄기");
			}
			else if (4 == m_pMeshCom->Get_AnimationIndex())
			{
				static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"불 켜기");
			}
		}
	}

	return UPDATE_OK;
}

HRESULT CTorch::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CTorch::LateInit_Object(void)
{
	m_pTorchFire = CTorchFire::Create(m_pGraphicDev, this);
	NULL_CHECK_RETURN(m_pTorchFire, E_FAIL);

	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	/*  Light Point Init  */
	{
		ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
		tLightInfo.Type = D3DLIGHT_POINT;

		tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.6235294f, 0.207843137f, 1.f);
		tLightInfo.Specular = tLightInfo.Diffuse;
		tLightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);

		//tLightInfo.Attenuation0 = 0.000000001f;										/*  광원으로부터 멀어질 떄의 감쇄값  */
		tLightInfo.Range = 7.f;													/*  조명이 미치는 범위  */
																					//tLightInfo.Position = _vec3(TERRAIN_NUM_X >> 1, 1.f, TERRAIN_NUM_Z / 1.9f);
		//tLightInfo.Position = tFeature.vPosition;
		tLightInfo.Position = static_cast<Engine::CTransform*>(m_pTorchFire->Get_Component(Engine::COM_TRANSFORM))->Get_InfoPos();
		//pTorchFire
		//ightInfo.Position.y += 1.f;
		FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, Engine::CLightMgr::GetInstance()->Get_LightSize()), E_FAIL);
		m_uLightIndex = Engine::CLightMgr::GetInstance()->Get_LightSize() - 1;
	}

	return S_OK;
}

void CTorch::Set_LightOnOff()
{
	m_bLightOn = (m_bLightOn == TRUE) ? FALSE : TRUE; 
	//m_pMeshCom->Set_TrackPosition(0.f); 
}

HRESULT CTorch::Add_Component()
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
	//pComponent = m_pColliderCom = Engine::CCubeCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), m_pMeshCom->Get_Stride());
	pComponent = m_pSphereColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, 0.5f, 25, 24 );
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

CTorch* CTorch::Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag)
{
	CTorch*	pInstance = new CTorch(_pGraphicDev);

	pInstance->Set_MeshTag(_pMeshTag);
	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTorch::Free(void)
{
	Engine::Safe_Release(m_pTorchFire);

	Engine::CGameObject::Free();
}