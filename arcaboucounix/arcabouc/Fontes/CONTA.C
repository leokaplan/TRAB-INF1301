/***************************************************************************
*  $MCI Módulo de implementação: CNT  Contadores de passagem
*
*  Arquivo gerado:              CONTA.c
*  Letras identificadoras:      CNT
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
#include    <time.h>
#include    <malloc.h>

#include "GENERICO.H"
#include "TABSIMB.H"

#ifdef _DEBUG
   #include "CESPDIN.H"
#endif

#define CONTA_OWN
#include "CONTA.H"
#undef CONTA_OWN

#define   ARQUIVO_ACUMULADOR      1
#define   ARQUIVO_DEFINICAO       2

#define   FALSE                   0
#define   TRUE                    1

#define   DIM_NOME_CONTADOR       100
#define   STR_DIM_NOME_CONTADOR   "98"
#define   DIM_NOME_ARQUIVO        120
#define   DIM_BUFFER              150
#define   DIM_TABELA              317

#define   TRIM_CHAR               " \n\r\t\x1A"

#define   ERRO_CONTADOR           "cc>"

#define   EXT_ACUMULADOR          "count"
#define   EXT_DEFINICAO           "count"

#define   COMENTARIO              "//"
#define   OP_ATRIBUICAO           '='

const char ContadorNaoInicializado[ ] = "Módulo contadores ainda não foi inicializado." ;

/***********************************************************************
*
*  $TC Tipo de dados: CNT Descritor de contador
*
*
*  $ED Descrição do tipo
*     Cada contador é identificado por um nome.
*     O acesso ao nome se dará atrabés de uma tabela de símbolos.
*     Além disso será mantida uma lista linear de contadores, ordenada
*     em ordem crescente pelo nome do contador.
*
***********************************************************************/

   typedef struct tgContador {

         struct tgContador * pProxContador ;
               /* Próximo contador na lista ordenada de contadores */

         long Contagem ;
               /* Valor de contagem do contador
               *
               *$ED Descrição
               *
               *   Conta o número de vezes que a função CNT_Contar
               *   foi ativada com relação ao contador */

         char NomeContador[ DIM_NOME_CONTADOR ] ;
               /* Nome do contador, string C padrão */

         int Fonte ;
               /* Fonte do nome
               *
               *$ED Descrição
               *   O atributo contém "ARQUIVO_ACUMULADOR" se o contador
               *   tiver sido lido de um arquivo acumulador ou
               *   "ARQUIVO_DEFINICAO" se tiver sido lido de um arquivo
               *   de definicao. Se ao ler de um arquivo de definicao
               *   o contador estiver marcado "ARQUIVO_ACUMULADOR",
               *   o indicador é mudado para "ARQUIVO_DEFINICAO".
               *   Se já era "ARQUIVO_DEFINICAO", é emitida uma mensagem
               *   de erro de duplicidade de definição. */

   } tpContador ;

/*****  Dados encapsulados no módulo  *****/

      static int Inicializado = FALSE ;
            /* Controle de inicialização de contadores
               *
               *$ED Descrição
               *   Controla se o sistema de contagem foi ou não iniciado */

      static int EfetuaContagem = FALSE ;
            /* Controle de contagem
               *
               *$ED Descrição
               *   Contagem é realizada somente se EfetuaContagem == TRUE */

      static TBS_tppTabela pTabela = NULL ;
            /* Ponteiro para a tabela de símbolos em uso */

      static tpContador * pContadorCorr = NULL ;
            /* Ponteiro para contador corrente do iterador */

      static tpContador * pOrgListaContadores = NULL ;
            /* Origem da lista ordenada de contadores */

      static int numContadores = 0 ;
            /* Número total de contadores */

      static int numTotalErros = 0 ;
            /* Número total de erros encontrados até o momento */

      static long ContagemTotal = 0 ;
            /* Contagem total
               *
               *$ED Descrição
               *     Contém a somatória de todas as contagens */

      static char NomeArquivoAcumulador[ DIM_NOME_ARQUIVO ] ;
            /* Nome do arquivo acumulador de contadores */

      static int    numLinha = 0 ;
            /* Número da linha do arquivo sendo lido */

      static char   BufferLeitura[ DIM_BUFFER ] ;
            /* Buffer de leitura de contadores */

