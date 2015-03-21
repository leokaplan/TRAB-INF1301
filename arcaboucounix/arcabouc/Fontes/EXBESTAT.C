/***************************************************************************
*  $MCI Módulo de implementação: EXES Exibir estatisticas de teste acumuladas
*
*  Arquivo gerado:              Exbestat.c
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
*     Exibe o conteúdo do arquivo acumulador de estatísticas de massas de teste.
*
*  $EIU Interface com o usuário pessoa
*     Deve ser fornecido um parâmetro /a<nome>
*     no qual <nome> é o nome do arquivo de acumulação de estatísticas
*     gerado com apoio do arcabouço.
*
***************************************************************************/

#include    <stdio.h>
#include    <string.h>


#define  DEFAULT_EXT_ACUM  ".estat"
#define  CHAR_EXT          '.'
#define  CHAR_SEP          '\\'

#define  PARM_NOME_ARQ_1   'e'
#define  PARM_NOME_ARQ_2   'E'

#define  PARM_NOME_HELP_1  '?'
#define  PARM_NOME_HELP_2  'h'
#define  PARM_NOME_HELP_3  'H'

#define  DIM_NOME_ARQ      250

/***** Protótipos das funções encapuladas no módulo *****/

   static char * TST_ObterInicioExtensao( char * NomeArqParm ) ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: EXES &Exibir conteúdo do arquivo estatisticas
