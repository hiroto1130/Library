#include"Box.h"

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace Library
{
	//HRESULT InitSphere(LPDIRECT3DDEVICE9 pDevice,THING* pThing)
	//スフィアの計算およびスフィアを視認可能にするためにスフィアメッシュを作成する
	HRESULT Box::Create(Thing* thing)
	{
		HRESULT hr = NULL;
		LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
		VOID* pVertices = NULL;
		D3DXVECTOR3 vecMax, vecMin;

		//メッシュの頂点バッファーをロックする
		if (FAILED(thing->pMesh->GetVertexBuffer(&pVB)))
		{
			return E_FAIL;
		}
		if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
		{
			pVB->Release();
			pVB = NULL;
			return E_FAIL;
		}
		// メッシュの外接円の中心と半径を計算する
		hr = D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices, thing->pMesh->GetNumVertices(),
			D3DXGetFVFVertexSize(thing->pMesh->GetFVF()), &vecMin, &vecMax);
		pVB->Unlock();
		pVB->Release();
		pVB = NULL;

		if (FAILED(hr))
		{
			return hr;
		}

		box.vecMax = vecMax;
		box.vecMin = vecMin;

		// 得られた中心と半径を基にメッシュとしてのボックスを作成する
		LPDIRECT3DDEVICE9 pDevice;
		Device::GetDevice(&pDevice);


		hr = D3DXCreateBox(pDevice, vecMax.x * 2, vecMax.y * 2, vecMax.z * 2, &pBBoxMesh, NULL);
		if (FAILED(hr))
		{
			return hr;
		}
		//ボックスメッシュのマテリアル　白色、半透明、光沢強
		pBBoxMeshMaterials = new D3DMATERIAL9;
		pBBoxMeshMaterials->Diffuse.r = 1.0f;
		pBBoxMeshMaterials->Diffuse.g = 1.0f;
		pBBoxMeshMaterials->Diffuse.b = 1.0f;
		pBBoxMeshMaterials->Diffuse.a = 0.5f;
		pBBoxMeshMaterials->Ambient = pBBoxMeshMaterials->Diffuse;
		pBBoxMeshMaterials->Specular.r = 1.0f;
		pBBoxMeshMaterials->Specular.g = 1.0f;
		pBBoxMeshMaterials->Specular.b = 1.0f;
		pBBoxMeshMaterials->Emissive.r = 0.1f;
		pBBoxMeshMaterials->Emissive.g = 0.1f;
		pBBoxMeshMaterials->Emissive.b = 0.1f;
		pBBoxMeshMaterials->Power = 120.0f;

		return S_OK;
	}

	void Box::Render()
	{

		LPDIRECT3DDEVICE9 pDevice;
		Device::GetDevice(&pDevice);

		pDevice->SetMaterial(pBBoxMeshMaterials);
		pBBoxMesh->DrawSubset(0);

	}
}