/***** Protótipos das funções encapuladas no módulo *****/

   static char * ObterNomeContador( void * pDado ) ;

   static void MontarNomeArquivo( char * NomeArquivo ,
                                  char * NomeExtensao ) ;

   static int LerLinha( FILE * pArquivo ) ;

   static int LerContadores( char * NomeArquivo , int Fonte ) ;

   static void ExibirPrefixo( int numLinha ) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: CNT  &Inicializar contadores
*  ****/

   CNT_tpCondRet CNT_InicializarContadores( char * NomeArquivo )
   {

      if ( Inicializado == TRUE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , "Contadores já foram inicializados."  ) ;
         return CNT_CondRetInicializado ;
      } /* if */

      /* Inicializar todas as constantes globais do contador */

         EfetuaContagem       = FALSE ;
         pOrgListaContadores  = NULL ;
         pContadorCorr        = NULL ;
         numContadores        = 0 ;
         ContagemTotal        = 0 ;
         numLinha             = -1 ;
         memset( BufferLeitura , '?' , DIM_BUFFER ) ;
         BufferLeitura[ DIM_BUFFER - 1 ] = 0 ;

      /* Criar tabela de simbolos dos contadores */

         pTabela = TBS_CriarTabela( DIM_TABELA , ObterNomeContador , NULL ) ;

         TST_ASSERT( pTabela != NULL ) ;

      /* Registrar o arquivo de contagem acumulado */

         Inicializado = TRUE ;

         if ( NomeArquivo == NULL )
         {
            NomeArquivoAcumulador[ 0 ] = 0 ;
         } else {
            if ( numContadores != 0 )
            {
               ExibirPrefixo( -1 ) ;
               fprintf( TST_ObterArqLog( ) , "\nError: O arquivo acumulador deve ser lido antes de todos os outros."  ) ;
               return CNT_CondRetErro ;
            } else
            {
               strcpy( NomeArquivoAcumulador , NomeArquivo ) ;
               MontarNomeArquivo( NomeArquivoAcumulador , EXT_ACUMULADOR ) ;
               if ( LerContadores( NomeArquivoAcumulador , ARQUIVO_ACUMULADOR ) != 0 )
               {
                  return CNT_CondRetErro ;
               } /* if */
            } /* if */
         } /* if */
         return CNT_CondRetOK ;

   } /* Fim função: CNT  &Inicializar contadores */

/***************************************************************************
*
*  Função: CNT  &Terminar contadores
*  ****/

   CNT_tpCondRet CNT_TerminarContadores( )
   {

      int numErros = 0 ;

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return CNT_CondRetErro ;
      } /* if */

      /* Gravar a contagem acumulada */

         if ( NomeArquivoAcumulador[ 0 ] != 0 )
         {
            numErros = CNT_GravarContadores( NomeArquivoAcumulador ) ;
         } /* if */

      /* Inicializar as variáveis de controle e a tabela */

         TBS_DestruirTabela( pTabela ) ;
         pTabela = NULL ;

         Inicializado = FALSE ;

         if ( numErros != 0 )
         {
            return CNT_CondRetErro ;
         } /* if */

         return CNT_CondRetOK ;

   } /* Fim função: CNT  &Terminar contadores */

/***************************************************************************
*
*  Função: CNT  &Iniciar a contagem
*  ****/

   void CNT_IniciarContagem( )
   {

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , "Contadores ainda não foram inicializados."  ) ;
         return ;
      } /* if */

      EfetuaContagem = TRUE ;

   } /* Fim função: CNT  &Iniciar a contagem */

/***************************************************************************
*
*  Função: CNT  &Parar contagem
*  ****/

   void CNT_PararContagem( )
   {

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return ;
      } /* if */

      EfetuaContagem = FALSE ;

   } /* Fim função: CNT  &Parar contagem */

