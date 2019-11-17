#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct no
{
	int dado;
	struct no *esq, *dir;
}No;

int menu();
int qtd_elementos(No**);
void inserir(No**, int);
void pre_ordem(No**);
void pos_ordem(No**);
void em_ordem(No**);
No *maior_elemento(No**);
No *maior_elemento_rec(No**);
No *menor_elemento_rec(No**);
No *menor_elemento(No**);
int apagar_maior(No**);
int mostrar_folha(No**);
int comp_raiz_ate_n(No**,int);
int existe_n(No**,int);
void mostrar_caminho_n_ate_raiz(No**,int);
int altura(No**);
int remover_no(No**,int);
int qtd_em_um_nivel(No**,int);
void nos_em_um_nivel(No**,int);
void nos_distantes(No**);
int qtd_ultimo_nivel(No**);
int arvore_balanceada(No**);
void copia(No**,No**);
void destruir(No**);
int arvores_iguais(No**,No**);
int height(No**,int*);
int busca(No**,int);

clock_t tempoOrd,t_ini,t_fim,tempo;
 
int main(){
	No *aux2 = NULL;
	No *r = NULL;
	No *r2 = NULL;
	int op;
	int aux;
	do{
		if(arvore_balanceada(&r)==1) printf("A árvore está balanceada.\n");
		else printf("A árvore não está balanceada.\n");
		op = menu();
		switch(op){
			case 0: 
				break;
			case 1:
				printf("Digite o valor a ser inserido\n");
				scanf("%d",&aux);
				inserir(&r,aux);
				break;
			case 2:
				printf("Pré-Ordem:\n\n");
				pre_ordem(&r);
				printf("\n");
				break;
			case 3:
				printf("Em Ordem:\n\n");
				em_ordem(&r);
				printf("\n");
				break;
			case 4:
				printf("Pós Ordem:\n\n");
				pos_ordem(&r);
				printf("\n");
				break;
			case 5:
				printf("Quantidade de elementos: %d.\n", qtd_elementos(&r));
				break;
			case 6:
				scanf("%d",&aux);
				remover_no(&r,aux);
				break;
			case 24:
				scanf("%d",&aux);
				busca(&r,aux);
				break;
			case 8:
				aux2 = menor_elemento(&r);
				printf("%d\n", aux2->dado);
				break;
			case 9:
				aux2 = menor_elemento_rec(&r);
				printf("%d\n", aux2->dado);
				break;
			case 10:
				apagar_maior(&r);
				break;
			case 11:
				mostrar_folha(&r);
				printf("\n");
				break;
			case 12:{		
				int n;
				printf("Informe o valor: ");
				scanf("%d", &n);
				printf("O comprimento é: %d\n", comp_raiz_ate_n(&r, n));
				break;
			}		
			case 13:{
				int n;
				printf("Informe o valor: ");
				scanf("%d", &n);
				mostrar_caminho_n_ate_raiz(&r, n);
				break;
			}
			case 14:{
				int n;
				printf("Informe o valor: ");
				scanf("%d", &n);
				if(remover_no(&r, n)) printf("\n..Elemento removido..\n");
				else printf("\n..Elemento não encontrado..\n");
				break;
			}
			case 15:{
				int i=0;
				printf("A altura da árvore é %d.\n", height(&r,&i));
				printf("Chamadas: %d\n",i);
				break;
			}
			case 16:{
				int n;
				printf("Informe o nível: ");
				scanf("%d", &n);
				printf("A quantidade de nós no nível %d é %d.\n", n, qtd_em_um_nivel(&r, n));
				break;
			}
			case 17:{
				int n;
				printf("Informe o nível: ");
				scanf("%d", &n);
				nos_em_um_nivel(&r, n);
				printf("\n");
				break;
			}
			case 29:
				nos_distantes(&r);
				printf("\n");
				break;
			case 19:
				printf("Quantidade de nós no último nível: %d\n", qtd_ultimo_nivel(&r));
				printf("\n");
				break;
			case 20:
				copia(&r, &r2);
				break;
			case 21:
				pre_ordem(&r2);
				printf("\n");
				break;
			case 22:
				destruir(&r);
				break;
			case 23:
				if(arvores_iguais(&r,&r2)==0) printf("As árvores não são iguais.\n");
				else printf("As árvores são iguais.\n");
				break;
			case 30:
				t_ini=time(NULL);
				tempoOrd = clock();
				break;
			case 25:
				t_fim=time(NULL);
				tempo=difftime(t_fim,t_ini);
				tempoOrd = clock() - tempoOrd;
				printf("%f\n",((float)tempoOrd)/CLOCKS_PER_SEC );
				break;
		}
	}while(op!=0);
	return 0;	
}

