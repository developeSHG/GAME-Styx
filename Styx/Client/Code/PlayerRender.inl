void CPlayer::PreRender_Object(void)
{
	//if(TRUE == m_bMysteryState)
	//	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
	//else
	//	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	if (PLAYER_STATE::PLAYER_TRUNK_OUT == m_eCurState
		|| FALSE == m_pPlayerBehavior->Get_Moving())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CPlayer::Render_Object(void)
{
	PreRender_Object();

	m_pMeshCom->Play_Animation(m_fTimeDelta);
	m_pMeshCom_Elite->Play_Animation(m_fTimeDelta);

	/*  Dynamic Mesh, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		if (FALSE == m_bStyxEliteRender
			&& FALSE == m_bTranslucent)
		{
			pEffect->AddRef();

			SetUp_ConstantTable(pEffect);

			_matrix matWorld;
			if (FALSE == m_bEliteState)
				m_pTransCom->Get_WorldMatrix(&matWorld);
			else
				m_pTransCom_Elite->Get_WorldMatrix(&matWorld);

			pEffect->SetMatrix("g_matWorld", &matWorld);
			pEffect->SetFloat("g_fAlpha", 1.f);
			pEffect->SetBool("g_bSpecular", false);
			pEffect->SetBool("g_bCatsleIn", true);
			pEffect->SetBool("g_bTranslucent", false);

			Safe_Release(pEffect);

			_uint iPassCnt = 0;

			pEffect->Begin(&iPassCnt, 0);
			pEffect->BeginPass(0);
			if (FALSE == m_bEliteState)
			{
				m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

				if (FALSE == m_bFirstPersonState)
					m_pMeshCom->Render_Meshe(pEffect);
			}
			else
			{
				m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom_Elite->m_matWorld);
				m_pMeshCom_Elite->Render_Meshe(pEffect);

				if (m_pPlayerSword)
					m_pPlayerSword->Update_Object(m_fTimeDelta);
			}

			pEffect->EndPass();
			pEffect->End();
		}
		/*  Transluent  */
		else if (TRUE == m_bTranslucent)
		{
			_matrix matWorld;
			_uint iPassCnt = 0;

			/*  Styx  */
			if(FALSE == static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Get_ElitePerson())
			{
				pEffect->AddRef();

				SetUp_ConstantTable(pEffect);

				m_pTransCom->Get_WorldMatrix(&matWorld);
				pEffect->SetMatrix("g_matWorld", &matWorld);
				pEffect->SetFloat("g_fAlpha", 0.25f);
				pEffect->SetBool("g_bSpecular", false);
				pEffect->SetBool("g_bCatsleIn", true);
				pEffect->SetBool("g_bTranslucent", true);

				Safe_Release(pEffect);

				pEffect->Begin(&iPassCnt, 0);
				pEffect->BeginPass(0);

				m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
				m_pMeshCom->Render_Meshe(pEffect);

				pEffect->EndPass();
				pEffect->End();
			}
			/*  Elite  */
			{
				pEffect->AddRef();

				SetUp_ConstantTable(pEffect);

				m_pTransCom_Elite->Get_WorldMatrix(&matWorld);
				pEffect->SetMatrix("g_matWorld", &matWorld);
				pEffect->SetFloat("g_fAlpha", 1.f);
				pEffect->SetBool("g_bSpecular", true);
				pEffect->SetBool("g_bCatsleIn", true);
				pEffect->SetBool("g_bTranslucent", false);

				Safe_Release(pEffect);

				pEffect->Begin(&iPassCnt, 0);
				pEffect->BeginPass(0);

				m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom_Elite->m_matWorld);
				m_pMeshCom_Elite->Render_Meshe(pEffect);

				pEffect->EndPass();
				pEffect->End();

				if (m_pPlayerSword)
					m_pPlayerSword->Update_Object(m_fTimeDelta);
			}

		}
		else if (TRUE == m_bStyxEliteRender
				&& FALSE == m_bTranslucent)
		{
			_matrix matWorld;
			_uint iPassCnt = 0;

			/*  Styx  */
			{
				pEffect->AddRef();

				SetUp_ConstantTable(pEffect);

				m_pTransCom->Get_WorldMatrix(&matWorld);
				pEffect->SetMatrix("g_matWorld", &matWorld);
				pEffect->SetFloat("g_fAlpha", 1.f);
				pEffect->SetBool("g_bSpecular", false);
				pEffect->SetBool("g_bCatsleIn", true);
				pEffect->SetBool("g_bTranslucent", false);

				Safe_Release(pEffect);

				pEffect->Begin(&iPassCnt, 0);
				pEffect->BeginPass(0);

				m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
				m_pMeshCom->Render_Meshe(pEffect);

				pEffect->EndPass();
				pEffect->End();
			}
			/*  Elite  */
			{
				pEffect->AddRef();

				SetUp_ConstantTable(pEffect);

				m_pTransCom_Elite->Get_WorldMatrix(&matWorld);
				pEffect->SetMatrix("g_matWorld", &matWorld);
				pEffect->SetFloat("g_fAlpha", 1.f);
				pEffect->SetBool("g_bSpecular", true);
				pEffect->SetBool("g_bCatsleIn", true);
				pEffect->SetBool("g_bTranslucent", false);

				Safe_Release(pEffect);

				pEffect->Begin(&iPassCnt, 0);
				pEffect->BeginPass(0);

				m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom_Elite->m_matWorld);
				m_pMeshCom_Elite->Render_Meshe(pEffect);

				pEffect->EndPass();
				pEffect->End();

				if (m_pPlayerSword)
					m_pPlayerSword->Update_Object(m_fTimeDelta);
			}

		}
	}

	m_pAABBColliderCom->Render_Collider(&m_pTransCom->m_matAABBWorld);
	m_pSphereColliderCom->Render_Collider(&m_pTransCom->m_matWorld);

	PostRender_Object();
}

void CPlayer::PostRender_Object(void)
{
	if (PLAYER_STATE::PLAYER_TRUNK_OUT == m_eCurState
		|| FALSE == m_pPlayerBehavior->Get_Moving())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo();
	NULL_CHECK_RETURN(pLightInfo, E_FAIL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);

	D3DMATERIAL9			tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 1.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	return S_OK;
}
