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