/***************************************************************************
*
*  Função: CNT  &Registrar arquivo acumulador
*  ****/

   void CNT_RegistrarAcumulador( char * NomeArquivo )
   {

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return ;
      } /* if */

      if ( NomeArquivo == NULL )
      {
         NomeArquivoAcumulador[ 0 ] = 0 ;
      } else {
         strcpy( NomeArquivoAcumulador , NomeArquivo ) ;
         MontarNomeArquivo( NomeArquivoAcumulador , EXT_ACUMULADOR ) ;
      } /* if */

   } /* Fim função: CNT  &Registrar arquivo acumulador */

/***************************************************************************
*
*  Função: CNT  &Ler arquivo de definição de contadores
*  ****/

   int CNT_LerContadores( char * NomeArquivoDefinicao )
   {

      char NomeArquivo[ DIM_NOME_ARQUIVO ] ;

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return 1 ;
      } /* if */

      strcpy( NomeArquivo , NomeArquivoDefinicao ) ;

      MontarNomeArquivo( NomeArquivo , EXT_DEFINICAO ) ;

      return LerContadores( NomeArquivo , ARQUIVO_DEFINICAO ) ;

   } /* Fim função: CNT  &Ler arquivo de definição de contadores */

/***************************************************************************
*
*  Função: CNT  &Gravar arquivo de contagem acumulada
*  ****/

   int CNT_GravarContadores( char * NomeArquivo )
   {

      FILE * pArq ;

      char CharTime[ 25 ] ;

      time_t StartTime ;
      struct tm * pTime ;

      tpContador * pContador ;

      /* Abrir arquivo acumulador para gravação */

         pArq = fopen( NomeArquivo , "w" ) ;
         if ( pArq == NULL )
         {
            ExibirPrefixo( -1 ) ;
            fprintf( TST_ObterArqLog( ) , "Não abriu o arquivo de acumuladores %s" ,
                     NomeArquivo ) ;
            return 1 ;
         } /* if */

      /* Gerar arquivo acumulador */

         /* Gravar cabeçalho de arquivo de acumuladores */

            time( & StartTime ) ;
            pTime = localtime( & StartTime ) ;

            sprintf( CharTime , "%04i/%02i/%02i-%02i:%02i:%02i" ,
                     pTime->tm_year + 1900 , pTime->tm_mon + 1 ,
                     pTime->tm_mday        , pTime->tm_hour ,
                     pTime->tm_min         , pTime->tm_sec ) ;

            fprintf( pArq ,   "//////////////////////////////////////////////////////////////////////" ) ;
            fprintf( pArq , "\n// Contadores acumulados, gerado: %s" , CharTime ) ;
            fprintf( pArq , "\n//////////////////////////////////////////////////////////////////////" ) ;
            fprintf( pArq , "\n" ) ;

         /* Gravar todos contadores e respectivas contagens */

            pContador = pOrgListaContadores ;
            while ( pContador != NULL )
            {
               fprintf( pArq , "\n%s\\%c%ld" , pContador->NomeContador ,
                        OP_ATRIBUICAO , pContador->Contagem ) ;
               pContador = pContador->pProxContador ;
            } /* while */

         /* Gravar final de arquivo acumulador */

            fprintf( pArq , "\n" ) ;
            fprintf( pArq , "\n//////////////////////////////////////////////////////////////////////" ) ;
            fprintf( pArq , "\n// Fim arquivo de contadores acumulados" ) ;
            fprintf( pArq , "\n//////////////////////////////////////////////////////////////////////" ) ;
            fprintf( pArq , "\n" ) ;

      /* Fechar arquivo acumulador */

         fclose( pArq ) ;

         return 0 ;

   } /* Fim função: CNT  &Gravar arquivo de contagem acumulada */

