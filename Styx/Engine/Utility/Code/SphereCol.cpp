#include "SphereCol.h"

USING(Engine)

Engine::CSphereCol::CSphereCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CSphereCol::~CSphereCol(void)
{

}

HRESULT Engine::CSphereCol::Ready_Buffer(const _float fRadius, 
											const _int iSlices, 
											const _int iStacks)
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = (iSlices + 1) * 2 + ((iStacks - 3) * iSlices);
	m_dwTriCnt = (iSlices * 2) + ((iStacks - 2) * 2) * iSlices;
	m_dwVtxFVF = FVF_COL;

	D3DXCreateSphere(m_pGraphicDev, fRadius, iSlices, iStacks, &m_pMesh, NULL);

	VTXCOL*		pVtxCol = new VTXCOL[m_dwVtxCnt];

	pVtxCol[0].vPos.x = 0.f;

	pVtxCol[0].vPos.x = 0.0f;
	pVtxCol[0].vPos.y = fRadius;
	pVtxCol[0].vPos.z = 0.0f;
	pVtxCol[0].dwColor = 0xffffffff;

	for (int i = 0 + 1, k = 1; i < iStacks; ++i)
	{
		D3DXVECTOR3 vTmp(0.0f, fRadius * cosf((D3DX_PI / iStacks) * i), fRadius * sinf((D3DX_PI / iStacks) * i));
		for (int j = 0; j < iSlices; ++j, ++k)
		{
			float fTemp = fRadius - (fRadius - vTmp.z);
			pVtxCol[k].vPos.x = fTemp * cosf(D3DXToRadian((360 / iSlices) * j));
			pVtxCol[k].vPos.y = vTmp.y;
			pVtxCol[k].vPos.z = fTemp * sinf(D3DXToRadian((360 / iSlices) * j));
			pVtxCol[k].dwColor = 0xffffffff;
		}
	}

	pVtxCol[m_dwVtxCnt - 1].vPos.x = 0.f;
	pVtxCol[m_dwVtxCnt - 1].vPos.y = -fRadius;
	pVtxCol[m_dwVtxCnt - 1].vPos.z = 0.f;
	pVtxCol[m_dwVtxCnt - 1].dwColor = 0xffffffff;

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * sizeof(VTXCOL),
												0, FVF_COL,
												D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return E_FAIL;
	}

	VOID* pVertices;

	if (FAILED(m_pVB->Lock(0, sizeof(pVtxCol), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, pVtxCol, sizeof(VTXCOL) * m_dwVtxCnt);
	m_pVB->Unlock();

	Safe_Delete_Array(pVtxCol);

	int j = 0;
	int z = 0;

	INDEX *index = new INDEX[m_dwTriCnt];

	for (z = 0; z < iSlices; ++z)
	{
		index[z]._0 = 0;
		index[z]._1 = z + 1;
		index[z]._2 = z + 2 - j;
		if (z == iSlices - 2)
		{
			j = iSlices;
		}
	}

	int t = z;
	int tmp = 0;
	int tmp2 = 0;
	int a = 0;


	for (int i = 0; i < iStacks - 2; ++i)
	{
		tmp = (iSlices + 1) + (i * iSlices);
		tmp2 = 2 + (i * iSlices);
		for (int j = 0; j < iSlices; ++j)
		{
			if (j == (iSlices - 1))
			{
				a = iSlices;
			}
			index[t]._0 = tmp2 + j - a;
			index[t]._1 = tmp2 + j - 1;
			index[t]._2 = tmp2 + j + iSlices - a;

			index[t + 1]._0 = tmp + j + 1 - a;
			index[t + 1]._1 = tmp + j - (iSlices);
			index[t + 1]._2 = tmp + j;
			t += 2;
		}
		a = 0;
	}

	int b;

	b = t;

	int q = 0;
	for (int i = 0; i < iSlices; ++i)
	{
		index[b]._0 = tmp + i;
		index[b]._1 = tmp + iSlices;
		index[b]._2 = tmp + i + 1 - q;
		if (i == iSlices - 2)
		{
			q = iSlices - i;
		}
		b++;
	}

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwTriCnt * sizeof(INDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		return E_FAIL;
	}

	VOID* pIndices;

	if (FAILED(m_pIB->Lock(0, sizeof(INDEX) * m_dwTriCnt, (void**)&pIndices, 0)))
		return E_FAIL;

	memcpy(pIndices, index, sizeof(INDEX) * m_dwTriCnt);

	m_pIB->Unlock();

	Safe_Delete_Array(index);

	return S_OK;
}

CSphereCol* Engine::CSphereCol::Create(LPDIRECT3DDEVICE9 pGraphicDev,
												const _float fRadius,
												const _int iSlices,
												const _int iStacks)
{
	CSphereCol*		pInstance = new CSphereCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(fRadius, iSlices, iStacks)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CSphereCol::Free(void)
{
	Engine::Safe_Release(m_pMesh);

	CVIBuffer::Free();
}

