TARGET := main
EXECUTABLE := termproject

OBJ_FILES := \
	main.o \
	keyboard.o \
	shader.o

CXX := clang++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wpedantic `pkg-config --cflags glfw3 glew` 
LINKLIBRARIES := `pkg-config --libs glfw3 glew` -framework OpenGL

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(EXECUTABLE) $(LINKLIBRARIES)

clean:
	rm -rf *.o *~ $(EXECUTABLE)

depend:
	$(CXX) $(CXXFLAGS) -MM *.cpp

shader.o: shader.hpp shader.cpp
keyboard.o: keyboard.cpp keyboard.hpp
main.o: main.cpp keyboard.hpp shader.hpp
