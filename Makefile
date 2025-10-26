run:
	@./build/hash_binary ./t8.shakespeare.txt

runCpp:
	@mkdir -p ./build && g++ -Wall -Wextra -ggdb -pedantic -o ./build/hashcpp ./hash.cpp && ./build/hashcpp ./t8.shakespeare.txt
