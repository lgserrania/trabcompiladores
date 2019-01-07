simples:
	flex -t lexico.l > lexico.c
	bison -v -d sintatico.y -o sintatico.c
	gcc sintatico.c -o simples
	./simples +d <teste.simples> teste.dot
	dot -Tsvg teste.dot -o teste.svg
	./simples +g <teste.simples> teste.asm
limpa:
	rm -f lexico.c sintatico.c sintatico.output *~ sintatico.h simples
