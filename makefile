CXX = g++
CXXFLAGS = -Wall -g -I C:/Users/mashi/Desktop/VSCode/c++/GnosiaSolver


EXE = main
OBJS = main.o ConstraintVertex.o VariableVertex.o Vertex.o

VERTEX_LOC = src/graphImplementation/vertices

OBJS_DIR = .objs

#####################
# .PHONY Targets
# all objects
all: $(EXE)
# clean all object files and execute files
clean:
	-@rm -rf $(OBJS_DIR)/*.o $(EXE)

#####################
# Creates all directories specified along the OBJS_DIR path
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

#####################
# EXE
$(EXE):
	$(CXX) $(CXXFLAGS) -o $@ $^
# EXE dependencies
# essentially every .o in OBJS, except their path point inside OBJS_DIR
$(EXE): $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJS))

$(OBJS_DIR)/main.o: main.cpp $(VERTEX_LOC)/VariableVertex.h | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

#####################
# .o files for Vertices
$(OBJS_DIR)/ConstraintVertex.o: $(VERTEX_LOC)/ConstraintVertex.cpp $(VERTEX_LOC)/ConstraintVertex.h $(VERTEX_LOC)/VariableVertex.h $(VERTEX_LOC)/Vertex.h | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJS_DIR)/VariableVertex.o: $(VERTEX_LOC)/VariableVertex.cpp $(VERTEX_LOC)/VariableVertex.h $(VERTEX_LOC)/Vertex.h | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJS_DIR)/Vertex.o: $(VERTEX_LOC)/Vertex.cpp $(VERTEX_LOC)/Vertex.h | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: all clean