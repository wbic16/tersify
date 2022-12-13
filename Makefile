trsi:  tersify.o
	gcc tersify.o -o trsi
tersify.h:
	touch tersify.h
tersify.o:  tersify.c tersify.h
	gcc -c tersify.c
