#ifndef RENDER_H
#define RENDER_H


#include<string>
#include<windows.h>
#include<d3dx9.h>
#include<vector>

#include "../Utility/Position.h"
#include "../Utility/Matrix.h"

#include "../Device/Device.h"

using Utility::Position;
using Utility::Rotation;

namespace Library
{

	struct Thing
	{
		Thing()
		{
			ZeroMemory(this, sizeof(Thing));
			scale = 1;
		}

		~Thing()
		{
			delete pMeshMaterials;
		}

		LPD3DXMESH pMesh;
		D3DMATERIAL9* pMeshMaterials = nullptr;
		LPDIRECT3DTEXTURE9* pMeshTextures;
		DWORD dwNumMaterials;

		//! 移動変数
		D3DXVECTOR3 Position;

		//! 回転変数
		Rotation rotation_matrix{ 0,0,0 };

		//! 大きさの変数
		float scale = 1.0f;

	};

	class Camera
	{
	public:

		//!カメラ変数
		Position pos{ 0.0f,2.0f,-10.0f };

		Position another_pos{ 0.0f,0.0f,0.0f };

		Rotation rotation_matrix{ 0,0,0 };

		float Perspective = 4;

		// 角度
		Position angle_pos{ 0,0,0 };

	};

	class Render
	{
	public:

		bool LoadMesh(Thing& thing, LPCSTR file_name, D3DXVECTOR3* pvecPosition);

		void RenderThing(Thing& thing);

		Camera camera;

	private:

		//D3DXMATRIXA16 PitcMatrix;

		D3DLIGHT9 light;

		LPD3DXFONT pFont;

		LPDIRECT3DVERTEXBUFFER9 pVB = NULL;

		std::vector<Thing> things;

		static const int THING_AMOUNT = 4;

		bool boRenderSphere = true;

		struct CUSTOMVERTEX
		{
			FLOAT x, y, z;
			DWORD color;
		};

		void WorldTransform(Thing& thing);

		void ViewTransform();

		void ProjectionTransform();

		void Rendering(Thing& thing);

		void SetLight();

	};

}


#endif
