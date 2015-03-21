/***************************************************************************
*  $MCI Módulo de implementação: TCED Teste controle de espaço dinâmico
*
*  Arquivo gerado:              TESTCED.c
*  Letras identificadoras:      TCED
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
#include "CESPDIN.H"
#include "LERPARM.H"


typedef     int      BOOL ;

/* Tabela dos nomes dos comandos de teste específicos */

static const char ResetEspacosCmd[ ]            = "=resetespaco" ;
static const char AlocarEspacoCmd[ ]            = "=alocarespaco" ;
static const char DesalocarEspacoCmd[ ]         = "=desalocarespaco" ;
static const char ExibirEspacoBrutoCmd[ ]       = "=exibirbruto" ;
static const char ExibirEspacoCmd[ ]            = "=exibirespaco" ;
static const char InserirStringEspacoCmd[ ]     = "=inserirstringespaco" ;
static const char DefinirTipoEspacoCmd[ ]       = "=definirtipoespaco" ;
static const char ObterTipoEspacoCmd[ ]         = "=obtertipoespaco" ;
static const char ObterStringEspacoCmd[ ]       = "=obterstringespaco" ;
static const char ObterTamanhoEspacoCmd[ ]      = "=obtertamanhoespaco" ;
static const char VerificarEspacoCmd[ ]         = "=verificarespaco" ;
static const char DeturparEspacoCmd[ ]          = "=deturparespaco" ;
static const char MarcarEspacoAtivoCmd[ ]       = "=marcarespacoativo" ;
static const char MarcarEspacoNaoAtivoCmd[ ]    = "=marcarespaconaoativo" ;
static const char MarcarTodosNaoAtivosCmd[ ]    = "=marcartodosnaoativos" ;
static const char VerificarEspacoAtivoCmd[ ]    = "=verificarespacoativo" ;
static const char IniciarIteradorEspacosCmd[ ]  = "=iniciariteradorespacos" ;
static const char AvancarProximoEspacoCmd[ ]    = "=avancarproximoespaco" ;
static const char ObterEspacoCorrenteCmd[ ]     = "=obterespacocorrente" ;
static const char ExisteEspacoCorrenteCmd[ ]    = "=existeespacocorrente" ;
static const char TerminarIteradorEspacoCmd[ ]  = "=terminariteradorespaco" ;
static const char ExcluirEspacoCorrenteCmd[ ]   = "=excluirespacocorrente" ;
static const char VerificarFrequenciaCmd[ ]     = "=verificarfrequencia" ;

#define     TRUE     1
#define     FALSE    0
#define     MSG_ERRO    ">>>"

#define     DIM_VT_ESPACO  5

static void * vtEspaco[ DIM_VT_ESPACO ] ;
       
static int EhPrimeiraVez = TRUE ;

/***** Protótipos das funções encapuladas no módulo *****/

   static void InstrumentarString( char * pString , int dimString ) ;

   static BOOL VerificarInxEspaco( int inxEspacoParm , BOOL EhNaoNulo ) ;

   static int VerificarValor( void * pValor ) ;

   static BOOL VerificarString( char * String , int dimString ) ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TCED &Efetuar comando de teste do controle de espaço dinâmico
