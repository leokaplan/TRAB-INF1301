/***************************************************************************
*  $MCI Módulo de implementação: TSTG Controlador de teste genérico
*
*  Arquivo gerado:              GENERICO.c
*  Letras identificadoras:      TST
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
#include    <stdlib.h>
// #include    <process.h>

#define GENERICO_OWN
#include "GENERICO.H"
#undef GENERICO_OWN

#include "LERPARM.H"
#include "INTRPCNT.H"
#include "INTRPCED.H"
#include "CESPDIN.H"

/* Constantes internas */

#define     DIM_NOME_ARQ        250
#define     DIM_COMANDO_TESTE   100
#define     DIM_LINHA            16
#define     DIM_MSG             800

#define     DEFAULT_EXT_LOG     ".log"
#define     DEFAULT_EXT_SCRIPT  ".script"
#define     DEFAULT_EXT_ACUM    ".estat"

#define     CHAR_EXT            '.'
#define     CHAR_SEP            '\\'


#define     TRIM_CHAR           " \n\r\t\x026"

/* Comandos genéricos do script de teste */

static const char COMMENT_CMD[ ]           = "//" ;
static const char INICIO_CMD[ ]            = "==" ;
static const char RECUPERA_CMD[ ]          = "=recuperar" ;
static const char BKPT_CMD[ ]              = "=breakpoint" ;
static const char CANCELA_CMD[ ]           = "=cancelar" ;
static const char ACEITA_ERRO_CMD[ ]       = "=AceitaFalhasAcumuladas" ;

static const char InicializarEspacosCmd[ ] = "=inicializarespacos" ;

/***********************************************************************
*
*  $TC Tipo de dados: TST Cond ret módulo de teste genérico
*
*
***********************************************************************/

   typedef enum {

         CondOK ,
               /* Generico executou correto */

         CondNaoAbriu
               /* Genérico não abriu arquivo */

   } tpCond ;

/*****  Dados encapsulados no módulo  *****/

      static char NomeArqLog[ DIM_NOME_ARQ ] = "" ;
            /* Nome do arquivo log */

      static FILE * pArqLog = NULL ;
            /* Ponteiro para o descritor do arquivo log */

      static char NomeArquivoAcumulador[ DIM_NOME_ARQ ] = "" ;
            /* Nome do arquivo acumulador */

      static int ContaCasosTeste = 0 ;
            /* Número de casos de teste efetuados */

      static int ContaFalhas = 0 ;
            /* Número de falhas encontradas */

      static int ContaComandosExecutados = 0 ;
            /* Número de comandos específicos interpretados corretamente */

      #define  DIM_LINHA     16
      #define  TITULO_CHAR   "0123456789ABCDEF"
            /* Cabeçalho de exibição de espaço útil */

/***** Protótipos das funções encapuladas no módulo *****/

   static void InicializarTeste( void ) ;

   static void TerminarTeste( void ) ;

   static tpCond AbrirArquivoLog( char * NomeArqParm ) ;

   static TST_tpCondRet RealizarTeste( char * Construto ) ;

   static TST_tpCondRet InterpretarComandos( char * ComandoTeste ) ;

   static void ImprimirHexadecimal( void * pValorParm ,
                                    int    Offset     ,
                                    int    TamValor    ) ;

   static void ExibirResultados( FILE * pArq ) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: TSTG &Controlar uma sessão de teste
*  ****/

   int TST_ControlarSessaoTeste( char * NomeConstrutoParm ,
                                 char * NomeArqScriptParm ,
                                 char * NomeArqLogParm    ,
                                 char * NomeArqAcumParm    )
   {

      TST_tpCondRet CondRet ;

      /* Inicializar a sessão de teste */

         InicializarTeste( ) ;

      /* Abrir o arquivo script */

         if ( LER_AbrirArquivoScript( NomeArqScriptParm ) != LER_CondRetOK )
         {
            printf( "\n>>>   Nao foi possivel abrir arquivo script: %s"  ,
                      NomeArqScriptParm ) ;
            return 4 ;
         } /* if */

      /* Abrir o arquivo log */

         if ( NomeArqLogParm[ 0 ] != 0 )
         {
            if ( AbrirArquivoLog( NomeArqLogParm ) != CondOK )
            {
               printf( "\n>>>   Nao foi possivel abrir arquivo log: %s" ,
                         NomeArqLogParm ) ;
               printf( "\n>>>   A saida sera gerada na console." ) ;

            } /* if */
         } /* if */

      /* Registrar o arquivo acumulador */

         strcpy( NomeArquivoAcumulador , NomeArqAcumParm ) ;

      /* Efetuar o teste */

         CondRet = RealizarTeste( NomeConstrutoParm ) ;

      /* Concluir a execução do teste */

         TerminarTeste( ) ;

         if ( CondRet == TST_CondRetOK )
         {
            return 0 ;
         } /* if */

         return 4 ;

   } /* Fim função: TSTG &Controlar uma sessão de teste */

