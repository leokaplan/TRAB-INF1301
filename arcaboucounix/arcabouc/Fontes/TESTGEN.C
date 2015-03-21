/***************************************************************************
*  $MCI Módulo de implementação: TGEN Teste específico para o módulo teste genérico
*
*  Arquivo gerado:              TESTGEN.c
*  Letras identificadoras:      TGEN
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

static const char ESPAC_CMD     [ ] = "=espaco"       ;
static const char BOOL_CMD      [ ] = "=compararbool" ;
static const char CHAR_CMD      [ ] = "=compararchar" ;
static const char INT_CMD       [ ] = "=compararint"  ;
static const char LONG_CMD      [ ] = "=compararlong" ;
static const char FLOAT_CMD     [ ] = "=compararfloat" ;
static const char STRING_CMD    [ ] = "=compararstring" ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TGEN &Testar módulo TesteGen
*
*  $ED Descrição da função
*     Efetua os diversos comandos de teste específicos para o módulo
*     TesteGen
*
*     Comandos interpretados:
*
*     =compararbool  <bool esperado> <bool obtido>
*        bool:  0 => FALSE    !=0 => TRUE
*
*     =compararchar  <char esperado> <char obtido>
*        compara sensível a caixa e diacrítico
*
*     =compararint   <int esperado>  <int obtido>
*
*     =compararlong  <long esperado  <long obtido>
*
*     =compararfloat <float esperado>  <float obtido>  <float tolerancia>
*         Vale se  1 - Tolerancia < Esperado / Obtido < 1 + Tolerancia
*
*     =compararstring <string esperado>  <string obtido>
*
*     =espaco        <espaco esperado>  <espaco obtido>
*         esperado deve estar entre aspas duplas
*         obtido inicia no primeiro não branco
*         valor  == .  sinaliza espaço tamanho 0
*
*  $EP Parâmetros
*     $P ComandoTeste - String contendo o comando
*     $P CondRetParm  - parâmetro de retorno utilizado para retornar
*                       o valor da condição de retorno da função testada
*                       Conterá -1 caso tenha sido observada uma falha
*                       e que não corresponda a uma condição de retorno
*                       definida em ARV_tpCondRet.
*
*  $FV Valor retornado
*     TST_CondRetOK
*     TST_CondRetNaoConhec
*     TST_CondRetParm
*     TST_CondRetErro      - a função testada resultou em erro.
*                            Neste caso é possível recuperar a falha,
*                            caso a condição de retorno fornecida via
*                            CondRetParm seja igual à condição esperada
*                            no comando =recuperar.
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      int  NumLidos      = -1 ;

      int IntEsperado,
          IntObtido ;

      char CharEsperado,
           CharObtido ;

      long LongEsperado,
           LongObtido ;

      double FloatEsperado ,
             FloatObtido   ,
             Tolerancia     ;

      char StringEsperado[ 250 ] ,
           StringObtido[   250 ] ;

      char EspacEsperado[ 100 ] ;
      char EspacObtido[   100 ] ;

      /* Testar Booleano */

         if ( strcmp( ComandoTeste , BOOL_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "ii" ,
                               &IntEsperado , &IntObtido ) ;
            if ( NumLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararBool( IntEsperado , IntObtido ,
                                     "Booleanos não são iguais." ) ;

         } /* fim ativa: Testar Booleano */

      /* Testar caractere */

         else if ( strcmp( ComandoTeste , CHAR_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "cc" ,
                               &CharEsperado , &CharObtido ) ;
            if ( NumLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararChar( CharEsperado , CharObtido ,
                                     "Caracteres não são iguais." ) ;

         } /* fim ativa: Testar caractere */

      /* Testar inteiro normal */

         else if ( strcmp( ComandoTeste , INT_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "ii" ,
                               &IntEsperado , &IntObtido ) ;
            if ( NumLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( IntEsperado , IntObtido ,
                                    "Inteiros não são iguais." ) ;

         } /* fim ativa: Testar inteiro normal */

      /* Testar inteiro longo */

         else if ( strcmp( ComandoTeste , LONG_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "ii" ,
                               &LongEsperado , &LongObtido ) ;
            if ( NumLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( LongEsperado , LongObtido ,
                                    "Inteiros longos não são iguais." ) ;

         } /* fim ativa: Testar inteiro longo */

      /* Testar flutuante */

         else if ( strcmp( ComandoTeste , FLOAT_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "fff" ,
                               &FloatEsperado , &FloatObtido , &Tolerancia ) ;
            if ( NumLidos != 3 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararFloat( FloatEsperado , FloatObtido , Tolerancia ,
                                      "Flutuantes fora da tolerância." ) ;

         } /* fim ativa: Testar flutuante */

      /* Testar string */

         else if ( strcmp( ComandoTeste , STRING_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "ss" ,
                               StringEsperado , StringObtido ) ;
            if ( NumLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararString( StringEsperado , StringObtido ,
                                       "Strings não são iguais." ) ;

         } /* fim ativa: Testar string */

      /* Testar acesso a espaços */

         else if ( strcmp( ComandoTeste , ESPAC_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "ss" ,
                               EspacEsperado , EspacObtido ) ;
            if ( NumLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

            if ( EspacEsperado[ 0 ] == '.' )   /* gera string vazio */
            {
               EspacEsperado[ 0 ] = 0 ;
            } /* if */

            if ( EspacObtido[ 0 ] == '.' )     /* gera string com algum lixo */
            {
               memcpy( EspacObtido , &NumLidos , 20 ) ; /* força espaco errado contendo resíduo da pilha */
            } /* if */

            return TST_CompararEspaco( EspacEsperado , EspacObtido ,
                                       strlen( EspacEsperado ) ,
                                       "Espaços não são iguais." ) ;

         } /* fim ativa: Testar acesso a espaços */

      return TST_CondRetNaoConhec ;

   } /* Fim função: TGEN &Testar módulo TesteGen */

/********** Fim do módulo de implementação: TGEN Teste específico para o módulo teste genérico **********/

