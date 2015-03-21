/***************************************************************************
*  $MCI Módulo de implementação: CED  Controlador de espaços de dados alocados dinamicamente
*
*  Arquivo gerado:              CESPDIN.c
*  Letras identificadoras:      CED
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

#include    <malloc.h>
#include    <stdio.h>
#include    <memory.h>
#include    <string.h>

#define CESPDIN_OWN
#include "CESPDIN.H"
#undef CESPDIN_OWN

#include "GENERICO.H"
#include "GERAALT.H"

#include "../TABELAS/IDTIPOSESPACO.DEF"

    /* O pragma é necessário para assegurar alinhamento a byte,
       uma vez que toda a aritmética com ponteiros assume isso */

#pragma pack (1)

#define     DIM_NOME_ARQ_FONTE  32
#define     DIM_PONTEIRO         4

#define     TIPO_MSG_INFO     "!!!"
#define     TIPO_MSG_ERRO     "ed>"
#define     TIPO_MSG_VAZIA    "   "

#define     CHAR_ALOC      '|'
#define     CHAR_DESALOC   '?'

#define     LIMITE_FREQUENCIA      1000
#define     LIMITE_INF_FREQUENCIA    10
#define     LIMITE_SUP_FREQUENCIA  1000

/* Controle do conteúdo do espaço alocado
*
* Ao serem alocados, a parte Valor dos espaços é inicializados para
* CHAR_ALOC ("|"). Ao serem desalocados, os espaço todo é preenchido
* com CHAR_DESALOC ("?"). Isto permite verificar o estado do espaço
* ao acessá-lo. O objetivo é provocar erros sistemáticos caso um
* programa tente utilizar espaços desalocados, ou se esqueça de
* inicializar campos que contenha ponteiros.
*/

#define     DIM_VALOR      2
#define     DIM_CONTROLE   2
#define     CONTROLE       "\x96\xC3"
#define     CONTROLE_ERRO  "\x02\0"

/* Todos os espaços são antecedidos e sucedidos por strings constantes
* de controle (CONTROLE). Isto permite verificar se houve extravasão
* ao transferir dados para o espaço.
* O CONTROLE_ERRO é inserido após quando o espaço for liberado.
* Procura evitar que strings possam extravasar o espaço disponível.
*/

#define     ESPACO_NULL        0x98FB5D32L

/* Ao dar free o primeiro long é tornado igual a zero
*  Este valor indica que ainda não ocorreu um free no espaço
*/

#define     IMPRESSAO_DIGITAL  0xC07146AEL

/* Todos os espacos de dados teem esta marca digital.
* Isto permite assegurar que se trata de um espaço controlado.
* O que, por sua vez permite misturar módulos instrumentados
* com módulos não instrumentados sem que ocorram erros
* de processamento que acabam levando ao cancelamento do programa
*/

/***********************************************************************
*
*  $TC Tipo de dados: CED Elemento da lista de controle de espaços de dados
*
*
*  $ED Descrição do tipo
*     Esta estrutura define a organização de um espaço de dados
*     alocado pelo controlador de vazamento.
*     O elemento prevê campos para a identificação do tipo do espaço de
*     dados e para o controle de extravasão dos limites estabelecidos
*     para o espaço de dados útil.
*
***********************************************************************/

   typedef struct tgElemListaControle {

         unsigned long EspacoNULL ;
               /* Espaço para o NULL */

         unsigned long ImpressaoDigital ;
               /* Marcador de espaço controlado
               *
               *$ED Descrição
               *   O valor contido neste marcador é sempre o mesmo.
               *   Tem por objetivo servir de um identificador de que o espaço de dados
               *   corresponde a um espaço controlado.
               *   Caso não exista as operações de manipulação de espaço controlado são
               *   inibidas. Isto é particularmente importante para o free,
               *   uma vez que o espaço a ser liberado poderia ter sido criado
               *   sem em um módulo que não aloca de forma controlada. */

         int tamTotal ;
               /* Tamanho total do espaço
               *
               *$ED Descrição
               *   Tem por objetivo estabelecer uma redundância com tamValor. */

         struct tgElemListaControle * pAnt ;
               /* Ponteiro para o elemento anterior na lista de espaços alocados */

         struct tgElemListaControle * pProx ;
               /* Ponteiro para o elemento a seguir na lista de espaços alocados */

         int idEspaco ;
               /* Identificador do espaço
               *
               *$ED Descrição
               *   Identificador único do espaço alocado */

         int ehAtivo ;
               /* Marca de espaço ativo
               *
               *$ED Descrição
               *   Indica se o espaço é ou não ativo. A marca é estabelecida pelo usuário. */

         int idTipoValor ;
               /* Identificador do tipo do valor contido no espaço alocado
               *
               *$ED Descrição
               *   O tipo específico do valor contido neste elemento pode ser identificado
               *   por este atributo. Ao instrumentar um ou mais módulos, o programador
               *   cliente deve definir um inteiro que esteja em correspondência
               *   um para um com os diversos tipos de valores que serão alocados
               *   dinamicamente. Após criar o elemento deve ser atribuído o
               *   identificador do tipo do valor que será inserido no elemento. */

         int tamValor ;
               /* Dimensão, em bytes, do valor contido no espaço alocado */

         int numLinhaFonte ;
               /* Número da linha do código fonte */

         char NomeArquivoFonte[ DIM_NOME_ARQ_FONTE ] ;
               /* Nome arquivo fonte
               *
               *$ED Descrição
               *   Os atributos NumLinhaFonte e NomeArquivoFonte identificam o local
               *   no código fonte na qual a função CED_malloc foi chamada.
               *   Isto ajuda a identificar a causa dos possíveis problemas de manipulação
               *   de memória dinâmica encontrados ao executar o módulo controlado. */

         char ControleAntes[ DIM_CONTROLE ] ;
               /* Marcador de controle de extravasão antes do espaço */

         char Valor[ DIM_VALOR ] ;
               /* Dummy a ser substituído pelo espaço a ser ocupado pelo valor */

         char ControleApos[  DIM_CONTROLE ] ;
               /* Marcador de controle de extravasão após
               *
               *$ED Descrição
               *   A origem deste atributo varia em função da dimensão do valor
               *   contido no elemento. Ele é declarado somente para que se
               *   possa determinar o sizeof do elemento sem contar com o
               *   espaço requerido pelo valor. */

   } tpElemListaControle ;

