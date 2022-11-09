#include "stdafx.h"
#include "SceneAlpha.h"

#include "Export_Function.h"

#include "PlayerBehavior.h"

CSceneAlpha::CSceneAlpha(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CSceneAlpha::~CSceneAlpha(void)
{
}

HRESULT CSceneAlpha::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(0.f, 0.f, 1.f);
	m_pTransCom->Set_Scale(WINCX << 1, WINCY << 1, 1.f);

	m_tMtrl = InitMtrl(BLACK, BLACK, BLACK, BLACK, 0.f);
	m_tMtrl.Diffuse.a = 1.f;

	return S_OK;
}

_int CSceneAlpha::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	/*  Scene Go = TRUE  */
	if (TRUE == m_bFade)
	{
		if (0.f < m_tMtrl.Diffuse.a)
		{
			m_tMtrl.Diffuse.a += -0.5f * fTimeDelta;

			if (TRUE == m_bDead)
				m_tMtrl.Diffuse.a += -0.3f * fTimeDelta;
		}
		else
		{
			m_tMtrl.Diffuse.a = 0.f;

			if (TRUE == m_bDead)
			{
				m_bDead = FALSE;

				static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_DeadState(FALSE);
			}
		}
	}
	/*  Scene Out = FALSE  */
	else
	{
		if (1.f > m_tMtrl.Diffuse.a)
		{
			m_tMtrl.Diffuse.a += 0.5f * fTimeDelta;

			if(TRUE == m_bDead)
				m_tMtrl.Diffuse.a += 0.8f * fTimeDelta;
		}
		else
		{
			m_tMtrl.Diffuse.a = 1.f;

			if (TRUE == m_bDead)
			{
				m_fDeadTime += 1.f * fTimeDelta;

				if (1.f < m_fDeadTime)
				{
					m_bFade = TRUE;
					m_fDeadTime = 0.f;
					static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_InitPlayerLook(&_vec3(0.f, 0.f, -1.f));
					static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_EliteState(TRUE);
					static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_NextState(PLAYER_STATE::PLAYER_SICK_LOOP);
					static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_EliteMeshCom()->Set_AnimationSet(214);
					static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_PlayerBehavior()->Set_OnePlayAni(1.6);
					static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_DeadState(FALSE);
					static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CollToElite()->Set_Hide(TRUE);
				}
			}
		}
	}

	return UPDATE_OK;
}

_int CSceneAlpha::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_UI, this);

	return UPDATE_OK;
}

void CSceneAlpha::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetMaterial(&m_tMtrl);

	/*  Alpha Blending On  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);				// 주사위 알파
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);			// 1 - 주사위 알파

	/*  셰이드 과정에서 난반사 컬로로 알파를 계산하는 것  */
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

	/*  OrthoView Init */
	//D3DXMATRIX matView, matProj;
	//D3DXMatrixIdentity(&matView);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	//D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 1.f, 10.f);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CSceneAlpha::Render_Object(void)
{
	if (0.f == m_tMtrl.Diffuse.a)
		return;

	PreRender_Object();

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	PostRender_Object();
}

void CSceneAlpha::PostRender_Object(void)
{
	/*  Alpha Blending Off  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	/*  PerspectiveView Init */
	//if (Engine::Find_StaticCameraObject())
	//{
	//	SCAST<Engine::CCamera*>(Engine::Find_StaticCameraObject())->TransformViewSpaceMatrix();
	//	SCAST<Engine::CCamera*>(Engine::Find_StaticCameraObject())->TransformProjectionMatrix();
	//}
	//else if (Engine::Find_DynamicCameraObject())
	//{
	//	SCAST<Engine::CCamera*>(Engine::Find_DynamicCameraObject())->TransformViewSpaceMatrix();
	//	SCAST<Engine::CCamera*>(Engine::Find_DynamicCameraObject())->TransformProjectionMatrix();
	//}
}

HRESULT CSceneAlpha::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Alpha"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TEXTURE, pComponent);

	/*  Transform  */
	pComponent = m_pTransCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM, pComponent);

	/*  Renderer  */
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_RENDERER, pComponent);


	return S_OK;
}

CSceneAlpha * CSceneAlpha::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSceneAlpha *	pInstance = new CSceneAlpha(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);
	
	return pInstance;
}

void CSceneAlpha::Free(void)
{
	Engine::CGameObject::Free();
}

