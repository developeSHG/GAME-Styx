#include "stdafx.h"
#include "Marker.h"

CMarker::CMarker(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{

}

CMarker::~CMarker(void)
{

}

HRESULT CMarker::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pPos[0] = static_cast<Engine::CTransform*>(Engine::Find_ObjectListBack(Engine::OBJECT_SERVANT_QUEST)->Get_Component(Engine::COM_TRANSFORM))->Get_InfoPosPointer();
	//m_pObjectWorld[0] = static_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COM_TRANSFORM))->Get_WorldMatrixPointer();
	m_pPos[1] = static_cast<Engine::CTransform*>(Engine::Find_ObjectListFront(Engine::OBJECT_GRID)->Get_Component(Engine::COM_TRANSFORM))->Get_InfoPosPointer();
	for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECT_ITEM))
	{
		if (Engine::COL_OBJECTID::COL_OBJECT_KEY == iter->Get_ColObjectID())
		{
			m_pPos[2] = static_cast<Engine::CTransform*>(iter->Get_Component(Engine::COM_TRANSFORM))->Get_InfoPosPointer();
			break;
		}
	}
	//m_pInfoPos[2] = static_cast<Engine::CTransform*>(Engine::Find_ObjectListBack(Engine::OBJECT)->Get_Component(Engine::COM_TRANSFORM))->Get_WorldMatrixPointer();
	m_pPos[3] = static_cast<Engine::CTransform*>(Engine::Find_ObjectListBack(Engine::OBJECT_SERVANT_RESCUE)->Get_Component(Engine::COM_TRANSFORM))->Get_InfoPosPointer();

	D3DXMatrixScaling(&m_matScale[0], 0.1f, 0.1f, 0.1f);
	D3DXMatrixScaling(&m_matScale[1], 0.1f, 0.1f, 0.1f);
	D3DXMatrixScaling(&m_matScale[2], 0.1f, 0.1f, 0.1f);
	D3DXMatrixScaling(&m_matScale[3], 0.1f, 0.1f, 0.1f);

	for (_ushort i = 0; i < MARKER_NUM; ++i)
	{
		_vec3 vPos;
		memcpy(&vPos, m_pPos[i], sizeof(_vec3));

		if (0 == i)			/*  Quest  */
			vPos.y += 1.95f;
		else if (1 == i)	/*  Grid  */
			vPos.y += 3.2f;
		else if (2 == i)	/*  Key  */
			vPos.y += 0.75f;
		else if (3 == i)	/*  Rescue  */
			vPos.y += 1.95f;

		D3DXMatrixTranslation(&m_matTrans[i], vPos.x, vPos.y, /*matView._43 +2.f*/ /*matView._43 +*/ vPos.z);
	}

	return S_OK;
}

_int CMarker::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3 vCamera;
	memcpy(&vCamera, matView.m[3], sizeof(_vec3));

	for (_ushort i = 0; i < MARKER_NUM; ++i)
	{
		_float fDist = fabsf(m_pPos[i]->z - vCamera.z);

		_float fSize = 0.12f;
		if (3.f < fDist)
			fSize = 0.15f;
		if (4.5f < fDist)
			fSize = 0.2f;
		if (6.f < fDist)
			fSize = 0.25f;
		if (10.f < fDist)
			fSize = 0.3f;
		if (17.f < fDist)
			fSize = 0.35f;
		if (25.f < fDist)
			fSize = 0.4f;
		if (30.f < fDist)
			fSize = 0.45f;
		if (35.f < fDist)
			fSize = 0.5f;
		if (40.f < fDist)
			fSize = 0.6f;
		if (45.f < fDist)
			fSize = 0.65f;
		if (50.f < fDist)
			fSize = 0.7f;
		if (55.f < fDist)
			fSize = 0.5f;
		if (60.f < fDist)
			fSize = 0.8f;

		if (1 == i)	/*  Grid  */
			fSize += 0.045f;
		else if (2 == i)	/*  Key  */
			fSize += 0.011f;
		else if (3 == i)	/*  Rescue  */
			fSize += 0.011f;

		m_fSize[i] += (fSize - m_fSize[i]) * 2.f * fTimeDelta;

		if (fabsf(m_fSize[i] - fSize) < 0.001f)
			m_fSize[i] = fSize;
		
		D3DXMatrixScaling(&m_matScale[i], m_fSize[i], m_fSize[i], m_fSize[i]);
	}

	return UPDATE_OK;
}

_int CMarker::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	return UPDATE_OK;
}

void CMarker::PreRender_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

}

void CMarker::Render_Object(void)
{
	PreRender_Object();

	/*  RcTex Buffer, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		_matrix matWorld[MARKER_NUM];
		_matrix matView, matProj, matBill;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &matProj);

		/*  BillBoard  */
		{
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
		}

		for (_ushort i = 0; i < MARKER_NUM; ++i)
		{
			matWorld[i] = m_matScale[i] * m_matTrans[i];
			matWorld[i] = (matBill * matWorld[i]);
			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld[i]);

			pEffect->SetMatrix("g_matWorld", &matWorld[i]);

			m_pTextureCom->Render_Texture(pEffect, "g_BaseTexture", i);

			//SetUp_ConstantTable(pEffect, i);

			_uint iPassCnt = 0;

			pEffect->Begin(&iPassCnt, 0);
			pEffect->BeginPass(1);

			m_pBufferCom->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}
	}

	PostRender_Object();
}

void CMarker::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CMarker::SetUp_ConstantTable(LPD3DXEFFECT pEffect, _ushort _uIndex)
{
	//pEffect->AddRef();

	//_matrix		matWorld, matView, matProj, matBill;

	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	//D3DXMatrixIdentity(&matBill);
	///*  Y  */
	//matBill._11 = matView._11;
	//matBill._13 = matView._13;
	//matBill._31 = matView._31;
	//matBill._33 = matView._33;
	///*  X  */
	//matBill._22 = matView._22;
	//matBill._23 = matView._23;
	//matBill._32 = matView._32;

	//D3DXMatrixInverse(&matBill, 0, &matBill);

	//matWorld = m_matScale[_uIndex] * m_matTrans[_uIndex];
	//matWorld = (matBill * matWorld);

	//pEffect->SetMatrix("g_matWorld", &matWorld);
	//pEffect->SetMatrix("g_matView", &matView);
	//pEffect->SetMatrix("g_matProj", &matProj);

	//m_pTextureCom->Render_Texture(pEffect, "g_BaseTexture", _uIndex);

	//Safe_Release(pEffect);

	return S_OK;
}

HRESULT CMarker::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Marker"));
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
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"Shader_Alpha"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SHADER_ALPHA, pComponent);

	return S_OK;
}

CMarker* CMarker::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CMarker*	pInstance = new CMarker(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMarker::Free(void)
{
	Engine::CGameObject::Free();
}