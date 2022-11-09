#include "stdafx.h"
#include "ItemList.h"

#include "PlayerBehavior.h"

#include "StaticCamera.h"

CItemList::CItemList(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{

}

CItemList::~CItemList(void)
{

}

HRESULT CItemList::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::Ready_Object(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(974.f, -390.f, 1.f);
	m_pTransCom->m_vScale = { 130.f, 130.f, 1.f };

	m_pTransCom->Update_Component();

	m_vecItemNum.reserve(6);
	for (_ushort i = 0; i < 6; ++i)
		m_vecItemNum.push_back(0);

	return S_OK;
}

_int CItemList::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (Engine::Get_DIKeyDown(DIK_NUMPAD1))
	{
		if (0 > --m_wItemSelect)
			m_wItemSelect = 5;
	}
	else if (Engine::Get_DIKeyDown(DIK_NUMPAD3))
	{
		if (5 < ++m_wItemSelect)
			m_wItemSelect = 0;
	}

	if (TRUE == m_bTimeActive)
	{
		m_dTime += 1.f * fTimeDelta;

		if (0.5 < m_dTime)
		{
			static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_NextState(PLAYER_STATE::PLAYER_THROW_CUTLASS);
			static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_AnimationSet(140);
			static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_PlayerBehavior()->Set_OnePlayAni(0.95);

			m_dTime = 0;
			m_bTimeActive = FALSE;

			Engine::MyPlaySound(L"Cutlass.mp3", Engine::CHANNELID::PLAYER);
		}
	}

	return UPDATE_OK;
}

_int CItemList::LateUpdate_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDERID::RENDER_UI, this);

	/*  Select Item  */
	Select_Item();

	return UPDATE_OK;
}

void CItemList::PreRender_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CItemList::Render_Object(void)
{
	PreRender_Object();

	/*  RcTex Buffer, Shader Render  */
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		SetUp_ConstantTable(pEffect);

		_uint iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(1);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}

	_tchar	szNum[32];
	swprintf_s(szNum, L"%d", m_vecItemNum[m_wItemSelect]);

	_vec2 vPos = _vec2((m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].x / 2.f + WINCX / 2.f) + 26.5f,
						(WINCY / 2.f - m_pTransCom->m_vInfo[Engine::INFO::INFO_POS].y / 2.f) - 45.f);
	Engine::Render_Font(L"Font_UI", szNum, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	PostRender_Object();
}

void CItemList::PostRender_Object(void)
{
	
}

HRESULT CItemList::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Render_Texture(pEffect, "g_BaseTexture", m_wItemSelect);

	Safe_Release(pEffect);

	return S_OK;
}

void CItemList::Set_AddItem(const _ushort& _uIndex)
{
	m_wItemSelect = _uIndex;
	m_vecItemNum[m_wItemSelect]++;
}

void CItemList::Set_MiusItem(const _ushort& _uIndex)
{
	m_wItemSelect = _uIndex;
	m_vecItemNum[m_wItemSelect]--;
}

HRESULT CItemList::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	/*  Buffer  */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COMPONENTID::ID_STATIC].emplace(Engine::COMPONENTTYPE::COM_BUFFER, pComponent);

	/*  Texture  */
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Item"));
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