/*****  Dados encapsulados no módulo  *****/

      #define  TRUE     1
      #define  FALSE    0
            /* Constantes lógicas */

      static int estaInicializado = FALSE ;
            /* Controle de inicialização */

      static tpElemListaControle * pOrgLista = NULL;
            /* Origem da lista de espaços alocados */

      static tpElemListaControle * pEspacoCorr = NULL;
            /* Iterador: espaço corrente
               *
               *$ED Descrição
               *   Aponta para o espaço corrente do iterador.
               *   Se NULL o iterador não está ativo. */

      static long numEspacosAlocados = 0 ;
            /* Contador de espaços alocados */

      static long TotalAlocacoes = 0 ;
            /* Contador do total de chamadas de alocação */

      static long EspacoTotalAlocado = 0 ;
            /* Espaço total alocado */

      static long LimiteMemoria = 0 ;
            /* Limite de memória disponível
               *
               *$ED Descrição
               *   Caso este valor seja maior do que zero, este valor restringe
               *   o total de memória que pode estar alocado a cada momento. */

      static long LimiteNumEspacos = 0 ;
            /* Limite de número de espaços alocados */

      static int FrequenciaMalloc ;
            /* Limite de freqüência estabelecido
               *
               *$ED Descrição
               *   Determina a freqüência de allocações que serão realizadas.
               *   Deixarão de ser realizadas alocações numa freqüência de
               *   1 - ( FrequenciaMalloc / LIMITE_FREQUENCIA ) */

      static long ContaNULL ;
            /* Contagem de NULLs gerados
               *
               *$ED Descrição
               *   Conta o número de vezes que o malloc não foi gerado devido a algum
               *   limite de alocação simulado. */

/***** Protótipos das funções encapuladas no módulo *****/

   static void DesalocarEspaco( tpElemListaControle * pEspaco ) ;

   static tpElemListaControle * ObterOrigemElemento( void * Ponteiro ) ;

   static void ExibirCabecalho( char * TipoMsg , tpElemListaControle * pEspaco ) ;

   static void ExibirMensagem( char * Msg ) ;

   static int VerificarEspaco( tpElemListaControle * pEspaco ) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: CED  &Inicializar o controlador de alocação de espaços dinâmicos
*  ****/

   void CED_InicializarControlador( void )
   {

      TST_ASSERT( !estaInicializado ) ;

      pOrgLista            = NULL ;
      pEspacoCorr          = NULL ;
      numEspacosAlocados   = 0 ;
      TotalAlocacoes       = 0 ;
      EspacoTotalAlocado   = 0 ;
      LimiteMemoria        = 0 ;
      LimiteNumEspacos     = 0 ;
      FrequenciaMalloc     = 0 ;
      ContaNULL            = 0 ;

      ALT_Inicializar( 0 ) ;

      estaInicializado     = TRUE ;

   } /* Fim função: CED  &Inicializar o controlador de alocação de espaços dinâmicos */

