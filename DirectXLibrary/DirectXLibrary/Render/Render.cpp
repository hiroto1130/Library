#include "Render.h"

namespace Library
{

	void Render::WorldTransform(Thing& thing)
	{
		D3DXMATRIXA16 PositionMatrix;
		D3DXMATRIXA16 WorldMatrix;
		D3DXMATRIXA16 RotationMatrix, RotationMatrix_2;

		D3DXMatrixIdentity(&WorldMatrix);
		// 移動処理
		D3DXMatrixTranslation(&PositionMatrix, thing.Position.x, thing.Position.y, thing.Position.z);
		D3DXMatrixMultiply(&WorldMatrix, &WorldMatrix, &PositionMatrix);
		// 回転処理
		D3DXMatrixIdentity(&RotationMatrix);
		D3DXMatrixRotationX(&RotationMatrix_2, thing.rotation_matrix.pitch);
		D3DXMatrixMultiply(&RotationMatrix, &RotationMatrix, &RotationMatrix_2);
		D3DXMatrixRotationY(&RotationMatrix_2, thing.rotation_matrix.yaw);
		D3DXMatrixMultiply(&RotationMatrix, &RotationMatrix, &RotationMatrix_2);
		D3DXMatrixRotationZ(&RotationMatrix_2, thing.rotation_matrix.roll);
		D3DXMatrixMultiply(&RotationMatrix, &RotationMatrix, &RotationMatrix_2);
		D3DXMatrixMultiply(&WorldMatrix, &WorldMatrix, &RotationMatrix);
		
		// 拡縮
		D3DXMATRIXA16 ScaleMatrix;

		D3DXMatrixScaling(&ScaleMatrix, thing.scale, thing.scale, thing.scale);
		D3DXMatrixMultiply(&WorldMatrix, &WorldMatrix, &ScaleMatrix);

		LPDIRECT3DDEVICE9 pDevice;
		Device::GetDevice(&pDevice);
		pDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);
	}

	void Render::ViewTransform()
	{
		D3DXMATRIXA16 ViewMatrix;

		D3DXVECTOR3 vecEyePt(camera.pos.x, camera.pos.y, camera.pos.z); //カメラ（視点）位置
		D3DXVECTOR3 vecLookatPt(camera.another_pos.x, camera.another_pos.y - 1.0f, camera.another_pos.z + 3.0f);//注視位置
		D3DXVECTOR3 vecUpVec(0.0f, 1.0f, 0.0f);//上方位置

		// 注視点を原点に移動
		D3DXVECTOR3 vec3 = vecEyePt - vecLookatPt;

		D3DXMATRIXA16 YawMatrix;
		D3DXMATRIXA16 PitchMatrix;
		D3DXMATRIXA16 RollMatrix;

		D3DXMatrixRotationX(&PitchMatrix, camera.rotation_matrix.pitch);
		D3DXMatrixRotationY(&YawMatrix, camera.rotation_matrix.yaw);
		D3DXMatrixRotationZ(&RollMatrix, camera.rotation_matrix.roll);

		D3DXVec3TransformCoord(&vec3, &vec3, &PitchMatrix);
		D3DXVec3TransformCoord(&vec3, &vec3, &YawMatrix);
		D3DXVec3TransformCoord(&vec3, &vec3, &RollMatrix);

		vecEyePt = vec3 + vecLookatPt;

		D3DXMatrixIdentity(&ViewMatrix);

		D3DXMATRIXA16 CameraPositionMatrix;

		D3DXMatrixLookAtLH(&CameraPositionMatrix, &vecEyePt, &vecLookatPt, &vecUpVec);

		D3DXMatrixMultiply(&ViewMatrix, &ViewMatrix, &CameraPositionMatrix);

		LPDIRECT3DDEVICE9 pDevice;
		Device::GetDevice(&pDevice);

		pDevice->SetTransform(D3DTS_VIEW, &ViewMatrix);
	}

	void Render::ProjectionTransform()
	{
		D3DXMATRIXA16 ProjectionMatrix;

		D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, D3DX_PI / camera.Perspective, 1.0f, 1.0f, 100.0f);

		LPDIRECT3DDEVICE9 pDevice;
		Device::GetDevice(&pDevice);

		pDevice->SetTransform(D3DTS_PROJECTION, &ProjectionMatrix);
	}

	void Render::Rendering(Thing& thing)
	{

		LPDIRECT3DDEVICE9 pDevice;
		Device::GetDevice(&pDevice);

		for (DWORD i = 0; i < thing.dwNumMaterials; i++)
		{
			pDevice->SetMaterial(&thing.pMeshMaterials[i]);
			pDevice->SetTexture(0, thing.pMeshTextures[i]);
			thing.pMesh->DrawSubset(i);
		}
	}

	bool Render::MeshRoad(Thing& thing, LPCSTR file_name, D3DXVECTOR3* pvecPosition)
	{
		// メッシュの初期位置
		memcpy(&thing.Position, pvecPosition, sizeof(D3DXVECTOR3));

		// Xファイルからメッシュをロードする
		LPD3DXBUFFER pD3DXMtrlBuffer = NULL;
		
		LPDIRECT3DDEVICE9 pDevice;
		Device::GetDevice(&pDevice);

		if (FAILED(D3DXLoadMeshFromXA(file_name, D3DXMESH_SYSTEMMEM,
			pDevice, NULL, &pD3DXMtrlBuffer, NULL,
			&thing.dwNumMaterials, &thing.pMesh)))
		{
			MessageBox(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
			return E_FAIL;
		}

		D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

		thing.pMeshMaterials = new D3DMATERIAL9[thing.dwNumMaterials];
		thing.pMeshTextures = new LPDIRECT3DTEXTURE9[thing.dwNumMaterials];

		for (DWORD i = 0; i < thing.dwNumMaterials; i++)
		{
			thing.pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
			thing.pMeshMaterials[i].Ambient = thing.pMeshMaterials[i].Diffuse;
			thing.pMeshTextures[i] = NULL;
			if (d3dxMaterials[i].pTextureFilename != NULL &&
				lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
			{
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					d3dxMaterials[i].pTextureFilename, &thing.pMeshTextures[i])))
				{
					MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				}
			}
		}

		pD3DXMtrlBuffer->Release();

		return true;
	}

	void Render::RenderThing(Thing& thing)
	{

		LPDIRECT3DDEVICE9 pDevice;
		Device::GetDevice(&pDevice);

		pDevice->BeginScene();
		
		for (DWORD i = 0; i < THING_AMOUNT; i++)
		{
			WorldTransform(thing);
			ViewTransform();
			ProjectionTransform();
			SetLight();
			Rendering(thing);
		}
		
		pDevice->EndScene();

	}

	void Render::SetLight()
	{
		D3DXVECTOR3 vecDirection(1, 1, 1);
		ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse.r = 1.0f;
		light.Diffuse.g = 1.0f;
		light.Diffuse.b = 1.0f;
		light.Specular.r = 1.0f;
		light.Specular.g = 1.0f;
		light.Specular.b = 1.0f;
		D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirection);
		light.Range = 200.0f;

		LPDIRECT3DDEVICE9 pDevice;
		Device::GetDevice(&pDevice);

		pDevice->SetLight(0, &light);
		pDevice->LightEnable(0, TRUE);
	}

}
