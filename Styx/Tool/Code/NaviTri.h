#ifndef NaviTri_h__
#define NaviTri_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CSphereCollider;

END

class CNaviPoint;

class CNaviTri : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CNaviTri(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CNaviTri(void);

public:
	/*  General  */
	virtual	HRESULT					Ready_Object(void) override;
	virtual	_int					Update_Object(const _float& fTimeDelta) override;
	virtual	_int					LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void					PreRender_Object(void) override;
	virtual	void					Render_Object(void) override;
	virtual void					PostRender_Object(void) override;

public:
	/*  Set  */

public:
	/*  Get  */
						
private:
	HRESULT							Add_Component(void); 
	HRESULT							SetUp_Material(void);

public:
	void							CellLineRender(void);
	void							CellIndexRender(void);

public:
	HRESULT							PointCreate(const _vec3 _vPos, Engine::NAVI_TYPE _eType);
	HRESULT							BackDelete();

public:
	Engine::CTransform*				m_pTransCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;

public:
	LPD3DXLINE						m_pD3DXLine = nullptr;
	vector<vector<CNaviPoint*>>		m_vecvecNaviTri;
	_ushort							m_uPointIndex;
	_bool							m_bPick = FALSE;



	/*  Creation and destruction  */
public:
	static CNaviTri*				Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void					Free(void);
};


#endif // NaviTri_h__
