/*bibliotecas*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAXCHAVES 3 /*num  maximo de chaves que significa ordem 4 portanto maxkey = 4 - 1 */
#define MINCHAVES 1 /* minimo de chaves que e MAXCHAVES/2 */
#define NULO -1 
#define NO   "@@@@@@@@@@@@@" 
#define NAO   0 
#define SIM   1
#define TAM_STRUCT 100 
 
/* structs */
/* struct da arvore b */
typedef struct pag{ 
	int cont;          		 	/* cont de chaves na página */
    char chave[MAXCHAVES][14];  /* chave atual */
    int filhos[MAXCHAVES+1];	/* ponteiro para os rrn filhos | o num de filhos e igual a ordem entao maxchaves + 1 */
    int offset[MAXCHAVES]; 		/* offsets dos registros  */
}BTpagina; 

/* arquivo biblioteca.bin */
typedef struct tipo_livro{
  char isbn[14];
  char titulo[50];
  char autor[50];
  char ano[5];
}livro;
livro arq_livros[TAM_STRUCT];

/* arquivo de busca_arq.bin */
struct busca_arquivo{
	char isbn[14];
}arq_busca[TAM_STRUCT];

/* arquivo de busca casada.bin*/
struct busca_casada{
	char isbn1[14];
	char isbn2[14];
}arq_consulta_casada[TAM_STRUCT];

/* arquivo remove.bin */
struct busca_remove{
	char isbn[14];
}arq_remove[TAM_STRUCT];

#define TAMPAG sizeof(BTpagina) 

/* variaveis globais */ 
int raiz;    /* rrn da raiz */
FILE *btfd;  /* arquivo da arvore b */
FILE *infd;  /* arquivo da biblioteca */
FILE *arq;	 /* outros arquivos */
FILE *arq1;	 /* arquivo da consulta casada */
FILE *arq2;  /* arquivo da consulta casada */

/* variaveis globais */
char atualizar[] = "ab", leitura[] = "rb+", escrever[] = "wb";
int qtd_chave_s = 0, tam_vet_inserir = 0, cont_buscas = 0, cont_consulta_casada = 0, cont_remove = 0;
 
/* prototipos */ 
void split(char chave[], int offset, int r_filho, BTpagina *p_pag_antiga, char promo_chave[], int *promo_offset,int *promo_r_filho, BTpagina *p_nova_pag);
void redistribuicao(int pos_reg_pai, BTpagina *pai, int pos_pag_irmao, BTpagina *irmao, BTpagina *alvo, int tipo_redist); 
void substituir_elemento_caso2(int pos_pagina, int pos_registro, char sucessor[], int offset_sucessor);
void esta_na_pagina (char chave[], int offset, int r_filho, BTpagina *p_pag);
void remover_caso1(int pos_registro, int pos_pagina, BTpagina *p_pag); 
void escrever_arquivo (int rrn, BTpagina *ponteiro_pag);
void reorganiza_pai(BTpagina *pai, int pos_pai);
void inicializar_pagina (BTpagina *p_pag); 
void exibir_pagina(BTpagina pagina);
void fechar_arquivo (FILE **p_arq);
void exibir_registro(int offset);
void carregar_arquivo(int resp);
void inserir_raiz(int raiz); 
void consulta_casada();
void match();
void merge();
int buscar();
int pegar_raiz (); 
int pegar_pagina ();
int remover_usuario();
int inserir_arq_principal();
int percorrer_arvore(int raiz);
int criar_arquivo(char nome_arq[]);
int criar_arvore(char chave[], int offset);
int pesquisa_maiores(int rrn, char valor[]);
int pesquisa_menores(int rrn, char valor[]);
int procurar_no(char chave[], BTpagina *p_pag, int *pos);
int abrir_arquivo (char nome_arq[], char tipo_abertura[]); 
int inserir_arq_indice(char isbn[], int offset, int cont_insercao);
int criar_raiz(char chave[], int offset, int esquerda, int direita);
int remover(char isbn[], int raiz, int pos_pagina, int pos_registro, int offset);
int buscar_isbn(char chave[], int raiz, int *pos_pagina, int *pos_registro, int *offset);
int achar_sucessor_imediato(int rrn, char sucessor[], int *offset_sucessor, int *pos_pag_suc, int *pos_reg_suc);
int inserir_arvoreB(int rrn, char chave[], int offset, int *promo_r_filho, char promo_chave[], int *promo_offset);
int achar_pag_irmas(int pos_pagina, int raiz, char isbn[], int *pos_pag_pai, int *pos_reg_pai, int *pos_pag_irmaE, int *pos_pag_irmaD);
int concatenacao(int pos_reg_pai, int pos_pag_pai, BTpagina *pai, int pos_pag_irmaE, BTpagina *irmaE, int pos_pag_irmaD, BTpagina *irmaD, int pos_pag_alvo, BTpagina *pag_alvo);
BTpagina ler_arquivo (int rrn, BTpagina *ponteiro_pag); 

/* menu */
int main(){     
	int resp, sair = 0;
	char arq_livros[]="livros.bin", arq_arvoreB[]="btree.bin";
	do{ 
		system("cls");
	 	printf("\n        Menu");
	  	printf("\n1 - Insercao");
	  	printf("\n2 - Listar Livros");
	  	printf("\n3 - Procurar Livro");
	  	printf("\n4 - Consulta Casada");
	  	printf("\n5 - Remover Livro");
	  	printf("\n6 - Carregar Arquivos");
	  	printf("\n7 - Sair");
	  	printf("\nOpcao: ");
		scanf("%d",&resp);
		switch(resp){
			case 1:{
				inserir_arq_principal();	
				break;
			}
		  	case 2:{
		  		system("cls");
		  		printf("Livros em ordem crescente de ISBN");
		  		abrir_arquivo(arq_arvoreB, leitura);
				int raiz = pegar_raiz();
		  		percorrer_arvore(raiz);
		  		fclose(btfd);
				getch();
				break;
			}
		  	case 3:{
				buscar();
	  			break;
	  		}
	  		case 4:{
	  			consulta_casada();
		  		break;
			}
	  		case 5:{
				remover_usuario();
				break;
			}
	  		case 6:{
	  			resp=0;
			    carregar_arquivo(resp); 
			    printf("\nArquivos Carregados");
			    getch();
		    	break;
			}
			case 7:{
				sair = 1;
				break;
			}
		  	default:{
			    printf("\nOpcao invalida!");
			    getch();
				break;
			} 
		}
	}while(sair != 1);
	
	fclose(btfd);
	fclose(infd);
	fclose(arq);
}   

