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
 
/* structs do arquivo */
/* struct da arvore b */
typedef struct pag{ 
	int cont;          		 /* cont de chaves na página */
    char chave[MAXCHAVES][14];   /* chave[] atual */
    int filhos[MAXCHAVES+1]; /* ponteiro para os rrn filhos | o num de filhos e igual a ordem entao maxchaves + 1 */
    int offset[MAXCHAVES]; //offsets dos registros  
}BTpagina; 

/* arquivo biblioteca.bin */
struct livro{
  char isbn[14];
  char titulo[50];
  char autor[50];
  char ano[5];
}arq_livros[TAM_STRUCT];

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

/* variaveis globais para abertura de arquivo */
char atualizar[] = "ab", leitura[] = "r+b", escrever[] = "wb";
int qtd_chave_s = 0;
 
/* prototypes */ 
void split(char chave[], int r_filho, BTpagina *p_pag_antiga, char promo_chave[], int *promo_r_filho, BTpagina *p_nova_pag); 
void esta_na_pagina (char chave[], int r_filho, BTpagina *p_pag); 
void escrever_arquivo (int rrn, BTpagina *ponteiro_pag);
void inicializar_pagina (BTpagina *p_pag); 
void inserir_raiz(int raiz); 
void fechar_arquivo (FILE **p_arq);
int abrir_arquivo (char nome_arq[], char tipo_abertura[]); 
int pegar_raiz (); 
int pegar_pagina ();
int criar_arvore(char chave[]);
int carregar_arquivo(int resp);
int procurar_no(char chave[], BTpagina *p_pag, int *pos);
int criar_raiz(char chave[], int esquerda, int direita);
int inserir (int rrn, char chave[], int *promo_r_filho, char promo_chave[]);
BTpagina ler_arquivo (int rrn, BTpagina *ponteiro_pag); 

int main(){ 
	int promovido;		/* boolean: tells if a promotion from below */
    int raiz, i=0,   	/* rrn of raiz page */
		promo_rrn;  	/* rrn promovido from below */
    char promo_chave[14],	/* chave[] promovido from below */ 
        chave[] = "1111111111111", 
		whatchave[][14]={"2222222222222", "9999999999999", "4444444444444", "5555555555555",
		"3333333333333", "8888888888888", "6666666666666", "7777777777777", "1111111111111",
		"9999995555555", "1111112222222", "8888889999999", "7777788888888"};          /* next chave[] to inserir in tree */
    printf("\n hello there\n");     
    
	int resp, sair = 0, tam_vet_inserir = 0;
	char arq_livros[]="livros.bin", arq_arvoreB[]="btree.bin";
	do{ 
		system("cls");
	 	printf("\n        Menu");
	  	printf("\n1 - Insercao");
	  	printf("\n2 - Listar Livros");
	  	printf("\n3 - Procurar Livro");
	  	printf("\n4 - Consulta Casada");
	  	printf("\n5 - Carregar Arquivos");
	  	printf("\n6 - Remover Livro");
	  	printf("\n7 - Dump Arquivo ");
	  	printf("\n8 - Sair");
	  	printf("\nOpcao: ");
		// scanf("%d",&resp);
		resp=1;
		switch(resp){
			case 1:{
	    			//insercao arrumar pra caso geral ao invez do while direto
					if (abrir_arquivo("btree.bin", "rt+") == 1){ 
						printf("\n\nArquivo já existe");  
					    raiz = pegar_raiz(); 
					    printf("\nRaiz: %d", raiz);
					} 
					else { 
						printf("\n\nArquivo nao existe, criando arvore.");  
					    raiz = criar_arvore(chave);
					    printf("\nteste: %d", pegar_raiz());
					}   
					
					getch();
					 
					while (strcmp(chave,"7777788888888")!=0) { 	
						strcpy(chave,whatchave[i]); 	
						printf("\nInserindo a chave %s", chave);
					    promovido = inserir(raiz, chave, &promo_rrn, promo_chave); 
					    if (promovido) {
					    	printf ("raiz %s promovida\n\007", promo_chave); 
						    raiz = criar_raiz(promo_chave, raiz, promo_rrn);
						}
					    i++;
					    getch();
					}
					fechar_arquivo(&btfd); 
					
					printf("\nsaindo...");
					getch();
				break;
			}
		  	case 2:{
		  		
				break;
			}
		  	case 3: {
	  			
	  			break;
	  		}
	  		case 4:{
		  		break;
			}
	  		case 5:{
				resp=0;
			    tam_vet_inserir = carregar_arquivo(resp); 
			    printf("\nArquivos Carregados");
			    system("pause");
				break;
			}
	  		case 6:{
		    	break;
			}
			case 7:{
				//dump arquivo antigo
					/*	system("cls");
		  		do{
					printf("\n\nDigite qual arquivo deseja exibir: ");
					printf("\n1 - Principal");
					printf("\n2 - Indice Primario");
					printf("\n3 - Indice Secundario");
					printf("\n4 - Lista Invertida");
					printf("\nopcao:");
					scanf("%d",&resp);
				}while(resp < 1 || resp > 4);
			    exibir_arquivo(resp);*/
				break;
			}
			case 8:{
				sair = 1;
				break;
			}
		  	default:{
			    printf("\nOpcao invalida!");
				break;
			} 
		}
		sair=1;
	}while(sair != 1);
}   

