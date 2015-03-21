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
*     Provê funções para a leitura e análise léxica dos comandos de teste.
*     Pressupõe-se que cada comando de teste esteja integralmente em
*     uma linha.
*     Cada comando de teste inicia com o caractere '=' seguido de um string
*     que identifica o comando.
*     Cada comando pode requerer zero ou mais parâmetros que se encontram na
*     mesma linha que o comando.
*     Parâmetros podem ser literis ou simbólicos.
*     Os parâmetros simbólicos precisam ser declarados antes de serem
*     utilizados.
*     Os parâmetros têm tipo e a leitura deve respeitar esses tipos.
*
*     Se for do interesse do programador, módulos de teste específico
*     podem ler e processar por conta própria linhas do script. Isto
*     pode ser necessário quando um módulo necessita de um grande
*     número de parâmetros ou de dados especiais.
*
*  $EIU Interface com o usuário pessoa
*     Este módulo interpreta os comandos de teste globalmente disponíveis:
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
*     Literais devem obedecer às regras léxicas utilizadas pela linguagem C.
*
*     Estes comandos sempre estarão disponíveis. Os demais comandos de
*     teste estão impementados no módulo TESTLER específico para este módulo.
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
         /* Tamanho do string contido no buffer de leitura
               *
               *$ED Descrição
               *   Contém o número total de caracteres do buffer, excluindo o zero.
               *   Jamais altere este valor. */

   LerParm_EXT int  LER_InxBuffer ;
         /* Cursor do buffer de leitura
               *
               *$ED Descrição
               *   O cursor de leitura indexa o primeiro caractere não lido contido
               *   no buffer.
               *   O cursor é zerado sempre que uma nova linha for lida.
               *   O cursor é avançado ao ler um parâmetro, sendo colocado no
               *   primeiro caractere imediatamente após ao parâmetro lido.
               *   Evite acessar caracteres quando LER_InxBuffer > LER_TamBuffer */

   LerParm_EXT char LER_Buffer[ LER_DIM_BUFFER + 1 ] ;
         /* Buffer de leitura
               *
               *$ED Descrição
               *   Contém a linha corrente do script a ser processada.
               *   Jamais altere o valor do buffer. */


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
*  $FC Função: LER  &Terminar módulo LerParm
*
*  $ED Descrição da função
*     Fecha o arquivo de leitura e sinaliza módulo de leitura não
*     inicializado.
*     Após esta função somente poderá ser chamada a funçãp
*     LER_InicializarLeitura
*
***********************************************************************/

   void LER_TerminarLeitura( void ) ;


/***********************************************************************
*
*  $FC Função: LER  &Obter nome do arquivo script
*
*  $FV Valor retornado
*     Retorna o ponteiro para o nome do arquivo script sendo processado.
*     Deve-se tomar cuidado para não alterar este nome.
*     Será NULL se nãa tem arquivo aberto ou o módulo não está inicializado.
*
***********************************************************************/

   char * LER_ObterNomeArqScript( void ) ;


/***********************************************************************
*
*  $FC Função: LER  &Obter número de linhas lidas até o momento
*
***********************************************************************/

   int LER_ObterNumLinhas( void ) ;


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
*  $FC Função: LER  &Interpretar comandos de teste
*
*  $ED Descrição da função
*     Interpreta os comandos de teste disponibilizados pelo módulo
*     de leitura.
*
*     Ver a descrição do módulo para mais detalhes
*
*  $EP Parâmetros
*     $P - Comando de teste a ser interpretado
*
*  $FV Valor retornado
*     TST_CondRetOK          - caso tenha interpretado corretamente
*     TST_CondRetNaoExecutou - caso o comando de teste a interpretar
*               não pertença ao conjunto de LER
*     TST_CondRetErro        - caso o comando de teste é do conjunto de LER
*                              e está sintaticamente errado.
*                              Neste caso é gerada uma mensagem de erro no
*                              log.
*
***********************************************************************/

   TST_tpCondRet LER_InterpretarComandos( char * ComandoTeste ) ;


/***********************************************************************
*
*  $FC Função: LER  &Declarar parâmetro simbólico
*
*  $EP Parâmetros
*     Interpreta uma linha de comando de teste contendo uma declaração de
*     nome simbólico.
*     Todas as declarações têm o formato:
*       =declararparm  <nome> <tipo> <literal>
*          em que o <literal> deve estar consistente com o <tipo>
*     São geradas mensagens de erro no log de teste caso a declaração
*     não possa ser realizada.
*
*  $FV Valor retornado
*     TRUE  - se a declaração ocorreu
*     FALSE - caso contrário
*
***********************************************************************/

   int LER_DeclararParametro( void ) ;


/***********************************************************************
*
*  $FC Função: LER  &Exibir declaração de símbolo
*
*  $ED Descrição da função
*     Exibe o Símbolo e o valor associado ao símbolo, sendo que este
*     é exibido em hexadecimal.
*
***********************************************************************/

   void LER_ExibirParametro( char * Simbolo ) ;


