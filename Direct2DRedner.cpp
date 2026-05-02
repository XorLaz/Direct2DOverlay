#include "Direct2DRedner.h"

#include <dwmapi.h>
#include <stdio.h>
#include <ShellScalingApi.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "Shcore.lib")

Direct2DRedner Direct2DRedner::Draw;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


void Direct2DRedner::DrawLine(float x1, float y1, float x2, float y2, float r, float g, float b, float a, float thickness) {
	g_pBrush->SetColor(D2D1::ColorF(r, g, b, a));
	g_pRenderTarget->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), g_pBrush, thickness);
}
void Direct2DRedner::DrawRect(float x, float y, float w, float h, float r, float g, float b, float a, float thickness) {
	g_pBrush->SetColor(D2D1::ColorF(r, g, b, a));
	g_pRenderTarget->DrawRectangle(D2D1::RectF(x, y, x + w, y + h), g_pBrush, thickness);
}
void Direct2DRedner::FillRectA(float x, float y, float w, float h, float r, float g, float b, float a) {
	g_pBrush->SetColor(D2D1::ColorF(r, g, b, a));
	g_pRenderTarget->FillRectangle(D2D1::RectF(x, y, x + w, y + h), g_pBrush);
}
void Direct2DRedner::DrawCircle(float x, float y, float radius, float r, float g, float b, float a, float thickness) {
	g_pBrush->SetColor(D2D1::ColorF(r, g, b, a));
	g_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), g_pBrush, thickness);
}
void Direct2DRedner::FillCircle(float x, float y, float radius, float r, float g, float b, float a) {
	g_pBrush->SetColor(D2D1::ColorF(r, g, b, a));
	g_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), g_pBrush);
}

// 原汁原味的绘制方法
void Direct2DRedner::DrawTextA(const wchar_t* text, float x, float y, float r, float g, float b, float a) {
	g_pBrush->SetColor(D2D1::ColorF(r, g, b, a));
	g_pRenderTarget->DrawText(text, (UINT32)wcslen(text), g_pTextFormat,
		D2D1::RectF(x, y, x + 1000, y + 100), g_pBrush);
}

// 带描边文本的绘制方法
void Direct2DRedner::DrawTextF(float x, float y, float r, float g, float b, float a, const wchar_t* format, ...) {
	wchar_t buffer[512];
	va_list args;
	va_start(args, format);
	vswprintf_s(buffer, 512, format, args);
	va_end(args);

	UINT32 textLength = (UINT32)wcslen(buffer);

	// 8方向描边（上下左右+四个对角）
	D2D1_POINT_2F offsets[] = {
		D2D1::Point2F(x - 1, y),      // 左
		D2D1::Point2F(x + 1, y),      // 右
		D2D1::Point2F(x, y - 1),      // 上
		D2D1::Point2F(x, y + 1),      // 下
		D2D1::Point2F(x - 1, y - 1),  // 左上
		D2D1::Point2F(x + 1, y - 1),  // 右上
		D2D1::Point2F(x - 1, y + 1),  // 左下
		D2D1::Point2F(x + 1, y + 1)   // 右下
	};

	// 黑色描边
	g_pBrush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.8f)); // 更明显的描边
	for (int i = 0; i < 8; i++) {
		D2D1_RECT_F shadowRect = D2D1::RectF(
			offsets[i].x, offsets[i].y,
			offsets[i].x + 1000, offsets[i].y + 100
		);
		g_pRenderTarget->DrawText(buffer, textLength, g_pTextFormat, shadowRect, g_pBrush);
	}

	// 绘制主文本
	g_pBrush->SetColor(D2D1::ColorF(r, g, b, a));
	D2D1_RECT_F textRect = D2D1::RectF(x, y, x + 1000, y + 100);
	g_pRenderTarget->DrawText(buffer, textLength, g_pTextFormat, textRect, g_pBrush);
}

void Direct2DRedner::DrawTextFBottom(float rect_x, float rect_y, float rect_width, float r, float g, float b, float a, const wchar_t* format, ...) {
	wchar_t buffer[512];
	va_list args;
	va_start(args, format);
	vswprintf_s(buffer, 512, format, args);
	va_end(args);

	UINT32 textLength = (UINT32)wcslen(buffer);

	// 计算文本尺寸
	IDWriteTextLayout* textLayout = nullptr;
	g_pDWriteFactory->CreateTextLayout(
		buffer, textLength, g_pTextFormat,
		1000.0f, 100.0f, &textLayout
	);

	DWRITE_TEXT_METRICS textMetrics;
	textLayout->GetMetrics(&textMetrics);
	float text_width = textMetrics.width;

	// 计算居中位置
	float text_x = rect_x + (rect_width - text_width) / 2.0f;
	float text_y = rect_y;

	// 8方向描边
	D2D1_POINT_2F offsets[] = {
		D2D1::Point2F(text_x - 1, text_y),      // 左
		D2D1::Point2F(text_x + 1, text_y),      // 右
		D2D1::Point2F(text_x, text_y - 1),      // 上
		D2D1::Point2F(text_x, text_y + 1),      // 下
		D2D1::Point2F(text_x - 1, text_y - 1),  // 左上
		D2D1::Point2F(text_x + 1, text_y - 1),  // 右上
		D2D1::Point2F(text_x - 1, text_y + 1),  // 左下
		D2D1::Point2F(text_x + 1, text_y + 1)   // 右下
	};

	// 黑色描边 (200/255 ≈ 0.784)
	g_pBrush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.784f));
	for (int i = 0; i < 8; i++) {
		D2D1_RECT_F shadowRect = D2D1::RectF(
			offsets[i].x, offsets[i].y,
			offsets[i].x + text_width + 10, offsets[i].y + 100
		);
		g_pRenderTarget->DrawText(buffer, textLength, g_pTextFormat, shadowRect, g_pBrush);
	}

	// 绘制主文本
	g_pBrush->SetColor(D2D1::ColorF(r, g, b, a));
	D2D1_RECT_F textRect = D2D1::RectF(text_x, text_y, text_x + text_width + 10, text_y + 100);
	g_pRenderTarget->DrawText(buffer, textLength, g_pTextFormat, textRect, g_pBrush);

	// 释放资源
	textLayout->Release();
}

