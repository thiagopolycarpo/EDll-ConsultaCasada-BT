#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAXCHAVES 3 //num  maximo de chaves que significa ordem 4 portanto maxkey = 4 - 1
#define MINCHAVES 1 // minimo de chaves que e MAXCHAVES/2 
#define NULO -1 
#define NO   '@' 
#define NAO   0 
#define SIM   1 
 
typedef struct pag{ 
	int cont;          		//cont de chaves na página
    char chave[MAXCHAVES];    //chave atual 
    int filhos[MAXCHAVES+1];  //ponteiro para os rrn filhos | o num de filhos e igual a ordem entao maxchaves + 1
    int offset[MAXCHAVES];	//offset do arquivo de chaves
}BTpagina; 


#define TAMPAG sizeof(BTpagina) 
 
int raiz;             // rrn da raiz 
FILE *btfd;           // arquivo da arvore b 
FILE *infd;           // arquivo da biblioteca 
 
/* prototypes */ 
void fechar_arquivo (); 
int abrir_arquivo (); 
BTpagina ler_arquivo (int rrn, BTpagina *ponteiro_pag); 
void escrever_arquivo (int rrn, BTpagina *ponteiro_pag); 
int criar_raiz (char chave, int esquerda, int direita); 
int criar_arvore(); 
int pegar_pagina (); 
int pegar_raiz (); 
int inserir (int rrn, char chave, int *promo_r_filho, char *promo_chave); 
void esta_na_pagina (char chave,int r_filho, BTpagina *p_pag); 
void inicializar_pagina (BTpagina *p_pag); 
void inserir_raiz(int raiz); 
int procurar_no (char chave, BTpagina *p_pag, int *pos); 
void split(char chave, int r_filho, BTpagina *p_pag_antiga, char *promo_chave, int *promo_r_filho, BTpagina *p_nova_pag); 

 int main() { 
	int promovido;      // boolean: tells if a promotion from below 
    int raiz,i=0,        // rrn of raiz page 
        promo_rrn;   // rrn promovido from below 
    char promo_chave,    // chave promovido from below 
        chave='t', 
		whatchave[12]={'h','i','a','g','o','p','o','l','y','c','a','q'};          // next chave to inserir in tree 
    printf("\n hello there\n");     
    if (abrir_arquivo() == 1) { 
    	 printf("\nentrei abrir arquivo\n");  
        raiz = pegar_raiz(); 
    } 
    else { 
    	printf("\n entrei criar arquivo there\n");  
        raiz = criar_arvore();     }    
    while (chave !='q') { 	
    	
    	printf("\n entrei while main chave --> %c\n", chave);
        promovido = inserir(raiz, chave, &promo_rrn, &promo_chave); 
        if (promovido) {
 	    	printf ("raiz %c promovida\n\007", promo_chave); 
		    raiz = criar_raiz(promo_chave, raiz, promo_rrn);
		}
		chave = whatchave[i]; 
 	    i++;
    }
	fechar_arquivo(); 
    printf("\n saindo...\n");
}   

int inserir (int rrn, char chave, int *promo_r_filho, char *promo_chave){ 
	BTpagina pagina,		//pagina atual 
           nova_pagina;    	//nova pagina caso exista split 
	int achou, promovido;  	//valores booleanos 
    int pos, 
	    p_b_rrn;        	// rrn promovido da recursão abaixo 
    char p_b_chave;         // chave promovido da recursão abaixo 
        
    if (rrn == NULO) 
    {   
    	*promo_chave = chave; 
        *promo_r_filho = NULO; 
        return(SIM); 
    } 
    ler_arquivo(rrn, &pagina); 
    //verifica se existe chave duplicada
    achou = procurar_no ( chave, &pagina, &pos); 
    if (achou) 
    { 
    	printf ("Erro: Chave %c duplicada\n\007", chave); 
        return(0); 
    } 
    promovido = inserir(pagina.filhos[pos], chave, &p_b_rrn, &p_b_chave); 
    if (!promovido) 
    { 
    	return(NAO); 
	}
	printf ("Chave %c promovida\n\007", chave); 
    if(pagina.cont < MAXCHAVES) 
    { 
    	esta_na_pagina(p_b_chave, p_b_rrn, &pagina); 
        escrever_arquivo(rrn, &pagina);
		printf ("Erro: Chave %c duplicada\n\007", chave);  
        return(NAO); 
    } 
    else{
		split(p_b_chave, p_b_rrn, &pagina, promo_chave, promo_r_filho, &nova_pagina); 
		printf("\nChave %c inserida com sucesso.\n",promo_chave);
    	escrever_arquivo(rrn, &pagina); 
	    escrever_arquivo(*promo_r_filho, &nova_pagina);
		return(SIM); 
	} 
} 
  
int abrir_arquivo() { 
	if((btfd = fopen("btree.dat", "rt+")) == NULL)
		return 0;
    return 1; 
} 
 
void fechar_arquivo(){ 
	fclose(btfd); 
} 
 
int pegar_raiz() { 
	fseek(btfd, 0, 0); 
    if (fread(&raiz,sizeof(int),1, btfd) == NULL) { 
    	printf("Erro: Impossivel pegar raiz. \007\n"); 
        exit(1); 
	}
    return (raiz); 
} 
 
void inserir_raiz(int raiz){ 	
	fseek(btfd, 0, 0); 
    fwrite(&raiz, sizeof(raiz),1,btfd); 
	printf("\n entrei inserir raiz %c\n", raiz);
} 
 
