# # Makefile 1
# foo: foo.c
# 	clang -c foo.c
# 	clang -o foo foo.o

# Makefile 2
foo: foo.o
	clang -o foo foo.o

foo.o: foo.c
	clang -c foo.c

clean:
	rm foo foo.o