/***************************************************************************
*
*  Função: CED  &Alocar espaço malloc
*  ****/

   void * CED_Malloc( size_t Tamanho ,
                      int    numLinhaFonte ,
                      char * NomeArquivoFonte )
   {

      tpElemListaControle * pEspaco ;

      int tamNomeArq ;

      TST_ASSERT( Tamanho > 0 ) ;
      TST_ASSERT( numLinhaFonte > 0 ) ;
      TST_ASSERT( NomeArquivoFonte != NULL ) ;

      /* Alocar sem controle quando não inicializado */

         if ( ! estaInicializado )
         {
            return malloc( Tamanho ) ;
         } /* if */

      /* Controlar limites de memória */

         if ( LimiteMemoria > 0 )
         {
            if ( LimiteMemoria < EspacoTotalAlocado + ( long ) Tamanho )
            {
               TST_ExibirPrefixo( TIPO_MSG_INFO , "Simula falta de memoria." ) ;
               ContaNULL++ ;
               return NULL ;
            } /* if */
         } /* if */

         if ( LimiteNumEspacos > 0 )
         {
            if ( numEspacosAlocados >= LimiteNumEspacos )
            {
               TST_ExibirPrefixo( TIPO_MSG_INFO , "Limita numero de espacos alocados." ) ;
               ContaNULL++ ;
               return NULL ;
            } /* if */
         } /* if */

         if ( FrequenciaMalloc > 0 )
         {
            if ( ALT_GerarFrequencia( 1 , &FrequenciaMalloc , LIMITE_FREQUENCIA ) > 0)
            {
               TST_ExibirPrefixo( TIPO_MSG_INFO , "Simulou NULL por frequencia." ) ;
               ContaNULL++ ;
               return NULL ;
            } /* if */
         } /* if */

      /* Alocar o espaço solicitado */

         pEspaco = ( tpElemListaControle * )
                     malloc( sizeof( tpElemListaControle ) + Tamanho -
                             DIM_VALOR ) ;

         if ( pEspaco == NULL )
         {
            TST_ExibirPrefixo( TIPO_MSG_INFO , "Memoria real insuficiente." ) ;
            return NULL ;
         } /* if */


         /* Alocar o espaço solicitado */

         pEspaco = ( tpElemListaControle * )
                     malloc( sizeof( tpElemListaControle ) + Tamanho -
                             DIM_VALOR ) ;

         if ( pEspaco == NULL )
         {
            return NULL ;
         }

         EspacoTotalAlocado += Tamanho ;

      /* Inicializar os valores de controle do espaço */

         pEspaco->EspacoNULL       = ESPACO_NULL ;
         pEspaco->ImpressaoDigital = IMPRESSAO_DIGITAL ;
         pEspaco->tamTotal         = Tamanho + sizeof( tpElemListaControle ) ;
         pEspaco->idEspaco         = TotalAlocacoes ;
         pEspaco->idTipoValor      = CED_ID_TIPO_VALOR_NULO ;
         pEspaco->ehAtivo          = TRUE ;
         pEspaco->tamValor         = Tamanho ;
         memcpy( pEspaco->ControleAntes ,   CONTROLE  , DIM_CONTROLE ) ;
         memset( pEspaco->Valor ,           CHAR_ALOC , Tamanho ) ;
         memcpy( pEspaco->Valor + Tamanho , CONTROLE  , DIM_CONTROLE ) ;

      /* Inicializar a identificação do código fonte */

         pEspaco->numLinhaFonte = numLinhaFonte ;

         tamNomeArq = strlen( NomeArquivoFonte ) ;
         if ( tamNomeArq >= DIM_NOME_ARQ_FONTE )
         {
            NomeArquivoFonte += ( tamNomeArq - DIM_NOME_ARQ_FONTE + 1 ) ;
         } /* if */
         strcpy( pEspaco->NomeArquivoFonte , NomeArquivoFonte ) ;

      /* Encadear na origem da lista de espaços alocados */

         pEspaco->pAnt      = NULL ;
         pEspaco->pProx     = pOrgLista ;
         if ( pOrgLista != NULL )
         {
            pOrgLista->pAnt = pEspaco ;
         } /* if */
         pOrgLista          = pEspaco ;

         numEspacosAlocados ++ ;
         TotalAlocacoes ++ ;

      return ( void * ) &( pEspaco->Valor ) ;

   } /* Fim função: CED  &Alocar espaço malloc */

/***************************************************************************
*
*  Função: CED  &Alocar espaço todo zerado calloc
*  ****/

   void * CED_Calloc( size_t Tamanho ,
                      int    numLinhaFonte ,
                      char * NomeArquivoFonte )
   {

      char * pAlloc ;

      pAlloc = ( char * ) CED_Malloc( Tamanho , numLinhaFonte ,
                                      NomeArquivoFonte ) ;
      if ( pAlloc != NULL )
      {
         memset( pAlloc , '\0' , Tamanho ) ;
      } /* if */

      return pAlloc ;

   } /* Fim função: CED  &Alocar espaço todo zerado calloc */

/***************************************************************************
*
*  Função: CED  &Realocar espaço alocado realloc
*  ****/

   void * CED_Realloc( size_t Tamanho ,
                       int    numLinhaFonte ,
                       char * NomeArquivoFonte )
   {

      char Msg[ 200 ] ;

      sprintf( Msg ,
               "realloc( %d ) nao esta disponivel em CESPDIN. "
               "Linha: %d  Modulo: %s" ,
               Tamanho , numLinhaFonte , NomeArquivoFonte ) ;

      ExibirMensagem( Msg ) ;
      return NULL ;

   } /* Fim função: CED  &Realocar espaço alocado realloc */

/***************************************************************************
*
*  Função: CED  &Desalocar espaço
*  ****/

   void CED_Free( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      if ( Ponteiro == NULL )
      {
         TST_ExibirPrefixo( TIPO_MSG_INFO , "Desalocar espaco recebeu ponteiro nulo" ) ;
         return ;
      } /* if */

      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         free( Ponteiro ) ;
         return ;
      } /* if */

      DesalocarEspaco( pEspaco ) ;

   } /* Fim função: CED  &Desalocar espaço */

/***************************************************************************
*
*  Função: CED  &Exibir conteúdo do espaço
*  ****/

   void CED_ExibirEspaco( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      if ( Ponteiro == NULL )
      {
         TST_ExibirPrefixo( TIPO_MSG_INFO , "Exibir espaco recebeu ponteiro nulo." ) ;
         return ;
      } /* if */

      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         return ;
      } /* if */

      ExibirCabecalho( TIPO_MSG_INFO , pEspaco ) ;

      TST_ExibirEspacoHexa( pEspaco->tamValor , pEspaco->Valor ) ;

   } /* Fim função: CED  &Exibir conteúdo do espaço */

