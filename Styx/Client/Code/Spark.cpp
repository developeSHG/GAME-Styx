#include "stdafx.h"
#include "Spark.h"

CSpark::CSpark(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{

}

CSpark::~CSpark(void)
{

}

HRESULT CSpark::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vScale = _vec3{ 1.2f, 1.2f, 1.f };

	return S_OK;
}

_int CSpark::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pTransCom->m_vInfo[Engine::INFO_POS] = m_pPlayerTransCom->Get_InfoPos();
	m_pTransCom->m_vInfo[Engine::INFO_POS] += m_pPlayerTransCom->m_vInfo[Engine::INFO_LOOK] * 50.f;
	m_pTransCom->m_vInfo[Engine::INFO_POS].y += 0.85f;

	m_pTransCom->m_vScale.x += 8.f * fTimeDelta;
	m_pTransCom->m_vScale.y += 5.f * fTimeDelta;

	m_pTransCom->Update_Component();

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	m_pPlayer->Set_SparkRender(FALSE);

	return UPDATE_OK;
}

_int CSpark::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CSpark::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
	
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	/*  Alpha Blending On  */
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	/*  Alpha Blending On  */
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	////m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	////m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	////m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

	//m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(DWORD(100), 255, 255, 255));
	
}

void CSpark::Render_Object(void)
{
	PreRender_Object();

	/*  RcTex Buffer, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		/*  Spark1  */
		{
			SetUp_ConstantTable(pEffect, 0);

			_uint iPassCnt = 0;

			pEffect->Begin(&iPassCnt, 0);
			pEffect->BeginPass(2);

			m_pBufferCom->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}

		/*  Spark2  */
		{
			SetUp_ConstantTable(pEffect, 1);

			_uint iPassCnt = 0;

			pEffect->Begin(&iPassCnt, 0);
			pEffect->BeginPass(2);

			m_pBufferCom->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}

		//m_pTextureCom->Render_Texture(1);
		//m_pBufferCom->Render_Buffer();
	}

	PostRender_Object();
}

void CSpark::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	/*  Alpha Blending Off  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CSpark::SetUp_ConstantTable(LPD3DXEFFECT pEffect, _ushort _uIndex)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj, matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixIdentity(&matBill);
	/*  Y  */
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	/*  X  */
	matBill._22 = matView._22;
	matBill._23 = matView._23;
	matBill._32 = matView._32;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	matWorld = (matBill * matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Render_Texture(pEffect, "g_BaseTexture", _uIndex);

	Safe_Release(pEffect);

	return S_OK;
}

void CSpark::Set_Player(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayerMeshCom = SCAST<Engine::CDynamicMesh*>(m_pPlayer->Get_Component(Engine::COMPONENTTYPE::COM_MESH));
	m_pPlayerTransCom = SCAST<Engine::CTransform*>(m_pPlayer->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));
}

void CSpark::Set_InitScale(void)
{
	m_pTransCom->m_vScale = _vec3{ 1.2f, 1.2f, 1.f };
	m_pTransCom->Update_Component();
}

HRESULT CSpark::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Spark"));
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
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Alpha"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SHADER_ALPHA, pComponent);

	return S_OK;
}

CSpark* CSpark::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CSpark*	pInstance = new CSpark(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSpark::Free(void)
{
	Engine::CGameObject::Free();
}