/* inserir no arquivo principal */
int inserir_arq_principal(){
	char nome_arq[] = "livros.bin", bt_arq[]="btree.bin";
	int cont_insercao, pos_registro, pos_pagina, offset, achou = 0;
	livro lvr;
	
	system("cls");
	
	if(!abrir_arquivo(nome_arq, leitura)){
		criar_arquivo(nome_arq);
	}
	
	fread(&cont_insercao, sizeof(int), 1, arq);
	printf("\nContador insercao: %d", cont_insercao);
	fclose(arq);
	
	if(cont_insercao < tam_vet_inserir){
		
		if(!abrir_arquivo(nome_arq, atualizar)){
			printf("\nErro, arquivo nao pode ser aberto");
			getch();
			return 0;
		}
		
		/* buscando chave para ver se ha chave duplicada 
		   para a primeira inserção não é necessario a busca */
		if(cont_insercao > 0){
			abrir_arquivo(bt_arq, leitura);
			achou = buscar_isbn(arq_livros[cont_insercao].isbn, pegar_raiz(), &pos_pagina, &pos_registro, &offset);
			fclose(btfd);
		}
		
		if(!achou){	/* verifica se achou o isbn */
			/* inserindo livro no arquivo */
			fwrite(&arq_livros[cont_insercao], sizeof(livro), 1, arq);
			pos_registro =  ftell(arq) - sizeof(livro);	 /* pegando posição do registro */
			printf("\nArquivo com chave %s inserido com sucesso no arquivo de dados", arq_livros[cont_insercao].isbn);
			fclose(arq);
			
			/* inserindo no arquivo de indice */
			inserir_arq_indice(arq_livros[cont_insercao].isbn, pos_registro, cont_insercao);
		}else{
			printf("\nISBN %s duplicado", arq_livros[cont_insercao].isbn);
		}
		
		/* Atualizando contador de inserção */
		abrir_arquivo(nome_arq, leitura);    
		fseek(arq, 0, 0);
		cont_insercao++;
		fwrite(&cont_insercao, sizeof(int), 1, arq);
		fclose(arq);
	}else{
		printf("\nNao ha mais livros a serem inseridos\n");
	}	
	getch();
}

/* inserir no arquivo de indice */
int inserir_arq_indice(char chave[], int offset, int cont_insercao){
	int promovido,		/* verifica se chave foi promovida */
     	raiz, i=0,   	/* rrn da pagina raiz */
		promo_rrn,  	/* rrn promovido debaixo */
		promo_offset;
    char promo_chave[14], bt_arq[]="btree.bin";
	
	if(abrir_arquivo(bt_arq, leitura) == 1){ 
	    raiz = pegar_raiz(); 
	} 
	else { 
		printf("\n\nArquivo de indice nao existe, criando arquivo e arvore.");  
	    raiz = criar_arvore(chave, offset);
	}   
	
	/* Se for a primeira inserção o isbn já foi registrado, assim, pula o código de baixo */
	if(cont_insercao != 0){
		getch();	 
		/* inserindo chave e offset na arvoreB */
		printf("\n\ninserindo na Arvore B");
		printf("\nInserindo a chave %s", chave);
	    promovido = inserir_arvoreB(raiz, chave, offset, &promo_rrn, promo_chave, &promo_offset); 
	    if (promovido){
		    raiz = criar_raiz(promo_chave, promo_offset, raiz, promo_rrn);
		}
	}
	fechar_arquivo(&btfd); 					
}

/* insercao recursiva na arvore b */
int inserir_arvoreB(int rrn, char chave[], int offset, int *promo_r_filho, char promo_chave[], int *promo_offset){ 
	BTpagina pagina,		/* pagina atual */
           nova_pagina;    	/* nova pagina caso exista split */
	int achou, promovido;  	/* valores booleanos */
    int pos, 
	    p_b_rrn,			/* rrn promovido da recursão abaixo */
		p_b_offset;        	/* offset promovido da recursão abaixo */
    char p_b_chave[14];     /*chave promovido da recursão abaixo */
        
    if(rrn == NULO){   
    	strcpy(promo_chave, chave);
		*promo_offset = offset; 
        *promo_r_filho = NULO; 
        return(SIM); 
    } 
    ler_arquivo(rrn, &pagina); 
    /* verifica se existe chave duplicada */
    achou = procurar_no(chave, &pagina, &pos);     
    
    if(achou){ 
    	printf("Erro: chave %s duplicada\n\007", chave); 
        return(0); 
    } 
    promovido = inserir_arvoreB(pagina.filhos[pos], chave, offset, &p_b_rrn, p_b_chave, &p_b_offset); 
    if(!promovido){ 
    	return(NAO); 
	}
	if(pagina.cont < MAXCHAVES){ 
    	esta_na_pagina(p_b_chave, p_b_offset, p_b_rrn, &pagina); 
        escrever_arquivo(rrn, &pagina);
        printf("\nChave %s inserida com sucesso", p_b_chave);
        return(NAO); 
    }else{
		split(p_b_chave, p_b_offset, p_b_rrn, &pagina, promo_chave, promo_offset, promo_r_filho, &nova_pagina);		
		escrever_arquivo(rrn, &pagina); 
	    escrever_arquivo(*promo_r_filho, &nova_pagina);
		return(SIM); 
	} 
} 
  
/* abre arquivos */ 
int abrir_arquivo(char nome_arq[], char tipo_abertura[]) { 
	if(strcmp(nome_arq, "btree.bin") == 0){
		if((btfd = fopen(nome_arq, tipo_abertura)) == NULL)
			return 0;
	    return 1;
	}
	if((arq = fopen(nome_arq, tipo_abertura)) == NULL)
			return 0;
	return 1;	 
} 
 
