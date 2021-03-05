CPPC = g++
CPP_FLAGS = -std=c++17 -g
CPP_INC_PATH = ../io/

main.exe : main.o matrixfun.o
	$(CPPC) $(CPP_FLAGS) main.o matrixfun.o -o main.exe

main.o : main.cpp matrix.hpp matrixfun.hpp
	$(CPPC) -c $(CPP_FLAGS) -I $(CPP_INC_PATH) main.cpp -o main.o

matrixfun.o : matrixfun.cpp matrix.hpp matrixfun.hpp
	$(CPPC) -c $(CPP_FLAGS) -I $(CPP_INC_PATH) matrixfun.cpp -o matrixfun.o
