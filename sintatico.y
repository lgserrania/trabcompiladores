/*+−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
| UNIFAL − UNIVERSIDADE FEDERAL DE ALFENAS
| BACHARELADO EM CIENCIA DA COMPUTACAO.
| Trabalho..: PARTE 1 - Construção Arvore Sintatica e PARTE 2 - Geração de codigo MIPS a partir da AS
| Disciplina: Compiladores
| Professor.: Luiz Eduardo da Silva
| Aluno.....: Luis Gustavo de Souza Carvalho
| Data......: 08/01/2019
+−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−*/


%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utils.c"
#include "lexico.c" 
#include "tree.c"
//Foram utilizadas as seguintes variáveis
//Raiz para ser a raiz da árvore
//No para auxiliar na criação dos nós
//auxp para ser um ponteiro auxiliar na hora de montar a árvore
//auxl para ser um ponteiro auxiliar nos casos em que possuem laços (enquanto/seleção)
//auxd para ser um ponteiro auxiliar nos casos de mais de um tipo de declaração de variáveis
//laco para verificar se está dentro de um laço
//dot para verificar se é pra gerar o arquivo dot
//mips para verificar se é pra gerar o arquivo mips
//con para gerar os valores _const 
ptno raiz = NULL, no, auxp, auxl, auxd;
int laco = 0;
int dot = 0;
int mips = 0;
int con = 0;

int yylex();
void yyerror(char *);

%}




%start programa 

//terminais
%token T_PROGRAMA
%token T_INICIO
%token T_FIM
%token T_IDENTIF
%token T_DECLARACAO
%token T_VARIAVEL;
%token T_VARIAVEIS
%token T_TIPO
%token T_LISTACOMANDOS
%token T_ENTRADASAIDA
%token T_LEIA
%token T_ESCREVA
%token T_ENQTO
%token T_FACA
%token T_FIMENQTO
%token T_SE
%token T_ENTAO
%token T_SENAO
%token T_FIMSE
%token T_ATRIB
%token T_VEZES
%token T_DIV
%token T_MAIS
%token T_MENOS
%token T_MAIOR
%token T_MENOR
%token T_IGUAL
%token T_E
%token T_OU
%token T_V
%token T_F
%token T_NUMERO
%token T_TEXTO
%token T_NAO
%token T_ABRE
%token T_FECHA
%token T_LOGICO
%token T_INTEIRO

//Precedencia (da maior para a maior)
%left T_E T_OU
%left T_IGUAL
%left T_MAIOR T_MENOR
%left T_MAIS T_MENOS
%left T_VEZES T_DIV

%%

//regras de produção
/*aqui vem as regras da gramatica*/
/*regra --------------------------> semantica*/

programa
	: cabecalho 
		{
			//Cria a raiz do programa e adiciona como filho o identificador do msm
			raiz = criaNo(T_PROGRAMA, "");
			adicionaFilho(raiz, $1);
		}
	variaveis
		{
			//adiciona a lista de variáveis completa na raiz
			adicionaFilho(raiz, $3);
			auxp = NULL;
		}
	T_INICIO lista_comandos
		{
			//Adiciona na raiz a variável auxiliar de lista_comandos
			adicionaFilho(raiz,auxp);
		}
	T_FIM
		{
			if(dot)
				iniciaDot(raiz);
			if(mips)
				geraCod(raiz);
		}
	;

cabecalho
	: T_PROGRAMA T_IDENTIF
		{
			//Retorna o nome do programa
			no = criaNo(T_IDENTIF, atomo);
			$$ = no;
		}
	;

variaveis
	: /* vazio */
	| declaracao_variaveis
		{
			$$ = $1;
		}
	;
	
declaracao_variaveis: 
	declaracao_variaveis
	tipo
		{
			CONTA_VARS = 0;
			$$ = $1;
		}
	lista_variaveis
	 	{	
			//As declarações subsequentes cairão aqui, onde cria-se um novo nó de declaração
			//e o adiciona como filho da variável auxiliar, que passa a apontar pra esse nó criado
			//Com isso sempre terei acesso direto ao último nó de declaração criado
			no = auxd;
			adicionaFilho(no, criaNo(T_DECLARACAO, ""));
			no = no->filho;
			auxd = no;
			adicionaFilho(no, $2);
			adicionaFilho(no, $4);
			$$ = $3;
		}
	//A primeira declaração será executada abaixo.
	//Primeiro retorna o tipo, depois cria um nó de declaração e adiciona o tipo
	//E o retorno de lista de variáveis
	|tipo
		{
			CONTA_VARS = 0;
			$$ = $1;
		}
	lista_variaveis
	  	{
			no = criaNo(T_DECLARACAO, "");
			auxd = no;
			adicionaFilho(no, $2);
			adicionaFilho(no, $3);
			$$ = no;
			
		}
	;
//Retorna o tipo da declaração de variáveis atual	
tipo
	: T_LOGICO
		{
			no = criaNo(T_TIPO, "logico");
			$$ = no;
		}
	| T_INTEIRO
		{
			no = criaNo(T_TIPO, "inteiro");
			$$ = no;
		}
	;

