#include "Line.h"

USING(Engine)

Engine::CLine::CLine(void)
{

}

Engine::CLine::~CLine(void)
{

}

HRESULT CLine::Ready_Line(const _vec2* pPointStart, const _vec2* pPointFinish)
{
	m_vPoint[POINT_START] = *pPointStart;
	m_vPoint[POINT_FINISH] = *pPointFinish;

	m_vDirection = m_vPoint[POINT_FINISH] - m_vPoint[POINT_START];
	m_vNormal = _vec2(m_vDirection.y * -1.f, m_vDirection.x);
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	return S_OK;
}

CLine::COMPARE CLine::Compare(const _vec2 * pEndPos)
{
	_vec2		vDest = *pEndPos - m_vPoint[POINT_START];

	_float		fResult = D3DXVec2Dot(D3DXVec2Normalize(&vDest, &vDest), &m_vNormal);

	if (0.f <= fResult)
		return COMPARE_LEFT;
	else
		return COMPARE_RIGHT;
}

CLine* Engine::CLine::Create(const _vec2* pPointA, const _vec2* pPointB)
{
	CLine*	pInstance = new CLine;

	if (FAILED(pInstance->Ready_Line(pPointA, pPointB)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLine::Free(void)
{

}

