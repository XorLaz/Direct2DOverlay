#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include <cstdint>
#include <vector>

class Direct2DRedner {

public:

	static Direct2DRedner Draw;

	void DrawBegin();
	void DrawEnd();
	void Cleanup();
	bool Initialize();

	void DrawLine(float x1, float y1, float x2, float y2, float r, float g, float b, float a, float thickness);
	void DrawRect(float x, float y, float w, float h, float r, float g, float b, float a, float thickness);
	void FillRectA(float x, float y, float w, float h, float r, float g, float b, float a);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a, float thickness);
	void FillCircle(float x, float y, float radius, float r, float g, float b, float a);
	// 原汁原味的绘制方法
	void DrawTextA(const wchar_t* text, float x, float y, float r, float g, float b, float a);
	// 带描边文本的绘制方法
	void DrawTextF(float x, float y, float r, float g, float b, float a, const wchar_t* format, ...);
	void DrawTextFBottom(float rect_x, float rect_y, float rect_width, float r, float g, float b, float a, const wchar_t* format, ...);
	// 绘制血条（横向）
	void DrawHealth(float x, float y, float stringLength, float Distance, float hp, float maxhp);
	// 绘制信号血条（竖向）
	void DrawSignalHP(float x, float y, float h, float hp, float maxhp);


	uint32_t GetWidth() { return g_ScreenWidth; }
	uint32_t GetHeight() { return g_ScreenHeight; }
	HWND GetHWND() { return g_hWnd; }

	void UpdateFPS();  // 更新FPS
	float GetFPS();    // 获取当前FPS

	void SetWidthHeight(uint32_t Width,uint32_t Height) {
		g_ScreenWidth = Width;
		g_ScreenHeight = Height;
	}

private:

	HWND g_hWnd = nullptr;
	ID2D1Factory* g_pD2DFactory = nullptr;
	ID2D1HwndRenderTarget* g_pRenderTarget = nullptr;
	ID2D1SolidColorBrush* g_pBrush = nullptr;
	IDWriteFactory* g_pDWriteFactory = nullptr;
	IDWriteTextFormat* g_pTextFormat = nullptr;
	uint32_t g_ScreenWidth = 0;
	uint32_t g_ScreenHeight = 0;

	// FPS 计算相关
	LARGE_INTEGER frequency;
	LARGE_INTEGER lastTime;
	int frameCount;
	float fps;

};