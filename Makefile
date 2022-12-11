tersify:  tersify.o
	gcc tersify.o -o tersify
tersify.h:
	touch tersify.h
tersify.o:  tersify.c tersify.h
	gcc -c tersify.c
