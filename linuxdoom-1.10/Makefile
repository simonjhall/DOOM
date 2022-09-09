################################################################
#
# $Id:$
#
# $Log:$
#

PREFIX=/Users/simon_hall/riscv-linux32/bin/riscv32-unknown-linux-gnu-

CC=$(PREFIX)gcc
CXX=$(PREFIX)g++

# CFLAGS=-g -Wall -DNORMALUNIX -Wno-unused-const-variable -Werror=pointer-to-int-cast -Werror=int-to-pointer-cast
CFLAGS=-march=rv32im -mno-div -mabi=ilp32 -fno-pie --static -g -Wall -DNORMALUNIX -DLINUX -Wno-unused-const-variable -Wl,-Tscript_ram.lds -O3
# LDFLAGS=-L/usr/X11R6/lib
# LIBS=-lXext -lX11 -lnsl -lm
# LIBS=-lSDL2

# subdirectory for objects
O=linux

# not too sophisticated dependency
OBJS=				\
		$(O)/doomdef.o		\
		$(O)/doomstat.o		\
		$(O)/dstrings.o		\
		$(O)/i_system.o		\
		$(O)/i_sound.o		\
		$(O)/i_video_riscv.o		\
		$(O)/i_net.o			\
		$(O)/tables.o			\
		$(O)/f_finale.o		\
		$(O)/f_wipe.o 		\
		$(O)/d_main.o			\
		$(O)/d_net.o			\
		$(O)/d_items.o		\
		$(O)/g_game.o			\
		$(O)/m_menu.o			\
		$(O)/m_misc.o			\
		$(O)/m_argv.o  		\
		$(O)/m_bbox.o			\
		$(O)/m_fixed.o		\
		$(O)/m_swap.o			\
		$(O)/m_cheat.o		\
		$(O)/m_random.o		\
		$(O)/am_map.o			\
		$(O)/p_ceilng.o		\
		$(O)/p_doors.o		\
		$(O)/p_enemy.o		\
		$(O)/p_floor.o		\
		$(O)/p_inter.o		\
		$(O)/p_lights.o		\
		$(O)/p_map.o			\
		$(O)/p_maputl.o		\
		$(O)/p_plats.o		\
		$(O)/p_pspr.o			\
		$(O)/p_setup.o		\
		$(O)/p_sight.o		\
		$(O)/p_spec.o			\
		$(O)/p_switch.o		\
		$(O)/p_mobj.o			\
		$(O)/p_telept.o		\
		$(O)/p_tick.o			\
		$(O)/p_saveg.o		\
		$(O)/p_user.o			\
		$(O)/r_bsp.o			\
		$(O)/r_data.o			\
		$(O)/r_draw.o			\
		$(O)/r_main.o			\
		$(O)/r_plane.o		\
		$(O)/r_segs.o			\
		$(O)/r_sky.o			\
		$(O)/r_things.o		\
		$(O)/w_wad.o			\
		$(O)/wi_stuff.o		\
		$(O)/v_video.o		\
		$(O)/st_lib.o			\
		$(O)/st_stuff.o		\
		$(O)/hu_stuff.o		\
		$(O)/hu_lib.o			\
		$(O)/s_sound.o		\
		$(O)/z_zone.o			\
		$(O)/info.o				\
		$(O)/sounds.o \
		$(O)/display.o

all:	 $(O)/linuxxdoom $(O)/linuxxdoom.bin

clean:
	rm -f *.o *~ *.flc
	rm -f linux/*

$(O)/linuxxdoom:	$(OBJS) $(O)/i_main.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $(OBJS) $(O)/i_main.o \
	-o $(O)/linuxxdoom $(LIBS)

$(O)/linuxxdoom.bin: $(O)/linuxxdoom
	$(PREFIX)objcopy -O binary $^ $@

$(O)/linuxxdoom.header: $(O)/linuxxdoom $(O)/linuxxdoom.bin
	~/git/EmbeddedDebugger/args beefcafe `stat -f "%z" $(basename $@).bin` `$(PREFIX)readelf -h $(basename $@) |grep Entry| sed 's/^.*0x//g'` `crc32 $(basename $@).bin`
	mv header.bin $@

$(O)/%.o:	%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(O)/%.o:	%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

#############################################################
#
#############################################################
