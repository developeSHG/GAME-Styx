#include "stdafx.h"
#include "Elite.h"

#include "Export_Function.h"

#include "PlayerBehavior.h"

#include "EliteBehavior.h"
#include "EliteMarker.h"
#include "EliteState.inl"

CElite::CElite(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
	m_eOwnerObjectID = Engine::OBJECTID::OBJECT_ELITE;
	m_eColObjectID = Engine::COL_OBJECTID::COL_OBJECT_ELITE;
}

CElite::~CElite(void)
{

}

HRESULT CElite::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_AnimationSet(151);

	/*  Player  */
	m_pPlayer = static_cast<CPlayer*>(Engine::Find_PlayerObject());
	m_pPlayerTransCom = static_cast<Engine::CTransform*>(m_pPlayer->Get_Component(Engine::COMPONENTTYPE::COM_TRANSFORM));
	m_pPlayerMeshCom = static_cast<Engine::CDynamicMesh*>(m_pPlayer->Get_Component(Engine::COMPONENTTYPE::COM_MESH));

	m_pEliteBehavior->Set_Player(m_pPlayer);

	return S_OK;
}

_int CElite::Update_Object(const _float& fTimeDelta)
{
	if (TRUE == m_bHide)
		return UPDATE_OK;

	Engine::CGameObject::Update_Object(fTimeDelta);
	
	/*  Elite ID11. View  */
	if (Engine::Get_DIKeyDown(DIK_8)
		&& 11 == m_uEliteID)
	{
		if (FALSE == static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Get_ElitePerson())
		{
			static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_ElitePerson(TRUE);
			_vec3 vPos = m_pTransCom->Get_InfoPos();
			vPos += m_pTransCom->m_vInfo[Engine::INFO_LOOK] * 3.f;
			vPos.y += 2.f;
			static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_Eye(&vPos);
		}
		else
			static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_ElitePerson(FALSE);
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_NONALPHA, this);

	/*  MoveOn NaviMesh Check  */
	if (ELITE_STATE::ELITE_HIDE != m_eCurState
		&& ELITE_STATE::ELITE_TRUNK_IN != m_eCurState
		&& ELITE_STATE::ELITE_CUPBOARD_IN != m_eCurState
		&& ELITE_STATE::ELITE_TRUNK_CATCH != m_eCurState
		&& ELITE_STATE::ELITE_CUPBOARD_CATCH != m_eCurState
		&& ELITE_STATE::ELITE_CATCH_ATTACK != m_eCurState
		&& 12 != m_uEliteID )
	{
		m_bIsOnLand = m_pNaviMeshCom->MoveOn_NaviMeshElite(&m_pTransCom->m_vInfo[Engine::INFO_POS],
															&m_vNewPos,
															m_bCompare);
	}

	/*  Coll False Init  */
	m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_FALSE);

	return UPDATE_OK;
}

_int CElite::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	/*  Update IsOnLand  */
	{
		/*  Compare Move  */
		if (TRUE == m_bCompare)
		{
			
		}
		/*  Compare Stop / Sliding  */
		else if (FALSE == m_bCompare)
		{
			
		}
		m_pTransCom->Set_Pos(&m_vNewPos);
	}

	/*  State Check  */
	Update_State();

	/*  Static Transform Update  */
	StaticTransform_Update();

	return UPDATE_OK;
}