/* fecha arquivos */ 
void fechar_arquivo(FILE **p_arq){ 
	fclose(*p_arq); 
} 
 
/* pega pagina raiz da arvore B */
int pegar_raiz() { 
	fseek(btfd, 0, 0); 
    if(fread(&raiz,sizeof(int),1, btfd) == NULL) { 
    	printf("Erro: Impossivel pegar raiz. \007\n"); 
        exit(1); 
	}
    return(raiz); 
} 
 
/* insere o rrn da raiz no inicio */ 
void inserir_raiz(int raiz){ 	
	fseek(btfd, 0, 0); 
    fwrite(&raiz, sizeof(raiz),1,btfd); 
} 

/* cria a arquivo da raiz */ 
int criar_arvore(char chave[], int offset){ 
	char bt_arq[]="btree.bin";
	btfd = fopen(bt_arq,escrever); 
	printf("\narquivo criado");
	inserir_raiz(-1);  									
	fclose (btfd); 
	getch();
	abrir_arquivo(bt_arq,escrever); 
	return (criar_raiz(chave, offset, NULO, NULO)); 
} 

/* pega pagina */ 
int pegar_pagina(){ 
	int addr; 
	fseek(btfd, 0, 2);
	addr = ftell(btfd) + 4; 
	return (addr/TAMPAG); 
} 
 
/* le uma pagina trazendo pra struct */ 
BTpagina ler_arquivo(int rrn, BTpagina *ponteiro_pag){ 
	int addr; 
	addr  = (rrn * TAMPAG) + 4; 	
	fseek(btfd, addr, 0); 
	fread(ponteiro_pag, sizeof(BTpagina), 1, btfd);
	return *ponteiro_pag;
} 

/* grava uma pagina no arquivo */ 
void escrever_arquivo(int rrn, BTpagina *ponteiro_pag) { 
	int addr; 
	addr = (rrn * TAMPAG) + 4; 
	fseek(btfd, addr, 0); 
	fwrite(&*ponteiro_pag,TAMPAG, 1,btfd); 
}                  

/* cria pagina da raiz */
int criar_raiz(char chave[], int offset, int esquerda, int direita) { 
   	BTpagina pagina; 
   	int rrn; 
   	rrn = pegar_pagina();
   	inicializar_pagina(&pagina); 
   	strcpy(pagina.chave[0],chave); 
   	pagina.offset[0] = offset;						
   	pagina.filhos[0] = esquerda; 
   	pagina.filhos[1] = direita; 
   	pagina.cont = 1; 
   	escrever_arquivo(rrn, &pagina); 
   	inserir_raiz(rrn); 
   	printf("\nRaiz criada com a chave %s", chave);
   	return(rrn); 
} 
 
/* inicializa pagina da arvore B com valores nulos */ 
void inicializar_pagina(BTpagina *p_pag) { 
	int j; 
	for(j = 0; j < MAXCHAVES; j++){ 
	   strcpy(p_pag->chave[j],NO); 
	   p_pag->filhos[j] = NULO; 
	   p_pag->offset[j] = NULO;
	} 
	p_pag->filhos[MAXCHAVES] = NULO; 
} 

/* procura no no arquivo */ 
int procurar_no(char chave[], BTpagina *p_pag, int *pos){ 
    int i; 
    for(i = 0; i < p_pag->cont && (strcmp(chave,p_pag->chave[i])>0); i++);
    *pos = i; 
    if(*pos < p_pag->cont && (strcmp(chave,p_pag->chave[*pos])==0)){ 
    	return(SIM); 
    } 
    return(NAO); 
}

/* verifica se chave esta na pagina */              
void esta_na_pagina(char chave[], int offset, int r_filho, BTpagina *p_pag) { 
	int j; 
	for(j = p_pag->cont; (strcmp(chave, p_pag->chave[j-1]) < 0) && j > 0; j--){ 
		strcpy(p_pag->chave[j],p_pag->chave[j-1]); 
		p_pag->offset[j] = p_pag->offset[j-1];
	    p_pag->filhos[j+1] = p_pag->filhos[j]; 
	} 
	p_pag->cont++; 
	strcpy(p_pag->chave[j], chave); 
	p_pag->offset[j] = offset;
	p_pag->filhos[j+1] = r_filho; 
} 

/* divide pagina quando necessario */ 
void split(char chave[], int offset, int r_filho, BTpagina *p_pag_antiga, char promo_chave[], int *promo_offset,
				 int *promo_r_filho, BTpagina *p_nova_pag){ 
    int j; 
    int mid; 
	char vet_chave[MAXCHAVES+1][14]; 
	int vet_filhos[MAXCHAVES+2];
	int vet_offset[MAXCHAVES+1];
    printf("\nDivisao de no\n");
	/* coloca os elementos da pagina no vetor temporario */
    for(j = 0; j < MAXCHAVES; j++){ 
    	strcpy(vet_chave[j], p_pag_antiga->chave[j]); 
        vet_filhos[j] = p_pag_antiga->filhos[j];
        vet_offset[j] = p_pag_antiga->offset[j];
	}
    vet_filhos[3] = p_pag_antiga->filhos[3];
    
    /* reoordena e encontra o lugar em que a nova chave[] deve ser inserida */
    for(j = MAXCHAVES; (strcmp(chave, vet_chave[j-1]) < 0) && j > 0; j--){ 
        strcpy(vet_chave[j], vet_chave[j-1]); 
        vet_filhos[j+1] = vet_filhos[j];
        vet_offset[j] = p_pag_antiga->offset[j-1];
    }
	/* insere chave[] onde deve ficar e coloca chave[] antiga no filho */
    strcpy(vet_chave[j], chave); 
    vet_offset[j] = offset;
    vet_filhos[j+1] = r_filho; 
	
	/* cria nova pagina com o promovido */
	(*promo_r_filho) = pegar_pagina(); 
	inicializar_pagina(p_nova_pag);
	inicializar_pagina(p_pag_antiga);
	
	/* coloca as paginas nos devidos lugares */
	p_pag_antiga->cont = 2; 
	strcpy(p_pag_antiga->chave[0],vet_chave[0]);
	strcpy(p_pag_antiga->chave[1],vet_chave[1]);
	  
	p_pag_antiga->offset[0] = vet_offset[0];
	p_pag_antiga->offset[1] = vet_offset[1];
	  
	p_pag_antiga->filhos[0] = vet_filhos[0]; 	
	p_pag_antiga->filhos[1] = vet_filhos[1]; 	
	p_pag_antiga->filhos[2] = vet_filhos[2];
	
	strcpy(p_pag_antiga->chave[2],NO);
	p_pag_antiga->offset[2] = NULO;            
   
   	/* faz nova pagina receber o terceiro no que e o promovido */
   	p_nova_pag->cont = MINCHAVES;
   	strcpy(p_nova_pag->chave[0],vet_chave[3]);
	p_nova_pag->offset[0] = vet_offset[3]; 
	p_nova_pag->filhos[0] = vet_filhos[3];
	p_nova_pag->filhos[1] = vet_filhos[4]; 
	
	strcpy(promo_chave, vet_chave[2]);
	*promo_offset = vet_offset[2];
	printf("\nchave %s promovida", promo_chave);
}

