#ifndef QuadTree_h__
#define QuadTree_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CQuadTree : public CComponent
{
private:
	/*  Structor  */
	explicit CQuadTree(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuadTree(void);


private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;


	/*  Creation and destruction  */
public:
	static CQuadTree*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void				Free(void);
};

END
#endif // QuadTree_h__