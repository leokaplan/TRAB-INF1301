/***************************************************************************
*  $MCI Módulo de implementação: LER  Ler parâmetros de comandos
*
*  Arquivo gerado:              LerParm.c
*  Letras identificadoras:      LER
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

#include  <stdio.h>
#include  <string.h>
#include  <stdarg.h>
#include  <ctype.h>
#include  <malloc.h>

#include "GENERICO.H"
#include "TABSIMB.H"

#define LerParm_OWN
#include "LERPARM.H"
#undef LerParm_OWN

#define            ID_TIPO_NOME            'n'
#define            ID_TIPO_STRING          's'
#define            ID_TIPO_CHAR            'c'
#define            ID_TIPO_INT             'i'
#define            ID_TIPO_DOUBLE          'f'

static const char  NOME_TIPO_NOME[ ]       = "nome" ;
static const char  NOME_TIPO_STRING[ ]     = "string" ;
static const char  NOME_TIPO_CHAR[ ]       = "char" ;
static const char  NOME_TIPO_INT[ ]        = "int" ;
static const char  NOME_TIPO_DOUBLE[ ]     = "double" ;

static const char DECLARAR_SIMBOLO_CMD[ ]  = "=declararparm" ;
static const char EXIBIR_SIMBOLO_CMD[ ]    = "=exibirdeclaracao" ;

#define  DIM_NOME_ARQ   250
#define  DIM_TABELA     317
#define  DIM_TIPO        16

#define  TRUE      1
#define  FALSE     0

#define  ERRO_LER  "ll>"
#define  INFO_LER  "\nll!"

#define  DEFAULT_EXT_SCRIPT  ".script"

#define  TRIM_CHAR           " \n\r\t\x026"

#define  PREFIXO_COMANDO     '='

/***********************************************************************
*
*  $TC Tipo de dados: LER Valores tipados dos parâmetros
*
*
***********************************************************************/

   typedef union {

         int ValorInt ;
               /* Valor tipo inteiro */

         char ValorChar ;
               /* Valor tipo char */

         double ValorDouble ;
               /* Valor tipo double */

         char * ValorString ;
               /* Valor tipo string */

   } tpValorTipado ;

/***********************************************************************
*
*  $TC Tipo de dados: LER Elemento de tabela de simbolos
*
*
***********************************************************************/

   typedef struct {

         char Nome[ LER_DIM_NOME ] ;
               /* Nome do parâmetro */

         tpValorTipado ValorTipado ;
               /* Valor do parâmetro */

         int tamString ;
               /* Tamanho de valor tipo string */

         char idTipo ;
               /* Tipo do parâmetro */

   } tpElemSimbolo ;

/*****  Dados encapsulados no módulo  *****/

      static char NomeArqScript[ DIM_NOME_ARQ ] ;
            /* Nome do arquivo script */

      static FILE * pArqScript = NULL ;
            /* Ponteiro para o descritor do arquivo script */

      static int ContaLinhas = 0 ;
            /* Número de linhas lidas */

      static int AcabouScript = 1 ;
            /* Indicador de término de leitura
               *
               *$EED Assertivas estruturais
               *   == 0 se o arquivo ainda não chegou ao seu final. */

      static int  Inicializado = 0 ;
            /* Estado de inicialização do módulo LER */

      static TBS_tppTabela pTabela = NULL ;
            /* Ponteiro para a tabela de símbolos utilizada */

/***** Protótipos das funções encapuladas no módulo *****/

   static void FecharScript( void ) ;

   static char LerCharCntrl( void ) ;

   static char * ObterSimbolo( void * pDado ) ;

   static tpElemSimbolo * ObterDado( char idTipo ) ;

   static void PularBrancos( void ) ;

   static int EhBranco( char Ch ) ;

   static void ExibirErro( char * Msg ) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: LER  &Inicializar módulo LerParm
*  ****/

   int LER_InicializarLeitura( void )
   {

      #ifdef _DEBUG
         TST_ASSERT( ! Inicializado ) ;
      #endif

      NomeArqScript[ 0 ] = 0 ;
      pArqScript         = NULL ;
      AcabouScript       = 1 ;
      ContaLinhas        = 0 ;

      LER_Buffer[ 0 ]    = 0 ;
      LER_TamBuffer      = 0 ;

      pTabela = TBS_CriarTabela( DIM_TABELA , ObterSimbolo , NULL ) ;
      if ( pTabela == NULL )
      {
         return FALSE ;
      } /* if */

      Inicializado = 1 ;

      return TRUE ;

   } /* Fim função: LER  &Inicializar módulo LerParm */

