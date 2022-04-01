#include <assert.h>
#include "doomtype.h"
#include "doomdef.h"

#include "display.h"

#ifndef __riscv
static Display *g_pDisplay;
#endif
static PalettisedFramebuffer<SCREENWIDTH, SCREENHEIGHT> *g_pFb;

extern byte*				screens[5];	


extern "C" void I_ShutdownGraphics(void)
{

}



//
// I_StartFrame
//
extern "C" void I_StartFrame (void)
{

}

extern "C" void I_GetEvent(void)
{
#ifdef __riscv
#else
	bool haveKeyEvent;
	unsigned char code;
	bool down;
	bool haveMouseEvent;
	int relX, relY;
	bool leftDown, rightDown;

	g_pDisplay->GetEvent(haveKeyEvent, code, down, haveMouseEvent, relX, relY, leftDown, rightDown);
#endif
}

//
// I_StartTic
//
extern "C" void I_StartTic (void)
{
	I_GetEvent();
}


//
// I_UpdateNoBlit
//
extern "C" void I_UpdateNoBlit (void)
{
}

//
// I_FinishUpdate
//
extern "C" void I_FinishUpdate (void)
{
#ifdef __riscv
	for (unsigned int y = 0; y < SCREENHEIGHT; y++)
	{
		unsigned char *pSdlRow = ((unsigned char *)0x17c00000) + y * (640 * 2);

		for (unsigned int x = 0; x < SCREENWIDTH; x++)
		{
			unsigned short pix;
			g_pFb->Read(&pix, (y * SCREENWIDTH + x) * 2, 2);

			unsigned short *pDest = (unsigned short *)&pSdlRow[x * 2];
			*pDest = pix;
		}
	}
#else
	g_pDisplay->Update();
	I_GetEvent();
#endif
}


//
// I_ReadScreen
//
extern "C" void I_ReadScreen (byte* scr)
{
}

//
// I_SetPalette
//
extern "C" void I_SetPalette (byte* palette)
{
	g_pFb->UploadPalette(palette);
}




extern "C" void I_InitGraphics(void)
{
	g_pFb = new PalettisedFramebuffer<SCREENWIDTH, SCREENHEIGHT>;
	assert(g_pFb);

	assert(screens[0]);
	g_pFb->SetData(screens[0]);

#ifdef __riscv
#else
	g_pDisplay = new Display(SCREENWIDTH, SCREENHEIGHT, g_pFb, 0);
	assert(g_pDisplay);

	g_pDisplay->Init();
#endif
}

