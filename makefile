
# framework information for older version of MACOS
#INCLUDES = -Iinclude -F/System/Library/Frameworks -framework OpenGL -framework GLUT -lm

# frameworks for newer MACOS, where include files are moved 
#INCLUDES = -Iinclude -F/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/ -framework OpenGL -framework GLUT -lm -Wno-deprecated-declarations

# WSL / Linux 
INCLUDES = -Iinclude -lm -lglut -lGLU -lGL
CC=gcc
OBJ = src/a1.o src/playerController.o src/projectile.o src/world.o src/graphics.o src/visible.o src/list.o
EXE = a1

a1: $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(INCLUDES)

%.o : %.c
	$(CC) -c $*.c -o $*.o $(INCLUDES)

clean:
	rm $(EXE) *.o src/*.o

run:
	./$(EXE)

mac:
	gcc src/a1.c src/playerController.c src/world.c src/graphics.c src/visible.c src/list.c -o $(EXE) $(INCLUDES)

install:
	sudo apt-get install freeglut3-dev

setup:
	export DISPLAY=:0
