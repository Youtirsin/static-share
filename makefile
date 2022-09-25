INC = -I/home/youtirsin/boost_1_80_0
LIB = -L/home/youtirsin/boost_1_80_0/stage/lib
# lib = -lboost-filesystem

main:
		g++ -o main main.cpp -lpthread

test:
	g++ -o test test.cpp $(INC) $(LIB) $(lib)