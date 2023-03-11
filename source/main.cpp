#include <citro2d.h>

#include <assert.h>
#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

//This includes a header containing definitions of our image
#include "buy1_bgr.h"
#include "buy2_bgr.h"
#include "toptest_bgr.h"

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

C2D_Font font;

// Simple sprite struct
typedef struct
{
	C2D_Sprite spr;
	//float dx, dy; // velocity
} Sprite;

int clickUpPrice = 15;
int clickUpOwn = 0;
int clicks = 0;
int CPC = 1;
int CPS = 0;
int cpsTimer = 0;
int clickerprice = 50;
int clickerown = 0;
int clickerUprice = 1000;
int clickerUown = 1;
int clickm2price = 500;
int clickm2own = 0;
int frame = 0;

int buyscreen = 1;
int buytime = 0;
bool buy1 = false;
bool buy2 = false;

static C2D_SpriteSheet spriteSheet;
static Sprite sprites[100];

static void drawDynamicText(C2D_TextBuf buffer, float x, float y, float scale, u32 color, C2D_Font rfont, u32 flags, const char* text, ...) {
	char buff[160];
	C2D_Text textVar;
	va_list va;
	va_start(va, text);
	vsnprintf(buff, sizeof(buff), text, va);
	va_end(va);
	C2D_TextFontParse(&textVar, rfont, buffer, buff);
	C2D_TextOptimize(&textVar);
	C2D_DrawText(&textVar, flags | C2D_WithColor, x, y, 0.0f, scale, scale, color);
}

static void drawRect(float x, float y, float w, float h, float p, u32 color, u32 border) {
	C2D_DrawRectSolid(x, y, 0.0f, w, h, border);
	C2D_DrawRectSolid(x + p, y + p, 0.0f, w - p * 2, h - p * 2, color);
}

