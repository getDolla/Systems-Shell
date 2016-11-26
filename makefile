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

run: $(output)
	./$(output)

# Other stuff
clean:
	rm -f $(output)
	rm -f *.o
	rm -f *~

gdb: $(output)
	gdb $(output)

valgrind: $(output)
	valgrind ./$(output) --leak-check=full --trace-children=yes