int menu(){
	int op;
	printf("[0]  Sair\n");
	printf("[1]  Inserir\n");
	printf("[2]  Preordemm\n");
	printf("[3]  Em Ordem\n");
	printf("[4]  Pós Ordem\n");
	printf("[5]  Imprimir a quantidade de elementos\n");
	printf("[6]  Imprimir o maior elemento\n");
	printf("[7]  Imprimir o maior elemento recursivo\n");
	printf("[8]  Imprimir o menor elemento\n");
	printf("[9]  Imprimir o menor elemento recursivo\n");
	printf("[10] Apagar o maior valor\n");
	printf("[11] Mostrar folhas\n");
	printf("[12] Comprimento da raiz até N\n");
	printf("[13] Mostrar o caminho de N até a Raíz\n");
	printf("[14] Remover um elemento qualquer\n");
	printf("[15] Mostrar altura da árvore\n");
	printf("[16] Mostrar quantidade em um nível\n");
	printf("[17] Mostrar os nós em um nível qualquer\n");
	printf("[18] Mostrar nós mais distantes da raiz.\n");
	printf("[19] Quantidade de nós no último nível\n");
	printf("[20] Fazer cópia para r2\n");
	printf("[21] Pré-Ordem r2\n");
	printf("[22] Destruir a árvore\n");
	printf("[23] Verificar igualdade entre r1 e r2\n");
	scanf("%d",&op);
	//system("clear");
	return op;
}

int height(No **r, int *i){
	if(*r==NULL) return -1;
	(*i)++;
	int u, v;
	u = height(&(*r)->esq,i);
	v = height(&(*r)->dir,i);
	if(u>v) return u+1;
	else return v+1;
}

int qtd_elementos(No **r){
	if(*r == NULL){
		return 0;
	}
	return 1 + qtd_elementos(&(*r)->esq) + qtd_elementos(&(*r)->dir);
}

void inserir(No **r, int vlr){
	if(*r==NULL){
		No *novo = (No*)malloc(sizeof(No));
		novo->dado = vlr;
		novo->esq = NULL;
		novo->dir = NULL;
		*r = novo;
		return;
	}
	if(vlr>(*r)->dado){
		inserir(&(*r)->dir, vlr);
	}else{
		inserir(&(*r)->esq, vlr);
	}
}

void pre_ordem(No** r){
	if(*r!=NULL){
		printf("%d ",(*r)->dado);
		pre_ordem(&(*r)->esq);
		pre_ordem(&(*r)->dir);
	}
}

void pos_ordem(No **r){
	if(*r != NULL){
		pos_ordem(&(*r)->esq);
		pos_ordem(&(*r)->dir);
		printf("%d ",(*r)->dado );
	}
}

void em_ordem(No **r){
	if(*r != NULL){
		em_ordem(&(*r)->esq);
		printf("%d ",(*r)->dado );
		em_ordem(&(*r)->dir);
	}
}

No *maior_elemento(No **r){
	if(*r==NULL) return NULL;
	No *aux = (No*)malloc(sizeof(No));
	aux = (*r);
	while(1){
		if(aux->dir == NULL){
			return aux;
		}
		aux = aux->dir;
	}
}


No *maior_elemento_rec(No **r){
	if((*r)->dir == NULL){
		return *r;
	}
	maior_elemento_rec(&(*r)->dir);
}

No *menor_elemento_rec(No **r){
	if((*r)->esq == NULL){
		return *r;
	}
	menor_elemento_rec(&(*r)->esq);
}

No *menor_elemento(No **r){
	No *aux = (No*)malloc(sizeof(No));
	aux = (*r);
	while(1){
		if(aux->esq == NULL){
			return aux;
		}
		aux = aux->esq;
	}
}

int apagar_maior(No **r){
	if(*r == NULL){
		return 0;
	}
	if((*r)->dir == NULL){
		No *aux;
		aux = *r;	
		*r = (*r)->esq;
		free(aux);
		return 1;
	}
	else{
		return apagar_maior(&(*r)->dir);		
	}
}

int mostrar_folha(No **r){
	if(*r == NULL){
		return 0;
	}
	if((*r)->dir == NULL && (*r)->esq == NULL){
		printf("%d ",(*r)->dado );
	}
	mostrar_folha(&(*r)->esq);
	mostrar_folha(&(*r)->dir);
}