/***************************************************************************
*
*  Função: TSTG &Comparar booleano
*  ****/

   TST_tpCondRet TST_CompararBool( int    ValorEsperado ,
                                   int    ValorObtido   ,
                                   char * pMensagem      )
   {

      if ( ( ValorObtido   != 0 )
        && ( ValorEsperado == 0 ))
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Deveria ser: FALSE É: TRUE" ,
                  ValorEsperado , ValorObtido ) ;
         return TST_CondRetErro ;
      } /* if */

      if ( ( ValorObtido   == 0 )
        && ( ValorEsperado != 0 ))
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Deveria ser: TRUE É: FALSE" ,
                  ValorEsperado , ValorObtido ) ;
         return TST_CondRetErro ;
      } /* if */

      return TST_CondRetOK ;

   } /* Fim função: TSTG &Comparar booleano */

/***************************************************************************
*
*  Função: TSTG &Comparar caractere
*  ****/

   TST_tpCondRet TST_CompararChar( char   ValorEsperado ,
                                   char   ValorObtido   ,
                                   char * pMensagem       )
   {

      if ( ValorObtido != ValorEsperado )
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Deveria ser: '%c'  É: '%c'" ,
                  ValorEsperado , ValorObtido ) ;
         return TST_CondRetErro ;
      } /* if */
      return TST_CondRetOK ;

   } /* Fim função: TSTG &Comparar caractere */

/***************************************************************************
*
*  Função: TSTG &Comparar inteiro
*  ****/

   TST_tpCondRet TST_CompararInt( long   ValorEsperado ,
                                  long   ValorObtido   ,
                                  char * pMensagem       )
   {

      if ( ValorObtido != ValorEsperado )
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Deveria ser: %ld  É: %ld" ,
                  ValorEsperado , ValorObtido ) ;
         return TST_CondRetErro ;
      } /* if */
      return TST_CondRetOK ;

   } /* Fim função: TSTG &Comparar inteiro */

/***************************************************************************
*
*  Função: TSTG &Comparar flutuante
*  ****/

   TST_tpCondRet TST_CompararFloat( double   ValorEsperado ,
                                    double   ValorObtido   ,
                                    double   Tolerancia    ,
                                    char *   pMensagem      )
   {

      double Razao ;

      Razao = ValorEsperado / ValorObtido ;
         
      if ( ( Razao < 1.0 - Tolerancia )
        || ( Razao > 1.0 + Tolerancia ))
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Tolerância %lg, esperado: %lg , obtido: %lg" ,
                  Tolerancia , ValorEsperado , ValorObtido ) ;
         return TST_CondRetErro ;
      } /* if */
      return TST_CondRetOK ;
         

   } /* Fim função: TSTG &Comparar flutuante */

/***************************************************************************
*
*  Função: TSTG &Comparar string
*  ****/

   TST_tpCondRet TST_CompararString( char * ValorEsperado ,
                                     char * ValorObtido   ,
                                     char * pMensagem       )
   {

      if ( strcmp( ValorObtido , ValorEsperado ) != 0 )
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Deveria ser: >%s<  É: >%s<" ,
                  ValorEsperado , ValorObtido ) ;
         return TST_CondRetErro ;
      } /* if */
      return TST_CondRetOK ;

   } /* Fim função: TSTG &Comparar string */

/***************************************************************************
*
*  Função: TSTG &Comparar espaço
*  ****/

   TST_tpCondRet TST_CompararEspaco( void * ValorEsperado ,
                                     void * ValorObtido   ,
                                     int    TamEspaco     ,
                                     char * pMensagem       )
   {

      int i ;

      /* Tratar espaços não iguais */

         if ( memcmp( ValorObtido , ValorEsperado , TamEspaco ) != 0 )
         {

            ContaFalhas ++ ;
            TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
            fprintf( pArqLog , "\n" ) ;

            for( i = 0 ; i < TamEspaco ; i += DIM_LINHA )
            {
               fprintf( pArqLog , "\n   %05d Nao é: " , i ) ;
               ImprimirHexadecimal( ValorEsperado , i , TamEspaco ) ;

               fprintf( pArqLog , "\n         É    : " ) ;
               ImprimirHexadecimal( ValorObtido , i , TamEspaco ) ;

               fprintf( pArqLog , "\n" ) ;
            } /* for */

            return TST_CondRetErro ;

         } /* fim ativa: Tratar espaços não iguais */

      return TST_CondRetOK ;

   } /* Fim função: TSTG &Comparar espaço */

/***************************************************************************
*
*  Função: TSTG &Comparar ponteiro
*  ****/

   TST_tpCondRet TST_CompararPonteiro( void * PonteiroEsperado ,
                                       void * PonteiroObtido   ,
                                       char * pMensagem       )
   {

      if ( PonteiroObtido != PonteiroEsperado )
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Deveria ser: %p  É: %p" ,
                  PonteiroEsperado , PonteiroObtido ) ;
         return TST_CondRetErro ;
      } /* if */
      return TST_CondRetOK ;

   } /* Fim função: TSTG &Comparar ponteiro */