*
***********************************************************************/

   int main( int NumParm , char ** vtValorParm )
   {

      char NomeArquivoEstatisticas[  DIM_NOME_ARQ ] = "" ;

      char ListaArquivoEstatisticas[ DIM_NOME_ARQ ] = "" ;

      int  inxParm ;
      int  Erro = 0 ;
      char ParmBuffer[ 250 ] ;

      int numMassasTeste    = 0 ;
      int numCasosTeste     = 0 ;
      int numCmdExecutados  = 0 ;
      int numLinhasLidas    = 0 ;
      int numFalhas         = 0 ;

      char   NomeArquivoCorr[ DIM_NOME_ARQ ] ;

      char * NomeArquivoProx = NULL ;

      FILE * pArqAcumula ;

      int numMassasTesteCorr    ,
          numCasosTesteCorr     ,
          numCmdExecutadosCorr  ,
          numLinhasLidasCorr    ,
          numFalhasCorr         ,
          numLidos               ;

      /* Exibir logo do programa exibir estatisticas */

         printf( "\nLES - Laboratorio de Engenharia de Software DI/PUC-Rio" ) ;
         printf( "\n      Teste automatizado de modulos C, exibir estatisticas (c) 2006\n" ) ;

      /* Obter parâmetros exibe estatisticas */

         for ( inxParm = 1 ; inxParm < NumParm ; inxParm ++ )
         {
            strcpy( ParmBuffer , vtValorParm[ inxParm ] ) ;

            if ( ( ParmBuffer[ 0 ] == '/' )
              || ( ParmBuffer[ 0 ] == '-' ))
            {
               switch ( ParmBuffer[ 1 ] )
               {
                  case PARM_NOME_ARQ_1 :
                  case PARM_NOME_ARQ_2 :
                  {
                     if ( NomeArquivoEstatisticas[ 0 ] == 0 )
                     {
                        strcpy( NomeArquivoEstatisticas , ParmBuffer + 2 ) ;
                     } else
                     {
                        printf( "\n   Arquivo de estisticas acumuladas ja foi definido." ) ;
                        Erro = 4 ;
                     } /* if */
                     break ;
                  } /* fim case */

                  case PARM_NOME_HELP_1 :
                  case PARM_NOME_HELP_2 :
                  case PARM_NOME_HELP_3 :
                  {
                     Erro = 2 ;
                     break ;
                  } /* fim case */

                  default :
                  {
                     printf( "\n   Parametro desconhecido: %s" , ParmBuffer ) ;
                     Erro = 4 ;
                     break ;
                  } /* fim case */

               } /* fim switch */
            } else
            {
               printf( "\n   Sintaxe de parametro errada: %s" , ParmBuffer ) ;
               Erro = 4 ;
            } /* fim if */
         } /* fim repete */

      /* Controlar parâmetros exibir estatisticas */

         if ( NomeArquivoEstatisticas[ 0 ] == 0 )
         {

            if ( Erro == 0 )
            {
               printf( "\nFaltou o nome do arquivo de estatisticas." ) ;
               Erro = 4 ;
            } /* if */

         } /* fim ativa: Controlar parâmetros exibir estatisticas */

      /* Exibir sintaxe de uso do programa exibir estatisticas */

         if ( Erro != 0 )
         {

            printf( "\n\nModo de uso do programa" ) ;
            printf(   "\n  ExbEstat /%c<ArqAcum> {/? | /h }" , PARM_NOME_ARQ_1 ) ;
            printf( "\n\n  <ArqAcum> - nome de um ou mais arquivos de estatisticas" ) ;
            printf(   "\n              acumuladas, separados por \';\'." ) ;
            printf(   "\n              Parametro obrigatorio." ) ;
            printf(   "\n              Extensao default: %s" ,  DEFAULT_EXT_ACUM ) ;
            printf(   "\n  /? ou /h  - Solicitacao de auxilio.\n" ) ;

            return Erro ;

         } /* fim ativa: Exibir sintaxe de uso do programa exibir estatisticas */

      /* Exibir conteúdo do estatisticas */

         /* Obter total de vários acumuladores */

            NomeArquivoProx = NomeArquivoEstatisticas ;

            while ( NomeArquivoProx != NULL ) {

            /* Extrair nome do arquivo acumulador corrente */

               NomeArquivoProx = strchr( NomeArquivoEstatisticas , ';' ) ;
               if ( NomeArquivoProx != 0 )
               {
                  * NomeArquivoProx = 0 ;
                  strcpy( NomeArquivoCorr , NomeArquivoEstatisticas ) ;
                  NomeArquivoProx++ ;
                  strcpy( NomeArquivoEstatisticas , NomeArquivoProx ) ;

               } else
               {
                  strcpy( NomeArquivoCorr , NomeArquivoEstatisticas ) ;
               } /* if */

            /* Efetuar a acumulação de acumuladores */

               if ( TST_ObterInicioExtensao( NomeArquivoCorr ) == NULL )
               {
                  strcat( NomeArquivoCorr , DEFAULT_EXT_ACUM ) ;
               } /* if */
                  
               pArqAcumula = fopen( NomeArquivoCorr , "r" ) ;
               if ( pArqAcumula != 0 )
               {
                  numLidos = fscanf( pArqAcumula , " %d %d %d %d %d" ,
                               &numMassasTesteCorr , &numCmdExecutadosCorr ,
                               &numCasosTesteCorr  , &numLinhasLidasCorr   ,
                               &numFalhasCorr ) ;
                  if ( numLidos != 5 )
                  {
                     printf( "\n\n>>> Arquivo de estatisticas: %s esta' sintaticamente errado\n" ,
                             NomeArquivoCorr ) ;
                     return 4 ;
                  } /* if */

                  fclose( pArqAcumula ) ;

                  strcat( ListaArquivoEstatisticas , "  " ) ;
                  strcat( ListaArquivoEstatisticas , NomeArquivoCorr ) ;

                  numMassasTeste    += numMassasTesteCorr ;
                  numCmdExecutados  += numCmdExecutadosCorr ;
                  numCasosTeste     += numCasosTesteCorr ;
                  numLinhasLidas    += numLinhasLidasCorr ;
                  numFalhas         += numFalhasCorr ;

               } else
               {
                  printf( "\n\n>>> Arquivo de estatisticas: %s nao existe\n" ,
                          NomeArquivoCorr ) ;
                  return 4 ;
               } /* if */

            } /* fim repete: Obter total de vários acumuladores */

         /* Exibir total acumulado de todos acumuladores */

            printf( "\n!!! --- Estatisticas de teste acumuladas: %s ---" ,
                    ListaArquivoEstatisticas ) ;
            printf( "\n!!! Numero de massas de teste:    %7d" , numMassasTeste ) ;
            printf( "\n!!! Numero de linhas lidas:       %7d" , numLinhasLidas ) ;
            printf( "\n!!! Numero de casos de teste:     %7d" , numCasosTeste ) ;
            printf( "\n!!! Numero de comandos de teste:  %7d" , numCmdExecutados ) ;
            printf( "\n!!! Numero de falhas encontradas: %7d" , numFalhas ) ;
            printf( "\n!!! ---------------------------------------------------------------" ) ;

         /* Exibir laudo final de acumuladores */

            if ( numFalhas == 0 )
            {
               printf( "\n\n!!! Nao foram encontrados problemas\n\n" ) ;

            } else
            {
               printf( "\n\n>>> Foram encontrados problemas!\n\n" ) ;
               return 4 ;
            } /* if */

      return 0 ;

   } /* Fim função: EXES &Exibir conteúdo do arquivo estatisticas */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: TSTG &Obter origem do nome da extensão de arquivo
*
*  $ED Descrição da função
*     Procura o início do nome da extensão contido no nome do arquivo
*     dado por parâmetro
*
*     Evite o uso de nomes de arquivos com dois ou mais caracteres ".".
*     Isto pode confundir no caso de se desejar a adição de algum nome
*     de extensão padrão.
*
*  $EP Parâmetros
*     $P NomeArqParm - nome de arquivo a ser analisado
*
*  $FV Valor retornado
*     Ponteiro para o caractere '.' que separa a extensão.
*     Será NULL caso o nome do arquivo não contenha extensão.
*
***********************************************************************/

   char * TST_ObterInicioExtensao( char * NomeArqParm )
   {

      int i ;

      for( i = ( int ) strlen( NomeArqParm ) - 1 ; i >= 0 ; i-- )
      {
         if ( NomeArqParm[ i ] == CHAR_EXT )
         {
            return NomeArqParm + i ;
         } /* if */
         if ( NomeArqParm[ i ] == CHAR_SEP )
         {
            return NULL ;
         } /* if */
      } /* for */

      return NULL ;

   } /* Fim função: TSTG &Obter origem do nome da extensão de arquivo */

/********** Fim do módulo de implementação: EXES Exibir estatisticas de teste acumuladas **********/

