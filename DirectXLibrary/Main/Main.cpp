#include <windows.h>
#include <d3d9.h>

#include <crtdbg.h>

#include "../DirectXLibrary/Window/Window.h"
#include "../DirectXLibrary/Device/Device.h"
#include "../DirectXLibrary/Device/Input/Key.h"
#include "../DirectXLibrary/Render/Render.h"
#include "../DirectXLibrary/Font/Font.h"
#include "../DirectXLibrary/Sphere/Sphere.h"
#include "../DirectXLibrary/Collision/Collision.h"


using namespace Library;
using namespace Utility;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, int iCmdShow)
{

	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	Window* window = new Window("Test");
	WindowParameter::SetRectSize(RectSize{ 640,480 });

	window->Create();

	Render* render = new Render;
	Thing move_thing;
	Thing stop_thing;

	Device* device = new Device;

	render->RoadMesh(move_thing, "Box.x", &D3DXVECTOR3(0, -5, 10));
	render->RoadMesh(stop_thing, "Box.x", &D3DXVECTOR3(0, -5, 50));

	Key* key = new Key;
	Sphere sphere;
	Font font(60);
	Collision collision;

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

			key->UpdateKeyStatus();
			device->Clear();

			render->RenderThing(stop_thing);
			render->RenderThing(move_thing);


			sphere.Create(&move_thing);
			sphere.Render();

			if (true == collision.Impact(move_thing, stop_thing))
			{
				font.Rendering("衝突", 10, 10);
			}
			else
			{
				font.Rendering("未衝突", 10, 10);
			}

			device->Present();

			// 前(カメラの移動方向)
			if (key->IsPressed(DIK_UP) || key->IsHeld(DIK_UP))
			{
				render->camera.pos.y -= 0.1f;
				render->camera.another_pos.y -= 0.1f;
			}

			// 後ろ(カメラの移動方向)
			if (key->IsPressed(DIK_DOWN) || key->IsHeld(DIK_DOWN))
			{
				render->camera.pos.y += 0.1f;
				render->camera.another_pos.y += 0.1f;
			}

			// 右(カメラの移動方向)
			if (key->IsPressed(DIK_RIGHT) || key->IsHeld(DIK_RIGHT))
			{
				render->camera.pos.x += 0.1f;
				render->camera.another_pos.x += 0.1f;
			}

			// 左(カメラの移動方向)
			if (key->IsPressed(DIK_LEFT) || key->IsHeld(DIK_LEFT))
			{
				render->camera.pos.x -= 0.1f;
				render->camera.another_pos.x -= 0.1f;
			}

			// 左(物体の移動方向)
			if (key->IsPressed(DIK_A) || key->IsHeld(DIK_A))
			{
				move_thing.Position.x -= 0.1f;
			}
			// 右(物体の移動方向)
			if (key->IsPressed(DIK_D) || key->IsHeld(DIK_D))
			{
				move_thing.Position.x += 0.1f;
			}
			// 前(物体の移動方向)
			if (key->IsPressed(DIK_W) || key->IsHeld(DIK_W))
			{
				move_thing.Position.z += 0.1f;

				render->camera.pos.z += 0.1f;
				render->camera.another_pos.z += 0.1f;
			}
			// 後(物体の移動方向)
			if (key->IsPressed(DIK_S) || key->IsHeld(DIK_S))
			{
				move_thing.Position.z -= 0.1f;

				render->camera.pos.z -= 0.1f;
				render->camera.another_pos.z -= 0.1f;
			}

		}
	}


	delete window;
	delete render;
	delete device;
	delete key;

	return (INT)msg.wParam;

}

