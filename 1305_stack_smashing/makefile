all:
	make ex1
	make ex3

ex1:
	gcc -w -fno-stack-protector example1.c -o example1
	make ex1-asm

ex1-asm:
	gcc -w example1.c -S -o example1.asm

ex3:
	gcc -w -fno-stack-protector example3.c -o example3
	make ex3-asm

ex3-asm:
	gcc -w example3.c -S -o example3.asm