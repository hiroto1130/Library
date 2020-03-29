#include "Collision.h"

//衝突判定
bool Collision::Impact(Thing* thingA, Thing* thingB)
{
	//２つの物体の中心間の距離を求める
	D3DXVECTOR3 vecLength = thingB->Position - thingA->Position;
	FLOAT fLength = D3DXVec3Length(&vecLength);
	// その距離が、2物体の半径を足したものより小さいということは、
	//境界球同士が重なっている（衝突している）ということ

	//if (fLength < thingA->Radius + thingB->Radius)
	//{
	//	return TRUE;
	//}

	return FALSE;
}