/* carrega todos arquivos */ 
void carregar_arquivo(int resp){
	char cadastro_arq[]="biblioteca.bin", busca_arq[]="busca.bin", consulta_casada_arq[]="consulta_casada.bin",
		 remove_arq[]= "remove.bin";
  	int i, todos=0;
  
  	system("cls");
  	if(resp != 0){
  		todos = 1;
  	}
  	/* abre arquivo biblioteca, carrega em vetor de struct */
  	if(resp == 1 || todos == 0){
		if(abrir_arquivo(cadastro_arq, leitura)){
	  		i=0;
	    	while(fread(&arq_livros[i], sizeof(livro), 1, arq)){
	    		printf("\nIsbn: %s - Titulo: %s - Autor: %s - Ano: %s", arq_livros[i].isbn, arq_livros[i].titulo, 
						arq_livros[i].autor, arq_livros[i].ano);
	      		i++;  
	    	}
	    fechar_arquivo(&arq);
	  	tam_vet_inserir = i;  
	  }
  	}
  	/* abre arquivo busca.bin, carrega em vetor de struct */
  	printf("\n\ndados da busca:");
  	if(resp == 2 || todos == 0){
		if(abrir_arquivo(busca_arq, leitura)){
		    i=0;
			while(fread(&arq_busca[i], sizeof(struct busca_arquivo), 1, arq)){
				printf("\nIsbn: %s", arq_busca[i].isbn);
				i++;
			}
			cont_buscas = i;
			fechar_arquivo(&arq);
		}
  	} 
  	/* abre arquivo consulta_casada.bin, carrega em vetor de struct */
  	printf("\n\nDados consulta casada");
  	if(resp == 3 || todos == 0){
	  	if(abrir_arquivo(consulta_casada_arq, leitura)){
	    	i=0;
			while(fread(&arq_consulta_casada[i], sizeof(struct busca_casada), 1, arq)){
			  printf("\nIsbn CC 1: %s", arq_consulta_casada[i].isbn1);//FUNCIONA?
			  printf("\nIsbn CC 2: %s", arq_consulta_casada[i].isbn2);
			  i++;
			}
			cont_consulta_casada = i;
			fechar_arquivo(&arq);
	  	}
  	}
  	/* abre arquico remover.bin, carrega em vetor de struct */
  	printf("\n\nDados remover");
  	if(resp == 4 || todos == 0){
	  	if(abrir_arquivo(remove_arq, leitura)){
	    	i=0;
			while(fread(&arq_remove[i], sizeof(struct busca_remove), 1, arq)){
			  printf("\nIsbn CC 1: %s", arq_remove[i]);
			  i++;
			}
			cont_remove = i;
			fechar_arquivo(&arq);
	  	}
  	}
}

/* cria arquivo de livros com os contadores */
int criar_arquivo(char nome_arq[]){
	int cont = 0;
	
	abrir_arquivo(nome_arq, escrever);
	fwrite(&cont, sizeof(int), 1, arq);  /* contador de inserção 0 */
	fwrite(&cont, sizeof(int), 1, arq);  /* contador de buscas 	4 */
	fwrite(&cont, sizeof(int), 1, arq);  /* contador de consulta casada 8 */
	fwrite(&cont, sizeof(int), 1, arq);  /* contador de remoção 12 */
	fclose(arq);
	printf("\nArquivo Criado");
}

/* percorre a arvore em ordem crescente de isbn */
int percorrer_arvore(int raiz){
	BTpagina pagina;
	int i;
	
	if(raiz != NULO)
		ler_arquivo(raiz, &pagina);
	else
		pagina.cont = 1;	
		
	for(i = 0; i < pagina.cont; i++){
		if(raiz == NULO) return 0;
		percorrer_arvore(pagina.filhos[i]);
		exibir_registro(pagina.offset[i]);	
	}	
	percorrer_arvore(pagina.filhos[i]);
	return 1;
}

/* exibe o registro que está na posição passada */
void exibir_registro(int offset){
	char arq_livro[]="livros.bin";
	livro lvr;
	abrir_arquivo(arq_livro, leitura);
	fseek(arq, offset, 0);
	fread(&lvr, sizeof(livro), 1, arq);
	printf("\n%s | %s | %s | %s", lvr.isbn, lvr.titulo, lvr.autor, lvr.ano);
	fclose(arq);
}

/* exibe os isbn de uma pagina */
void exibir_pagina(BTpagina pagina){
	for(int i = 0; i < pagina.cont; i++){
		printf("\nIsbn: %s  || offset: %d || esq: %d || dir: %d || cont : %d", pagina.chave[i], pagina.offset[i] 
			   , pagina.filhos[i], pagina.filhos[i+1], pagina.cont);
	}
}

