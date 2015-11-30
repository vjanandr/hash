CC=gcc
CFLAGS=-Wall
ODIR=obj
SOURCES=hashMap.cpp hashUtil.cpp hashOpen.cpp logger.cpp
_OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS=$(patsubst %,$(ODIR)/%,$(_OBJECTS))

all: mkdir hashmod


mkdir:
	echo "Making obj dir"
	mkdir -p obj


$(ODIR)/%.o: %.cpp
	echo "Target $@"
	$(CC) -c -o $@ $<

hashmod: $(OBJECTS) hashModMain.cpp
	$(CC) -o $@ $^
