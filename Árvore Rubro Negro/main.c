#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum{VERMELHO,PRETO} Cor; //0 vermelho 1 preto

typedef struct no{
    int dado;
    Cor cor;
    struct no *esq, *dir, *pai;
}No;

int menu();
void pre_ordem(No**);
No *inserir(No**,int);
int inserirRB(No**,int);
void destruir(No**);
int remover(No**,int,No**);   
int removerRB(No**,int,No**);
No *maior_elemento(No**);
void correcao(No**,No**);
void corrigir_pai_que_e_filho_esquerdo(No**,No**);
void corrigir_pai_que_e_filho_direito(No**,No**);
void correcao_na_remocao(No**,No**);
void correcao_remocao_filho_esquerdo(No**,No**);
void correcao_remocao_filho_direito(No**,No**);
int irmao_preto_tem_2filhos_pretos(No*);
int irmao_preto_tem_filhoDireito_preto(No*);
No *tem_filho_vermelho(No**);
int isVermelho(No*);
void rse(No**);
void rsd(No**);
int busca(No**,int);

clock_t tempoOrd,t_ini,t_fim,tempo;

int main(){
    No **r;
    *r=NULL;
    int op,n;
    do{
        op=menu();
        switch(op){
            case 0:
                break;
            case 1:{
                printf("Informe os valores: ");
                scanf("%d",&n);
                inserirRB(r,n);
				break;
            }
            case 2:
                printf("Informe o valor a ser removido: ");
                scanf("%d",&n);
                removerRB(r,n,r);
                break;
            case 3:
                pre_ordem(r);
                break;  
            case 7:
                printf("Informe o valor que deseja buscar: ");
                scanf("%d",&n);
                printf((busca(r,n))?"Elemento encontrado.\n":"Não encontrado.\n");
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
            default:
                printf("Opção inválida.\n");
                break;
        }
    }while(op!=0);
    destruir(r);
    return 0;
}

int menu(){
	int op;
    printf("+-------------------------------+\n");
    printf("|              MENU             |\n");
    printf("+-------------------------------+\n");
	printf("|   [0] Sair                    |\n");
	printf("|   [1] Inserir                 |\n");
    printf("|   [2] Remover um nó           |\n");
    printf("|   [3] Pré-Ordem               |\n");
    printf("|   [4] Buscar                  |\n");
    printf("+-------------------------------+\n");
	scanf("%d",&op);
    //system("clear");
	return op;
}

No *inserir(No **r, int vlr){
	if(*r==NULL){
		No *novo = (No*)malloc(sizeof(No));
		novo->dado = vlr;
		novo->esq = NULL;
		novo->dir = NULL;
        novo->pai = NULL;
        novo->cor = VERMELHO;
		*r = novo;
		return novo;
	}else{
        No *retorno;
        if(vlr>(*r)->dado){
            retorno = inserir(&(*r)->dir, vlr);
            //Atualizando o pai do filho que foi adicionado
            (*r)->dir->pai = *r;
        }else if(vlr<(*r)->dado){
            retorno = inserir(&(*r)->esq, vlr);
            //Atualizando o pai do filho que foi adicionado
            (*r)->esq->pai = *r;
        }else 
            return NULL;
        return retorno;
    }
}

int inserirRB(No **r, int vlr){
    No *noInserido;
    noInserido = inserir(r,vlr);
    if(noInserido==NULL)
        return 0;
    correcao(r,&noInserido);
    //Para garantir que a raiz será preta;
    (*r)->cor = PRETO; 
    return 1;
}

void correcao(No **r, No **avaliado){
    while((*avaliado)->pai!=NULL){
        if((*avaliado)->pai->cor==PRETO) 
            break;
        No *pai = (*avaliado)->pai;
        No *avo = (*avaliado)->pai->pai;
        if(avo->esq==pai){ //O pai é um filho esquerdo
            corrigir_pai_que_e_filho_esquerdo(r,avaliado);
        }else{ //O pai é um filho direito
            corrigir_pai_que_e_filho_direito(r,avaliado);
        }
    }
    if((*avaliado)->pai==NULL)
        *r = *avaliado;
}

