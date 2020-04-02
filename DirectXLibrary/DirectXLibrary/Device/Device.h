#ifndef DEVICE_H
#define DEVICE_H

#include <windows.h>
#include <d3d9.h>

#include "../Window/Window.h"

namespace Library
{

	class Device
	{
	public:
		Device() { Initialize(); }
		virtual ~Device() {}

		static inline void GetDevice(LPDIRECT3DDEVICE9* Device)
		{
			if (pDevice != nullptr)
			{
				*Device = pDevice;
			}
		}

		static inline void SetDevice(const LPDIRECT3DDEVICE9& Device)
		{
			pDevice = Device;
		}

		bool DrawStart();

		void DrawEnd();

		void Clear();

		void Present();

	private:

		LPDIRECT3D9 pDirect3d;
		static LPDIRECT3DDEVICE9 pDevice;
		D3DPRESENT_PARAMETERS d3dpp;

		HRESULT Initialize();


	};

}

#endif
