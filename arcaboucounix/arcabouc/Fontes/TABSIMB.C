/***************************************************************************
*  $MCI Módulo de implementação: TBS  Tabela de símbolos
*
*  Arquivo gerado:              TABSIMB.c
*  Letras identificadoras:      TBS
*
*  Nome da base de software:    Arcabouço para a automação de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\ARCABOUC.BSW
*
*  Projeto: INF 1301 / 1628 Automatização dos testes de módulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     5       avs   18/mai/2008 corrigir e ampliar CESPDIN
*     4       avs   01/fev/2006 criar linguagem script simbólica
*     3       avs   08/dez/2004 uniformização dos exemplos
*     2       avs   07/jul/2003 unificação de todos os módulos em um só projeto
*     1       avs   16/abr/2003 início desenvolvimento
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>

#define TABSIMB_OWN
#include "TABSIMB.H"
#undef TABSIMB_OWN

#ifdef _DEBUG
   #include "CESPDIN.H"
#endif

/***********************************************************************
*
*  $TC Tipo de dados: TBS Elemento de lista de colisão
*
*
***********************************************************************/

   typedef struct tagLista {

         void * pDado ;
               /* Ponteiro para o dado contido no elemento */

         struct tagLista * pAnt ;
               /* Ponteiro para elemento predecessor */

         struct tagLista * pProx ;
               /* Ponteiro para lemento sucessor */

   } tpLista ;

/***********************************************************************
*
*  $TC Tipo de dados: TBS Tipo cabeça de tabela
*
*
***********************************************************************/

   typedef struct TBS_tagTabela {

         unsigned tamVtHash ;
               /* Tamanho do vetor de randomização */

         tpLista ** pVtHash ;
               /* Ponteiro para o vetor de randomização
               *
               *$ED Descrição
               *   Este ponteiro aponta para um vetor alocado dinamicamente.
               *   Vetores são tratados como ponteiros, donde surge o ponteiro para
               *   ponteiro ( "**" ) na declaração. */

         char * ( * ObterSimbolo) ( void * pDado ) ;
               /* Ponteiro para função obter símbolo */

         void ( * LiberarDado ) ( void * pDado ) ;
               /* Ponteiro para a função de destruição da estrutura de dados que contém o símbolo
               *
               *$ED Descrição
               *   Caso elemento da tabela de símbolos contenha um ou mais ponteiros
               *   para espaços que devem ser desalocados quando for desalocado o
               *   elemento, este atributo deve conter o ponteiro para a função que
               *   realiza o free destes espaços. */

   } TBS_tpTabela ;

/***** Protótipos das funções encapuladas no módulo *****/

   static void LiberarElemento( TBS_tpTabela * pTabela ,
                                tpLista      * pElem    ) ;

   static tpLista * ProcurarSimbolo( TBS_tpTabela * pTabela  ,
                                     char         * pSimbolo ,
                                     unsigned     * inxHash   ) ;

   static unsigned Hash( char * pSimbolo , unsigned tamVtHash ) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: TBS  &Criar tabela de símbolos
*  ****/

   TBS_tppTabela TBS_CriarTabela(
             int tamVetor                               ,
             char * ( * ObterSimbolo ) ( void * pDado ) ,
             void   ( * LiberarDado  ) ( void * pDado )  )
   {

      TBS_tpTabela * pTabela = NULL ;
      int i ;

      #ifdef _DEBUG
         TST_ASSERT( tamVetor      > 1    ) ;
         TST_ASSERT( ObterSimbolo != NULL ) ;
      #endif

      pTabela = ( TBS_tpTabela * ) malloc( sizeof( TBS_tpTabela )) ;
      if ( pTabela == NULL )
      {
         return NULL ;
      } /* if */

      pTabela->pVtHash = ( tpLista ** ) malloc( tamVetor * sizeof( tpLista * )) ;
      if ( pTabela->pVtHash == NULL )
      {
         free( pTabela ) ;
         return NULL ;
      } /* if */

      for( i = 0 ; i < tamVetor ; i++ )
      {
         pTabela->pVtHash[ i ] = NULL ;
      } /* for */

      pTabela->tamVtHash    = tamVetor ;
      pTabela->ObterSimbolo = ObterSimbolo ;
      pTabela->LiberarDado  = LiberarDado ;

      return pTabela ;

   } /* Fim função: TBS  &Criar tabela de símbolos */

