/***************************************************************************
*  $MCI Módulo de implementação: GRA  Grafo genêrico
*
*  Arquivo gerado:              GRAFO.c
*  Letras identificadoras:      GRA
*
*  Nome da base de software:    Arcabouço para a automação de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\GRAFO.BSW
*
*  Projeto: INF 1301 / Labirinto
*  Gestor:  LSKDANEAV
*  Autores: eav
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*		 1	   eav	 25/09/2014		//
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>
#include   <assert.h>

#define GRAFO_OWN
#include "GRAFO.h"
#undef GRAFO_OWN


/***********************************************************************
*
*  $TC Tipo de dados: GRA Elemento de nó
*
*
***********************************************************************/

	 typedef struct tpNode_ {

				void* pValor ;
				/* Ponteiro para o valor contido no elemento */

				LIS_tppLista arestas ;
				//Lista de tppVertice
                /* Ponteiro para a cabeça de aresta */
	 } tpNode ;

/***********************************************************************
*
*  $TC Tipo de dados: GRA Elemento de vértice
*
*
***********************************************************************/

	 typedef struct GRA_tpVertice_ {
	 	tpNode* pNode ;
							 /* Ponteiro para a cabeça de nó */      
	 } GRA_tpVertice ;

		
/***********************************************************************
*
*  $TC Tipo de dados: GRA Elemento de componente
*
*
***********************************************************************/

	 typedef struct tagElemComp {

		 struct tagElemVertice* pVertice;
			 /* Ponteiro para o vértice origem do componente */

	 } tpElemComp ;


/***********************************************************************
*
*  $TC Tipo de dados: GRA Descritor da cabeça de grafo
*
*
***********************************************************************/

	typedef struct GRA_tagGrafo {
		LIS_tppLista vertices;
	 
		LIS_tppLista componentes;
	 
		void ( * ExcluirValor ) ( void * pValor ) ;
				 /* Ponteiro para a função de destruição do valor contido em um elemento */
	 } GRA_tpGrafo;

/***** Protótipos das funções encapsuladas no módulo *****/

static void RemoverAresta(GRA_tpVertice* u, GRA_tpVertice* v);

static GRA_tpVertice* CriarVertice(GRA_tppGrafo grafo, void* pValor);

static int BFS(GRA_tpVertice* v, GRA_tpVertice* u);

static void ExcluirAresta (GRA_tppGrafo grafo, GRA_tpVertice* v, GRA_tpVertice* u);

static GRA_tpVertice* ObterOrigem (GRA_tppGrafo grafo, GRA_tpVertice* v);

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: GRA  &Criar Grafo
*  ****/


	 GRA_tppGrafo GRA_CriarGrafo( void (*ExcluirValor) (void* pDado) )
	 {

			GRA_tpGrafo * pGrafo = NULL ;

			pGrafo = ( GRA_tpGrafo * ) malloc( sizeof( GRA_tpGrafo )) ;
			if (pGrafo == NULL) {
			
				 return NULL ;
			} /* if */
			LIS_tppLista l_vertice = LIS_CriarLista(free); //A Exclusão será manual, iterando sobre os elementos;
			LIS_tppLista l_componente = LIS_CriarLista(NULL); //Controle interno e autocontido; Componentes não cuidam de dados do valor, só armazenam ponteiros.
			
			pGrafo->vertices = l_vertice;
			pGrafo->componentes = l_componente;
			pGrafo->ExcluirValor = ExcluirValor ;

			return pGrafo ;

	 } 
     /* Fim função: GRA  &Criar grafo */

/***************************************************************************
*
*  Função: GRA  &Destruir grafo
*  ****/

	 GRA_tpCondRet GRA_DestruirGrafo( GRA_tppGrafo pGrafo )
	 {
	 		tpNode* no;
			#ifdef _DEBUG
				 assert( pGrafo != NULL ) ;
			#endif

			LIS_DestruirLista(pGrafo->componentes);

			LIS_IrInicioLista(pGrafo->vertices);
			do {
				no = LIS_ObterValor(pGrafo->vertices);
				if (no->pValor != NULL) {
					pGrafo->ExcluirValor(no->pValor);
				}
				LIS_DestruirLista(no->arestas);
				free(no);
			} while(LIS_AvancarElementoCorrente(pGrafo->vertices, 1) != LIS_CondRetFimLista);

			LIS_DestruirLista(pGrafo->vertices);

			free( pGrafo ) ;
			
			return GRA_CondRetOK;
	 } 
     /* Fim função: GRA  &Destruir grafo */


