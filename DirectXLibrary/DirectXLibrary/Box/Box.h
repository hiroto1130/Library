#ifndef BOX_H
#define BOX_H

#include<windows.h>
#include<d3dx9.h>

#include "../Device/Device.h"
#include "../Render/Render.h"

namespace Library
{
	struct BBOX
	{
		D3DXVECTOR3 vecMax;
		D3DXVECTOR3 vecMin;
	};

	class Box
	{
	public:

		~Box() { delete pBBoxMeshMaterials; }

		HRESULT Create(Thing* thing);
		void Render();

	private:

		D3DMATERIAL9* pBBoxMeshMaterials = nullptr;
		LPD3DXMESH pBBoxMesh = nullptr;

		BBOX box;
		D3DXVECTOR3 Center;
		float Radius = 0;
	};
}

#endif