/***********************************************************************
*
*  $FC Função: LER  &Ler linha de parâmetros
*
*  $ED Descrição da função
*     Lê um número variável de parâmetros
*
*  $EP Parâmetros
*     $P Tipos   é um string contendo a identificação dos tipos dos parâmetros
*                a serem lidos.
*     $P ...     Os demais parâmetros são referências (ponteiros) para os
*                espaços onde deverão ser armazenados os valores lidos.
*
*     Tal como em scanf deve-se tomar o cuidado de assegurar que a lista
*     de referência e o string de tipos tenha o mesmo tamanho
*     e que os indicadores de tipo sejam consistentes com a referência
*     posicionalmente correspondentes.
*
*     Ao ler um campo, primeiro procura-se ler um nome de parâmetro já
*     declarado. Caso isto seja possível e o tipo esteja correto
*     é "lido" o valor contido no parâmetro.
*     Caso não seja possível, tenta ler um literal em acordo com o tipo
*     dado.
*
*     Gera mensagens de erro para parâmetros desconhecidos, sintaxe errada
*     e tipo inconsistente com o solicitado no correspondente caractere no
*     string Tipos.
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
*  $EP Parâmetros
*     $P Parm é um ponteiro para um caractere que receberá o valor lido
*
*  $FV Valor retornado
*     TRUE  - se leu
*     FALSE - se ocorreu algum erro
*
*  $FGP Tipos e Variáveis globais externas do próprio módulo
*     inxBuffer - é avançado para o primeiro caractere branco imediatamente
*                 a seguir do caractere lido
*
*  $EIU Interface com usuário pessoa
*     Um caractere deve vir entre aspas simples. Valem os caracteres de controle
*     \n \r \t \a \v \f \\ \' \" \? e \0
*     São geradas mensagens de erro no log
*
***********************************************************************/

   int LER_LerParmChar( char * Parm ) ;


/***********************************************************************
*
*  $FC Função: LER  &Ler parâmetro inteiro
*
*  $EP Parâmetros
*     $P Parm é um ponteiro para um int que receberá o valor lido
*
*  $FV Valor retornado
*     TRUE  - se leu
*     FALSE - se ocorreu algum erro
*
*  $FGP Tipos e Variáveis globais externas do próprio módulo
*     inxBuffer - é avançado para o primeiro caractere branco imediatamente
*                 a seguir do int lido
*
*  $EIU Interface com usuário pessoa
*     São geradas mensagens de erro no log
*
***********************************************************************/

   int LER_LerParmInt( int * Parm ) ;


/***********************************************************************
*
*  $FC Função: LER  &Ler parâmetro flutuante
*
*  $EP Parâmetros
*     $P Parm é um ponteiro para um double que receberá o valor lido
*
*  $FV Valor retornado
*     TRUE  - se leu
*     FALSE - se ocorreu algum erro
*
*  $FGP Tipos e Variáveis globais externas do próprio módulo
*     inxBuffer - é avançado para o primeiro caractere branco imediatamente
*                 a seguir do double lido
*
*  $EIU Interface com usuário pessoa
*     São válidas todas as formas de literal double aceitas pelo scanf.
*     São geradas mensagens de erro no log
*
***********************************************************************/

   int LER_LerParmDouble( double * Parm ) ;


/***********************************************************************
*
*  $FC Função: LER  &Ler parâmetro nome
*
*  $EP Parâmetros
*     $P Parm       é um ponteiro para um caractere que receberá o nome lido
*     $P tamString  é o tamanho do nome que foi lido
*     $P dimString  é o tamanho máximo que o nome poderá ter, já
*                   descontado o zero final
*
*  $FV Valor retornado
*     TRUE  - se leu
*     FALSE - se ocorreu algum erro
*
*  $FGP Tipos e Variáveis globais externas do próprio módulo
*     inxBuffer - é avançado para o primeiro caractere branco imediatamente
*                 a seguir do nome lido
*
*  $EIU Interface com usuário pessoa
*     São válidas as mesmas regras de nomes C.
*     São geradas mensagens de erro no log
*
***********************************************************************/

   int LER_LerParmNome( char * Parm , int * tamNome , int dimNome ) ;


/***********************************************************************
*
*  $FC Função: LER  &Ler parâmetro string
*
*  $EP Parâmetros
*     $P Parm       é um ponteiro para um caractere que receberá o string lido
*     $P tamString  é o tamanho do string que foi lido
*     $P dimString  é o tamanho máximo que o string poderá ter, já
*                   descontado o zero final
*
*  $FV Valor retornado
*     TRUE  - se leu
*     FALSE - se ocorreu algum erro
*
*  $FGP Tipos e Variáveis globais externas do próprio módulo
*     inxBuffer - é avançado para o primeiro caractere branco imediatamente
*                 a seguir do string lido
*
*  $EIU Interface com usuário pessoa
*     Um string deve vir entre aspas duplas.
*     Um string pode conter caracteres de controle
*     \n \r \t \a \v \f \\ \' \" \? e \0
*
*     São geradas mensagens de erro no log
*
***********************************************************************/

   int LER_LerParmString( char * Parm , int * tamString , int dimString ) ;


/***********************************************************************
*
*  $FC Função: LER  &Pular comando de teste
*
*  $ED Descrição da função
*     Avança o cursor do buffer de leitura por cima de um comando de teste.
*     Todos os comandos de teste principiam com o prefixo '='
*
***********************************************************************/

   void LER_PularComando( void ) ;

#undef LerParm_EXT

/********** Fim do módulo de definição: LER  Ler parâmetros de comandos **********/

#else
#endif
