#include "stdafx.h"
#include "LowerText.h"

CLowerText::CLowerText(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{

}

CLowerText::~CLowerText(void)
{

}

HRESULT CLowerText::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CLowerText::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (TRUE == m_bTipOneRender)
	{
		_vec3 vPos, vDist, vPlayerPos;
		if (4 == Engine::Get_EliteWave())
		{
			vPlayerPos = static_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COM_TRANSFORM))->Get_InfoPos();
			vPos = _vec3(53.0665f, vPlayerPos.y, 63.8913f);
			vDist = vPos - vPlayerPos;
			if (0.6f > D3DXVec3Length(&vDist))
			{
				Set_Index(15);
				m_bTipOneRender = FALSE;
			}
		}
		if (6 == Engine::Get_EliteWave())
		{
			vPlayerPos = static_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COM_TRANSFORM))->Get_InfoPos();
			vPos = _vec3(46.577f, vPlayerPos.y, 76.6162f);
			vDist = vPos - vPlayerPos;
			if (1.8f > D3DXVec3Length(&vDist))
			{
				Set_Index(16);
				m_bTipOneRender = FALSE;
			}
		}
		else if (9 == Engine::Get_EliteWave())
		{
			vPlayerPos = static_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COM_TRANSFORM))->Get_InfoPos();
			vPos = _vec3(83.1636f, vPlayerPos.y, 56.7404f);
			vDist = vPos - vPlayerPos;
			if (0.6f > D3DXVec3Length(&vDist))
			{
				Set_Index(17);
				m_bTipOneRender = FALSE;
			}
		}
		else if (12 == Engine::Get_EliteWave())
		{
			vPlayerPos = static_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COM_TRANSFORM))->Get_InfoPos();
			vPos = _vec3(75.3197f, vPlayerPos.y, 56.f);
			vDist = vPos - vPlayerPos;
			if (1.8f > D3DXVec3Length(&vDist))
			{
				Set_Index(19);
				m_bTipOneRender = FALSE;
				m_bMakeSoundActive = TRUE;
			}
		}
		/*vPlayerPos = static_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COM_TRANSFORM))->Get_InfoPos();
		cout << "X: " << vPlayerPos.x << endl;
		cout << "Z: " << vPlayerPos.z << endl;
		cout << "Dist: " << D3DXVec3Length(&vDist) << endl;*/
	}
	//cout << m_bTipOneRender << endl;

	return UPDATE_OK;
}

_int CLowerText::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	if (TRUE == m_bMakeSoundActive)
	{
		if(Engine::COL_FALSE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_IsCollide())
			static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_MOUSE, L"소리내기");
	}

	if (TRUE == m_bRender)
	{
		if (TRUE == m_bAlphaUpDown)
		{
			if (250.f > m_fAlpha)
				m_fAlpha += 180.f * fTimeDelta;
			else
				m_fAlpha = 255.f;
		}
		else if (FALSE == m_bAlphaUpDown)
		{
			if (5.f < m_fAlpha)
				m_fAlpha += -180.f * fTimeDelta;
			else
			{
				m_fAlpha = 0.f;
				m_bRender = FALSE;
				m_fTime = 0.f;
				m_bAlphaUpDown = TRUE;
			}
		}

		m_fTime += 1.f * fTimeDelta;

		if (4.f < m_fTime)
			m_bAlphaUpDown = FALSE;

		/*  Size  */
		if (0 == m_uIndex || 2 == m_uIndex || 3 == m_uIndex || 4 == m_uIndex
			|| 5 == m_uIndex || 6 == m_uIndex
			|| 8 == m_uIndex || 9 == m_uIndex || 10 == m_uIndex || 11 == m_uIndex
			|| 12 == m_uIndex || 13 == m_uIndex || 14 == m_uIndex
			|| 15 == m_uIndex || 16 == m_uIndex || 17 == m_uIndex || 18 == m_uIndex || 19 == m_uIndex)
		{
			m_pTransCom->Set_Pos(0, -600.f, 1.f);
			m_pTransCom->m_vScale = { 1450.f, 85.f, 1.f };

			m_pTransCom->Update_Component();
		}
		else if (1 == m_uIndex)
		{
			m_pTransCom->Set_Pos(0, -570.f, 1.f);
			m_pTransCom->m_vScale = { 1600.f, 280.f, 1.f };

			m_pTransCom->Update_Component();
		}
		if (7 == m_uIndex)
		{
			m_pTransCom->Set_Pos(0, -600.f, 1.f);
			m_pTransCom->m_vScale = { 1350.f, 85.f, 1.f };

			m_pTransCom->Update_Component();
		}

		m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_UI, this);
	}

	return UPDATE_OK;
}

void CLowerText::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	/*  Alpha Blending On  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(DWORD(m_fAlpha), 255, 255, 255));
}

void CLowerText::Render_Object(void)
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

	m_pTextureCom->Render_Texture(m_uIndex);
	m_pBufferCom->Render_Buffer();

	PostRender_Object();
}

void CLowerText::PostRender_Object(void)
{
	/*  Alpha Blending Off  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

HRESULT CLowerText::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Render_Texture(pEffect, "g_BaseTexture", m_uIndex);

	Safe_Release(pEffect);

	return S_OK;
}

void CLowerText::Set_Index(const _ushort& _uIndex)
{
	m_fAlpha = 0.f;
	m_fTime = 0.f;
	m_bAlphaUpDown = TRUE;
	m_bRender = TRUE;
	m_uIndex = _uIndex;
}

void CLowerText::Set_RenderExit()
{
	m_bAlphaUpDown = FALSE;
}

HRESULT CLowerText::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_LowerText"));
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

CLowerText* CLowerText::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CLowerText*	pInstance = new CLowerText(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLowerText::Free(void)
{
	Engine::CGameObject::Free();
}