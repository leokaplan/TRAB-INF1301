#if ! defined( LerParm_ )
#define LerParm_
/***************************************************************************
*
*  $MCD Módulo de definição: LER  Ler parâmetros de comandos
*
*  Arquivo gerado:              LerParm.h
*  Letras identificadoras:      LER
*
*  Nome da base de software:    Arcabouço para a automação de testes de programas redigidos em C
*  Arquivo da base de software: C:\AUTOTEST\PROJETOS\INSTRUM.BSW
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
*     Le parâmetros contidos em um buffer.
*     Os parâmetros podem ser simbólicos ou literais.
*     Parâmetros simbólicos devem ser declarados antes de serem utilizados.
*
***************************************************************************/
 
#if defined( LerParm_OWN )
   #define LerParm_EXT
#else
   #define LerParm_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

#define  LER_DIM_NOME     32
#define  LER_DIM_STRING  500
#define  LER_DIM_BUFFER      250


/***********************************************************************
*
*  $TC Tipo de dados: LER Condições de retorno
*
*
***********************************************************************/

   typedef enum {

         LER_CondRetOK ,
               /* Ler retorno OK */

         LER_CondRetNaoAbriu
               /* Ler não abriu arquivo */

   } LER_tpCondRet ;

/***** Dados exportados pelo módulo *****/

   /* Estrutura de dados: *LER  Dados globais públicos */

   LerParm_EXT int LER_TamBuffer ;
         /* Tamanho do string contido no buffer de leitura */

   LerParm_EXT char LER_Buffer[ LER_DIM_BUFFER + 1 ] ;
         /* Buffer de leitura */


/***********************************************************************
*
*  $FC Função: LER  &Inicializar módulo LerParm
*
*  $ED Descrição da função
*     Inicializa o módulo.
*     Deve ser chamada antes de iniciar a análise de comandos.
*     Deve ser chamada uma única vez no início ou após terminar o módulo.
*
***********************************************************************/

   int LER_InicializarLeitura( void ) ;


/***********************************************************************
*
*  $FC Função: LER  &Obter nome do arquivo script
*
***********************************************************************/

   char * LER_ObterNomeArqScript( void ) ;


/***********************************************************************
*
*  $FC Função: LER  &Interpretar comandos de teste
*
*  $ED Descrição da função
*     Interpreta os comandos de teste disponibilizados pelo módulo
*     de leitura.
*
*     Ver a descrição do módulo para mais detalhes
*
*     =declararparm   <nome>  <idTipo>  <literal>
*        declara o parâmetro <nome> do <tipo> definido com o valor <literal>
*        <tipo> pode ser um de:
*           i - inteiro (long)
*           f - flutuante (double)
*           c - caractere
*           s - string
*        O valor de literal tem que estar em conformidade com o tipo
*
*     =exibirdeclaracao <nome>
*        exibe a declaração do parâmetro <nome> em formato "dump de memória"
*
*     Estes comandos sempres estarão disponíveis. Os demais comandos de
*     teste estão impementados no módulo TESTLER específico para este módulo.
*
***********************************************************************/

   TST_tpCondRet LER_InterpretarComandos( char * ComandoTeste ) ;


/***********************************************************************
*
*  $FC Função: LER  &Terminar módulo LerParm
*
***********************************************************************/

   void LER_TerminarLeitura( void ) ;


/***********************************************************************
*
*  $FC Função: LER  &Abrir arquivo script de teste
*
*  $ED Descrição da função
*     Abre o arquivo que contém o script de teste.
*     A extensão default do arquivo de comandos script de teste é
*     ".script" e será acrescida ao nome do arquivo caso este não
*     contenha nome de extensão.
*
*  $EP Parâmetros
*     $P NomeArqParm - nome do arquivo contendo o script de teste
*
*  $FV Valor retornado
*     LER_CondRetOK        - caso tenha sido possível abrir o arquivo.
*     LER_CondRetNaoAbriu  - caso tenha ocorrido algum erro.
*                            Não há discriminação do erro encontrado.
*
***********************************************************************/

   LER_tpCondRet LER_AbrirArquivoScript( char * NomeArqParm ) ;


