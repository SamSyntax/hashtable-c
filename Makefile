run:
	cc -Wall -Wextra -ggdb -pedantic -o bit ./main.c && ./bit ./t8.shakespeare.txt

runCpp:
	g++ -Wall -Wextra -ggdb -pedantic -o hashcpp ./hash.cpp && ./hashcpp ./t8.shakespeare.txt
