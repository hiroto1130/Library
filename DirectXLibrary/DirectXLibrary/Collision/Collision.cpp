#include "Collision.h"

namespace Library
{

	//衝突判定
	bool Collision::Sphere(const Thing& left_thing, const Thing& right_thing)
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

	bool Collision::Box(const Thing& left_thing, const BBOX& left_box, const Thing& right_thing, const BBOX& right_box)
	{
		//物体A用に２つの頂点を用意し初期化する
		D3DXVECTOR3 vecMaxA, vecMinA;
		vecMaxA = left_box.vecMax + left_thing.Position;
		vecMinA = left_box.vecMin + left_thing.Position;

		//物体B用に８つの頂点を用意し初期化する
		D3DXVECTOR3 vecB[9];
		vecB[0].x = right_box.vecMin.x;
		vecB[0].y = right_box.vecMin.y;
		vecB[0].z = right_box.vecMin.z;
		vecB[1].x = right_box.vecMax.x;
		vecB[1].y = right_box.vecMin.y;
		vecB[1].z = right_box.vecMin.z;
		vecB[2].x = right_box.vecMin.x;
		vecB[2].y = right_box.vecMax.y;
		vecB[2].z = right_box.vecMin.z;
		vecB[3].x = right_box.vecMin.x;
		vecB[3].y = right_box.vecMin.y;
		vecB[3].z = right_box.vecMax.z;
		vecB[4].x = right_box.vecMax.x;
		vecB[4].y = right_box.vecMax.y;
		vecB[4].z = right_box.vecMin.z;
		vecB[5].x = right_box.vecMax.x;
		vecB[5].y = right_box.vecMin.y;
		vecB[5].z = right_box.vecMax.z;
		vecB[6].x = right_box.vecMin.x;
		vecB[6].y = right_box.vecMax.y;
		vecB[6].z = right_box.vecMax.z;
		vecB[7].x = right_box.vecMax.x;
		vecB[7].y = right_box.vecMax.y;
		vecB[7].z = right_box.vecMax.z;
		// 物体Bの８つの頂点全てについて、物体Aのボリューム内であるかどうか調べる
		for (DWORD i = 0; i < 8; i++)
		{
			vecB[i] += right_thing.Position;
			if (vecB[i].x >= vecMinA.x && vecB[i].x <= vecMaxA.x)
			{
				if (vecB[i].y >= vecMinA.y && vecB[i].y <= vecMaxA.y)
				{
					if (vecB[i].z >= vecMinA.z && vecB[i].z <= vecMaxA.z)
					{
						return TRUE;
					}
				}
			}
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