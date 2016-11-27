GCC = gcc -g
input = shell
output = shell

# Compile and run
compile: input.o cd.o
	$(GCC) cd.o $(input).o -o $(output)

input.o: $(input).c
	$(GCC) -c $(input).c

cd.o: cd.c
	$(GCC) -c cd.c

run: compile
	./$(output)

# Other stuff
clean:
	rm -f $(output)
	rm -f *.o
	rm -f *~

gdb: compile
	gdb $(output)

valgrind: compile
	valgrind ./$(output) --leak-check=full --trace-children=yes