/***************************************************************************
*
*  Função: GRA  &Inserir vertice 
*  ****/

	 GRA_tpCondRet GRA_InserirVertice (GRA_tppGrafo pGrafo, GRA_tppVertice* pVertice, void* pValor)
	 {

		#ifdef _DEBUG
			 assert( pGrafo != NULL ) ;
		#endif

		/* Criar o Vertice antes */

		GRA_tppVertice pElem = CriarVertice( pGrafo, pValor ) ;
		
        if ( pElem == NULL )
			return GRA_CondRetFaltouMemoria ;

	 	if( LIS_InserirElementoApos (pGrafo->componentes, pElem) != LIS_CondRetOK)
			return GRA_CondRetFaltouMemoria ;
		
		*pVertice = pElem;
        
		return GRA_CondRetOK ;

	 } 
	 /* Fim função: GRA  &Inserir vertice */


/***************************************************************************
*
*  Função: GRA  &Excluir vertice 
*  ****/
     
    GRA_tpCondRet GRA_ExcluirVertice (GRA_tppGrafo pGrafo, GRA_tppVertice pVertice) {
        GRA_tpVertice* vizinho = NULL;
        tpNode* no = NULL;

        if (pGrafo->vertices == NULL) {
            return GRA_CondRetNaoEhVertice;
        }
        if (LIS_ProcurarValor(pGrafo->vertices, pVertice) != LIS_CondRetOK) { //Verifica se vertice pertence ao grafo.
            return GRA_CondRetNaoEhVertice;
        }
        
        no = LIS_ObterValor(pGrafo->vertices);
        pGrafo->ExcluirValor(no->pValor);

        // arestas
        LIS_IrInicioLista(no->arestas);
        do {
            vizinho = LIS_ObterValor(no->arestas);
            ExcluirAresta(pGrafo, pVertice, vizinho); 
        } 
        while (LIS_AvancarElementoCorrente(no->arestas, 1) != LIS_CondRetFimLista);


        LIS_DestruirLista(no->arestas);
        free(no);

        LIS_ExcluirElemento(pGrafo->vertices);

        return GRA_CondRetOK;
    } 
    /* Fim função: GRA  &Excluir vertice */
  
/***************************************************************************
*
*  Função: GRA  &Inserir Aresta
*  ****/
  
    GRA_tpCondRet GRA_InserirAresta( GRA_tppGrafo pGrafo, GRA_tppVertice pVertice1, GRA_tppVertice pVertice2 ) {
        GRA_tpVertice* origem1= NULL;
        GRA_tpVertice* origem2= NULL;
        /* Verifica se vertice pertence ao grafo; */
        if (LIS_ProcurarValor(pGrafo->vertices, pVertice1) != LIS_CondRetOK) {
            return GRA_CondRetNaoEhVertice;
        }

        /* Verifica se vertice pertence ao grafo; */
        if (LIS_ProcurarValor(pGrafo->vertices, pVertice2) != LIS_CondRetOK) {
            return GRA_CondRetNaoEhVertice;
        }

        if (pVertice1 == pVertice2) {
            return GRA_CondRetEhVizinho;
        }

        if (LIS_ProcurarValor(pVertice1->pNode->arestas, pVertice2) != LIS_CondRetOK && 
            LIS_ProcurarValor(pVertice2->pNode->arestas, pVertice1) != LIS_CondRetOK ) {

            origem1 = ObterOrigem(pGrafo, pVertice1);
            origem2 = ObterOrigem(pGrafo, pVertice2);

            if (origem1 != origem2) { //Estavam em componentes distintas? Se sim, junta
                LIS_ProcurarValor(pGrafo->componentes, origem1);
                LIS_ExcluirElemento(pGrafo->componentes);
            }

            LIS_InserirElementoApos(pVertice1->pNode->arestas, pVertice2);
            LIS_InserirElementoApos(pVertice2->pNode->arestas, pVertice1);

            return GRA_CondRetOK;
        } 
        else {
            return GRA_CondRetEhVizinho;
        }
    }
    /* Fim função: GRA  &Inserir Aresta */
  