/* efetua a busca de um registro dado o isbn */
int buscar(){	
	char arq_livro[]="livros.bin", bt_arq[]="btree.bin";
	int cont, raiz, pos_pagina = -1, pos_registro = -1, offset = -1, achou; 
	system("cls");
	abrir_arquivo(arq_livro, leitura);
	fseek(arq, 4,0);
	fread(&cont, sizeof(int), 1, arq);
	printf("\nContador: %d", cont);
	fclose(arq);
	
	if(cont < cont_buscas){
		abrir_arquivo(bt_arq, leitura);
		raiz = pegar_raiz();
		printf("\nProcurando chave %s", arq_busca[cont].isbn);
		achou = buscar_isbn(arq_busca[cont].isbn, raiz, &pos_pagina, &pos_registro, &offset);
		fclose(btfd);
		
		if(achou){
			printf("\n\nChave %s encontrada, pagina %d, posicao %d", arq_busca[cont].isbn, pos_pagina, pos_registro);
			printf("\n\nLivro:");
			exibir_registro(offset);
		}else{
			printf("\n\nChave %s nao encontrada", arq_busca[cont].isbn);
		}
		
		/* atualizando contador de buscas */
		abrir_arquivo(arq_livro, leitura);
		fseek(arq, 4, 0);
		cont++;
		fwrite(&cont, sizeof(int), 1, arq);
		fclose(arq);
	}else{
	  	printf("\n\nNao ha mais buscas");
	}
	getch();
}

/* efetua busca recursiva na arvore */
int buscar_isbn(char chave[], int raiz, int *pos_pagina, int *pos_registro, int *offset){
	BTpagina pagina;
	int achou, posicao;
	
	if(raiz == NULO) return NAO;
	
	ler_arquivo(raiz, &pagina);
	
	if(procurar_no(chave, &pagina, &posicao)){
		*pos_pagina = raiz;
		*pos_registro = posicao;
		*offset = pagina.offset[posicao];
		return SIM;
	}
	achou = buscar_isbn(chave, pagina.filhos[posicao], pos_pagina, pos_registro, offset);
	return achou;
}

/* faz a busca casada */
void consulta_casada(){
	char isbn2[14], arq_livro[]="livros.bin", bt_arq[]="btree.bin";;
	int resp, cont, raiz;
	
	arq1 = fopen("arq1.bin","w+b");
	arq2 = fopen("arq2.bin","w+b");
	
	system("cls");
	abrir_arquivo(arq_livro, leitura);
	fseek(arq, 8,0);
	fread(&cont, sizeof(int), 1, arq);
	printf("\nContador: %d", cont);
	fclose(arq);
	
	if(cont < cont_consulta_casada){
		abrir_arquivo(bt_arq, leitura);
		raiz = pegar_raiz();
		
		
		printf("\nValores maiores que: %s -> ", arq_consulta_casada[cont].isbn1);
		pesquisa_maiores(raiz, arq_consulta_casada[cont].isbn1);
		
		printf("\nValores menores que: %s -> ", arq_consulta_casada[cont].isbn2);
		pesquisa_menores(raiz, arq_consulta_casada[cont].isbn2);
		fclose(btfd);
		
		printf("\n Deseja fazer 1-match ou 2-merge?");
		scanf("%d",&resp);
			
		if(resp == 1){
			printf("\n Matching -> ");
			match();
		}else{
			printf("\n Merging -> ");
			merge();
		}
		/* atualizando contador de busca casada */
		abrir_arquivo(arq_livro, leitura);
		fseek(arq, 8, 0);
		cont++;
		fwrite(&cont, sizeof(int), 1, arq);
		fclose(arq);
	}else{
	  	printf("\n\nNao ha mais buscas");
	}
	getch();
	fclose(arq1);
	fclose(arq2);
	remove("arq1.bin");
	remove("arq2.bin");
}

/* pesquisa isbn maiores que o isbn dado */
int pesquisa_maiores(int raiz, char valor[]){
	BTpagina pagina;
	int i;
	
	if(raiz != NULO)
		ler_arquivo(raiz, &pagina);
	else
		pagina.cont = 1;	
		
	for(i = 0; i < pagina.cont; i++){
		if(raiz == NULO) return 0;
		pesquisa_maiores(pagina.filhos[i],valor);
		if(!strcmp(pagina.chave[i],NO)==0){
			if((strcmp(pagina.chave[i],valor)>0) || (strcmp(valor,pagina.chave[i])==0)){
				printf(" %s ", pagina.chave[i]);
				fwrite(pagina.chave[i],sizeof(char),14,arq1);
			}
		}	
	}	
	pesquisa_maiores(pagina.filhos[i],valor);
	return 1;
}

/* pesquisa isbn menores que o isbn dado */
int pesquisa_menores(int raiz, char valor[]){
	BTpagina pagina;
	int i;
	
	if(raiz != NULO)
		ler_arquivo(raiz, &pagina);
	else
		pagina.cont = 1;	
		
	for(i = 0; i < pagina.cont; i++){
		if(raiz == NULO) return 0;
		pesquisa_menores(pagina.filhos[i],valor);
		if(!strcmp(pagina.chave[i],NO)==0){
			if((strcmp(pagina.chave[i],valor)<0) || (strcmp(valor,pagina.chave[i])==0)){
				printf(" %s ", pagina.chave[i]);
				fwrite(pagina.chave[i],sizeof(char),14,arq2);
			}
		}	
	}	
	pesquisa_menores(pagina.filhos[i],valor);
	return 1;
}

