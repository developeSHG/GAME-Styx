#include "AniCtrl.h"
#include <iostream>
USING(Engine)

Engine::CAniCtrl::CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
	, m_fAccTime(0.f)
	, m_iOldAniIdx(999)
	, m_dPeriod(0.0)
{
	m_pAniCtrl->AddRef();
}

Engine::CAniCtrl::CAniCtrl(const CAniCtrl& rhs)
	: m_iCurrentTrack(rhs.m_iCurrentTrack)
	, m_iNewTrack(rhs.m_iNewTrack)
	, m_fAccTime(rhs.m_fAccTime)
	, m_iOldAniIdx(rhs.m_iOldAniIdx)
	, m_dPeriod(0.0)
{
	// �ִϸ��̼��� ������ �Ǿ �ȵǱ� ������ ���ʿ� ���� �Լ��� �����Ѵ�
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),	// ���� �� ���� ��ü���� �����ǰ� �ִ� �ִϸ��̼��� ������ ��ȯ
											rhs.m_pAniCtrl->GetMaxNumAnimationSets(),		// ���� ������ �ִϸ��̼��� �ִ� ����(ù ��° ���ڰ��� ���� ����)
											rhs.m_pAniCtrl->GetMaxNumTracks(),				// Ʈ�� ����(�� ����-�ִϸ��̼� ��-�� Ʈ�� ���� �÷����� ����ϴ� ����, ���� ���� ��, Ʈ���� ��κ� �� ���� ��������� �ʿ信 ���� �� �� ������ ���)
											rhs.m_pAniCtrl->GetMaxNumEvents(),				// ���� �޽� �ȿ� ����Ǿ� �ִ� ��Ư�� ȿ���� ������ �ǹ�, ������ �츮�� ������� ���Ѵ�.
											&m_pAniCtrl);
}

Engine::CAniCtrl::~CAniCtrl(void)
{

}

HRESULT Engine::CAniCtrl::Ready_AnimationCtrl(void)
{
	return S_OK;
}

void Engine::CAniCtrl::Set_AnimationSet(const _uint& iIndex, 
										const _float& fCurTrackSpeed /*= 1.f*/, 
										const _float& fNewTrackSpeed /*= 1.f*/,
										const _float& fWeight /*= 0.1f*/,
										const _float& fDuration /*= 0.25f*/)
{
	if (m_iOldAniIdx == iIndex)
		return;

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);


	LPD3DXANIMATIONSET		pAS = nullptr;		// �ִϸ��̼� ���� ������ �����ϴ� ��ü
	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);
	// m_pAniCtrl->GetAnimationSetByName();

	// ���� �ִϸ��̼� ���� ��ü ��� �ð��� ��ȯ�ϴ� �Լ�
	m_dPeriod = pAS->GetPeriod();


	// �ִϸ��̼� ���� ������ ����.
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);


	// �̺�Ʈ(�޽� ���� �ִ� ����Ʈ)�� ������� ����(�̺�Ʈ�� ������ ���� ������ �ȵǴ� ��� �߻�)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// Ű ������ : �ִϸ��̼� ���� �� Ư�� ������ ������ �ǹ�, ���� ��� ���� ��� �� �Լ��� ȣ��Ǵ� ������ ������ Ű �������̶� �� �� �ִ�.
	// �ִϸ޽��� ��� ��� Ű �������� ���� �� �� ����. ��� ���۸� ���� �Ǿ� �־� ���� ������ ���� ���� ��� �ϰ� �ȴ�.

	// ���� ������ Ʈ���� ��� ������ �����ϴ� �Լ�, 3���� : �������� Ʈ���� ���¸� ������ ���ΰ�(�Ǽ� ������ double���� ���)
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + fDuration);

	// ���� ����ϴ� Ʈ���� �ӵ��� �����ϴ� �Լ�, �ӵ��� �⺻ ����� 1.f
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, fCurTrackSpeed, m_fAccTime, fDuration, D3DXTRANSITION_LINEAR);

	// �ִϸ��̼� �ռ� ��, � �������� �������� �� ������ ������ ������ �����ϴ� �Լ�(����ġ ���� : ����ġ ���� �� Ű �������� ���� 1�̾�߸� �Ѵ�)
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, fWeight, m_fAccTime, fDuration, D3DXTRANSITION_LINEAR);

	// Ʈ���� Ȱ��ȭ, Ȱ��ȭ �Լ��� �� ����ϴ� �Լ��� �ƴϴ�.
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, fNewTrackSpeed, m_fAccTime, fDuration, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 1.f - fWeight, m_fAccTime, fDuration, D3DXTRANSITION_LINEAR);

	// ���� �ִϸ��̼��� ����ǰ� �־��� �ð��� �ʱ�ȭ�ϴ� �Լ�(advanced �Լ� ȣ�� �� ���������� �ð��� �����ϰ� �ִµ� �� �ð� ���� �ʱ�ȭ�ϰڴٴ� �ǹ�)
	m_pAniCtrl->ResetTime();
	m_fAccTime = 0.f;

	// ������ ��� ���̴� Ʈ������ ���ο� Ʈ���� �����ϰ� �Ǿ��� ��, 0�ʺ��� �����ϵ��� �����ϴ� �Լ�
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIdx = iIndex;

	m_iCurrentTrack = m_iNewTrack;
}