/***************************************************************************
*
*  Função: CNT  &Contar
*  ****/

   CNT_tpCondRet CNT_Contar( char * NomeContador , int numLinha )
   {

      tpContador * pContador ;

      #ifdef _DEBUG
         TST_ASSERT( NomeContador != NULL ) ;
      #endif

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , "Contadores ainda não foram inicializados."  ) ;
         return CNT_CondRetNaoInicializado ;
      } /* if */

      if ( EfetuaContagem )
      {
         pContador = ( tpContador * ) TBS_ProcurarSimbolo( pTabela, NomeContador ) ;
         if ( pContador != NULL )
         {
            if ( pContador->Contagem == -1 )
            {
               pContador->Contagem = 1 ;
            } else if ( pContador->Contagem >= 0 )
            {
               pContador->Contagem ++ ;
               ContagemTotal ++ ;
            } else if ( pContador->Contagem == -2 )
            {
               ExibirPrefixo( numLinha ) ;
               fprintf( TST_ObterArqLog( ) , "Contador \"%s\" não deve ser contado." ,
                        NomeContador ) ;
               return CNT_CondRetProibido ;
            } else
            {
               ExibirPrefixo( numLinha ) ;
               fprintf( TST_ObterArqLog( ) , "Contador \"%s\" contém valor ilegal: %d." ,
                        NomeContador , pContador->Contagem ) ;
               return CNT_CondRetNaoContador ;
            } /* if */
         } else
         {
            ExibirPrefixo( numLinha ) ;
            fprintf( TST_ObterArqLog( ) , "Contador \"%s\" não existe." ,
                     NomeContador ) ;
            return CNT_CondRetNaoContador ;
         } /* if */
      } /* if */

      return CNT_CondRetOK ;

   } /* Fim função: CNT  &Contar */

/***************************************************************************
*
*  Função: CNT  &Verificar contagem
*  ****/

   int CNT_VerificarContagem( )
   {

      tpContador * pContador ;
      int numErros = 0 ;

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return -1 ;
      } /* if */

      pContador = pOrgListaContadores ;
      while ( pContador != NULL )
      {
         if ( pContador->Contagem == 0 )
         {
            numErros ++ ;
            ExibirPrefixo( 0 ) ;
            fprintf( TST_ObterArqLog( ) , "Contagem em \"%s\" é zero." ,
                     pContador->NomeContador ) ;
         } /* if */
         pContador = pContador->pProxContador ;
      } /* while */

      return numErros ;

   } /* Fim função: CNT  &Verificar contagem */

/***************************************************************************
*
*  Função: CNT  &Obter valor de contagem
*  ****/

   long CNT_ObterContagem( char * NomeContador )
   {

      tpContador * pContador ;

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return -1 ;
      } /* if */

      pContador = ( tpContador * ) TBS_ProcurarSimbolo( pTabela , NomeContador ) ;
      if ( pContador == NULL )
      {
         return CNT_CondRetNaoContador ;
      } /* if */

      return pContador->Contagem ;

   } /* Fim função: CNT  &Obter valor de contagem */

/***************************************************************************
*
*  Função: CNT  &Obter número de contadores
*  ****/

   int CNT_ObterNumeroContadores( )
   {

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return -1 ;
      } /* if */

      return numContadores ;

   } /* Fim função: CNT  &Obter número de contadores */

/***************************************************************************
*
*  Função: CNT  &Obter total de contagem
*  ****/

   long CNT_ObterContagemTotal( )
   {

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return -1 ;
      } /* if */

      return ContagemTotal ;

   } /* Fim função: CNT  &Obter total de contagem */

/***************************************************************************
*
*  Função: CNT  &Iterador: obter contagem corrente
*  ****/

   long CNT_ObterContagemCorrente( )
   {

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return -1 ;
      } /* if */

      if ( pContadorCorr != NULL )
      {
         return pContadorCorr->Contagem ;
      } /* if */

      return CNT_CondRetNaoIterador ;

   } /* Fim função: CNT  &Iterador: obter contagem corrente */

/***************************************************************************
*
*  Função: CNT  &Iterator: obter nome de contador corrente
*  ****/

   char * CNT_ObterContadorCorrente( )
   {

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return NULL ;
      } /* if */

      if ( pContadorCorr != NULL )
      {
         return pContadorCorr->NomeContador ;
      } /* if */

      return NULL ;

   } /* Fim função: CNT  &Iterator: obter nome de contador corrente */

/***************************************************************************
*
*  Função: CNT  &Iterador: avançar para o próximo contador
*  ****/

   void CNT_IrProximoContador( )
   {

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return ;
      } /* if */

      if ( pContadorCorr != NULL )
      {
         pContadorCorr = pContadorCorr->pProxContador ;
      } /* if */

   } /* Fim função: CNT  &Iterador: avançar para o próximo contador */

