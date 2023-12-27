CXX = g++
CXXFLAGS = -Wall -g -I C:/Users/mashi/Desktop/VSCode/c++/GnosiaSolver

EXE = main
OBJS = main.o ConstraintVertex.o VariableVertex.o Vertex.o

VERTEX_LOC = src/graphImplementation/vertices

OBJS_DIR = .objs

# TESTS
TESTFLAGS = C:/msys64/ucrt64/lib/libboost_unit_test_framework-mt.a

TEST_GRAPH = testGraph
TEST_GRAPH_OBJS = testGraph.o

TEST_OBJS_DIR = test/.objs



#####################
# .PHONY Targets
.PHONY: all allTest clean exeClean testClean
# all objects
all: $(EXE)
# all tests
allTest: $(TEST_GRAPH)
# clean all object files and execute files
clean:
	exeClean testClean
# clean all object files & execute files around the project
exeClean:
	-@rm -rf $(OBJS_DIR)/*.o all
# clean all object files & execute files around testing
testClean:
	-@rm -rf $(TEST_OBJS_DIR)/*.o allTest

#####################
# Creates all directories specified along the OBJS_DIR path
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
# Same for test objects
$(TEST_OBJS_DIR):
	@mkdir -p $(TEST_OBJS_DIR)



#####################
# EXE
$(EXE):
	$(CXX) $(CXXFLAGS) -o $@ $^
# EXE dependencies
# essentially every .o in OBJS, but now their path point inside OBJS_DIR
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



#####################
# TEST_GRAPH
$(TEST_GRAPH):
	$(CXX) $(CXXFLAGS) -o $@ $^ $(TESTFLAGS)
# TEST_GRAPH dependencies
# essentially every .o in TEST_OBJS_DIR, but now their path point inside TEST_OBJS_DIR
$(TEST_GRAPH): $(patsubst %.o, $(TEST_OBJS_DIR)/%.o, $(TEST_GRAPH_OBJS))

#####################
# .o files for testGraph.cpp
$(TEST_OBJS_DIR)/testGraph.o: test/graphImplementation/testGraph.cpp | $(TEST_OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<