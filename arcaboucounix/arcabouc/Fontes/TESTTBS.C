/***************************************************************************
*  $MCI Módulo de implementação: TTBS Teste tabela de símbolos
*
*  Arquivo gerado:              TestTBS.c
*  Letras identificadoras:      TTBS
*
*  Nome da base de software:    Arcabouço para a automação de testes de programas redigidos em C
*  Arquivo da base de software: C:\AUTOTEST\PROJETOS\ARCABOUC.BSW
*
*  Projeto: INF 1301 / 1628 Automatização dos testes de módulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     4       avs   01/fev/2006 criar linguagem script simbólica
*     3       avs   08/dez/2004 uniformização dos exemplos
*     2       avs   07/jul/2003 unificação de todos os módulos em um só projeto
*     1       avs   16/abr/2003 início desenvolvimento
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include "TST_ESPC.H"

#include "GENERICO.H"
#include "TABSIMB.H"
#include "LERPARM.H"

#ifdef _DEBUG
   #include "CESPDIN.H"
#endif

static const char RESET_TAB_CMD           [ ] = "=resettestetab"  ;
static const char CRIAR_TAB_CMD           [ ] = "=criartab"       ;
static const char DESTRUIR_TAB_CMD        [ ] = "=destruirtab"    ;
static const char INS_SIMB_UM_CMD         [ ] = "=inssimboloum"   ;
static const char INS_SIMB_DOIS_CMD       [ ] = "=inssimbolodois" ;
static const char INS_SIMB_TRES_CMD       [ ] = "=inssimbolotres" ;
static const char PROCURAR_SIMB_UM_CMD    [ ] = "=procurarsimboloum"  ;
static const char PROCURAR_SIMB_DOIS_CMD  [ ] = "=procurarsimbolodois"  ;
static const char PROCURAR_SIMB_TRES_CMD  [ ] = "=procurarsimbolotres"  ;
static const char EXC_SIMB_CMD            [ ] = "=excluirsimbolo" ;
static const char VALIDAR_TAB_CMD         [ ] = "=validartab"     ;


#define TRUE  1
#define FALSE 0

#define ID_TIPO_1 1
#define ID_TIPO_2 2
#define ID_TIPO_3 3

#define VAZIO     0
#define NAO_VAZIO 1

#define DIM_VT_TABELAS  10
#define DIM_SIMB        50

typedef struct
{
   int Conta ;
   char SimboloUm[ DIM_SIMB ] ;
} tpTipo1 ;

typedef struct
{
   char SimboloDois[ DIM_SIMB ] ;
   char Id ;
} tpTipo2 ;

typedef struct
{
   char   SimboloTres[ DIM_SIMB ] ;
   char * pString ;
} tpTipo3 ;

TBS_tppTabela   vtTabelas[ DIM_VT_TABELAS ] ;

/***** Protótipos das funções encapuladas no módulo *****/

   static char * ObterSimboloTipoUm( void * pDado ) ;

   static char * ObterSimboloTipoDois( void * pDado ) ;

   static char * ObterSimboloTipoTres( void * pDado ) ;

   static void LiberarDadoTipoTres( void * pDado ) ;

   static int ValidarInxTabela( int inxTabela , int Modo ) ;

   static int ValidarIdTipo( int idTipo ) ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TTBS &Testar tabela de símbolos