/***************************************************************************
*
*  Função: LER  &Terminar módulo LerParm
*  ****/

   void LER_TerminarLeitura( void )
   {

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
      #endif

      FecharScript( ) ;

      TBS_DestruirTabela( pTabela ) ;
      pTabela = NULL ;

      Inicializado = 0 ;

   } /* Fim função: LER  &Terminar módulo LerParm */

/***************************************************************************
*
*  Função: LER  &Obter nome do arquivo script
*  ****/

   char * LER_ObterNomeArqScript( void )
   {

      if ( Inicializado )
      {
         return NomeArqScript ;
      } /* if */

      return NULL ;

   } /* Fim função: LER  &Obter nome do arquivo script */

/***************************************************************************
*
*  Função: LER  &Obter número de linhas lidas até o momento
*  ****/

   int LER_ObterNumLinhas( void )
   {

      return ContaLinhas ;

   } /* Fim função: LER  &Obter número de linhas lidas até o momento */

/***************************************************************************
*
*  Função: LER  &Abrir arquivo script de teste
*  ****/

   LER_tpCondRet LER_AbrirArquivoScript( char * NomeArqParm )
   {

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
      #endif

      strcpy( NomeArqScript , NomeArqParm ) ;
      if ( TST_ObterInicioExtensao( NomeArqScript ) == NULL )
      {
         strcat( NomeArqScript , DEFAULT_EXT_SCRIPT ) ;
      } /* if */

      pArqScript = fopen( NomeArqScript , "rb" ) ;
      if ( pArqScript != NULL )
      {
         printf( "\n      Arquivo de teste:    %s\n" , NomeArqParm ) ;
         AcabouScript = 0 ;
         return LER_CondRetOK ;
      } /* if */

      NomeArqScript[ 0 ] = 0 ;
      return LER_CondRetNaoAbriu ;

   } /* Fim função: LER  &Abrir arquivo script de teste */

/***************************************************************************
*
*  Função: LER  &Ler linha script eliminando brancos ao final
*  ****/

   int LER_LerLinhaScript( void )
   {

      char * pLido ;

      int i ;

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
         TST_ASSERT( pArqScript != NULL ) ;
      #endif

      /* Tratar arquivo já chegou ao final */

         if ( feof( pArqScript )
           || AcabouScript      )
         {
            AcabouScript     = 1 ;
            LER_TamBuffer    = 0 ;
            LER_Buffer[ 0 ]  = 0 ;
            return -1 ;
         } /* if */

      /* Ler linha */

         LER_InxBuffer = 0 ;
         pLido = fgets( LER_Buffer , LER_DIM_BUFFER , pArqScript ) ;

         if ( pLido != NULL )
         {
            ContaLinhas ++ ;
            LER_TamBuffer = strlen( LER_Buffer ) ;
         } else
         {
            AcabouScript     = 1 ;
            LER_Buffer[ 0 ]  = 0 ;
            LER_TamBuffer    = 0 ;
            if ( feof( pArqScript ))
            {
               return -1 ;
            } else
            {
               return -2 ;
            } /* if */
         } /* if */

      /* Eliminar brancos ao final */

         for ( i = LER_TamBuffer - 1 ; i >= 0 ; i-- )
         {
            if ( strchr( TRIM_CHAR , LER_Buffer[ i ] ) == NULL )
            {
               break ;
            } /* if */
         } /* for */

         LER_TamBuffer = i + 1 ;
         LER_Buffer[ LER_TamBuffer ] = 0 ;

         return LER_TamBuffer ;

   } /* Fim função: LER  &Ler linha script eliminando brancos ao final */

