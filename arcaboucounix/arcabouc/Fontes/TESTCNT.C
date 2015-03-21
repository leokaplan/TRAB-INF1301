/***************************************************************************
*  $MCI Módulo de implementação: TCNT Teste contadores de passagem
*
*  Arquivo gerado:              TestCnt.c
*  Letras identificadoras:      TCNT
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

#include "TST_ESPC.H"

#include "GENERICO.H"
#include "LERPARM.H"
#include "CONTA.H"

#define     DIM_MSG             500
#define     DIM_NOME_ARQ        500
#define     DIM_NOME_CONTADOR   500

/* Comandos de contagem */

static const char CONTAR_PASSAGEM_CMD[ ]          = "=contar" ;
static const char INICIAR_ITERADOR_CMD[ ]         = "=iniciariteradorcontadores" ;
static const char EXIBIR_CONTADOR_CORR_CMD[ ]     = "=exibircontadorcorrente" ;
static const char OBTER_CONTAGEM_CORR_CMD[ ]      = "=contagemcorrente" ;
static const char OBTER_NOME_CONTADOR_CORR_CMD[ ] = "=nomecorrente" ;
static const char IR_PROX_CONTADOR_CMD[ ]         = "=iterarproximocontador" ;
static const char EH_ITERADOR_ATIVO_CMD[ ]        = "=ehiteradorativo" ;
static const char VERIFICAR_ESTRUTURA_CMD[ ]      = "=verificarestruturacontadores" ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TCNT &Teste de contador de passagem
*
*  $ED Descrição da função
*     =contar                    <nome contador> <num linha> <condRet>
*     =iniciariteradorcontadores
*     =exibircontadorcorrente
*     =contagemcorrente
*     =nomecorrente
*     =iterarproximocontador
*     =ehiteradorativo
*     =verificarestruturacontadores
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      char   Msg[ DIM_MSG ] ;
      char   NomeContador[ DIM_NOME_CONTADOR ] ;
      char * pNome ;

      int    numLidos ,
             IntEsp    ;

      CNT_tpCondRet CondRetEsp ;

      int  numLinha ;

      /* Tratar CNT  &Contar */

         if ( strcmp( ComandoTeste , CONTAR_PASSAGEM_CMD ) == 0 )
         {

            /*
               CNT_tpCondRet CNT_Contar( char * NomeContador , int numLinha )
            */

            CondRetEsp = CNT_CondRetOK ;

            numLidos = LER_LerParametros( "sii" ,
                               NomeContador , &numLinha , &CondRetEsp ) ;
            if ( numLidos < 2 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( CondRetEsp ,
                      CNT_Contar( NomeContador , numLinha ) ,
                      "Condição de retorno errada." ) ;

         } /* fim ativa: Tratar CNT  &Contar */

      /* Tratar CNT  &Iterador: iniciar percorrimento da lista em ordem alfabética */

         else if ( strcmp( ComandoTeste , INICIAR_ITERADOR_CMD ) == 0 )
         {

            /*
               void CNT_IniciarIterador( )
            */

            CNT_IniciarIterador( ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar CNT  &Iterador: iniciar percorrimento da lista em ordem alfabética */

      /* Tratar CNT  &Iterador: exibir contador corrente */

         else if ( strcmp( ComandoTeste , EXIBIR_CONTADOR_CORR_CMD ) == 0 )
         {

            pNome = CNT_ObterContadorCorrente( ) ;
            if ( pNome != NULL )
            {
               sprintf( Msg ,  "Contagem: %5ld  Contador: \"%s\"" ,
                         CNT_ObterContagemCorrente( ) , pNome ) ;

               TST_ExibirPrefixo( "!!!" , Msg ) ;
            } else
            {
               return TST_NotificarFalha( "Iterador não está ativo." ) ;
            } /* if */

            return TST_CondRetOK ;

         } /* fim ativa: Tratar CNT  &Iterador: exibir contador corrente */

      /* Tratar CNT  &Iterador: obter contagem corrente */

         else if ( strcmp( ComandoTeste , OBTER_CONTAGEM_CORR_CMD ) == 0 )
         {

            /*
               long CNT_ObterContagemCorrente( )
            */

            numLidos = LER_LerParametros( "i" ,
                               &IntEsp ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( IntEsp , CNT_ObterContagemCorrente( ) ,
                      "Contagem errada." ) ;

         } /* fim ativa: Tratar CNT  &Iterador: obter contagem corrente */

      /* Tratar CNT  &Iterator: obter nome de contador corrente */

         else if ( strcmp( ComandoTeste , OBTER_NOME_CONTADOR_CORR_CMD ) == 0 )
         {

            /*
               char * CNT_ObterContadorCorrente( )
            */

            numLidos = LER_LerParametros( "s" ,
                               Msg ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            pNome = CNT_ObterContadorCorrente( ) ;
            if ( pNome == NULL )
            {
               return TST_NotificarFalha( "Iterador não está ativo." ) ;
            } /* if */

            return TST_CompararString( Msg , pNome ,
                      "Nome de contador errado. " ) ;

         } /* fim ativa: Tratar CNT  &Iterator: obter nome de contador corrente */

      /* Tratar CNT  &Iterador: avançar para o próximo contador */

         else if ( strcmp( ComandoTeste , IR_PROX_CONTADOR_CMD ) == 0 )
         {

            /*
               void CNT_IrProximoContador( )
            */

            CNT_IrProximoContador( ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar CNT  &Iterador: avançar para o próximo contador */

      /* Tratar CNT  &Iterator: verificar se o iterador está ativo */

         else if ( strcmp( ComandoTeste , EH_ITERADOR_ATIVO_CMD ) == 0 )
         {

            /*
               int CNT_EhAtivoIterador( )
            */

            numLidos = LER_LerParametros( "i" ,
                               &IntEsp ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararBool( IntEsp ,
                      CNT_EhAtivoIterador( ) ,
                      "Estado iterador ativo errado." ) ;

         } /* fim ativa: Tratar CNT  &Iterator: verificar se o iterador está ativo */

      /* Tratar CNT  &Verificar a estrutura dos contadores */
      #ifdef _DEBUG

         else if ( strcmp( ComandoTeste , VERIFICAR_ESTRUTURA_CMD ) == 0 )
         {

            /*
               int CNT_VerificarEstruturaContadores( )
            */

            numLidos = LER_LerParametros( "i" ,
                               &IntEsp ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( IntEsp ,
                      CNT_VerificarEstruturaContadores( ) ,
                      "Estado errado da estrutura de dados." ) ;

         } /* fim ativa: Tratar CNT  &Verificar a estrutura dos contadores */
      #endif

      return TST_CondRetNaoConhec ;

   } /* Fim função: TCNT &Teste de contador de passagem */

/********** Fim do módulo de implementação: TCNT Teste contadores de passagem **********/

