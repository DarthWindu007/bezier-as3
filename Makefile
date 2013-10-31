CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin -O3
	LDFLAGS = -lglut -lGL -lGLU
endif
	
RM = /bin/rm -f 
all: clean main 
main:  vector.o main.o normal.o point.o
	$(CC) $(CFLAGS) -o as3 normal.o point.o vector.o main.o $(LDFLAGS) 
vector.o: vector.cpp
	$(CC) $(CFLAGS) -c vector.cpp -o vector.o
normal.o: normal.cpp
	$(CC) $(CFLAGS) -c normal.cpp -o normal.o
point.o: point.cpp
	$(CC) $(CFLAGS) -c point.cpp -o point.o
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o
#testing.o: testing.cpp
#	$(CC) -c $(CFLAGS) testing.cpp -o testing.o
clean: 
	$(RM) *.o as3
 