void CElite::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CElite::Render_Object(void)
{
	m_pMeshCom->Play_Animation(m_fTimeDelta);

	/*  Frustum Culling  */
	if (false == m_pOptimizationCom->Is_InFrustum_ForObject(&m_pTransCom->m_vInfo[Engine::INFO::INFO_POS], 2.f))
		return;

	/*  CollisionMgr AddObject  */
	DWORD dwFlag = COL_FLAG_SPHERE_SOUR;
	Engine::AddObject_CollisionMgr(dwFlag, this);

	PreRender_Object();

	/*  Dynamic Mesh, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(0);
		
		m_pMeshCom->Render_Meshe(pEffect);

		pEffect->EndPass();
		pEffect->End();
	}

	m_pSphereColliderCom->Render_Collider(&m_pTransCom->m_matWorld);

	if (m_pSword)
		m_pSword->Update_Object(m_fTimeDelta);

	if (m_pCup)
		m_pCup->Update_Object(m_fTimeDelta);

	if (m_pBroom)
		m_pBroom->Update_Object(m_fTimeDelta);

	if (m_pEliteMarker
		&& TRUE == m_bSwordActive
		&& TRUE == m_bLive
		&& ELITE_SWORD_ACTIVEOUT != m_eCurState)
		m_pEliteMarker->Update_Object(m_fTimeDelta);

	PostRender_Object();
}

void CElite::PostRender_Object(void)
{

}

_int CElite::CollisionUpdate_Object(const _float& fTimeDelta, 
									const DWORD _dwFlag, 
									const Engine::COL_OBJECTID _eColObjectID, 
									const _float& _fColValue /*= 0.f*/, 
									Engine::CGameObject* _pObject /*= nullptr*/)
{
	if (_dwFlag & COL_FLAG_SPHERE_SOUR)
	{
		if (Engine::COL_OBJECTID::COL_OBJECT_PLAYER == _eColObjectID
			&& FALSE == m_pPlayer->Get_DeadState()
			&& FALSE == m_bHide)
		{
			m_pSphereColliderCom->Set_IsCollide(Engine::COLLTYPE::COL_TRUE);

			if (TRUE == m_bLive)
			{
				/*  OnMoveWay  */
				if (FALSE == m_bSwordActive)
				{
					/*if (ELITE_STATE::ELITE_WALK == m_eCurState || ELITE_STATE::ELITE_IDLE == m_eCurState
						|| ELITE_STATE::ELITE_IDLE_TURN_L180 == m_eCurState || ELITE_STATE::ELITE_IDLE_TURN_L90 == m_eCurState
						|| ELITE_STATE::ELITE_IDLE_TURN_R180 == m_eCurState || ELITE_STATE::ELITE_IDLE_TURN_R90 == m_eCurState)*/
					if (9 != m_uEliteID
						&& 10 != m_uEliteID
						&& 11 != m_uEliteID)
					{
						if (ELITE_STATE::ELITE_ASSASS_IN != m_eCurState
							&& ELITE_STATE::ELITE_ASSASS_LOOP != m_eCurState
							&& ELITE_STATE::ELITE_ASSASS_OUT != m_eCurState)
							static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"암살");
					}
					else if (9 == m_uEliteID
							|| 10 == m_uEliteID)
					{
						static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"근무교대");

						if (Engine::Get_DIKeyDown(DIK_E)
							&& 9 == m_uEliteID)
						{
							if (m_uEliteID == Engine::Get_EliteWave())
								m_bWorkChange = TRUE;
						}
					}
				}
				/*  No OnMoveWay  */
				else if (TRUE == m_bSwordActive)
				{
					if (11 == m_uEliteID)
					{
						if (TRUE == static_cast<CGrid*>(Engine::Find_ObjectListBack(Engine::OBJECT_GRID))->Get_Open())
							static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"암살");
					}
					if (ELITE_STATE::ELITE_ATTACK_COUNTER == m_eCurState)
					{
						if (m_pMeshCom->Is_AnimationSetRateEnd(0.6))
						{
							static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"살해");

							if (Engine::Get_DIKeyDown(DIK_E)
								&& FALSE == m_bFrontAss)
							{
								m_bFrontAss = TRUE;
								m_pPlayer->Get_PlayerBehavior()->Set_SinkTime(TRUE);
							}
						}
					}

					/*  Attack State Animation  */
					if (ELITE_STATE::ELITE_ATTACK_UP == m_eCurState
						|| ELITE_STATE::ELITE_ATTACK_LEFT == m_eCurState
						|| ELITE_STATE::ELITE_ATTACK_RIGHT == m_eCurState)
						//144 == m_pMeshCom->Get_AnimationIndex()
					{
						if (Engine::Get_DIMouseDown(Engine::DIM_LB))
							m_bCounterKey = TRUE;
				
						if (!m_pMeshCom->Is_AnimationSetRateEnd(0.7))
							static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_MOUSE, L"팅겨내기");
						/*  Counter  */
						if (m_pMeshCom->Is_AnimationSetRateEnd(0.35))
						{
							if (TRUE == m_bCounterKey)
							{
								if (PLAYER_STATE::PLAYER_FIGHT_COUNTER != m_pPlayer->Get_CurState()
									&& FALSE == m_bPlayerAniOverlap)
								{
									m_eCurState = ELITE_STATE::ELITE_ATTACK_COUNTER;
									m_pEliteBehavior->Set_OnePlayAni(0.88);
									m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_COUNTER);
									m_bPlayerAniOverlap = TRUE;
									Engine::MyPlaySound(L"SwordCrash.wav", Engine::CHANNELID::SWORD_CRASH);

								/*	m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_COUNTER);
									m_bPlayerAniOverlap = TRUE;*/
								}
							}
						}
						/*  Hurt  */
						if (m_pMeshCom->Is_AnimationSetRateEnd(0.45))
						{
							if (FALSE == m_bCounterKey)
							{
								if (PLAYER_STATE::PLAYER_FIGHT_HURT != m_pPlayer->Get_CurState()
									&& FALSE == m_bPlayerAniOverlap)
								{
									m_pPlayer->Set_NextState(PLAYER_STATE::PLAYER_FIGHT_HURT);
									m_bPlayerAniOverlap = TRUE;
									static_cast<CHp*>(Find_ObjectListFront(Engine::OBJECT_HP))->m_fuvX -= 0.3f;
									Engine::MyPlaySound(L"SwordDraw.mp3", Engine::CHANNELID::SWORD_DRAW);
								}
							}
							
						}

					}
				}
			}
			else if (FALSE == m_bLive)
			{
				//if (PLAYER_COL_ELITE & static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_ColTo())
				{
					if(ELITE_STATE::ELITE_ASSASS_OUT == m_eCurState
						|| ELITE_STATE::ELITE_DEAD == m_eCurState)
						static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_MOUSE, L"시체 옮기기");
					else if (ELITE_STATE::ELITE_CARRY_IDLE == m_eCurState
						|| ELITE_STATE::ELITE_CARRY_WALK == m_eCurState
						|| ELITE_STATE::ELITE_CARRY_RUN == m_eCurState)
						static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_MOUSE, L"시체 떨구기");
				}
			}

		}
	}

	return UPDATE_OK;
}