void CAniCtrl::Set_TrackPosition(const _double & dPos)
{
	D3DXTRACK_DESC		tTrackInfo;
	ZeroMemory(&tTrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &tTrackInfo);

	tTrackInfo.Position = m_dPeriod * dPos;
	//tTrackInfo.Position = dPos;

	m_fAccTime = float(tTrackInfo.Position);

	m_pAniCtrl->SetTrackDesc(m_iCurrentTrack, &tTrackInfo);
}

void Engine::CAniCtrl::Play_Animation(const _float& fTimeDelta, const _bool& bUpDown /*= TRUE*/)
{
	// �ִϸ��̼��� ����ϴ� �Լ�
	//m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);		// 2���� : �ִϸ��̼� ���ۿ� ���� ���峪 ����Ʈ�� ���� ó���� ����ϴ� ��ü�� �ּ�, �� �༮�� ������� �ʴ� ���� ������ ������ ������ �����δ� �ڵ��� ����ġ�� Ŀ����.

	if (TRUE == bUpDown)
		m_fAccTime += fTimeDelta;
	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);
}

_bool Engine::CAniCtrl::Is_AnimationSetEnd(const _float& fTime /*= 0.1f*/)
{
	D3DXTRACK_DESC		tTrackInfo;
	ZeroMemory(&tTrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &tTrackInfo);

	if (tTrackInfo.Position >= m_dPeriod - fTime)
		return true;

	return false;
}

_bool Engine::CAniCtrl::Is_AnimationSetRateEnd(const _double& dTime /*= 1.f*/)
{
	D3DXTRACK_DESC		tTrackInfo;
	ZeroMemory(&tTrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &tTrackInfo);

	_double dLimitRate = m_dPeriod * (dTime);

	if (tTrackInfo.Position >= dLimitRate)
		return true;

	return false;
}

void Engine::CAniCtrl::Render_AniTrack() const
{
	D3DXTRACK_DESC		tTrackInfo;
	ZeroMemory(&tTrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &tTrackInfo);

	cout << "position : " << tTrackInfo.Position << endl;
	std::cout << "Period : " << m_dPeriod << endl;
}

CAniCtrl* Engine::CAniCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAniCtrl*	pInstance = new CAniCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

CAniCtrl* Engine::CAniCtrl::Create(const CAniCtrl& rhs)
{
	CAniCtrl*	pInstance = new CAniCtrl(rhs);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CAniCtrl::Free(void)
{
	Safe_Release(m_pAniCtrl);
}