/***************************************************************************
*
*  Função: GRA  &Excluir Aresta
*  ****/
          
    GRA_tpCondRet GRA_ExcluirAresta( GRA_tppGrafo pGrafo , GRA_tppVertice pVertice1, GRA_tppVertice pVertice2 ) {

        /* Verifica se vertice pertence ao grafo; */
        if (LIS_ProcurarValor(pGrafo->vertices, pVertice1) != LIS_CondRetOK) {
            return GRA_CondRetNaoEhVertice;
        }

        /* Verifica se vertice pertence ao grafo; */
        if (LIS_ProcurarValor(pGrafo->vertices, pVertice2) != LIS_CondRetOK) {
            return GRA_CondRetNaoEhVertice;
        }

        ExcluirAresta(pGrafo, pVertice1, pVertice2);

        return GRA_CondRetOK;
    }
    /* Fim função: GRA  &Excluir Aresta */

  
/***************************************************************************
*
*  Função: GRA  &Obter Vizinhos
*  ****/
     
    GRA_tpCondRet GRA_ObterVizinhos ( GRA_tppGrafo pGrafo, GRA_tppVertice pVertice, LIS_tppLista * pLista) {
        
        if (pVertice == NULL) 
            return GRA_CondRetNaoEhVertice; 
        if (LIS_ProcurarValor(pGrafo->vertices, pVertice) != LIS_CondRetOK) {
        	return GRA_CondRetNaoEhVertice; 
        }

        LIS_tppLista Ret_vizinhos = LIS_CriarLista(NULL);
        if (Ret_vizinhos == NULL)
            return GRA_CondRetFaltouMemoria;
        LIS_tppLista vizinhos = pVertice->pNode->arestas;
        LIS_IrInicioLista(vizinhos);
        do {
                    GRA_tppVertice no = LIS_ObterValor(vizinhos);
                    LIS_InserirElementoApos(Ret_vizinhos,no);
                } while(LIS_AvancarElementoCorrente(vizinhos, 1) != LIS_CondRetFimLista);
        *pLista = Ret_vizinhos;

        return GRA_CondRetOK;
    }
    /* Fim função: GRA  &Obter Vizinhos */

/***************************************************************************
*
*  Função: GRA  &Obter Vizinhos
*  ****/
     
    GRA_tpCondRet GRA_ObterOrigens( GRA_tppGrafo pGrafo, LIS_tppLista * pLista) {
        LIS_tppLista Ret_origens = LIS_CriarLista(NULL);
        
        if (Ret_origens == NULL) {
            return GRA_CondRetFaltouMemoria;
        }

        LIS_tppLista origens = pGrafo->componentes;

        LIS_IrInicioLista(origens);
        do {
            GRA_tppVertice no = LIS_ObterValor(origens);
            LIS_InserirElementoApos(Ret_origens,no);
        } 
        while(LIS_AvancarElementoCorrente(origens, 1) != LIS_CondRetFimLista);
        
        *pLista = Ret_origens;

        return GRA_CondRetOK;
    }
    /* Fim função: GRA  &Obter Vizinhos */
 
    
/***************************************************************************
*
*  Função: GRA  &Obter Valor
*  ****/    
    
    GRA_tpCondRet GRA_ObterValor( GRA_tppGrafo pGrafo , GRA_tppVertice pVertice , void** pDado ) {

        /* Verifica se vertice pertence ao grafo; */
        if (LIS_ProcurarValor(pGrafo->vertices, pVertice) != LIS_CondRetOK) {
            return GRA_CondRetNaoEhVertice;
        }
        
        *pDado = pVertice->pNode->pValor;
        
        return GRA_CondRetOK;

    }
    /* Fim função: GRA  &Alterar Valor */   
    
/***************************************************************************
*
*  Função: GRA  &Alterar Valor
*  ****/    
    
    GRA_tpCondRet GRA_AlterarValor( GRA_tppGrafo pGrafo , GRA_tppVertice pVertice , void* pDado ) {

        /* Verifica se vertice pertence ao grafo; */
        if (LIS_ProcurarValor(pGrafo->vertices, pVertice) != LIS_CondRetOK) {
            return GRA_CondRetNaoEhVertice;
        }
        
        pVertice->pNode->pValor = pDado;
            
        return GRA_CondRetOK;

    }
    /* Fim função: GRA  &Alterar Valor */
    
/*****  Código das funções encapsuladas no módulo  *****/

