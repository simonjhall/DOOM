/*
 * display.cpp
 *
 *  Created on: 6 Apr 2020
 *      Author: simon
 */

#include <assert.h>
#include "display.h"

bool first_init = false;

Display::Display(unsigned int width, unsigned int height, PhysicalMemoryRegion *pMemory, unsigned int offset)
		: m_width(width), m_height(height), m_offset(offset), m_pMemory(pMemory)
{
}

void Display::Init(void)
{
	SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
	SDL_Init(SDL_INIT_VIDEO);

	printf("window dims %d x %d\n", m_width, m_height);
	m_pWindow = SDL_CreateWindow("framebuffer", 0, 0, m_width * 2, m_height * 2, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	assert(m_pWindow);

	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
	assert(m_pRenderer);

	m_pTexture = SDL_CreateTexture(m_pRenderer,
					   SDL_PIXELFORMAT_RGB555,
					   SDL_TEXTUREACCESS_STREAMING,
					   m_width,
					   m_height);
	assert(m_pTexture);

//	assert(dynamic_cast<SmallRamRegion<4 * 1024 * 1024> *>(m_pMemory));

	InitKeycodes();
}

void Display::Update(void)
{
	clock_t start = clock();

	void *pPixels;
	int pitch;

	SDL_LockTexture(m_pTexture,
			NULL,      // NULL means the *whole texture* here.
			&pPixels,
			&pitch);

	assert(pPixels);

	for (unsigned int y = 0; y < m_height; y++)
	{
		unsigned char *pSdlRow = ((unsigned char *)pPixels) + y * pitch;

		for (unsigned int x = 0; x < m_width; x++)
		{
			unsigned short pix;
			m_pMemory->Read(&pix, (y * m_width + x) * 2, 2);

			unsigned short *pDest = (unsigned short *)&pSdlRow[x * 2];
			*pDest = pix;
		}
	}

	SDL_UnlockTexture(m_pTexture);

	SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, NULL);
	SDL_RenderPresent(m_pRenderer);

	clock_t finish = clock();
	float msElapsed = (float)(finish - start) / (CLOCKS_PER_SEC / 1000);

	char title[100];
	sprintf(title, "framebuffer: refresh took %.2f ms", msElapsed);
	SDL_SetWindowTitle(m_pWindow, title);
}

void Display::GetEvent(bool &rHaveKeyEvent, unsigned char &rCode, bool &rDown, bool &rHaveMouseEvent, int &rRelX, int &rRelY, bool &rLeftDown, bool &rRightDown)
{
	SDL_PumpEvents();

	SDL_Event events[1];

	rHaveKeyEvent = false;
	rHaveMouseEvent = false;

	while (SDL_PeepEvents(events, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT))
	{
		SDL_Event event;

		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				exit(0);
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				rHaveKeyEvent = true;
				rCode = keycodes[event.key.keysym.scancode];

				if (event.type == SDL_KEYUP)
					rDown = false;
				else
					rDown = true;

//					printf("key %d down %d\n", rCode, rDown);
				break;
			case SDL_MOUSEMOTION:
				rHaveMouseEvent = true;
				rRelX = event.motion.xrel;
				rRelY = event.motion.yrel;

				rLeftDown = (event.motion.state & SDL_BUTTON_LMASK) ? true : false;
				rRightDown = (event.motion.state & SDL_BUTTON_RMASK) ? true : false;

				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			{
				int x, y;
				unsigned int state;

				rHaveMouseEvent = true;
				rRelX = 0;
				rRelY = 0;

				state = SDL_GetMouseState(&x, &y);

				rLeftDown = (state & SDL_BUTTON_LMASK) ? true : false;
				rRightDown = (state & SDL_BUTTON_RMASK) ? true : false;

				break;
			}
			default:
				break;
			}
		}

		if (rHaveKeyEvent || rHaveMouseEvent)
			break;
	}
}

