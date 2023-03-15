#include <assert.h>
#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "vshader_shbin.h"
#include "cube.h"

#define CLEAR_COLOR 0x1E1E2EFF
#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

C2D_Font font;

static DVLB_s* vshader_dvlb;
static shaderProgram_s program;
static int uLoc_projection, uLoc_modelView;
static C3D_Mtx projection;

static C3D_LightEnv lightEnv;
static C3D_Light light;
static C3D_LightLut lut_Spec;

static C3D_AttrInfo vbo_attrInfo;
static C3D_BufInfo vbo_bufInfo;
static void* vbo_data;
static float angleX = 0.0, angleY = 0.0, angleZ = 0.0;

unsigned long long int clickUpPrice = 15;
unsigned long long int clickUpOwn = 0;
unsigned long long int clicks = 0;
unsigned long long int CPC = 1;
unsigned long long int CPS = 0;
unsigned long long int clickerprice = 50;
unsigned long long int clickerown = 0;
unsigned long long int clickerUprice = 1000;
unsigned long long int clickerUown = 1;
unsigned long long int clickm2price = 500;
unsigned long long int clickm2own = 0;
int frame = 0;
int cpsTimer = 0;
int buyscreen = 1;
int buytime = 0;
int buyRepeatDelay = 5;
int buyScreens = 3;
bool buy1 = false;
bool buy2 = false;
bool controls = true;
bool DEBUG = false;
bool gradient = true;

