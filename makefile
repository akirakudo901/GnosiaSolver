# GENERAL CONVENTIONS: AS FOUND FROM HERE[https://www.gnu.org/software/make/manual/html_node/Makefile-Basics.html]
# SHELL = /bin/sh

# Reset and set suffixes
.SUFFIXES:
.SUFFIXES: .cpp .h .o .d

# Set up commands INSTALL, INSTALL_PROGRAM, INSTALL_DATA;
# SINCE I DON'T UNDERSTAND, MIGHT BE BETTER NOT DEFINING THEM.
# INSTALL
# INSTALL_PROGRAM = $(INSTALL)
# INSTALL_DATA = ${INSTALL} -m 644

#####################
# matching prerequisites are searched in these directories before VPATH
vpath test%.o $(TEST_OBJS_DIR)
vpath %.o $(OBJS_DIR)
vpath %.d $(DEPENDENCY_MAKEFILE_DIR)

# after vpath is searched, we search through VPATH
VPATH = src/cspSolver:\
		src/cspSolver/frontier:\
		src/graphImplementation:\
        src/graphImplementation/vertices:\
		src/graphImplementation/edges:\
		test/cspSolver:\
		test/cspSolver/frontier:\
		test/graphImplementation:\
		test/graphImplementation/edges:\
		test/graphImplementation/vertices:\
		$(OBJS_DIR):\
		$(TEST_OBJS_DIR):\
		$(DEPENDENCY_MAKEFILE_DIR):\

#####################
# Compiler & Linker flags
CXX = g++
CXXFLAGS = -Wall -g -I C:/Users/mashi/Desktop/VSCode/c++/GnosiaSolver

LD = g++
LDFLAGS = -Wall -g -I C:/Users/mashi/Desktop/VSCode/c++/GnosiaSolver

#####################
# Special directories and paths
OBJS_DIR = .objs
TEST_OBJS_DIR = test/.objs

BOOST_PATH = C:/msys64/ucrt64/lib/libboost_unit_test_framework-mt.dll.a
# related to dependencies of makefile as obtained via -MM commands
DEPENDENCY_MAKEFILE_DIR = dependencies

.INCLUDE_DIRS += $(DEPENDENCY_MAKEFILE_DIR)


#####################
# all objects - this has to be the primary "make" command so put it here
all: non-test test




##########################
#  LIKELY MODIFY BELOW   #
##########################


#####################
# NON-TEST EXECUTABLES
ALL_EXE = $(MAIN)
ALL_TEST = $(TEST_GRAPH_IMPLEMENTATION) $(TEST_CSPSOLVER_IMPLEMENTATION)

MAIN      = main
MAIN_OBJS = main.o ConstraintVertex.o CSPGraph.o CSPGraphCreator.o Frontier.o Graph.o VariableVertex.o Vertex.o



# TESTS
# TEST GRAPH IMPLEMENTATION
TEST_GRAPH_IMPLEMENTATION = testGraphImplementation
# essentially relevant .o in TEST_OBJS_DIR, but now their path point inside OBJS_DIR or TEST_OBJS_DIR
T_GRAPH_IMPL_NON_TEST_OBJS = Graph.o VariableVertex.o Vertex.o ConstraintVertex.o
T_GRAPH_IMPL_TEST_OBJS     = testConstraintVertex.o testGraph.o testVariableVertex.o testVertex.o

TEST_GRAPH_IMPLEMENTATION_OBJS = $(T_GRAPH_IMPL_NON_TEST_OBJS) $(T_GRAPH_IMPL_TEST_OBJS)

# TEST CSP SOLVER IMPLEMENTATION
TEST_CSPSOLVER_IMPLEMENTATION = testCSPSolverImplementation
# essentially relevant .o in TEST_OBJS_DIR, but now their path point inside OBJS_DIR or TEST_OBJS_DIR
T_CSPSOLVER_IMPL_NON_TEST_OBJS = CSPGraph.o CSPGraphCreator.o CSPSolver.o ConstraintVertex.o Frontier.o Graph.o VariableVertex.o Vertex.o
T_CSPSOLVER_IMPL_TEST_OBJS     = testCSPGraph.o testCSPGraphCreator.o testCSPSolver.o testFrontier.o

