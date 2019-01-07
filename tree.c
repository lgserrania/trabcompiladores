ptno criaNo(int tipo, char *valor){
	ptno n = (ptno) malloc (sizeof(struct no));
    n->tipo = tipo;
    n->valor = (char *)malloc(strlen(valor)*sizeof(char)+1);
	strcpy(n->valor,valor);
    n->filho = NULL;
	n->irmao = NULL;
    return n;
}

ptpi pilha = NULL;
FILE *arq;
int cons = 0;

void adicionaFilho(ptno pai, ptno filho){
	if(filho){
		filho->irmao = pai->filho;
		pai->filho = filho;	
	}
}

void mostra(ptno raiz, int nivel){
	ptno p;
	int i;
	for(i = 0; i < nivel; i++)
		printf("..");
	switch(raiz->tipo){
		case T_PROGRAMA: printf ("Programa: %s\n", raiz->valor); break;
		case T_IDENTIF: printf ("Identificador: %s\n", raiz->valor); break;
		case T_DECLARACAO: printf ("Declaração: %s\n", raiz->valor); break;
		case T_TIPO: printf ("Tipo: %s\n", raiz->valor); break;
		case T_VARIAVEIS: printf ("Lista Variáveis: %s\n", raiz->valor); break;
		case T_LISTACOMANDOS: printf ("Lista de Comandos: %s\n", raiz->valor); break;
		case T_LEIA: printf ("Leitura: %s\n", raiz->valor); break;
		case T_ESCREVA: printf ("Escrita: %s\n", raiz->valor); break;
		case T_FACA: printf ("Faça: %s\n", raiz->valor); break;
		case T_VARIAVEL: printf ("Variável: %s\n", raiz->valor); break;
		case T_ENQTO: printf ("Enquanto: %s\n", raiz->valor); break;
		case T_FIMENQTO: printf ("Fimenquanto: %s\n", raiz->valor); break;
		case T_MAIOR: printf ("Maior: %s\n", raiz->valor); break;
		case T_MENOR: printf ("Menor: %s\n", raiz->valor); break;
		case T_NAO: printf ("Negação: %s\n", raiz->valor); break;
		case T_NUMERO: printf ("Número: %s\n", raiz->valor); break;
		case T_MENOS: printf ("Menos: %s\n", raiz->valor); break;
		case T_ATRIB: printf ("Atribuição: %s\n", raiz->valor); break;
		case T_SE: printf ("Seleção: %s\n", raiz->valor); break;
		case T_TEXTO: printf("Texto: %s\n", raiz->valor);break;
	}
	p = raiz->filho;
	while(p) {
		mostra(p, nivel+1);
		p = p->irmao;
	}
}

void push(ptno pt){
	if(pilha == NULL){
		pilha = (ptpi)malloc(sizeof(struct pi));
		pilha->no = pt;
		pilha->prox = NULL;
		return;
	}
	
	ptpi aux = (ptpi)malloc(sizeof(struct pi));
	aux->no = pt;
	aux->prox = pilha;
	pilha = aux;
}

ptno pop(){
	if(pilha == NULL){
		return NULL;
	}
	
	ptno retorno = pilha->no;
	ptpi aux = pilha;
	pilha = pilha->prox;
	free(aux);
	return retorno;
}


int vazia(){
	if(pilha == NULL){
		return 1;
	}else{
		return 0;
	}
}

void mostrapilha(){
	
	ptpi aux = pilha;
	while(aux != NULL){
		printf("[%s]\n", aux->no->valor);
		aux = aux->prox;
	}
	
}

char * tabelaToken(int token){
	switch(token){
		case T_PROGRAMA: return "Programa"; break;
		case T_IDENTIF: return "Identificador"; break;
		case T_DECLARACAO: return "Declaração"; break;
		case T_TIPO: return "Tipo"; break;
		case T_VARIAVEIS: return "Lista de Variáveis"; break;
		case T_LISTACOMANDOS: return "Lista de Comandos"; break;
		case T_LEIA: return "Leia"; break;
		case T_ESCREVA: return "Escreva"; break;
		//case T_FACA: return "Repeticao"; break;
		case T_VARIAVEL: return "Variável"; break;
		case T_ENQTO: return "Repetição"; break;
		case T_MAIOR: return "Compara Maior"; break;
		case T_MENOR: return "Compara Menor"; break;
		case T_IGUAL: return "Compara Igual"; break;
		case T_E: return "Conjunção"; break;
		case T_OU: return "Disjunção"; break;
		case T_NAO: return "Negação (NÃO)"; break;
		case T_NUMERO: return "Número"; break;
		case T_MENOS: return "Subtrai"; break;
		case T_MAIS: return "Soma"; break;
		case T_VEZES: return "Multiplicação"; break;
		case T_DIV: return "Divisão"; break;
		case T_ATRIB: return "Atribuição"; break;
		case T_SE: return "Seleção"; break;
		case T_TEXTO: return "Texto"; break;
		case T_V: return "Verdadeiro"; break;
		case T_F: return "Falso"; break;
		default: return NULL;
	}
}