/***************************************************************************
*
*  Função: TSTG &Comparar ponteiro nulo
*  ****/

   TST_tpCondRet TST_CompararPonteiroNulo( int    ModoEsperado ,
                                           void * PonteiroObtido   ,
                                           char * pMensagem       )
   {

      if ( (( ModoEsperado == 0 ) && ( PonteiroObtido != NULL ))
        || (( ModoEsperado != 0 ) && ( PonteiroObtido == NULL )) )
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         if ( ModoEsperado )
         {
            fprintf( pArqLog , "Deveria ser: não NULL  É: NULL" ) ;
         } else
         {
            fprintf( pArqLog , "Deveria ser: NULL  É: %p" ,
                     PonteiroObtido ) ;
         } /* if */
         return TST_CondRetErro ;
      } /* if */
      return TST_CondRetOK ;

   } /* Fim função: TSTG &Comparar ponteiro nulo */

/***************************************************************************
*
*  Função: TSTG &Assertiva controlada
*  ****/

   void TST_Assert( int Expressao , int Linha , char * NomeArq )
   {

      char Msg[ DIM_MSG ] ;

      if ( Expressao )
      {
         return ;
      } /* if */

      sprintf( Msg , "Assertiva falhou:  linha %d  arquivo: %s" ,
                          Linha , NomeArq ) ;

      TST_ExibirPrefixo( "\n   " , "************************************\n" ) ;
      TST_NotificarFalha( Msg ) ;

      TST_ExibirPrefixo( "\n   " , "Assertiva  Assertiva  Assertiva\n" ) ;

      if ( pArqLog != stdout )
      {
         fclose( pArqLog ) ;
         pArqLog = stdout ;

         TST_ExibirPrefixo( "\n   " , "************************************\n" ) ;
         TST_NotificarFalha( Msg ) ;

         TST_ExibirPrefixo( "\n   " , "Assertiva  Assertiva  Assertiva\n" ) ;

      } /* if */

      exit( 4 ) ;

   } /* Fim função: TSTG &Assertiva controlada */

/***************************************************************************
*
*  Função: TSTG &Exibir prefixo da mensagem
*  ****/

   void TST_ExibirPrefixo( char * Indicador , char * pMensagem )
   {

      fprintf( pArqLog , "\n%s %4d  Linha: %5d  %s " ,
               Indicador , ContaFalhas , LER_ObterNumLinhas( ) , pMensagem ) ;

   } /* Fim função: TSTG &Exibir prefixo da mensagem */

/***************************************************************************
*
*  Função: TSTG &Notificar falha
*  ****/

   TST_tpCondRet TST_NotificarFalha( char * pMensagem )
   {

      ContaFalhas ++ ;
      TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
      return TST_CondRetErro ;

   } /* Fim função: TSTG &Notificar falha */

/***************************************************************************
*
*  Função: TSTG &Incrementar contador de falhas
*  ****/

   void TST_ContarFalhas( )
   {

      ContaFalhas ++ ;

   } /* Fim função: TSTG &Incrementar contador de falhas */

/***************************************************************************
*
*  Função: TSTG &Obter número de casos de teste efetuados
*  ****/

   int TST_ObterNumCasosTeste( )
   {

      return ContaCasosTeste ;

   } /* Fim função: TSTG &Obter número de casos de teste efetuados */

/***************************************************************************
*
*  Função: TSTG &Obter número de comandos específicos corretamente interpretados
*  ****/

   int TST_ObterNumComandosExecutados( void )
   {

      return ContaComandosExecutados ;

   } /* Fim função: TSTG &Obter número de comandos específicos corretamente interpretados */

/***************************************************************************
*
*  Função: TSTG &Obter número de falhas encontradas
*  ****/

   int TST_ObterNumFalhas( void )
   {

      return ContaFalhas ;

   } /* Fim função: TSTG &Obter número de falhas encontradas */


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

/***************************************************************************
*
*  Função: TSTG &Obter ponteiro para arquivo LOG
*  ****/

   FILE * TST_ObterArqLog( void )
   {

      if ( pArqLog == NULL )
      {
         return stdout ;
      } else {
         return pArqLog ;
      } /* if */

   } /* Fim função: TSTG &Obter ponteiro para arquivo LOG */

