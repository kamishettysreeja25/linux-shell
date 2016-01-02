she1.c contains the main code
parse.c contain the tokenisation code
execute.c contain the fork and wait execvp code

gcc -o main she1.c parse.c execute.c

./main
