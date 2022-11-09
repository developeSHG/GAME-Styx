#ifndef Pattern_h__
#define Pattern_h__

/************************ (SongSong) ************************/
/*  Tool Pattern.h  */

#include "Engine_Include.h"
#include "GameObject.h"

/************************ (SongSong) ************************/
/*  World  */
#include "Terrain.h"
#include "Catsle.h"
#include "Prison.h"
#include "SkyBox.h"
#include "Feature.h"
#include "DynamicFeature.h"

/************************ (SongSong) ************************/
/*  UI  */
#include "DynamicCamera.h"
#include "CameraHierarchy.h"
#include "CameraPoint.h"
#include "NaviTri.h"

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
