#ifndef Component_h__
#define Component_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent : public CBase
{
protected:
	/*  Structor  */
	explicit CComponent();
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent(void);

public:
	/*  General  */
	virtual _int				Update_Component(const _float& fTimeDelta = 0.f) { return UPDATE_OK; }
	virtual _int				LateUpdate_Component(const _float& fTimeDelta = 0.f) { return UPDATE_OK; }



	/*  Creation and destruction  */
public:
	virtual CComponent*			Clone_Component(void) { return nullptr; }
public:
	virtual void				Free(void) PURE;
};

END

#endif // Component_h__
