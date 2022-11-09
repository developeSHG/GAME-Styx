#include "stdafx.h"
#include "Grid.h"

#include "Export_Function.h"

CGrid::CGrid(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	m_eOwnerObjectID = Engine::OBJECTID::OBJECT_GRID;
	m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_GRID;
}

CGrid::~CGrid(void)
{

}

HRESULT CGrid::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_AnimationSet(2);

	return S_OK;
}

_int CGrid::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	/*  Front용 열때  */
	if (TRUE == m_bActive && FALSE == m_bActive2)
	{
		CLever* pLever = static_cast<CLever*>(Engine::Find_ObjectListFront(Engine::OBJECT_LEVER));
		if (1 == static_cast<Engine::CDynamicMesh*>(pLever->Get_Component(Engine::COM_MESH))->Get_AnimationIndex()
			&& static_cast<Engine::CDynamicMesh*>(pLever->Get_Component(Engine::COM_MESH))->Is_AnimationSetRateEnd(0.9))
		{
			static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_GRID);
			m_bOpen = TRUE;
		}
		
		if (TRUE == m_bOpen)
		{
			if (2 == m_pMeshCom->Get_AnimationIndex())
			{
				m_pMeshCom->Set_AnimationSet(1);
				Engine::MyPlaySound(L"GridUp.mp3", Engine::CHANNELID::GRIDUP);
			}
			else if (1 == m_pMeshCom->Get_AnimationIndex())
			{
				if (m_pMeshCom->Is_AnimationSetRateEnd(0.99))
				{
					m_pMeshCom->Set_TrackPosition(0.99);
					m_pMeshCom->Set_AnimationSet(0, 0.f, 1.f, 0.f, 0.01f);
				}
			}
			else if (0 == m_pMeshCom->Get_AnimationIndex())
			{
				if (m_pMeshCom->Is_AnimationSetRateEnd(0.99))
				{
					m_bActive = FALSE;
				}
			}
		}
	}
	/*  Back용 열때, 닫을때  */
	else if (TRUE == m_bActive2 && FALSE == m_bActive)
	{
		CLever* pLever = static_cast<CLever*>(Engine::Find_ObjectListBack(Engine::OBJECT_LEVER));
		if (static_cast<Engine::CDynamicMesh*>(pLever->Get_Component(Engine::COM_MESH))->Is_AnimationSetRateEnd(0.9)
			&& FALSE == m_bOpen)
		{
			m_bOpen = TRUE;
		}
		else if (TRUE == m_bOpen && TRUE == m_bClose && FALSE == m_bCloseActive
				&& 0 == m_pMeshCom->Get_AnimationIndex())
		{
			m_bCloseActive = TRUE;
		}

		if (TRUE == m_bOpen && FALSE == m_bClose
			&& FALSE == m_bCloseActive)
		{
			if (2 == m_pMeshCom->Get_AnimationIndex())
			{
				m_pMeshCom->Set_AnimationSet(1);
				Engine::MyPlaySound(L"GridUp.mp3", Engine::CHANNELID::GRIDUP);
			}
			else if (1 == m_pMeshCom->Get_AnimationIndex())
			{
				if (m_pMeshCom->Is_AnimationSetRateEnd(0.99))
				{
					m_pMeshCom->Set_TrackPosition(0.99);
					m_pMeshCom->Set_AnimationSet(0, 0.f, 1.f, 0.f, 0.01f);
				}
			}
			else if (0 == m_pMeshCom->Get_AnimationIndex())
			{
				if (m_pMeshCom->Is_AnimationSetRateEnd(0.99))
				{
					m_bActive2 = FALSE;
					m_bClose = TRUE;
				}
			}
		}
		else if (TRUE == m_bClose && TRUE == m_bCloseActive)
		{
			if (0 == m_pMeshCom->Get_AnimationIndex())
			{
				m_pMeshCom->Set_AnimationSet(1);
				m_dAccTime = 1;
			}
			else if (1 == m_pMeshCom->Get_AnimationIndex())
			{                                
				if (!m_pMeshCom->Is_AnimationSetRateEnd(0.03))
				{
					m_dAccTime = 0.01;
					m_pMeshCom->Set_AnimationSet(2, 0.f, 0.f, 0.f, 0.01f);
				}
			}
			else if (2 == m_pMeshCom->Get_AnimationIndex())
			{
				m_bActive2 = FALSE;
				m_dAccTime = 0.5;
				m_pMeshCom->Set_TrackPosition(0.0f);
			}	
		}
	}

	/*  Coll False Init  */
	m_pAABBColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);

	return UPDATE_OK;
}

_int CGrid::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CGrid::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CGrid::Render_Object(void)
{
	/*  Frustum Culling  */
	if (false == m_pOptimizationCom->Is_InFrustum_ForObject(&m_pTransCom->m_vInfo[Engine::INFO::INFO_POS], 4.f))
		return;

	/*  CollisionMgr AddObject  */
	if (2 == m_pMeshCom->Get_AnimationIndex())
	{
		DWORD dwFlag = COL_FLAG_AABB_SOUR;
		Engine::AddObject_CollisionMgr(dwFlag, this);
	}

	PreRender_Object();

	/*  Dynamic Mesh, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(0);

		if(FALSE == m_bCloseActive)
			m_pMeshCom->Play_Animation(m_fTimeDelta);
		else
		{
			if (2 != m_pMeshCom->Get_AnimationIndex())
			{
				m_dAccTime -= 0.8 * m_fTimeDelta;
				m_pMeshCom->Set_TrackPosition(m_dAccTime);
				m_pMeshCom->Play_Animation(m_fTimeDelta, FALSE);
			}
		}
		m_pMeshCom->Render_Meshe(pEffect);

		pEffect->EndPass();
		pEffect->End();
	}

	m_pAABBColliderCom->Render_Collider(&m_pTransCom->m_matWorld);

	PostRender_Object();
}

void CGrid::PostRender_Object(void)
{

}

HRESULT CGrid::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

_int CGrid::CollisionUpdate_Object(const _float& fTimeDelta, 
									const DWORD _dwFlag, 
									const Engine::COL_OBJECTID _eColObjectID, 
									const _float& _fColValue /*= 0.f*/, 
									Engine::CGameObject* _pObject /*= nullptr*/)
{
	if (_dwFlag & COL_FLAG_AABB_SOUR)
	{
		if (Engine::COL_OBJECTID::COL_OBJECT_PLAYER == _eColObjectID)
		{
			m_pAABBColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);
		}
	}

	return UPDATE_OK;
}

HRESULT CGrid::Add_Component()
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

	/*  AABB Collider  */
	pComponent = m_pAABBColliderCom = Engine::CCubeCollider::Create(m_pGraphicDev, _vec3(-0.25f, 0.f, -1.6f), _vec3(0.25f, 3.f, 1.6f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_AABB_COLLIDER, pComponent);

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

CGrid* CGrid::Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag)
{
	CGrid*	pInstance = new CGrid(_pGraphicDev);

	pInstance->Set_MeshTag(_pMeshTag);
	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CGrid::Free(void)
{
	Engine::CGameObject::Free();
}