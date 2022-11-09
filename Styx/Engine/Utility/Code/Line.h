#ifndef Line_h__
#define Line_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL	CLine : public CBase
{
public:
	enum POINT { POINT_START, POINT_FINISH, POINT_END };
	enum COMPARE { COMPARE_LEFT, COMPARE_RIGHT };

private:
	/*  Structor  */
	explicit CLine(void);
	virtual ~CLine(void);

public:
	HRESULT					Ready_Line(const _vec2* pPointStart, const _vec2* pPointFinish);
	COMPARE					Compare(const _vec2* pEndPos);

private:
	_vec2					m_vPoint[POINT_END];
	_vec2					m_vDirection;
	_vec2					m_vNormal;


	/*  Creation and destruction  */
public:
	static CLine*			Create(const _vec2* pPointA, const _vec2* pPointB);
public:
	virtual void			Free(void);
};
END

#endif // Line_h__
