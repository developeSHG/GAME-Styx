#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "Engine_Include.h"
#include "Include.h"

#include "World.h"

class CSceneSelector
{
public:
	explicit CSceneSelector(TOOLSCENEID _eSceneID) { m_eSceneID = _eSceneID; }
	~CSceneSelector(){}

public:
	Engine::CScene* operator()(Engine::CScene** _pScene, LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	TOOLSCENEID		m_eSceneID;
};

inline Engine::CScene* CSceneSelector::operator()(Engine::CScene** _pScene, LPDIRECT3DDEVICE9 _pGraphicDev)
{
	if (nullptr != *_pScene)
		Engine::Safe_Release(*_pScene);

	switch (m_eSceneID)
	{
	case TOOLSCENEID::TOOLSCENE_WORLD:
		*_pScene = CWorld::Create(_pGraphicDev);
		break;

	}

	return *_pScene;
}

#endif // SceneSelector_h__