void corrigir_pai_que_e_filho_esquerdo(No **r, No **avaliado){
    No *pai = (*avaliado)->pai;
    No *avo = pai->pai;
    No *tio = avo->dir;
    if(isVermelho(tio)){ //Caso 1 Pai que é filho esquerdo 
        pai->cor = PRETO;
        avo->cor = VERMELHO;
        tio->cor = PRETO;
        (*avaliado) = avo;
    }else{
        if(pai->dir==(*avaliado)){ //Caso 2 Pai que é filho esquerdo 
            if(avo==NULL){
                rse(&pai);
                *r = pai;
            }else{
                if(avo->esq==pai){
                    rse(&pai);
                    avo->esq = pai;
                }else{
                    rse(&pai);
                    avo->dir = pai;
                }
            }
            (*avaliado) = pai->esq;
        }
        if(avo->pai==NULL){ //Caso 3 Pai que é filho esquerdo
            rsd(&avo);
            *r = avo;
        }else{
            if(avo->pai->esq==avo){
                rsd(&avo);
                avo->pai->esq = avo;
            }else{
                rsd(&avo);
                avo->pai->dir = avo;
            }
        }
        pai->cor = PRETO;
        pai->dir->cor = VERMELHO;
    }
}

void corrigir_pai_que_e_filho_direito(No **r, No **avaliado){
    No *pai = (*avaliado)->pai;
    No *avo = pai->pai;
    No *tio = avo->esq;
    if(isVermelho(tio)){ //Caso 1 Pai que é filho direito 
        pai->cor = PRETO;
        avo->cor = VERMELHO;
        tio->cor = PRETO;
        (*avaliado) = avo;
    }else{
        if(pai->esq==(*avaliado)){ //Caso 2 Pai que é filho direito 
            if(avo==NULL){
                rsd(&pai);
                *r = pai;
            }else{
                if(avo->dir==pai){
                    rsd(&pai);
                    avo->dir = pai;
                }else{
                    rsd(&pai);
                    avo->esq = pai;
                }
            }
            (*avaliado) = pai->dir;
        }
        if(avo->pai==NULL){ //Caso 3 Pai que é filho direito
            rse(&avo);
            *r = avo;
        }else{
            if(avo->pai->dir==avo){
                rse(&avo);
                avo->pai->dir = avo;
            }else{
                rse(&avo);
                avo->pai->esq = avo;
            }
        }
        pai->cor = PRETO;
        pai->esq->cor = VERMELHO;
    }
}

int isVermelho(No *no){
    if(no==NULL) return 0;
    if(no->cor==VERMELHO) return 1;
    return 0;
}