/* faz o merge com os isbns */
void merge(){
	char isbn_arq1[14], isbn_arq2[14], maior_nome[14]="9999999999999", menor_nome[14]="0000000000000";
	int fim_arq1 = 0, fim_arq2 = 0;
	
	fseek(arq1,0,0);
	fseek(arq2,0,0);
	arq = fopen("merge.bin","w+b");
	
	fread(isbn_arq1,sizeof(char),14,arq1);
	fread(isbn_arq2,sizeof(char),14,arq2);
	
	do{
		if(strcmp(isbn_arq1,isbn_arq2)<0){
			printf(" %s ",isbn_arq1);
			fwrite(isbn_arq1,sizeof(char),14,arq);
			if(fread(isbn_arq1,sizeof(char),14,arq1));
			else fim_arq1 = 1;
		}
		else if(strcmp(isbn_arq1,isbn_arq2)>0){
			printf(" %s ",isbn_arq2);
			fwrite(isbn_arq2,sizeof(char),14,arq);
			if(fread(isbn_arq2,sizeof(char),14,arq2));
			else fim_arq2 = 1;
		}
		else{
			printf(" %s ",isbn_arq1);
			fwrite(isbn_arq1,sizeof(char),14,arq);
			if(fread(isbn_arq1,sizeof(char),14,arq1));
			else fim_arq1 = 1;
			
			if(fread(isbn_arq2,sizeof(char),14,arq2));
			else fim_arq2 = 1;
		}
	}while(!fim_arq1 && !fim_arq2);
	
	if(!fim_arq2/*fread(isbn_arq2,sizeof(char),14,arq2)*/){
		fseek(arq2,-14,1);
		while(fread(isbn_arq2,sizeof(char),14,arq2)){
			printf(" %s ",isbn_arq2);
			fwrite(isbn_arq2,sizeof(char),14,arq);
		}
	}
	if(!fim_arq1/*fread(isbn_arq1,sizeof(char),14,arq1)*/){
		fseek(arq1,-14,1);
		while(fread(isbn_arq1,sizeof(char),14,arq1)){
			printf(" %s ",isbn_arq1);
			fwrite(isbn_arq1,sizeof(char),14,arq);
		}
	}
	fclose(arq);	
}

/* faz o match com os isbns */
void match(){
	char isbn_arq1[14], isbn_arq2[14];
	int fim_arq1 = 0, fim_arq2 = 0;
	
	fseek(arq1,0,0);
	fseek(arq2,0,0);
	arq = fopen("match.bin","w+b");
	
	fread(isbn_arq1,sizeof(char),14,arq1);
	fread(isbn_arq2,sizeof(char),14,arq2);
	
	do{
		if(strcmp(isbn_arq1,isbn_arq2)<0){
			if(fread(isbn_arq1,sizeof(char),14,arq1));
			else fim_arq1 = 1;
		}
		else if(strcmp(isbn_arq1,isbn_arq2)>0){
			if(fread(isbn_arq2,sizeof(char),14,arq2));
			else fim_arq2 = 1;
			
		}
		else{
			printf(" %s ",isbn_arq1);
			fwrite(isbn_arq1,sizeof(char),14,arq);
			if(fread(isbn_arq1,sizeof(char),14,arq1));
			else fim_arq1 = 1;
			
			if(fread(isbn_arq2,sizeof(char),14,arq2));
			else fim_arq2 = 1;
			
		}
	}while(!fim_arq1 && !fim_arq2);
	
	fclose(arq);
}

/* trata o arquivo remover.bin */
int remover_usuario(){
	int pos_registro, pos_pagina, offset, achou, raiz, cont;
	char isbn[14], livros_arq[] = "livros.bin", BT_arq[] = "btree.bin";
	system("cls");	   
	if(!abrir_arquivo(BT_arq, leitura)){
		printf("\n Arquivo da Arvore B nao existe");
	}
	if(!abrir_arquivo(livros_arq, leitura)){
		printf("\n Arquivo da Biblioteca nao existe");
	}
	fseek(arq,12,0);
	fread(&cont, sizeof(int), 1, arq);
	printf("\nContador remocao: %d", cont);
	fclose(arq);
	
	if(cont < cont_remove){
		strcpy(isbn,arq_remove[cont].isbn);
		raiz = pegar_raiz();
		achou = buscar_isbn(isbn, raiz, &pos_pagina, &pos_registro, &offset);
		
		if(achou){
			remover(isbn, raiz, pos_pagina, pos_registro, offset);
		}else{
			printf("\nIsbn %s nao esta na arvore!", isbn);
		}
	}else{
		printf("\nNao ha mais livros a serem deletados no arquivo de remocao\n");
	}
	/* Atualizando contador de remocao */
	abrir_arquivo(livros_arq, leitura);    
	fseek(arq, 12, 0);
	cont=cont+1;
	fwrite(&cont, sizeof(int), 1, arq);
	fclose(arq);	
		
	fclose(btfd);
	getch();
}

