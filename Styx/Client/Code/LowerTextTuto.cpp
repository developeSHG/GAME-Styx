#include "stdafx.h"
#include "LowerTextTuto.h"

CLowerTextTuto::CLowerTextTuto(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{

}

CLowerTextTuto::~CLowerTextTuto(void)
{

}

HRESULT CLowerTextTuto::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	/*  Size  */
	_matrix matScale, matTrans;

	D3DXMatrixScaling(&matScale, 1450.f, 190.f, 1.f);
	D3DXMatrixTranslation(&matTrans, 0.f, -570.f, 1.f);
	m_matWorld[0] = matScale * matTrans;

	D3DXMatrixScaling(&matScale, 1450.f, 85.f, 1.f);
	D3DXMatrixTranslation(&matTrans, 0.f, -570.f, 1.f);
	m_matWorld[1] = matScale * matTrans;

	D3DXMatrixScaling(&matScale, 1450.f, 190.f, 1.f);
	D3DXMatrixTranslation(&matTrans, 0.f, -570.f, 1.f);
	m_matWorld[2] = matScale * matTrans;

	D3DXMatrixScaling(&matScale, 1300.f, 190.f, 1.f);
	D3DXMatrixTranslation(&matTrans, 0.f, -570.f, 1.f);
	m_matWorld[3] = matScale * matTrans;

	return S_OK;
}

_int CLowerTextTuto::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	return UPDATE_OK;
}

_int CLowerTextTuto::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	if (TRUE == m_bRender)
	{
		m_fTime += 1.f * fTimeDelta;
 
		if (8.5f < m_fTime)
			m_uWave = 1;
		if (15.f < m_fTime)
			m_uWave = 2;
		if (21.5f < m_fTime)
			m_uWave = 3;
		if (28.f < m_fTime)
			m_uWave = 4;

		if (0 == m_uWave
			&& 2.f < m_fTime)
			m_bAlphaUpDown[0] = TRUE;
		else if (1 == m_uWave)
		{
			m_bAlphaUpDown[0] = FALSE;
			m_bAlphaUpDown[1] = TRUE;
		}
		else if (2 == m_uWave)
		{
			m_bAlphaUpDown[1] = FALSE;
			m_bAlphaUpDown[2] = TRUE;
		}
		else if (3 == m_uWave)
		{
			m_bAlphaUpDown[2] = FALSE;
			m_bAlphaUpDown[3] = TRUE;
		}
		else if (4 == m_uWave)
		{
			m_bAlphaUpDown[3] = FALSE;
		}

		for (_ushort i = 0; i < TEXT_NUM; ++i)
		{
			if (TRUE == m_bAlphaUpDown[i])
			{
				if (250.f > m_fAlpha[i])
					m_fAlpha[i] += 180.f * fTimeDelta;
				else
					m_fAlpha[i] = 255.f;
			}
			else if (FALSE == m_bAlphaUpDown[i])
			{
				if (5.f < m_fAlpha[i])
					m_fAlpha[i] += -180.f * fTimeDelta;
				else
					m_fAlpha[i] = 0.f;
			}

			if (4 == m_uWave
				&& 0.f >= m_fAlpha[3])
				m_bRender = FALSE;
		}

		m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_UI, this);
	}

	return UPDATE_OK;
}

void CLowerTextTuto::PreRender_Object(void)
{
	/*  Alpha Blending On  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
}

void CLowerTextTuto::Render_Object(void)
{
	PreRender_Object();

	/*  RcTex Buffer, Shader Render  */
	{
	/*	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(1);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();*/
	}

	for (_ushort i = 0; i < TEXT_NUM; ++i)
	{
		if (0.f < m_fAlpha[i])
		{
			m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(DWORD(m_fAlpha[i]), 255, 255, 255));
			m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld[i]);
			m_pTextureCom->Render_Texture(i);
			m_pBufferCom->Render_Buffer();
		}
	}

	PostRender_Object();
}

void CLowerTextTuto::PostRender_Object(void)
{
	/*  Alpha Blending Off  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

HRESULT CLowerTextTuto::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CLowerTextTuto::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_LowerTextTuto"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TEXTURE, pComponent);

	/*  Transform  */
	/*  Static Choice!!  */
	/*pComponent = m_pTransCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM, pComponent);*/

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

CLowerTextTuto* CLowerTextTuto::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CLowerTextTuto*	pInstance = new CLowerTextTuto(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLowerTextTuto::Free(void)
{
	Engine::CGameObject::Free();
}