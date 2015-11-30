CC=g++
CFLAGS=-Wall -g
ODIR=obj
SOURCES=hashMap.cpp hashUtil.cpp hashMapOpen.cpp logger.cpp
_OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS=$(patsubst %,$(ODIR)/%,$(_OBJECTS))

all: mkdir hashmod

clean:
	rm -rf obj
	rm -rf hashmod

mkdir:
	@echo "Making obj dir"
	mkdir -p obj


$(ODIR)/%.o: %.cpp
	@echo "Target $@"
	$(CC) -c -o $@ $< $(CFLAGS)

hashmod: $(OBJECTS) hashModMain.cpp
	$(CC) -o $@ $^ $(CFLAGS)