/* verifica os 5 casos da remocao */
int remover(char isbn[], int raiz, int pos_pagina, int pos_registro, int offset){
	BTpagina pagina, irmaE, irmaD, pai; 
	int offset_sucessor, pos_pagina_sucessor, pos_registro_sucessor, pos_pag_pai, pos_reg_pai, pos_pag_irmaE, pos_pag_irmaD;
	char sucessor[14];
	
	/* pega a pagina do alvo */
	ler_arquivo(pos_pagina, &pagina);
	//	printf("\npagina atual:");
	//	printf("\nisbn: %s   ponteiro e: %d   ponteiro d: %d", pagina.chave[0], pagina.filhos[0], pagina.filhos[1]);
	/* Verifica se a pagina tem o minimo de elementos obrigatórios para a remoção */
	if(pagina.filhos[1] == NULO){	/* verifica se a pagina é folha. obs: tem q ser o segundo... */
		if((pagina.cont > MINCHAVES)){					/* ...filho para o tratamento da propagação funcionar */
			remover_caso1(pos_registro, pos_pagina, &pagina);
			printf("\nIsbn %s removido, Caso 1", isbn);
		}else{
			achar_pag_irmas(pos_pagina, raiz, isbn, &pos_pag_pai, &pos_reg_pai, &pos_pag_irmaE, &pos_pag_irmaD);
					
			ler_arquivo(pos_pag_pai, &pai); /* pega a pagina pai */
			ler_arquivo(pos_pag_irmaE, &irmaE); /* pega a pagina irma esquerda */
			ler_arquivo(pos_pag_irmaD, &irmaD); /* pega a pagina irma direita */
			
			/* verifica se ha irmao a esquerda e se ele pode dar elementos */
			if((pos_pag_irmaE != -1)  && (irmaE.cont > MINCHAVES)){ 
				/* efetua a redistribuição a direita */
				redistribuicao(pos_reg_pai - 1, &pai, pos_pag_irmaE, &irmaE, &pagina, 0); /* faz a redistribuição */
				/* escreve paginas atualizadas na arvore */
				escrever_arquivo(pos_pag_pai, &pai);
				escrever_arquivo(pos_pag_irmaE, &irmaE);
				escrever_arquivo(pos_pagina, &pagina);
				
//				printf("\nDepois reds.");
//				printf("\nPAI");
//				exibir_pagina(pai);
//				printf("\nirma E");
//				exibir_pagina(irmaE);
//				printf("\nPagina");
//				exibir_pagina(pagina);
				
				printf("\nIsbn %s removido com sucesso, redistribuicao d, caso 3", isbn);
				getch();
				return 1;
			}
			
			/* verifica se ha irmao a direita e se ele pode dar elementos */
			if((pos_pag_irmaD != -1) && (irmaD.cont > MINCHAVES)){  
				/* efetua a redistribuição a direita */
				redistribuicao(pos_reg_pai, &pai, pos_pag_irmaD, &irmaD, &pagina, 1); /* faz a redistribuição */
				/* escreve paginas atualizadas na arvore */
				escrever_arquivo(pos_pag_pai, &pai);
////			escrever_arquivo(pos_pag_irmaD, &irmaD);
				escrever_arquivo(pos_pagina, &pagina);
				
//				printf("\nDepois reds.");
//				printf("\nPAI");
//				exibir_pagina(pai);
//				printf("\nirma D");
//				exibir_pagina(irmaD);
//				printf("\nPagina");
//				exibir_pagina(pagina);
				
				printf("\nIsbn %s removido com sucesso, redistribuicao e, caso 3", isbn);
				getch();
				return 1;
				
			}
			
			/* se nenhum dos irmãos pode emprestar chama a concatenação */
			concatenacao(pos_reg_pai, pos_pag_pai, &pai, pos_pag_irmaE, &irmaE, pos_pag_irmaD, &irmaD, pos_pagina, &pagina);
//			printf("\nisbn pai: %s   ponteiro e: %d   ponteiro d: %d", pai.chave[0], pai.filhos[0], pai.filhos[1]);
//			exibir_pagina(pagina);
			printf("\nConcatenacao, caso 4");
			getch();
			
			/* verifica se propagou para cima */
			if(pai.cont == 0){
				printf("\nPropagou para cima!!!");
				if(pos_pag_pai == raiz){   /* se o pai for a raiz da arvore é só reescrever a raiz */
					inserir_raiz(pai.filhos[0]);
					printf("\nReescrevendo Raiz");
					return 1;
				}
				remover(pai.chave[0], raiz, pos_pag_pai, 0, NULO);  /* removendo o elemento do pai */
			}
		}
	}else{ //caso 2
		/* acha elemento que ira substituir o isbn alvo na arvore B */
		achar_sucessor_imediato(pagina.filhos[pos_registro], sucessor, &offset_sucessor, 
									&pos_pagina_sucessor, &pos_registro_sucessor); 
		substituir_elemento_caso2(pos_pagina, pos_registro, sucessor, offset_sucessor);
		printf("\nIsbn %s alvo substituido por Isbn %s, caso 2", isbn, sucessor);
		/* removendo o sucessor de sua pagina antiga */
		remover(sucessor, raiz, pos_pagina_sucessor, pos_registro_sucessor, offset_sucessor);
		
	}
	return 1;
}

/* remoção simples da arvore B */
void remover_caso1(int pos_registro, int pos_pagina, BTpagina *p_pag){
	int j; 
	for(j = pos_registro; j < (p_pag->cont - 1) ; j++){ 
		strcpy(p_pag->chave[j],p_pag->chave[j+1]);
		p_pag->offset[j] = p_pag->offset[j+1];
	    p_pag->filhos[j] = p_pag->filhos[j+1];
	} 
	p_pag->filhos[j] = p_pag->filhos[j+1];
	p_pag->cont--;
	escrever_arquivo(pos_pagina, p_pag);  /* escrevendo pagina atualizada na arvore */
}

/* acha o sucessor imediato a ESQUERDA do elemento removido */
int achar_sucessor_imediato(int rrn, char sucessor[], int *offset_sucessor, int *pos_pag_suc, int *pos_reg_suc){
	BTpagina pagina;
	
	ler_arquivo(rrn, &pagina); /* le pagina atual */
	
	if(pagina.filhos[pagina.cont] == NULO) {
		/* sucessor = pagina.chave[pagina.cont - 1]; */
		strcpy(sucessor, pagina.chave[pagina.cont - 1]);
		*offset_sucessor = pagina.offset[pagina.cont - 1];
		*pos_pag_suc = rrn;
		*pos_reg_suc = pagina.cont - 1;
		return 1;
	};

	achar_sucessor_imediato(pagina.filhos[pagina.cont], sucessor, offset_sucessor, pos_pag_suc, pos_reg_suc );
	return 1;
}

/* substituir chave alvo pelo sucessor imediato, caso 2 */
void substituir_elemento_caso2(int pos_pagina, int pos_registro, char sucessor[], int offset_sucessor){
	BTpagina pagina;
	
	ler_arquivo(pos_pagina, &pagina); /* pegando pagina do sucessor */
	
	strcpy(pagina.chave[pos_registro], sucessor); /* trocando chave a ser removida pelo sucessor  */
	pagina.offset[pos_registro] = offset_sucessor; /* trocando offset a ser removido pelo offset do sucessor */
	
	escrever_arquivo(pos_pagina, &pagina);   /* escrevendo pagina com novo elemento no arquivo */
}

