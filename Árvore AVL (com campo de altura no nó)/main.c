#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct no
{
	int dado;
    int altura;
	struct no *esq, *dir;
}No;

int menu();
int maximo(No**,No**);
int qtd_elementos(No**);
void inserir(No**,int);
void pre_ordem(No**);
void pos_ordem(No**);
void em_ordem(No**);
void rse(No**);
void rsd(No**);
void rdd(No**);
void rde(No**);
void corrigir_balanceamento(No**);
int fator_balanceamento(No**); 
int nao_folha(No**);
void destruir(No**);
int remover(No**,int);
int maior(No**);
int busca(No**,int);

clock_t tempoOrd,t_ini,t_fim,tempo;

int main(void){
    No **r;
    *r = NULL;
    int op;
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
			case 5:
				printf("Quantidade de nós não folha: %d\n", nao_folha(r));
				break;
            case 6:{
				int dado;
                printf("Informe o nó que deseja remover: \n");
				scanf("%d",&dado);
				printf((remover(r,dado))?"Elemento removido.\n":"Não encontrado.\n");
                break;
			}
			case 7:{
				int dado;
				scanf("%d",&dado);
				printf((busca(r,dado))?"Elemento encontrado.\n":"Não encontrado.\n");
				break;
			}
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
    destruir(r);
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
	printf("[6] Remover\n");
	printf("[7] Buscar\n");
    scanf("%d",&op);
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
        novo->altura = 0;
		novo->esq = NULL;
		novo->dir = NULL;
		*r = novo;
	}else{
        if(vlr>(*r)->dado)
            inserir(&(*r)->dir, vlr);
        else
            inserir(&(*r)->esq, vlr);
        (*r)->altura = maximo(&(*r)->esq, &(*r)->dir)+1;
        corrigir_balanceamento(r);
    }
}

void pre_ordem(No** r){
	if(*r!=NULL){
        printf("---------------------------\n");
		printf("%d\n",(*r)->dado);
        printf("Altura: %d\n", (*r)->altura);
        printf("Fator de balanceamento: %d\n", fator_balanceamento(r));
		printf("---------------------------\n");
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
    //Atualizar altura
    aux->altura = maximo(&aux->esq, &aux->dir)+1;
    (*r)->altura = maximo(&(*r)->esq, &(*r)->dir)+1;
}

void rsd(No **r){
    if(*r==NULL) return;
    No *aux;
    aux = *r;
    *r = (*r)->esq;
    aux->esq = (*r)->dir;
    (*r)->dir = aux;
    //Atualizar altura
    aux->altura = maximo(&aux->esq, &aux->dir)+1;
    (*r)->altura = maximo(&(*r)->esq, &(*r)->dir)+1;
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
	if(*r==NULL) 
        return 0;
    if((*r)->esq==NULL&&(*r)->dir==NULL) 
        return 0;
    if((*r)->esq==NULL) 
        return ((*r)->dir->altura+1)*(-1);
    if((*r)->dir==NULL)
        return ((*r)->esq->altura+1);
	return (*r)->esq->altura-(*r)->dir->altura; 
}

int maximo(No **esq, No **dir){
    if((*esq==NULL)&&(*dir==NULL))
        return -1;
    if(*esq==NULL) 
        return (*dir)->altura;
    if(*dir==NULL)
        return (*esq)->altura;
    if((*esq)->altura>(*dir)->altura)
        return (*esq)->altura;
    return (*dir)->altura;
}

void destruir(No** r){
	if(*r!=NULL){
		destruir(&(*r)->esq);
		destruir(&(*r)->dir);
		free(*r);
		*r = NULL;
	}
}

int remover(No **r, int vlr){
	if(*r==NULL) 
		return 0;
	if((*r)->dado==vlr){
		if((*r)->esq==NULL){
			No *aux = *r;
			*r = (*r)->dir;
			free(aux);
			return 1;
		} 
		if((*r)->dir==NULL){
			No *aux = *r;
			*r = (*r)->esq;
			free(aux);
			return 1;
		}
		(*r)->dado = maior(&(*r)->esq);
		int ret = remover(&(*r)->esq, (*r)->dado);
		if(ret)
			(*r)->altura = maximo(&(*r)->esq, &(*r)->dir)+1;
		return ret;
	}
	int ret;
	if(vlr>(*r)->dado)
		ret = remover(&(*r)->dir, vlr);
	else
		ret = remover(&(*r)->esq, vlr);
	if(ret)
		(*r)->altura = maximo(&(*r)->esq, &(*r)->dir)+1;
	return ret;
}

int maior(No **r){
	if((*r)->dir==NULL)
		return (*r)->dado;
	return maior(&(*r)->dir);
}

int busca(No **r, int vlr){
	if(*r==NULL) return 0;
	if((*r)->dado==vlr) return 1;
	if(vlr>(*r)->dado) return busca(&(*r)->dir,vlr);
	return busca(&(*r)->esq,vlr);
}