/***************************************************************************
*
*  Função: CED  &Exibir todos os espaços
*  ****/

   void CED_ExibirTodosEspacos( CED_tpModoExibir Regra )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      pEspaco = pOrgLista ;

      while ( pEspaco != NULL ) {

      /* Tratar modo de exibir */

         switch ( Regra ) {

         /* Exibir todos os espaços */

            case CED_ExibirTodos :
            {

               CED_ExibirEspaco( &( pEspaco->Valor )) ;

               break ;

            } /* fim ativa: Exibir todos os espaços */

         /* Exibir todos os espaços alocados ativos */

            case CED_ExibirAtivos :
            {

               if ( pEspaco->ehAtivo )
               {
                  CED_ExibirEspaco( &( pEspaco->Valor )) ;
               } /* if */

               break ;

            } /* fim ativa: Exibir todos os espaços alocados ativos */

         /* Exibir todos os espaços alocados inativos */

            case CED_ExibirInativos :
            {

               if ( !( pEspaco->ehAtivo ))
               {
                  CED_ExibirEspaco( &( pEspaco->Valor )) ;
               } /* if */

               break ;

            } /* fim ativa: Exibir todos os espaços alocados inativos */

         /* Exibir nada */

            default :
            {

               break ;

            } /* fim ativa: Exibir nada */

         } /* fim seleciona: Tratar modo de exibir */

      /* Avançar para o próximo espaço a exibir */

         pEspaco = pEspaco->pProx ;

      } /* fim repete: Raiz de CED  &Exibir todos os espaços */

   } /* Fim função: CED  &Exibir todos os espaços */

/***************************************************************************
*
*  Função: CED  &Definir tipo do espaço
*  ****/

   int CED_DefinirTipoEspaco( void * Ponteiro , int idTipo )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      if ( Ponteiro == NULL )
      {
         return FALSE ;
      } /* if */

      pEspaco = ObterOrigemElemento( Ponteiro ) ;

      if ( pEspaco == NULL )
      {
         return FALSE ;
      } /* if */

      if ( ( idTipo == CED_ID_TIPO_VALOR_NULO )
        || ( pEspaco->idTipoValor == CED_ID_TIPO_VALOR_NULO ))
      {
         pEspaco->idTipoValor = idTipo ;
         return TRUE ;
      } /* if */

      return FALSE ;

   } /* Fim função: CED  &Definir tipo do espaço */

/***************************************************************************
*
*  Função: CED  &Marcar ativo o espaço
*  ****/

   void CED_MarcarEspacoAtivo( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      if ( Ponteiro == NULL )
      {
         return ;
      } /* if */

      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         return ;
      } /* if */

      pEspaco->ehAtivo = TRUE ;

   } /* Fim função: CED  &Marcar ativo o espaço */

/***************************************************************************
*
*  Função: CED  &Marcar não ativo o espaço
*  ****/

   void CED_MarcarEspacoNaoAtivo( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;
         
      if ( Ponteiro == NULL )
      {
         return ;
      } /* if */
         
      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         return ;
      } /* if */
         
      pEspaco->ehAtivo = FALSE ;

   } /* Fim função: CED  &Marcar não ativo o espaço */

/***************************************************************************
*
*  Função: CED  &Marcar não ativos todos os espaços
*  ****/

   void CED_MarcarTodosEspacosInativos( )
   {

      tpElemListaControle * pEspaco ;
         

      TST_ASSERT( estaInicializado ) ;
         
      pEspaco = pOrgLista ;
      while ( pEspaco != NULL )
      {
         pEspaco->ehAtivo = FALSE ;
         pEspaco = pEspaco->pProx ;
      } /* while */

   } /* Fim função: CED  &Marcar não ativos todos os espaços */

/***************************************************************************
*
*  Função: CED  &Limitar memória disponível
*  ****/

   void CED_LimitarMemoriaDisponivel( long NovoLimiteMemoria )
   {

      TST_ASSERT( estaInicializado ) ;
      TST_ASSERT( NovoLimiteMemoria >= 0 ) ;

      LimiteMemoria = NovoLimiteMemoria ;

   } /* Fim função: CED  &Limitar memória disponível */

/***************************************************************************
*
*  Função: CED  &Limitar número de espaços alocados
*  ****/

   void CED_LimitarNumeroTotalEspacos( long numTotalEspacos )
   {

      TST_ASSERT( estaInicializado ) ;
      TST_ASSERT( numTotalEspacos >= 0 ) ;

      LimiteNumEspacos = numTotalEspacos ;

   } /* Fim função: CED  &Limitar número de espaços alocados */

/***************************************************************************
*
*  Função: CED  &Limitar com freqüência de geração de NULL
*  ****/

   int CED_LimitarFrequencia( int Frequencia )
   {

      TST_ASSERT( estaInicializado ) ;

      if ( ( Frequencia <  LIMITE_INF_FREQUENCIA )
        || ( Frequencia >= LIMITE_SUP_FREQUENCIA ))
      {
         return FALSE ;
      } /* if */

      FrequenciaMalloc = Frequencia ;

      return TRUE ;

   } /* Fim função: CED  &Limitar com freqüência de geração de NULL */

