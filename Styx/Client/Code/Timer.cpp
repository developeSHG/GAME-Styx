#include "stdafx.h"
#include "Timer.h"

CTimer::CTimer(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	
}

CTimer::~CTimer(void)
{
	
}

HRESULT CTimer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);
 
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(980.f, 280.f, 1.f);
	m_pTransCom->m_vScale = { 310.f, 55.f, 1.f };

	m_pTransCom->Update_Component();

	return S_OK;
}

_int CTimer::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (Engine::Get_DIKeyPressing(DIK_5))
	{
		if(9 > m_uMinTime)
			m_uMinTime++;
	}
	if (TRUE == m_bTimerActive
		&& 255.f > m_fAlpha)
	{
		if (250.f >= m_fAlpha)
			m_fAlpha += 170.f * fTimeDelta;
		else
		{
			m_fAlpha = 255.f;
			m_bTimerStart = TRUE;
		}
	}

	if (TRUE == m_bTimerStart)
	{
		m_fSecTime -= 1.f * fTimeDelta;

		if (0.f > m_fSecTime
			&& 0 < m_uMinTime)
		{
			m_fSecTime = 59.f;
			m_uMinTime--;
		}
		else if (0 == m_uMinTime)
		{
			/*  Dead  */
			if (0.f >= m_fSecTime)
				m_fSecTime = 0.f;
		}
	}

	return UPDATE_OK;
}

_int CTimer::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_UI, this);

	return UPDATE_OK;
}

void CTimer::PreRender_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMpBar_GREATER);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

	/*  Alpha Blending On  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(DWORD(255), 255, 255, 255));
}

void CTimer::Render_Object(void)
{
	if (FALSE == m_bTimerActive)
		return;

	PreRender_Object();

	/*  RcTex Buffer, Shader Render  */
	{
		//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		//NULL_CHECK(pEffect);

		//SetUp_ConstantTable(pEffect);

		//_uint iPassCnt = 0;

		//pEffect->Begin(&iPassCnt, 0);
		//pEffect->BeginPass(1);

		//m_pBufferCom->Render_Buffer();

		//pEffect->EndPass();
		//pEffect->End();
	}

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(DWORD(m_fAlpha), 255, 255, 255));
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	wstring wstrTime = L"0";
	wstrTime += to_wstring(int(m_uMinTime));
	wstrTime += L" : ";
	wstring wstrSecTime = to_wstring(int(m_fSecTime));
	if (10.f > m_fSecTime)
		wstrTime += L"0";
	wstrTime += wstrSecTime;

	_vec2 vPos = _vec2((m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].x / 2.f + WINCX / 2.f) - 26.5f,
		(WINCY / 2.f - m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].y / 2.f) - 7.f);
	Engine::Render_Font(L"Font_Timer", wstrTime.c_str(), &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	
	PostRender_Object();
}

void CTimer::PostRender_Object(void)
{
	/*  Alpha Blending Off  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

HRESULT CTimer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Render_Texture(pEffect, "g_BaseTexture", 0);

	Safe_Release(pEffect);

	return S_OK;
}

HRESULT CTimer::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Timer"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TEXTURE, pComponent);

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

	/*  Shader  */
	/*pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Alpha"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SHADER_ALPHA, pComponent);*/

	return S_OK;
}

CTimer* CTimer::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CTimer*	pInstance = new CTimer(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTimer::Free(void)
{
	Engine::CGameObject::Free();
}