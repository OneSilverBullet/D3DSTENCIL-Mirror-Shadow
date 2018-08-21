//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: d3dinit.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Demonstrates how to initialize Direct3D, how to use the book's framework
//       functions, and how to clear the screen to black.  Note that the Direct3D
//       initialization code is in the d3dUtility.h/.cpp files.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include "cube.h"
#include<windows.h>

//
// Globals
//

IDirect3DDevice9* Device = 0;
const int width = 640;
const int height = 480;
IDirect3DVertexBuffer9* VB = 0;

IDirect3DTexture9* wallTex = 0;
IDirect3DTexture9* floorTex = 0;
IDirect3DTexture9* mirroTex = 0;

D3DMATERIAL9 FloorMt = d3d::WHITE_MTRL;
D3DMATERIAL9 WallMt = d3d::WHITE_MTRL;
D3DMATERIAL9 MirroMt = d3d::WHITE_MTRL;

ID3DXMesh* Teapot = 0;
D3DXVECTOR3 TeapotPosition(0.0f, 3.0f, -7.5f);
D3DMATERIAL9 TeapotMt = d3d::YELLOW_MTRL;

void RenderScene();
void RenderMirro();
void RenderShadow();

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		_x = x;
		_y = y;
		_z = z;
		_nx = nx;
		_ny = ny;
		_nz = nz;
		_u = u;
		_v = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