// 绘制血条（横向）
void Direct2DRedner::DrawHealth(float x, float y, float stringLength, float Distance, float hp, float maxhp) {
	// 根据血量选择颜色
	float r, g, b, a;
	if (hp >= 75) {
		r = 255.0f / 255.0f; g = 236.0f / 255.0f; b = 233.0f / 255.0f; a = 210.0f / 255.0f;
	}
	else if (hp < 75 && hp >= 35) {
		r = 249.0f / 255.0f; g = 136.0f / 255.0f; b = 25.0f / 255.0f; a = 210.0f / 255.0f;
	}
	else {
		r = 230.0f / 255.0f; g = 50.0f / 255.0f; b = 35.0f / 255.0f; a = 210.0f / 255.0f;
	}

	float barWidth = stringLength - Distance / maxhp;
	float barX = x - stringLength / 2.0f;
	float barY = y - 10.0f;

	// 绘制血条外框（白色边框）
	DrawRect(barX, barY, barWidth, 5, 1.0f, 1.0f, 1.0f, 250.0f / 255.0f, 1.0f);

	// 绘制血条填充（根据当前血量）
	float currentBarWidth = barWidth / maxhp * hp;
	FillRectA(barX, barY + 1, currentBarWidth, 3, r, g, b, a);
}

// 绘制信号血条（竖向）
void Direct2DRedner::DrawSignalHP(float x, float y, float h, float hp, float maxhp) {
	// 计算当前血量对应的高度
	float currentH = h / maxhp * hp;

	// 绘制竖向血条（黄绿色）
	DrawLine(x - 4, y, x - 4, y + currentH,
		235.0f / 255.0f, 233.0f / 255.0f, 78.0f / 255.0f, 220.0f / 255.0f,
		4.5f);
}

void Direct2DRedner::DrawBegin(){
	g_pRenderTarget->BeginDraw();
	g_pRenderTarget->Clear(D2D1::ColorF(0, 0, 0, 0));

}

void Direct2DRedner::DrawEnd() {

	g_pRenderTarget->EndDraw();

}





void Direct2DRedner::Cleanup() {
	if (g_pTextFormat) g_pTextFormat->Release();
	if (g_pDWriteFactory) g_pDWriteFactory->Release();
	if (g_pBrush) g_pBrush->Release();
	if (g_pRenderTarget) g_pRenderTarget->Release();
	if (g_pD2DFactory) g_pD2DFactory->Release();
}
bool Direct2DRedner::Initialize() {

	// 设置DPI感知
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Example", nullptr };
	::RegisterClassExW(&wc);
	HWND hwnd = ::CreateWindowExW(WS_EX_TOPMOST, wc.lpszClassName, L"Example", WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);


	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	MARGINS margins = { -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(hwnd, &margins);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	SetWindowLong(hwnd, GWL_EXSTYLE,
		GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);

	// 初始化 Direct2D
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);

	// 关键：使用真实屏幕分辨率创建渲染目标
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		96.0f, 96.0f  // 让系统自动处理DPI
	);

	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps = D2D1::HwndRenderTargetProperties(
		hwnd,
		D2D1::SizeU(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN))  // 使用真实分辨率
	);

	g_pD2DFactory->CreateHwndRenderTarget(props, hwndProps, &g_pRenderTarget);

	g_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &g_pBrush);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&g_pDWriteFactory));

	g_pDWriteFactory->CreateTextFormat(
		L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 18.0f, L"en-us", &g_pTextFormat
	);


	// 初始化FPS计算
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&lastTime);
	frameCount = 0;
	fps = 0.0f;



	return true;
}


void Direct2DRedner::UpdateFPS() {
	frameCount++;

	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	// 每秒更新一次FPS
	double elapsed = (double)(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
	if (elapsed >= 1.0) {
		fps = frameCount / (float)elapsed;
		frameCount = 0;
		lastTime = currentTime;
	}
}

float Direct2DRedner::GetFPS() {
	return fps;
}