TEST_CSPSOLVER_IMPLEMENTATION_OBJS = $(T_CSPSOLVER_IMPL_NON_TEST_OBJS) $(T_CSPSOLVER_IMPL_TEST_OBJS)



#####################
# SOURCE FILES
SOURCES = $(NON_TEST_SOURCES) $(TEST_SOURCES)
NON_TEST_SOURCES = ConstraintVertex.cpp CSPGraph.cpp CSPGraphCreator.cpp CSPSolver.cpp Frontier.cpp Graph.cpp main.cpp VariableVertex.cpp Vertex.cpp 
TEST_SOURCES = testConstraintVertex.cpp testCSPGraph.cpp testCSPGraphCreator.cpp testCSPSolver.cpp testEdge.cpp testFrontier.cpp testGraph.cpp testVariableVertex.cpp testVertex.cpp

#####################
# Non-test object dependencies
$(MAIN): $(MAIN_OBJS)

# Test object dependencies
$(TEST_GRAPH_IMPLEMENTATION): $(TEST_GRAPH_IMPLEMENTATION_OBJS)
$(TEST_CSPSOLVER_IMPLEMENTATION): $(TEST_CSPSOLVER_IMPLEMENTATION_OBJS)

#####################
# .PHONY Targets
# all non-tests
non-test: $(ALL_EXE)
# all tests
test: $(ALL_TEST)
# run all tests
runTest: 
	@$(foreach file, $(ALL_TEST), ./$(file);)

# clean all object files and execute files
clean: exeClean testClean makeClean
# clean all object files & execute files around the project
exeClean:
	-@rm -rf $(OBJS_DIR)/*.o $(ALL_EXE)
# clean all object files & execute files around testing
testClean:
	-@rm -rf $(TEST_OBJS_DIR)/*.o $(ALL_TEST)
# clean all makefiles within $(DEPENDENCY_MAKEFILE_DIR)
makeClean:
	-@rm -rf $(DEPENDENCY_MAKEFILE_DIR)/*.d

.PHONY: all non-test test runTest clean exeClean testClean makeClean






##########################################
#  NO NEED FOR ANY MODIFICATIONS BELOW   #
##########################################

#####################
# Creates directories specified along the given paths
# mkdir -p IS INTERESTINGLY ACTIVELY DISCOURAGED DUE TO NON-SUPPORT IN SOME ENVIRONMENTS
# AS OF HERE[https://www.gnu.org/software/make/manual/html_node/Utilities-in-Makefiles.html]
# BUT I WILL BE LAZY HERE
$(OBJS_DIR) $(TEST_OBJS_DIR) $(DEPENDENCY_MAKEFILE_DIR):
	@mkdir -p $@

#####################
# Rule to import all makefiles for all source files
include $(patsubst %.d, $(DEPENDENCY_MAKEFILE_DIR)/%.d, $(SOURCES:.cpp=.d))

# Pattern rule for creating makefiles corresponding to each source file (.cpp)
# taken from here[https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html]
$(DEPENDENCY_MAKEFILE_DIR)/%.d : %.cpp | $(DEPENDENCY_MAKEFILE_DIR)
	@set -e; rm -f $@; \
    $(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
    sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $(DEPENDENCY_MAKEFILE_DIR)/$(@F); \
	rm -f $@.$$$$

#####################
# Pattern rule to add OBJS_DIR or TEST_OBJS_DIR as order-only prerequisite
# to any .o file
# First rule is for test .o files - matched as it produces shorter stem
test%.o : test%.cpp | $(TEST_OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_OBJS_DIR)/$@ -c $<
# Second rule is for other .o files - matched as the remaining
%.o : %.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -o $(OBJS_DIR)/$@ -c $<



#####################
# Non-test objects
$(ALL_EXE):
	$(LD) $(LDFLAGS) -o $@ $(patsubst %.o, $(OBJS_DIR)/%.o, $(^F))

#####################
# Test objects
$(ALL_TEST):
	$(LD) $(LDFLAGS) -o $@ \
    $(patsubst %.o, $(OBJS_DIR)/%.o,      $(filter-out test%, $(^F))) \
    $(patsubst %.o, $(TEST_OBJS_DIR)/%.o, $(filter     test%, $(^F))) \
    $(BOOST_PATH)