#ifndef ItemList_h__
#define ItemList_h__

#include "Engine_Include.h"
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CShader;

END

class CItemList : public Engine::CGameObject
{
private:
	/*  Structor  */
	explicit CItemList(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CItemList(void);

public:
	/*  General  */
	virtual	HRESULT				Ready_Object(void) override;
	virtual	_int				Update_Object(const _float& fTimeDelta) override;
	virtual	_int				LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void				PreRender_Object(void) override;
	virtual	void				Render_Object(void) override;
	virtual void				PostRender_Object(void) override;

public:
	/*  Choice  */
	virtual HRESULT				SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	/*  Set  */
	void						Set_AddItem(const _ushort& _uIndex);
	void						Set_MiusItem(const _ushort& _uIndex);
	void						Set_MiusCurItem(void) { m_vecItemNum[m_wItemSelect]--; }
	
public:
	/*  Get  */
	const _short&				Get_ItemSelect(void) const { return m_wItemSelect; }
	const _ushort&				Get_ItemNum(const _short& _w) const { return m_vecItemNum[_w]; }
	const _ushort&				Get_CurItemNum(void) const { return m_vecItemNum[m_wItemSelect]; }

private:
	HRESULT						Add_Component(void);
	void						Select_Item(void);
	void						ColFalse_Use_Item(void);
	void						ColTrue_Use_Item(void);

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	vector<_ushort>				m_vecItemNum;
	_short						m_wItemSelect = 0;
	_bool						m_bTimeActive = FALSE;
	_double						m_dTime = 0;



	/*  Creation and destruction  */
public:
	static CItemList*			Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual void				Free(void);
};


#endif // ItemList_h__
