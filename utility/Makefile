COMPILER 				:= 	g++
TARGET					:= testapp
LD_L_PATH 				:= -L/usr/lib/i386-linux-gnu
INCLUDES 				:= -I./src -I/usr/include/i386-linux-gnu -I/usr/include -I/usr/local/include



all: build

build:	
	$(COMPILER) -O3 $(INCLUDES) -fPIC -c src/Logger.cpp
	$(COMPILER) -O3 $(INCLUDES) -fPIC -c src/utility.c
	$(COMPILER) -O3 $(INCLUDES) -fPIC -c testapp.cpp
	$(COMPILER) -O3 -o $(TARGET) testapp.o Logger.o utility.o $(LD_L_PATH)
	strip $(TARGET)

clean:
	rm -rf *.o *.a
	rm $(TARGET)
	rm $(TARGET2)
