#ifndef ComponentMgr_h__
#define ComponentMgr_h__

#include "Engine_Include.h"
#include "Base.h"

#include "Component.h"

/*  Component  */
#include "Transform.h"
#include "Calculator.h"
#include "Collider.h"
#include "Optimization.h"
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CComponentMgr : public CBase
{
	DECLARE_SINGLETON(CComponentMgr)

private:
	typedef map<const _tchar*, CComponent*>			MAPCOMPONENT;

private:
	/*  Structor  */
	explicit CComponentMgr();
	~CComponentMgr(void);

public:
	HRESULT									Add_Component(const _ushort& _wContainerIdx,
															const _tchar* _pComponentTag,
															COMPONENTTYPE _eType);
	CComponent*								Find_Component(const _ushort& _wContainerIdx,
															const _tchar* _pComponentTag);

public:
	/*  Set  */
	void									Set_ComponentMgrGraphicDev(LPDIRECT3DDEVICE9 _pDevice) { m_pGraphicDev = _pDevice; }
	HRESULT									Set_ObjectComponent(const _tchar* _pObjTag,
																const _tchar* _pComponentTag,
																CComponent* _pComponent);

public:
	/*  Get  */
	CComponent*								Get_ObjectComponent(const _tchar* _pObjTag,
																const _tchar* _pComponentTag);
	
private:
	LPDIRECT3DDEVICE9						m_pGraphicDev;

private:
	MAPCOMPONENT							m_mapComponent[COMPONENTID::ID_END];
	map<const _tchar*, MAPCOMPONENT>		m_mapObjectmapComponent;


	/*  Creation and destruction  */
public:
	CComponent*								Clone_Component(const _ushort& _wContainerIdx,
															const _tchar* _pComponentTag);
public:
	virtual void							Free(void);
};

END

#endif // ComponentMgr_h__
