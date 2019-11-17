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
void rse(No**);
void rsd(No**);
void rdd(No**);
void rde(No**);
void corrigir_balanceamento(No**);
int fator_balanceamento(No**); 
int height(No**);
int nao_folha(No**);
No *maior_elemento_rec(No**);
int remover_no(No**,int);
int busca(No**,int);

clock_t tempoOrd,t_ini,t_fim,tempo;

int main(void){
    No **r;
    *r = NULL;
    int op,aux;
    do{
        op=menu();
        switch(op){
            case 0:
                break;
            case 1:{
				int n;
                printf("Informe o valor: ");
				scanf("%d",&n);
				inserir(r,n);
				break;
            }
			case 2:
				printf("Pré-Ordem:\n\n");
				pre_ordem(r);
				printf("\n");
				break;
			case 3:
				printf("Em Ordem:\n\n");
				em_ordem(r);
				printf("\n");
				break;
			case 4:
				printf("Pós Ordem:\n\n");
				pos_ordem(r);
				printf("\n");
				break;
			case 6:
				scanf("%d",&aux);
				remover_no(r,aux);
				break;
			case 7:
				scanf("%d",&aux);
				busca(r,aux);
				break;
			case 24:
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
    printf("-------------------------------\n");
    printf("              MENU             \n");
    printf("-------------------------------\n");
	printf("[0] Sair\n");
	printf("[1] Inserir\n");
	printf("[2] Preordemm\n");
	printf("[3] Em Ordem\n");
	printf("[4] Pós Ordem\n");
	printf("[5] Quantidade de nós que NÃO são folha\n");
	scanf("%d",&op);
	//system("clear");
	return op;
}

int nao_folha(No **r){
	if(*r==NULL) return 0;
	if(((*r)->esq==NULL)&&((*r)->dir==NULL)) return 0;
	return 1+nao_folha(&(*r)->esq)+nao_folha(&(*r)->dir);
}

int qtd_elementos(No **r){
	if(*r == NULL){
		return 0;
	}
	return 1+qtd_elementos(&(*r)->esq)+qtd_elementos(&(*r)->dir);
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
	corrigir_balanceamento(r);
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

void rse(No **r){
    if(*r==NULL) return;
    No *aux;
    aux = *r;
    *r = (*r)->dir;
    aux->dir = (*r)->esq;
    (*r)->esq = aux;
}

void rsd(No **r){
    if(*r==NULL) return;
    No *aux;
    aux = *r;
    *r = (*r)->esq;
    aux->esq = (*r)->dir;
    (*r)->dir = aux;
}

void rdd(No **r){
	rse(&(*r)->esq);
	rsd(r);
}

void rde(No **r){
	rsd(&(*r)->dir);
	rse(r);
}

void corrigir_balanceamento(No **r){
	if(*r==NULL) return;
	corrigir_balanceamento(&(*r)->esq);
	corrigir_balanceamento(&(*r)->dir);
	int sinal = fator_balanceamento(r);
	if(sinal>1){
		if(fator_balanceamento(&(*r)->esq)>=0){
			//printf("Fazendo Rotação Simples a Direita.\n");
			rsd(r);
		}else{
			//printf("Fazendo Rotação Dupla a Direita.\n");
			rdd(r);
		}
	}else if(sinal<-1){
		if(fator_balanceamento(&(*r)->dir)<=0){
			//printf("Fazendo Rotação Simples a Esquerda.\n");
			rse(r);
		}else{
			//printf("Fazendo Rotação Dupla a Esquerda.\n");
			rde(r);
		}
	}
}

int fator_balanceamento(No **r){
	if(*r==NULL) return 0;
	return height(&(*r)->esq)-height(&(*r)->dir); 
}

int height(No **r){
    int u, v;
    if (*r == NULL) return -1;
    u = height(&(*r)->esq);
    v = height(&(*r)->dir);
    if(u>v) return u+1;
    else return v+1;
}

int busca(No **r, int vlr){
	if(*r==NULL) return 0;
	if(vlr==(*r)->dado) return 1;
	if(vlr>(*r)->dado) return busca(&(*r)->dir,vlr);
	return busca(&(*r)->esq,vlr);
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
			aux = maior_elemento_rec(&(*r)->esq);
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

No *maior_elemento_rec(No **r){
	if((*r)->dir == NULL){
		return *r;
	}
	maior_elemento_rec(&(*r)->dir);
}