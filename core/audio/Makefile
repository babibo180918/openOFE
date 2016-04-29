COMPILER 				:= 	g++
TARGET					:= testapp
ROOT                    := $(shell pwd)
I_FFMPEG                := .
I_UTILITY               := /Users/nhanndt/Works/C_SOURCE/openOFE
I_CORE                  := /Users/nhanndt/Works/C_SOURCE/openOFE/core
INCLUDES 				:= -I$(ROOT) -I$(ROOT)/include -I/usr/include -I/usr/local/include -I$(I_FFMPEG) -I$(I_UTILITY) -I$(I_CORE)
L_FFMPEG                := .
LD_L_PATH 				:= -L/usr/lib -L/usr/local/lib -L$(L_FFMPEG)
LD_LIBS                 := -lavformat -lavcodec -lavutil -lswresample



all: build

build:	
	$(COMPILER) -O3 $(INCLUDES) -fPIC -c src/PCMDecoder.cpp
	$(COMPILER) -O3 $(INCLUDES) -fPIC -c $(I_UTILITY)/utility/utility.c
	$(COMPILER) -O3 $(INCLUDES) -fPIC -c $(I_UTILITY)/utility/Logger.cpp
	$(COMPILER) -O3 $(INCLUDES) -fPIC -c Audio_TestApp.cpp
	$(COMPILER) -O3 -o $(TARGET) Audio_TestApp.o PCMDecoder.o Logger.o utility.o $(LD_L_PATH) $(LD_LIBS)
	strip $(TARGET)

clean:
	rm -rf *.o *.a
	rm $(TARGET)