void verificaConstantes(){
	ptno atual = pop();
	if(atual->tipo == T_TEXTO){
		if(cons == 0){
			printf("\n\t_esp: .asciiz \" \"\n\t_ent: .asciiz \"\\n\"");
		}
		printf("\n\t_const%d: .asciiz %s", cons, atual->valor);
		sprintf(atual->valor, "_const%d", cons);
		cons++;
	}
	ptno aux = atual->filho;
	while(aux != NULL){
		push(aux);
		aux = aux->irmao;
	}
	if(!vazia()){
		verificaConstantes();
	}
}

void verificaVariaveis(){
	ptno atual = pop();
	ptno aux = atual->filho;
	
	while(aux != NULL){
		push(aux);
		aux = aux->irmao;
	}
	
	if(!vazia()){
		verificaVariaveis();
	}
	
	if(atual->tipo == T_IDENTIF){
		printf("\n\t%s: .word 1", atual->valor);
	}
}

void geraCod(ptno p){
	if(p == NULL) return;
	ptno p1, p2, p3;
	switch(p->tipo){
		
		case T_PROGRAMA:{	
			p1 = p->filho;
			p2 = p1->irmao;
			p3 = p2->irmao;
			printf(".data");
			push(p1);
			verificaConstantes();
			push(p2);
			verificaVariaveis();
			printf("\n.text");
			printf("\n\t.globl main");
			printf("\nmain:\tnop");
			geraCod(p1);
			printf("\nfim:\tnop");
			break;
		}
		case T_LISTACOMANDOS:{
			p1 = p->filho;
			p2 = p1->irmao;
			if(p2 != NULL) geraCod(p2);
			geraCod(p1);
		}
		case T_ESCREVA:{
			p1 = p->filho;
			if(p1->tipo == T_TEXTO)
				printf("\n\tla $a0, %s\n\tli $v0, 4\n\tsyscall", p1->valor);
			else if(p1->tipo == T_NUMERO) 
				printf("\n\tla $a0, %s\n\tli $v0, 1\n\tsyscall", p1->valor);
			else if(p1->tipo == T_VARIAVEL){
				geraCod(p1);
				printf("\n\tli $v0, 1\n\tsyscall");
			}
			break;
		}
		case T_LEIA:{
			printf("\n\tli $v0, 5\n\tsyscall\n\tsw $v0, %s", p->filho->valor);
			break;
		}
		case T_ENQTO:{
			printf("\nL%d:\tnop", ++ROTULO);
			//pushrot(rot);
			empilha(ROTULO);
			p1 = p->filho;//faca
			p2 = p1->irmao;//condicao
			geraCod(p2);			
			printf("\n\tbeqz $a0, L%d", ++ROTULO);
			//pushrot(rot);
			empilha(ROTULO);
			geraCod(p1);
			int aux = desempilha();
			printf("\n\tj L%d", desempilha());
			printf("\nL%d:\tnop", aux);
			break;
		}
		case T_FACA:{
			p1 = p->filho;
			geraCod(p1);
			//printf("\nbeqz $a0, L%d", ++rot);
			break;
		}
		case T_FIMENQTO:{
			int auxrot = desempilha();
			printf("\n\tj L%d\nL%d:\tnop", desempilha(), auxrot);
			break;
		}
		case T_SE:{
			p1 = p->filho;	
			p2 = p1->irmao;
			if(p2->irmao != NULL)
				p3 = p2->irmao;
			else
				p3 = NULL;
			
			if(p3 != NULL){
				geraCod(p3);
				printf("\n\tbeqz $a0, L%d", ++ROTULO);
				//pushrot(rot);
				empilha(ROTULO);
				geraCod(p2);
				printf("\n\tj L%d", ++ROTULO);
				printf("\nL%d:\tnop", desempilha());
				//pushrot(rot);
				empilha(ROTULO);
				geraCod(p1);
				printf("\nL%d:\tnop", desempilha());
			}else{
				geraCod(p2);
				printf("\n\tbeqz $a0, L%d", ++ROTULO);
				geraCod(p1);
				printf("\n\tj L%d", ++ROTULO);
				printf("\nL%d:\tnop", desempilha());
			}
			break;
		}
		case T_ENTAO:{
			p1 = p->filho;
			geraCod(p1);
			break;
		}
		case T_SENAO:{
			p1 = p->filho;
			geraCod(p1);
			break;
		}
		case T_MENOR:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			printf("\n\tslt $a0, $t1, $a0");
			break;
		}
		case T_MAIOR:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			printf("\n\tslt $a0, $a0, $t1");
			break;
		}
		case T_IGUAL:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			printf("\n\tbeq $a0, $t1, L%d", ++ROTULO);
			printf("\n\tli $a0, 0\n\tj L%d", ++ROTULO);
			printf("\nL%d:\tli $a0, 1", --ROTULO);
			printf("\nL%d:\tnop", ++ROTULO);
			break;
		}
		case T_NAO:{
			p1 = p->filho;
			geraCod(p1);
			printf("\n\tbeqz $a0 L%d", ++ROTULO);
			printf("\n\tli $a0, 0\n\tj L%d", ++ROTULO);
			printf("\nL%d:\tli $a0, 1", --ROTULO);
			printf("\nL%d:\tnop", ++ROTULO);
			break;
			
		}
		case T_ATRIB:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p1);
			printf("\n\tsw $a0, %s", p2->valor);
			break;
		}
		case T_MENOS:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			printf("\n\tsub $a0, $t1, $a0");
			break;
		}
		case T_MAIS:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			printf("\n\tadd $a0 $t1 $a0");
			break;
		}
		case T_VEZES:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			printf("\n\tmultu $t1, $a0\n\tmflo $a0");
			break;
		}
		case T_DIV:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			printf("\n\tdivu $t1, $a0\n\tmflo $a0");
			break;
		}
		case T_NUMERO:{
			printf("\n\tli $a0 %s", p->valor);
			break;
		}
		case T_VARIAVEL:{
			printf("\n\tlw $a0 %s", p->valor);
			break;
		}
		
	}
	return;
	
}