int criar_arvore(){ 
      char chave; 
      btfd = fopen("btree.dat","wt+"); 
      printf("\n arquivo criado \n");  
      fclose (btfd); 
      abrir_arquivo(); 
      chave = 't';//getchar(); 
      return (criar_raiz(chave, NULO, NULO)); 
} 
 
int pegar_pagina() { 
	printf("\n\nentrei pegar pagina\n");
	int addr; 
	fseek(btfd, 0, 2);
	addr = ftell(btfd); 
	return (addr/TAMPAG); 
} 
 
BTpagina ler_arquivo (int rrn, BTpagina *ponteiro_pag){ 
	int addr; 
	addr  = rrn * TAMPAG; 
	fseek(btfd, addr, 0); 
	fread(ponteiro_pag, sizeof(BTpagina), 1, btfd); 
	return *ponteiro_pag;
} 
 
void escrever_arquivo(int rrn, BTpagina *ponteiro_pag) { 
	int addr; 
	addr = rrn * TAMPAG; 
	fseek(btfd, addr, 0); 
	fwrite(&*ponteiro_pag,TAMPAG, 1,btfd); 
	printf("\n\nescrevi no arquivo\n");
}                  

int criar_raiz(char chave, int esquerda, int direita) { 
	printf("\n\nentrei criar raiz\n");
   	BTpagina pagina; 
   	int rrn; 
   	rrn = pegar_pagina();
	printf("\n\nrrn %d\n",rrn); 
   	inicializar_pagina (&pagina); 
   	pagina.chave[0] = chave; 
   	pagina.filhos[0] = esquerda; 
   	pagina.filhos[1] = direita; 
   	pagina.cont = 1; 
   	escrever_arquivo(rrn, &pagina); 
   	inserir_raiz(rrn); 
   	printf("\n entrei criar raiz, \n\nraiz criada %d\n", rrn);
   	return(rrn); 
} 
 
void inicializar_pagina(BTpagina *p_pag) { 
	int j; 
	printf("\n\entrei inicializar pagina\n");
	for (j = 0; j < MAXCHAVES; j++){ 
	   p_pag->chave[j] = NO; 
	   p_pag->filhos[j] = NULO; 
	} 
	p_pag->filhos[MAXCHAVES] = NULO; 
} 
 
int procurar_no(char chave, BTpagina *p_pag, int *pos){ 
    int i; 
    for (i = 0; i < p_pag->cont && chave > p_pag->chave[i]; i++); 
    *pos = i; 
    if (*pos < p_pag->cont && chave == p_pag->chave[*pos]) { 
    	return(SIM); 
    } 
    return(NAO); 
}
              
void esta_na_pagina(char chave,int r_filho, BTpagina *p_pag) { 
	int j; 
	for(j = p_pag-> cont; chave < p_pag->chave[j-1] && j > 0; j--){ 
		p_pag->chave[j] = p_pag->chave[j-1]; 
	    p_pag->filhos[j+1] = p_pag->filhos[j]; 
	} 
	p_pag->cont++; 
	p_pag->chave[j] = chave; 
	p_pag->filhos[j+1] = r_filho; 
} 
 
void split(char chave, int r_filho, BTpagina *p_pag_antiga, char *promo_chave, int *promo_r_filho, BTpagina *p_nova_pag){ 
    int j; 
    int mid; 
	char workkeys[MAXCHAVES+1]; 
	int workchil[MAXCHAVES+2]; 
    printf("\nDivisão de nó\n");
	//coloca os elementos da pagina no vetor    
    for (j = 0; j < MAXCHAVES; j++){ 
    	workkeys[j] = p_pag_antiga->chave[j]; 
        workchil[j] = p_pag_antiga->filhos[j];
		printf(" %c ",workkeys[j]); 
	}
    workchil[j] = p_pag_antiga->filhos[j]; 
    
    //reoordena e encontra o lugar em que a nova chave deve ser inserida
    for (j = MAXCHAVES; chave < workkeys[j-1] && j > 0; j--){ 
        workkeys[j] = workkeys[j-1]; 
        workchil[j+1] = workchil[j]; 
    }
	//insere chave onde deve ficar e coloca chave antiga no filho 
    workkeys[j] = chave; 
    workchil[j+1] = r_filho; 
	
	//cria nova pagina com o promovido
	*promo_r_filho = pegar_pagina(); 
	inicializar_pagina(p_nova_pag);
	
	//coloca as paginas nos devidos lugares 
	for (j = 0; j < MINCHAVES; j++){ 
		p_pag_antiga->chave[j] = workkeys[j]; 
	   	p_pag_antiga->filhos[j] = workchil[j]; 
	    p_nova_pag->chave[j] = workkeys[j+MINCHAVES]; 
	    p_nova_pag->filhos[j] = workchil[j+MINCHAVES]; 
	    p_pag_antiga->chave[j+MINCHAVES] = NO;            
	    p_pag_antiga->filhos[j+MINCHAVES] = NULO; 
   } 
   //nao sei se ta certo, conferir
   p_pag_antiga->filhos[0] = workchil[0]; 
   p_nova_pag->filhos[0] = workchil[MINCHAVES]; 
   p_nova_pag->cont = MAXCHAVES - MINCHAVES; 
   p_pag_antiga->cont = MINCHAVES; 
   *promo_chave = workkeys[MINCHAVES+1];
   printf ("\nChave %c promovida\n", *promo_chave); 
} 


