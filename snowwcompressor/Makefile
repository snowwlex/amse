CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		 main.o compressor.o filestructure.o bitfile.o huffy/huffy.o huffy/hbtree.o lzw/lzw.o ppm/ppm.o ppm/rangecoder.o ppm/model.o

LIBS =

TARGET =	compressX

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
