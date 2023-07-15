
EXENAME := ping.exe
PKGS=
CFLAGS = -g -O0 -Wall  $(shell pkg-config --cflags $(PKGS) ) -I. -I./include -std=c99 -D _WIN32_IE=0x0500 -D WINVER=0x0500
LIBS = -static $(shell pkg-config --libs $(PKGS) ) -lm -lcomctl32 -lws2_32 -lwinmm

CSRCSCAN := $(wildcard ./*.c) 
HSRCSCAN := $(wildcard ./*.h)
CSRCS := $(filter %.c,$(CSRCSCAN))
objects := $(patsubst %.c,obj/%.o,$(CSRCS))

OBJSCAN_PRE1 := $(wildcard ./obj/**/*.o) $(wildcard ./obj/*.o) $(wildcard ./obj/**/**/*.o)
OBJSCAN := $(EXENAME) $(OBJSCAN_PRE1)

all:	$(objects) 
	$(foreach objFile,$(HSRCSCAN),$(gcc -E $(objFile)))
	gcc -o $(EXENAME) $(objects) $(LIBS)

# Build the resources
# obj/Resource.o: res/Resource.rc res/Application.manifest res/Application.ico include/Resource.h | obj
# 	windres -I./include -I./res -i "$<" -o "$@"

$(objects):	obj/%.o: %.c
	$(shell sh.exe -c "mkdir -p ./$(dir $@)  2> /dev/null")
	gcc -g -O0 -Wall $(CFLAGS) -c $< -o $@

clean:
#$(shell cmd.exe /c "echo $(CSRCS) > test.txt")
#	$(shell cmd.exe /c "echo $(OBJSCAN_PRE1) > test2.txt")
#	$(shell cmd.exe /c "echo $(HSRCSCAN) > test3.txt")
	$(foreach objFile,$(OBJSCAN),$(shell sh.exe -c "rm $(objFile)  2> /dev/null"))
