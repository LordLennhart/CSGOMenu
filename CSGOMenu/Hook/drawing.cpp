#include "../includes.h"
#include "../ESP/esp.h"

extern Esp::ESP* esp;


// filled rectangle
void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
	D3DRECT rect = { x,y,x + w,y + h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color) {
	if(!esp->LineL)
		D3DXCreateLine(pDevice, &esp->LineL);


	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	esp->LineL->SetWidth(thickness);
	esp->LineL->Draw(Line, 2, color);
}

void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color) {
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawEspBox2D(Vec2 top, Vec2 bot, int thickness, D3DCOLOR color) {
	int height = ABS(top.y - bot.y);
	Vec2 tl, tr, bl, br;
	
	tl.x = top.x - height / 4;
	tr.x = top.x + height / 4;
	tl.y = tr.y = top.y;

	bl.x = bot.x - height / 4;
	br.x = bot.x + height / 4;
	bl.y = br.y = bot.y;


	DrawLine(tl, tr, thickness, color);
	DrawLine(bl, br, thickness, color);
	DrawLine(tl, bl, thickness, color);
	DrawLine(tr, br, thickness, color);

}

void DrawEspBox3D(Vec3 top, Vec3 bot, float angle, int width, int thickness, D3DCOLOR color){
	int height3D = ABS(top.z - bot.z);
	Vec3 b1, b2, b3, b4, t1, t2, t3, t4;

	b1.z = b2.z = b3.z = b4.z = bot.z;

	b1.x = bot.x + (cos(TORAD(angle + 45)) * width);
	b1.y = bot.y + (sin(TORAD(angle + 45)) * width);

	b2.x = bot.x + (cos(TORAD(angle + 135)) * width);
	b2.y = bot.y + (sin(TORAD(angle + 135)) * width);

	b3.x = bot.x + (cos(TORAD(angle + 225)) * width);
	b3.y = bot.y + (sin(TORAD(angle + 225)) * width);

	b4.x = bot.x + (cos(TORAD(angle + 315)) * width);
	b4.y = bot.y + (sin(TORAD(angle + 315)) * width);

	t1.x = b1.x;
	t1.y = b1.y;
	t1.z = b1.z + height3D;

	t2.x = b2.x;
	t2.y = b2.y;
	t2.z = b2.z + height3D;

	t3.x = b3.x;
	t3.y = b3.y;
	t3.z = b3.z + height3D;

	t4.x = b4.x;
	t4.y = b4.y;
	t4.z = b4.z + height3D;

	Vec2 b1_2D, b2_2D, b3_2D, b4_2D, t1_2D, t2_2D, t3_2D, t4_2D;
	if (W2S(b1, b1_2D) && W2S(b2, b2_2D) && W2S(b3, b3_2D) && W2S(b4, b4_2D) && W2S(t1, t1_2D) && W2S(t2, t2_2D) && W2S(t3, t3_2D) && W2S(t4, t4_2D)) {
		//cols
		DrawLine(t1_2D, b1_2D, thickness, color);
		DrawLine(t2_2D, b2_2D, thickness, color);
		DrawLine(t3_2D, b3_2D, thickness, color);
		DrawLine(t4_2D, b4_2D, thickness, color);
		//top base
		DrawLine(t1_2D, t2_2D, thickness, color);
		DrawLine(t2_2D, t3_2D, thickness, color);
		DrawLine(t3_2D, t4_2D, thickness, color);
		DrawLine(t4_2D, t1_2D, thickness, color);
		//bottom base
		DrawLine(b1_2D, b2_2D, thickness, color);
		DrawLine(b2_2D, b3_2D, thickness, color);
		DrawLine(b3_2D, b4_2D, thickness, color);
		DrawLine(b4_2D, b1_2D, thickness, color);
	}
}

void DrawText(const char* text, float x, float y, D3DCOLOR color) {
	RECT rect;

	if (!esp->FontF)
		D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &esp->FontF);

	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	esp->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	SetRect(&rect, x, y, x, y);
	esp->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);

}