#include "Direct2DRedner.h"


void Render() {
	MSG msg = { 0 };

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			return;
	}



	Direct2DRedner::Draw.DrawBegin();


	Direct2DRedner::Draw.UpdateFPS();

	// 绘制FPS（右上角）
	Direct2DRedner::Draw.DrawTextF(
		10.F, 10.0f,  // 右上角位置
		0.0f, 1.0f, 0.0f, 1.0f,         // 绿色
		L"FPS: %.1f", Direct2DRedner::Draw.GetFPS()
	);

	POINT p;
	GetCursorPos(&p);


	Direct2DRedner::Draw.DrawRect((float)p.x, (float)p.y, 200, 150, 1.0f, 0.0f, 0.0f, 1.0f, 1);           // 红色矩形
	Direct2DRedner::Draw.FillRectA((float)p.x + 350, (float)p.y + 100, 200, 150, 0.0f, 1.0f, 0.0f, 0.5f);     // 半透明绿色填充矩形
	Direct2DRedner::Draw.DrawCircle(450, 350, 50, 0.0f, 0.0f, 1.0f, 1.0f, 1);               // 蓝色圆形
	Direct2DRedner::Draw.FillCircle(650, 350, 50, 1.0f, 1.0f, 0.0f, 0.7f);         // 半透明黄色填充圆
	Direct2DRedner::Draw.DrawLine((float)p.x + 100, (float)p.y + 400, 700, 400, 1.0f, 0.0f, 1.0f, 1.0f, 3.0f); // 粉色线条

	float aaa = 1145.1F;
	Direct2DRedner::Draw.DrawTextF(100, 100, 1.0f, 1.0f, 0.0f, 1.0f, L"距离: %.1fM", aaa);


	float box_x = 200;
	float box_y = 300;
	float box_width = 200;
	float box_height = 300;

	// 绘制方框
	Direct2DRedner::Draw.DrawRect(box_x, box_y, box_width, box_height, 1.0f, 0.0f, 0.0f, 1.0f, 5);

	// 在方框底部居中绘制文本
	float distance = 25.5f;
	Direct2DRedner::Draw.DrawTextFBottom(
		box_x,                    // 方框X
		box_y + box_height + 5,   // 方框底部Y（加5像素间距）
		box_width,                // 方框宽度
		1.0f, 1.0f, 0.0f, 1.0f,  // 白色
		L"距离: %.1fM", distance
	);

	// 也可以在方框顶部绘制（改Y坐标即可）
	Direct2DRedner::Draw.DrawTextFBottom(
		box_x,
		box_y - 25,              // 方框顶部（向上偏移25像素）
		box_width,
		0.0f, 1.0f, 0.0f, 1.0f, // 绿色
		L"敌人"
	);


	// 横血条
	float player_hp = 29.0f;     
	float player_maxhp = 100.0f;  

	Direct2DRedner::Draw.DrawHealth(
		box_x + box_width / 2,  
		box_y,   
		200,       // 血条长度
		1000,        // Distance参数
		player_hp,
		player_maxhp
	);

	// 竖向血条

	Direct2DRedner::Draw.DrawSignalHP(
		box_x,         // X坐标（方框左边）
		box_y,         // Y起始坐标
		box_height,    // 血条最大高度
		75.0f,         // 当前血量
		100.0f         // 最大血量
	);

	Direct2DRedner::Draw.DrawEnd();
}


void ThreadProc() {



	while (true) {

		Render();



	}

}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {

	if (!Direct2DRedner::Draw.Initialize()) { return 0; }


	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadProc, NULL, 0, NULL);


	Sleep(99999999);
	return 0;
}