/***************************************************************************
*
*  Função: CNT  &Iterator: iniciar percorrimento da lista em ordem alfabética
*  ****/

   void CNT_IniciarIterador( )
   {

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return ;
      } /* if */

      pContadorCorr = pOrgListaContadores ;

   } /* Fim função: CNT  &Iterator: iniciar percorrimento da lista em ordem alfabética */

/***************************************************************************
*
*  Função: CNT  &Iterator: verificar se o iterador está ativo
*  ****/

   int CNT_EhAtivoIterador( )
   {

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return FALSE ;
      } /* if */

      if ( pContadorCorr != NULL )
      {
         return TRUE ;
      } /* if */
      return FALSE ;

   } /* Fim função: CNT  &Iterator: verificar se o iterador está ativo */

/***************************************************************************
*
*  Função: CNT  &Zerar todos contadores
*  ****/

   CNT_tpCondRet CNT_ZerarContadores( )
   {

      tpContador * pContadorCorr ;

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return CNT_CondRetNaoInicializado ;
      } /* if */

      for( pContadorCorr  = pOrgListaContadores ;
           pContadorCorr != NULL ;
           pContadorCorr  = pContadorCorr->pProxContador )
      {
         if ( pContadorCorr->Contagem > 0 )
         {
            pContadorCorr->Contagem = 0 ;
         } /* if */
      } /* for */

      return CNT_CondRetOK ;

   } /* Fim função: CNT  &Zerar todos contadores */

/***************************************************************************
*
*  Função: CNT  &Zerar contador dado
*  ****/

   CNT_tpCondRet CNT_ZerarContador( char * NomeContador )
   {

      tpContador * pContador ;

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return CNT_CondRetNaoInicializado ;
      } /* if */

      pContador = ( tpContador * ) TBS_ProcurarSimbolo( pTabela , NomeContador ) ;

      if ( pContador != NULL )
      {
         if ( pContador->Contagem > 0 )
         {
            pContador->Contagem = 0 ;
         } /* if */
      } else
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , "Contador \"%s\" não existe." ,
                   NomeContador ) ;
         return CNT_CondRetNaoContador ;

      } /* if */

      return CNT_CondRetOK ;

   } /* Fim função: CNT  &Zerar contador dado */

#ifdef _DEBUG

/***************************************************************************
*
*  Função: CNT  &Verificar a estrutura dos contadores
*  ****/

   int CNT_VerificarEstruturaContadores( )
   {

      tpContador * pContador ;
      tpContador * pContadorAnt ;

      int numErros    = 0 ;
      int numSimbolos = 0 ;

      if ( Inicializado == FALSE )
      {
         ExibirPrefixo( -1 ) ;
         fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
         return 1 ;
      } /* if */

      /* Verificar se contadores está inicializado */

         if ( Inicializado == FALSE )
         {
            ExibirPrefixo( -1 ) ;
            fprintf( TST_ObterArqLog( ) , ContadorNaoInicializado ) ;
            return 1 ;
         } /* if */

      /* Verificar tabela de simbolos contadores */

         if ( TBS_ValidarTabela( pTabela ) != TBS_CondRetOK )
         {
            numErros ++ ;
            ExibirPrefixo( -1 ) ;
            fprintf( TST_ObterArqLog( ) , "Tabela de símbolos (contadores) em erro." ) ;
         } /* if */

      /* Verificar a lista de contadores */

         if ( pOrgListaContadores != NULL )
         {

            numSimbolos  = 0 ;

            pContadorAnt = NULL ;
            pContador    = pOrgListaContadores ;

            while ( pContador != NULL ) {

            /* Verificar conteúdo de contagem */

               numSimbolos++ ;

               if ( pContador->Contagem < -2 )
               {
                  numErros ++ ;
                  ExibirPrefixo( 0 ) ;
                  fprintf( TST_ObterArqLog( ) , "Valor de contagem ilegal: %d  no contador %s" ,
                           pContador->Contagem , pContador->NomeContador ) ;
               } /* if */

            /* Verificar nome de contador */

               if ( pContador->NomeContador[ 0 ] == 0 )
               {
                  numErros   ++ ;
                  ExibirPrefixo( 0 ) ;
                  fprintf( TST_ObterArqLog( ) , "Nome de contador é vazio." ) ;
               } /* if */

            /* Verificar ordenação do nome de contadores */

               if ( pContadorAnt != NULL )
               {
                  if ( strcmp( pContadorAnt->NomeContador , pContador->NomeContador ) >= 0 )
                  {
                     numErros ++ ;
                     ExibirPrefixo( 0 ) ;
                     fprintf( TST_ObterArqLog( ) , "Erro de ordenação na lsiat de contadores." ) ;
                     fprintf( TST_ObterArqLog( ) , "\n                    Primeiro: %s" ,
                                 pContadorAnt->NomeContador ) ;
                     fprintf( TST_ObterArqLog( ) , "\n                    Segundo:  %s" ,
                                 pContador->NomeContador ) ;
                  } /* if */
               } /* if */

            /* Avançar para o próximo contador */

               pContadorAnt = pContador ;
               pContador    = pContador->pProxContador ;

            } /* fim repete: Verificar a lista de contadores */

            if ( numSimbolos != numContadores )
            {
               numErros   ++ ;
               ExibirPrefixo( 0 ) ;
               fprintf( TST_ObterArqLog( ) , "Número de contadores errado. Tabela é: %d  Lista é: %d" ,
                        numContadores , numSimbolos ) ;
            } /* if */

         } /* fim ativa: Verificar a lista de contadores */

      return numErros ;

   } /* Fim função: CNT  &Verificar a estrutura dos contadores */

