
# framework information for older version of MACOS
#INCLUDES = -F/System/Library/Frameworks -framework OpenGL -framework GLUT -lm

# frameworks for newer MACOS, where include files are moved 
#INCLUDES = -F/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/ -framework OpenGL -framework GLUT -lm -Wno-deprecated-declarations

# WSL / Linux 
INCLUDES = -Iinclude -lm -lglut -lGLU -lGL
CC=gcc
OBJ = src/a1.o src/utils.o src/graphics.o src/visible.o 
EXE = a1

a1: $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(INCLUDES)

%.o : %.c
	$(CC) -c $*.c -o $*.o $(INCLUDES)

clean:
	rm $(EXE) *.o src/*.o

