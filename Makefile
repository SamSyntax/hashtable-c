run:
	cc -Wall -Wextra -ggdb -pedantic -o ./build/bit ./main.c && ./build/bit ./t8.shakespeare.txt

runCpp:
	g++ -Wall -Wextra -ggdb -pedantic -o ./build/hashcpp ./hash.cpp && ./build/hashcpp ./t8.shakespeare.txt