lista_variaveis: 
	lista_variaveis{
		//Retorno da lista de variáveis a cada execução da mesma
		$$ = $1;
	}
	T_IDENTIF
	  	{
			//Recebe o retorno da passada, cria um novo nó de lista de variáveis e adiciona esse retorno como filho
			//Adiciona como filho também o identificador da variável declarada.
			insere_variavel(atomo); 
			CONTA_VARS++;
			no = criaNo(T_VARIAVEIS, "");
			adicionaFilho(no, criaNo(T_IDENTIF, atomo));
			adicionaFilho(no, $2);
			$$ = no;
		}
	|T_IDENTIF
		{
			//Executa na primeira variável declarada. Cria um nó de lista de variáveis
			//e adiciona como filho o identificador da variável
			insere_variavel(atomo);		
			CONTA_VARS++;
			no = criaNo(T_VARIAVEIS, "");
			adicionaFilho(no, criaNo(T_IDENTIF, atomo));
			$$ = no;
		}
	;
	
lista_comandos
	: /* vazio */
	| comando{

	}	
	
	lista_comandos
		{
			//Caso não esteja em um laço, o retorno é adicionado na variável auxiliar
			//Como os retornos vem do último ao primeiro, essa variável auxiliar
			//Visa colocar eles na ordem certa
			if(!laco){
				adicionaFilho($1, auxp);
				auxp = $1;
			}else{
				//Caso esteja em um laço, o retorno é adicionado na variável auxiliar de laço
				//Ela é usada para que depois tudo seja adicionado como filho do nó correspondente ao laço
				adicionaFilho($1, auxl);
				auxl = $1;
			}
		}
	;
	
comando
	: entrada_saida
		{
			//Cria um nó de entrada e saída e adiciona como filho o seu retorno
			no = criaNo(T_LISTACOMANDOS, "");
			adicionaFilho(no, $1);
			$$ = no;
		}
	| repeticao
		{
			//Cria um nó de repetição e adiciona como filho o seu retorno
			//Tambem decrementa-se a variável de laço pra indicar que esse laço terminou
			no = criaNo(T_LISTACOMANDOS, "");
			adicionaFilho(no, $1);
			laco--;
			auxl = NULL;
			$$ = no;
		}
	| selecao
		{
			//Cria um nó de seleção e adiciona como filho o seu retorno
			//Tambem decrementa-se a variável de laço pra indicar que esse laço terminou
			no = criaNo(T_LISTACOMANDOS, "");
			adicionaFilho(no, $1);
			laco--;
			auxl = NULL;
			$$ = no;
		}
	| atribuicao
		{
			//Cria um nó de atribuição e adiciona como filho o seu retorno
			no = criaNo(T_LISTACOMANDOS, "");
			adicionaFilho(no, $1);
			$$ = no;
		}
	;
	
//retorna diretamente o que foi retornado por leitura ou escrita
entrada_saida
	: leitura
		{
			$$ = $1;
		}
	| escrita
		{
			$$ = $1;
		}
	;

leitura
	:T_LEIA
	 T_IDENTIF
	  	{
	  		POS_SIMB = busca_simbolo(atomo);
	  		if(POS_SIMB == -1)
	  			ERRO("Variavel [%s] nao declarada!", atomo);
	  		else{
				//Cria um nó de leitura e adiciona como filho o identificador do que está sendo lido
				no = criaNo(T_LEIA, "");
				adicionaFilho(no, criaNo(T_IDENTIF, atomo));
				$$ = no;
	  		}
	  	}
	;

escrita
	:T_ESCREVA expressao
		{
			//Cria um nó de escrita e adiciona como filho o retorno da expressão
			no = criaNo(T_ESCREVA, "");
			adicionaFilho(no, $2);
			$$ = no;
		}
	;
	
repeticao
	:T_ENQTO
		{
			//Incrementa-se o laço para indicar que um laço iniciou
			laco++;
			empilha(ROTULO);
			//Cria o nó raiz do laço
			$$ = criaNo(T_ENQTO, "");
		}
	expressao T_FACA
		{	
			//Adiciona como filho da raiz criada a subárvore correspondente a condição do laço  
			adicionaFilho($2, $3);
			empilha(ROTULO);
			$$ = $2;
		}
	
	lista_comandos{
		
	}
	T_FIMENQTO
		{	
			//No fim, adiciona-se como filho da raiz do laço a variável auxiliar
			//que contém toda a lista de comandos correspondente ao mesmo
			no = $2;
			adicionaFilho(no, auxl);
			$$ = no;
		}
	;
	
selecao
	:T_SE expressao
		{			
			//Ocorre semelhante a repetição
			//Incrementa-se a variável laço e retorna a subárvore correspondente a condição
			laco++;
			empilha(ROTULO);
			$$ = $2;
		}
		
	
	T_ENTAO
	lista_comandos
		{
			//Retorna a lista de comandos correspondente ao ENTÃO e zera a variável auxiliar
			//Para que a mesma seja usada no SENÃO
			$$ = auxl;
			auxl = NULL;
		}
	T_SENAO
		{	
			empilha(ROTULO);
		}
	
	lista_comandos
		{
			//Retorna a lista de comandos correspondente ao SENÃO e zera a variável auxiliar
			$$ = auxl;
			auxl = NULL;
		}
	T_FIMSE
		{
			//Cria-se o nó raiz da seleção
			//Adiciona-se respectivamente subárvore correspondente a condição, a subárvore correspondente ao ENTÃO
			//e a subárvore correspondente ao SENÃO
			no = criaNo(T_SE, "SE");	
			adicionaFilho(no, $3);
			adicionaFilho(no, $6);
			adicionaFilho(no, $10);
			$$ = no;
		}
	;

