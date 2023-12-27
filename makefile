CXX = g++
CXXFLAGS = -Wall -g -I C:/Users/mashi/Desktop/VSCode/c++/GnosiaSolver

ALL_EXE = $(EXE)
ALL_TEST = $(TEST_GRAPH_IMPLEMENTATION)

EXE  = main
OBJS = main.o ConstraintVertex.o VariableVertex.o Vertex.o

VERTEX_LOC = src/graphImplementation/vertices

OBJS_DIR = .objs

# TESTS
TESTFLAGS = C:/msys64/ucrt64/lib/libboost_unit_test_framework-mt.dll.a

# TEST GRAPH IMPLEMENTATION
TEST_GRAPH_IMPLEMENTATION = testGraphImplementation
TEST_GRAPH_IMPLEMENTATION_OBJS = testConstraintVertex.o testGraph.o testVariableVertex.o testVertex.o

TEST_VERTEX_LOC = test/graphImplementation/vertices

TEST_OBJS_DIR = test/.objs



#####################
# .PHONY Targets
.PHONY: all allTest runTest clean exeClean testClean
# all objects
all: $(ALL_EXE)
# all tests
test: $(ALL_TEST)
# run all tests
runTest: 
	@$(foreach file, $(ALL_TEST), ./$(file);)

# clean all object files and execute files
clean: exeClean testClean
# clean all object files & execute files around the project
exeClean:
	-@rm -rf $(OBJS_DIR)/*.o $(ALL_EXE)
# clean all object files & execute files around testing
testClean:
	-@rm -rf $(TEST_OBJS_DIR)/*.o $(ALL_TEST)

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
# Test objects
$(ALL_TEST):
	$(CXX) $(CXXFLAGS) -o $@ $^ $(TESTFLAGS)

# TEST_GRAPH_IMPLEMENTATION dependencies
# essentially every .o in TEST_OBJS_DIR, but now their path point inside TEST_OBJS_DIR
$(TEST_GRAPH_IMPLEMENTATION): $(patsubst %.o, $(TEST_OBJS_DIR)/%.o, $(TEST_GRAPH_IMPLEMENTATION_OBJS))

#####################
# .o files for test .cpp files
$(TEST_OBJS_DIR)/testGraph.o: test/graphImplementation/testGraph.cpp | $(TEST_OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(TEST_OBJS_DIR)/testConstraintVertex.o: $(TEST_VERTEX_LOC)/testConstraintVertex.cpp | $(TEST_OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(TEST_OBJS_DIR)/testVariableVertex.o: $(TEST_VERTEX_LOC)/testVariableVertex.cpp | $(TEST_OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(TEST_OBJS_DIR)/testVertex.o: $(TEST_VERTEX_LOC)/testVertex.cpp | $(TEST_OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<