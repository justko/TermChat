.SUFFIXES: .cpp  .out
.PHONY:clean
.cpp.out:
	g++ -std=c++11 $< -o $@  -lpthread
clean:
	rm *.out