void pre_ordem(No **r){
    if(*r!=NULL){
        if((*r)->cor==0)
            printf("VERMELHO ");
        else
            printf("PRETO ");
        if((*r)->pai==NULL)
            printf("%02d [RAIZ]\n", (*r)->dado);
        else 
            printf("%02d [%02d]\n", (*r)->dado, (*r)->pai->dado);
        pre_ordem(&(*r)->esq);
        pre_ordem(&(*r)->dir);
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

void correcao_na_remocao(No **avaliado, No **r){
    No *aux = *avaliado;
    if(isVermelho(*avaliado))
        return;
    No *filho = tem_filho_vermelho(avaliado);
    if(filho!=NULL){
        filho->cor = PRETO;
    }else{
        No *pai = (*avaliado)->pai;
        while((*avaliado)->pai!=NULL){
            if(pai->esq==*avaliado){
                printf("Correção pai que é filho esquerdo.\n");
                correcao_remocao_filho_esquerdo(avaliado,r);
            }else{
                printf("Correção pai que é filho direito.\n");
                correcao_remocao_filho_direito(avaliado,r);
            }
        }
        *avaliado = aux;
    }
}

void correcao_remocao_filho_esquerdo(No **avaliado, No **r){
    No *irmao = (*avaliado)->pai->dir;
    No *pai = (*avaliado)->pai;
    No *avo = (*avaliado)->pai->pai;
    if(isVermelho(irmao)){
        printf("Caso 1: Remoção - correção filho esquerdo\n");
        if(avo==NULL){
            rse(&pai);
            *r = pai;
        }else if(avo->esq==pai){
            rse(&pai);
            avo->esq = pai;
        }else{
            rse(&pai);
            avo->dir=pai;
        }
        //Atualizando quem é quem 
        pai = pai->esq;
        irmao = pai->dir;
        avo = pai->pai;
        //Corrigindo as cores 
        pai->cor = VERMELHO;
        avo->cor = PRETO;
    }
    if(irmao_preto_tem_2filhos_pretos(irmao)){
        printf("Caso 2: Remoção - irmão preto com dois filhos pretos\n");
        irmao->cor=VERMELHO;
        if(isVermelho(pai)){
            pai->cor=PRETO;
            *avaliado = *r;
        }else{
            *avaliado = pai;
        }
    }else{
        if(irmao->cor==PRETO && !isVermelho(irmao->dir)){
            printf("Caso 3: Remoção - irmão preto com filho direito preto\n");
            if(pai->dir==irmao){
                rsd(&irmao);
                pai->dir=irmao;
            }else{
                rsd(&irmao);
                pai->esq=irmao;
            }
            irmao->cor=PRETO;
            irmao->dir->cor=VERMELHO;
        }
        printf("Caso 4: Remoção - irmão preto com filho direito vermelho\n");
        if(avo==NULL){
            rse(&pai);
            *r = pai;
        }else if(avo->esq==pai){
            rse(&pai);
            avo->esq=pai;
        }else{
            rse(&pai);
            avo->dir=pai;
        }
        //Atualizando quem é quem
        avo = pai;
        pai = pai->esq;
        irmao = pai->dir;
        //Atualizando as cores
        avo->cor = pai->cor;
        pai->cor = PRETO;
        avo->dir->cor = PRETO;
        *avaliado = *r;
    }    
}

void correcao_remocao_filho_direito(No **avaliado, No **r){
    No *irmao = (*avaliado)->pai->esq;
    No *pai = (*avaliado)->pai;
    No *avo = (*avaliado)->pai->pai;
    if(isVermelho(irmao)){
        printf("Caso 1: Remoção - correção filho direito\n");
        if(avo==NULL){
            rsd(&pai);
            *r = pai;
        }else if(avo->dir==pai){
            rsd(&pai);
            avo->dir = pai;
        }else{
            rsd(&pai);
            avo->esq=pai;
        }
        //Atualizando quem é quem 
        pai = pai->dir;
        irmao = pai->esq;
        avo = pai->pai;
        //Corrigindo as cores 
        pai->cor = VERMELHO;
        avo->cor = PRETO;
    }
    if(irmao_preto_tem_2filhos_pretos(irmao)){
        printf("Caso 2: Remoção - irmão preto com dois filhos pretos\n");
        irmao->cor=VERMELHO;
        if(isVermelho(pai)){
            pai->cor=PRETO;
            *avaliado = *r;
        }else{
            *avaliado = pai;
        }
    }else{
        if(irmao->cor==PRETO && !isVermelho(irmao->dir)){
            printf("Caso 3: Remoção - irmão preto com filho direito preto\n");
            if(pai->esq==irmao){
                rse(&irmao);
                pai->esq=irmao;
            }else{
                rse(&irmao);
                pai->dir=irmao;
            }
            irmao->cor=PRETO;
            irmao->dir->cor=VERMELHO;
        }
        printf("Caso 4: Remoção - irmão preto com filho esquerdo vermelho\n");
        if(avo==NULL){
            rsd(&pai);
            *r=pai;
        }else if(avo->dir==pai){
            rsd(&pai);
            avo->dir=pai;
        }else{
            rsd(&pai);
            avo->esq=pai;
        }
        //Atualizando quem é quem
        avo = pai;
        pai = pai->dir;
        irmao = pai->esq;
        //Atualizando as cores
        avo->cor = pai->cor;
        pai->cor = PRETO;
        avo->esq->cor = PRETO;
        *avaliado = *r;
    }    
}

int irmao_preto_tem_2filhos_pretos(No *r){
    if(r==NULL){
        return 0;
    }
    if(r->cor!=PRETO) return 0;
    if(r->esq==NULL&&r->dir==NULL)
        return 1;
    if(r->esq==NULL||r->dir==NULL){
        if(r->esq==NULL){
            if(r->dir->cor==PRETO)
                return 1;
        }else{
            if(r->esq->cor==PRETO)
                return 1;
        }
        return 0;
    }
    if(r->esq->cor==PRETO&&r->dir->cor==PRETO)
        return 1;
    return 0;
}

No *tem_filho_vermelho(No **r){
    if(*r==NULL){
        printf("Caso estranho acontecendo na função 'tem_filho_vermelho'... Chamar debug.\n");
        exit(1);
    }
    if((*r)->esq==NULL&&(*r)->dir==NULL)
        return NULL;
    if((*r)->dir==NULL){
        if((*r)->esq->cor==VERMELHO)
            return (*r)->esq;
    }
    if((*r)->esq==NULL){
        if((*r)->dir->cor==VERMELHO)
            return (*r)->dir;
    }
    return NULL;
}

int removerRB(No **r, int valor, No **raiz){
    return remover(r,valor,raiz);
}

int remover(No **R, int valor,No **raiz){
	if (*R == NULL)
		return 0;
	if ((*R)->dado == valor){
		No *aux;
		if ((*R)->esq == NULL){
            aux = *R;
			correcao_na_remocao(R,raiz);
            *R = (*R)->dir;
			//Atualizar o pai se necessário
			if (*R != NULL){
				(*R)->pai = aux->pai;
			}
			free(aux);
			return 1;
		}
		if ((*R)->dir == NULL){
            aux = *R;
            correcao_na_remocao(R,raiz);
            *R = (*R)->esq;
			//Atualizar o pai se necessário
			if (*R != NULL){
				(*R)->pai = aux->pai;
			}

			free(aux);
			return 1;
		}
        aux = maior_elemento(&((*R)->esq));
		(*R)->dado = aux->dado; 
		return (remover(&((*R)->esq), (*R)->dado,raiz));		
	}
	int ret;
	if (valor < (*R)->dado)
		ret = (remover(&( (*R)->esq ),valor,raiz));
	else
		ret = (remover(&( (*R)->dir ),valor,raiz));
	return ret;		
}

No *maior_elemento(No **r){
    if(*r==NULL) 
        return NULL;
    if((*r)->dir==NULL)
        return *r;
    maior_elemento(&(*r)->dir);
}

void rse(No **r){
    if(*r==NULL){
        printf("..(ERRO FATAL) Não foi possível executar a rotação..\n");
        exit(1);
    }
    No *aux;
    aux = *r;
    *r = (*r)->dir;
    aux->dir = (*r)->esq;
    (*r)->esq = aux;
    //Atualizando o pai 
    (*r)->pai = aux->pai;
    aux->pai = *r;
    if(aux->dir!=NULL)
        aux->dir->pai = aux;
}

void rsd(No **r){
    if(*r==NULL){
        printf("..(ERRO FATAL) Não foi possível executar a rotação..\n");
        exit(1);
    }
    No *aux;
    aux = *r;
    *r = (*r)->esq;
    aux->esq = (*r)->dir;
    (*r)->dir = aux;
    //Atualizando o pai 
    (*r)->pai = aux->pai;
    aux->pai = *r;
    if(aux->esq!=NULL)
        aux->esq->pai = aux;
}

int busca(No **r, int vlr){
    if(*r==NULL) return 0;
    if((*r)->dado==vlr) return 1;
    if(vlr>(*r)->dado) return busca(&(*r)->dir,vlr);
    return busca(&(*r)->esq,vlr);
}