/***********************************************************************
*
*  $FC Função: GRA -Criar Vértice
*
*  $ED Descrição da função
*
***********************************************************************/

    //cria um vertice e sua estrutura interna
    static GRA_tpVertice* CriarVertice(GRA_tppGrafo grafo, void* pValor) {
            GRA_tpVertice* v = NULL;    
            tpNode* no = NULL;
            LIS_tppLista arestas = NULL;

            v = (GRA_tpVertice*) malloc( sizeof(GRA_tpVertice) );
            if (v == NULL) {
                return NULL;
            }

            no = (tpNode*) malloc( sizeof(tpNode) );
            if (no == NULL) {
                free(v);
                return NULL;
            }

            arestas = LIS_CriarLista(NULL);
            if(arestas == NULL) {
                free(v);
                free(no);
                return NULL;
            }

            no->arestas = NULL;
            no->pValor = pValor; //Really?
            v->pNode = no;

            return v;
    }

/***********************************************************************
*
*  $FC Função: GRA -Remover Aresta
*
*  $ED Descrição da função
*
***********************************************************************/

//as funcoes de exclusao de vertice e arestas sao chamadas internamente e externamente

//apaga aresta(u,v) e a aresta(v,u)
//essa tambem sera exportada

    static void RemoverAresta(GRA_tpVertice* u, GRA_tpVertice* v) {
        LIS_tppLista vizinhos;
        vizinhos = u->pNode->arestas;
        LIS_ProcurarValor(vizinhos, v);
        LIS_ExcluirElemento(vizinhos);
    }

/***********************************************************************
*
*  $FC Função: GRA -Excluir Aresta
*
*  $ED Descrição da função
*
***********************************************************************/

    static void ExcluirAresta (GRA_tppGrafo grafo, GRA_tpVertice* v, GRA_tpVertice* u) {
        RemoverAresta(u, v);
        RemoverAresta(v, u);
        //BFS pra detectar se é necessário gerar nova componente.
        if (BFS(v,u) == 0) { //Estão em componentes distintas
            LIS_InserirElementoApos(grafo->componentes, u);
        }
    }   
    
/***********************************************************************
*
*  $FC Função: GRA -BFS
*
*  $ED Descrição da função
*
***********************************************************************/

    static int BFS(GRA_tpVertice* v, GRA_tpVertice* u) {
        LIS_tppLista V = NULL; // LISTA VERTICE VISITADOS
        LIS_tppLista Q = NULL; //FILA
        LIS_tppLista arestas = NULL;
        GRA_tpVertice* t = NULL;
        GRA_tpVertice* s = NULL;
        int achou = 0;

        V = LIS_CriarLista(NULL); // dados são referenciados por outros, não devem ser apagados
        Q = LIS_CriarLista(NULL); // dados são referenciados por outros, não devem ser apagados

        LIS_InserirElementoApos(V, v);
        LIS_InserirElementoApos(Q, v); //Usado como uma Fila.

        while (LIS_NumeroDeElementos(Q) > 0) {

            LIS_IrInicioLista(Q);
            t = LIS_ObterValor(Q);
            LIS_ExcluirElemento(Q);

            if (t == u) {
                achou = 1; 
                break;
            }

            arestas = t->pNode->arestas;
            LIS_IrInicioLista(arestas);
            do {
                s = LIS_ObterValor(arestas);
                if (LIS_ProcurarValor(V, s) == LIS_CondRetNaoAchou) {
                    LIS_InserirElementoApos(V, s);
                    LIS_InserirElementoApos(Q, s);
                }
            } while(LIS_AvancarElementoCorrente(arestas, 1) != LIS_CondRetFimLista);
        }

        LIS_DestruirLista(V);
        LIS_DestruirLista(Q);

        return achou;
    }

/***********************************************************************
*
*  $FC Função: GRA -Obter Origem
*
*  $ED Descrição da função
*
***********************************************************************/
        
    static GRA_tpVertice* ObterOrigem (GRA_tppGrafo grafo, GRA_tpVertice* v) {
        GRA_tpVertice* u = NULL;
        LIS_tppLista origens = grafo->componentes;

        if (LIS_ProcurarValor(origens, v) == LIS_CondRetOK) {
            return v; //é a origem da própria componente
        }

        LIS_IrInicioLista(origens);

        do {
            u = LIS_ObterValor(origens);
            if (BFS(u,v) != 0) {
                return u;
            }
        } while(LIS_AvancarElementoCorrente(origens, 1) != LIS_CondRetFimLista);
        return NULL;
    }

/********** Fim do módulo de implementação: GRA  Grafo **********/
