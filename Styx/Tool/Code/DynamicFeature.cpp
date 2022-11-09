#include "stdafx.h"
#include "DynamicFeature.h"

#include "Export_Function.h"

CDynamicFeature::CDynamicFeature(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	
}

CDynamicFeature::~CDynamicFeature(void)
{

}

HRESULT CDynamicFeature::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (TOOLSCENEID::TOOLSCENE_SPACE == Engine::Get_CurScene())
		m_pTransCom->Set_Pos(TERRAIN_NUM_X >> 1, 0.f, TERRAIN_NUM_Z / 2.25f);

	//m_pTransCom->Set_Scale(0.025f, 0.025f, 0.025f);
	m_pTransCom->Set_Scale(0.01f, 0.01f, 0.01f);

	m_pMeshCom->Set_AnimationSet(0);

	if (!lstrcmp(m_pMeshTag, L"Mesh_Monster_Elite"))
		m_pMeshCom->Set_AnimationSet(101);

	return S_OK;
}

_int CDynamicFeature::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	if (TOOLSCENEID::TOOLSCENE_SPACE == Engine::Get_CurScene())
	{
		if (TRUE == m_pMeshCom->Is_AnimationSetEnd())
		{
			if (m_uCurrentIndex == m_pMeshCom->Get_AnimationIndex())
			{
				m_pMeshCom->Set_AnimationSet(m_uNexIndex, 
											m_tCurrentAniCtrlInfo.fTrackSpeed, 
											m_tNewAniCtrlInfo.fTrackSpeed,
											m_tCurrentAniCtrlInfo.fWeight,
											m_tCurrentAniCtrlInfo.fDuration);
			}
			else if (m_uNexIndex == m_pMeshCom->Get_AnimationIndex())
			{
				m_pMeshCom->Set_AnimationSet(m_uCurrentIndex,
											m_tNewAniCtrlInfo.fTrackSpeed,
											m_tCurrentAniCtrlInfo.fTrackSpeed,
											m_tNewAniCtrlInfo.fWeight,
											m_tNewAniCtrlInfo.fDuration);
			}
		}
	}

	return UPDATE_OK;
}

_int CDynamicFeature::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CDynamicFeature::PreRender_Object(void)
{
	if (TRUE == CToolMgr::GetInstance()->m_pMeshTool->m_ctrlFillModeRadio[FILLMODE::FILL_SOLID].GetCheck())
		CToolMgr::GetInstance()->m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	else
		CToolMgr::GetInstance()->m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CDynamicFeature::Render_Object(void)
{
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

	if (TRUE == m_bPick && TOOLID::TOOL_MESH == CToolMgr::GetInstance()->m_eCurTool)
		m_pColliderCom->Render_Collider(&m_pTransCom->m_matWorld);

	if (TOOLSCENEID::TOOLSCENE_SPACE == Engine::Get_CurScene())
	{
		POINT		ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(CToolMgr::GetInstance()->m_hWnd, &ptMouse);

		if (m_uCurrentIndex == m_pMeshCom->Get_AnimationIndex())
		{
			_tchar szTemp[32];
			swprintf_s(szTemp, L"%d", m_uCurrentIndex);
			CString strTag = L"Current Animation Index.";
			strTag += szTemp;

			Engine::Render_Font(L"Font_Default", strTag, &_vec2(100.f, 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
		
		else if (m_uNexIndex == m_pMeshCom->Get_AnimationIndex())
		{
			_tchar szTemp[32];
			swprintf_s(szTemp, L"%d", m_uNexIndex);
			CString strTag = L"New Animation Index.";
			strTag += szTemp;

			Engine::Render_Font(L"Font_Default", strTag, &_vec2(100.f, 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
	}

	PostRender_Object();
}

void CDynamicFeature::PostRender_Object(void)
{

}

HRESULT CDynamicFeature::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	if (SCENEID::SCENE_STAGE == Engine::Get_CurScene())
		pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	else if (SCENEID::SCENE_LOGO == Engine::Get_CurScene())
		pEffect->SetVector("g_vLightAmbient", &_vec4(1.f, 1.f, 1.f, 1.f));

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

HRESULT CDynamicFeature::Add_Component()
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

	/*  Collider  */
	//pComponent = m_pColliderCom = Engine::CCubeCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), m_pMeshCom->Get_Stride());
	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, 1, 25, 24 );
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SPHERE_COLLIDER, pComponent);
	///*  CollisionMgr AddObject  */
	//DWORD dwFlag = COL_FLAG_AABB_DEST | COL_FLAG_OBB_DEST | COL_FLAG_SPHERE_DEST;
	//Engine::AddObject_CollisionMgr(dwFlag, this);

	/*  Shader  */
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SHADER_MESH, pComponent);

	return S_OK;
}

CDynamicFeature* CDynamicFeature::Create(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag)
{
	CDynamicFeature*	pInstance = new CDynamicFeature(_pGraphicDev);

	pInstance->Set_MeshTag(_pMeshTag);
	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDynamicFeature::Free(void)
{
	Engine::CGameObject::Free();
}