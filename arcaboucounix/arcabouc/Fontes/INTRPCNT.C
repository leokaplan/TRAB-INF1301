/***************************************************************************
*  $MCI Módulo de implementação: ICNT Interpretador de contadores
*
*  Arquivo gerado:              INTRPCNT.C
*  Letras identificadoras:      ICNT
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
*     5       avs   18/mai/2008 corrigir e ampliar CESPDIN
*     4       avs   01/fev/2006 criar linguagem script simbólica
*     3       avs   08/dez/2004 uniformização dos exemplos
*     2       avs   07/jul/2003 unificação de todos os módulos em um só projeto
*     1       avs   16/abr/2003 início desenvolvimento
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>

#define INTRPCNT_OWN
#include "INTRPCNT.H"
#undef INTRPCNT_OWN

#include "GENERICO.H"
#include "LERPARM.H"
#include "CONTA.H"

#define     DIM_MSG             500
#define     DIM_NOME_ARQ        500
#define     DIM_NOME_CONTADOR   500

/* Comandos de contagem */

static const char INICIALIZAR_CONTADORES_CMD[ ]   = "=inicializarcontadores" ;
static const char TERMINAR_CONTADORES_CMD[ ]      = "=terminarcontadores" ;
static const char REGISTRAR_ACUMULADOR_CMD[ ]     = "=registraracumulador" ;
static const char LER_CONTADORES_CMD[ ]           = "=lercontadores" ;
static const char GRAVAR_CONTADORES_CMD[ ]        = "=gravarcontadores" ;
static const char ZERAR_CONTADORES_CMD[ ]         = "=zerartodoscontadores" ;
static const char ZERAR_CONTADOR_DADO_CMD[ ]      = "=zerarcontador" ;
static const char INICIAR_CONTAGEM_CMD[ ]         = "=iniciarcontagem" ;
static const char PARAR_CONTAGEM_CMD[ ]           = "=pararcontagem" ;
static const char OBTER_CONTAGEM_CMD[ ]           = "=contagemcontador" ;
static const char EXIBIR_CONTAGEM_CMD[ ]          = "=exibircontagem" ;
static const char OBTER_NUM_CONTADORES_CMD[ ]     = "=numcontadores" ;
static const char OBTER_TOTAL_CONTAGEM_CMD[ ]     = "=contagemtotal" ;
static const char VERIFICAR_CONTAGENS_CMD[ ]      = "=verificarcontagens" ;