/***************************************************************************
*
*  Função: CED  &Limitar, eliminar limites
*  ****/

   void CED_EliminarLimites( )
   {

      TST_ASSERT( estaInicializado ) ;

      LimiteMemoria        = 0 ;
      LimiteNumEspacos     = 0 ;
      FrequenciaMalloc     = 0 ;

   } /* Fim função: CED  &Limitar, eliminar limites */

/***************************************************************************
*
*  Função: CED  &Obter o tipo do espaço de dados
*  ****/

   int CED_ObterTipoEspaco( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      if ( Ponteiro == NULL )
      {
         return CED_ID_TIPO_NULL ;
      } /* if */

      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         return CED_ID_TIPO_ILEGAL ;
      } /* if */

      return pEspaco->idTipoValor ;

   } /* Fim função: CED  &Obter o tipo do espaço de dados */

/***************************************************************************
*
*  Função: CED  &Obter tamanho do valor contido no espaço
*  ****/

   int CED_ObterTamanhoValor( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;
         
      if ( Ponteiro == NULL )
      {
         return -1 ;
      } /* if */
         
      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         return -1 ;
      } /* if */

      return pEspaco->tamValor ;

   } /* Fim função: CED  &Obter tamanho do valor contido no espaço */

/***************************************************************************
*
*  Função: CED  &Obter tamanho do espaço alocado _msize
*  ****/

   size_t CED_Msize( void * Ponteiro )
   {

      return ((size_t) CED_ObterTamanhoValor( Ponteiro )) ;

   } /* Fim função: CED  &Obter tamanho do espaço alocado _msize */

/***************************************************************************
*
*  Função: CED  &Obter número de espaços alocados
*  ****/

   int CED_ObterNumeroEspacosAlocados( )
   {

      TST_ASSERT( estaInicializado ) ;

      return numEspacosAlocados ;

   } /* Fim função: CED  &Obter número de espaços alocados */

/***************************************************************************
*
*  Função: CED  &Obter total de espaços alocados
*  ****/

   int CED_ObterTotalAlocacoes( )
   {

      TST_ASSERT( estaInicializado ) ;

      return TotalAlocacoes ;

   } /* Fim função: CED  &Obter total de espaços alocados */

/***************************************************************************
*
*  Função: CED  &Obter número total de NULLs gerados
*  ****/

   long CED_ObterNumNULL( void )
   {

      TST_ASSERT( estaInicializado ) ;

      return ContaNULL ;

   } /* Fim função: CED  &Obter número total de NULLs gerados */

/***************************************************************************
*
*  Função: CED  &Obter número de espaços segundo regra
*  ****/

   int CED_ObterNumeroEspacos( CED_tpModoExibir Regra )
   {

      tpElemListaControle * pEspaco ;
      int Conta = 0 ;

      TST_ASSERT( estaInicializado ) ;
               
      pEspaco = pOrgLista ;

      while ( pEspaco != NULL )
      {
         switch ( Regra )
         {
            case CED_ExibirTodos :
               Conta ++ ;
               break ;

            case CED_ExibirAtivos :
               if ( pEspaco->ehAtivo )
               {
                  Conta ++ ;
               } /* if */
               break ;

            case CED_ExibirInativos :
               if ( !( pEspaco->ehAtivo ))
               {
                  Conta ++ ;
               } /* if */
               break ;

            default:
               break ;

         } /* fim switch */

         pEspaco = pEspaco->pProx ;
      } /* while */

      return Conta ;

   } /* Fim função: CED  &Obter número de espaços segundo regra */

/***************************************************************************
*
*  Função: CED  &Obter espaço total alocado
*  ****/

   long CDE_ObterEspacoTotalAlocado( )
   {

      TST_ASSERT( estaInicializado ) ;

      return EspacoTotalAlocado ;

   } /* Fim função: CED  &Obter espaço total alocado */

/***************************************************************************
*
*  Função: CED  &Iterador: iniciar iterador de espaços
*  ****/

   void CED_InicializarIteradorEspacos( )
   {

      TST_ASSERT( estaInicializado ) ;

      pEspacoCorr = pOrgLista ;

   } /* Fim função: CED  &Iterador: iniciar iterador de espaços */

/***************************************************************************
*
*  Função: CED  &Iterador: avançar para o próximo espaço
*  ****/

   int CED_AvancarProximoEspaco( )
   {

      TST_ASSERT( estaInicializado ) ;

      if ( pEspacoCorr == NULL )
      {
         return FALSE ;
      } /* if */

      pEspacoCorr = pEspacoCorr->pProx ;

      if ( pEspacoCorr != NULL )
      {
         return TRUE ;
      } else {
         return FALSE ;
      } /* if */

   } /* Fim função: CED  &Iterador: avançar para o próximo espaço */

/***************************************************************************
*
*  Função: CED  &Iterador: obter referência para o espaço corrente
*  ****/

   void * CED_ObterPonteiroEspacoCorrente( )
   {

      TST_ASSERT( estaInicializado ) ;

      if ( pEspacoCorr == NULL )
      {
         return NULL ;
      } /* if */

      return ( void * ) &( pEspacoCorr->Valor ) ;

   } /* Fim função: CED  &Iterador: obter referência para o espaço corrente */

