VPATH = src/graphImplementation:\
        src/graphImplementation/vertices:\
		src/graphImplementation/edges:\
	    test/graphImplementation:\
		test/graphImplementation/edges:\
		test/graphImplementation/vertices:\
		$(OBJS_DIR):\
		$(TEST_OBJS_DIR):

CXX = g++
CXXFLAGS = -Wall -g -I C:/Users/mashi/Desktop/VSCode/c++/GnosiaSolver

ALL_EXE = $(EXE)
ALL_TEST = $(TEST_GRAPH_IMPLEMENTATION)

EXE  = main
OBJS = main.o ConstraintVertex.o VariableVertex.o Vertex.o

OBJS_DIR = .objs

# TESTS
BOOST_PATH = C:/msys64/ucrt64/lib/libboost_unit_test_framework-mt.dll.a

# TEST GRAPH IMPLEMENTATION
TEST_GRAPH_IMPLEMENTATION = testGraphImplementation
# essentially every .o in TEST_OBJS_DIR, but now their path point inside OBJS_DIR or TEST_OBJS_DIR
T_GRAPH_IMPL_NON_TEST_OBJS = Graph.o VariableVertex.o Vertex.o ConstraintVertex.o
T_GRAPH_IMPL_TEST_OBJS     = testConstraintVertex.o testGraph.o testVariableVertex.o testVertex.o

TEST_GRAPH_IMPLEMENTATION_OBJS = $(T_GRAPH_IMPL_NON_TEST_OBJS) $(T_GRAPH_IMPL_TEST_OBJS)

TEST_OBJS_DIR = test/.objs



#####################
# .PHONY Targets
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

.PHONY: all allTest runTest clean exeClean testClean

#####################
# Creates directories specified along the OBJS_DIR or TEST_OBJS_DIR paths
$(OBJS_DIR) $(TEST_OBJS_DIR):
	@mkdir -p $@


#####################
# EXE
$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(patsubst %.o, $(OBJS_DIR)/%.o, $(^F))

main.o: main.cpp VariableVertex.h | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $(OBJS_DIR)/$@ -c $<

#####################
# .o files for Vertices
ConstraintVertex.o: ConstraintVertex.cpp ConstraintVertex.h VariableVertex.h Vertex.h | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $(OBJS_DIR)/$@ -c $<

VariableVertex.o: VariableVertex.cpp VariableVertex.h Vertex.h | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $(OBJS_DIR)/$@ -c $<

Vertex.o: Vertex.cpp Vertex.h | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $(OBJS_DIR)/$@ -c $<

#####################
# .o files for Graph
Graph.o: Graph.cpp Graph.h Edge.h Vertex.h | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $(OBJS_DIR)/$@ -c $<



#####################
# Test objects
$(ALL_TEST):
	$(CXX) $(CXXFLAGS) -o $@ \
    $(patsubst %.o, $(OBJS_DIR)/%.o,      $(filter-out test%, $(^F))) \
    $(patsubst %.o, $(TEST_OBJS_DIR)/%.o, $(filter     test%, $(^F))) \
    $(BOOST_PATH)

# TEST_GRAPH_IMPLEMENTATION dependencies
$(TEST_GRAPH_IMPLEMENTATION): $(TEST_GRAPH_IMPLEMENTATION_OBJS)

#####################
# .o files for test .cpp files
testEdge.o testConstraintVertex.o testVariableVertex.o testVertex.o: %.o : %.cpp | $(TEST_OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_OBJS_DIR)/$@ -c $<

testGraph.o: testGraph.cpp Graph.h ConstraintVertex.h VariableVertex.h Vertex.h | $(TEST_OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_OBJS_DIR)/$@ -c $<