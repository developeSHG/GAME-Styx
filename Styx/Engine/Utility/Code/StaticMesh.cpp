#include "StaticMesh.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubsetBuffer(rhs.m_pSubsetBuffer)
	, m_pSubsets(rhs.m_pSubsets)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_pVtxPos(rhs.m_pVtxPos)
	, m_dwVtxNum(rhs.m_dwVtxNum)
	, m_dwStride(rhs.m_dwStride)

{
	/*  �޽��� �ؽ��Ĵ� �������簡 �̷������Ѵ�. (������ ��������� �ƴ�)  */
	m_ppTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];

	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTexture[i] = rhs.m_ppTexture[i];
		m_ppTexture[i]->AddRef();
	}
	m_pOriMesh->AddRef();
	m_pMesh->AddRef();
	m_pAdjacency->AddRef();
	m_pSubsetBuffer->AddRef();
}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

HRESULT Engine::CStaticMesh::Ready_Meshe(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar szFullPath[128] = L"";

	/*  �ش�����(/) ������ ����  */
	lstrcpy(szFullPath, pFilePath);
	/*  ���� �� �����̸� �̾���̱�  */
	lstrcat(szFullPath, pFileName);

	/*  Mesh Load  */
	FAILED_CHECK_RETURN(D3DXLoadMeshFromX(szFullPath,
										D3DXMESH_MANAGED,
										m_pGraphicDev,
										&m_pAdjacency,
										&m_pSubsetBuffer,
										NULL,
										&m_dwSubsetCnt,
										&m_pOriMesh),
										E_FAIL);

	/*  ���� �޽��� ���� ������ �Ӽ� ���� ������ �Լ�  */
	_ulong		dwFVF = m_pOriMesh->GetFVF();
	
	/*  Load�� OriMesh�� ��������� ������ �ִ°� �ƴϱ� ������ �ִ��� üũ�Ŀ�  */
	/*  m_pMesh�� ���� �����ؼ� �Ѱ��ִ� ������ �� �� �̴�  */
	/*  FVF �Ӽ� �� NORMAL ������ ���� ��� ( ��������� ���ٴ� ��)  */
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		/*  ������ Load�� �޽��� FVF ������ �����ϴ� �Լ�  */
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);

		/*  �޽� ������ NORMAL ���� ����Ͽ� �־��ִ� �Լ�  */
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}

	void*		pVertex = nullptr;

	/*  �޽��� �� ������ ������ �Լ�  */
	m_dwVtxNum = m_pMesh->GetNumVertices();

	m_pVtxPos = new _vec3[m_dwVtxNum];

	/*  �������� POS�� �˱����� �޽��� ������ ���̴�  */
	m_pMesh->LockVertexBuffer(0, &pVertex);

	/*  MAX_FVF_DECL_SIZE = ���� �ϳ��� ���� �� �ִ� �ִ��� FVF ����  */
	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	/*  �Ž� ������ ������ ���´�  */
	m_pMesh->GetDeclaration(Decl);

	/*  OffSet�� FVF������ �߿��� ����� �ּҿ� �ִ��� üũ�� �뵵�� �� ���̴�  */
	_ulong byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		/*  Stream�� ���� ���������θ� ��� 0,1 �ǿ��� ���ڸ� ��  */
		/*  Usage�� FVF �Ӽ��� �ǹ��� (���̴����� ���)  */
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			/*  Decl[i].Offset�� �޸� ����  */
			/*  ����ȯ �Ͽ��� ����  */
			byOffset = (_ulong)Decl[i].Offset;
			break;
		}
	}

	/*  ������ ������ �˰� ���� �� ������ ���� ũ�⸦ ���´�. (�׳� �������� �����̶�� ��������)  */
	/*  = ������ FVF�� �ٰŷ� ������ ũ�⸦ �����´�.  */
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);


	for (_ulong i = 0; i < m_dwVtxNum; ++i)
	{
		/*  pVertex : �� �� �ּ�   */
		/*  byOffset : FVF Pos�� ���۵Ǵ� �ּ�  */
		/*  m_dwStride : �������� ����  */
		//m_pVtxPos[i] = *(_vec3*)(((_ubyte*)pVertex) + (byOffset + (m_dwStride * i)));
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (m_dwStride * i + byOffset)));
	}

	/*  �İ�ü�� ��� �ִ� ���� ���� �� ���� �� �ּҸ� �����ϴ� �Լ�  */
	m_pSubsets = (D3DXMATERIAL*)m_pSubsetBuffer->GetBufferPointer();

	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	/*  ������� ������ �ؽ����� ����, ������ ������ ���⿡  */
	/*  �ؽ��ĸ� �ε����ش�.  */
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar szFileName[128] = L"";

		/*  �ٽ� �ش�����(/) ������ ����  */
		lstrcpy(szFullPath, pFilePath);

		/*  ASKI �ڵ带 UNI �ڵ�� ��ȯ ���ִ� �Լ�  */
		MultiByteToWideChar(CP_ACP,
							0,
							m_pSubsets[i].pTextureFilename,
							strlen(m_pSubsets[i].pTextureFilename),
							szFileName,
							128);

		lstrcat(szFullPath, szFileName);

		FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTexture[i]), E_FAIL);
	}
	
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

void Engine::CStaticMesh::Render_Meshe(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetMaterial(&m_pSubsets[i].MatD3D);
		m_pGraphicDev->SetTexture(0, m_ppTexture[i]);
		m_pMesh->DrawSubset(i);
	}
}

void Engine::CStaticMesh::Render_Meshe(LPD3DXEFFECT pEffect, _uint iPassCnt /*= 0*/)
{
	pEffect->AddRef();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_bool	bAlpha = false;
		//_uint iPassCnt = 0;

		//if (bAlpha = Find_Alpha(m_pSubsets[i].pTextureFilename))
		//	iPassCnt = 1;

		pEffect->SetTexture("g_BaseTexture", m_ppTexture[i]);   
		pEffect->SetTexture("g_DiffuseTexture", m_ppTexture[i]);
		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          pEffect->SetTexture("g_NormalTexture", m_ppTexture[i]);
		//if(1 <= i)
			//pEffect->SetTexture("g_NormalTexture", m_ppTexture[i]);
		//if (2 <= i)
		//	pEffect->SetTexture("g_NormalTexture", m_ppTexture[MESHTEXTURE::TYPE_SPECULAR]);
		//if (3 <= i)
		//	pEffect->SetTexture("g_NormalTexture", m_ppTexture[MESHTEXTURE::TYPE_EMISSIVE]);

		pEffect->CommitChanges();	// ����� �ؽ�ó ������ �������ִ� �Լ�

		pEffect->BeginPass(iPassCnt);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
	}

	Safe_Release(pEffect);
}

CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStaticMesh*		pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshe(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CResources* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{
	CMesh::Free();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		Safe_Release(m_ppTexture[i]);
	}

	Safe_Release(m_pOriMesh);
	Safe_Release(m_pMesh);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubsetBuffer);

	Safe_Delete_Array(m_ppTexture);

	if (false == m_bClone)
		Safe_Delete_Array(m_pVtxPos);
}

LPDIRECT3DBASETEXTURE9 Engine::CStaticMesh::Get_Texture(_ulong dwMaterialIndex, MESHTEXTURE::TYPE eType)
{
	if (nullptr == m_ppTexture ||
		m_dwSubsetCnt <= dwMaterialIndex)
		return nullptr;

	return m_ppTexture[eType];
}

