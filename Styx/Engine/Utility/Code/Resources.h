#ifndef Resources_h__
#define Resources_h__

#include "Engine_Include.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResources : public CComponent
{
protected:
	/*  Structor  */
	explicit CResources(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CResources(const CResources& rhs);
	virtual ~CResources(void);

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;


	/*  Creation and destruction  */
public:
	virtual CResources*			Clone(void) PURE;
protected:
	inline virtual void			Free(void);
};

END


#endif // Resources_h__
