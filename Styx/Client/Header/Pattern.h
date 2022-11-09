#ifndef Pattern_h__
#define Pattern_h__

/************************ (SongSong) ************************/
/*  Client Pattern.h  */

#include "Engine_Include.h"
#include "GameObject.h"

/************************ (SongSong) ************************/
/*  Logo  */
#include "BackGround.h"
#include "Title.h"
#include "TitleLight.h"
#include "MeshLoading.h"
#include "PressEnter.h"
#include "CopyRight.h"
#include "Cyanide.h"
#include "Enreal.h"
#include "Focus.h"
#include "Jusin.h"
#include "Pegi.h"
#include "Ps4.h"
#include "Styx.h"
#include "DrawBridge.h"
#include "SkyBoxL.h"
#include "SkyBoxWhiteL.h"


/************************ (SongSong) ************************/
/*  Stage  */
#include "Player.h"
#include "Monster.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Catsle.h"
#include "Stone.h"
#include "Prison.h"
#include "Elite.h"
#include "Servant_Quest.h"
#include "Servant_Rescue.h"
#include "Dagger.h"
#include "Sword.h"
#include "Door.h"
#include "Cupboard.h"
#include "Lever.h"
#include "Torch.h"
#include "Trunk.h"
#include "Grid.h"
#include "Spark.h"
#include "Cutlass.h"
#include "Broom.h"
#include "Cup.h"
#include "PlayerSword.h"
#include "Feature.h"

/************************ (SongSong) ************************/
/*  UI  */
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "CameraHierarchy.h"
#include "CameraPoint.h"
#include "SceneAlpha.h"
#include "Button.h"
#include "Mission.h"
#include "Timer.h"
#include "ItemList.h"
#include "ItemSelectCircle.h"
#include "ItemNumCircle.h"
#include "ItemPortrait.h"
#include "HpBar.h"
#include "MpBar.h"
#include "Hp.h"
#include "Mp.h"
#include "LowerText.h"
#include "LowerTextTuto.h"
#include "Blind.h"
#include "Marker.h"
#include "Cursor.h"

template <typename T>
class CAbstractFactoryPattern
{
public:
	static Engine::CGameObject*	CreateObject(LPDIRECT3DDEVICE9 _pGraphicDev)
	{
		return T::Create(_pGraphicDev);
	}
	static Engine::CGameObject*	CreateFeatureObject(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::_tchar* _pMeshTag)
	{
		return T::Create(_pGraphicDev, _pMeshTag);
	}
};



#endif // Pattern_h__
