#include <citro2d.h>

#include <assert.h>
#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

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
unsigned long long int clicks = 0;
unsigned long long int CPC = 1;
unsigned long long int CPS = 0;
int cpsTimer = 0;
int clickerprice = 50;
int clickerown = 0;
unsigned long long int clickerUprice = 1000;
unsigned long long int clickerUown = 1;
int clickm2price = 500;
int clickm2own = 0;
int frame = 0;

int buyscreen = 1;
int buytime = 0;
int buyRepeatDelay = 5;
bool buy1 = false;
bool buy2 = false;
bool controls = true;
bool DEBUG = false;

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

		if (kDown & KEY_SELECT) {
			if (DEBUG) { DEBUG = false; } else { DEBUG = true; controls = false;}
		}

		//Read the touch screen coordinates and add them to a variable
		touchPosition touch;
		hidTouchRead(&touch);

		//buttons
		if (DEBUG) {
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
			//add clickers
			if (kDown & KEY_A) { clickerown += 100; }
			//add clicker multiplier
			if (kDown & KEY_B) { clickerUown += 1; }
			//cpc ++++
			if (kDown & KEY_X) { CPC += 100; }
			//literally just multiply values
			if (kDown & KEY_Y) { CPC *= 2; clickerUown *= 2; clickerown *= 2; }
			//speedy click button
			if (kHeld & KEY_R) { clicks += CPC; }
		} else {
			//controls screen switch
			if (kDown & KEY_X) {
				if (controls) controls = false; else controls = true;
			}
			// buy screen switcher
			if (kDown & KEY_ZR || kDown & KEY_DRIGHT) {
				buyscreen += 1;
				if (buyscreen > 2) {
					buyscreen = 1;
				}
			}
			if (kDown & KEY_ZL || kDown & KEY_DLEFT) {
				buyscreen -= 1;
				if (buyscreen < 1) {
					buyscreen = 2;
				}
			}

			if (kDown & KEY_DUP) {
				buyRepeatDelay++;
				if (buyRepeatDelay > 30) {
					buyRepeatDelay = 30;
				}
			}

			if (kDown & KEY_DDOWN) {
				buyRepeatDelay--;
				if (buyRepeatDelay < 1) {
					buyRepeatDelay = 1;
				}
			}

			// click buttons
			if (kDown != kDownOld || kHeld != kHeldOld || kUp != kUpOld) {
				if (kDown & KEY_A || kDown & KEY_L || kDown & KEY_R) { clicks += CPC; }
			}
		}

		//figure out if the buy buttons are getting pressed
		if (touch.px > 5 && touch.px < 155 && touch.py > 40 && touch.py < 235) buy1 = true; else buy1 = false;
		if (touch.px > 165 && touch.px < 315 && touch.py > 40 && touch.py < 235) buy2 = true; else buy2 = false;
		if (buytime >= 1) buytime -= 1;

		//purchasing code
		if (buy1 && buytime == 0) {
			//click upgrade
			if (clicks >= clickUpPrice && buyscreen == 1) {
				clicks -= clickUpPrice;
				clickUpPrice += 5;
				CPC += 1;
				clickUpOwn++;
				buytime = buyRepeatDelay;
			}
			//clicker upgrade
			if (clicks >= clickerUprice && buyscreen == 2) {
				clicks -= clickerUprice;
				clickerUprice = clickerUprice*1.1f;
				clickerUown += 1;
				buytime = buyRepeatDelay;
			}
		}

		if (buy2 && buytime == 0) {
			//click upgrade, mk2
			if (clicks >= clickm2price && buyscreen == 1) {
				clicks -= clickm2price;
				clickm2price += 100;
				clickm2own++;
				CPC += 10;
				buytime = buyRepeatDelay;
			}
			//clicker
			if (clicks >= clickerprice && buyscreen == 2) {
				clicks -= clickerprice;
				clickerprice += 25;
				clickerown += 1;
				buytime = buyRepeatDelay;
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

		//render scenes
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32(0x1E, 0x1E, 0x2E, 0xFF));
		C2D_SceneBegin(top);

		u32 col = 0xCDD6F4FF;
		//main CPS / CPC display
		drawRect(10, 10, 380, 80, 5, C2D_Color32(0x18, 0x18, 0x28, 0xFF), C2D_Color32(0x58, 0x5B, 0x70, 0xFF));
		drawDynamicText(g_dynBuf, 20.0f, 15.0f, 1.0f, col, font, C2D_AlignLeft, "Clicks: %llu\nCPS   : %llu\nCPC   : %llu", clicks, CPS, CPC);

		drawRect(10, 100, 320, 120, 5, C2D_Color32(0x18, 0x18, 0x28, 0xFF), C2D_Color32(0x58, 0x5B, 0x70, 0xFF));
		if (controls) {
			drawDynamicText(g_dynBuf, 20.0f, 105.0f, 1.0f, col, font, C2D_AlignLeft, "Controls:\nA/L/R: Click\nX: Toggle Control Help\nZL/ZR/D-Pad: Switch Shop Page\nStart: Exit");
		} else {
			if (DEBUG) {
				drawDynamicText(g_dynBuf, 20.0f, 105.0f, 1.0f, col, font, C2D_AlignLeft, "Add Clickers: A\nAdd Clicker Multi: B\nAdd CPC: X\nMult values by 2: Y\nFast Click: R");
				drawDynamicText(g_dynBuf, 5.0f, 220.0f, 0.7f, col, font, C2D_AlignLeft, "DEBUG ENABLED | Frame: %i", frame);
			} else {
				drawDynamicText(g_dynBuf, 20.0f, 105.0f, 1.0f, col, font, C2D_AlignLeft, "Misc:\nBuy Cooldown: %i\nBuy Repeat Delay: %i", buytime, buyRepeatDelay);
				drawDynamicText(g_dynBuf, 5.0f, 220.0f, 0.7f, col, font, C2D_AlignLeft, "Frame: %i", frame);
			}
		}

		//bottom
		C2D_SceneBegin(bottom);
		C2D_TargetClear(bottom, C2D_Color32(0x1E, 0x1E, 0x2E, 0xFF));

		//draw the buy buttons
		//bord: C2D_Color32(0x58, 0x5B, 0x70, 0xFF);
		//fill: C2D_Color32(0x18, 0x18, 0x25, 0xFF);
		drawRect(5 , 40, 150, 195, 5, C2D_Color32(0x18, 0x18, 0x25, 0xFF), C2D_Color32(0x58, 0x5B, 0x70, 0xFF));
		drawRect(165, 40, 150, 195, 5, C2D_Color32(0x18, 0x18, 0x25, 0xFF), C2D_Color32(0x58, 0x5B, 0x70, 0xFF));
		//5 + 145

		//draw the buy buttons' text
		switch (buyscreen) {
			case 1:
				drawDynamicText(g_dynBuf, 160.0f, 5.0f, 1.0f, col, font, C2D_AlignCenter, "Shop: Page 1");
				drawDynamicText(g_dynBuf, 80.0f, 50.0f, 0.9f, col, font, C2D_AlignCenter, "CPC Upgrade\n+1 CPC\nPrice: %i\nPrice+: 5\nOwned: %i", clickUpPrice, clickUpOwn);
				drawDynamicText(g_dynBuf, 240.0f, 50.0f, 0.9f, col, font, C2D_AlignCenter, "CPC Mk.2\n+10 CPC\nPrice: %i\nPrice+: 100\nOwned: %i", clickm2price, clickm2own);
				break;
			case 2:
				drawDynamicText(g_dynBuf, 160.0f, 5.0f, 1.0f, col, font, C2D_AlignCenter, "Shop: Page 2");
				drawDynamicText(g_dynBuf, 80.0f, 50.0f, 0.9f, col, font, C2D_AlignCenter, "Clicker+\nEach clicker\ngets +1 CPS\nPrice: %llu\nPrice+: %llu\nOwned: %llu", clickerUprice, clickerUprice/10, clickerUown-1);
				drawDynamicText(g_dynBuf, 240.0f, 50.0f, 0.9f, col, font, C2D_AlignCenter, "Clicker\n+%llu CPS\nPrice: %i\nPrice+: 25\nOwned: %i", clickerUown, clickerprice, clickerown);
				break;
		}

		C3D_FrameEnd(0);

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
