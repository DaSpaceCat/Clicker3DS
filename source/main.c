#include <citro2d.h>

#include <assert.h>
#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//This includes a header containing definitions of our image
#include "buy1_bgr.h"
#include "buy2_bgr.h"
#include "toptest_bgr.h"

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

C2D_TextBuf g_dynBuf;
C2D_Font font;

// Simple sprite struct
typedef struct
{
	C2D_Sprite spr;
	//float dx, dy; // velocity
} Sprite;

int clickUpPrice = 15;
int clicks = 0;
int CPC = 1;
int CPS = 0;
int cpsTimer = 0;
int clickerprice = 50;
int clickerown = 0;
int clickerUprice = 1000;
int clickerUown = 1;
int clickm2price = 500;

int buyscreen = 1;
int buytime = 0;
bool buy1 = false;
bool buy2 = false;

static C2D_SpriteSheet spriteSheet;
static Sprite sprites[100];

int main(int argc, char **argv)
{
	//Matrix containing the name of each key. Useful for printing when a key is pressed
	/*char keysNames[32][32] = {
		"KEY_A", "KEY_B", "KEY_SELECT", "KEY_START",
		"KEY_DRIGHT", "KEY_DLEFT", "KEY_DUP", "KEY_DDOWN",
		"KEY_R", "KEY_L", "KEY_X", "KEY_Y",
		"", "", "KEY_ZL", "KEY_ZR",
		"", "", "", "",
		"KEY_TOUCH", "", "", "",
		"KEY_CSTICK_RIGHT", "KEY_CSTICK_LEFT", "KEY_CSTICK_UP", "KEY_CSTICK_DOWN",
		"KEY_CPAD_RIGHT", "KEY_CPAD_LEFT", "KEY_CPAD_UP", "KEY_CPAD_DOWN"
	};*/

	// Initialize services
	romfsInit();
	cfguInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	g_dynBuf = C2D_TextBufNew(4096);
	font = C2D_FontLoad("romfs:/FiraCode-Regular.bcfnt");

	// Load graphics
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
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
	u8* fbb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

	u32 kDownOld = 0, kHeldOld = 0, kUpOld = 0; //In these variables there will be information about keys detected in the previous frame

	// Main loop
	while (aptMainLoop())
	{
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

		//memcpy(fbt, toptest_bgr, toptest_bgr_size);
		//render text scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
		C2D_SceneBegin(top);

		char buf[160];
		C2D_Text dynText;
		snprintf(buf, sizeof(buf), "Clicks %i\nCPS: %i", clicks, CPS);
		C2D_TextFontParse(&dynText, font, g_dynBuf, buf);
		C2D_TextOptimize(&dynText);
		C2D_DrawText(&dynText, C2D_AlignLeft, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f);

		C3D_FrameEnd(0);

		//copy our image into the bottom screen's frame buffer
		if (buyscreen == 1) memcpy(fbb, buy1_bgr, buy1_bgr_size);
		if (buyscreen == 2) memcpy(fbb, buy2_bgr, buy2_bgr_size);

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
				CPC += 10;
				buytime = 30;
			}
		}

		//calc CPS
		CPS = clickerown*clickerUown;

		//CPS loop
		if (CPS > 60) clicks += CPS/60; else {
			if (cpsTimer == 0) {
				cpsTimer = 60;
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

		//Set keys old values for the next frame
		kDownOld = kDown;
		kHeldOld = kHeld;
		kUpOld = kUp;

		for (size_t i = 0; i < 100; i ++)
			C2D_DrawSprite(&sprites[i].spr);

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
