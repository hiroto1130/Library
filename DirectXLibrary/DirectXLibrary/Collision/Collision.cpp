#include "Collision.h"

namespace Library
{

	//衝突判定
	bool Collision::Impact(const Thing& left_thing, const Thing& right_thing)
	{
		//２つの物体の中心間の距離を求める
		D3DXVECTOR3 vecLength = right_thing.Position - left_thing.Position;
		FLOAT fLength = D3DXVec3Length(&vecLength);

		// その距離が、2物体の半径を足したものより小さいということは、
		//境界球同士が重なっている（衝突している）ということ

		FLOAT left_thing_radius = 0.0f;
		FLOAT right_thing_radius = 0.0f;

		left_thing_radius = GetRadius(left_thing);
		right_thing_radius = GetRadius(right_thing);

		if (fLength < left_thing_radius + right_thing_radius)
		{
			return TRUE;
		}

		return FALSE;
	}

	FLOAT Collision::GetRadius(const Thing& thing)
	{
		VOID* pVertices = NULL;
		D3DXVECTOR3 center;
		FLOAT radius;
		HRESULT hr = NULL;
		LPDIRECT3DVERTEXBUFFER9 pVB = NULL;

		//メッシュの頂点バッファーをロックする
		if (FAILED(thing.pMesh->GetVertexBuffer(&pVB)))
		{
			return E_FAIL;
		}
		if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
		{
			pVB->Release();
			pVB = NULL;
			return E_FAIL;
		}

		hr = D3DXComputeBoundingSphere((D3DXVECTOR3*)pVertices,thing.pMesh->GetNumVertices(),
			D3DXGetFVFVertexSize(thing.pMesh->GetFVF()),&center,&radius);

		pVB->Unlock();
		pVB->Release();

		return radius;
	}
}