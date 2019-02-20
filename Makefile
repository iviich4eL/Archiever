CXX = g++
CXXFLAGS = -Wall -pedantic-errors -std=c++11
OBJECTS = main.o MTFAlgorithm.o HuffmanAlgorithm.o BWTAlgorithm.o BufferBlock.o
SRC = src
PROJECT_NAME = Archiever

# make DEBUG=yes -- команда для запуска Debug ветки
ifeq ($(DEBUG),yes)
	CXXFLAGS += -g
endif

all: $(PROJECT_NAME) 

# Если команда порождена неявным правилом, 
# макрос $< равен имени  файла, вызвавшего  действие.

%.o : src/MTFAlgorithm/%.cpp
	$(CXX) $(CXXFLAGS) -c $<
%.o : src/HuffmanAlgorithm/%.cpp
	$(CXX) $(CXXFLAGS) -c $<
%.o : src/BWTAlgorithm/%.cpp
	$(CXX) $(CXXFLAGS) -c $<
%.o : src/BufferBlock/%.cpp
	$(CXX) $(CXXFLAGS) -c $<
# %.o : src/%.cpp
# 	$(CXX) $(CXXFLAGS) -c $<
main.o: main.cpp 
	$(CXX) $(CXXFLAGS) -c main.cpp
$(PROJECT_NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(PROJECT_NAME)

clean : 
	rm $(OBJECTS)

# Цели для тестирования в docker
docker: main clean
main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o main
