/*+−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
| UNIFAL − UNIVERSIDADE FEDERAL DE ALFENAS
| BACHARELADO EM CIENCIA DA COMPUTACAO.
| Trabalho..: PARTE 1 - Construção Arvore Sintatica e PARTE 2 - Geração de codigo MIPS a partir da AS
| Disciplina: Compiladores
| Professor.: Luiz Eduardo da Silva
| Aluno.....: Luis Gustavo de Souza Carvalho
| Data......: 08/01/2019
+−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−*/

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
int cons = 0; //Variável que irá manipular os _const

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

//Adiciona um nó na pilha
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

//Retira um nó da pilha
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

//Verifica se a pilha ta vazia
int vazia(){
	if(pilha == NULL){
		return 1;
	}else{
		return 0;
	}
}

//Tabela de tokens para a geração do .dot
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

//Faz uma recursão para verificar todos os textos na árvore
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

//Verifica e declara as variáveis presentes no nó Declaração
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

//Gera o código mips por meio de recursao
void geraCod(ptno p){
	if(p == NULL) return;
	ptno p1, p2, p3;
	switch(p->tipo){ 
		case T_PROGRAMA:{
			//Cria o cabeçalho do mips com a declaração de constantes e variáveis
			p1 = p->filho;
			p2 = p1->irmao;
			printf(".data");
			push(p1);
			verificaConstantes();
			push(p2);
			verificaVariaveis();
			printf("\n.text");
			printf("\n\t.globl main");
			printf("\nmain:\tnop");
			//Terminado o cabeçalho, começa a gerar o codigo da lista de comandos
			geraCod(p1);
			printf("\nfim:\tnop");
			break;
		}
		case T_LISTACOMANDOS:{
			//Caso seja uma lista de comandos, ele apenas gera o codigo dos filhos
			p1 = p->filho;
			p2 = p1->irmao;
			if(p2 != NULL) geraCod(p2);
			geraCod(p1);
		}
		case T_ESCREVA:{
			//Escreve o código mips referente ao ESCR de acordo com o que será escrito
			p1 = p->filho;
			if(p1->tipo == T_TEXTO)
				printf("\n\tla $a0, %s\n\tli $v0, 4\n\tsyscall", p1->valor);
			else if(p1->tipo == T_NUMERO) 
				printf("\n\tla $a0, %s\n\tli $v0, 1\n\tsyscall", p1->valor);
			else if(p1->tipo == T_VARIAVEL){
				//Caso seja uma variável, ele gera o cod da mesma pra que ocorra o seu carregamento
				geraCod(p1);
				printf("\n\tli $v0, 1\n\tsyscall");
			}
			break;
		}
		case T_LEIA:{
			//Código mips referente ao LEIA
			printf("\n\tli $v0, 5\n\tsyscall\n\tsw $v0, %s", p->filho->valor);
			break;
		}
		case T_ENQTO:{
			//Codigo mips referente ao NADA
			printf("\nL%d:\tnop", ++ROTULO);
			empilha(ROTULO);
			p1 = p->filho;//faca
			p2 = p1->irmao;//condicao
			geraCod(p2);			
			//Código mips referente ao DSVF
			printf("\n\tbeqz $a0, L%d", ++ROTULO);
			empilha(ROTULO);
			geraCod(p1);
			int aux = desempilha();
			//Código mips referente ao DSVS
			printf("\n\tj L%d", desempilha());
			//Código mips referente ao NADA
			printf("\nL%d:\tnop", aux);
			break;
		}
		case T_SE:{
			//Caso tenha um senão, a variável p3 recebe o irmão de p2
			p1 = p->filho;	
			p2 = p1->irmao;
			if(p2->irmao != NULL)
				p3 = p2->irmao;
			else
				p3 = NULL;
			
			if(p3 != NULL){
				geraCod(p3);
				//Código referente ao DSVF
				printf("\n\tbeqz $a0, L%d", ++ROTULO);
				empilha(ROTULO);
				geraCod(p2);
				//Código referente ao DSVS
				printf("\n\tj L%d", ++ROTULO);
				//Código referente ao NADA
				printf("\nL%d:\tnop", desempilha());
				empilha(ROTULO);
				geraCod(p1);
				//Código referente ao NADA
				printf("\nL%d:\tnop", desempilha());
			}else{
				geraCod(p2);
				//Código referente ao DSVF
				printf("\n\tbeqz $a0, L%d", ++ROTULO);
				geraCod(p1);
				//Código referente ao DSVS
				printf("\n\tj L%d", ++ROTULO);
				//Código referente ao NADA
				printf("\nL%d:\tnop", desempilha());
			}
			break;
		}
		case T_MENOR:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			//Código referente ao EMPILHA
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			//Código referente ao DESEMPILHA
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			//Código referente ao CMME
			printf("\n\tslt $a0, $t1, $a0");
			break;
		}
		case T_MAIOR:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			//Código referente ao EMPILHA
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			//Código referente ao DESEMPILHA
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			//Código referente ao CMMA
			printf("\n\tslt $a0, $a0, $t1");
			break;
		}
		case T_IGUAL:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			//Código referente ao EMPILHA
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			//Código referente ao DESEMPILHA
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			//Código referente ao CMIG
			printf("\n\tbeq $a0, $t1, L%d", ++ROTULO);
			printf("\n\tli $a0, 0\n\tj L%d", ++ROTULO);
			printf("\nL%d:\tli $a0, 1", --ROTULO);
			printf("\nL%d:\tnop", ++ROTULO);
			break;
		}
		case T_NAO:{
			p1 = p->filho;
			geraCod(p1);
			//Código referente ao NEGA
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
			//Código referente ao ARZG
			printf("\n\tsw $a0, %s", p2->valor);
			break;
		}
		case T_MENOS:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			//Código referente ao EMPILHA
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			//Código referente ao DESEMPILHA
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			//Código referente ao SUBT
			printf("\n\tsub $a0, $t1, $a0");
			break;
		}
		case T_MAIS:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			//Código referente ao EMPILHA
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			//Código referente ao DESEMPILHA
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			//Código referente ao SOMA
			printf("\n\tadd $a0 $t1 $a0");
			break;
		}
		case T_VEZES:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			//Código referente ao EMPILHA
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			//Código referente ao DESEMPILHA
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			//Código referente ao MULT
			printf("\n\tmultu $t1, $a0\n\tmflo $a0");
			break;
		}
		case T_DIV:{
			p1 = p->filho;
			p2 = p1->irmao;
			geraCod(p2);
			//Código referente ao EMPILHA
			printf("\n\tsw $a0 0($sp)\n\taddiu $sp $sp -4");
			geraCod(p1);
			//Código referente ao DESEMPILHA
			printf("\n\tlw $t1 4($sp)\n\taddiu $sp $sp 4");
			//Código referente ao DIVI
			printf("\n\tdivu $t1, $a0\n\tmflo $a0");
			break;
		}
		case T_NUMERO:{
			//Código referente ao CRCT
			printf("\n\tli $a0 %s", p->valor);
			break;
		}
		case T_VARIAVEL:{
			//Código referente ao CRVG
			printf("\n\tlw $a0 %s", p->valor);
			break;
		}
		case T_V:{
			//Código referente ao CRCT
			printf("\n\tlw $a0 1");
			break;
		}
		case T_F:{
			//Código referente ao CRCT
			printf("\n\tlw $a0 0");
			break;
		}
		
	}
	return;
	
}

void iniciaDot(ptno raiz){
	//Escreve o cabeçalho do .dot
	printf("digraph {\n");
	printf("\tnode [shape=record, height = .1];\n");
	push(raiz);
	//Empilha a raiz e vai procurar os nós existentes para declarar
	achaNos(raiz);
	push(raiz);
	ligacoes();
	printf("}");
	
}

//Encontra e declara todos os nós
void achaNos(){
	ptno atual = pop();
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

//Encontra e escreve as ligações entre os nós
void ligacoes(){
	ptno atual = pop();
	ptno aux = atual->filho;
	while(aux != NULL){
		push(aux);
		if(tabelaToken(aux->tipo) != NULL){
			printf("\tn%p -> ", atual);
			printf("n%p\n", aux);		
		}
		aux = aux->irmao;
	}
	if(!vazia()){
		ligacoes();
	}
}