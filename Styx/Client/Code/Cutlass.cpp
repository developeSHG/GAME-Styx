#include "stdafx.h"
#include "Cutlass.h"

#include "EliteBehavior.h"

CCutlass::CCutlass(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	m_eOwnerObjectID = Engine::OBJECTID::OBJECT_CUTLASS;
	m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_CUTLASS;
}

CCutlass::~CCutlass(void)
{

}

HRESULT CCutlass::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*	pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Find_PlayerObject()->Get_Component(Engine::COMPONENTTYPE::COM_MESH));
		NULL_CHECK_RETURN(pPlayerMeshCom, E_FAIL);

		const Engine::D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("b_Weapon_L");

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		Engine::CTransform*		pPlayerTransCom = dynamic_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));
		NULL_CHECK_RETURN(pPlayerTransCom, E_FAIL);

		m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrixPointer();
	}

	return S_OK;
}

_int CCutlass::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (TRUE == m_bRender)
	{
		if (TRUE == m_bThrow)
		{
			m_pTransCom->m_vAngle.x = -90.f;
			if (0 == m_uPattern)
				m_pTransCom->m_vAngle.y = -90.f;
			else if(1 == m_uPattern)
				m_pTransCom->m_vAngle.y = -90.f;
			else if (2 == m_uPattern)
				m_pTransCom->m_vAngle.y = 180.f;
			m_pTransCom->m_vScale = _vec3(0.01f, 0.01f, 0.01f);

			_vec3 vDir;
			_vec3 vElitePos;

			if (0 == m_uPattern)
			{
				vDir = static_cast<Engine::CTransform*>(Engine::Find_PlayerObject()->Get_Component(Engine::COM_TRANSFORM))->m_vInfo[Engine::INFO_LOOK];
			}
			else if (1 == m_uPattern || 2 == m_uPattern)
			{
				for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECT_ELITE))
				{
					if (1 == m_uPattern
						&& 5 == static_cast<CElite*>(iter)->Get_EliteID())
					{
						m_pElite = static_cast<CElite*>(iter);
						Engine::CTransform* pEliteTransCom = static_cast<Engine::CTransform*>(m_pElite->Get_Component(Engine::COM_TRANSFORM));
						vElitePos = pEliteTransCom->Get_InfoPos();

						vDir = vElitePos - m_pTransCom->Get_InfoPos();
						vDir.y += 1.4f;
						break;
					}
					else if (2 == m_uPattern
							&& 6 == static_cast<CElite*>(iter)->Get_EliteID())
					{
						m_pElite = static_cast<CElite*>(iter);
						Engine::CTransform* pEliteTransCom = static_cast<Engine::CTransform*>(m_pElite->Get_Component(Engine::COM_TRANSFORM));
						vElitePos = pEliteTransCom->Get_InfoPos();

						vDir = vElitePos - m_pTransCom->Get_InfoPos();
						vDir.y += 1.2f;
						break;
					}
				}
			}

			D3DXVec3Normalize(&vDir, &vDir);
			m_pTransCom->Move_Pos(&(vDir * fTimeDelta * 11.f));

			m_pTransCom->m_vInfo[Engine::INFO_LOOK] = vDir;
			D3DXVec3Cross(&m_pTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &vDir);
			D3DXVec3Cross(&m_pTransCom->m_vInfo[Engine::INFO_UP], &vDir, &m_pTransCom->m_vInfo[Engine::INFO_RIGHT]);
			D3DXVec3Normalize(&m_pTransCom->m_vInfo[Engine::INFO_UP], &m_pTransCom->m_vInfo[Engine::INFO_UP]);

			/*  Scale  */
			for (Engine::_uint i = 0; i < Engine::INFO::INFO_POS; ++i)
			{
				D3DXVec3Normalize(&m_pTransCom->m_vInfo[i], &m_pTransCom->m_vInfo[i]);
				m_pTransCom->m_vInfo[i] *= *(((Engine::_float*)&m_pTransCom->m_vScale) + i);
			}

			/*  Rot  */
			_matrix		matRot[Engine::ROT_END];
			D3DXMatrixRotationX(&matRot[Engine::ROT_X], m_pTransCom->m_vAngle.x);
			D3DXMatrixRotationY(&matRot[Engine::ROT_Y], m_pTransCom->m_vAngle.y);
			D3DXMatrixRotationZ(&matRot[Engine::ROT_Z], m_pTransCom->m_vAngle.z);
			for (_uint i = 0; i < Engine::INFO_POS; ++i)
			{
				for (_uint j = 0; j < Engine::ROT_END; ++j)
				{
					D3DXVec3TransformNormal(&m_pTransCom->m_vInfo[i], &m_pTransCom->m_vInfo[i], &matRot[j]);
				}
			}

			/*  World Set  */
			for (Engine::_uint i = 0; i < Engine::INFO::INFO_END; ++i)
				memcpy(&m_pTransCom->m_matWorld.m[i][0], &m_pTransCom->m_vInfo[i], sizeof(Engine::_vec3));

			/*  Pattern  */
			if (0 == m_uPattern)
			{
				_vec3 vPlayerPos;
				memcpy(&vPlayerPos, &m_pParentWorldMatrix->m[3], sizeof(_vec3));

				_vec3 vDist = vPlayerPos - m_pTransCom->Get_InfoPos();

				if (10.f < D3DXVec3Length(&vDist))
				{
					m_bRender = FALSE;
					m_bThrow = FALSE;
					m_uPattern++;
				}
			}
			else if (1 == m_uPattern || 2 == m_uPattern)
			{
				_vec3 vDist = vElitePos - m_pTransCom->Get_InfoPos();
				vDist.y = 0.f;
				
				if (0.1f > D3DXVec3Length(&vDist))
				{
					if (m_pElite)
					{
						if (ELITE_CUTLASS_DEAD != m_pElite->Get_CurState()
							&& ELITE_DEAD != m_pElite->Get_CurState())
						{
							m_pElite->Set_CurState(ELITE_STATE::ELITE_CUTLASS_DEAD);
							m_pElite->Get_EliteBehavior()->Set_OnePlayAni(0.85);
						}
						if (ELITE_DEAD == m_pElite->Get_CurState())
						{
							m_bRender = FALSE;
							m_bThrow = FALSE;
							m_uPattern++;
							m_pElite = nullptr;
						}
					}
				}
			}
		}
		else if (FALSE == m_bThrow)
		{
			ZeroMemory(&m_pTransCom->m_vAngle, sizeof(_vec3));
			for (_uint i = 0; i < Engine::INFO_END; ++i)
				ZeroMemory(&m_pTransCom->m_vInfo[i], sizeof(_vec3));
			m_pTransCom->Update_Component();

			m_pTransCom->Set_ParentMatrix(&((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix)));
		}

		m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);
	}

	/*  CollisionMgr AddObject  */
	//DWORD dwFlag = COL_FLAG_SPHERE_SOUR;
	//Engine::AddObject_CollisionMgr(dwFlag, this);

	/*  Coll False Init  */
	m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);

	return UPDATE_OK;
}

