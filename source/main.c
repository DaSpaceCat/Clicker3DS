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
#include "buy_bgr.h"
#include "text_bgr.h"

int clickUpPrice = 15;
int clicks = 0;
int CPC = 1;
int CPS = 0;
int cpsTimer = 0;
int clickerprice = 50;

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
		//copy our image into the bottom screen's frame buffer
		memcpy(fb, buy_bgr, buy_bgr_size);

		//attempted to display text on top of the store page, didn't work properly
		//memcpy(fb, text_bgr, text_bgr_size);

		//CPS loop
		if (cpsTimer == 0) {
			cpsTimer = 60;
			clicks += CPS;
		} else {cpsTimer -= 1;}
		//update clicks
		consoleSelect(&top);
		printf("\x1b[1;1HClicks: %d\n", clicks);


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
			printf("\x1b[6;1HPress X to upgrade your CPC for %d\n", clickUpPrice);
			printf("\x1b[7;1HPress Y to purchase a clicker, to add to the CPS, for %d\n", clickerprice);
			if(kDown & KEY_X) {
				if (clicks >= clickUpPrice) {
					clicks -= clickUpPrice;
					CPC += 1;
					clickUpPrice += 5;
				}
			}
			if(kDown & KEY_Y) {
				if (clicks >= clickerprice) {
					clicks -= clickerprice;
					clickerprice += 25;
					CPS += 1;
				}
			}
		}

		printf("\x1b[10;1HTouch Screen position X: %d\n", touch.px);
		printf("\x1b[11;1HTouch Screen position Y: %d\n", touch.py);

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