*
*  $ED Descrição da função
*     Esta função testa as funções do módulo de controle
*     de espaço dinâmico que não são controladas pelo módulo
*     INTESPAC ICED Interpretador de comandos de controle de espaço dinâmico
*
*     O módulo TCNTESP interpreta os comandos de teste utilizados para
*     controlar os acesso a espaços de dados dinâmicos.
*     O contexto é formado por um vetor de 4 espaços que podem ser
*     endereçados pelo parâmetro <inxEspaco> dom comando.
*     São seguintes os comandos interpretados:
*
*     =resetespaco
*         esvazia todos os registros de referência a espaços alocados
*
*     =alocarespaco               <inxEspaco>  <tam>  <lin>  <NomeArquivo>
*         efetua um malloc
*
*     =desalocarespaco            <inxEspaco>
*         efetua um free
*
*     =exibirespaco               <inxEspaco>
*         exibe o conteúdo de um espaço em formato hexadecimal
*
*     =inserirstringespaco        <inxEspaco>  string terminado em \n
*         atribui um valor ao espaço. Pode ser utilizado para corromper
*         as estruturas de dados armazenadas em memória dinâmica
*
*     =deturparespaco             <inxEspaco>  <deslocamento>  <charHexa>
*         atribui um valor em memória relativa a um dado espaço.
*         Pode ser utilizado para corromper a estrutura de controle
*         de espaços dinâmicos
*
*     =verificarespaco            <inxEspaco>  <numFalhasEsperadas>
*         examina a validade do registro de determinado espaço. O espaço
*         pode conter falhas em virtude de possíveis corrupções com
*         o comando =deturparespaco
*
*     =definirtipoespaco          <inxEspaco>  <idTipo>  <condEsperada>
*         define o tipo associado ao espaço
*
*     =obtertipoespaco            <inxEspaco>  <idTipoEsperado>
*         obtém o tipo do espaço
*
*     =obterstringespaco          <inxEspaco>  stringEsperado terminado em \n
*         obtém o string contido no espaço
*
*     =obtertamanhoespaco         <inxEspaco>  <TamanhoEsperado>
*         obtém o tamanho do valor contido no espaço
*
*     =marcarespacoativo          <inxEspaco>
*         marca o espaço ativo
*
*     =marcarespaconaoativo       <inxEspaco>
*         marca o espaço inativo
*
*     =marcartodosnaoativos
*         torna inativos todos os espaços
*
*     =verificarespacoativo       <inxEspaco>
*         verifica se determinado espaço é ativo
*
*     =iniciariteradorespacos
*         inicia ou restaura o iterador que percorre o conjunto de todos os
*         espaços dinâmicos alocados
*
*     =avancarproximoespaco       <condEsperada>
*         caminha para o próximo espaço na lista
*
*     =obterespacocorrente        <inxEspaco>
*         obtém a referência para o espaço corrente do iterador atribuindo-a
*         ao elemento <inxEspaco>
*
*     =existeespacocorrente       <condEsperada>
*         verifica se o iterador está ou não ativo
*
*     =terminariteradorespaco
*         termina aexecução do iterador
*
*     =excluirespacocorrente
*         elimina o espaço corrente da lista de espaços controlados
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      int  i ;
         
      int  numLidos    = -1 ;
      int  inxEspaco   = -1 ;
      int  valEsperado = -1 ;
      int  valObtido   = -1 ;
         
      #define  DIM_STRING_PARM   100
      char StringParm[ DIM_STRING_PARM ] ;
         
      int       Deslocamento ;
      int       Valor ;

      int   Tamanho ;
      int   numLinhaFonte ;

      int idTipo ;

      int numAllocs  = -1 ,
          Frequencia =  0 ,
          ContaNULL  =  0 ;

      double Tolerancia      ,
             PercentEsperado ,
             PercentObtido   ;

      void * pEspaco = NULL ;

      InstrumentarString( StringParm , DIM_STRING_PARM ) ;

      /* Tratar: Reinicializar módulo de teste espeçifico */

         if ( strcmp( ComandoTeste , ResetEspacosCmd ) == 0 )
         {

            if ( EhPrimeiraVez )
            {
               EhPrimeiraVez = FALSE ;

               for( i = 0 ; i < DIM_VT_ESPACO ; i ++ )
               {
                  vtEspaco[ i ] = NULL ;
               } /* for */

               return TST_CondRetOK ;
            } /* if */

            for( i = 0 ; i < DIM_VT_ESPACO ; i++ )
            {
               if ( vtEspaco[ i ] != NULL )
               {
                  CED_Free( vtEspaco[ i ] ) ;
                  vtEspaco[ i ] = NULL ;
               } /* if */
            } /* for */

            return TST_CondRetOK ;

         } /* fim ativa: Tratar: Reinicializar módulo de teste espeçifico */

      /* Tratar: Deturpar espaço */

         else if ( strcmp( ComandoTeste , DeturparEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "iii" ,
                               &inxEspaco , &Deslocamento , &Valor ) ;

            if ( ( numLidos != 3 )
              || !VerificarInxEspaco( inxEspaco , TRUE ))
            {
               return TST_CondRetParm ;
            } /* if */

            (( char * ) vtEspaco[ inxEspaco ] )[ Deslocamento ] = ( char ) Valor ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar: Deturpar espaço */

      /* Tratar: Atribuir string ao espaço */

         else if ( strcmp( ComandoTeste , InserirStringEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "is" ,
                               &inxEspaco , StringParm ) ;

            if ( ( numLidos != 2 )
              || !VerificarInxEspaco( inxEspaco , TRUE )
              || !VerificarString( StringParm , DIM_STRING_PARM ))
            {
               return TST_CondRetParm ;
            } /* if */

            strcpy( vtEspaco[ inxEspaco ] , StringParm ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar: Atribuir string ao espaço */

      /* Testar: Obter string contido no espaço */

         else if ( strcmp( ComandoTeste , ObterStringEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "is" ,
                               &inxEspaco , StringParm ) ;

            if ( ( numLidos != 2 )
              || !VerificarInxEspaco( inxEspaco , TRUE )
              || !VerificarString( StringParm , DIM_STRING_PARM ))
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararEspaco( StringParm , vtEspaco[ inxEspaco ] ,
                      strlen( StringParm ) , "Conteúdo do valor errado." ) ;

         } /* fim ativa: Testar: Obter string contido no espaço */

      /* Testar: CED &Alocar espaço */

         else if ( strcmp( ComandoTeste , AlocarEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "iiis" ,
                               &inxEspaco , &Tamanho , &numLinhaFonte , StringParm ) ;

            if ( ( numLidos != 4 )
              || ( Tamanho  <= 0 )
              || ( numLinhaFonte < 1 )
              || !VerificarInxEspaco( inxEspaco , FALSE )
              || !VerificarString( StringParm , DIM_STRING_PARM ))
            {
               return TST_CondRetParm ;
            } /* if */

            vtEspaco[ inxEspaco ] =
                      CED_Malloc( Tamanho , numLinhaFonte , StringParm ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Alocar espaço */

      /* Testar: CED &Desalocar espaço */

         else if ( strcmp( ComandoTeste , DesalocarEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxEspaco ) ;

            if ( ( numLidos != 1 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            CED_Free( vtEspaco[ inxEspaco ] ) ;

            vtEspaco[ inxEspaco ] = NULL ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Desalocar espaço */

      /* Testar: CED &Exibir conteúdo bruto do espaço */

         else if ( strcmp( ComandoTeste , ExibirEspacoBrutoCmd ) == 0 )
         {

            #define  DIM_ESPACO  70

            numLidos = LER_LerParametros( "i" ,
                               &inxEspaco ) ;

            if ( ( numLidos != 1 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            TST_ExibirEspacoHexa( DIM_ESPACO + 4 , (( char * ) vtEspaco[ inxEspaco ] ) - DIM_ESPACO ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Exibir conteúdo bruto do espaço */

      /* Testar: CED &Exibir conteúdo útil do espaço */

         else if ( strcmp( ComandoTeste , ExibirEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxEspaco ) ;

            if ( ( numLidos != 1 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            CED_ExibirEspaco( vtEspaco[ inxEspaco ] ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Exibir conteúdo útil do espaço */

      /* Testar: CED &Verificar a integridade de um espaço de dados */

         else if ( strcmp( ComandoTeste , VerificarEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &inxEspaco , &valEsperado ) ;

            if ( ( numLidos != 2 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_VerificarEspaco( vtEspaco[ inxEspaco ] ,
                                             VerificarValor ) ;

            return TST_CompararBool( valEsperado , valObtido ,
                   "Verificação resultou errado." ) ;

         } /* fim ativa: Testar: CED &Verificar a integridade de um espaço de dados */

      /* Testar: CED &Definir o tipo do espaço */

         else if ( strcmp( ComandoTeste , DefinirTipoEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "iii" ,
                               &inxEspaco , &idTipo , &valEsperado ) ;

            if ( ( numLidos != 3 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_DefinirTipoEspaco( vtEspaco[ inxEspaco ] , idTipo );

            return TST_CompararInt( valEsperado , valObtido ,
                   "Condição de retorno errada." ) ;

         } /* fim ativa: Testar: CED &Definir o tipo do espaço */

      /* Testar: CED &Obter o tipo do espaço */

         else if ( strcmp( ComandoTeste , ObterTipoEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &inxEspaco , &valEsperado ) ;

            if ( ( numLidos != 2 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_ObterTipoEspaco( vtEspaco[ inxEspaco ] ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "Tipo do espaço errado." ) ;

         } /* fim ativa: Testar: CED &Obter o tipo do espaço */

      /* Testar: CED &Obter tamanho do valor contido no espaço */

         else if ( strcmp( ComandoTeste , ObterTamanhoEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &inxEspaco , &valEsperado ) ;

            if ( ( numLidos != 2 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_ObterTamanhoValor( vtEspaco[ inxEspaco ] ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "Tamanho do valor errado." ) ;

         } /* fim ativa: Testar: CED &Obter tamanho do valor contido no espaço */

      /* Testar: CED &Marcar ativo o espaço */

         else if ( strcmp( ComandoTeste , MarcarEspacoAtivoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxEspaco ) ;

            if ( ( numLidos != 1 )
              || !VerificarInxEspaco( inxEspaco , TRUE ))
            {
               return TST_CondRetParm ;
            } /* if */

            CED_MarcarEspacoAtivo( vtEspaco[ inxEspaco ] ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Marcar ativo o espaço */

      /* Testar: CED &Marcar não ativo o espaço */

         else if ( strcmp( ComandoTeste , MarcarEspacoNaoAtivoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxEspaco ) ;

            if ( ( numLidos != 1 )
              || !VerificarInxEspaco( inxEspaco , TRUE ))
            {
               return TST_CondRetParm ;
            } /* if */

            CED_MarcarEspacoNaoAtivo( vtEspaco[ inxEspaco ] ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Marcar não ativo o espaço */

      /* Testar: CED &Marcar não ativos todos os espaços */

         else if ( strcmp( ComandoTeste , MarcarTodosNaoAtivosCmd ) == 0 )
         {

            CED_MarcarTodosEspacosInativos( ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Marcar não ativos todos os espaços */

      /* Testar: CED &Verificar se espaço é ativo */

         else if ( strcmp( ComandoTeste , VerificarEspacoAtivoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &inxEspaco , &valEsperado ) ;

            if ( ( numLidos != 2 )
              || !VerificarInxEspaco( inxEspaco , TRUE ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_EhEspacoAtivo( vtEspaco[ inxEspaco ] ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "Marca de espaço ativo errada." ) ;

         } /* fim ativa: Testar: CED &Verificar se espaço é ativo */

      /* Testar CED  &Limitar com freqüência de geração de NULL */

         else if ( strcmp( ComandoTeste , VerificarFrequenciaCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "iif" ,
                               &numAllocs , &Frequencia , &Tolerancia ) ;

            if ( ( numLidos != 3   )
              || ( numAllocs < 100 ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_LimitarFrequencia( Frequencia ) ;
            if ( !valObtido )
            {
               return TST_NotificarFalha( "Freqüência limite de malloc em erro." ) ;
            } /* if */

            for( i = 0 ; i < numAllocs ; i++ )
            {
               pEspaco = CED_Malloc( 100 , i + 1 , "TesteFrequencia" ) ;
               if ( pEspaco == NULL )
               {
                  ContaNULL ++ ;
               } else
               {
                  CED_Free( pEspaco ) ;
               } /* if */
            } /* for */

            PercentEsperado = ( 1000. - Frequencia ) / 1000. ;

            PercentObtido   = ContaNULL ;
            PercentObtido   = PercentObtido / numAllocs ;

            return TST_CompararFloat( PercentEsperado , PercentObtido , Tolerancia ,
                        "Número de NULLs gerados está fora da tolerância." ) ;

         } /* fim ativa: Testar CED  &Limitar com freqüência de geração de NULL */

      /* Testar: CED &Iterador: iniciar iterador de espaços */

         else if ( strcmp( ComandoTeste , IniciarIteradorEspacosCmd ) == 0 )
         {

            CED_InicializarIteradorEspacos(  ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Iterador: iniciar iterador de espaços */

      /* Testar: CED &Iterador: avançar para o próximo espaço */

         else if ( strcmp( ComandoTeste , AvancarProximoEspacoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &valEsperado ) ;

            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_AvancarProximoEspaco( ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "Condição de avanço errada." ) ;

         } /* fim ativa: Testar: CED &Iterador: avançar para o próximo espaço */

      /* Testar: CED &Iterador: obter referência para o espaço corrente */

         else if ( strcmp( ComandoTeste , ObterEspacoCorrenteCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxEspaco ) ;

            if ( ( numLidos != 1 )
              || !VerificarInxEspaco( inxEspaco , FALSE ))
            {
               return TST_CondRetParm ;
            } /* if */

            vtEspaco[ inxEspaco ] = CED_ObterPonteiroEspacoCorrente( ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Iterador: obter referência para o espaço corrente */

      /* Testar: CED &Iterador: existe espaço corrente */

         else if ( strcmp( ComandoTeste , ExisteEspacoCorrenteCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &valEsperado ) ;

            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_ExisteEspacoCorrente( ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "Condição de existência de iterador errada." ) ; {

            } /* fim repete: Testar: CED &Iterador: existe espaço corrente */

         } /* fim ativa: Testar: CED &Iterador: existe espaço corrente */

      /* Testar: CED &Iterador: terminar iterador */

         else if ( strcmp( ComandoTeste , TerminarIteradorEspacoCmd ) == 0 )
         {

            CED_TerminarIteradorEspacos(  ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Iterador: terminar iterador */

      /* Testar: CED &Iterador: excluir espaço corrente */

         else if ( strcmp( ComandoTeste , ExcluirEspacoCorrenteCmd ) == 0 )
         {

            CED_ExcluirEspacoCorrente( ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Iterador: excluir espaço corrente */

      return TST_CondRetNaoConhec ;

   } /* Fim função: TCED &Efetuar comando de teste do controle de espaço dinâmico */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: TCED -Instrumentar string local
*
*  $ED Descrição da função
*     Adiciona controle de extravasão a um espaço destinado para string
*
***********************************************************************/

   void InstrumentarString( char * pString , int dimString )
   {

      TST_ASSERT( dimString >= 2 ) ;

      pString[ dimString - 1 ] = 0 ;
      pString[ dimString - 2 ] = '\xFC' ;
      memset( pString , '?' , dimString - 2 ) ;

   } /* Fim função: TCED -Instrumentar string local */


/***********************************************************************
*
*  $FC Função: TCED -Verificar índice de espaço
*
*  $ED Descrição da função
*     Verifica se o índice do registro de espaço de dados contido no comando
*     de teste é válido.
*
***********************************************************************/

   BOOL VerificarInxEspaco( int inxEspacoParm , BOOL EhNaoNulo )
   {

      if ( ( inxEspacoParm <  0 )
        || ( inxEspacoParm >= DIM_VT_ESPACO ))
      {
         return FALSE ;
      } /* if */

      if ( EhNaoNulo )
      {
         if ( vtEspaco[ inxEspacoParm ] == NULL )
         {
            return FALSE ;
         } /* if */
      } /* if */

      return TRUE ;

   } /* Fim função: TCED -Verificar índice de espaço */


/***********************************************************************
*
*  $FC Função: TCED -Verificar valor contido no espaço
*
*  $ED Descrição da função
*     Esta função ilustra a interface de verificação
*     Se o espaço inicia com caractere "vazio (recém alocado)"
*     todo ele deve estar vazio.
*
*     A implementação da presente função não pode ser utilizada
*     quando a alocação de espaços for realizada por malloc,
*     uma vez que depende da função CED_ObterEspaco que não
*     existe para espaços alocados com malloc.
*
*     Para poder ser utilizada com espaços alocados com malloc,
*     a verificação deve ser possível conhecendo-se apenas o
*     ponteiro para o valor.
*
***********************************************************************/

   int VerificarValor( void * pValor )
   {

      int Tamanho ;
      int i ;

      Tamanho = CED_ObterTamanhoValor( pValor ) ;

      if ( (( char * ) pValor )[ 0 ] == '|' )
      {
         for( i = 0 ; i < Tamanho ; i++ )
         {
            if ( (( char * ) pValor )[ i ] != '|' )
            {
               return 0 ;
            } /* if */
         } /* for */
      } /* if */

      return 1 ;

   } /* Fim função: TCED -Verificar valor contido no espaço */


/***********************************************************************
*
*  $FC Função: TCED -Verificar extravasão de string
*
*  $ED Descrição da função
*     Verifica se ocorreu extravasão em string local controlado
*
***********************************************************************/

   BOOL VerificarString( char * String , int dimString )
   {

      if ( ( String[ dimString - 1 ] != 0      )
        || ( String[ dimString - 2 ] != '\xFC' ))
      {
         return FALSE ;
      } /* if */
      return TRUE ;

   } /* Fim função: TCED -Verificar extravasão de string */

/********** Fim do módulo de implementação: TCED Teste controle de espaço dinâmico **********/