#endif


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: CNT  -Obter nome do contador
*
*  $ED Descrição da função
*     Obtém o ponteiro para o nome do contador
*
***********************************************************************/

   char * ObterNomeContador( void * pDado )
   {

      tpContador * pContador ;

      pContador = ( tpContador * ) pDado ;
      return pContador->NomeContador ;

   } /* Fim função: CNT  -Obter nome do contador */


/***********************************************************************
*
*  $FC Função: CNT  -Montar nome de arquivo
*
*  $ED Descrição da função
*     Adiciona o nome de extensão caso o nome de arquivo não o contenha
*
***********************************************************************/

   void MontarNomeArquivo( char * NomeArquivo ,
                           char * NomeExtensao )
   {

      int    i ;

      for( i = strlen( NomeArquivo ) ; i > 0 ; i -- )
      {
         if ( NomeArquivo[ i ] == '.' )
         {
            return ;
         } else if ( NomeArquivo[ i ] == '\\' )
         {
            break ;
         } /* if */
      } /* for */

      strcat( NomeArquivo , "." ) ;
      strcat( NomeArquivo , NomeExtensao ) ;

   } /* Fim função: CNT  -Montar nome de arquivo */


/***********************************************************************
*
*  $FC Função: CNT  -Ler linha limpa
*
*  $ED Descrição da função
*     Le linha acumulador limpa
*
*  $FV Valor retornado
*     Retorna o número de linhas lidas
*       -1 se fim de arquivo
*       -2 se erro de leitura
*
***********************************************************************/

   int LerLinha( FILE * pArquivo )
   {

      char * pLido ;

      int i ;

      /* Tratar fim de arquivo em contadores */

         if ( feof( pArquivo ))
         {
            BufferLeitura[ 0 ]  = 0 ;
            return -1 ;
         } /* if */

      /* Ler a linha de contadores */

         pLido = fgets( BufferLeitura , DIM_BUFFER , pArquivo ) ;

         if ( pLido != NULL )
         {
            numLinha ++ ;
         } else
         {
            BufferLeitura[ 0 ] = 0 ;
            if ( feof( pArquivo ))
            {
               return -1 ;
            } else
            {
               ExibirPrefixo( numLinha ) ;
               fprintf( TST_ObterArqLog( ) , "Erro de leitura." ) ;
               return -2 ;
            } /* if */
         } /* if */

      /* Limpar a linha de contadores */

         for ( i = strlen( BufferLeitura ) - 1 ; i >= 0 ; i-- )
         {
            if ( strchr( TRIM_CHAR , BufferLeitura[ i ] ) == NULL )
            {
               break ;
            } /* if */
         } /* for */

         BufferLeitura[ i + 1 ] = 0 ;

         return i + 1 ;

   } /* Fim função: CNT  -Ler linha limpa */


