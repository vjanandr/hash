CC=g++
#CFLAGS=-Wall -g -DDEBUG
CFLAGS=-Wall -g
ODIR=obj
BDIR=./bin/
SOURCES=hashMap.cpp hashUtil.cpp hashMapOpen.cpp logger.cpp hashMapClosed.cpp
_OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS=$(patsubst %,$(ODIR)/%,$(_OBJECTS))

all: mkdir hashmod wcOpen wcOpenCRC wcClosedLinearCRC wcClosedQuadCRC wcClosedDoubleCRC



clean:
	rm -rf obj
	rm -rf bin

mkdir:
	@echo "Making obj dir"
	mkdir -p obj
	mkdir -p bin


$(ODIR)/%.o: %.cpp
	@echo "Target $@"
	$(CC) -c -o $@ $< $(CFLAGS)

hashmod: $(OBJECTS) hashModMain.cpp
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

wcOpen: $(OBJECTS) wordCountOpenMain.cpp
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

wcOpenCRC: $(OBJECTS) wordCountOpenCRCMain.cpp
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

wcClosedLinearCRC: $(OBJECTS) wcClosedLinearCRCMain.cpp
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

wcClosedDoubleCRC: $(OBJECTS) wcClosedDoubleCRCMain.cpp
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

wcClosedQuadCRC: $(OBJECTS) wcClosedQuadCRCMain.cpp
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)
