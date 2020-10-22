CXX=g++
CXXFLAGS=
LDFLAGS=-lSDL -lSDL_gfx
INC=
LIB=

EXEC=AsciiSpaceDestroyer
SRC=$(shell find . -name '*.cpp') 
OBJ=$(SRC:.cpp=.o)

$(EXEC): $(OBJ)
	@mkdir -p bin
	$(CXX) -o bin/$@ $^ $(LDFLAGS) $(LIB)

#TODO add .h in case of global variables
%.o : %.cpp 
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INC)
	
clean:
	rm -rf $(OBJ)

distclean: clean
	rm -rf $(EXEC)

exec: $(EXEC)
	./bin/$(EXEC)
