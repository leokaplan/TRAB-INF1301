/***************************************************************************
*  $MCI Módulo de implementação: TLER Teste de ler parâmetros
*
*  Arquivo gerado:              TESTLER.c
*  Letras identificadoras:      TLER
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
*     Teste do módulo ler parâmetros.
*     Este módulo será desenvolvido em duas etapas
*     - a primeira um teste direto
*     - a segunda um teste utilizando o próprio módulo lerparm para
*       ler os parâmetros dos comandos.
*
***************************************************************************/

#include     <stdio.h>
#include     <string.h>
#include     <stdarg.h>
#include     <ctype.h>

#include "TST_ESPC.H"

#include "GENERICO.H"
#include "LERPARM.H"

static const char LER_PARM_CIS_CMD[ ]      = "=lerparm-cis:" ;
static const char LER_PARM_SS_CMD[ ]       = "=lerparm-ss:" ;
static const char LER_PARM_S_CMD[ ]        = "=lerparm-s:" ;
static const char LER_PARM_II_CMD[ ]       = "=lerparm-ii:" ;
static const char LER_PARM_FI_CMD[ ]       = "=lerparm-fi:" ;
static const char LER_PARM_N_CMD[ ]        = "=lerparm-n:" ;
static const char COMPARAR_CHAR_CMD[ ]     = "=compararparmchar" ;
static const char COMPARAR_INT_CMD[ ]      = "=compararparmint" ;
static const char COMPARAR_DOUBLE_CMD[ ]   = "=compararparmdouble" ;
static const char COMPARAR_STRING_CMD[ ]   = "=compararparmstring" ;