/***************************************************************************
*
*  Função: CED  &Iterador: existe espaço corrente
*  ****/

   int CED_ExisteEspacoCorrente( )
   {

      TST_ASSERT( estaInicializado ) ;

      if ( pEspacoCorr == NULL )
      {
         return FALSE ;
      } /* if */

      return TRUE ;

   } /* Fim função: CED  &Iterador: existe espaço corrente */

/***************************************************************************
*
*  Função: CED  &Iterador: terminar iterador
*  ****/

   void CED_TerminarIteradorEspacos( )
   {

      pEspacoCorr = NULL ;

   } /* Fim função: CED  &Iterador: terminar iterador */

/***************************************************************************
*
*  Função: CED  &Iterador: excluir espaço corrente
*  ****/

   void CED_ExcluirEspacoCorrente( )
   {

      tpElemListaControle * pDesaloca ;

      TST_ASSERT( estaInicializado ) ;

      if ( pEspacoCorr != NULL )
      {
         pDesaloca   = pEspacoCorr ;
         pEspacoCorr = pEspacoCorr->pProx ;
         DesalocarEspaco( pDesaloca ) ;
      } /* if */

   } /* Fim função: CED  &Iterador: excluir espaço corrente */

/***************************************************************************
*
*  Função: CED  &Verificar se espaço é ativo
*  ****/

   int CED_EhEspacoAtivo( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      if ( Ponteiro == NULL )
      {
         return FALSE ;
      } /* if */

      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         return FALSE ;
      } /* if */

      return pEspaco->ehAtivo ;

   } /* Fim função: CED  &Verificar se espaço é ativo */

/***************************************************************************
*
*  Função: CED  &Verificar a integridade de um espaço de dados
*  ****/

   int CED_VerificarEspaco( void * Ponteiro ,
                            int ( * pVerificarValor )( void * pValor ))
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      /* Verificar as referências para o espaço */

         if ( Ponteiro == NULL )
         {
            return TRUE ;
         } /* if */

         pEspaco = ObterOrigemElemento( Ponteiro ) ;

         if ( pEspaco == NULL )
         {
            return FALSE ;
         } /* if */

      /* Verificar o conteúdo do espaço */

         if ( pVerificarValor != NULL )
         {
            if ( pVerificarValor( pEspaco->Valor ) == 0 )
            {
               ExibirMensagem( "Valor contido no espaco esta incorreto." ) ;
               ExibirCabecalho( TIPO_MSG_VAZIA , pEspaco ) ;
               fprintf( TST_ObterArqLog( ) , "\n" ) ;
               return FALSE ;
            } /* if */
         } /* if */

      return TRUE ;

   } /* Fim função: CED  &Verificar a integridade de um espaço de dados */

/***************************************************************************
*
*  Função: CED  &Verificar todos os espaços alocados
*  ****/

   int CED_VerificarTudo( int ( * pVerificarValor )( void * pValor ))
   {

      tpElemListaControle * pEspaco ;

      int ContaErro   = 0 ,
          ContaEspaco = 0 ;

      TST_ASSERT( estaInicializado ) ;

      /* Verificar toda a lista de espaços */

         pEspaco = pOrgLista ;
         while ( pEspaco != NULL )
         {
            ContaEspaco ++ ;
            if ( ! CED_VerificarEspaco( pEspaco->Valor , pVerificarValor ))
            {
               ContaErro ++ ;
            } /* if */
            pEspaco = pEspaco->pProx ;
         } /* while */

      /* Verificar número de espaços alocados */

         if ( ContaEspaco != numEspacosAlocados )
         {
            ExibirMensagem( "Número de espaços alocados errado." ) ;
            ContaErro ++ ;
         } /* if */

         if ( numEspacosAlocados > TotalAlocacoes )
         {
            ExibirMensagem( "Número de espaços alocados maior do que total alocado." ) ;
            ContaErro ++ ;
         } /* if */

      return ContaErro == 0 ;

   } /* Fim função: CED  &Verificar todos os espaços alocados */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: CED  -Desalocar espaço dado
*
***********************************************************************/

   void DesalocarEspaco( tpElemListaControle * pEspaco )
   {

      /* Desalocar o espaço apontado */

         if ( pEspaco->pProx != NULL )
         {
            pEspaco->pProx->pAnt  = pEspaco->pAnt ;
         } /* if */

         if ( pEspaco->pAnt != NULL )
         {
            pEspaco->pAnt->pProx = pEspaco->pProx ;
         } else {
            pOrgLista = pEspaco->pProx ;
         } /* if */

         numEspacosAlocados -- ;
         EspacoTotalAlocado -= pEspaco->tamValor ;

      /* Limpar o espaço a liberar */

         memset( pEspaco->ControleAntes , CHAR_DESALOC ,
                 pEspaco->tamValor + DIM_CONTROLE ) ;
         * (( unsigned long * ) ( pEspaco->Valor )) = 0 ;
         memcpy( pEspaco->ControleAntes + pEspaco->tamValor + DIM_CONTROLE ,
                 CONTROLE_ERRO , DIM_CONTROLE ) ;

         pEspaco->idTipoValor = CED_ID_TIPO_ILEGAL ;
         free( pEspaco ) ;

   } /* Fim função: CED  -Desalocar espaço dado */