/***********************************************************************
*
*  $FC Função: CNT  -Ler arquivo de definição de contadores
*
*  $ED Descrição da função
*     Lê o conjunto de contadores definidos em um arquivo
*
*  $EP Parâmetros
*     NomeArquivo
*     Fonte       - identifica o tipo de arquivo
*                      ARQUIVO_DEFINICAO
*                      ARQUIVO_ACUMULADOR
*                   A fonte é utilizada para controlar duplicidade de
*                   declaração de contadores
*
*  $FV Valor retornado
*     Retorna o número de erros encontrados
*
*  $FA Arquivos manipulados
*     Formato do arquivo de contadores
*
*     Cada linha poderá ser uma de
*       //              - comentário, ignorado
*       linha em branco - ignorada
*       nome            - nome de um contador, inicializado para 0
*       nome\=vv        - nome de um contador inicializado para vv
*                    Valores especiais de vv:
*                      vv == -1 contador opcional
*                      vv == -2 contador proibido.
*                      vv <= -3 erro de dado
*     Nomes podem ser quaisquer strings, podendo conter brancos.
*     Os caracteres em branco finais serão eliminados
*
*  $EIU Interface com usuário pessoa
*     Erro - arquivo não existe e é de definição
*     Erro - nome definido mais de uma vez
*
***********************************************************************/

   int LerContadores( char * NomeArquivo , int Fonte )
   {

      FILE * pArq ;
      int    numErros ;
      int    numLidos ;

      tpContador * pContador ;
      char NomeContador[ DIM_NOME_CONTADOR + 1 ] ;

      char OpAtribuicao ;
      int  numCampos ;
      long ValorContagem ;

      tpContador * pContadorCorr ;
      tpContador * pContadorAnt ;

      numErros   = 0 ;
      numLinha   = 0 ;
      pArq       = NULL ;

      /* Abrir arquivo de contadores para leitura */

         pArq = fopen( NomeArquivo , "r" ) ;
         if ( pArq == NULL )
         {
            if ( Fonte == ARQUIVO_DEFINICAO )
            {
               ExibirPrefixo( -1 ) ;
               fprintf( TST_ObterArqLog( ) , "Não abriu o arquivo \"%s\"" ,
                        NomeArquivo ) ;
               numErros ++ ;
            } /* if */

            return numErros ;
         } /* if */

      /* Interpretar todas as linhas do arquivo de contadores */

         numLidos = LerLinha( pArq ) ;

         while ( numLidos >= 0 ) {

         /* Tratar linha de contadores em branco */

            if ( numLidos == 0 )
            {

            } /* fim ativa: Tratar linha de contadores em branco */

         /* Tratar linha de contadores comentário */

            else if ( memcmp( BufferLeitura , COMENTARIO , strlen( COMENTARIO )) == 0 )
            {

            } /* fim ativa: Tratar linha de contadores comentário */

         /* Tratar linha de declaração de contador */

            else
            {

               /* Extrair nome e inicialização de linha */

                  ValorContagem = 0 ;

                  numCampos = sscanf( BufferLeitura , " %" STR_DIM_NOME_CONTADOR "[^\\]\\%c%ld" ,
                                      NomeContador , &OpAtribuicao , &ValorContagem ) ;

                  if ( numCampos == 3 )
                  {
                     if ( OpAtribuicao != OP_ATRIBUICAO )
                     {
                        ExibirPrefixo( numLinha ) ;
                        fprintf( TST_ObterArqLog( ) , "\nOperador atribuição incorreto: %s" ,
                                 BufferLeitura ) ;
                        numErros ++ ;
                     } /* if */

                     if ( ValorContagem <= -3 )
                     {
                        ExibirPrefixo( numLinha ) ;
                        fprintf( TST_ObterArqLog( ) , "\nValor inicial da contagem ilegal: %s" ,
                                 BufferLeitura ) ;
                        numErros ++ ;
                     } /* if */

                  } else if ( numCampos != 1 )
                  {
                     ExibirPrefixo( numLinha ) ;
                     fprintf( TST_ObterArqLog( ) , "\nFormato correto: nome\\=valor. É: %s" ,
                              BufferLeitura ) ;
                     numErros ++ ;
                  } /* if */

               /* Criar estrutura contador */

                  pContador = ( tpContador * ) malloc( sizeof( tpContador )) ;
                  pContador->pProxContador = NULL ;
                  pContador->Contagem      = ValorContagem ;
                  pContador->Fonte         = Fonte ;
                  strcpy( pContador->NomeContador , NomeContador ) ;

               /* Inserir contador na tabela */

                  if ( TBS_InserirSimbolo( pTabela , pContador ) != TBS_CondRetOK  )
                  {

                     free( pContador ) ;

                     pContador = ( tpContador * ) TBS_ProcurarSimbolo( pTabela, NomeContador ) ;
                     TST_ASSERT( pContador != NULL ) ;

                     if ( Fonte == ARQUIVO_ACUMULADOR )
                     {
                        ExibirPrefixo( numLinha ) ;
                        fprintf( TST_ObterArqLog( ) , "Nome de contador duplicado em acumulador: %s" ,
                                 NomeContador ) ;
                        numErros ++ ;
                     } else {
                        if ( pContador->Fonte == ARQUIVO_DEFINICAO )
                        {
                           ExibirPrefixo( numLinha ) ;
                           fprintf( TST_ObterArqLog( ) , "Nome de contador duplicado em arquivo de definição: %s" ,
                                    NomeContador ) ;
                           numErros ++ ;
                        } else {
                           pContador->Fonte = ARQUIVO_DEFINICAO ;
                        } /* if */
                     } /* if */

                  } /* fim ativa: Inserir contador na tabela */

               /* Inserir contador em lista ordenada */

                  else
                  {

                     numContadores ++ ;

                     pContadorCorr = pOrgListaContadores ;
                     pContadorAnt  = NULL ;

                     while ( pContadorCorr != NULL )
                     {
                        if ( strcmp( NomeContador ,
                                     pContadorCorr->NomeContador ) < 0 )
                        {
                           break ;
                        } /* if */
                        pContadorAnt  = pContadorCorr ;
                        pContadorCorr = pContadorCorr->pProxContador ;
                     } /* while */

                     if ( pContadorAnt == NULL )
                     {
                        pContador->pProxContador = pOrgListaContadores ;
                        pOrgListaContadores      = pContador ;
                     } else
                     {
                        pContador->pProxContador    = pContadorCorr ;
                        pContadorAnt->pProxContador = pContador ;
                     } /* if */

                  } /* fim ativa: Inserir contador em lista ordenada */

            } /* fim ativa: Tratar linha de declaração de contador */

         /* Ler nova linha de contador */

            numLidos = LerLinha( pArq ) ;

         } /* fim repete: Interpretar todas as linhas do arquivo de contadores */

      /* Fechar o arquivo de contadores lido */

         if ( pArq != NULL )
         {
            fclose( pArq ) ;
         } /* if */

      return numErros ;

   } /* Fim função: CNT  -Ler arquivo de definição de contadores */


/***********************************************************************
*
*  $FC Função: CNT  -Exibir prefixo de mensagem
*
*  $ED Descrição da função
*     Exibe o prefixo de mensagens de erro geradas pelo módulo de contagem
*
***********************************************************************/

   void ExibirPrefixo( int numLinha )
   {

      char Msg[ DIM_BUFFER ] ;

      Msg[ 0 ] = 0 ;

      if ( numLinha > 0 )
      {
         sprintf( Msg , "  Linha: %5d " , numLinha ) ;
      } /* if */

      TST_ContarFalhas( ) ;

      TST_ExibirPrefixo( ERRO_CONTADOR , Msg ) ;

   } /* Fim função: CNT  -Exibir prefixo de mensagem */

/********** Fim do módulo de implementação: CNT  Contadores de passagem **********/

