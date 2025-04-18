
g++ -fsanitize=address -g -o your_program your_program.cpp
valgrind --tool=memcheck --leak-check=full ./your_program