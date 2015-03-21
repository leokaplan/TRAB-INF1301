/***************************************************************************
*  $MCI Módulo de implementação: TALT Teste do gerador de números aleatórios
*
*  Arquivo gerado:              TESTALT.c
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
*  $ED Descrição do módulo
*     Testa e exemplifica o uso do gerador de números aleatórios
*
***************************************************************************/

#include    <stdio.h>
#include    <string.h>
#include    <math.h>

#include "TST_ESPC.H"

#include "GENERICO.H"
#include "LERPARM.H"

#include "GERAALT.H"

#define   NUM_POR_LINHA  5
#define   NUM_GERA       10000000
#define   NUM_GERA_FLOAT 10000000.0
#define   DIM_VET   100

#define   ERRO_LER       ">>>"

static int Vet[ DIM_VET + 1 ] ;

static char TestarInicializarCmd[ ] = "=inicializar" ;
static char ExibirUniformeCmd[   ]  = "=exibiruniforme" ;
static char ExibirFrequenciaCmd[ ]  = "=exibirfrequencia" ;
static char TestarPermutacaoCmd[ ]  = "=exibirpermutacao" ;

/***** Protótipos das funções encapuladas no módulo *****/

   static TST_tpCondRet ExibirPermutacao( ) ;

   static void ExibirDistribuicao( double numGera ,  int Inf , int Sup , char * Mensagem ) ;

   static void ZerarVetor( void ) ;

   static TST_tpCondRet ExibirUniforme( void ) ;

   static TST_tpCondRet ExibirFrequencia( void ) ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TALT &Efetuar o teste de função do módulo aleatorio
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      unsigned int Semente ;
      int      numLidos ;

      /* Testar Inicializar gerador de números aleatórios */

         if ( strcmp( ComandoTeste , TestarInicializarCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" , &Semente ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            ALT_Inicializar( Semente ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar Inicializar gerador de números aleatórios */

      /* Testar distribuição uniforme */

         else if ( strcmp( ComandoTeste , ExibirUniformeCmd ) == 0 )
         {

            return ExibirUniforme( ) ;

         } /* fim ativa: Testar distribuição uniforme */

      /* Testar distribuição freqüências */

         else if ( strcmp( ComandoTeste , ExibirFrequenciaCmd ) == 0 )
         {

            return ExibirFrequencia( ) ;

         } /* fim ativa: Testar distribuição freqüências */

      /* Testar permutação randômica */

         else if ( strcmp( ComandoTeste , TestarPermutacaoCmd ) == 0 )
         {

            return ExibirPermutacao( ) ;

         } /* fim ativa: Testar permutação randômica */

      return TST_CondRetNaoConhec ;

   } /* Fim função: TALT &Efetuar o teste de função do módulo aleatorio */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: TALT -Exibir permutação randômica uniformemente distribuída
*
***********************************************************************/

   TST_tpCondRet ExibirPermutacao( )
   {

      int Vetor[ DIM_VET ] ;

      int numGera  = -1 ,
          numLidos = -1 ,
          numElem  = -1  ;

      int i ;

      /* Ler parâmetros do vetor */

         numLidos = LER_LerParametros( "ii" , &numGera , &numElem ) ;
         if ( ( numLidos != 2 )
           || ( numGera  <  1 )
           || ( numElem  <  2 )
           || ( numElem  >  DIM_VET ))
         {
            return TST_CondRetParm ;
         } /* if */

      /* Embaralhar vetor */

         for( i = 0 ; i < numElem ; i++ )
         {
            Vetor[ i ] = i ;
         } /* for */

         ALT_GerarPermutacao( numElem , Vetor ) ;

      /* Exibir Permutação */

         fprintf( TST_ObterArqLog( ) , "\n\nPermutação" ) ;

         for( i = 0 ; i < numElem ; i ++ )
         {
            fprintf( TST_ObterArqLog( ) , " %4d" , Vetor[ i ] ) ;
         } /* for */
         fprintf( TST_ObterArqLog( ) , "\n" ) ;

      return TST_CondRetOK ;

   } /* Fim função: TALT -Exibir permutação randômica uniformemente distribuída */


/***********************************************************************
*
*  $FC Função: TALT -Exibir distribuicao
*
***********************************************************************/

   void ExibirDistribuicao( double numGera ,  int Inf , int Sup , char * Mensagem )
   {

      int i ,
          j ;

      double A ;


      fprintf( TST_ObterArqLog( ) , "\n\n%s, contagem\n" , Mensagem ) ;

      j = 0 ;

      for ( i = Inf ; i <= Sup ; i++ )
      {
         if ( j % NUM_POR_LINHA == 0 )
         {
            fprintf( TST_ObterArqLog( ) , "\n" ) ;
         }
         j++ ;
         fprintf( TST_ObterArqLog( ) , " %4d = %6d" , i , Vet[ i ] ) ;
      } /* for */

      fprintf( TST_ObterArqLog( ) , "\n\n%s, fração do total gerado\n" , Mensagem ) ;

      j = 0 ;

      for ( i = Inf ; i <= Sup ; i++ )
      {
         if ( j % NUM_POR_LINHA == 0 )
         {
            fprintf( TST_ObterArqLog( ) , "\n" ) ;
         }
         j++ ;
         A = Vet[ i ] ;
         A = A / numGera ;
         fprintf( TST_ObterArqLog( ) , " %4d= %7lf" , i , A ) ;
      } /* for */

      fprintf( TST_ObterArqLog( ) , "\n\n" ) ;

   } /* Fim função: TALT -Exibir distribuicao */


/***********************************************************************
*
*  $FC Função: TALT -Zerar vetor de números gerados
*
***********************************************************************/

   void ZerarVetor( void )
   {

      int i ;

      for ( i = 0 ; i <= DIM_VET ; i++ )
      {
         Vet[ i ] = 0 ;
      } /* for */

   } /* Fim função: TALT -Zerar vetor de números gerados */


/***********************************************************************
*
*  $FC Função: TALT -Exibir distribuição uniforme
*
***********************************************************************/

   TST_tpCondRet ExibirUniforme( void )
   {

      int numLidos = 0  ,
          numGera  = 0  ,
          Inf      = -1 ,
          Sup      = -1 ,
          i ;

      double DesvioEsp  = 0. ,
             Tolerancia = 0.  ;


      double MediaEsperada,
             Desvio       ,
             A             ;

      /* Ler parâmetros distribuição uniforme */

         numLidos = LER_LerParametros( "iiiff" , &numGera , &Inf , &Sup ,
                                       &DesvioEsp , &Tolerancia ) ;

         if ( ( numLidos != 5 )
           || ( numGera < 2   )
           || ( Inf     < 0   )
           || ( Inf     > Sup )
           || ( Sup     > DIM_VET ))

         {
            return TST_CondRetParm ;
         } /* if */

      /* Gerar distribuição uniforme */

         ZerarVetor( ) ;

         for ( i = 0 ; i < numGera ; i++ )
         {
            Vet[ ALT_GerarDistUniforme( Inf , Sup ) ] ++ ;
         } /* for */

      /* Calcular média e desvio padrão uniforme */

         MediaEsperada = ( 1. / ( Sup - Inf + 1 )) ;

         Desvio        = 0. ;

         for( i = Inf ; i <= Sup ; i ++ )
         {
            A = Vet[ i ] ;
            A = A / numGera ;

            Desvio      += pow( ( A - MediaEsperada ) , 2 ) ;
         } /* for */

         Desvio        = sqrt( Desvio ) ;

         fprintf( TST_ObterArqLog( ) , "\n\n   Desvio: %8lf" ,
                  Desvio ) ;

      /* Exibir distribuição uniforme */

         ExibirDistribuicao( numGera , Inf , Sup , "Distribuição uniforme." ) ;

         return TST_CompararFloat( DesvioEsp , Desvio , Tolerancia ,
                  "Desvio discrepa muito." ) ;

   } /* Fim função: TALT -Exibir distribuição uniforme */


/***********************************************************************
*
*  $FC Função: TALT -Exibir distribuição de freqüências
*
***********************************************************************/

   TST_tpCondRet ExibirFrequencia( void )
   {

      int numLidos = -1 ,
          numGera  = -1 ,
          numElem  = -1 ,
          Anterior ,
          Limite   ,
          i ;

      int vetElem[ DIM_VET + 1 ]  ;

      /* Ler parâmetros freqüências */

         numLidos = LER_LerParametros( "ii" , &numGera , &Limite ) ;
         if ( ( numLidos != 2 )
           || ( numGera  < 10 )
           || ( Limite   < 10 ))
         {
            return TST_CondRetParm ;
         } /* if */

         numElem  = 0 ;

         Anterior = 0 ;

         while ( LER_LerParmInt( &( vetElem[ numElem ] )))
         {
            if ( vetElem[ numElem ] < 0 )
            {
               break ;
            } /* if */

            if ( vetElem[ numElem ] <= Anterior )
            {
               TST_ExibirPrefixo( ERRO_LER , "Parâmetro freqüência fora de ordem" ) ;
               return TST_CondRetParm ;
            } /* if */

            Anterior = vetElem[ numElem ] ;
            numElem ++ ;

         } /* while */

      /* Gerar tabela de freqüências */

         ZerarVetor( ) ;

         for ( i = 0 ; i < numGera ; i++ )
         {
            Vet[ ALT_GerarFrequencia( numElem , vetElem , Limite ) ] ++ ;
         } /* for */

         ExibirDistribuicao( numGera , 0 , numElem , "Distribuição de freqüência." ) ;

      return TST_CondRetOK ;

   } /* Fim função: TALT -Exibir distribuição de freqüências */

/********** Fim do módulo de implementação: TALT Teste do gerador de números aleatórios **********/

