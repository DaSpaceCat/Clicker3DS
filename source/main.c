/*
	Circle Pad example made by Aurelio Mannara for libctru
	Please refer to https://github.com/devkitPro/libctru/blob/master/libctru/include/3ds/services/hid.h for more information
	This code was modified for the last time on: 12/13/2014 2:20 UTC+1

	This wouldn't be possible without the amazing work done by:
	-Smealum
	-fincs
	-WinterMute
	-yellows8
	-plutoo
	-mtheall
	-Many others who worked on 3DS and I'm surely forgetting about
*/

#include <3ds.h>
#include <stdio.h>
#include <string.h>

//This include a header containing definitions of our image
#include "buy1_bgr.h"
#include "buy2_bgr.h"

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
	gfxInitDefault();

	PrintConsole top;

	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	consoleInit(GFX_TOP, &top);
	//consoleInit(GFX_BOTTOM, &bottom);

	//set framebuffers
	gfxSetDoubleBuffering(GFX_BOTTOM, false);

	//Get the bottom screen's frame buffer
	u8* fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

	u32 kDownOld = 0, kHeldOld = 0, kUpOld = 0; //In these variables there will be information about keys detected in the previous frame
	
	consoleSelect(&top);
	printf("\x1b[1;1HPress A to start.");

	//consoleSelect(&bottom);
	//printf("\x1b[1;1HPress Start to exit.");

	// Main loop
	while (aptMainLoop())
	{
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

		//copy our image into the bottom screen's frame buffer
		if (buyscreen == 1) memcpy(fb, buy1_bgr, buy1_bgr_size);
		if (buyscreen == 2) memcpy(fb, buy2_bgr, buy2_bgr_size);

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
				clickerUown = clickerUown*2;
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
		//update clicks
		consoleSelect(&top);
		printf("\x1b[1;1HClicks: %d\n", clicks);

		//Do the keys printing only if keys have changed
		if (kDown != kDownOld || kHeld != kHeldOld || kUp != kUpOld)
		{
			//Clear console
			consoleClear();
			
			/*consoleSelect(&bottom);
			//These two lines must be rewritten because we cleared the whole console
			printf("\x1b[1;1HPress Start to exit.");
			printf("\x1b[2;1HCirclePad position:");*/

			consoleSelect(&top);
			consoleClear();
			if (kDown & KEY_A || kDown & KEY_L || kDown & KEY_R) {clicks += CPC;}
			printf("\x1b[3;1HClicks per Click: %d\n", CPC);
			printf("\x1b[4;1HClicks per Second: %d\n", CPS);
			if (buyscreen == 1) {
				printf("\x1b[6;1HCPC upgrade is: %d\n", clickUpPrice);
				printf("\x1b[7;1HClickers are: %d\n", clickerprice);
			}
			if (buyscreen == 2) {
				printf("\x1b[6;1HClicker Upgrade is: %d\n", clickerUprice);
				printf("\x1b[7;1HCPC Upgrade mk.2 is: %d\n", clickm2price);
			}
	
		}

		//we don't need these anymore, but they just show the position of the touchscreen input on the top display.
		/*printf("\x1b[10;1HTouch Screen position X: %d\n", touch.px);
		printf("\x1b[11;1HTouch Screen position Y: %d\n", touch.py);*/

		//Set keys old values for the next frame
		kDownOld = kDown;
		kHeldOld = kHeld;
		kUpOld = kUp;

		circlePosition pos;

		//Read the CirclePad position
		hidCircleRead(&pos);

		/*consoleSelect(&bottom);
		//Print the CirclePad position
		printf("\x1b[3;1H%04d; %04d", pos.dx, pos.dy);

		//print the CPS timer value
		printf("\x1b[29 ;1HCPS timer value: %d\n", cpsTimer);*/

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}

	// Exit services
	gfxExit();
	return 0;
}