/* acha as paginas irmãs da pagina do registro a ser removido */
int achar_pag_irmas(int pos_pagina, int raiz, char isbn[], int *pos_pag_pai, int *pos_reg_pai, 
										int *pos_pag_irmaE, int *pos_pag_irmaD){
	BTpagina pagina;
	int j, pos;
	
	ler_arquivo(raiz, &pagina); //pega a raiz da arvore
	
	procurar_no(isbn, &pagina, &pos);
	//verifica se alguem da pagina é pai do elemento a ser removido
	// pos_pag_irmaE = pagina irmã a esquerda
	// pos_pag_irmaD = pagina irmã a direita
	if(pagina.filhos[pos] == pos_pagina){
		*pos_pag_pai = raiz; 
		*pos_reg_pai = pos;
		if(pos == 0){		//se a pos = 0, só tem irmão a direita
			*pos_pag_irmaE = NULO;
			*pos_pag_irmaD = pagina.filhos[pos + 1];		
		}else if(pos == pagina.cont){	//se a pos for igual a quantidade de elementos na pag só tem irmão a esquerda
			*pos_pag_irmaE = pagina.filhos[pos - 1];
			*pos_pag_irmaD = NULO;
		}else{							//duas pag irmãs
			*pos_pag_irmaE = pagina.filhos[pos - 1];
			*pos_pag_irmaD = pagina.filhos[pos + 1];
		}
		return 1;
	}
	achar_pag_irmas(pos_pagina, pagina.filhos[pos], isbn, pos_pag_pai, pos_reg_pai, pos_pag_irmaE, pos_pag_irmaD);
	return 0;
}

/* redistribuicao caso 3 */
void redistribuicao(int pos_reg_pai, BTpagina *pai, int pos_pag_irmao, BTpagina *irmao, BTpagina *alvo, int tipo_redist){
	int posicao_chave_irmao;
	int j;
	
	/* definindo a chave que sera passada do irmao */
	if(tipo_redist == 0){
		posicao_chave_irmao = irmao->cont - 1; 
	}else{
		posicao_chave_irmao = 0;
	}
	
	/* relizando a redistribuicao */
	strcpy(alvo->chave[0], pai->chave[pos_reg_pai]);
	strcpy(pai->chave[pos_reg_pai], irmao->chave[posicao_chave_irmao]);
	
	alvo->offset[0] = pai->offset[pos_reg_pai];
	pai->offset[pos_reg_pai] = irmao->offset[posicao_chave_irmao];
	
	if(alvo->cont == 0) (alvo->cont)++;		/* caso a redistribuição ocorra na propagação o cont do alvo vai estar zero */
											/* sendo assim, incrementa o contador da pag do alvo */			
	if(tipo_redist == 0){
		/* arrumando os ponteiros, necessario para a restribuições de nós que não são folhas */
		alvo->filhos[1] = alvo->filhos[0];
		alvo->filhos[0] = irmao->filhos[irmao->cont];
		/* retirando da pag irmã a chave que foi passada para o pai */
		(irmao->cont)--; 
	}else{
		/* arrumando os ponteiros, necessario para a restribuições de nós que não são folhas */
		alvo->filhos[1] = irmao->filhos[0];
		/* retirando da pag irmã a chave que foi passada para o pai */
		remover_caso1(0, pos_pag_irmao, irmao);																			
	}
}

/* concatenacao caso 5 */
int concatenacao(int pos_reg_pai, int pos_pag_pai, BTpagina *pai, int pos_pag_irmaE, BTpagina *irmaE, 
						int pos_pag_irmaD, BTpagina *irmaD, int pos_pag_alvo, BTpagina *pag_alvo){
	/* se pagina irma a esquerda for -1, a irma a direita será utilizada para fazer a
	   concatenação, com a pagina do elemento a ser removido (pag_alvo) sendo a pagina que ira conter os registros
	   e a irma a direita será  a pagina deletada, caso a irma esquerda não for -1 ela será utilizada para conter os 
	   registros e a pagina alvo será eliminada. */
	if(pos_pag_irmaE == NULO){
		/* copiando primeiro e segundo elemento do irmao para a pagina em que está o elemento a ser removido */
		strcpy(pag_alvo->chave[0], pai->chave[0]);
		pag_alvo->offset[0] = pai->offset[0];
		
		strcpy(pag_alvo->chave[1], irmaD->chave[0]);
		pag_alvo->offset[1] = irmaD->offset[0];
		
		/* pegando os filhos da pagina irma */
		pag_alvo->filhos[1] = irmaD->filhos[0];
		pag_alvo->filhos[2] = irmaD->filhos[1];
		/* incrementando contador de elementos da pagina */
		pag_alvo->cont = 2;
		
		/* reorganizando pagina pai	*/
		reorganiza_pai(pai, 0);
		
		inicializar_pagina(irmaD);
		
		/* escrevendo paginas atualizadas na arvore */
		escrever_arquivo(pos_pag_pai, pai);
		escrever_arquivo(pos_pag_alvo, pag_alvo);
		escrever_arquivo(pos_pag_irmaD, irmaD);
		
	}else{
		/* inserindo na pagina irma a esquerda a chave do pai */
		strcpy(irmaE->chave[1], pai->chave[pos_reg_pai - 1]);
		irmaE->offset[1] = pai->offset[pos_reg_pai - 1];
		
		/* aponta para a pagina filha da pagina que será apagada */
		irmaE->filhos[2] = pag_alvo->filhos[0];
		
		/* incremantando contador da pagina irma esquerda */
		irmaE->cont = 2;
		
		inicializar_pagina(pag_alvo);
		
		/* reorganizando pagina pai */
		reorganiza_pai(pai, pos_reg_pai - 1);
		
		/* escrevendo paginas atualizadas na arvore */
		escrever_arquivo(pos_pag_pai, pai);
		escrever_arquivo(pos_pag_irmaE, irmaE);
		escrever_arquivo(pos_pag_alvo, pag_alvo);
	} 
}

/* reorganiza pagina pai da concatenação */
void reorganiza_pai(BTpagina *pai, int pos_pai){
	int i;
	if(pai->cont != 1){
		for(i = pos_pai; i < (pai->cont); i++){
			strcpy(pai->chave[i], pai->chave[i+1]);
			pai->offset[i] = pai->offset[i+1];
			pai->filhos[i+1] = pai->filhos[i+2];
		} 
	}else{
		pai->filhos[1] = NULO;
	}
	
	(pai->cont)--;	
}