int comp_raiz_ate_n(No **r, int vlr){
	if(*r==NULL) return -1;
	if((*r)->dado==vlr){
		return 0;
	}
	if(vlr>(*r)->dado){
		return 1+comp_raiz_ate_n(&(*r)->dir, vlr);
	}else{
		return 1+comp_raiz_ate_n(&(*r)->esq, vlr);
	}
}

int existe_n(No **r, int n){
	if(*r==NULL) return 0;
	if((*r)->dado==n) return 1;
	if(n>(*r)->dado) return existe_n(&(*r)->dir, n);
	return existe_n(&(*r)->esq, n);
}

void mostrar_caminho_n_ate_raiz(No** r,int n){
	if(*r==NULL) return;
	if((*r)->dado==n){
		printf("%d\n", (*r)->dado);
	}else if(n>(*r)->dado){ 
		mostrar_caminho_n_ate_raiz(&(*r)->dir, n);
		printf("%d\n", (*r)->dado);
	}else{
		mostrar_caminho_n_ate_raiz(&(*r)->esq, n);
		printf("%d\n", (*r)->dado);
	}
}


int altura(No** r){
	if(*r==NULL) return -1;
	if(altura(&(*r)->esq)>altura(&(*r)->dir)){
		return 1 + altura(&(*r)->esq);
	}
	return 1 + altura(&(*r)->dir); 
}

int remover_no(No** r, int n){
	if(*r==NULL) 
		return 0;
	if((*r)->dado==n){
		if((*r)->esq==NULL){
			//Caso 1
			No* aux = *r;
			*r = (*r)->dir;
			free(aux);
			return 1;
		}
		if((*r)->dir==NULL){
			//Caso 2
			No* aux = *r;
			*r = (*r)->esq;
			free(aux);
			return 1;
		}
		else{
			//Caso 3
			No* aux;
			aux = maior_elemento(&(*r)->esq);
			(*r)->dado = aux->dado;
			return remover_no(&(*r)->esq, (*r)->dado);
		}
	}
	if(n>(*r)->dado){
		return remover_no(&(*r)->dir, n);
	}else{
		return remover_no(&(*r)->esq, n);
	}
}

int qtd_em_um_nivel(No **r, int n){
	if(altura(r)<n) return -1;
	if(*r==NULL) return 0;
	if(n==0) return 1;
	return qtd_em_um_nivel(&(*r)->esq, n-1)+qtd_em_um_nivel(&(*r)->dir, n-1);
}

void nos_em_um_nivel(No** r, int n){
	if(altura(r)<n) return;
	if(*r==NULL) return;
	if(n==0){
		printf("%d ", (*r)->dado);
	}
	nos_em_um_nivel(&(*r)->dir, n-1);
	nos_em_um_nivel(&(*r)->esq, n-1);
}

void nos_distantes(No** r){
	if(*r==NULL) return;
	nos_em_um_nivel(r, altura(r));
}

int qtd_ultimo_nivel(No** r){
	if(*r==NULL) return -1;
	qtd_em_um_nivel(r, altura(r));
}

int arvore_balanceada(No** r){
	if(*r==NULL) return -1;
	int diferenca;
	diferenca = qtd_elementos(&(*r)->esq)-qtd_elementos(&(*r)->dir);
	if(diferenca<0) diferenca*=(-1);
	if(diferenca<=1) return 1;
	return 0;
}

void copia(No** r1,No** r2){
	if(*r1!=NULL){
		inserir(r2, (*r1)->dado);
		copia(&(*r1)->esq, r2);
		copia(&(*r1)->dir, r2);
	}
}

void destruir(No** r){
	if(*r!=NULL){
		destruir(&(*r)->esq);
		destruir(&(*r)->dir);
		free(*r);
		*r = NULL;
	}
}

int arvores_iguais(No** r1,No** r2){
	if((*r1==NULL)&&(*r2==NULL)) return 1;
	if((*r1==NULL)||(*r2==NULL)) return 0;
	if((*r1)->dado!=(*r2)->dado) return 0;
	return arvores_iguais(&(*r1)->dir, &(*r2)->dir)&&arvores_iguais(&(*r1)->esq, &(*r2)->esq);
}

int busca(No **r, int vlr){
	if(*r==NULL) return 0;
	if(vlr==(*r)->dado) return 1;
	if(vlr>(*r)->dado) return busca(&(*r)->dir,vlr);
	return busca(&(*r)->esq,vlr);
}