HRESULT CElite::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CElite::LateInit_Object(void)
{
	m_pEliteBehavior->Set_Elite(this);
	
	/*  Cup  */
	if (4 == m_uEliteID)
	{
		m_pCup = CCup::Create(m_pGraphicDev, this);
		NULL_CHECK_RETURN(m_pCup, E_FAIL);
	}

	/*  Broom  */
	if (5 == m_uEliteID)
	{
		m_pBroom = CBroom::Create(m_pGraphicDev, this);
		NULL_CHECK_RETURN(m_pBroom, E_FAIL);
	}
	/*  Sword  */
	else
	{
		m_pSword = CSword::Create(m_pGraphicDev, this);
		NULL_CHECK_RETURN(m_pSword, E_FAIL);
	}

	return S_OK;
}

void CElite::Set_InitEliteLook(const _vec3* _vPos)
{
	m_pTransCom->m_vInfo[Engine::INFO_LOOK] = *_vPos;
	D3DXVec3Normalize(&m_pTransCom->m_vInfo[Engine::INFO_LOOK], &m_pTransCom->m_vInfo[Engine::INFO_LOOK]);
	m_pEliteBehavior->Set_NewEliteLook(m_pTransCom->m_vInfo[Engine::INFO_LOOK]);
	D3DXVec3Cross(&m_pTransCom->m_vInfo[Engine::INFO_RIGHT], &Engine::_vec3(0.f, 1.f, 0.f), &m_pTransCom->m_vInfo[Engine::INFO_LOOK]);
	D3DXVec3Cross(&m_pTransCom->m_vInfo[Engine::INFO_UP], &m_pTransCom->m_vInfo[Engine::INFO_LOOK], &m_pTransCom->m_vInfo[Engine::INFO_RIGHT]);
	D3DXVec3Normalize(&m_pTransCom->m_vInfo[Engine::INFO_UP], &m_pTransCom->m_vInfo[Engine::INFO_UP]);
}