/***********************************************************************
*
*  $FC Função: CED  -Obter ponteiro para o elemento da lista de espaços ativos
*
*  $ED Descrição da função
*     Retorna o ponteiro para o início do elemento da lista de espaços
*     alocados, no entanto sem controlar a integridade do espáco.
*
*  $EP Parâmetros
*     $P Ponteiro - aponta para o campo Valor do espaço
*
*  $FV Valor retornado
*     Se o espaço estiver válido, retorna o ponteiro para ele,
*     caso contrário retorna NULL.
*
***********************************************************************/

   tpElemListaControle * ObterOrigemElemento( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( Ponteiro != NULL ) ;

      pEspaco = ( tpElemListaControle * )
                  ((( char * ) Ponteiro ) - ( sizeof( tpElemListaControle ) -
                     DIM_VALOR - DIM_CONTROLE )) ;

      if ( pEspaco->ImpressaoDigital != IMPRESSAO_DIGITAL )
      {
         return NULL ;
      } /* if */

      if ( VerificarEspaco( pEspaco ))
      {
         ExibirMensagem(  "Espaco de dados esta corrompido." ) ;

         fprintf( TST_ObterArqLog( ) , " Id: %d  pValor: %p" ,
                  pEspaco->idEspaco , pEspaco->Valor ) ;

         ExibirCabecalho( TIPO_MSG_VAZIA , pEspaco ) ;

         fprintf( TST_ObterArqLog( ) , "\n" ) ;

         return NULL ;
      } /* if */

      return pEspaco ;

   } /* Fim função: CED  -Obter ponteiro para o elemento da lista de espaços ativos */


/***********************************************************************
*
*  $FC Função: CED  -Exibir cabecalho do espaco
*
*  $ED Descrição da função
*     Exibe todos os atributos, exceto o valor, contido no espaço.
*
*  $EP Parâmetros
*     $P TipoMsg - string (3 caracteres) indicando o significado do texto
*                  exibido na saída
*     $P pEspaco - ponteiro para a origem do espaco (não para o valor)
*
***********************************************************************/

   void ExibirCabecalho( char * TipoMsg , tpElemListaControle * pEspaco )
   {

      FILE * pArqLog ;

      int i ;

      char * pVal  ;
      char * pChar ;

      pArqLog = TST_ObterArqLog( ) ;

      fprintf( pArqLog , "\n\n%s  Espaco de dados, id: %5d" ,
                  TipoMsg , pEspaco->idEspaco ) ;

      /* Exibir ponteiro do espaço */

         pVal  = pEspaco->Valor ;

         fprintf( pArqLog , "  pValor: %p  hexa:" , pVal ) ;

         pChar = ( char * ) &pVal ;
         for( i = 0 ; i < DIM_PONTEIRO ; i++ )
         {
            fprintf( pArqLog , " %02X" , pChar[ i ] ) ;
         } /* for */

      /* Exibir identificação do espaço antecessor */

         if ( pEspaco->pAnt != NULL  )
         {
            fprintf( pArqLog , "  Id ant: %5d" , pEspaco->pAnt->idEspaco ) ;
         } else {
            fprintf( pArqLog , "  E' origem lista" ) ;
         } /* if */

      /* Exibir identificação do espaço sucessor */

         if ( pEspaco->pProx != NULL )
         {
            fprintf( pArqLog , "  Id suc: %5d" , pEspaco->pProx->idEspaco ) ;
         } else {
            fprintf( pArqLog , "  E' final lista" ) ;
         } /* if */

      /* Exibir o identificador do tipo do valor */

         if ( pEspaco->idTipoValor == CED_ID_TIPO_VALOR_NULO )
         {
            fprintf( pArqLog , "\n     Tipo indefinido" ) ;
         } else if ( pEspaco->idTipoValor == CED_ID_TIPO_ILEGAL )
         {
            fprintf( pArqLog , "\n     Tipo ilegal" ) ;
         } else
         {
            fprintf( pArqLog , "\n     Id tipo valor: %d" , pEspaco->idTipoValor ) ;
         } /* if */

      /* Tamanho útil do valor alocado */

         fprintf( pArqLog , "  Tamanho: %d" , pEspaco->tamValor ) ;

      /* Exibir flag ativo */

         if ( pEspaco->ehAtivo )
         {
            fprintf( pArqLog , "  Ativo" ) ;
         } else {
            fprintf( pArqLog , "  Nao ativo" ) ;
         } /* if */

      /* Exibir número da linha de código fonte onde foi alocado */

         fprintf( pArqLog , "  Linha onde alocou: %d" , pEspaco->numLinhaFonte ) ;

      /* Exibir nome do arquivo onde alocou */

         fprintf( pArqLog , "  Arquivo: %s" , pEspaco->NomeArquivoFonte ) ;

      /* Exibir controle antes */

         if ( memcmp( pEspaco->ControleAntes , CONTROLE , DIM_CONTROLE ) == 0 )
         {
            fprintf( pArqLog , "\n     Controle antes OK" ) ;
         } else {
            fprintf( pArqLog , "\n     Controle antes errado:" ) ;
            for( i = 0 ; i < DIM_CONTROLE ; i++ )
            {
               fprintf( pArqLog , " %02X" , pEspaco->ControleAntes[ i ] ) ;
            } /* for */
         } /* if */

      /* Exibir controle após */

         pChar = pEspaco->ControleApos - DIM_VALOR + pEspaco->tamValor ;
         if ( memcmp( pChar , CONTROLE , DIM_CONTROLE ) == 0 )
         {
            fprintf( pArqLog , "  Controle apos OK" ) ;
         } else {
            fprintf( pArqLog , "  Controle apos errado:" ) ;
            for( i = 0 ; i < DIM_CONTROLE ; i++ )
            {
               fprintf( pArqLog , " %02X" , pChar[ i ] ) ;
            } /* for */
         } /* if */

   } /* Fim função: CED  -Exibir cabecalho do espaco */


