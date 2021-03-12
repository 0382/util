CPPC = g++
CPP_FLAGS = -std=c++17 -g
CPP_INC_PATH = ../io/
HeadFiles = mymath.hpp matrix.hpp matrixfun.hpp integral.hpp interpolation.hpp

test.exe : test.o matrixfun.o
	$(CPPC) $(CPP_FLAGS) test.o matrixfun.o -o test.exe

test.o : test.cpp $(HeadFiles)
	$(CPPC) -c $(CPP_FLAGS) -I $(CPP_INC_PATH) test.cpp -o test.o

matrixfun.o : matrixfun.cpp matrix.hpp matrixfun.hpp
	$(CPPC) -c $(CPP_FLAGS) -I $(CPP_INC_PATH) matrixfun.cpp -o matrixfun.o