void iniciaDot(ptno raiz){
	
	
	
	arq = fopen("graph.dot", "w");
	//fprintf(arq, "%s\n", "digraph {");
	printf("digraph {\n");
	//fprintf(arq, "%s\n", "node [shape=record, height = .1];");
	printf("\tnode [shape=record, height = .1];\n");
	push(raiz);
	achaNos(raiz);
	push(raiz);
	ligacoes();
	//fprintf(arq, "%s", "}");
	printf("}");
	fclose(arq);
	
}

void achaNos(){
	ptno atual = pop();
	//fprintf(arq, "\tn%p", atual);	
	//fprintf(arq, " [label = \"%s | %s \"];\n", tabelaToken(atual->tipo), atual->valor);
	if(tabelaToken(atual->tipo) != NULL){
		printf("\tn%p", atual);
		printf("[label = \"%s | %s \"];\n", tabelaToken(atual->tipo), atual->valor);
	}
	ptno aux = atual->filho;
	while(aux != NULL){
		push(aux);
		aux = aux->irmao;
	}
	if(!vazia()){
		achaNos();
	}
}

void ligacoes(){
	ptno atual = pop();
	ptno aux = atual->filho;
	//if(aux == NULL){
	//	if(!vazia()){
	//		ligacoes(vo, pai);
	//		return;
	//	}
	//}
	//if(tabelaToken(atual->tipo) == NULL){
	//	if(atual->irmao == pai){
	//		atual = vo;
	//	}else{
	//		atual = pai;
	//	}
	//}
	while(aux != NULL){
		push(aux);
		if(tabelaToken(aux->tipo) != NULL){
			//fprintf(arq, "\tn%p -> ", atual);
		printf("\tn%p -> ", atual);
			//fprintf(arq, "n%p\n", aux);
		printf("n%p\n", aux);		
		}
		aux = aux->irmao;
	}
	if(!vazia()){
		ligacoes();
	}
}