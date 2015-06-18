all: shower_thoughts cl_shower_thoughts test_number

shower_thoughts: shower_thoughts.c
	clang -Wall shower_thoughts.c -o shower_thoughts

cl_shower_thoughts: cl_shower_thoughts.c
	clang -framework OpenCL -Wall cl_shower_thoughts.c -o cl_shower_thoughts

test_number: test_number.c
	clang -Wall test_number.c -o test_number

clean:
	rm shower_thoughts cl_shower_thoughts test_number