/***************************************************************************
*
*  Função: LER  &Interpretar comandos de teste
*  ****/

   TST_tpCondRet LER_InterpretarComandos( char * ComandoTeste )
   {

      char Parm[ LER_DIM_NOME ] ;
      int  tamParm ;

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
      #endif

      /* Tratar declaração de nome */

         if ( strcmp( ComandoTeste , DECLARAR_SIMBOLO_CMD ) == 0 )
         {

            if ( ! LER_DeclararParametro( ))
            {
               return TST_NotificarFalha( "Não declarou o parâmetro." ) ;
            } /* if */

            return TST_CondRetOK ;

         } /* fim ativa: Tratar declaração de nome */

      /* Tratar exibição de uma declaração */

         else if ( strcmp( ComandoTeste , EXIBIR_SIMBOLO_CMD ) == 0 )
         {

            LER_PularComando( ) ;

            if ( ! LER_LerParmNome( Parm , &tamParm , LER_DIM_NOME ))
            {
               return TST_CondRetParm ;
            } /* if */

            LER_ExibirParametro( Parm ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar exibição de uma declaração */

      /* Tratar comando não é para LER */

         return TST_CondRetNaoExecutou ;

   } /* Fim função: LER  &Interpretar comandos de teste */

/***************************************************************************
*
*  Função: LER  &Declarar parâmetro simbólico
*  ****/

   int LER_DeclararParametro( void )
   {

      char NomeDeclarado[ LER_DIM_NOME ] ;

      tpElemSimbolo * pElem = NULL ;

      int tamNome ;

      char NomeTipo[ DIM_TIPO ] ;

      int   tamString ;
      char  valString[ LER_DIM_STRING ] ;

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
      #endif

      LER_InxBuffer = 0 ;

      /* Criar declaração de parâmetro */

         LER_PularComando( ) ;

         if ( LER_LerParmNome( NomeDeclarado , &tamNome , LER_DIM_NOME ) == FALSE )
         {
            ExibirErro( "Nome a declarar está errado." ) ;
            return FALSE ;
         } /* if */

         if ( TBS_ProcurarSimbolo( pTabela, NomeDeclarado ) != NULL )
         {
            ExibirErro( "Nome já foi declarado antes:" ) ;
            fprintf( TST_ObterArqLog( ) , " %s" , NomeDeclarado ) ;
            return FALSE ;
         } /* if */

         pElem = ( tpElemSimbolo * ) malloc( sizeof( tpElemSimbolo )) ;
         memset( pElem , 0 , sizeof( tpElemSimbolo )) ;

         if ( pElem == NULL )
         {
            ExibirErro( "Faltou memória." ) ;
            fprintf( TST_ObterArqLog( ) , " %s" , NomeDeclarado ) ;
            return FALSE ;
         } /* if */

         strcpy( pElem->Nome , NomeDeclarado ) ;

      /* Inserir tipo e valor */

         if ( LER_LerParmNome( NomeTipo , &tamNome , DIM_TIPO ) == FALSE )
         {
            ExibirErro( "Nome de tipo incorreto." ) ;
            free( pElem ) ;
            return FALSE ;
         } /* if */

         /* Inserir char */

            if ( strcmp( NomeTipo , NOME_TIPO_CHAR ) == 0 )
            {

               pElem->idTipo = ID_TIPO_CHAR ;
               if ( LER_LerParmChar( &( pElem->ValorTipado.ValorChar ) ) == FALSE )
               {
                  free( pElem ) ;
                  return FALSE ;
               } /* if */

            } /* fim ativa: Inserir char */

         /* Inserir inteiro */

            else if ( strcmp( NomeTipo , NOME_TIPO_INT ) == 0 )
            {

               pElem->idTipo = ID_TIPO_INT ;
               if ( LER_LerParmInt( &( pElem->ValorTipado.ValorInt ) ) == FALSE )
               {
                  free( pElem ) ;
                  return FALSE ;
               } /* if */

            } /* fim ativa: Inserir inteiro */

         /* Inserir double */

            else if ( strcmp( NomeTipo , NOME_TIPO_DOUBLE ) == 0 )
            {

               pElem->idTipo = ID_TIPO_DOUBLE ;
               if ( LER_LerParmDouble( &( pElem->ValorTipado.ValorDouble ) ) == FALSE )
               {
                  free( pElem ) ;
                  return FALSE ;
               } /* if */

            } /* fim ativa: Inserir double */

         /* Inserir string */

            else if ( strcmp( NomeTipo , NOME_TIPO_STRING ) == 0 )
            {

               pElem->idTipo = ID_TIPO_STRING ;
               if ( LER_LerParmString( valString , &tamString , LER_DIM_STRING ) == FALSE )
               {
                  free( pElem ) ;
                  return FALSE ;
               } /* if */
               pElem->ValorTipado.ValorString = ( char * ) malloc( tamString + 1 ) ;
               pElem->tamString               = tamString ;
               memcpy( pElem->ValorTipado.ValorString , valString , tamString + 1 ) ;

            } /* fim ativa: Inserir string */

         /* Tratar erro tipo desconhecido */

            else
            {

               ExibirErro( "Tipo de parâmetro desconhecido." ) ;
               free( pElem ) ;
               return FALSE ;

            } /* fim ativa: Tratar erro tipo desconhecido */

      /* Inserir declaração na tabela */

         if ( TBS_InserirSimbolo( pTabela, pElem ) != TBS_CondRetOK )
         {
            ExibirErro( "Ocorreu erro ao inserir símbolo na tabela." ) ;
            free( pElem ) ;
            return FALSE ;
         } /* if */

         return TRUE ;

   } /* Fim função: LER  &Declarar parâmetro simbólico */

/***************************************************************************
*
*  Função: LER  &Exibir declaração de símbolo
*  ****/

   void LER_ExibirParametro( char * Simbolo )
   {

      tpElemSimbolo * pElem ;

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
      #endif

      pElem = ( tpElemSimbolo * ) TBS_ProcurarSimbolo( pTabela , Simbolo ) ;

      if ( pElem == NULL )
      {
         TST_ExibirPrefixo( INFO_LER , "Parâmetro não definido: " ) ;
         fprintf( TST_ObterArqLog( ) , "%s" , Simbolo ) ;
         return ;
      } /* if */

      TST_ExibirPrefixo( INFO_LER , "Parâmetro: " ) ;
      fprintf( TST_ObterArqLog( ) , "%s" , Simbolo ) ;
      TST_ExibirEspacoHexa( sizeof( tpElemSimbolo ) , pElem ) ;

      if ( ( pElem->idTipo == ID_TIPO_STRING )
        || ( pElem->idTipo == ID_TIPO_NOME ))
      {
         TST_ExibirEspacoHexa( strlen( pElem->ValorTipado.ValorString ) + 1 ,
                                       pElem->ValorTipado.ValorString ) ;
      } /* if */

   } /* Fim função: LER  &Exibir declaração de símbolo */

/***************************************************************************
*
*  Função: LER  &Ler linha de parâmetros
*  ****/

   int LER_LerParametros( char * Tipos , ... )
   {

      va_list ListaParm ;

      int ContaCampos = 0 ;
      int i ;

      int CondRet ;

      char * pChar ;

      int * pInt ;

      double * pDouble ;

      char * pNome ;
      int    tamNome ;

      char * pString ;
      int    tamString ;

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
         TST_ASSERT( Tipos != NULL ) ;
      #endif

      va_start( ListaParm, Tipos );

      LER_InxBuffer = 0 ;

      LER_PularComando( ) ;

      for( i = 0 ; Tipos[ i ] != '\0' ; i++ ) {

      /* Extrair parâmetro */

         if ( LER_InxBuffer >= LER_TamBuffer )
         {
            va_end( ListaParm );
            return ContaCampos ;
         } /* if */

         switch ( Tipos[ i ] ) {

         /* Tratar parâmetro tipo caractere */

            case ID_TIPO_CHAR :
            {

               pChar   = va_arg( ListaParm, char * ) ;

               TST_ASSERT( pChar != NULL ) ;

               CondRet = LER_LerParmChar( pChar ) ;
               if ( CondRet == 0 )
               {
                  va_end( ListaParm );
                  return ContaCampos ;
               } /* if */

               break;

            } /* fim ativa: Tratar parâmetro tipo caractere */

         /* Tratar parâmetro tipo inteiro */

            case ID_TIPO_INT :
            {

               pInt = va_arg( ListaParm, int * );

               TST_ASSERT( pInt != NULL ) ;

               CondRet = LER_LerParmInt( pInt ) ;
               if ( CondRet == 0 )
               {
                  va_end( ListaParm );
                  return ContaCampos ;
               } /* if */

               break;

            } /* fim ativa: Tratar parâmetro tipo inteiro */

         /* Tratar parâmetro tipo flutuante */

            case ID_TIPO_DOUBLE :
            {

               pDouble = va_arg( ListaParm, double * );

               TST_ASSERT( pDouble != NULL ) ;

               CondRet = LER_LerParmDouble( pDouble ) ;
               if ( CondRet == 0 )
               {
                  va_end( ListaParm );
                  return ContaCampos ;
               } /* if */

               break;

            } /* fim ativa: Tratar parâmetro tipo flutuante */

         /* Tratar parâmetro tipo nome */

            case ID_TIPO_NOME :
            {

               pNome = va_arg( ListaParm, char * );

               TST_ASSERT( pNome != NULL ) ;

               CondRet = LER_LerParmNome( pNome , &tamNome , LER_DIM_NOME ) ;
               if ( CondRet == 0 )
               {
                  va_end( ListaParm );
                  return ContaCampos ;
               } /* if */

               break;

            } /* fim ativa: Tratar parâmetro tipo nome */

         /* Tratar parâmetro tipo string */

            case ID_TIPO_STRING :
            {

               pString = va_arg( ListaParm, char * );

               TST_ASSERT( pString != NULL ) ;

               CondRet = LER_LerParmString( pString , &tamString , LER_DIM_STRING ) ;
               if ( CondRet == 0 )
               {
                  va_end( ListaParm );
                  return ContaCampos ;
               } /* if */

               break;

            } /* fim ativa: Tratar parâmetro tipo string */

         /* Tratar erro de parâmetro */

            default:
            {

               ExibirErro( "Tipo de parâmetro desconhecido." ) ;
               va_end( ListaParm );
               return ContaCampos ;

               break ;

            } /* fim ativa: Tratar erro de parâmetro */

         } /* fim seleciona: Extrair parâmetro */

         ContaCampos ++ ;

      } /* fim repete: Raiz de LER  &Ler linha de parâmetros */

      va_end( ListaParm );

      return ContaCampos ;

   } /* Fim função: LER  &Ler linha de parâmetros */

/***************************************************************************
*
*  Função: LER  &Ler parâmetro caractere
*  ****/

   int LER_LerParmChar( char * Parm )
   {

      tpElemSimbolo * pElem = NULL ;

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
         TST_ASSERT( Parm != NULL ) ;
      #endif

      PularBrancos( ) ;

      /* Tratar parâmetro char simbólico */

         if ( isalpha( LER_Buffer[ LER_InxBuffer ] ))
         {

            pElem = ObterDado( ID_TIPO_CHAR ) ;
            if ( pElem == NULL )
            {
               *Parm = 0 ;
               return FALSE ;
            } /* if */

            * Parm = pElem->ValorTipado.ValorChar ;
            return TRUE ;

         } /* fim ativa: Tratar parâmetro char simbólico */

      /* Tratar parâmetro char literal */

         if ( LER_Buffer[ LER_InxBuffer ] != '\'' )
         {
            * Parm = 0 ;
            ExibirErro( "Faltou aspas antes de literal char." ) ;
            return FALSE ;
         } /* if */
         LER_InxBuffer ++ ;

         if ( LER_Buffer[ LER_InxBuffer ] == '\\' )
         {
            LER_InxBuffer ++ ;
            *Parm = LerCharCntrl( ) ;

         } else
         {
            *Parm = LER_Buffer[ LER_InxBuffer ] ;
            LER_InxBuffer ++ ;
         } /* if */

         if ( LER_Buffer[ LER_InxBuffer ] != '\'' )
         {
            ExibirErro( "Faltou aspas após literal char." ) ;
            return FALSE ;
         } /* if */
         LER_InxBuffer ++ ;

         return TRUE ;

   } /* Fim função: LER  &Ler parâmetro caractere */

/***************************************************************************
*
*  Função: LER  &Ler parâmetro inteiro
*  ****/

   int LER_LerParmInt( int * Parm )
   {

      tpElemSimbolo * pElem = NULL ;

      int inxOrg ;
      int ValInt ;

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
         TST_ASSERT( Parm != NULL ) ;
      #endif

      PularBrancos( ) ;

      /* Tratar parâmetro int simbólico */

         if ( isalpha( LER_Buffer[ LER_InxBuffer ] ))
         {

            pElem = ObterDado( ID_TIPO_INT ) ;
            if ( pElem == NULL )
            {
               *Parm = 0 ;
               return FALSE ;
            } /* if */

            * Parm = pElem->ValorTipado.ValorInt ;
            return TRUE ;

         } /* fim ativa: Tratar parâmetro int simbólico */

      /* Tratar parâmetro int literal */

         inxOrg = LER_InxBuffer ;
         if ( ( LER_Buffer[ LER_InxBuffer ] == '-' )
           || ( LER_Buffer[ LER_InxBuffer ] == '+' ))
         {
            LER_InxBuffer ++ ;
         } /* if */

         while ( LER_InxBuffer < LER_TamBuffer )
         {
            if ( !isdigit( LER_Buffer[ LER_InxBuffer ] ) )
            {
               break ;
            } /* if */
            LER_InxBuffer ++ ;
         } /* while */

         if ( sscanf( LER_Buffer + inxOrg , "%d" , &ValInt ) != 1 )
         {
            *Parm = 0 ;
            ExibirErro( "Literal int errado." ) ;
            return FALSE ;
         } /* if */

         *Parm = ValInt ;

         return TRUE ;

   } /* Fim função: LER  &Ler parâmetro inteiro */

/***************************************************************************
*
*  Função: LER  &Ler parâmetro flutuante
*  ****/

   int LER_LerParmDouble( double * Parm )
   {

      tpElemSimbolo * pElem = NULL ;

      int    inxOrg ;
      double ValDouble ;

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
         TST_ASSERT( Parm != NULL ) ;
      #endif

      PularBrancos( ) ;

      /* Tratar parâmetro double simbólico */

         if ( isalpha( LER_Buffer[ LER_InxBuffer ] ))
         {

            pElem = ObterDado( ID_TIPO_DOUBLE ) ;
            if ( pElem == NULL )
            {
               *Parm = 0. ;
               return FALSE ;
            } /* if */

            * Parm = pElem->ValorTipado.ValorDouble ;
            return TRUE ;

         } /* fim ativa: Tratar parâmetro double simbólico */

      /* Tratar parâmetro double literal */

         inxOrg = LER_InxBuffer ;

         while ( LER_InxBuffer < LER_TamBuffer )
         {
            if ( EhBranco( LER_Buffer[ LER_InxBuffer ] ))
            {
               break ;
            } /* if */
            LER_InxBuffer ++ ;
         } /* while */

         if ( sscanf( LER_Buffer + inxOrg , "%lf" , &ValDouble ) != 1 )
         {
            ExibirErro( "Literal double errado." ) ;
            *Parm = 0. ;
            return FALSE ;
         } /* if */

         *Parm = ValDouble ;

         return TRUE ;

   } /* Fim função: LER  &Ler parâmetro flutuante */

/***************************************************************************
*
*  Função: LER  &Ler parâmetro nome
*  ****/

   int LER_LerParmNome( char * Parm , int * tamNome , int dimNome )
   {

      int inxParm = 0 ;

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
      #endif

      PularBrancos( ) ;

      *tamNome = 0 ;

      if ( ! isalpha( LER_Buffer[ LER_InxBuffer ] ))
      {
         ExibirErro( "Nome deve iniciar com letra." ) ;
         Parm[ inxParm ] = 0 ;
         return 0 ;
      } /* if */

      Parm[ inxParm ] = LER_Buffer[ LER_InxBuffer ] ;

      LER_InxBuffer ++ ;
      inxParm ++ ;

      while ( LER_InxBuffer < LER_TamBuffer )
      {
         if ( (isalnum(LER_Buffer[ LER_InxBuffer ]) || (( LER_Buffer[ LER_InxBuffer ]) == '_')))
         // if ( __iscsym( LER_Buffer[ LER_InxBuffer ] ))
         {
            Parm[ inxParm ] = LER_Buffer[ LER_InxBuffer ] ;

         } else
         {
            Parm[ inxParm ] = 0 ;
            *tamNome        = inxParm ;
            return TRUE ;
         } /* if */

         LER_InxBuffer ++ ;
         inxParm ++ ;

         if ( inxParm > dimNome )
         {
            ExibirErro( "Nome longo demais." ) ;
            Parm[ dimNome - 1 ] = 0 ;
            *tamNome            = dimNome - 1 ;
            return FALSE ;
         } /* if */
         
      } /* while */

      Parm[ inxParm ] = 0 ;
      *tamNome        = inxParm ;
      return TRUE ;

   } /* Fim função: LER  &Ler parâmetro nome */

/***************************************************************************
*
*  Função: LER  &Ler parâmetro string
*  ****/

   int LER_LerParmString( char * Parm , int * tamString , int dimString )
   {

      tpElemSimbolo * pElem = NULL ;

      int inxParm = 0 ;

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
      #endif

      PularBrancos( ) ;

      /* Tratar parâmetro string simbólico */

         if ( isalpha( LER_Buffer[ LER_InxBuffer ] ))
         {

            pElem = ObterDado( ID_TIPO_STRING ) ;
            if ( pElem == NULL )
            {
               *Parm = 0 ;
               return FALSE ;
            } /* if */

            if ( pElem->tamString < dimString )
            {
               *tamString = pElem->tamString ;
               memcpy( Parm , pElem->ValorTipado.ValorString , pElem->tamString + 1 ) ;
            } else
            {
               ExibirErro( "String em parâmetro é longo demais." ) ;
               *tamString = dimString - 1 ;
               memcpy( Parm , pElem->ValorTipado.ValorString , dimString ) ;

               return FALSE ;
            } /* if */

            return TRUE ;

         } /* fim ativa: Tratar parâmetro string simbólico */

      /* Tratar parâmetro string literal */

         *tamString = 0 ;

         if ( LER_Buffer[ LER_InxBuffer ] != '\"' )
         {
            ExibirErro( "Faltou aspas no início do string." ) ;
            Parm[ inxParm ] = 0 ;
            return FALSE ;
         } /* if */
         LER_InxBuffer ++ ;

         while ( LER_InxBuffer < LER_TamBuffer )
         {
            if ( inxParm > dimString )
            {
               ExibirErro( "String longo demais." ) ;
               Parm[ dimString - 1 ] = 0 ;
               *tamString            = dimString - 1 ;
               return FALSE ;
            } /* if */

            if ( LER_Buffer[ LER_InxBuffer ] == '\\' )
            {
               LER_InxBuffer ++ ;

               Parm[ inxParm ] = LerCharCntrl( ) ;

            } else if ( LER_Buffer[ LER_InxBuffer ] == '\"' )
            {
               LER_InxBuffer ++ ;
               Parm[ inxParm ] = 0 ;
               *tamString      = inxParm ;
               return TRUE ;
            } else
            {
               Parm[ inxParm ] = LER_Buffer[ LER_InxBuffer ] ;
               LER_InxBuffer ++ ;
            } /* if */
            inxParm ++ ;

         } /* while */

         ExibirErro( "Faltou aspas no final do string." ) ;
         Parm[ inxParm ] = 0 ;
         *tamString      = inxParm ;
         return FALSE ;

   } /* Fim função: LER  &Ler parâmetro string */

/***************************************************************************
*
*  Função: LER  &Pular comando de teste
*  ****/

   void LER_PularComando( void )
   {

      int TemComando = 0 ;

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
      #endif

      while ( LER_InxBuffer < LER_TamBuffer )
      {
         if ( LER_Buffer[ LER_InxBuffer ] == PREFIXO_COMANDO )
         {
            TemComando = 1 ;
         } else if ( EhBranco( LER_Buffer[ LER_InxBuffer ] ))
         {
            if ( TemComando )
            {
               break ;
            } /* if */
         } /* if */
         LER_InxBuffer ++ ;
      } /* while */

   } /* Fim função: LER  &Pular comando de teste */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: LER  -Fechar arquivo script
*
***********************************************************************/

   void FecharScript( void )
   {

      #ifdef _DEBUG
         TST_ASSERT( Inicializado ) ;
      #endif

      if ( pArqScript != NULL )
      {
         fclose( pArqScript ) ;
         pArqScript = NULL ;
         NomeArqScript[ 0 ] = 0 ;
      } /* if */

   } /* Fim função: LER  -Fechar arquivo script */


/***********************************************************************
*
*  $FC Função: LER  -Ler caractere de controle
*
***********************************************************************/

   char LerCharCntrl( void )
   {

      int  intChar ,
           i ;

      int val ;

      char ValChar ;

      /* Ler escape octal */

         if ( strchr( "01234567" , LER_Buffer[ LER_InxBuffer ] ) != NULL )
         {

            intChar = 0 ;
            i       = 0 ;
            while ( ( i < 3 )
                 && ( strchr( "01234567" , LER_Buffer[ LER_InxBuffer ] ) != NULL ))
            {
               intChar = intChar * 8 + ( LER_Buffer[ LER_InxBuffer ] - '0' ) ;
               LER_InxBuffer ++ ;
               i ++ ;
            } /* while */

            if ( intChar > 255 )
            {
               ExibirErro( "Valor de caractere octal maior do que 255." ) ;
            } /* if */

            return ( char ) intChar ;

         } /* fim ativa: Ler escape octal */

      /* Ler escape hexadecimal */

         else if ( ( LER_Buffer[ LER_InxBuffer ] == 'X' )
                || ( LER_Buffer[ LER_InxBuffer ] == 'x' ))
         {

            LER_InxBuffer ++ ;

            intChar = 0 ;
            i       = 0 ;
            while ( ( i < 2 )
                 && ( strchr( "0123456789ABCDEFabcdef" ,
                      LER_Buffer[ LER_InxBuffer ] ) != NULL ))
            {
               if ( LER_Buffer[ LER_InxBuffer ] < 'A' )
               {
                  val = LER_Buffer[ LER_InxBuffer ] - '0' ;
               } else if ( LER_Buffer[ LER_InxBuffer ] < 'a' )
               {
                  val = LER_Buffer[ LER_InxBuffer ] - 'A' + 10 ;
               } else
               {
                  val = LER_Buffer[ LER_InxBuffer ] - 'a' + 10 ;
               } /* if */
               intChar = intChar * 16 + val ;
               LER_InxBuffer ++ ;
               i ++ ;
            } /* while */

            if ( intChar > 255 )
            {
               ExibirErro( "Valor de caractere hexadecimal maior do que 255." ) ;
            } /* if */

            return ( char ) intChar ;

         } /* fim ativa: Ler escape hexadecimal */

      /* Ler caracteres de escape simples */

         else
         {

            switch ( LER_Buffer[ LER_InxBuffer ] )
            {
               case 'n' :
                  ValChar = '\n' ;
                  break ;

               case 'r' :
                  ValChar = '\r' ;
                  break ;

               case 't' :
                  ValChar = '\t' ;
                  break ;

               case 'v' :
                  ValChar = '\v' ;
                  break ;

               case 'a' :
                  ValChar = '\a' ;
                  break ;

               case 'f' :
                  ValChar = '\f' ;
                  break ;

               case '?' :
                  ValChar = '\?' ;
                  break ;

               case '\'' :
                  ValChar = '\'' ;
                  break ;

               case '\"' :
                  ValChar = '\"' ;
                  break ;

               case '\\' :
                  ValChar = '\\' ;
                  break ;

               default :
                  ExibirErro( "Código de caractere especial desconhecido." ) ;
                  return LER_Buffer[ LER_InxBuffer ] ;

            } /* switch */

            LER_InxBuffer ++ ;
            return ValChar ;

         } /* fim ativa: Ler caracteres de escape simples */

   } /* Fim função: LER  -Ler caractere de controle */


/***********************************************************************
*
*  $FC Função: LER  -Obter nome de parâmetro declarado
*
***********************************************************************/

   char * ObterSimbolo( void * pDado )
   {

      return (( tpElemSimbolo * ) pDado)->Nome ;

   } /* Fim função: LER  -Obter nome de parâmetro declarado */


/***********************************************************************
*
*  $FC Função: LER  -Obter dado associado a parâmetro simbólico
*
***********************************************************************/

   tpElemSimbolo * ObterDado( char idTipo )
   {

      char Nome[ LER_DIM_NOME ] ;

      tpElemSimbolo * pElem = NULL ;

      int tamNome ;

      if ( LER_LerParmNome( Nome , &tamNome , LER_DIM_NOME ) == FALSE )
      {
         ExibirErro( "Sintaxe errada para nome de parâmetro." ) ;
         return NULL ;
      } /* if */

      pElem = ( tpElemSimbolo * ) TBS_ProcurarSimbolo( pTabela , Nome ) ;

      if ( pElem == NULL )
      {
         ExibirErro( "Parâmetro não está definido: " ) ;
         fprintf( TST_ObterArqLog( ) , "%s" , Nome ) ;
         return NULL ;
      } /* if */

      if ( pElem->idTipo != idTipo )
      {
         ExibirErro( "Tipo do parâmetro incorreto. " ) ;
         fprintf( TST_ObterArqLog( ) , "%s" , Nome ) ;
         return NULL ;
      } /* if */

      return pElem ;

   } /* Fim função: LER  -Obter dado associado a parâmetro simbólico */


/***********************************************************************
*
*  $FC Função: LER  -Pular brancos
*
***********************************************************************/

   void PularBrancos( void )
   {

      while ( LER_InxBuffer < LER_TamBuffer )
      {
         if ( !EhBranco( LER_Buffer[ LER_InxBuffer ] ))
         {
            break ;
         } /* if */
         LER_InxBuffer ++ ;
      } /* while */

   } /* Fim função: LER  -Pular brancos */


/***********************************************************************
*
*  $FC Função: LER  -Verificar se é branco
*
***********************************************************************/

   int EhBranco( char Ch )
   {

      if ( ( Ch == ' '  )
        || ( Ch == '\t' ))
      {
         return TRUE ;
      } /* if */

      return FALSE ;

   } /* Fim função: LER  -Verificar se é branco */


/***********************************************************************
*
*  $FC Função: LER  -Exibir erro
*
***********************************************************************/

   void ExibirErro( char * Msg )
   {

      TST_ContarFalhas( ) ;
      TST_ExibirPrefixo( ERRO_LER , Msg ) ;

   } /* Fim função: LER  -Exibir erro */

/********** Fim do módulo de implementação: LER  Ler parâmetros de comandos **********/

