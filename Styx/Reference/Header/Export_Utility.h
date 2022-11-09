#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Include.h"

#include "Management.h"
#include "ComponentMgr.h"
#include "KeyMgr.h"
#include "ObjectMgr.h"
#include "Renderer.h"
#include "LightMgr.h"
#include "CollisionMgr.h"
#include "NaviMgr.h"
#include "Shader.h"
#include "ResourcesMgr.h"

BEGIN(Engine)

/*  Management  */
// Set
inline HRESULT				SetUp_Scene(CScene* pScene, _ushort _wSceneID);
inline void					Set_EliteWave();
// Get
inline const _ushort		Get_CurScene();
inline const _ushort		Get_EliteWave();
// General
inline HRESULT				Create_Management(CManagement** ppManagement);

/*  ComponentMgr  */
// Set
inline void					Set_ComponentMgrGraphicDev(LPDIRECT3DDEVICE9 _pDevice);
//inline HRESULT				Set_ObjectComponent(const _tchar* _pObjTag,
//												const _tchar* _pComponentTag,
//												CComponent* _pComponent);
// Get
//inline CComponent*			Get_ObjectComponent(const _tchar* _pObjTag,
//												const _tchar* _pComponentTag);
// General
inline HRESULT				Add_Component(const _ushort& _wContainerIdx,
											const _tchar* _pComponentTag,
											COMPONENTTYPE _eType);
inline CComponent*			Clone_Component(const _ushort& _wContainerIdx,
											const _tchar* _pComponentTag);

/*  KeyMgr  */
// Set
// Get
// General
//inline void					Update_KeyMgr(void);
//inline bool					KeyDown_KeyMgr(DWORD dwKey);
//inline bool					KeyUp_KeyMgr(DWORD dwKey);
//inline bool					KeyPressing_KeyMgr(DWORD dwKey);
//inline bool					KeyCombined_KeyMgr(DWORD dwFirst, DWORD dwSecond);

/*  ObjectMgr  */
// Set
inline HRESULT			  	Add_ObjectMgr(const OBJECTID _eID,
						  				CGameObject* _pGameObject);
// Get
inline list<CGameObject*>*	Find_ObjectList(const OBJECTID _eID);
inline CGameObject*		  	Find_ObjectListFront(const OBJECTID _eID);
inline CGameObject*		  	Find_ObjectListBack(const OBJECTID _eID);
inline CGameObject*		  	Find_PlayerObject();
inline list<CGameObject*>*	Find_MonsterObjectList();
inline CGameObject*		  	Find_StaticCameraObject();
inline CGameObject*		  	Find_DynamicCameraObject();
// General
inline void				  	Free_ObjectList();

/*  LightMgr  */
// Set
inline void					Set_LightActive(const _uint& iIndex = 0, bool _bActive = TRUE);
// Get
inline D3DLIGHT9*			Get_LightInfo(const _uint& iIndex = 0);
inline bool					Get_LightActive(const _uint& iIndex = 0);
// General
inline HRESULT				Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, 
										const D3DLIGHT9* pLightInfo, 
										const _uint& iIndex);

/*  CollisionMgr  */
// General
inline HRESULT				AddObject_CollisionMgr(DWORD _dwFlag, CGameObject* _pObject);
inline void					LateUpdate_CollisionMgr(const _float& fTimeDelta);

/*  NaviMgr  */
// Get
inline CNaviMgr*			Get_NaviMgr();

/*  Renderer  */
// Get
inline CRenderer*			Get_Renderer(void);

/*  ResourcesMgr  */
// Set
// Get
// General
inline HRESULT			Reserve_ContainerSize(const _ushort& wSize);
inline HRESULT			Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pBufferTag, BUFFERID eID, const _ulong& dwCntX = 1, const _ulong& dwCntZ = 1, const _ulong& dwVtxItv = 1);
inline HRESULT			Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt = 1);
inline HRESULT			Ready_Meshe(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName);
inline void				Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag);
inline void				Render_Texture(const _ushort& wContainerIdx, const _tchar* pTextureTag, const _uint& iIndex = 0);
inline CResources*		Clone(const _ushort& wContainerIdx, const _tchar* pResourceTag);


/*  Release Utility  */
inline void		Release_Utility(void);

#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