*
*  $ED Descrição da função
*     Podem ser criadas até 10 tabelas, identificadas pelos índices 0 a 10
*     Estão disponíveis 2 tipos de símbolos 1 e 2
*
*     Comandos disponíveis:
*
*     =resettestetab
*           - anula o vetor de tabelas. Provoca vazamento de memória
*     =criartab                     inxTab  tamTab  idTipo
*     =destruirtab                  inxTab
*     =validartab                   inxTab
*     =inssimboloum                 inxTab  valInt  Simbolo CondRetEsp
*     =inssimbolodois               inxTab  valChar Simbolo CondRetEsp
*     =procurarsimboloum            inxTab  Simbolo valInt
*     =procurarsimbolodois          inxTab  Simbolo valChar
*     =excluirsimbolo               inxTab  Simbolo CondRetEsp
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      int inxTabela  = -1 ,
          idTipo     = -1 ,
          numLidos   = -1 ,
          CondRetEsp = -1  ;

      TST_tpCondRet CondRet ;

      char StringDado[  DIM_SIMB ] ;
      char StringValor[ DIM_SIMB ] ;

      int i ;

      int tamTabela = -1 ;

      int ValInt = -1 ;

      tpTipo1 * pDado1 ;

      char ValChar = '\0' ;

      tpTipo2 * pDado2 ;

      tpTipo3 * pDado3 ;

      int ValIntEsp = -1 ;

      char ValCharEsp = '*' ;

      StringDado[ 0 ] = 0 ;

      /* Efetuar reset de teste de tabela */

         if ( strcmp( ComandoTeste , RESET_TAB_CMD ) == 0 )
         {

            for( i = 0 ; i < DIM_VT_TABELAS ; i++ )
            {
               vtTabelas[ i ] = NULL ;
            } /* for */

            return TST_CondRetOK ;

         } /* fim ativa: Efetuar reset de teste de tabela */

      /* Testar CriarTabela */

         else if ( strcmp( ComandoTeste , CRIAR_TAB_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iii" ,
                       &inxTabela , &tamTabela , &idTipo ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxTabela( inxTabela , VAZIO ))
              || ( ! ValidarIdTipo( idTipo ))
              || ( tamTabela <= 1 ))
            {
               return TST_CondRetParm ;
            } /* if */

            if ( idTipo == ID_TIPO_1 )
            {
               vtTabelas[ inxTabela ] =
                    TBS_CriarTabela( tamTabela , ObterSimboloTipoUm , NULL ) ;
            } else if ( idTipo == ID_TIPO_2 )
            {
               vtTabelas[ inxTabela ] =
                    TBS_CriarTabela( tamTabela , ObterSimboloTipoDois , NULL ) ;
            } else
            {
               vtTabelas[ inxTabela ] =
                    TBS_CriarTabela( tamTabela , ObterSimboloTipoTres ,
                                                 LiberarDadoTipoTres    ) ;
            } /* if */

            return TST_CompararPonteiroNulo( 1 , vtTabelas[ inxTabela ] ,
               "Erro em ponteiro de nova tabela."  ) ;

         } /* fim ativa: Testar CriarTabela */

      /* Testar Destruir tabela */

         else if ( strcmp( ComandoTeste , DESTRUIR_TAB_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxTabela ) ;

            if ( ( numLidos != 1 )
              || ( ! ValidarInxTabela( inxTabela , NAO_VAZIO )))
            {
               return TST_CondRetParm ;
            } /* if */

            TBS_DestruirTabela( vtTabelas[ inxTabela ] ) ;
            vtTabelas[ inxTabela ] = NULL ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar Destruir tabela */

      /* Testar Inserir simbolo tipo um */

         else if ( strcmp( ComandoTeste , INS_SIMB_UM_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iisi" ,
                       &inxTabela , &ValInt , StringDado , &CondRetEsp ) ;

            if ( ( numLidos != 4 )
              || ( ! ValidarInxTabela( inxTabela , NAO_VAZIO )) )
            {
               return TST_CondRetParm ;
            } /* if */

            pDado1 = ( tpTipo1 * ) malloc( sizeof( tpTipo1 )) ;
            if ( pDado1 == NULL )
            {
               return TST_CondRetMemoria ;
            } /* if */

            pDado1->Conta   = ValInt ;
            strcpy( pDado1->SimboloUm , StringDado ) ;


            CondRet = TBS_InserirSimbolo( vtTabelas[ inxTabela ] , pDado1 ) ;

            if ( CondRet != TBS_CondRetOK )
            {
               free( pDado1 ) ;
            } /* if */

            return TST_CompararInt( CondRetEsp , CondRet ,
                     "Condição de retorno errada em símbolo tipo um."                   ) ;

         } /* fim ativa: Testar Inserir simbolo tipo um */

      /* Testar Inserir simbolo tipo dois */

         else if ( strcmp( ComandoTeste , INS_SIMB_DOIS_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "icsi" ,
                       &inxTabela , &ValChar , StringDado , &CondRetEsp ) ;

            if ( ( numLidos != 4 )
              || ( ! ValidarInxTabela( inxTabela , NAO_VAZIO )) )
            {
               return TST_CondRetParm ;
            } /* if */

            pDado2 = ( tpTipo2 * ) malloc( sizeof( tpTipo2 )) ;
            if ( pDado2 == NULL )
            {
               return TST_CondRetMemoria ;
            } /* if */
            pDado2->Id   = ValChar ;
            strcpy( pDado2->SimboloDois , StringDado ) ;

            return TST_CompararInt( CondRetEsp ,
                          TBS_InserirSimbolo( vtTabelas[ inxTabela ] , pDado2 ) ,
                     "Condição de retorno errada em símbolo tipo dois."                   ) ;

         } /* fim ativa: Testar Inserir simbolo tipo dois */

      /* Testar Inserir simbolo tipo tres */

         else if ( strcmp( ComandoTeste , INS_SIMB_TRES_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "issi" ,
                       &inxTabela , StringDado , StringValor , &CondRetEsp ) ;

            if ( ( numLidos != 4 )
              || ( ! ValidarInxTabela( inxTabela , NAO_VAZIO )) )
            {
               return TST_CondRetParm ;
            } /* if */

            pDado3 = ( tpTipo3 * ) malloc( sizeof( tpTipo3 )) ;
            if ( pDado3 == NULL )
            {
               return TST_CondRetMemoria ;
            } /* if */

            pDado3->pString = malloc( strlen( StringValor ) + 1 ) ;
            strcpy( pDado3->pString , StringValor ) ;

            strcpy( pDado3->SimboloTres , StringDado ) ;

            return TST_CompararInt( CondRetEsp ,
                          TBS_InserirSimbolo( vtTabelas[ inxTabela ] , pDado3 ) ,
                     "Condição de retorno errada em símbolo tipo dois."                   ) ;

         } /* fim ativa: Testar Inserir simbolo tipo tres */

      /* Testar excluir simbolo */

         else if ( strcmp( ComandoTeste , EXC_SIMB_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "isi" ,
                  &inxTabela , StringDado , &CondRetEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxTabela( inxTabela , NAO_VAZIO )) )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( CondRetEsp ,
                      TBS_ExcluirSimbolo( vtTabelas[ inxTabela ] , StringDado ) ,
                     "Condição de retorno errada ao excluir."                   ) ;

         } /* fim ativa: Testar excluir simbolo */

      /* Testar procurar símbolo tipo 1 */

         else if ( strcmp( ComandoTeste , PROCURAR_SIMB_UM_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "isi" ,
                       &inxTabela , StringDado , &ValIntEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxTabela( inxTabela , NAO_VAZIO )) )
            {
               return TST_CondRetParm ;
            } /* if */

            pDado1 = ( tpTipo1 * ) TBS_ProcurarSimbolo( vtTabelas[ inxTabela ] ,
                                            StringDado ) ;
            if ( ValIntEsp == -1 )
            {
               return TST_CompararPonteiroNulo( 0 , pDado1 ,
                         "Dado tipo um não deveria existir." ) ;
            } /* if */

            if ( pDado1 == NULL )
            {
               return TST_CompararPonteiroNulo( 1 , pDado1 ,
                         "Dado tipo um deveria existir." ) ;
            } /* if */

            return TST_CompararInt( ValIntEsp , pDado1->Conta ,
                         "Valor tipo um errado." ) ;

         } /* fim ativa: Testar procurar símbolo tipo 1 */

      /* Testar procurar símbolo tipo 2 */

         else if ( strcmp( ComandoTeste , PROCURAR_SIMB_DOIS_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "isc" ,
                       &inxTabela , StringDado , &ValCharEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxTabela( inxTabela , NAO_VAZIO )) )
            {
               return TST_CondRetParm ;
            } /* if */

            pDado2 = ( tpTipo2 * ) TBS_ProcurarSimbolo( vtTabelas[ inxTabela ] ,
                                            StringDado ) ;
            if ( ValCharEsp == '*' )
            {
               return TST_CompararPonteiroNulo( 0 , pDado2 ,
                         "Dado tipo dois não deveria existir." ) ;
            } /* if */

            if ( pDado2 == NULL )
            {
               return TST_CompararPonteiroNulo( 1 , pDado2 ,
                         "Dado tipo dois deveria existir." ) ;
            } /* if */

            return TST_CompararChar( ValCharEsp , pDado2->Id ,
                         "Valor tipo dois errado." ) ;

         } /* fim ativa: Testar procurar símbolo tipo 2 */

      /* Testar procurar símbolo tipo 3 */

         else if ( strcmp( ComandoTeste , PROCURAR_SIMB_TRES_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "iss" ,
                       &inxTabela , StringDado , StringValor ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxTabela( inxTabela , NAO_VAZIO )) )
            {
               return TST_CondRetParm ;
            } /* if */

            pDado3 = ( tpTipo3 * ) TBS_ProcurarSimbolo( vtTabelas[ inxTabela ] ,
                                            StringDado ) ;

            if ( pDado3 == NULL )
            {
               return TST_CompararPonteiroNulo( 1 , pDado3 ,
                         "Dado tipo três deveria existir." ) ;
            } /* if */

            return TST_CompararString( StringValor , pDado3->pString ,
                         "Valor tipo três errado." ) ;

         } /* fim ativa: Testar procurar símbolo tipo 3 */

      /* Testar validar Tabela de símbolos */
      #ifdef _DEBUG

         else if ( strcmp( ComandoTeste , VALIDAR_TAB_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxTabela ) ;

            if ( ( numLidos != 1 )
              || ( ! ValidarInxTabela( inxTabela , NAO_VAZIO )))
            {
               return TST_CondRetParm ;
            } /* if */

            return TBS_ValidarTabela( vtTabelas[ inxTabela ] ) ;

         } /* fim ativa: Testar validar Tabela de símbolos */
      #endif

      return TST_CondRetNaoConhec ;

   } /* Fim função: TTBS &Testar tabela de símbolos */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: TTBS -Obter símbolo tipo 1