static void scene3dInt(void) {
	// Load the vertex shader, create a shader program and bind it
	vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);

	// Get the location of the uniforms
	uLoc_projection   = shaderInstanceGetUniformLocation(program.vertexShader, "projection");
	uLoc_modelView    = shaderInstanceGetUniformLocation(program.vertexShader, "modelView");

	// Configure attributes for use with the vertex shader
	AttrInfo_Init(&vbo_attrInfo);
	AttrInfo_AddLoader(&vbo_attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(&vbo_attrInfo, 1, GPU_FLOAT, 3); // v1=normal

	// Create the VBO (vertex buffer object)
	vbo_data = linearAlloc(sizeof(vertex_list));
	memcpy(vbo_data, vertex_list, sizeof(vertex_list));

	// Configure buffers
	BufInfo_Init(&vbo_bufInfo);
	BufInfo_Add(&vbo_bufInfo, vbo_data, sizeof(vertex), 2, 0x10);

	static const C3D_Material material =
		{
			{ 0.1f, 0.1f, 0.1f }, //ambient
			{ 0.4f, 0.4f, 0.4f }, //diffuse
			{ 0.5f, 0.5f, 0.5f }, //specular0
			{ 0.0f, 0.0f, 0.0f }, //specular1
			{ 0.0f, 0.0f, 0.0f }, //emission
		};

	C3D_LightEnvInit(&lightEnv);
	C3D_LightEnvMaterial(&lightEnv, &material);

	LightLut_Phong(&lut_Spec, 20.0f);
	C3D_LightEnvLut(&lightEnv, GPU_LUT_D0, GPU_LUTINPUT_NH, false, &lut_Spec);

	C3D_LightInit(&light, &lightEnv);
}
static void scene3dBind(void) {
	C3D_BindProgram(&program);
	C3D_SetAttrInfo(&vbo_attrInfo);
	C3D_SetBufInfo(&vbo_bufInfo);
	C3D_LightEnvBind(&lightEnv);
	C3D_DepthTest(true, GPU_GREATER, GPU_WRITE_ALL);
	C3D_CullFace(GPU_CULL_BACK_CCW);

	// Configure the first fragment shading substage to blend the fragment primary color
	// with the fragment secondary color.
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_FRAGMENT_PRIMARY_COLOR, GPU_FRAGMENT_SECONDARY_COLOR, GPU_PRIMARY_COLOR);
	C3D_TexEnvFunc(env, C3D_Both, GPU_ADD);

	// Clear out the other texenvs
	C3D_TexEnvInit(C3D_GetTexEnv(1));
	C3D_TexEnvInit(C3D_GetTexEnv(2));
	C3D_TexEnvInit(C3D_GetTexEnv(3));
	C3D_TexEnvInit(C3D_GetTexEnv(4));
	C3D_TexEnvInit(C3D_GetTexEnv(5));
}
static void scene3dRender(float iod) {
	// Bind the program to render the VBO scene
	scene3dBind();

	// Compute the projection matrix
	Mtx_PerspStereoTilt(&projection, C3D_AngleFromDegrees(40.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, iod, 2.0f, false);

	C3D_FVec objPos   = FVec4_New(0.0f, 0.0f, -7.0f, 1.0f);
	C3D_FVec lightPos = FVec4_New(0.0f, 0.0f, -0.5f, 1.0f);

	// Calculate the modelView matrix
	C3D_Mtx modelView;
	Mtx_Identity(&modelView);
	Mtx_Translate(&modelView, objPos.x, objPos.y, objPos.z, true);
	Mtx_RotateY(&modelView, C3D_Angle(angleY), true);
	Mtx_RotateX(&modelView, C3D_Angle(angleX), true);
	Mtx_RotateZ(&modelView, C3D_Angle(angleZ), true);
	Mtx_Scale(&modelView, 2.0f, 2.0f, 2.0f);

	C3D_LightPosition(&light, &lightPos);

	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_modelView,  &modelView);

	// Draw the VBO
	C3D_DrawArrays(GPU_TRIANGLES, 0, vertex_list_count);
}
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
static void drawGradientRect(float x, float y, float w, float h, float p, u32 color, int r1, int g1, int b1, int r2, int g2, int b2, int opacity) {
	C2D_DrawRectangle(x, y, 0.0f, w, h, C2D_Color32(r1, g1, b1, opacity), C2D_Color32((r1*w/(w+h) + r2*h/(w+h)), (g1*w/(w+h) + g2*h/(w+h)), (b1*w/(w+h) + b2*h/(w+h)), opacity), C2D_Color32((r1*h/(w+h) + r2*w/(w+h)), (g1*h/(w+h) + g2*w/(w+h)), (b1*h/(w+h) + b2*w/(w+h)), opacity), C2D_Color32(r2, g2, b2, opacity));
	C2D_DrawRectSolid(x + p, y + p, 0.0f, w - p * 2, h - p * 2, color);
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
	gfxSet3D(true);
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);

	C2D_TextBuf g_dynBuf;

	// Create screens..?
	//C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	C3D_RenderTarget* targetLeft  = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTarget* targetRight = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(targetLeft,  GFX_TOP, GFX_LEFT,  DISPLAY_TRANSFER_FLAGS);
	C3D_RenderTargetSetOutput(targetRight, GFX_TOP, GFX_RIGHT, DISPLAY_TRANSFER_FLAGS);

	scene3dInt();

	g_dynBuf = C2D_TextBufNew(4096);
	font = C2D_FontLoad("romfs:/FiraCode-Regular.bcfnt");

	//set framebuffers
	gfxSetDoubleBuffering(GFX_BOTTOM, false);
	//gfxSetDoubleBuffering(GFX_TOP, false);

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

		if (kHeld & KEY_START && kHeld & KEY_R) break; // break in order to return to hbmenu

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
				if (buyscreen > buyScreens) {
					buyscreen = 1;
				}
			}
			if (kDown & KEY_ZL) {
				buyscreen -= 1;
				if (buyscreen < 1) {
					buyscreen = buyScreens;
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
			if (kDown & KEY_Y) {
				if (gradient) { gradient = false; } else { gradient = true; }
			}
			// buy screen switcher
			if (kDown & KEY_ZR || kDown & KEY_DRIGHT) {
				buyscreen += 1;
				if (buyscreen > buyScreens) {
					buyscreen = 1;
				}
			}
			if (kDown & KEY_ZL || kDown & KEY_DLEFT) {
				buyscreen -= 1;
				if (buyscreen < 1) {
					buyscreen = buyScreens;
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

		//other 3d rendering stuff
		float slider = osGet3DSliderState();
		float iod = slider/3;

		//figure out if the buy buttons are getting pressed
		if (touch.px > 5 && touch.px < 155 && touch.py > 40 && touch.py < 235) buy1 = true; else buy1 = false;
		if (touch.px > 165 && touch.px < 315 && touch.py > 40 && touch.py < 235) buy2 = true; else buy2 = false;
		if (buytime >= 1) buytime -= 1;

		//purchasing code
		//left side
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
		//right side
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

		//rotate
		angleX += 1.0f/1024;
		angleY += 1.0f/256;
		angleZ += 1.0f/512;

		//render scenes
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		//C2D_TargetClear(top, C2D_Color32(0x1E, 0x1E, 0x2E, 0xFF));
		C3D_RenderTargetClear(targetLeft, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
		C3D_FrameDrawOn(targetLeft);
		C2D_SceneTarget(targetLeft);
		//C2D_SceneBegin(top);

		u32 col = 0xCDD6F4FF;
		int opacity = 0xEE;
		if (gradient) {
			opacity = 0xEE;
		} else {
			opacity = 0x00;
		}
		//main CPS / CPC display
		//drawGradientRect(0, 0, 400, 240, 0, C2D_Color32(0x18, 0x18, 0x28, 0x00), 0xA6, 0xE3, 0xA1, 0x89, 0xB4, 0xFA);
		scene3dRender(-iod);
		C2D_Prepare();
		drawGradientRect(10, 10, 380, 80, 5, C2D_Color32(0x18, 0x18, 0x28, 0x55), 0xF5, 0xC2, 0xE7, 0xFA, 0xB3, 0x87, opacity);
		drawDynamicText(g_dynBuf, 20.0f, 15.0f, 1.0f, col, font, C2D_AlignLeft, "Clicks.: %llu\nCPS....: %llu\nCPC....: %llu", clicks, CPS, CPC);

		if (controls) {
			drawGradientRect(10, 100, 320, 120, 5, C2D_Color32(0x18, 0x18, 0x28, 0x55), 0xFA, 0xB3, 0x87, 0xF5, 0xC2, 0xE7, opacity);
			drawDynamicText(g_dynBuf, 20.0f, 105.0f, 1.0f, col, font, C2D_AlignLeft, "Controls:\nA/L/R: Click\nX: Toggle Control Help\nZL/ZR/D-Pad: Switch Shop Page\nR+Start: Exit");
		} else {
			if (DEBUG) {
				drawGradientRect(10, 100, 320, 120, 5, C2D_Color32(0x18, 0x18, 0x28, 0x55), 0xFA, 0xB3, 0x87, 0xF5, 0xC2, 0xE7, opacity);
				drawDynamicText(g_dynBuf, 20.0f, 105.0f, 1.0f, col, font, C2D_AlignLeft, "Add Clickers: A\nAdd Clicker Multi: B\nAdd CPC: X\nMult values by 2: Y\nFast Click: R");
			} else {
				drawGradientRect(10, 100, 320, 120, 5, C2D_Color32(0x18, 0x18, 0x28, 0x55), 0xFA, 0xB3, 0x87, 0xF5, 0xC2, 0xE7, opacity);
				drawDynamicText(g_dynBuf, 20.0f, 105.0f, 1.0f, col, font, C2D_AlignLeft, "Misc:\nBuy Cooldown: %i\nBuy Repeat Delay: %i", buytime, buyRepeatDelay);
			}
		}

		/*if (iod > 0.0f) {
			C3D_RenderTargetClear(targetRight, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
			C3D_FrameDrawOn(targetRight);
			C2D_SceneTarget(targetRight);
			scene3dRender(iod);

			drawGradientRect(10, 10, 380, 80, 5, C2D_Color32(0x18, 0x18, 0x28, 0xEE), 0xF5, 0xC2, 0xE7, 0xFA, 0xB3, 0x87);
			drawDynamicText(g_dynBuf, 20.0f, 15.0f, 1.0f, col, font, C2D_AlignLeft, "Clicks.: %llu\nCPS....: %llu\nCPC....: %llu", clicks, CPS, CPC);

			if (controls) {
				drawGradientRect(10, 100, 320, 120, 5, C2D_Color32(0x18, 0x18, 0x28, 0xEE), 0xFA, 0xB3, 0x87, 0xF5, 0xC2, 0xE7);
				drawDynamicText(g_dynBuf, 20.0f, 105.0f, 1.0f, col, font, C2D_AlignLeft, "Controls:\nA/L/R: Click\nX: Toggle Control Help\nZL/ZR/D-Pad: Switch Shop Page\nStart: Exit");
			} else {
				if (DEBUG) {
					drawGradientRect(10, 100, 320, 120, 5, C2D_Color32(0x18, 0x18, 0x28, 0xEE), 0xFA, 0xB3, 0x87, 0xF5, 0xC2, 0xE7);
					drawDynamicText(g_dynBuf, 20.0f, 105.0f, 1.0f, col, font, C2D_AlignLeft, "Add Clickers: A\nAdd Clicker Multi: B\nAdd CPC: X\nMult values by 2: Y\nFast Click: R");
				} else {
					drawGradientRect(10, 100, 320, 120, 5, C2D_Color32(0x18, 0x18, 0x28, 0xEE), 0xFA, 0xB3, 0x87, 0xF5, 0xC2, 0xE7);
					drawDynamicText(g_dynBuf, 20.0f, 105.0f, 1.0f, col, font, C2D_AlignLeft, "Misc:\nBuy Cooldown: %i\nBuy Repeat Delay: %i", buytime, buyRepeatDelay);
				}
			}
		}*/

		//bottom
		C2D_SceneBegin(bottom);
		C2D_TargetClear(bottom, C2D_Color32(0x1E, 0x1E, 0x2E, 0xFF));

		//draw the buy buttons
		//bord: C2D_Color32(0x58, 0x5B, 0x70, 0xFF);
		//fill: C2D_Color32(0x18, 0x18, 0x25, 0xFF);
		drawGradientRect(5 , 40, 150, 195, 5, C2D_Color32(0x18, 0x18, 0x25, 0xEE), 0xF5, 0xC2, 0xE7, 0xFA, 0xB3, 0x87, 0xFF);
		drawGradientRect(165, 40, 150, 195, 5, C2D_Color32(0x18, 0x18, 0x25, 0xEE), 0xFA, 0xB3, 0x87, 0xF5, 0xC2, 0xE7, 0xFF);

		if (DEBUG) {
			drawDynamicText(g_dynBuf, 5.0f, 5.0f, 0.7f, col, font, C2D_AlignLeft, "Frame: %i", frame);
			drawDynamicText(g_dynBuf, 315.0f, 5.0f, 0.7f, col, font, C2D_AlignRight, "DEBUG ON");
		} else {
			drawDynamicText(g_dynBuf, 5.0f, 5.0f, 0.7f, col, font, C2D_AlignLeft, "Frame: %i", frame);
		}

		//draw the buy buttons' text
		switch (buyscreen) {
			case 1:
				drawDynamicText(g_dynBuf, 160.0f, 5.0f, 1.0f, col, font, C2D_AlignCenter, "Shop p.1");
				drawDynamicText(g_dynBuf, 80.0f, 50.0f, 0.9f, col, font, C2D_AlignCenter, "CPC Upgrade\n+1 CPC\nPrice: %llu\nPrice+: 5\nOwned: %llu", clickUpPrice, clickUpOwn);
				drawDynamicText(g_dynBuf, 240.0f, 50.0f, 0.9f, col, font, C2D_AlignCenter, "CPC Mk.2\n+10 CPC\nPrice: %llu\nPrice+: 100\nOwned: %llu", clickm2price, clickm2own);
				break;
			case 2:
				drawDynamicText(g_dynBuf, 160.0f, 5.0f, 1.0f, col, font, C2D_AlignCenter, "Shop p.2");
				drawDynamicText(g_dynBuf, 80.0f, 50.0f, 0.9f, col, font, C2D_AlignCenter, "Clicker+\nEach clicker\ngets +1 CPS\nPrice: %llu\nPrice+: %llu\nOwned: %llu", clickerUprice, clickerUprice/10, clickerUown-1);
				drawDynamicText(g_dynBuf, 240.0f, 50.0f, 0.9f, col, font, C2D_AlignCenter, "Clicker\n+%llu CPS\nPrice: %llu\nPrice+: 25\nOwned: %llu", clickerUown, clickerprice, clickerown);
				break;
			case 3:
				drawDynamicText(g_dynBuf, 160.0f, 5.0f, 1.0f, col, font, C2D_AlignCenter, "Shop p.3");
				drawDynamicText(g_dynBuf, 80.0f, 50.0f, 0.9f, col, font, C2D_AlignCenter, "shhh");
				drawDynamicText(g_dynBuf, 240.0f, 50.0f, 0.9f, col, font, C2D_AlignCenter, "it's a secret");
				break;
		}

		C3D_FrameEnd(0);

		//Set keys old values for the next frame
		kDownOld = kDown;
		kHeldOld = kHeld;
		kUpOld = kUp;

		//Wait for VBlank
		gspWaitForVBlank();
	}

	// Free the VBO
	linearFree(vbo_data);

	// Free the shader program
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	cfguExit();
	gfxExit();
	romfsExit();
	return 0;
}