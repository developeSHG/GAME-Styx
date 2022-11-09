/*  Management  */
// Set
HRESULT		SetUp_Scene(CScene* pScene, _ushort _wSceneID)
{
	return CManagement::GetInstance()->SetUp_Scene(pScene, _wSceneID);
}
void		Set_EliteWave()
{
	return CManagement::GetInstance()->Set_EliteWave();
}
// Get
const _ushort	Get_CurScene()
{
	return CManagement::GetInstance()->Get_CurScene();
}
const _ushort	Get_EliteWave()
{
	return CManagement::GetInstance()->Get_EliteWave();
}
// General
HRESULT		Create_Management(CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	*ppManagement = pManagement;

	return S_OK;
}

/*  ComponentMgr  */
// Set
void				Set_ComponentMgrGraphicDev(LPDIRECT3DDEVICE9 _pDevice)
{
	CComponentMgr::GetInstance()->Set_ComponentMgrGraphicDev(_pDevice);
}
//HRESULT				Set_ObjectComponent(const _tchar* _pObjTag,
//										const _tchar* _pComponentTag,
//										CComponent* _pComponent)
//{
//	return CComponentMgr::GetInstance()->Set_ObjectComponent(_pObjTag, _pComponentTag, _pComponent);
//}
// Get
//CComponent*	Get_ObjectComponent(const _tchar* _pObjTag,
//								const _tchar* _pComponentTag)
//{
//	return CComponentMgr::GetInstance()->Get_ObjectComponent(_pObjTag, _pComponentTag);
//}
// General
HRESULT				Add_Component(const _ushort& _wContainerIdx,
									const _tchar* _pComponentTag,
									COMPONENTTYPE _eType)
{
	return CComponentMgr::GetInstance()->Add_Component(_wContainerIdx, _pComponentTag, _eType);
}
CComponent*			Clone_Component(const _ushort& _wContainerIdx,
									const _tchar* _pComponentTag)
{
	return CComponentMgr::GetInstance()->Clone_Component(_wContainerIdx, _pComponentTag);
}

/*  KeyMgr  */
// Set
// Get
// General
//void				Update_KeyMgr(void)
//{
//	CKeyMgr::GetInstance()->Update();
//}
//bool				KeyDown_KeyMgr(DWORD dwKey)
//{
//	return CKeyMgr::GetInstance()->KeyDown(dwKey);
//}
//bool				KeyUp_KeyMgr(DWORD dwKey)
//{
//	return CKeyMgr::GetInstance()->KeyUp(dwKey);
//}
//bool				KeyPressing_KeyMgr(DWORD dwKey)
//{
//	return CKeyMgr::GetInstance()->KeyPressing(dwKey);
//}
//bool				KeyCombined_KeyMgr(DWORD dwFirst, DWORD dwSecond)
//{
//	return CKeyMgr::GetInstance()->KeyCombined(dwFirst, dwSecond);
//}

/*  ObjectMgr  */
// Set
HRESULT					Add_ObjectMgr(const Engine::OBJECTID _eID,
										CGameObject* _pGameObject)
{
	return CObjectMgr::GetInstance()->Add_ObjectMgr(_eID, _pGameObject);
}
// Get
list<CGameObject*>*		Find_ObjectList(const Engine::OBJECTID _eID)
{
	return CObjectMgr::GetInstance()->Find_ObjectList(_eID);
}
CGameObject*			Find_ObjectListFront(const Engine::OBJECTID _eID)
{
	return CObjectMgr::GetInstance()->Find_ObjectListFront(_eID);
}
CGameObject*			Find_ObjectListBack(const Engine::OBJECTID _eID)
{
	return CObjectMgr::GetInstance()->Find_ObjectListBack(_eID);
}
CGameObject*			Find_PlayerObject()
{
	return CObjectMgr::GetInstance()->Find_PlayerObject();
}
list<CGameObject*>*		Find_MonsterObjectList()
{
	return CObjectMgr::GetInstance()->Find_MonsterObjectList();
}
CGameObject*			Find_StaticCameraObject()
{
	return CObjectMgr::GetInstance()->Find_StaticCameraObject();
}
CGameObject*			Find_DynamicCameraObject()
{
	return CObjectMgr::GetInstance()->Find_DynamicCameraObject();
}
// General
void					Free_ObjectList()
{
	return CObjectMgr::GetInstance()->Free();
}

/*  LightMgr  */
// Set
void		Set_LightActive(const _uint& iIndex/* = 0*/, bool _bActive /*= TRUE*/)
{
	CLightMgr::GetInstance()->Set_LightActive(iIndex, _bActive);
}
// Get
D3DLIGHT9*		Get_LightInfo(const _uint& iIndex/* = 0*/)
{
	return CLightMgr::GetInstance()->Get_LightInfo(iIndex);
}
bool			Get_LightActive(const _uint& iIndex/* = 0*/)
{
	return CLightMgr::GetInstance()->Get_LightActive(iIndex);
}
// General
HRESULT					Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

/*  CollisionMgr  */
// General
HRESULT		AddObject_CollisionMgr(DWORD _dwFlag, CGameObject* _pObject)
{
	return CCollisionMgr::GetInstance()->AddObject_CollisionMgr(_dwFlag, _pObject);
}
void		LateUpdate_CollisionMgr(const _float& fTimeDelta)
{
	CCollisionMgr::GetInstance()->LateUpdate_CollisionMgr(fTimeDelta);
}

/*  NaviMgr  */
CNaviMgr*	Get_NaviMgr()
{
	return CNaviMgr::GetInstance();
}

/*  Renderer  */
// Get
CRenderer*		Get_Renderer(void)
{
	return CRenderer::GetInstance();
}

/*  ResourcesMgr  */
// Set
// Get
// General
HRESULT	Reserve_ContainerSize(const _ushort& wSize)
{
	return CResourcesMgr::GetInstance()->Reserve_ContainerSize(wSize);
}
HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pBufferTag, BUFFERID eID, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	return CResourcesMgr::GetInstance()->Ready_Buffer(pGraphicDev, wContainerIdx, pBufferTag, eID, dwCntX, dwCntZ, dwVtxItv);
}
HRESULT		Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt)
{
	return CResourcesMgr::GetInstance()->Ready_Texture(pGraphicDev, wContainerIdx, pTextureTag, eType, pPath, iCnt);
}
HRESULT		Ready_Meshe(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName)
{
	return CResourcesMgr::GetInstance()->Ready_Meshe(pGraphicDev, wContainerIdx, pMeshTag, eType, pFilePath, pFileName);
}
void		Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag)
{
	CResourcesMgr::GetInstance()->Render_Buffer(wContainerIdx, pBufferTag);
}
void		Render_Texture(const _ushort& wContainerIdx, const _tchar* pTextureTag, const _uint& iIndex)
{
	CResourcesMgr::GetInstance()->Render_Texture(wContainerIdx, pTextureTag, iIndex);
}
CResources*		Clone(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	return CResourcesMgr::GetInstance()->Clone(wContainerIdx, pResourceTag);
}



/*  Release Utility  */
void		Release_Utility(void)
{
	/*  순서들 잘 생각해서 지우기  */
	CResourcesMgr::GetInstance()->DestroyInstance();
	CNaviMgr::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	/*  Management가 Renderer보다 앞에 지워야함  */
	CManagement::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CComponentMgr::GetInstance()->DestroyInstance();
	//CKeyMgr::GetInstance()->DestroyInstance();
	CObjectMgr::GetInstance()->DestroyInstance();
}