int main(int argc, char **argv)
{
	// Initialize services
	romfsInit();
	cfguInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	C2D_TextBuf g_dynBuf;

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	g_dynBuf = C2D_TextBufNew(4096);
	font = C2D_FontLoad("romfs:/FiraCode-Regular.bcfnt");

	// Load graphics
	spriteSheet = C2D_SpriteSheetLoad("romfs:/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	size_t numImages = C2D_SpriteSheetCount(spriteSheet);
	for (size_t i = 0; i < numImages; i++) {
		C2D_SpriteFromSheet(&sprites->spr, spriteSheet, rand() % numImages);
		C2D_SpriteSetCenter(&sprites->spr, 0.5f, 0.5f);
		C2D_SpriteSetPos(&sprites->spr, rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
	}

	//set framebuffers
	gfxSetDoubleBuffering(GFX_BOTTOM, false);
	gfxSetDoubleBuffering(GFX_TOP, false);

	//Get the bottom and top screen frame buffers
	//u8* fbt = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	//u8* fbb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

	u32 kDownOld = 0, kHeldOld = 0, kUpOld = 0; //In these variables there will be information about keys detected in the previous frame

	// Main loop
	while (aptMainLoop())
	{
		frame++;
		C2D_TextBufClear(g_dynBuf);

		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();
		//hidKeysHeld returns information about which buttons have are held down in this frame
		u32 kHeld = hidKeysHeld();
		//hidKeysUp returns information about which buttons have been just released
		u32 kUp = hidKeysUp();

		if (kDown & KEY_START) break; // break in order to return to hbmenu

		//Read the touch screen coordinates and add them to a variable
		touchPosition touch;
		hidTouchRead(&touch);

		//shop page switching code
		if (kDown & KEY_ZR) {
			buyscreen += 1;
			if (buyscreen > 2) {
				buyscreen = 1;
			}
		}
		if (kDown & KEY_ZL) {
			buyscreen -= 1;
			if (buyscreen < 1) {
				buyscreen = 2;
			}
		}

		//figure out if the buy buttons are getting pressed
		if (touch.px > 20 && touch.px < 150 && touch.py > 40 && touch.py < 220) buy1 = true; else buy1 = false;
		if (touch.px > 170 && touch.px < 300 && touch.py > 40 && touch.py < 220) buy2 = true; else buy2 = false;
		if (buytime >= 1) buytime -= 1;

		//purchasing code
		if (buy1 && buytime == 0) {
			//click upgrade
			if (clicks >= clickUpPrice && buyscreen == 1) {
				clicks -= clickUpPrice;
				clickUpPrice += 5;
				CPC += 1;
				clickUpOwn++;
				buytime = 30;
			}
			//clicker upgrade
			if (clicks >= clickerUprice && buyscreen == 2) {
				clicks -= clickerUprice;
				clickerUprice = clickerUprice*2;
				clickerUown += 1;
				buytime = 30;
			}
		}

		if (buy2 && buytime == 0) {
			//clicker
			if (clicks >= clickerprice && buyscreen == 1) {
				clicks -= clickerprice;
				clickerprice += 25;
				clickerown += 1;
				buytime = 30;
			}
			//click upgrade, mk2
			if (clicks >= clickm2price && buyscreen == 2) {
				clicks -= clickm2price;
				clickm2price += 100;
				clickm2own++;
				CPC += 10;
				buytime = 30;
			}
		}

		//calc CPS
		CPS = clickerown*clickerUown;

		//CPS loop
		if (CPS > 30) clicks += CPS/30; else {
			if (cpsTimer == 0) {
				cpsTimer = 30;
				clicks += CPS;
			} else {cpsTimer -= 1;}
		}
		
		//update clicks (console)
		//consoleSelect(&top);
		//printf("\x1b[1;1HClicks: %d\n", clicks);

		if (kDown != kDownOld || kHeld != kHeldOld || kUp != kUpOld)
		{
			if (kDown & KEY_A || kDown & KEY_L || kDown & KEY_R) {clicks += CPC;}
		}

		//render text scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32(0x1E, 0x1E, 0x2E, 0xFF));
		C2D_SceneBegin(top);

		u32 col = 0xCDD6F4FF;
		//main CPS / CPC display
		drawDynamicText(g_dynBuf, 200.0f, 10.0f, 1.0f, col, font, C2D_AlignCenter, "Clicks %u\nCPS: %i\nCPC: %i", clicks, CPS, CPC);
		drawDynamicText(g_dynBuf, 5.0f, 75.0f, 1.0f, col, font, C2D_AlignLeft, "Misc:\nFrame: %i\nCPS Timer: %i\nBuy Cooldown: %i", frame, cpsTimer, buytime);

		//bottom
		C2D_SceneBegin(bottom);
		C2D_TargetClear(bottom, C2D_Color32(0x1E, 0x1E, 0x2E, 0xFF));

		//draw the buy buttons
		//bord: C2D_Color32(0x58, 0x5B, 0x70, 0xFF);
		//fill: C2D_Color32(0x18, 0x18, 0x25, 0xFF);
		drawRect(20 , 40, 130, 180, 5, C2D_Color32(0x18, 0x18, 0x25, 0xFF), C2D_Color32(0x58, 0x5B, 0x70, 0xFF));
		drawRect(170, 40, 130, 180, 5, C2D_Color32(0x18, 0x18, 0x25, 0xFF), C2D_Color32(0x58, 0x5B, 0x70, 0xFF));

		//draw the buy buttons' text
		switch (buyscreen) {
			case 1:
				drawDynamicText(g_dynBuf, 160.0f, 5.0f, 1.0f, col, font, C2D_AlignCenter, "Page 1");
				drawDynamicText(g_dynBuf, 85.0f, 80.0f, 0.9f, col, font, C2D_AlignCenter, "CPC Upgrade\n+1 CPC\nPrice: %i\nPrice+: 5\nOwned: %i", clickUpPrice, clickUpOwn);
				drawDynamicText(g_dynBuf, 235.0f, 80.0f, 0.9f, col, font, C2D_AlignCenter, "Clicker\n+1 CPS\nPrice: %i\nPrice+: 25\nOwned: %i", clickerprice, clickerown);
				break;
			case 2:
				drawDynamicText(g_dynBuf, 160.0f, 5.0f, 1.0f, col, font, C2D_AlignCenter, "Page 2");
				drawDynamicText(g_dynBuf, 85.0f, 80.0f, 0.9f, col, font, C2D_AlignCenter, "Clicker+\nPrice: %i\nPrice+: %i\nOwned: %i", clickerUprice, clickerUprice, clickerUown);
				drawDynamicText(g_dynBuf, 235.0f, 80.0f, 0.9f, col, font, C2D_AlignCenter, "CPC Mk.2\nPrice: %i\nPrice+: 100\nOwned: %i", clickm2price, clickm2own);
				break;
		}

		C3D_FrameEnd(0);

		//copy our image into the bottom screen's frame buffer
		//if (buyscreen == 1) memcpy(fbb, buy1_bgr, buy1_bgr_size);
		//if (buyscreen == 2) memcpy(fbb, buy2_bgr, buy2_bgr_size);

		//Set keys old values for the next frame
		kDownOld = kDown;
		kHeldOld = kHeld;
		kUpOld = kUp;

		/*for (size_t i = 0; i < 100; i ++)
			C2D_DrawSprite(&sprites[i].spr);*/

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}

	// Delete graphics
	C2D_SpriteSheetFree(spriteSheet);

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	cfguExit();
	gfxExit();
	romfsExit();
	return 0;
}