bool Setup()
{
	WallMt.Specular = d3d::WHITE*0.2f;

	D3DXCreateTeapot(Device, &Teapot, 0);

	Device->CreateVertexBuffer(
		24 * sizeof(Vertex),
		0,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	Vertex* v = 0;
	VB->Lock(0,0,(void**)&v, 0);
	// floor
	v[0] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	v[3] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[4] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// wall
	v[6] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[7] = Vertex(-7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[9] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[10] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[11] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Note: We leave gap in middle of walls for mirror

	v[12] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[13] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = Vertex(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[15] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[16] = Vertex(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[17] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// mirror
	v[18] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[21] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[23] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	VB->Unlock();

	//����ͼ
	D3DXCreateTextureFromFile(Device, "checker.jpg", &floorTex);
	D3DXCreateTextureFromFile(Device, "brick0.jpg", &wallTex);
	D3DXCreateTextureFromFile(Device, "ice.bmp", &mirroTex);

	//���ù�����
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//���ù�Դ
	D3DXVECTOR3 lightDir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&lightDir, &color);
	Device->SetLight(0, &light);
	Device->LightEnable(0, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);
	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	//���������
	D3DXVECTOR3 pos(-10.0f, 3.0f, -15.0f);
	D3DXVECTOR3 target(0, 0, 0);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX M;
	D3DXMatrixLookAtLH(&M, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &M);

	D3DXMATRIX K;
	D3DXMatrixPerspectiveFovLH(
		&K,
		D3DX_PI / 4.0f, // 45 - degree
		(float)width / (float)height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &K);

	return true;
}

void CleanUp()
{
	d3d::Release<IDirect3DVertexBuffer9*>(VB);
	d3d::Release<IDirect3DTexture9*>(wallTex);
	d3d::Release<IDirect3DTexture9*>(floorTex);
	d3d::Release<IDirect3DTexture9*>(mirroTex);
	d3d::Release<ID3DXMesh*>(Teapot);
}

bool Display(float timedelta)
{
	if (Device)
	{
		//�����İ뾶
		static float radius = 20.0f;
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000F)
		{
			TeapotPosition.x -= 0.3f*timedelta;
		}
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		{
			TeapotPosition.x += 0.3f*timedelta;
		}
		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		{
			radius -= 2.0f *timedelta;
		}
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		{
			radius += 2.0f * timedelta;
		}
		static float angle = (3.0f * D3DX_PI) / 2.0f;

		if (::GetAsyncKeyState('A')&0x8000f)
		{
			angle -= 0.5f*timedelta;
		}
		if (::GetAsyncKeyState('D')&0x8000f)
		{
			angle += 0.5f*timedelta;
		}
		
		//���������
		D3DXVECTOR3 position(cosf(angle)*radius, 3.0f, sinf(angle)*radius);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		Device->SetTransform(D3DTS_VIEW, &V);

		Device->Clear(0, 0,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
			0xff000000, 1.0f, 0L);
		Device->BeginScene();
		RenderScene();
		RenderMirro();
		RenderShadow();
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

void RenderScene()
{
	//���Ʋ��
	Device->SetMaterial(&TeapotMt);
	Device->SetTexture(0, 0);
	D3DXMATRIX W;
	D3DXMatrixTranslation(&W,
		TeapotPosition.x,
		TeapotPosition.y,
		TeapotPosition.z);
	Device->SetTransform(D3DTS_WORLD, &W);
	Teapot->DrawSubset(0);

	//����ǰ��������ع�
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I); //��õ�λ����
	Device->SetTransform(D3DTS_WORLD, &I);

	//���Ƶ�ǰ��buffer
	Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
	Device->SetFVF(Vertex::FVF);

	//���ݵ�ǰVB�Զ���ƽ����л���
	Device->SetMaterial(&FloorMt);
	Device->SetTexture(0, floorTex);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	Device->SetMaterial(&WallMt);
	Device->SetTexture(0, wallTex);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 6, 4);

	Device->SetMaterial(&MirroMt);
	Device->SetTexture(0, mirroTex);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
}

void RenderMirro()
{
	Device->SetRenderState(D3DRS_STENCILENABLE, true);
	Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); //��������Ϊ���Ǽ��ɹ�
	Device->SetRenderState(D3DRS_STENCILREF, 0x1); //refֵ
	Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff); //����ֵ
	Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff); //ֻд����
	Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP); //�����ȼ��ʧ���ˣ���ô����ģ�建�浱�е�ֵ���и���
	Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP); //���ģ����ʧ���ˣ� ��ô����ģ�建�浱�е�ֵ���и���
	Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);//������ģ�建�涼�ɹ��ˣ���ô��ʹ��refֵ�����滻���������еĶ�Ӧλ��
	
	//����д����Ȼ������̨����
	Device->SetRenderState(D3DRS_ZWRITEENABLE, false);    //�ر���ȼ��
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true); //���ں�
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO); //��Դ�����ں���������Ϊ0
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE); //��Ŀ�������ں���������Ϊ1 
	//��ֹ��̨�������ĸ���

	//���ƾ��ӵ�ģ�建����
	Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
	Device->SetFVF(Vertex::FVF);
	Device->SetMaterial(&MirroMt);
	Device->SetTexture(0, mirroTex);
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	Device->SetTransform(D3DTS_WORLD, &I);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);

	//���´�Z������
	Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//���浱�пɼ��������Ӧ��ģ�����ض������ó���0x1


	//ֻ���Ʒ���Ĳ������������Ƶĵط�
	Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP); //��������ģ����ɹ�����ô����ԭ����
	//�˴�ֻ����ģ������� �����Ǿ���

	//λ�÷���,���ȴ����������
	D3DXMATRIX W, T, R;

	D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f);
	D3DXMatrixReflect(&R, &plane);

	//�õ������λ�þ���
	D3DXMatrixTranslation(&T,
		TeapotPosition.x,
		TeapotPosition.y,
		TeapotPosition.z);

	//�����λ�þ�����Է�����󣬵õ�����Ĳ������
	W = T*R;

	//���z�����������ҿ�ʼ���������뾵�����blend
	Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0); //�Ծ������Ⱦ��ס�˲������Ⱦ��������Z����
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR); //ʹ����������Բ���뾵������ں�
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	//���õ�ǰ���������λ�ã����þ�������Ĳ���ĵط�
	Device->SetTransform(D3DTS_WORLD, &W);
	Device->SetMaterial(&TeapotMt);
	Device->SetTexture(0, 0);

	//����Ⱦǰ��ȷ����Ⱦ�������棬������з�ת
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	Teapot->DrawSubset(0);

	//�ر�֮ǰ�򿪵���Ⱦflag��
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	Device->SetRenderState(D3DRS_STENCILENABLE, false);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void RenderShadow()
{
	//���ڻ����������㣬��ô�������д�룬ģ����Զ�Ϊ��
	Device->SetRenderState(D3DRS_STENCILENABLE, true);
	Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	Device->SetRenderState(D3DRS_STENCILREF, 0x0); //ģ�建�������ж�ӦֵΪ0����ô�ͻ��Ƶ���̨��
	Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	//�״ν����������Ƶ���̨���ܳɹ�
	//��ͼ��һ���Ѿ���д������ؽ���д������ģ�����ʧ��
	Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

	//������Ӱ
	D3DXVECTOR4 lightDirection(0.707f, -0.707f, 0.707f, 0.0f);
	D3DXPLANE groundPlane(0.0f, -1.0f, 0.0f, 0.0f);
	D3DXMATRIX S;
	D3DXMatrixShadow(&S,
		&lightDirection,
		&groundPlane);

	D3DXMATRIX T;
	D3DXMatrixTranslation(&T,
		TeapotPosition.x,
		TeapotPosition.y,
		TeapotPosition.z);

	D3DXMATRIX W = T*S;
	Device->SetTransform(D3DTS_WORLD, &W);

	//����͸����50%�ĺ�ɫ���ʣ�������Ȼ��棬������Ӱ
	//Ȼ������������Ȼ��沢�ҽ���Alpha�ںϺ�ģ�����
	//
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 mtrl = d3d::InitMtrl(d3d::BLACK, d3d:: BLACK, d3d:: BLACK, d3d::BLACK, 0.0F);
	mtrl.Diffuse.a = 0.5f;

	Device->SetRenderState(D3DRS_ZENABLE, false);

	Device->SetMaterial(&mtrl);
	Device->SetTexture(0, 0);

	Teapot->DrawSubset(0);

	Device->SetRenderState(D3DRS_ZENABLE, true);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	Device->SetRenderState(D3DRS_STENCILENABLE, false);

}




LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			::DestroyWindow(hwnd);
		}
		break;
	default:
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		width, height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	CleanUp();

	Device->Release();

	return 0;
}