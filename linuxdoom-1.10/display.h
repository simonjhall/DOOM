/*
 * display.h
 *
 *  Created on: 6 Apr 2020
 *      Author: simon
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

#ifndef __riscv
#include <pthread.h>
#include <SDL2/SDL.h>
#ifdef __linux__
#include <linux/input.h>
#endif
#endif

class PhysicalMemoryRegion
{
public:
	virtual void Read(void *, uint64_t offset, uint64_t size) = 0;
};

template <uint64_t Width, uint64_t Height>
class PalettisedFramebuffer : public PhysicalMemoryRegion
{
public:
	PalettisedFramebuffer(void)
	{
	};

	~PalettisedFramebuffer()
	{
	}

	virtual void Read(void *p, uint64_t offset, uint64_t size)
	{
		assert(m_pData);
		
		offset = offset >> 1;
		assert(offset < Width * Height);

		//palette data
		unsigned char pix = m_pData[offset];

		//8bpp
		unsigned char r = m_palette[pix * 3];
		unsigned char g = m_palette[pix * 3 + 1];
		unsigned char b = m_palette[pix * 3 + 2];

		//make 555
		r = r >> 3;
		g = g >> 3;
		b = b >> 3;

		unsigned short inflated_pix = b | (g << 5) | (r << 10);

		memcpy(p, &inflated_pix, 2);
	}

	void SetData(unsigned char *p)
	{
		m_pData = p;
	}

	void UploadPalette(unsigned char *p)
	{
		memcpy(m_palette, p, 256 * 3);
	}

private:
	unsigned char *m_pData;

	unsigned char m_palette[256 * 3];
};

#ifndef __riscv
class Display
{
public:
	Display(unsigned int width, unsigned int height, PhysicalMemoryRegion *pMemory, unsigned int offset);

	void Init(void);
	void Update(void);
	void GetEvent(bool &rHaveKeyEvent, unsigned char &rCode, bool &rDown, bool &rHaveMouseEvent, int &rRelX, int &rRelY, bool &rLeftDown, bool &rRightDown);
	void InitKeycodes(void);

private:
	SDL_Window *m_pWindow;
	SDL_Renderer *m_pRenderer;
	SDL_Texture *m_pTexture;

	unsigned int m_width, m_height;
	unsigned int m_offset;

	PhysicalMemoryRegion *m_pMemory;

	unsigned int keycodes[SDL_SCANCODE_APP2];
};
#endif



#endif /* DISPLAY_H_ */