/***********************************************************************
*
*  $FC Função: LER  &Fechar arquivo script
*
***********************************************************************/

   void LER_FecharScript( void ) ;


/***********************************************************************
*
*  $FC Função: LER  &Declarar parâmetro simbólico
*
***********************************************************************/

   int LER_DeclararParametro( void ) ;


/***********************************************************************
*
*  $FC Função: LER  &Exibir declaração de símbolo
*
***********************************************************************/

   void LER_ExibirParametro( char * Simbolo ) ;


/***********************************************************************
*
*  $FC Função: LER  &Ler linha script eliminando brancos ao final
*
*  $ED Descrição da função
*     Lê uma linha do arquivo script e elimina os caracteres branco,
*     fim de linha, tabulação etc. do final da linha lida.
*
*  $FV Valor retornado
*     Retorna o tamanho da linha lida.
*     Será:
*        -1 caso tenha ocorrido fim de arquivo.
*        -2 caso tenha ocorrido algum problema de leitura.
*
***********************************************************************/

   int LER_LerLinhaScript( void ) ;


/***********************************************************************
*
*  $FC Função: LER  &Obter número de linhas lidas até o momento
*
***********************************************************************/

   int LER_ObterNumLinhas( void ) ;


/***********************************************************************
*
*  $FC Função: LER  &Ler linha de parâmetros
*
*  $ED Descrição da função
*     Lê um número variável de parâmetros
*
*  $EP Parâmetros
*     Tipos   é um string contendo a identificação dos tipos dos parâmetros
*             a serem lidos.
*     ...     Os demais parâmetros são referências (ponteiros) para os
*             espaços onde deverão ser armazenados os valores lidos.
*
*     Tal como em scanf deve-se tomar o cuidado de assegurar que a lista
*     de referência e o string de tipos tenha o mesmo tamanho
*     e que os indicadores de tipo sejam consistentes com a referência
*     posicionalmente correspondente.
*
*     Ao ler um campo primeiro procura ler um nome de parâmetro já
*     declarado. Caso isto seja possível e o tipo esteja correto
*     é " lido" o valor contido no parâmetro
*     Caso não seja possível tenta ler um literal em acordo com o tipo
*     dado.
*
*     Gera mensagens de erro para parâmetros desconhecidos, sintaxe errada
*     e tipo inconsistente com o solicitado.
*
*  $FV Valor retornado
*     Retorna o número de campos lidos até concluir ou até encontrar
*     um erro.
*
***********************************************************************/

   int LER_LerParametros( char * Tipos , ... ) ;


/***********************************************************************
*
*  $FC Função: LER  &Ler parâmetro caractere
*
***********************************************************************/

   int LER_LerParmChar( char * Parm ) ;


/***********************************************************************
*
*  $FC Função: LER  &Ler parâmetro flutuante
*
***********************************************************************/

   int LER_LerParmDouble( double * Parm ) ;


/***********************************************************************
*
*  $FC Função: LER  &Ler parâmetro nome
*
***********************************************************************/

   int LER_LerParmNome( char * Parm , int * tamNome , int dimNome ) ;


/***********************************************************************
*
*  $FC Função: LER  &Ler parâmetro inteiro
*
***********************************************************************/

   int LER_LerParmInt( int * Parm ) ;


/***********************************************************************
*
*  $FC Função: LER  &Ler parâmetro string
*
***********************************************************************/

   int LER_LerParmString( char * Parm , int * tamString , int dimString ) ;


/***********************************************************************
*
*  $FC Função: LER  &Pular comando de teste
*
***********************************************************************/

   void LER_PularComando( void ) ;

#undef LerParm_EXT

/********** Fim do módulo de definição: LER  Ler parâmetros de comandos **********/

#else
#endif