*
***********************************************************************/

   char * ObterSimboloTipoUm( void * pDado )
   {

      #ifdef _DEBUG
         TST_ASSERT( pDado != NULL ) ;
      #endif

      return (( tpTipo1 * ) pDado )->SimboloUm ;

   } /* Fim função: TTBS -Obter símbolo tipo 1 */


/***********************************************************************
*
*  $FC Função: TTBS -Obter símbolo tipo 2
*
***********************************************************************/

   char * ObterSimboloTipoDois( void * pDado )
   {

      #ifdef _DEBUG
         TST_ASSERT( pDado != NULL ) ;
      #endif

      return (( tpTipo2 * ) pDado )->SimboloDois ;

   } /* Fim função: TTBS -Obter símbolo tipo 2 */


/***********************************************************************
*
*  $FC Função: TTBS -Obter símbolo tipo 3
*
***********************************************************************/

   char * ObterSimboloTipoTres( void * pDado )
   {

      #ifdef _DEBUG
         TST_ASSERT( pDado != NULL ) ;
      #endif

      return (( tpTipo3 * ) pDado )->SimboloTres ;

   } /* Fim função: TTBS -Obter símbolo tipo 3 */


/***********************************************************************
*
*  $FC Função: TTBS -Liberar dado tipo 3
*
***********************************************************************/

   void LiberarDadoTipoTres( void * pDado )
   {

      #ifdef _DEBUG
         TST_ASSERT( pDado != NULL ) ;
      #endif

      free( (( tpTipo3 * ) pDado )->pString ) ;

   } /* Fim função: TTBS -Liberar dado tipo 3 */


/***********************************************************************
*
*  $FC Função: TTBS -Validar indice de tabela
*
***********************************************************************/

   int ValidarInxTabela( int inxTabela , int Modo )
   {

      if ( ( inxTabela <  0 )
        || ( inxTabela >= DIM_VT_TABELAS ))
      {
         return FALSE ;
      } /* if */
         
      if ( Modo == VAZIO )
      {
         if ( vtTabelas[ inxTabela ] != 0 )
         {
            return FALSE ;
         } /* if */
      } else
      {
         if ( vtTabelas[ inxTabela ] == 0 )
         {
            return FALSE ;
         } /* if */
      } /* if */
         
      return TRUE ;

   } /* Fim função: TTBS -Validar indice de tabela */


/***********************************************************************
*
*  $FC Função: TTBS -Validar id tipo
*
***********************************************************************/

   int ValidarIdTipo( int idTipo )
   {

      if ( ( idTipo == ID_TIPO_1 )
        || ( idTipo == ID_TIPO_2 )
        || ( idTipo == ID_TIPO_3 ))
      {
         return TRUE ;
      } /* if */

      return FALSE ;

   } /* Fim função: TTBS -Validar id tipo */

/********** Fim do módulo de implementação: TTBS Teste tabela de símbolos **********/

