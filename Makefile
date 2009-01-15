TARGET = InsanerzShooter
OBJS = main.o

PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin

INCDIR = 
#                       vvvvvvvv
CFLAGS = -O2 -G0 -Wall -DPSPFW3X $(shell $(PSPBIN)/sdl-config --cflags | sed s/-Dmain=SDL_main//) 
#                       ^^^^^^^^

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti 
ASFLAGS = $(CFLAGS)

# vvvvvvvv
BUILD_PRX=1
PSP_FW_VERSION=303
# ^^^^^^^^

LIBS = -lSDL_ttf -lfreetype -lSDL_mixer -lvorbisidec -lSDL_image -lpng -lz -ljpeg $(shell $(PSPBIN)/sdl-config --libs | sed s/-lSDLmain//) -lpspdebug -lstdc++
LIBDIR =
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = InsanerzShooter

include $(PSPSDK)/lib/build.mak
