TARGET := main
EXECUTABLE := termproject

SRC_FILES := \
	main.cpp \

OBJ_FILES := \
	main.o

CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wpedantic `pkg-config --cflags glfw3`
LINKLIBRARIES := `pkg-config --libs glfw3`

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(EXECUTABLE) $(LINKLIBRARIES)

clean:
	rm -rf *.o *~ $(EXECUTABLE)

depend:
	$(CXX) $(CXXFLAGS) -MM *.cpp

main.o: main.cpp 
