CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	LDFLAGS = -lglut -lGLU
endif

STD = c++11
LIB = libc++

RM = /bin/rm -f 
all:main

main: main.o window.o parser.o output.o
	$(CC) $(CFLAGS) -stdlib=$(LIB) -o $@ main.o window.o parser.o output.o $(LDFLAGS) 

%.o: %.cpp
	$(CC) $(CFLAGS) -stdlib=$(LIB) -c $< -o $@

clean: 
	$(RM) *.o main
 


