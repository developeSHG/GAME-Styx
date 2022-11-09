#ifndef Intro_h__
#define Intro_h__

#include "Engine_Include.h"
#include "Export_Function.h"
#include "Include.h"
#include "Scene.h"

#include "Loading.h"

#include <locale>
#include <iostream>
#include <dshow.h>
#pragma comment(lib, "Strmiids")

class CIntro : public Engine::CScene
{
private:
	/*  Structor  */
	explicit CIntro(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CIntro(void);

public:
	// General
	virtual HRESULT			Ready_Scene(void) override;
	virtual _int			Update_Scene(const _float& fTimeDelta) override;
	virtual _int			LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void			Render_Scene(void) override;

private:
	HRESULT					Video_Play();

	IGraphBuilder* pGraph = NULL;
	IMediaControl* pControl = NULL;
	IMediaEvent* pEvent = NULL;
	IVideoWindow* pVideoWindow = NULL;

private:
	CLoading*				m_pLoading = nullptr;
	_bool					m_bVideoPlay = false;
	float					m_fTime = 0.f;
	_bool					m_bSound = false;

public:
	static CIntro*			Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void			Free(void);
};


#endif // Intro_h__
