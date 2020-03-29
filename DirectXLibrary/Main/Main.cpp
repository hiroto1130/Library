#include <windows.h>
#include <d3d9.h>

#include "../DirectXLibrary/Winodw/Window.h"
#include "../DirectXLibrary/Device/Device.h"
#include "../DirectXLibrary/Device/Input/Key.h"
#include "../DirectXLibrary/Render/Render.h"
#include "../DirectXLibrary/Font/Font.h"

using namespace Library;
using namespace Utility;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, int iCmdShow)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	Window* window = new Window("Test");
	WindowParameter::SetRectSize(RectSize{640,480});

	window->Cenerate();

	Render* render = new Render;
	Thing thing;
	Device* device = new Device;

	render->RoadMesh(thing, "Box.x", &D3DXVECTOR3(0, -5, 10));

	Key* key = new Key;

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

			render->RenderThing(thing);
			
			// �O(�J�����̈ړ�����)
			if (key->IsPressed(DIK_UP) || key->IsHeld(DIK_UP))
			{
				render->camera.pos.y -= 0.1f;
				render->camera.another_pos.y -= 0.1f;
			}

			// ���(�J�����̈ړ�����)
			if (key->IsPressed(DIK_DOWN) || key->IsHeld(DIK_DOWN))
			{
				render->camera.pos.y += 0.1f;
				render->camera.another_pos.y += 0.1f;
			}

			// �E(�J�����̈ړ�����)
			if (key->IsPressed(DIK_RIGHT) || key->IsHeld(DIK_RIGHT))
			{
				render->camera.pos.x += 0.1f;
				render->camera.another_pos.x += 0.1f;
			}

			// ��(�J�����̈ړ�����)
			if (key->IsPressed(DIK_LEFT) || key->IsHeld(DIK_LEFT))
			{
				render->camera.pos.x -= 0.1f;
				render->camera.another_pos.x -= 0.1f;
			}

			// ��(���̂̈ړ�����)
			if (key->IsPressed(DIK_A) || key->IsHeld(DIK_A))
			{
				thing.Position.x -= 0.1f;
			}
			// �E(���̂̈ړ�����)
			if (key->IsPressed(DIK_D) || key->IsHeld(DIK_D))
			{
				thing.Position.x += 0.1f;
			}
			// �O(���̂̈ړ�����)
			if (key->IsPressed(DIK_W) || key->IsHeld(DIK_W))
			{
				thing.Position.z += 0.1f;

				render->camera.pos.z += 0.1f;
				render->camera.another_pos.z += 0.1f;
			}
			// ��(���̂̈ړ�����)
			if (key->IsPressed(DIK_S) || key->IsHeld(DIK_S))
			{
				thing.Position.z -= 0.1f;

				render->camera.pos.z -= 0.1f;
				render->camera.another_pos.z -= 0.1f;
			}
			
		}
	}

	delete window;
	delete key;

	return (INT)msg.wParam;

}