HRESULT CElite::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	
	/*  Mesh  */
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Monster_Elite"));
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

	/*  Sphere Collider  */
	//pComponent = m_pColliderCom = Engine::CCubeCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), m_pMeshCom->Get_Stride());
	pComponent = m_pSphereColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, 1, 25, 24 );
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_SPHERE_COLLIDER, pComponent);

	/*  Optimization  */
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_OPTIMIZATION, pComponent);

	/*  NaviMesh  */
	pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone(RESOURCEID::RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_NAVIMESH, pComponent);

	/*  Shader  */
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Component(Engine::COMPONENTID::ID_STATIC, L"NewShader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_NEWSHADER_MESH, pComponent);

	/*  Elite Component  */
	{
		/*  EliteBehavior  */
		pComponent = m_pEliteBehavior = CEliteBehavior::Create();
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::COMPONENTID::ID_DYNAMIC].emplace(Engine::COMPONENTTYPE::COM_ELITEBEHAVIOR, pComponent);
	}

	/*  EliteMarker  */
	m_pEliteMarker = CEliteMarker::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pEliteMarker, E_FAIL);
	m_pEliteMarker->Set_Elite(this);
	
	return S_OK;
}

void CElite::StaticTransform_Update(void)
{
	/*  Scale  */
	for (Engine::_uint i = 0; i < Engine::INFO::INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_pTransCom->m_vInfo[i], &m_pTransCom->m_vInfo[i]);
		m_pTransCom->m_vInfo[i] *= *(((Engine::_float*)&m_pTransCom->m_vScale) + i);
	}

	/*  World Set  */
	for (Engine::_uint i = 0; i < Engine::INFO::INFO_END; ++i)
		memcpy(&m_pTransCom->m_matWorld.m[i][0], &m_pTransCom->m_vInfo[i], sizeof(Engine::_vec3));
}

void CElite::Sink_PlayerCatch(void)
{
	Engine::CTransform* pHideTransform = static_cast<Engine::CTransform*>(m_pPlayer->Get_HideObject()->Get_Component(Engine::COM_TRANSFORM));
	
	_ushort uNum = 0;
	_bool bCheck = FALSE;
	_vec3 vPlayerPos = pHideTransform->Get_InfoPos();
	vPlayerPos.y = m_pPlayerTransCom->Get_InfoPos().y;

	for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_TRUNK))
	{
		if (iter == m_pPlayer->Get_HideObject())
		{
			bCheck = TRUE;
			break;
		}
		else
			uNum++;
	}

	if (0 == uNum)
		vPlayerPos.x += 0.055f;
	else if (1 == uNum)
		vPlayerPos.z -= 0.055f;

	if (FALSE == bCheck)
	{
		uNum = 0;
		vPlayerPos = pHideTransform->Get_InfoPos();
		vPlayerPos.y = m_pPlayerTransCom->Get_InfoPos().y;

		for (auto& iter : *Engine::Find_ObjectList(Engine::OBJECTID::OBJECT_CUPBOARD))
		{
			if (iter == m_pPlayer->Get_HideObject())
			{
				bCheck = TRUE;
				break;
			}
			else
				uNum++;
		}

		if (0 == uNum)
			vPlayerPos.x += 0.1f;
		else if (1 == uNum)
			vPlayerPos.z -= 0.1f;
		else if (2 == uNum)
			vPlayerPos.x += 0.1f;
	}

	if (TRUE == bCheck)
	{
		m_pPlayer->Set_NewPos(vPlayerPos);
		m_pPlayerTransCom->Set_Pos(&vPlayerPos);
		m_pPlayer->Get_PlayerBehavior()->Set_NewPlayerPos(vPlayerPos);

		m_pPlayer->Set_InitPlayerLook(&(pHideTransform->m_vInfo[Engine::INFO_LOOK]));
	}
}

CElite* CElite::Create(LPDIRECT3DDEVICE9 _pGraphicDev, const _ushort& _uID)
{
	CElite*	pInstance = new CElite(_pGraphicDev);

	pInstance->Set_EliteID(_uID);
	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CElite::Free(void)
{
	if (m_pCup)
		Engine::Safe_Release(m_pCup);

	if (m_pSword)
		Engine::Safe_Release(m_pSword);

	if (m_pBroom)
		Engine::Safe_Release(m_pBroom);

	if(m_pEliteMarker)
		Engine::Safe_Release(m_pEliteMarker);

	Engine::CGameObject::Free();
}