/***************************************************************************
*
*  Função: TSTG &Exibir um espaço qualquer em formato hexadecimal e char
*  ****/

   void TST_ExibirEspacoHexa( int    tamEspaco ,
                              void * Ponteiro   )
   {

      char * pEspaco ;

      int inxLinha ,
          i ;

      pEspaco = ( char * ) Ponteiro ;

      /* Exibir cabeçalho */

         fprintf( pArqLog , "\n        " ) ;

         for( i = 0  ; i < DIM_LINHA ; i++ )
         {
            fprintf( pArqLog , "%3X" , i ) ;
         } /* for */

         fprintf( pArqLog , "   " TITULO_CHAR ) ;

      /* Exibir todas as linhas */

         for ( inxLinha = 0 ; inxLinha < tamEspaco ; inxLinha += DIM_LINHA )
         {
            fprintf( pArqLog , "\n    %03X " , inxLinha ) ;
            ImprimirHexadecimal( pEspaco , inxLinha , tamEspaco ) ;
         } /* for */

         fprintf( pArqLog , "\n" ) ;

   } /* Fim função: TSTG &Exibir um espaço qualquer em formato hexadecimal e char */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: TSTG -Inicializar o módulo de teste
*
*  $ED Descrição da função
*     Inicializa todas as variáveis globais exportadas e encapsuladas
*     do módulo de teste genérico.
*
*     Embora pouco recomendável (ver descrição do módulo TesteSpc)
*     esta função pode ser alterada para assegurar o estabelecimento
*     do contexto inicial do módulo TesteSpc. Este deve conter uma
*     função de inicialização a ser chamada pela presente função.
*
***********************************************************************/

   void InicializarTeste( void )
   {

      NomeArqLog[ 0 ]    = 0 ;
      pArqLog            = stdout ;

      ContaComandosExecutados = 0 ;
      ContaFalhas        = 0 ;
      ContaCasosTeste    = 0 ;

      TST_ASSERT( LER_InicializarLeitura( ) ) ;

   } /* Fim função: TSTG -Inicializar o módulo de teste */


/***********************************************************************
*
*  $FC Função: TSTG -Terminar a execução do módulo de teste
*
*  $ED Descrição da função
*     Fecha todos os arquivos e desaloca todos os espaços de dados
*     alocados sob controle do módulo de teste genérico
*
***********************************************************************/

   void TerminarTeste( void )
   {

      FILE * pArqAcumula   = NULL ;
      int numMassasTeste   = 0 ;
      int numCasosTeste    = 0 ;
      int numCmdExecutados = 0 ;
      int numLinhasLidas   = 0 ;
      int numFalhas        = 0 ;

      /* Fechar o arquivo de script */

         LER_TerminarLeitura( ) ;

      /* Acumular os resultados dos testes */

         if ( NomeArquivoAcumulador[ 0 ] != 0 )
         {

            if ( TST_ObterInicioExtensao( NomeArquivoAcumulador ) == NULL )
            {
               strcat( NomeArquivoAcumulador , DEFAULT_EXT_ACUM ) ;
            } /* if */

            pArqAcumula = fopen( NomeArquivoAcumulador , "r" ) ;
            if ( pArqAcumula != 0 )
            {
               fscanf( pArqAcumula , " %d %d %d %d %d" ,
                            &numMassasTeste , &numCmdExecutados ,
                            &numCasosTeste  , &numLinhasLidas   ,
                            &numFalhas ) ;
               fclose( pArqAcumula ) ;
            } /* if */

            numMassasTeste   ++ ;
            numCasosTeste    += ContaCasosTeste ;
            numCmdExecutados += ContaComandosExecutados ;
            numLinhasLidas   += LER_ObterNumLinhas( ) ;
            numFalhas        += ContaFalhas ;

            pArqAcumula = fopen( NomeArquivoAcumulador , "w" ) ;
            if ( pArqAcumula != 0 )
            {
               fprintf( pArqAcumula , "  %d  %d  %d  %d  %d" ,
                            numMassasTeste  , numCmdExecutados ,
                            numCasosTeste   , numLinhasLidas   ,
                            numFalhas ) ;
               fclose( pArqAcumula ) ;
            } /* if */

         } /* fim ativa: Acumular os resultados dos testes */

      /* Exibir os resultados do teste */

         ExibirResultados( pArqLog ) ;

         if ( pArqLog != stdout )
         {
            ExibirResultados( stdout ) ;
         } /* if */

         fprintf( pArqLog , "\n\n!!! Numero de espacos ainda alocados: %d",
                  CED_ObterNumeroEspacosAlocados( ) ) ;

      /* Fechar o arquivo de log */

         if ( pArqLog != stdout )
         {

            fprintf( pArqLog , "\n" ) ;
            fclose(  pArqLog ) ;
            pArqLog = stdout ;
            NomeArqLog[ 0 ] = 0 ;

         } /* fim ativa: Fechar o arquivo de log */

   } /* Fim função: TSTG -Terminar a execução do módulo de teste */