static char CondRetErrada[ ] = "Condição de retorno errada." ;

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: ICNT &Interpretar comandos de contagem
*  ****/

   TST_tpCondRet ICNT_EfetuarComadoContagem( char * ComandoTeste )
   {

      char   NomeArquivo[  DIM_NOME_ARQ ] ;
      char   NomeContador[ DIM_NOME_CONTADOR ] ;

      int    numLidos ,
             IntEsp   ,
             IntObtido ;

      CNT_tpCondRet CondRetEsp ,
                    CondRetEh ;

      /* Tratar CNT  &Inicializar contadores */

         if ( strcmp( ComandoTeste , INICIALIZAR_CONTADORES_CMD ) == 0 )
         {

            /*
               CNT_tpCondRet CNT_InicializarContadores( FILE * pArqLog , char * NomeArquivo )
            */

            CondRetEsp = CNT_CondRetOK ;

            numLidos = LER_LerParametros( "si" ,
                               NomeArquivo , &CondRetEsp ) ;

            if ( numLidos < 1 )
            {
               CondRetEsp = CNT_CondRetErro ;
            } /* if */

            if ( ( NomeArquivo[ 0 ] == 0   )
              || ( numLidos < 1            ))
            {
               CondRetEh = CNT_InicializarContadores( NULL ) ;
            } else {
               CondRetEh = CNT_InicializarContadores( NomeArquivo ) ;
            } /* if */

            return TST_CompararInt( CondRetEsp , CondRetEh ,
                    "Condição de retorno errada." ) ;

         } /* fim ativa: Tratar CNT  &Inicializar contadores */

      /* Tratar CNT  &Terminar contadores */

         else if ( strcmp( ComandoTeste , TERMINAR_CONTADORES_CMD ) == 0 )
         {

            /*
               CNT_tpCondRet CNT_TerminarContadores( )
            */

            CondRetEsp = CNT_CondRetOK ;

            numLidos = LER_LerParametros( "i" ,
                               &CondRetEsp ) ;

            return TST_CompararInt( CondRetEsp , CNT_TerminarContadores( ) ,
                             CondRetErrada ) ;

         } /* fim ativa: Tratar CNT  &Terminar contadores */

      /* Tratar CNT  &Registrar arquivo acumulador */

         if ( strcmp( ComandoTeste , REGISTRAR_ACUMULADOR_CMD ) == 0 )
         {

            /*
               void CNT_RegistrarAcumulador( char * NomeArquivo ) ;
            */

            numLidos = LER_LerParametros( "s" ,
                               NomeArquivo ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            if ( NomeArquivo[ 0 ] == 0 )
            {
               CNT_RegistrarAcumulador( NULL ) ;
            } else {
               CNT_RegistrarAcumulador( NomeArquivo ) ;
            } /* if */

            return TST_CondRetOK ;

         } /* fim ativa: Tratar CNT  &Registrar arquivo acumulador */

      /* Tratar CNT  &Ler arquivo de definição de contadores */

         else if ( strcmp( ComandoTeste , LER_CONTADORES_CMD ) == 0 )
         {

            /*
               int CNT_LerContadores( char * NomeArquivoDefinicao )
            */
            IntEsp = 0 ;

            numLidos = LER_LerParametros( "si" ,
                               NomeArquivo , &IntEsp ) ;

            if ( ( numLidos < 1 )
              || ( IntEsp   < 0 ))
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( IntEsp , CNT_LerContadores( NomeArquivo ) ,
                      "Número de erros de leitura errado." ) ;

         } /* fim ativa: Tratar CNT  &Ler arquivo de definição de contadores */

      /* Tratar CNT  &Gravar arquivo de contagem acumulada */

         else if ( strcmp( ComandoTeste , GRAVAR_CONTADORES_CMD ) == 0 )
         {

            /*
               int CNT_GravarContadores( char * NomeArquivo )
            */
            IntEsp = 0 ;

            numLidos = LER_LerParametros( "si" ,
                               NomeArquivo , &IntEsp ) ;

            if ( ( numLidos < 1 )
              || ( IntEsp   < 0 ))
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( IntEsp , CNT_GravarContadores( NomeArquivo ) ,
                      "Número de erros de gravação errado." ) ;

         } /* fim ativa: Tratar CNT  &Gravar arquivo de contagem acumulada */

      /* Tratar CNT  &Zerar todos contadores */

         else if ( strcmp( ComandoTeste , ZERAR_CONTADORES_CMD ) == 0 )
         {

            /*
               CNT_tpCondRet CNT_ZerarContadores( )
            */

            CondRetEsp = CNT_CondRetOK ;

            numLidos = LER_LerParametros( "i" ,
                               &CondRetEsp ) ;

            return TST_CompararInt( CondRetEsp , CNT_ZerarContadores( ) ,
                      CondRetErrada ) ;

         } /* fim ativa: Tratar CNT  &Zerar todos contadores */

      /* Tratar CNT  &Zerar contador dado */

         else if ( strcmp( ComandoTeste , ZERAR_CONTADOR_DADO_CMD ) == 0 )
         {

            /*
               CNT_tpCondRet CNT_ZerarContador( char * NomeContador )
            */

            CondRetEsp = CNT_CondRetOK ;

            numLidos = LER_LerParametros( "si" ,
                               NomeContador , &CondRetEsp ) ;
            if ( numLidos < 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( CondRetEsp , CNT_ZerarContador( NomeContador ) ,
                      CondRetErrada ) ;

         } /* fim ativa: Tratar CNT  &Zerar contador dado */

      /* Tratar CNT  &Iniciar a contagem */

         else if ( strcmp( ComandoTeste , INICIAR_CONTAGEM_CMD ) == 0 )
         {

            /*
               void CNT_IniciarContagem( )
            */

            CNT_IniciarContagem( ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar CNT  &Iniciar a contagem */

      /* Tratar CNT  &Parar contagem */

         else if ( strcmp( ComandoTeste , PARAR_CONTAGEM_CMD ) == 0 )
         {

            /*
               void CNT_PararContagem( )
            */

            CNT_PararContagem( ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar CNT  &Parar contagem */

      /* Tratar CNT  &Obter valor de contagem */

         else if ( strcmp( ComandoTeste , OBTER_CONTAGEM_CMD ) == 0 )
         {

            /*
               long CNT_ObterContagem( char * NomeContador )
            */

            numLidos = LER_LerParametros( "si" ,
                               NomeContador , &IntEsp ) ;
            if ( numLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( IntEsp ,
                      ( int ) CNT_ObterContagem( NomeContador ) ,
                      "Valor de contagem errado." ) ;

         } /* fim ativa: Tratar CNT  &Obter valor de contagem */

      /* Tratar Exibir valor de contagem */

         else if ( strcmp( ComandoTeste , EXIBIR_CONTAGEM_CMD ) == 0 )
         {

            /*
               long CNT_ObterContagem( char * NomeContador )
            */

            numLidos = LER_LerParametros( "s" ,
                               NomeContador ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            IntObtido = ( int ) CNT_ObterContagem( NomeContador ) ;
            if ( IntObtido == CNT_CondRetNaoContador )
            {
               TST_NotificarFalha( "Contador desconhecido: " ) ;
               fprintf( TST_ObterArqLog( ) , "\"%s\"" , NomeContador ) ;
               return TST_CondRetErro ;
            } /* if */

            TST_ExibirPrefixo( SINALIZA_COMENTARIO , "" ) ;
            fprintf( TST_ObterArqLog( ) , "Valor do contador \"%s\" é: %d " ,
                               NomeContador , IntObtido ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar Exibir valor de contagem */

      /* Tratar CNT  &Obter número de contadores */

         else if ( strcmp( ComandoTeste , OBTER_NUM_CONTADORES_CMD ) == 0 )
         {

            /*
               int CNT_ObterNumeroContadores( )
            */

            numLidos = LER_LerParametros( "i" ,
                               &IntEsp ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( IntEsp , CNT_ObterNumeroContadores( ) ,
                      "Número de contadores errado." ) ;

         } /* fim ativa: Tratar CNT  &Obter número de contadores */

      /* Tratar CNT  &Obter total de contagem */

         else if ( strcmp( ComandoTeste , OBTER_TOTAL_CONTAGEM_CMD ) == 0 )
         {

            /*
               long CNT_ObterContagemTotal( )
            */

            numLidos = LER_LerParametros( "i" ,
                               &IntEsp ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( IntEsp ,
                      ( int ) CNT_ObterContagemTotal( ) ,
                      "Contagem total errada." ) ;

         } /* fim ativa: Tratar CNT  &Obter total de contagem */

      /* Tratar CNT  &Verificar contagem */

         else if ( strcmp( ComandoTeste , VERIFICAR_CONTAGENS_CMD ) == 0 )
         {

            /*
               int CNT_VerificarContagem( )
            */

            numLidos = LER_LerParametros( "i" ,
                               &IntEsp ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( IntEsp ,
                      CNT_VerificarContagem( ) ,
                      "Número de contadores zero errado." ) ;

         } /* fim ativa: Tratar CNT  &Verificar contagem */

      return TST_CondRetNaoExecutou ;

   } /* Fim função: ICNT &Interpretar comandos de contagem */

/********** Fim do módulo de implementação: ICNT Interpretador de contadores **********/

