#ifndef SPHERE_H
#define SPHERE_H

#include<windows.h>
#include<d3dx9.h>

#include "../Device/Device.h"
#include "../Render/Render.h"

namespace Library
{
	class Sphere
	{
	public:

		~Sphere() { delete pSphereMeshMaterials; }

		HRESULT Create(Thing* thing);
		void Render();

	private:

		D3DMATERIAL9* pSphereMeshMaterials = nullptr;
		LPD3DXMESH pSphereMesh = nullptr;

		D3DXVECTOR3 Center;
		float Radius;
	};
}

#endif