/***********************************************************************
*
*  $FC Função: TSTG -Abrir arquivo log
*
*  $ED Descrição da função
*     Abre o arquivo log a ser utilizado como saída durante os testes.
*     Todas as saídas geradas pelo módulo de teste são dirigidos para
*     este arquivo. Caso não tenha sido aberto, será utilizado o
*     arquivo stdout.
*
*     A extensão default do arquivo log é ".log" e será acrescida ao nome
*     do arquivo caso este não contenha nome de extensão.
*
*  $EP Parâmetros
*     $P NomeArqParm - nome do arquivo. Pode (deve) ser um nome relativo
*                      ao diretório corrente. Caso o arquivo
*                      exista, será destruido sem perguntar se pode.
*
*  $FV Valor retornado
*     TST_CondOK       - abriu corretamente
*     TST_CondNaoAbriu - ocorreu algum problema ao tentar abrir.
*                        O problema não é descriminado.
*                        O arquivo de saída do log volta a ser stdout
*
*  $EIU Interface com usuário pessoa
*     Esta função deve ser chamada antes de iniciar a execução dos testes.
*
***********************************************************************/

   tpCond AbrirArquivoLog( char * NomeArqParm )
   {

      strcpy( NomeArqLog , NomeArqParm ) ;
      if ( TST_ObterInicioExtensao( NomeArqLog ) == NULL )
      {
         strcat( NomeArqLog , DEFAULT_EXT_LOG ) ;
      } /* if */

      pArqLog = fopen( NomeArqLog , "w" ) ;
      if ( pArqLog != NULL )
      {
         return CondOK ;
      } /* if */

      pArqLog = stdout ;
      NomeArqLog[ 0 ] = 0 ;

      return CondNaoAbriu ;

   } /* Fim função: TSTG -Abrir arquivo log */