/***********************************************************************
*
*  $FC Função: CED  -Exibir mensagem de erro
*
*  $ED Descrição da função
*     Exibe uma mensagem de erro ou advertência no formato padrão
*
*  $EP Parâmetros
*     $P Msg  - mensagem a exibir. O string não deve conter controles de
*               avanço de linha
*
***********************************************************************/

   void ExibirMensagem( char * Msg )
   {

      TST_ContarFalhas( ) ;
      TST_ExibirPrefixo( TIPO_MSG_ERRO , Msg ) ;

   } /* Fim função: CED  -Exibir mensagem de erro */


/***********************************************************************
*
*  $FC Função: CED  -Verificar integridade de determinado espaço
*
*  $ED Descrição da função
*     Controla a integridade após conhecer o ponteiro para a origem
*     do espaço. Não controla o campo valor.
*
*  $EP Parâmetros
*     $P pEspaco - aponta a origem do espaço alocado
*
***********************************************************************/

   int VerificarEspaco( tpElemListaControle * pEspaco )
   {

      int ContaErro = 0 ;

      /* Verificar o marcador de espaco ainda não desalocado */

         if ( pEspaco->EspacoNULL != ESPACO_NULL )
         {
            ContaErro ++ ;
            if ( pEspaco->EspacoNULL == 0 )
            {
               ExibirMensagem( "Espaco ja\' foi desalocado: " ) ;
            } else
            {
               ExibirMensagem( "Controle inicial corrompido: " ) ;
            } /* if */
            return ContaErro ;
         } /* if */

      /* Verificar id espaço */

         if ( ( pEspaco->idEspaco   < 0 )
           || ( pEspaco->idEspaco   > TotalAlocacoes )
           || ( numEspacosAlocados  > TotalAlocacoes ))
         {
            ContaErro ++ ;
            ExibirMensagem( "Identificação do espaço está errada." ) ;
         } /* if */

      /* Verificar encadeamento origem da lista */

         if ( pOrgLista != NULL )
         {
            if ( pOrgLista->pAnt != NULL )
            {
               ContaErro ++ ;
               ExibirMensagem( "Origem da lista de espaços está errada." ) ;
            } /* if */
         } else {
            ContaErro ++ ;
            ExibirMensagem( "Origem da lista não vazia é NULL." ) ;
            pOrgLista = pEspaco ;
         } /* if */

      /* Verificar encadeamento antes */

         if ( pEspaco != pOrgLista )
         {
            if ( pEspaco->pAnt != NULL )
            {
               if ( pEspaco->pAnt->pProx != pEspaco )
               {
                  ContaErro ++ ;
                  ExibirMensagem( "Encadeamento antes está errado." ) ;
                  pEspaco->pAnt = NULL ;
               } /* if */
            } else {
               ContaErro ++ ;
               ExibirMensagem( "Espáco anterior == NULL, mas não é a origem da lista de espaços." ) ;
            } /* if */
         } else
         {
            if ( pEspaco->pAnt != NULL )
            {
               ContaErro ++ ;
               ExibirMensagem( "Espaço origem da lista está errada." ) ;
               pEspaco->pAnt = NULL ;
            } /* if */
         } /* if */

      /* Verificar encadeamento após */

         if ( pEspaco->pProx != NULL )
         {
            if ( pEspaco->pProx->pAnt != pEspaco )
            {
               ContaErro ++ ;
               ExibirMensagem( "Encadeamento após está errado." ) ;
               pEspaco->pProx = NULL ;
            } /* if */
         } /* if */

      /* Verificar tamanho */

         if ( ( pEspaco->tamTotal != pEspaco->tamValor +
                          ( int ) sizeof( tpElemListaControle ))
           || ( pEspaco->tamValor <= 0 ))
         {
            ContaErro ++ ;
            ExibirMensagem( "Tamanho do espaço está errado." ) ;
         } /* if */

      /* Verificar controle antes */

         if ( memcmp( pEspaco->ControleAntes , CONTROLE , DIM_CONTROLE ) != 0 )
         {
            ContaErro ++ ;
            ExibirMensagem( "Controle de extravasão antes está errado." ) ;
         } /* if */

      /* Verificar controle após */

         if ( memcmp( pEspaco->ControleApos - DIM_VALOR + pEspaco->tamValor ,
                      CONTROLE , DIM_CONTROLE ) != 0 )
         {
            ContaErro ++ ;
            ExibirMensagem( "Controle de extravasão após está errado." ) ;
         } /* if */

      return ContaErro ;

   } /* Fim função: CED  -Verificar integridade de determinado espaço */

/********** Fim do módulo de implementação: CED  Controlador de espaços de dados alocados dinamicamente **********/