/***************************************************************************
*
*  Função: TBS  &Destruir tabela de símbolos
*  ****/

   void TBS_DestruirTabela( TBS_tppTabela pTabela )
   {

      unsigned inxElem ;

      tpLista * pElem ;
      tpLista * pProx ;

      #ifdef _DEBUG
         TST_ASSERT( pTabela != NULL ) ;
      #endif

      for ( inxElem = 0 ; inxElem < pTabela->tamVtHash ; inxElem++ ) {

      /* Destruir todos elementos de lista de colisão */

         pElem = pTabela->pVtHash[ inxElem ] ;
         while ( pElem != NULL )
         {
            pProx = pElem->pProx ;
            LiberarElemento( pTabela , pElem ) ;
            pElem = pProx ;
         } /* while */

      } /* fim repete: Raiz de TBS  &Destruir tabela de símbolos */

      free( pTabela->pVtHash ) ;
      free( pTabela ) ;

   } /* Fim função: TBS  &Destruir tabela de símbolos */

#ifdef _DEBUG

/***************************************************************************
*
*  Função: TBS  &Validar tabela de símbolos
*  ****/

   TST_tpCondRet TBS_ValidarTabela( TBS_tppTabela pTabela )
   {

      unsigned inxHash ;

      tpLista * pElem ;

      #ifdef _DEBUG
         TST_ASSERT( pTabela != NULL ) ;
      #endif

      /* Validar existência de dados da cabeça */

         if ( pTabela->tamVtHash <= 1 )
         {
            return TST_NotificarFalha( "Tamanho incorreto do vetor de randomização." ) ;
         } /* if */

         if ( pTabela->pVtHash == NULL )
         {
            return TST_NotificarFalha( "Falta vetor de randomização." ) ;
         } /* if */

         if ( pTabela->ObterSimbolo == NULL )
         {
            return TST_NotificarFalha( "Falta função obter simbolo." ) ;
         } /* if */

      /* Validar listas de colisão */

         for ( inxHash = 0 ; inxHash < pTabela->tamVtHash ; inxHash ++ ) {

         /* Validar toda a lista de colisão */

            pElem = pTabela->pVtHash[ inxHash ] ;

            while ( pElem != NULL ) {

            /* Validar elemento da lista de colisão */

               if ( pElem->pDado == NULL )
               {
                  return TST_NotificarFalha( "Faltou dado em elemento de lista." ) ;
               } /* if */

               if ( Hash( pTabela->ObterSimbolo( pElem->pDado ) ,
                              pTabela->tamVtHash ) != inxHash )
               {
                  return TST_NotificarFalha( "Índice has de elemento está incorreto." ) ;
               } /* if */

               if ( pElem->pAnt != NULL )
               {
                  if ( pElem->pAnt->pProx != pElem )
                  {
                     return TST_NotificarFalha( "Erro de encadeamento à esquerda em elemento de lista." ) ;
                  } /* if */
               } else
               {
                  if ( pElem != pTabela->pVtHash[ inxHash ] )
                  {
                     return TST_NotificarFalha( "Erro de encadeamento origem em elemento de lista." ) ;
                  } /* if */
               } /* if */

               if ( pElem->pProx != NULL )
               {
                  if ( pElem->pProx->pAnt != pElem )
                  {
                     return TST_NotificarFalha( "Erro de encadeamento à direita em elemento de lista." ) ;
                  } /* if */
               } /* if */

               pElem = pElem->pProx ;

            } /* fim repete: Validar toda a lista de colisão */

         } /* fim repete: Validar listas de colisão */

      return TST_CondRetOK ;

   } /* Fim função: TBS  &Validar tabela de símbolos */

#endif

/***************************************************************************
*
*  Função: TBS  &Inserir símbolo
*  ****/

   TBS_tpCondRet TBS_InserirSimbolo( TBS_tppTabela pTabela ,
                                     void * pDado           )
   {

      unsigned inxHash ;

      tpLista * pElem ;

      #ifdef _DEBUG
         TST_ASSERT( pTabela != NULL ) ;
         TST_ASSERT( pDado   != NULL ) ;
      #endif

      /* Procurar símbolo */

         if ( ProcurarSimbolo( pTabela ,
                               pTabela->ObterSimbolo( pDado ) ,
                               &inxHash ) != NULL )
         {
            return TBS_CondRetSimboloExiste ;
         } /* if */

      /* Inserir o símbolo */

         pElem = ( tpLista * ) malloc( sizeof( tpLista )) ;
         pElem->pDado = pDado ;
         pElem->pAnt  = NULL ;
         pElem->pProx = pTabela->pVtHash[ inxHash ] ;
         if ( pElem->pProx != NULL )
         {
            pElem->pProx->pAnt = pElem ;
         } /* if */

         pTabela->pVtHash[ inxHash ] = pElem ;

         return TBS_CondRetOK ;

   } /* Fim função: TBS  &Inserir símbolo */