_int CCutlass::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	return UPDATE_OK;
}

void CCutlass::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CCutlass::Render_Object(void)
{
	PreRender_Object();

	/*  Static Mesh, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		
		m_pMeshCom->Render_Meshe(pEffect);

		pEffect->End();
	}

	m_pSphereColliderCom->Render_Collider(&m_pTransCom->m_matWorld, FALSE);

	PostRender_Object();
}

void CCutlass::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_int CCutlass::CollisionUpdate_Object(const _float& fTimeDelta, 
										const DWORD _dwFlag, 
										const Engine::COL_OBJECTID _eColObjectID, 
										const _float& _fColValue /*= 0.f*/, 
										Engine::CGameObject* _pObject /*= nullptr*/)
{
	if (_dwFlag & COL_FLAG_SPHERE_SOUR)
	{
		m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);
	}

	return UPDATE_OK;
}

HRESULT CCutlass::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void CCutlass::Set_Throw(const _bool& _bActive)
{
	m_bThrow = _bActive; 
	
	_vec3 vPlayerPos, vPlayerLook;
	memcpy(&vPlayerLook, &m_pParentWorldMatrix->m[2], sizeof(_vec3));
	memcpy(&vPlayerPos, &m_pParentWorldMatrix->m[3], sizeof(_vec3));

	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	m_pTransCom->m_vInfo[Engine::INFO_LOOK] = vPlayerLook;
	D3DXVec3Cross(&m_pTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &vPlayerLook);
	D3DXVec3Cross(&m_pTransCom->m_vInfo[Engine::INFO_UP], &vPlayerLook, &m_pTransCom->m_vInfo[Engine::INFO_RIGHT]);
	D3DXVec3Normalize(&m_pTransCom->m_vInfo[Engine::INFO_UP], &m_pTransCom->m_vInfo[Engine::INFO_UP]);

	vPlayerPos += m_pTransCom->m_vInfo[Engine::INFO_LOOK] * 0.65f;
	vPlayerPos.y += 1.05f;
	m_pTransCom->Set_Pos(&vPlayerPos);
}

HRESULT CCutlass::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	
	/*  Mesh  */
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Dagger_00"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_MESH, pComponent);

	/*  Transform  */
	pComponent = m_pTransCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_TRANSFORM, pComponent);
	
	/*  Renderer  */
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_RENDERER, pComponent);

	/*  Sphere Collider  */
	pComponent = m_pSphereColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, 0.08f, 25, 24);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SPHERE_COLLIDER, pComponent);

	/*  Shader  */
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"NewShader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_NEWSHADER_MESH, pComponent);

	return S_OK;
}

CCutlass* CCutlass::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CCutlass*	pInstance = new CCutlass(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCutlass::Free(void)
{
	Engine::CGameObject::Free();
}