/***********************************************************************
*
*  $FC Função: TSTG -Interpretar toda a massa de teste
*
*  $ED Descrição da função
*     Interpreta o arquivo de diretivas (script) de teste já aberto
*
*     A função de teste é dividida em três classes de comandos:
*
*     - os de teste genérico, que coordenam e controlam a realização do
*       teste. Estes comandos são interpretados por esta função.
*     - os de teste e controle de módulos componentes do arcabouço.
*       Estes comandos são interpretados por funções de teste específicas
*       para cada um dos módulos componentes.
*     - os de teste específico do módulo (ou módulos) a ser testado.
*       O módulo de teste específico interpreta os comandos projetados
*       para realizar os testes do módulo a testar.
*
*     A função de teste genérico lê um arquivo de diretivas de teste.
*     Cada linha deste arquivo é um comando de teste.
*     A função de teste lê uma a uma as linhas do arquivo, limpa os
*     caracteres em branco no final da linha e procura interpretar o
*     comando.
*
*     As funções de teste específicas comunicam os resultados da execução
*     de cada comando por meio de uma condição de retorno.
*
*     Cada linha do arquivo script de teste corresponde a um comando de
*     teste.
*
*     Caso seja encontrada uma falha não esperada (ver comando =recuperar)
*     os restantes comandos do caso de teste serão ignorados e contados
*     como problemas.
*
*     A sintaxe utilizada pelos comandos de teste é semelhante a assembler.
*     Esta sintaxe facilita a análise:
*
*        =<cmd> <parâmetros>
*
*     em que:
*        =            - todos os comandos a serem interpretados iniciam
*                       com este caractere
*        <cmd>        - é o string que identifica o comando
*        <parâmetros> - são zero ou mais itens a serem utilizados
*                       pelo comando de teste. Os itens devem, de preferência,
*                       ser separados um do outro por um caractere "espaço"
*
*     Deve ser adotado o seguinte padrão de uso:
*
*     - procure sempre utlizar uma das funções TST_CompararXXX, na forma:
*          return TST_CompararXXX( valoresperado, funcao a testar , mensagem )
*
*     - as funções de comparação fornecidas sempre geram as mensagens no
*       padrão estipulado
*
*     - quando não for possível utilizar uma função de comparação, reporte
*       a falha, esperadas ou não, através da função
*          TST_ImprimirPrefixo( Mensagem )
*
*     - a seguir mostre o valor esperado e o obtido
*
*     - Informações relativas a falhas dos testes são sinalizadas por
*       uma mensagem iniciando com ">>>" o número da linha do comando de teste
*       e o número de falhas encontradas até o momento.
*
*     - Informações relativas à execução dos testes são sinalizadas por
*       uma mensagem iniciando com "!!!".
*
*  $FV Valor retornado
*     TST_CondRetErro   - caso tenha sido encontrado um ou mais problemas
*     TST_CondRetOK     - caso o teste tenha terminado com zero problemas
*                         não esperados.
*
*  $FGP Tipos e Variáveis globais externas do próprio módulo
*     pArqScript - deve referenciar o arquivo script aberto
*     pArqLog    - deve referenciar o arquivo log aberto
*
***********************************************************************/

   TST_tpCondRet RealizarTeste( char * Construto )
   {

      int NumLidos ,
          SaltaComandos ;

      int EhCasoTesteVazio = 0 ;

      char ComandoTeste[ DIM_COMANDO_TESTE ] ;

      int Dummy ;

      TST_tpCondRet CondRetornada = TST_CondRetErro ;

      if ( pArqLog == stdout )
      {
         fprintf( pArqLog , "\n" ) ;

      } else
      {
         fprintf( pArqLog , "\n%s Inicio dos testes" ,
                  SINALIZA_COMENTARIO ) ;
         fprintf( pArqLog , "\n%s      Construto sob teste: %s" ,
                  SINALIZA_COMENTARIO , Construto ) ;
         fprintf( pArqLog , "\n%s      Script de teste:     %s\n" ,
                  SINALIZA_COMENTARIO , LER_ObterNomeArqScript( )) ;
      } /* if */

      SaltaComandos = 1 ;
      NumLidos = LER_LerLinhaScript( ) ;

      CED_InicializarControlador( ) ;

      while ( NumLidos >= 0 ) {

      /* Obter comando de teste */

         if ( LER_TamBuffer > 0 )
         {

            ComandoTeste[ 0 ] = 0 ;
            ComandoTeste[ DIM_COMANDO_TESTE - 1 ] = 0 ;
            ComandoTeste[ DIM_COMANDO_TESTE - 2 ] = '\xA9' ; /* controles de extravasão do buffer de leitura */

            if ( sscanf( LER_Buffer , "%s" , ComandoTeste ) != 1 )
            {
               ContaFalhas ++ ;
               TST_ExibirPrefixo( SINALIZA_ERRO , "Linha não contém comando:" ) ;
               fprintf( pArqLog , " %s" , LER_Buffer ) ;
               LER_TamBuffer = -1 ;

            } else if ( ( ComandoTeste[ DIM_COMANDO_TESTE - 1 ] != 0 )
                     && ( ComandoTeste[ DIM_COMANDO_TESTE - 2 ] != '\xA9' ))
            {
               ContaFalhas ++ ;
               TST_ExibirPrefixo( SINALIZA_ERRO , "Comando extravasou o espaço para leitura:" ) ;
               fprintf( pArqLog , " %s" , LER_Buffer ) ;
               LER_TamBuffer = -1 ;
               break ;
            } /* if */

         } /* fim ativa: Obter comando de teste */

      /* Tratar linha em branco */

         if ( LER_TamBuffer <= 0 )
         {

         } /* fim ativa: Tratar linha em branco */

      /* Tratar comentário */

         else if ( memcmp( ComandoTeste , COMMENT_CMD , strlen( COMMENT_CMD )) == 0 )
         {

            TST_ExibirPrefixo( SINALIZA_COMENTARIO , LER_Buffer ) ;

         } /* fim ativa: Tratar comentário */

      /* Tratar comando breakpoint */

         else if ( strcmp( ComandoTeste , BKPT_CMD ) == 0 )
         {

            Dummy = 0 ;

         } /* fim ativa: Tratar comando breakpoint */

      /* Tratar de comando de cancelamento */

         else if ( strcmp( ComandoTeste , CANCELA_CMD ) == 0 )
         {

            ContaFalhas ++ ;
            TST_ExibirPrefixo( SINALIZA_COMENTARIO ,
                      "Solicitado o cancelamento da execução do script de teste." ) ;
            break ;

         } /* fim ativa: Tratar de comando de cancelamento */

      /* Tratar início de caso de teste */

         else if ( memcmp( ComandoTeste , INICIO_CMD , strlen( INICIO_CMD )) == 0 )
         {

            if ( EhCasoTesteVazio != 0 )
            {
               ContaFalhas ++ ;
               TST_ExibirPrefixo( SINALIZA_ERRO , "O caso de teste anterior está vazio." ) ;
            } /* if */

            fprintf( pArqLog , "\n%5d  %s" , LER_ObterNumLinhas( ) , LER_Buffer ) ;
            ContaCasosTeste ++ ;
            SaltaComandos    = 0 ;

            EhCasoTesteVazio = 1 ;

         } /* fim ativa: Tratar início de caso de teste */

      /* Interpretar comandos de teste específicos */

         else if ( SaltaComandos == 0 )
         {

            EhCasoTesteVazio = 0 ;
            CondRetornada = InterpretarComandos( ComandoTeste ) ;

            switch ( CondRetornada ) {

            /* Tratar retorno OK */

               case TST_CondRetOK :
               {

                  ContaComandosExecutados ++ ;

                  break ;

               } /* fim ativa: Tratar retorno OK */

            /* Tratar retorno função testada não retornou OK */

               case TST_CondRetErro :
               {

                  NumLidos = LER_LerLinhaScript( ) ;

                  if ( memcmp( LER_Buffer , RECUPERA_CMD , strlen( RECUPERA_CMD )) == 0 )
                  {
                     ContaFalhas -- ;
                     TST_ExibirPrefixo( SINALIZA_RECUPERA , "Falha esperada foi recuperada." ) ;
                  } else {
                     if ( LER_Buffer[ 0 ] != 0 )
                     {
                        ContaFalhas ++ ;
                        TST_ExibirPrefixo( SINALIZA_ERRO , "Não é comando recuperar:" ) ;
                        fprintf( pArqLog , " %s" , LER_Buffer ) ;
                        SaltaComandos = 1 ;
                     } /* if */
                  } /* if */

                  break ;

               } /* fim ativa: Tratar retorno função testada não retornou OK */

            /* Tratar retorno erro de parâmetro em comando */

               case TST_CondRetParm :
               {

                  ContaFalhas ++ ;
                  TST_ExibirPrefixo( SINALIZA_ERRO , "Erro nos parâmetros do comando:" ) ;
                  fprintf( pArqLog , " %s" , LER_Buffer ) ;

                  NumLidos = LER_LerLinhaScript( ) ;

                  if ( memcmp( LER_Buffer , RECUPERA_CMD , strlen( RECUPERA_CMD )) == 0 )
                  {
                     ContaFalhas -- ;
                     TST_ExibirPrefixo( SINALIZA_RECUPERA , "Falha esperada foi recuperada." ) ;
                  } else {
                     SaltaComandos = 1 ;
                  } /* if */

                  break ;

               } /* fim ativa: Tratar retorno erro de parâmetro em comando */

            /* Tratar comando ainda não implementado */

               case TST_CondRetNaoImplementado :
               {

                  ContaFalhas ++ ;
                  TST_ExibirPrefixo( SINALIZA_ERRO , "Comando de teste ainda não implementado:" ) ;
                  fprintf( pArqLog , " %s" , LER_Buffer ) ;
                  SaltaComandos = 1 ;

                  break ;

               } /* fim ativa: Tratar comando ainda não implementado */

            /* Tratar retorno comando desconhecido */

               case TST_CondRetNaoConhec :
               case TST_CondRetNaoExecutou :
               {

                  ContaFalhas ++ ;
                  TST_ExibirPrefixo( SINALIZA_ERRO , "Comando não conhecido:" ) ;
                  fprintf( pArqLog , " %s" , LER_Buffer ) ;
                  SaltaComandos = 1 ;

                  break ;

               } /* fim ativa: Tratar retorno comando desconhecido */

            /* Tratar retorno faltou memória */

               case TST_CondRetMemoria :
               {

                  ContaFalhas ++ ;
                  TST_ExibirPrefixo( SINALIZA_ERRO , "Faltou memória para o comando:" ) ;
                  fprintf( pArqLog , " %s" , LER_Buffer ) ;
                  SaltaComandos = 1 ;

                  break ;

               } /* fim ativa: Tratar retorno faltou memória */

            /* Tratar retorno desconhecido */

               default :
               {

                  ContaFalhas ++ ;
                  TST_ExibirPrefixo( SINALIZA_ERRO , "Condição de retorno não conhecida:" ) ;
                  fprintf( pArqLog , " %d" , CondRetornada ) ;
                  SaltaComandos = 1 ;

                  break ;

               } /* fim ativa: Tratar retorno desconhecido */

            } /* fim seleciona: Interpretar comandos de teste específicos */

         } /* fim ativa: Interpretar comandos de teste específicos */

      /* Tratar salto de comandos */

         else
         {

            ContaFalhas ++ ;
            TST_ExibirPrefixo( SINALIZA_ERRO , "Comando de teste ignorado:" ) ;
            fprintf( pArqLog , " %s" , LER_Buffer ) ;

         } /* fim ativa: Tratar salto de comandos */

      /* Avançar para a próxima linha */

         NumLidos = LER_LerLinhaScript( ) ;

      } /* fim repete: Raiz de TSTG -Interpretar toda a massa de teste */

      if ( ContaFalhas > 0 )
      {
         return TST_CondRetErro ;
      } else {
         return TST_CondRetOK ;
      } /* if */

   } /* Fim função: TSTG -Interpretar toda a massa de teste */