void CItemList::Select_Item()
{
	if (PLAYER_STATE::PLAYER_DEAD_IN == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState()
		|| PLAYER_STATE::PLAYER_DEAD == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CurState()
		|| TRUE == static_cast<CLowerText*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_LOWERTEXT))->Get_MakeSoundActive())
		return;

	if (0 < m_vecItemNum[m_wItemSelect])
	{ 
		/*  Col False  */
		//if ((PLAYER_COL_END | static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_ColTo()))
		if (Engine::COL_FALSE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_IsCollide())
		{
			if (FALSE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_EliteState()
				&& FALSE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_Translucent())
			{
				/*  Hp, Mystery  */
				if (0 == m_wItemSelect
					|| 1 == m_wItemSelect)
					static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"사용하기");

				/*  Cutlass  */
				else if (2 == m_wItemSelect)
				{
					if (CAMERA_PURPOSE::PURPOSE_FIRST_PERSON != static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Get_CameraPurpose())
						static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"조준하기");
					else
					{
						static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"투척");
						static_cast<CButton*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_C, L"돌아가기");
					}
				}

				/*  Map  */
				else if (4 == m_wItemSelect)
					static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"지도 확인");

				/*  Key Not  */
				if (5 != m_wItemSelect)
				{
					if (CAMERA_PURPOSE::PURPOSE_FIRST_PERSON != static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Get_CameraPurpose())
						static_cast<CButton*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_C, L"버리기");
				}

				/*  Col False Use Item  */
				ColFalse_Use_Item();
			}
			else if (TRUE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_EliteState())
			{
				/*  Mystery  */
				if (1 == m_wItemSelect)
				{
					if (FALSE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_Translucent())
					{
						static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"사용하기");

						if (Engine::Get_DIKeyDown(DIK_E))
						{
							static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_NextState(PLAYER_STATE::PLAYER_DRINK_POSITION);
							static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_PlayerBehavior()->Set_OnePlayAni(0.85);
							Set_MiusCurItem();
						}
					}
				}
			}
		}
		/*  Col True  */
		else if (Engine::COL_TRUE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_IsCollide())
		{
			/*  Mystery  */
			if (1 == m_wItemSelect)
			{
				if (PLAYER_COL_ELITE & static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_ColTo()
					&& FALSE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_EliteState()
					&& FALSE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_Translucent())
				{
					if(FALSE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_CollToElite()->Get_Live())
						static_cast<CButton*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"사용하기");
				}
			}

			/*  Col False Use Item  */
			ColTrue_Use_Item();
		}

		/*  MapOnly  */
		if (Engine::COL_TRUE == static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_IsCollide()
			&& PLAYER_COL_SERVANT_QUEST & static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_ColTo())
		{
			/*  Map  */
			if (4 == m_wItemSelect)
			{ 
				static_cast<CButton*>(Engine::Find_ObjectListFront(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_E, L"지도 확인");
				static_cast<CButton*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BUTTON))->Set_ButtonSetting(BUTTON_C, L"버리기");
			
				if (Engine::Get_DIKeyDown(DIK_E))
				{
					static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_DynamicQuestTrigger(TRUE);
					Set_MiusCurItem();

					static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_PlayerBehavior()->Set_CameraPurpose(TRUE);
					static_cast<CServant_Quest*>(Engine::Find_ObjectListBack(Engine::OBJECT_SERVANT_QUEST))->Set_CameraPurpose(TRUE);
				}
			}
		}
	}
}

void CItemList::ColFalse_Use_Item()
{
	/*  Use  */
	if (Engine::Get_DIKeyDown(DIK_E))
	{
		/*  Hp, Mystery  */
		if (0 == m_wItemSelect
			|| 1 == m_wItemSelect)
		{
			static_cast<CPlayer*>(Engine::Find_PlayerObject())->Set_NextState(PLAYER_STATE::PLAYER_DRINK_POSITION);
			static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_PlayerBehavior()->Set_OnePlayAni(0.95);

			if (0 == m_wItemSelect)
				static_cast<CHp*>(Find_ObjectListFront(Engine::OBJECT_HP))->m_fuvX += 0.275f;

			Engine::MyPlaySound(L"Drink.wav", Engine::CHANNELID::PLAYER);
		}

		/*  Dagger  */
		else if (2 == m_wItemSelect)
		{
			if (CAMERA_PURPOSE::PURPOSE_FIRST_PERSON != static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Get_CameraPurpose())
			{
				static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_FIRST_PERSON);

				/*  Blind Render True  */
				static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(TRUE);
				static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Index(4);
			}
			/*  투척  */
			else
			{
				static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_END);
				
				/*  Blind False  */
				static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(FALSE);

				m_bTimeActive = TRUE;

				Set_MiusCurItem();
			}
		}

		/*  Map  */
		else if (4 == m_wItemSelect)
		{
			static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_DynamicQuestTrigger(TRUE);

			static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_PlayerBehavior()->Set_CameraPurpose(TRUE);
			static_cast<CServant_Quest*>(Engine::Find_ObjectListBack(Engine::OBJECT_SERVANT_QUEST))->Set_CameraPurpose(TRUE);

			Set_MiusCurItem();	
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_C))
	{
		/*  Dagger  */
		if (2 == m_wItemSelect)
		{
			/*  돌아가기  */
			if (CAMERA_PURPOSE::PURPOSE_FIRST_PERSON == static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Get_CameraPurpose())
			{
				static_cast<CStaticCamera*>(Engine::Find_StaticCameraObject())->Set_CameraPurpose(CAMERA_PURPOSE::PURPOSE_END);

				/*  Blind False  */
				static_cast<CBlind*>(Engine::Find_ObjectListBack(Engine::OBJECTID::OBJECT_BLIND))->Set_Render(FALSE);
			}
		}
	}
}

void CItemList::ColTrue_Use_Item(void)
{
	/*  Use  */
	//if (Engine::Get_DIKeyDown(DIK_E))
	//{
	//	/*  Key  */
	//	if (PLAYER_COL_LEVER | static_cast<CPlayer*>(Engine::Find_PlayerObject())->Get_ColTo()
	//		&& 0 < m_vecItemNum[5])
	//	{
	//		Set_MiusItem(5);
	//	}
	//}
}

CItemList* CItemList::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CItemList*	pInstance = new CItemList(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CItemList::Free(void)
{
	//for_each(m_vecItemList.begin(), m_vecItemList.end(), [&](auto& iter) {
	//	iter.clear();
	//});
	m_vecItemNum.clear();
	m_vecItemNum.shrink_to_fit();

	Engine::CGameObject::Free();
}