#define  TRUE      1
#define  FALSE     0

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TLER &Teste específico do módulo LerParm
*
*  $ED Descrição da função
*     =lerparm-cis:          int     char    string
*     =lerparm-ss:           string  string
*     =lerparm-s:            string
*     =lerparm-ii:           int     int
*     =lerparm-fi:           float   int
*     =lerparm-n:            nome
*     =compararparmchar      char    char
*     =compararparmint       int     int
*     =compararparmdouble    float   float
*     =compararparmstring    string  string
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      char ValChar1 = 'h' ;
      int  ValInt1  = -1 ;
      char ValStr1[ LER_DIM_STRING ] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa" ;

      char ValChar2 = 'i' ;
      int  ValInt2  = -2 ;
      char ValStr2[ LER_DIM_STRING ] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbb" ;

      char ParmChar1 = 'j' ;
      int  ParmInt1  = -3 ;
      char ParmStr1[ LER_DIM_STRING ] = "ccccccccccccccccccccccccccccc" ;

      int  ParmInt2  = -4 ;
      char ParmStr2[ LER_DIM_STRING ] = "ddddddddddddddddddddddddddddd" ;

      TST_tpCondRet CondRetAcum = TST_CondRetOK ;
      TST_tpCondRet CondRet     = TST_CondRetErro ;

      int numLidos = -1 ;

      double ParmDouble ;
      double ValDouble ;

      /* Ler linha tipo cis */

         if ( strcmp( ComandoTeste , LER_PARM_CIS_CMD ) == 0 )
         {

            numLidos = sscanf( LER_Buffer , "%*s \'%c\' %i \"%[^\"]\"" ,
                               &ParmChar1 , &ParmInt1 , &ParmStr1 ) ;
            if ( numLidos != 3 )
            {
               return TST_CondRetParm ;
            } /* if */

            numLidos = LER_LerParametros( "cis" , &ValChar1 , &ValInt1 , ValStr1 ) ;

            CondRet = TST_CompararInt( 3 , numLidos , "Não leu todos os parâmetros." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            CondRet = TST_CompararChar( ParmChar1 , ValChar1 , "Caractere errado." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            CondRet = TST_CompararInt( ParmInt1 , ValInt1 , "Inteiro errado." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            CondRet = TST_CompararString( ParmStr1 , ValStr1 , "String errado." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            return CondRetAcum ;

         } /* fim ativa: Ler linha tipo cis */

      /* Ler linha tipo ss */

         else if ( strcmp( ComandoTeste , LER_PARM_SS_CMD ) == 0 )
         {

            numLidos = sscanf( LER_Buffer , "%*s \"%[^\"]\" \"%[^\"]\"" ,
                               ParmStr1 , ParmStr2 ) ;
            if ( numLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

            numLidos = LER_LerParametros( "ss" , ValStr1 , ValStr2 ) ;

            CondRet = TST_CompararInt( 2 , numLidos , "Não leu todos os parâmetros." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            CondRet = TST_CompararString( ParmStr1 , ValStr1 , "Primeiro string errado." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */


            CondRet = TST_CompararString( ParmStr2 , ValStr2 , "Segundo string errado." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            return CondRetAcum ;

         } /* fim ativa: Ler linha tipo ss */

      /* Ler linha tipo s */

         else if ( strcmp( ComandoTeste , LER_PARM_S_CMD ) == 0 )
         {

            numLidos = sscanf( LER_Buffer , "%*s \"%[^\"]\"" ,
                               ParmStr1 ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            numLidos = LER_LerParametros( "s" , ValStr1 ) ;

            CondRet = TST_CompararInt( 1 , numLidos , "Não leu todos os parâmetros." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            CondRet = TST_CompararString( ParmStr1 , ValStr1 , "String errado." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            return CondRetAcum ;

         } /* fim ativa: Ler linha tipo s */

      /* Ler linha tipo ii */

         else if ( strcmp( ComandoTeste , LER_PARM_II_CMD ) == 0 )
         {

            numLidos = sscanf( LER_Buffer , "%*s %d %d" ,
                               &ParmInt1 , &ParmInt2 ) ;
            if ( numLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

            numLidos = LER_LerParametros( "ii" , &ValInt1 , &ValInt2 ) ;

            CondRet = TST_CompararInt( 2 , numLidos , "Não leu todos os parâmetros." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            CondRet = TST_CompararInt( ParmInt1 , ValInt1 , "Primeiro inteiro errado." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */


            CondRet = TST_CompararInt( ParmInt2 , ValInt2 , "Segundo inteiro errado." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            return CondRetAcum ;

         } /* fim ativa: Ler linha tipo ii */

      /* Ler linha tipo fi */

         else if ( strcmp( ComandoTeste , LER_PARM_FI_CMD ) == 0 )
         {

            numLidos = sscanf( LER_Buffer , "%*s %lf %d" ,
                               &ParmDouble , &ParmInt1 ) ;
            if ( numLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

            numLidos = LER_LerParametros( "fi" , &ValDouble , &ValInt1 ) ;

            CondRet = TST_CompararInt( 2 , numLidos , "Não leu todos os parâmetros." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            CondRet = TST_CompararFloat( ParmDouble , ValDouble , 0. , "Flutuante errado." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */


            CondRet = TST_CompararInt( ParmInt1 , ValInt1 , "Inteiro errado." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            return CondRetAcum ;

         } /* fim ativa: Ler linha tipo fi */

      /* Ler linha tipo n */

         else if ( strcmp( ComandoTeste , LER_PARM_N_CMD ) == 0 )
         {

            numLidos = sscanf( LER_Buffer , "%*s %s" ,
                               ParmStr1 ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            numLidos = LER_LerParametros( "n" , ValStr1 ) ;

            CondRet = TST_CompararInt( 1 , numLidos , "Não leu todos os parâmetros." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            CondRet = TST_CompararString( ParmStr1 , ValStr1 , "Nome errado." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            return CondRetAcum ;

         } /* fim ativa: Ler linha tipo n */

      /* Comparar parâmetros char */

         else if ( strcmp( ComandoTeste , COMPARAR_CHAR_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "cc" , &ValChar1 , &ValChar2 ) ;

            CondRet = TST_CompararInt( 2 , numLidos , "Não leu todos os parâmetros." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            return TST_CompararChar( ValChar1 , ValChar2 , "Caracteres diferentes." ) ;

         } /* fim ativa: Comparar parâmetros char */

      /* Comparar parâmetros inteiro */

         else if ( strcmp( ComandoTeste , COMPARAR_INT_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" , &ValInt1 , &ValInt2 ) ;

            CondRet = TST_CompararInt( 2 , numLidos , "Não leu todos os parâmetros." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            return TST_CompararInt( ValInt1 , ValInt2 , "Int diferentes." ) ;

         } /* fim ativa: Comparar parâmetros inteiro */

      /* Comparar parâmetros double */

         else if ( strcmp( ComandoTeste , COMPARAR_DOUBLE_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "ff" , &ValDouble , &ParmDouble ) ;

            CondRet = TST_CompararInt( 2 , numLidos , "Não leu todos os parâmetros." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            return TST_CompararFloat( ValDouble , ParmDouble , 0. , "Double diferentes." ) ;

         } /* fim ativa: Comparar parâmetros double */

      /* Comparar parâmetros string */

         else if ( strcmp( ComandoTeste , COMPARAR_STRING_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "ss" , ValStr1 , ValStr2 ) ;

            CondRet = TST_CompararInt( 2 , numLidos , "Não leu todos os parâmetros." ) ;

            if ( CondRet != TST_CondRetOK )
            {
               CondRetAcum = TST_CondRetErro ;
            } /* if */

            return TST_CompararString( ValStr1 , ValStr2 , "String diferentes." ) ;

         } /* fim ativa: Comparar parâmetros string */

      return TST_CondRetNaoConhec ;

   } /* Fim função: TLER &Teste específico do módulo LerParm */

/********** Fim do módulo de implementação: TLER Teste de ler parâmetros **********/

