#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void);

public:
	HRESULT									Ready_Meshe(const _tchar* pFilePath, const _tchar* pFileName);
	void									Render_Meshe(void);
	void									Render_Meshe(LPD3DXEFFECT pEffect);

private:
	// ��� ���� ��ȸ�ϸ鼭 ���� ������ �ִ� ��� ������ �����ϴ� �Լ�
	void									Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void									SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

public:
	/*  Active  */
	void									Play_Animation(const _float& fTimeDelta, const _bool& bUpDown = TRUE);
	_bool									Is_AnimationSetEnd(const _float& fTime = 0.1f);
	_bool									Is_AnimationSetRateEnd(const _double& dTime = 1.f);
	void									Render_AniTrack() const;

public:
	/*  Set  */
	void									Set_AnimationSet(const _uint& iIndex, 
																const _float& fCurTrackSpeed = 1.f,
																const _float& fNewTrackSpeed = 1.f,
																const _float& fWeight = 0.1f,
																const _float& fDuration = 0.25f);
	void									Set_TrackPosition(const _double & dPos);

public:
	/*  Get  */
	const D3DXFRAME_DERIVED*				Get_FrameByName(const char* pFrameName);
	CAniCtrl*								Get_AniCtrl() const { return m_pAniCtrl; }
	const _uint								Get_AnimationIndex() const { return m_pAniCtrl->Get_AnimationIndex(); }

private:
	D3DXFRAME*								m_pRootFrame;
	CHierarchyLoader*						m_pLoader;
	CAniCtrl*								m_pAniCtrl;
	list<D3DXMESHCONTAINER_DERIVED*>		m_MeshContainerList;


public:
	static CDynamicMesh*					Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
public:
	virtual CResources*						Clone(void);
private:
	virtual void							Free(void);
};

END
#endif // DynamicMesh_h__


//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;					// ���� �̸�(x���Ͽ��� ���� �ε� �� ����Ǿ��ִ� ������ �̸�)
//	D3DXMATRIX              TransformationMatrix;	// ���� �ε� ���¿��� ���밡 ���ϰ� �ִ� ���� ���	
//
//	LPD3DXMESHCONTAINER     pMeshContainer;			// �޽� �����̳ʴ� �޽��� ���� ������ �����ϴ� ����ü, 
//													// ��κ� null�� ä���� �ְ�, �޽��� ���� ���밡 �Ǵ� �༮�� �������� ���ϴ� ��
//	struct _D3DXFRAME       *pFrameSibling;			// ���� ���� �ּҸ� �����ϱ� ���� ������
//	struct _D3DXFRAME       *pFrameFirstChild;		// �ڽ� ���� �ּҸ� �����ϱ� ���� ������
//} D3DXFRAME, *LPD3DXFRAME;
//
//
//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;			// �� �̸��� ��쿡�� Ȯ���� ����ϴµ� ���� �޽� �����̳� �̸��� ��κ� null�� ä���ش�.
//
//	D3DXMESHDATA            MeshData;		// �޽� �İ�ü�� ��� �ִ� ����ü(�츮�� ù ��° ��� ������ ����� ����)
//
//	LPD3DXMATERIAL          pMaterials;		// �޽��� ���� ������ ��� ���� ����ü
//	LPD3DXEFFECTINSTANCE    pEffects;		// ������� �ʴ´�/
//	DWORD                   NumMaterials;	// ������ ����( == �ؽ�ó�� ���� == ������� ����)
//	DWORD                  *pAdjacency;		// ������ ������ ������ �����ϱ� ���� ������
//
//	LPD3DXSKININFO          pSkinInfo;		// ���� �ƴ� �޽��� ���õ� ���� ������ �����ϴ� �� ��ü(ex : �޽��� �׷��� �� � ���鿡 ������ �޴��� �˷��ִ� �Լ�, �� ���� ���鿡 ������ �ް� �ִ��� �˷��ִ� �Լ�)
//
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;	// ���� �޽������̳��� �ּҸ� �����ϴ� ������
//
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;

