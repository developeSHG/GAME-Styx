#ifndef Layer_h__
#define Layer_h__

#include "Engine_Include.h"
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	/*  Structor  */
	explicit CLayer(void);
	virtual ~CLayer(void);

public:
	/*  General  */
	virtual HRESULT		Ready_Layer(void);
	virtual _int		Update_Layer(const _float& fTimeDelta);
	virtual _int		LateUpdate_Layer(const _float& fTimeDelta);

public:
	void				Tool_DeleteAll();
	_bool				Tool_DeleteObject(const _tchar* pObjTag, CGameObject* pGameObject);

public:
	HRESULT				Add_Object(const _tchar* pObjTag, CGameObject* pGameObject);


private:
	multimap<wstring, CGameObject*>		m_mapObject;


	/*  Creation and destruction  */
public:
	static	CLayer*	Create(void);
public:
	virtual	void	Free(void);
};

END
#endif // Layer_h__
