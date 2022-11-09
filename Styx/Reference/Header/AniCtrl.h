#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAniCtrl(const CAniCtrl& rhs);
	virtual ~CAniCtrl(void);

public:
	HRESULT								Ready_AnimationCtrl(void);

public:
	/*  Active  */
	void								Play_Animation(const _float& fTimeDelta, const _bool& bUpDown = TRUE);
	_bool								Is_AnimationSetEnd(const _float& fTime = 0.1f);
	_bool								Is_AnimationSetRateEnd(const _double& dTime = 1.f);
	void								Render_AniTrack() const;

public:
	/*  Set  */
	void								Set_AnimationSet(const _uint& iIndex, 
															const _float& fCurTrackSpeed = 1.f, 
															const _float& fNewTrackSpeed = 1.f,
															const _float& fWeight = 0.1f,
															const _float& fDuration = 0.25f);
	void								Set_TrackPosition(const _double & dPos);

public:
	/*  Get  */
	LPD3DXANIMATIONCONTROLLER			Get_AniCtrl(void) { return m_pAniCtrl; }
	const _uint							Get_AnimationIndex() const { return m_iOldAniIdx; }

private:
	LPD3DXANIMATIONCONTROLLER			m_pAniCtrl;
	_uint								m_iCurrentTrack;
	_uint								m_iNewTrack;
	_float								m_fAccTime;
	_uint								m_iOldAniIdx;
	_double								m_dPeriod;


public:
	static	CAniCtrl*					Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static  CAniCtrl*					Create(const CAniCtrl& rhs);
public:
	virtual void						Free(void);
};

END
#endif // AniCtrl_h__
