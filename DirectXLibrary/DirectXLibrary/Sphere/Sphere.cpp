#include "Sphere.h"



namespace Library
{ 
	//HRESULT InitSphere(LPDIRECT3DDEVICE9 pDevice,THING* pThing)
	//スフィアの計算およびスフィアを視認可能にするためにスフィアメッシュを作成する
	HRESULT Sphere::Create(Thing* thing)
	{
		HRESULT hr = NULL;
		LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
		VOID* pVertices = NULL;
		D3DXVECTOR3 center;
		FLOAT radius;

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
		hr = D3DXComputeBoundingSphere(
			(D3DXVECTOR3*)pVertices,
			thing->pMesh->GetNumVertices(),
			D3DXGetFVFVertexSize(thing->pMesh->GetFVF()),
			&center,
			&radius);

		pVB->Unlock();
		pVB->Release();
		pVB = NULL;

		if (FAILED(hr))
		{
			return hr;
		}

		Center = center;
		Radius = radius;

		// 得られた中心と半径を基にメッシュとしてのスフィアを作成する
		LPDIRECT3DDEVICE9 pDevice;
		Device::GetWindowHandle(&pDevice);

		hr = D3DXCreateSphere(pDevice, radius, 24, 24, &pSphereMesh, NULL);
		if (FAILED(hr))
		{
			return hr;
		}

		//スフィアメッシュのマテリアル　白色、半透明、光沢強
		pSphereMeshMaterials = new D3DMATERIAL9;
		pSphereMeshMaterials->Diffuse.r = 1.0f;
		pSphereMeshMaterials->Diffuse.g = 1.0f;
		pSphereMeshMaterials->Diffuse.b = 1.0f;
		pSphereMeshMaterials->Diffuse.a = 0.5f;
		pSphereMeshMaterials->Ambient = pSphereMeshMaterials->Diffuse;
		pSphereMeshMaterials->Specular.r = 1.0f;
		pSphereMeshMaterials->Specular.g = 1.0f;
		pSphereMeshMaterials->Specular.b = 1.0f;
		pSphereMeshMaterials->Emissive.r = 0.1f;
		pSphereMeshMaterials->Emissive.g = 0.1f;
		pSphereMeshMaterials->Emissive.b = 0.1f;
		pSphereMeshMaterials->Power = 120.0f;

		return S_OK;
	}

	void Sphere::Render()
	{

		LPDIRECT3DDEVICE9 pDevice;
		Device::GetWindowHandle(&pDevice);

		if (SUCCEEDED(pDevice->BeginScene()))
		{
			pDevice->SetMaterial(pSphereMeshMaterials);
			pSphereMesh->DrawSubset(0);

			pDevice->EndScene();
		}

		
	}
}