atribuicao
	:T_IDENTIF
		{
			//Cria o no de atribuição e adiciona como filho o identificador 
			//da variável que irá receber a atribuição
			no = criaNo(T_ATRIB, "ATRIBUICAO");
			adicionaFilho(no, criaNo(T_IDENTIF, atomo));
			POS_SIMB = busca_simbolo(atomo);
			if(POS_SIMB == -1)
				ERRO("Variavel [%s] nao declarada!");
			else
				empilha (TSIMB[POS_SIMB].desloca);
			$$ = no;
		}
	T_ATRIB	expressao
		{
			//Adiciona como filho da atribuição o retorno da expressão
			adicionaFilho($2, $4);
			$$ = $2;
		}
	;

	//Essa parte é bem generalizada, pra cada tipo de expressão ele retorna
	//um nó que a identifica juntamente com os valores 
	//O termo retorna apenas a si mesmo
expressao
	: expressao T_VEZES expressao
		{
			no = criaNo(T_VEZES, "");
			adicionaFilho(no, $1);
			adicionaFilho(no, $3);
			$$ = no;
		}
	| expressao T_DIV expressao
		{
			no = criaNo(T_DIV, "");
			adicionaFilho(no, $1);
			adicionaFilho(no, $3);
			$$ = no;
		}
	| expressao T_MAIS expressao
		{
			no = criaNo(T_MAIS, "");
			adicionaFilho(no, $1);
			adicionaFilho(no, $3);
			$$ = no;
		}
	| expressao T_MENOS expressao
		{
			no = criaNo(T_MENOS, "");
			adicionaFilho(no, $1);
			adicionaFilho(no, $3);
			$$ = no;
		}
	| expressao T_MAIOR expressao
		{
			no = criaNo(T_MAIOR, "");
			adicionaFilho(no, $1);
			adicionaFilho(no, $3);
			$$ = no;
		}
	| expressao T_MENOR expressao
		{
			no = criaNo(T_MENOR, "MENOR");
			adicionaFilho(no, $1);
			adicionaFilho(no, $3);
			$$ = no;
		}
	| expressao T_IGUAL expressao
		{
			no = criaNo(T_IGUAL, "IGUAL");
			adicionaFilho(no, $1);
			adicionaFilho(no, $3);
			$$ = no;
		}
	| expressao T_E expressao
		{
			no = criaNo(T_E, "");
			adicionaFilho(no, $1);
			adicionaFilho(no, $3);
			$$ = no;
		}
	| expressao T_OU expressao
		{
			no = criaNo(T_VEZES, "");
			adicionaFilho(no, $1);
			adicionaFilho(no, $3);
			$$ = no;
		}
	| termo
		{
			$$ = $1;
		}
	;

	//Também generalizada, pra cada tipo de termo cria-se um nó que o identifica 
	//e o retorna. No caso do NAO, adiciona-se tbm o filho que será o termo a ser negado	
termo
	: 	T_IDENTIF
		{
			POS_SIMB = busca_simbolo(atomo);
			if(POS_SIMB == -1)
				ERRO("Variavel [%s] nao declarada");
			else{
				no = criaNo(T_VARIAVEL, atomo);
				$$ = no;
			}
		}
	| T_TEXTO
		{
			if(mips)
				no = criaNo(T_TEXTO, atomo);
			else{
				sprintf(atomo, "_const%d", con++);
				no = criaNo(T_TEXTO, atomo);
			}
			$$ = no;
		}
	| T_NUMERO
		{
			no = criaNo(T_NUMERO, atomo);
			$$ = no;
		}
	| 	T_V
		{
			no = criaNo(T_V, "");
			$$ = no;
		}
	| 	T_F
		{
			no = criaNo(T_F, "");
			$$ = no;
		}
	| T_NAO termo
		{
			no = criaNo(T_NAO, "");
			adicionaFilho(no, $2);
			$$ = no;
		}
	| T_ABRE expressao T_FECHA
		{
			$$ = $2;
		}
	;

%%

/*+-------------------------------------------------------------
  | 		CORPO PRINCIPAL DO PROGRAMA 					   |	
  +------------------------------------------------------------- 
*/


int yywrap (){
return 1;
}

void yyerror (char *s)
{
	ERRO("ERRO SINTATICO");
}

int main(int argc, const char* argv[]){
	int i;
	if((argc > 1) && (strcmp(argv[1], "+d") == 0)){
		dot = 1;
	}
	if((argc > 1) && (strcmp(argv[1], "+g") == 0)){
		mips = 1;
	}
	return yyparse();
}
