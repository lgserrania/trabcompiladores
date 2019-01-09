/*+−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
| UNIFAL − UNIVERSIDADE FEDERAL DE ALFENAS
| BACHARELADO EM CIENCIA DA COMPUTACAO.
| Trabalho..: PARTE 1 - Construção Arvore Sintatica e PARTE 2 - Geração de codigo MIPS a partir da AS
| Disciplina: Compiladores
| Professor.: Luiz Eduardo da Silva
| Aluno.....: Luis Gustavo de Souza Carvalho
| Data......: 08/01/2019
+−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−*/

typedef struct no *ptno; //renomeando "struct no *" para ptno
typedef struct pi *ptpi;

struct no {
	int tipo; 
    char *valor;
	ptno filho, irmao;
};

struct pi{
	ptno no;
	ptpi prox;
};

ptno criaNo (int, char*);
void adicionaFilho (ptno, ptno);
void mostra (ptno, int);
void verificaConstantes();
void push(ptno);
ptno pop();
int vazia();
void iniciaDot(ptno);
void achaNos();
void ligacoes();
void geraCod();