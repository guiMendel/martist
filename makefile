CC = g++

C_FLAGS = -std=c++17 -Wall -Wextra

INCLUDE_DIR = include
OBJECT_DIR = obj
SOURCE_DIR = src

_DEPS = ExpressionFactory.hpp ExpressionTree.hpp SpecReader.hpp 
DEPS = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS)) Martist.hpp

_OBJ = ExpressionFactory.o ExpressionTree.o SpecReader.o 
OBJ = $(patsubst %,$(OBJECT_DIR)/%,$(_OBJ)) 

OUTER_OBJ = Martist.o main.o

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(DEPS)
	mkdir -p obj; $(CC) -c -o $@ $< $(C_FLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(C_FLAGS)

martist: $(OUTER_OBJ) $(OBJ)
	$(CC) -o $@ $^ $(C_FLAGS)

.PHONY: clean

clean:
	rm -f -r $(OBJECT_DIR) *~ core $(INCDIR)/*~
	rm -f *.o *~ core $(INCDIR)/*~