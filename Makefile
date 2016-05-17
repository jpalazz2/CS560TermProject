TARGET := main
EXECUTABLE := termproject

OBJ_FILES := \
	main.o \
	input.o \
	shader.o \
	model.o

CXX := clang++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wpedantic `pkg-config --cflags glfw3 glew` -I/usr/local/include
LINKLIBRARIES := `pkg-config --libs glfw3 glew` -framework OpenGL

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(EXECUTABLE) $(LINKLIBRARIES)

clean:
	rm -rf *.o *~ $(EXECUTABLE)

depend:
	$(CXX) $(CXXFLAGS) -MM *.cpp

shader.o: shader.hpp shader.cpp
input.o: input.cpp input.hpp
model.o: model.hpp model.cpp
main.o: main.cpp input.hpp shader.hpp model.hpp
