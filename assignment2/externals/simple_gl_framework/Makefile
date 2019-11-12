CXX = c++
CXXFLAGS = -I. -DLINUX -std=c++0x
LDDFLAGS = -lglut -lGL -lGLU

SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

TARGET = example

default: $(TARGET)
	echo built $(TARGET)


$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDDFLAGS)  -o $(TARGET)


%: %.cpp
	$(CXX) $< $(CXXFLAGS) -o $@

clean:
	rm -f $(OBJECTS)


realclean:
	rm -f $(TARGET) *~