/***********************************************************************
*
*  $FC Função: TSTG -Efetuar os comandos de teste específicos
*
*  $ED Descrição da função
*     Esse módulo implementa a seqüência (daisy chain) de interpretação
*     de comandos de teste.
*     Todos os módulos que implementam instrumentação pertencente ao
*     arcabouço devem retornar TST_CondRetNaoExecutou  caso o
*     comando a interpretar não possa ser interpretado pelo módulo
*     em questão.
*
*     O interpretador de comandos específicos do módulo a testar
*     deve ser sempre o último a ser chamado. Caso não consiga
*     identificar o comando a interpretar, deve retornar
*     TST_CondRetNaoConhec
*
***********************************************************************/

   TST_tpCondRet InterpretarComandos( char * ComandoTeste )
   {

      TST_tpCondRet CondRetornada = TST_CondRetErro ;

      int IntEsp   = -1 ,
          numLidos = -1  ;

      /* Tratar comando de recuperação de falhas acumuladas */

         if ( strcmp( ComandoTeste , ACEITA_ERRO_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "i" , &IntEsp ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            if ( IntEsp == ContaFalhas )
            {
               ContaFalhas = 0 ;
               TST_ExibirPrefixo( "<<<" , "Falhas registradas eram esperadas e foram recuperadas." ) ;
               return TST_CondRetOK ;
            } /* if */

            return TST_CompararInt( IntEsp , ContaFalhas ,
                      "Número de falhas esperadas errado." ) ;

         } /* fim ativa: Tratar comando de recuperação de falhas acumuladas */

      /* Interpretar comandos de leitura */

         CondRetornada = LER_InterpretarComandos( ComandoTeste ) ;
         if ( CondRetornada != TST_CondRetNaoExecutou )
         {
            return CondRetornada ;
         } /* if */

      /* Interpretar comandos de contagem */

         CondRetornada = ICNT_EfetuarComadoContagem( ComandoTeste ) ;
         if ( CondRetornada != TST_CondRetNaoExecutou )
         {
            return CondRetornada ;
         } /* if */

      /* Interpretar comandos de controle de acesso */

         CondRetornada = ICED_InterpretarTesteEspaco( ComandoTeste ) ;
         if ( CondRetornada != TST_CondRetNaoExecutou )
         {
            return CondRetornada ;
         } /* if */

      /* Interpretar comandos específicos do módulo a testar */

         return TST_EfetuarComando( ComandoTeste ) ;

   } /* Fim função: TSTG -Efetuar os comandos de teste específicos */


/***********************************************************************
*
*  $FC Função: TSTG -Exibir linha hexadecimal
*
*  $ED Descrição da função
*     Imprime um string de valores hexadecimais a partir de um offseta
*     de um espaço indicado por ponteiro.
*
***********************************************************************/

   void ImprimirHexadecimal( void * pValorParm ,
                             int    Offset     ,
                             int    TamValor    )
   {

      char * pValor ;
      char   Ch ;
      int    numBytes ;
      int    ValChar ;
      int    i ;

      pValor = ( char * ) pValorParm ;
      numBytes   = TamValor - Offset ;
      if ( numBytes > DIM_LINHA )
      {
         numBytes = DIM_LINHA ;
      } /* if */

      /* Imprime a parte hexadecimal */

         for( i = 0 ; i < numBytes ; i ++ )
         {
            ValChar = *( pValor + Offset + i ) ;
            fprintf( pArqLog , " %02X" , ValChar ) ;
         } /* for */

      /* Imprime espaços para alinhar a parte ASCII */

         for( ; i < DIM_LINHA ; i ++ )
         {
            fprintf( pArqLog , "   " ) ;
         } /* for */

      /* Imprime a parte ASCII */

         fprintf( pArqLog , "   " ) ;
         for( i = 0 ; i < numBytes ; i ++ )
         {
            Ch = *( pValor + Offset + i ) ;
            if ( ( Ch <   32 )
              || ( Ch == 127 )
              || ( Ch == 255 ))
            {
               Ch = '.' ;
            } /* if */
            fprintf( pArqLog , "%c" , Ch ) ;
         } /* for */

   } /* Fim função: TSTG -Exibir linha hexadecimal */


/***********************************************************************
*
*  $FC Função: TSTG -Exibir resultados do teste
*
*  $ED Descrição da função
*     Exibe os contadores e a condição final dos testes
*
***********************************************************************/

   void ExibirResultados( FILE * pArq )
   {

      /* Exibir contagens */

         fprintf( pArq , "\n\n%s Número de linhas lidas:       %5i" ,
                  SINALIZA_COMENTARIO , LER_ObterNumLinhas( ) ) ;
         fprintf( pArq ,   "\n%s Número de casos de teste:     %5i" ,
                  SINALIZA_COMENTARIO , ContaCasosTeste ) ;
         fprintf( pArq ,   "\n%s Número comandos corretos:     %5i" ,
                  SINALIZA_COMENTARIO , ContaComandosExecutados ) ;
         fprintf( pArq ,   "\n%s Número de falhas encontradas: %5i" ,
                  SINALIZA_COMENTARIO , ContaFalhas ) ;

      /* Exibir laudo final */

         if ( ContaFalhas == 0 )
         {
            fprintf( pArq ,   "\n\n%s Não foram encontrados problemas\n" ,
                     SINALIZA_COMENTARIO ) ;
         } else {
            fprintf( pArq ,   "\n\n%s Foram encontrados problemas\n" ,
                     SINALIZA_COMENTARIO ) ;
         } /* if */

   } /* Fim função: TSTG -Exibir resultados do teste */

/********** Fim do módulo de implementação: TSTG Controlador de teste genérico **********/

