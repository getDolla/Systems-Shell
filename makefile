GCC = gcc -g
output = pshell

compile: executor.o
	$(GCC) executor.c -o $(output)

executor.o: executor.c
	$(GCC) -c executor.c

run: $(output)
	./$(output)

clean:
	rm -f $(output)
	rm -f executor.o
	rm -f *~
valgrind:
	valgrind ./$(output) --leak-check=full