int inserir(int rrn, char chave[], int *promo_r_filho, char promo_chave[]){ 
	BTpagina pagina,		//pagina atual 
           nova_pagina;    	//nova pagina caso exista split 
	int achou, promovido;  	//valores booleanos 
    int pos, 
	    p_b_rrn;        	// rrn promovido da recursão abaixo 
    char p_b_chave[14];         // chave promovido da recursão abaixo 
        
    if(rrn == NULO){   
    	strcpy(promo_chave,chave); 
        *promo_r_filho = NULO; 
        printf("\nFolha\n");
        return(SIM); 
    } 
    ler_arquivo(rrn, &pagina); 
    //verifica se existe chave[] duplicada
    achou = procurar_no(chave, &pagina, &pos); 
    
    //printf("\n\nPosicao: %d", pos);
    //printf("\nEndereço do filho da posicao %d: %d\n", pos, pagina.filhos[pos]);
    
    
    if(achou){ 
    	printf("Erro: chave %s duplicada\n\007", chave); 
        return(0); 
    } 
    promovido = inserir(pagina.filhos[pos], chave, &p_b_rrn, p_b_chave); 
    if(!promovido){ 
    	return(NAO); 
	}
	if(pagina.cont < MAXCHAVES){ 
    	esta_na_pagina(p_b_chave, p_b_rrn, &pagina); 
        escrever_arquivo(rrn, &pagina);
		//printf ("Erro: chave[] %c duplicada\n\007", chave[]);  
        return(NAO); 
    }else{
		split(p_b_chave, p_b_rrn, &pagina, promo_chave, promo_r_filho, &nova_pagina);
		printf ("chave %s promovida\n\007", promo_chave);  
		printf("\nChave %s inserida com sucesso.\n", promo_chave);
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
 
void fechar_arquivo(FILE **p_arq){ 
	fclose(*p_arq); 
} 
 
int pegar_raiz() { 
	fseek(btfd, 0, 0); 
    if(fread(&raiz,sizeof(int),1, btfd) == NULL) { 
    	printf("Erro: Impossivel pegar raiz. \007\n"); 
        exit(1); 
	}
    return(raiz); 
} 
 
void inserir_raiz(int raiz){ 	
	fseek(btfd, 0, 0); 
    fwrite(&raiz, sizeof(raiz),1,btfd); 
	printf("\n entrei inserir raiz: %d", raiz);
} 
 
int criar_arvore(char chave[]){ 
	btfd = fopen("btree.bin","wt+"); 
	printf("\narquivo criado");
	inserir_raiz(-1);  									//mudei
	fclose (btfd); 
	getch();
	abrir_arquivo("btree.bin","wt+"); 
	return (criar_raiz(chave, NULO, NULO)); 
} 
 
int pegar_pagina(){ 
	printf("\nentrei pegar pagina");
	int addr; 
	fseek(btfd, 0, 2);
	addr = ftell(btfd) + 4; 
	printf("\nendereco: %d", addr);
	return (addr/TAMPAG); 
} 
 
BTpagina ler_arquivo(int rrn, BTpagina *ponteiro_pag){ 
	int addr; 
	addr  = (rrn * TAMPAG) + 4; 	
	printf("\n\nendereco: %d", addr);
	fseek(btfd, addr, 0); 
	fread(ponteiro_pag, sizeof(BTpagina), 1, btfd); 
	printf("\nTESTE: %d\n", ponteiro_pag->cont);
	getch();
	return *ponteiro_pag;
} 
 
void escrever_arquivo(int rrn, BTpagina *ponteiro_pag) { 
	int addr; 
	addr = (rrn * TAMPAG) + 4; 
	fseek(btfd, addr, 0); 
	fwrite(&*ponteiro_pag,TAMPAG, 1,btfd); 
	printf("\nescrevi no arquivo");
}                  

int criar_raiz(char chave[], int esquerda, int direita) { 
	printf("\n\nentrei criar raiz");
   	BTpagina pagina; 
   	int rrn; 
   	rrn = pegar_pagina();
	printf("\nrrn: %d",rrn); 
   	inicializar_pagina(&pagina); 
   	strcpy(pagina.chave[0],chave); 
   	pagina.filhos[0] = esquerda; 
   	pagina.filhos[1] = direita; 
   	pagina.cont = 1; 
   	escrever_arquivo(rrn, &pagina); 
   	inserir_raiz(rrn); 
   	printf("\nraiz criada");
   	return(rrn); 
} 
 
void inicializar_pagina(BTpagina *p_pag) { 
	int j; 
	printf("\nentrei inicializar pagina");
	for(j = 0; j < MAXCHAVES; j++){ 
	   strcpy(p_pag->chave[j],NO); 
	   p_pag->filhos[j] = NULO; 
	   p_pag->offset[j] = NULO;
	} 
	p_pag->filhos[MAXCHAVES] = NULO; 
} 
 
int procurar_no(char chave[], BTpagina *p_pag, int *pos){ 
    int i; 
    for(i = 0; i < p_pag->cont && (strcmp(chave,p_pag->chave[i])>0); i++){
		printf("isbn: %s && filho e: %d && filho d: %d     ", p_pag->chave[i], p_pag->filhos[i], p_pag->filhos[i+1]);
	} 
	printf("isbn: %s && filho e: %d && filho d: %d     ", p_pag->chave[i], p_pag->filhos[i], p_pag->filhos[i+1]);
	printf("\n");
    *pos = i; 
    if(*pos < p_pag->cont && (strcmp(chave,p_pag->chave[*pos])==0)){ 
    	return(SIM); 
    } 
    return(NAO); 
}
              
void esta_na_pagina(char chave[],int r_filho, BTpagina *p_pag) { 
	int j; 
	for(j = p_pag->cont; (strcmp(chave,p_pag->chave[j-1])<0) && j > 0; j--){ 
		strcpy(p_pag->chave[j],p_pag->chave[j-1]); 
	    p_pag->filhos[j+1] = p_pag->filhos[j]; 
	} 
	p_pag->cont++; 
	strcpy(p_pag->chave[j],chave); 
	p_pag->filhos[j+1] = r_filho; 
} 
 
void split(char chave[], int r_filho, BTpagina *p_pag_antiga, char promo_chave[], int *promo_r_filho, BTpagina *p_nova_pag){ 
    int j; 
    int mid; 
	char vet_chave[MAXCHAVES+1][14]; 
	int vet_filhos[MAXCHAVES+2];
	int vet_offset[MAXCHAVES+1];
    printf("\nDivisão de nó\n");
	/* coloca os elementos da pagina no vetor temporario */
    for(j = 0; j < MAXCHAVES; j++){ 
    	strcpy(vet_chave[j], p_pag_antiga->chave[j]); 
        vet_filhos[j] = p_pag_antiga->filhos[j];
        vet_offset[j] = p_pag_antiga->offset[j];
	}
    vet_filhos[3] = p_pag_antiga->filhos[3];
//	vet_filhos[4] = p_pag_antiga->filhos[4]; 
    
    /* reoordena e encontra o lugar em que a nova chave[] deve ser inserida */
    for(j = MAXCHAVES; (strcmp(chave, vet_chave[j-1]) < 0) && j > 0; j--){ 
        strcpy(vet_chave[j], vet_chave[j-1]); 
        vet_filhos[j+1] = vet_filhos[j];
        vet_offset[j] = p_pag_antiga->offset[j];
    }
	/* insere chave[] onde deve ficar e coloca chave[] antiga no filho */
    strcpy(vet_chave[j], chave); 
    vet_filhos[j+1] = r_filho; 
	
	/* cria nova pagina com o promovido */
	(*promo_r_filho) = pegar_pagina(); 
	inicializar_pagina(p_nova_pag);
	inicializar_pagina(p_pag_antiga);
	
	/* coloca as paginas nos devidos lugares */
	p_pag_antiga->cont = 2; 
	strcpy(p_pag_antiga->chave[0],vet_chave[0]);
	strcpy(p_pag_antiga->chave[1],vet_chave[1]);    
	p_pag_antiga->filhos[0] = vet_filhos[0]; 	
	p_pag_antiga->filhos[1] = vet_filhos[1]; 	
	p_pag_antiga->filhos[2] = vet_filhos[2];
	
	strcpy(p_pag_antiga->chave[2],NO);            
	//p_pag_antiga->filhos[2] = NULO;   isso aqui ta errado!!
   
   	//nao sei se ta certo, conferir
   	p_nova_pag->cont = MINCHAVES;
   	strcpy(p_nova_pag->chave[0],vet_chave[3]); 
	p_nova_pag->filhos[0] = vet_filhos[3];
	p_nova_pag->filhos[1] = vet_filhos[4]; 
	
	strcpy(promo_chave, vet_chave[2]);
	printf ("\nChave %s promovida\n", promo_chave);
}

/* carrega todos arquivos */ 
int carregar_arquivo(int resp){
	char cadastro_arq[]="biblioteca.bin", busca_arq[]="busca.bin", consulta_casada_arq[]="consulta_casada.bin",
		 remove_arq[]= "remove.bin";
  	int i, tam_vet_inserir = 0, todos=0;
  
  	system("cls");
  	if(resp != 0){
  		todos = 1;
  	}
  	/* abre arquivo biblioteca, carrega em vetor de struct */
  	if(resp == 1 || todos == 0){
		if(abrir_arquivo(cadastro_arq, leitura)){
	  		i=0;
	    	while(fread(&arq_livros[i], sizeof(livro), 1, arq)){
	      		i++;  
	    	}
	    fechar_arquivo(&arq);
	  	tam_vet_inserir = i;  
	  }
  	}
  	/* abre arquivo busca.bin, carrega em vetor de struct */
  	if(resp == 2 || todos == 0){
		if(abrir_arquivo(busca_arq, leitura)){
		    i=0;
			while(fread(&arq_busca[i], sizeof(struct busca_arquivo), 1, arq)){
				i++;
			}
			fechar_arquivo(&arq);
		}
  	} 
  	/* abre arquivo consulta_casada.bin, carrega em vetor de struct */
  	if(resp == 3 || todos == 0){
	  	if(abrir_arquivo(consulta_casada_arq, leitura)){
	    	i=0;
			while(fread(&arq_consulta_casada[i], sizeof(struct busca_casada), 1, arq)){
			  i++;
			}
			fechar_arquivo(&arq);
	  	}
  	}
  	return tam_vet_inserir;
}
 