void Display::InitKeycodes(void)
{
	memset(keycodes, 0, sizeof(keycodes));

#ifdef __linux__
	keycodes[SDL_SCANCODE_A] = KEY_A;
	keycodes[SDL_SCANCODE_B] = KEY_B;
	keycodes[SDL_SCANCODE_C] = KEY_C;
	keycodes[SDL_SCANCODE_D] = KEY_D;
	keycodes[SDL_SCANCODE_E] = KEY_E;
	keycodes[SDL_SCANCODE_F] = KEY_F;
	keycodes[SDL_SCANCODE_G] = KEY_G;
	keycodes[SDL_SCANCODE_H] = KEY_H;
	keycodes[SDL_SCANCODE_I] = KEY_I;
	keycodes[SDL_SCANCODE_J] = KEY_J;
	keycodes[SDL_SCANCODE_K] = KEY_K;
	keycodes[SDL_SCANCODE_L] = KEY_L;
	keycodes[SDL_SCANCODE_M] = KEY_M;
	keycodes[SDL_SCANCODE_N] = KEY_N;
	keycodes[SDL_SCANCODE_O] = KEY_O;
	keycodes[SDL_SCANCODE_P] = KEY_P;
	keycodes[SDL_SCANCODE_Q] = KEY_Q;
	keycodes[SDL_SCANCODE_R] = KEY_R;
	keycodes[SDL_SCANCODE_S] = KEY_S;
	keycodes[SDL_SCANCODE_T] = KEY_T;
	keycodes[SDL_SCANCODE_U] = KEY_U;
	keycodes[SDL_SCANCODE_V] = KEY_V;
	keycodes[SDL_SCANCODE_W] = KEY_W;
	keycodes[SDL_SCANCODE_X] = KEY_X;
	keycodes[SDL_SCANCODE_Y] = KEY_Y;
	keycodes[SDL_SCANCODE_Z] = KEY_Z;

	keycodes[SDL_SCANCODE_0] = KEY_0;
	keycodes[SDL_SCANCODE_1] = KEY_1;
	keycodes[SDL_SCANCODE_2] = KEY_2;
	keycodes[SDL_SCANCODE_3] = KEY_3;
	keycodes[SDL_SCANCODE_4] = KEY_4;
	keycodes[SDL_SCANCODE_5] = KEY_5;
	keycodes[SDL_SCANCODE_6] = KEY_6;
	keycodes[SDL_SCANCODE_7] = KEY_7;
	keycodes[SDL_SCANCODE_8] = KEY_8;
	keycodes[SDL_SCANCODE_9] = KEY_9;

	keycodes[SDL_SCANCODE_RETURN] = KEY_ENTER;
	keycodes[SDL_SCANCODE_ESCAPE] = KEY_ESC;
	keycodes[SDL_SCANCODE_BACKSPACE] = KEY_BACKSPACE;
	keycodes[SDL_SCANCODE_TAB] = KEY_TAB;
	keycodes[SDL_SCANCODE_SPACE] = KEY_SPACE;
	keycodes[SDL_SCANCODE_MINUS] = KEY_MINUS;
	keycodes[SDL_SCANCODE_EQUALS] = KEY_EQUAL;
	keycodes[SDL_SCANCODE_LEFTBRACKET] = KEY_LEFTBRACE;
	keycodes[SDL_SCANCODE_RIGHTBRACKET] = KEY_RIGHTBRACE;
	keycodes[SDL_SCANCODE_BACKSLASH] = KEY_BACKSLASH;

	keycodes[SDL_SCANCODE_SEMICOLON] = KEY_SEMICOLON;
	keycodes[SDL_SCANCODE_APOSTROPHE] = KEY_APOSTROPHE;
	keycodes[SDL_SCANCODE_GRAVE] = KEY_GRAVE;
	keycodes[SDL_SCANCODE_COMMA] = KEY_COMMA;
	keycodes[SDL_SCANCODE_PERIOD] = KEY_DOT;
	keycodes[SDL_SCANCODE_SLASH] = KEY_SLASH;
	keycodes[SDL_SCANCODE_CAPSLOCK] = KEY_CAPSLOCK;

	keycodes[SDL_SCANCODE_INSERT] = KEY_INSERT;
	keycodes[SDL_SCANCODE_HOME] = KEY_HOME;
	keycodes[SDL_SCANCODE_PAGEUP] = KEY_PAGEUP;
	keycodes[SDL_SCANCODE_DELETE] = KEY_DELETE;
	keycodes[SDL_SCANCODE_END] = KEY_END;
	keycodes[SDL_SCANCODE_PAGEDOWN] = KEY_PAGEDOWN;
	keycodes[SDL_SCANCODE_RIGHT] = KEY_RIGHT;
	keycodes[SDL_SCANCODE_LEFT] = KEY_LEFT;
	keycodes[SDL_SCANCODE_DOWN] = KEY_DOWN;
	keycodes[SDL_SCANCODE_UP] = KEY_UP;

	keycodes[SDL_SCANCODE_LCTRL] = KEY_LEFTCTRL;
	keycodes[SDL_SCANCODE_LSHIFT] = KEY_LEFTSHIFT;
	keycodes[SDL_SCANCODE_LALT] = KEY_LEFTALT;
	keycodes[SDL_SCANCODE_RCTRL] = KEY_RIGHTCTRL;
	keycodes[SDL_SCANCODE_RSHIFT] = KEY_RIGHTSHIFT;
	keycodes[SDL_SCANCODE_RALT] = KEY_RIGHTALT;
#endif
}