/***************************************************************************
*
*  Função: TBS  &Procurar símbolo
*  ****/

   void * TBS_ProcurarSimbolo( TBS_tppTabela pTabela ,
                               char * pSimbolo         )
   {

      tpLista * pElem ;
      unsigned inxHash ;

      #ifdef _DEBUG
         TST_ASSERT( pTabela  != NULL ) ;
         TST_ASSERT( pSimbolo != NULL ) ;
      #endif

      pElem = ProcurarSimbolo( pTabela , pSimbolo , &inxHash ) ;
      if ( pElem == NULL )
      {
         return NULL ;
      } /* if */

      return pElem->pDado ;

   } /* Fim função: TBS  &Procurar símbolo */

/***************************************************************************
*
*  Função: TBS  &Excluir símbolo
*  ****/

   TBS_tpCondRet TBS_ExcluirSimbolo( TBS_tppTabela pTabela ,
                                     char * pSimbolo        )
   {

      unsigned inxHash ;
      tpLista * pElem ;

      #ifdef _DEBUG
         TST_ASSERT( pTabela  != NULL ) ;
         TST_ASSERT( pSimbolo != NULL ) ;
      #endif

      pElem = ProcurarSimbolo( pTabela , pSimbolo , &inxHash ) ;
      if ( pElem == NULL )
      {
         return TBS_CondRetSimboloNaoExiste ;
      } /* if */

      if ( pElem->pProx != NULL )
      {
         pElem->pProx->pAnt = pElem->pAnt ;
      } /* if */

      if ( pElem->pAnt != NULL )
      {
         pElem->pAnt->pProx = pElem->pProx ;
      } else {
         pTabela->pVtHash[ inxHash ] = pElem->pProx ;
      } /* if */

      LiberarElemento( pTabela , pElem ) ;

      return TBS_CondRetOK ;

   } /* Fim função: TBS  &Excluir símbolo */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: TBS  -Liberar elemento da tabela
*
*  $ED Descrição da função
*     Elimina os espaços apontados pelo valor do elemento, o valor e o
*     próprio elemento.
*
***********************************************************************/

   void LiberarElemento( TBS_tpTabela * pTabela ,
                         tpLista      * pElem    )
   {

      if ( pTabela->LiberarDado != NULL )
      {
         pTabela->LiberarDado( pElem->pDado ) ;
      } /* if */
      free( pElem->pDado ) ;
      free( pElem ) ;

   } /* Fim função: TBS  -Liberar elemento da tabela */


/***********************************************************************
*
*  $FC Função: TBS  -Procurar o símbolo
*
***********************************************************************/

   tpLista * ProcurarSimbolo( TBS_tpTabela * pTabela  ,
                              char         * pSimbolo ,
                              unsigned     * inxHash   )
   {

      tpLista * pElem ;

      *inxHash = Hash( pSimbolo , pTabela->tamVtHash ) ;
      pElem    = pTabela->pVtHash[ *inxHash ] ;
      while ( pElem != NULL )
      {
         if ( strcmp( pTabela->ObterSimbolo( pElem->pDado ) ,
                      pSimbolo ) == 0 )
         {
            return pElem ;
         } /* if */
         pElem = pElem->pProx ;
      } /* while */

      return NULL ;

   } /* Fim função: TBS  -Procurar o símbolo */


/***********************************************************************
*
*  $FC Função: TBS  -Computar hash
*
***********************************************************************/

   unsigned Hash( char * pSimbolo , unsigned tamVtHash )
   {

      unsigned inxHash = 0 ;
      unsigned i = 0 ;

      #ifdef _DEBUG
         TST_ASSERT( pSimbolo != NULL ) ;
      #endif

      for( i = 0 ; i < strlen( pSimbolo ) ; i ++ )
      {
         inxHash = ( inxHash << 2 ) + pSimbolo[ i ] ;
      } /* for */

      return inxHash % tamVtHash ;

   } /* Fim função: TBS  -Computar hash */

/********** Fim do módulo de implementação: TBS  Tabela de símbolos **********/

