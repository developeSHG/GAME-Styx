#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "Engine_Include.h"
#include "Include.h"

#include "Intro.h"
#include "Logo.h"
#include "Stage.h"

class CSceneSelector
{
public:
	explicit CSceneSelector(SCENEID _eSceneID) { m_eSceneID = _eSceneID; }
	~CSceneSelector(){}

public:
	Engine::CScene* operator()(Engine::CScene** _pScene, LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	SCENEID		m_eSceneID;
};

inline Engine::CScene* CSceneSelector::operator()(Engine::CScene** _pScene, LPDIRECT3DDEVICE9 _pGraphicDev)
{
	if (nullptr != *_pScene)
		Engine::Safe_Release(*_pScene);

	switch (m_eSceneID)
	{
	case SCENEID::SCENE_INTRO:
		*_pScene = CIntro::Create(_pGraphicDev);
		break;

	case SCENEID::SCENE_LOGO:
		*_pScene = CLogo::Create(_pGraphicDev);
		break;

	case SCENEID::SCENE_STAGE:
		*_pScene = CStage::Create(_pGraphicDev);
		break;
	}

	return *_pScene;
}

#endif // SceneSelector_h__
