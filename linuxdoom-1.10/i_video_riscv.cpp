#include <assert.h>
#include "doomtype.h"
#include "doomdef.h"

#ifndef __riscv
#include "display.h"

static Display *g_pDisplay;
static PalettisedFramebuffer<SCREENWIDTH, SCREENHEIGHT> *g_pFb;
#endif

byte*				screens[5];	


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
#ifndef __riscv
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
#ifndef __riscv
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
#ifndef __riscv
	g_pFb->UploadPalette(palette);
#endif
}




extern "C" void I_InitGraphics(void)
{
#ifndef __riscv
	g_pFb = new PalettisedFramebuffer<SCREENWIDTH, SCREENHEIGHT>;
	assert(g_pFb);

	assert(screens[0]);
	g_pFb->SetData(screens[0]);

	g_pDisplay = new Display(SCREENWIDTH, SCREENHEIGHT, g_pFb, 0);
	assert(g_pDisplay);

	g_pDisplay->Init();
#endif
}

