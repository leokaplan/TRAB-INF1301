#if ! defined( CESPDIN_ )
#define CESPDIN_
/***************************************************************************
*
*  $MCD Módulo de definição: CED  Controlador de espaços de dados alocados dinamicamente
*
*  Arquivo gerado:              CESPDIN.h
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
*  $ED Descrição do módulo
*     O módulo de instrumentação CESPDIN disponibiliza um conjunto de
*     funções para
*     - controlar a extravasão de espaços de dados alocados ao usuário
*     - controlar o acesso a espaços já desalocados
*     - controlar a consistência entre o tipo implícito de um ponteiro e o
*       tipo do espaço por ele apontado
*     - controlar o vazamento de memória
*     - simular ter-se atingido o limite de memória disponível
*     - gerar aleatoriamente a falta de memória
*     - controlar a integridade da estrutura de espaços de dados dinâmicos
*     - exibir o conteçudo de espaços de dados em formato hexadecimal
*
*  $EIU Interface com o usuário pessoa
*     Para fazer com que um determinado módulo tenha todos os usos de
*     malloc, calloc, _msize e free controlados, basta incluir o módulo de
*     definição deste módulo. Idealmente use uma construção similar a:
*
*     #ifdef _DEBUG
*        #include "CESPDIN.H"
*     #endif
*
*     Isto fará com que todas as chamadas malloc e free do módulo controlado
*     sejam direcionadas para as funções equivalentes disponibilizadas
*     pelo presente módulo.
*
*     Para possibilitar a remoção automática da instrumentação
*     circunde todos os usos de funções desse módulo porventura existentes
*     no módulo a testar com o comando de compilação condicional
*
*     #ifdef _DEBUG
*        chamadas a funócões de CESPDIN
*     #endif
*
*     Se corretamente codificado a recompilação do módulo instrumentado
*     sem o nome _DEBUG definido deveria resultar em zero erros de
*     compilação.
*
*     OBSERVAÇÃO: Antes de recompilar, destrua o correspondente módulo objeto,
*     para que o make não o encontre durante a reconstrução do construto.
*
*     Para o correto funcionamento das funções desse módulo, é necessário
*     que o módulo CESPDIN contenha um comando similar a:
*
*     #pragma pack (1)
*
*     existente no Visual Studio C/C++. Esse comando introduz dependências
*     de plataforma, tornando não portátil o módulo CESPDIN.
*
*     Ao mover para outra plataforma de desenvolvimento, procure
*     documentação sobre a forma com que esta plataforma aloca espaços,
*     em particular como alinha os espaços. Após faça os ajustes necessários,
*     até no struct que contém o espaço efetivamente alocado (espaço
*     instrumentado) de modo que satisfaça as regras de alinhamento da
*     nova plataforma.
*
*     Em geral, os sintomas de mau funcionamento após a remoção de #pragma
*     são pequenas, de maneira geral circunscritas a extravasões de espaço
*     passarem despercebidas pela instrumentação.
*
***************************************************************************/
 
#if defined( CESPDIN_OWN )
   #define CESPDIN_EXT
#else
   #define CESPDIN_EXT extern
#endif


/***********************************************************************
*
*  $TC Tipo de dados: CED Modos de exibição
*
*
*  $ED Descrição do tipo
*     Enumera todos os modos de listar espaços alocados
*
***********************************************************************/

   typedef enum {

         CED_ExibirTodos    = 1 ,
               /* Exibir todos os espaços */

         CED_ExibirAtivos   = 2 ,
               /* Exibir todos os espaços ativos */

         CED_ExibirInativos = 3
               /* Exibir todos os espaços inativos */

   } CED_tpModoExibir ;


/***********************************************************************
*
*  $FC Função: CED  &Inicializar o controlador de alocação de espaços dinâmicos
*
*  $ED Descrição da função
*     Esta função inicializa o controlador de espaços em memória dinâmica.
*     A função deve ser chamada uma única vez, antes de qualquer outra função
*     do módulo.
*     Sugere-se que seja chamada ao iniciar o teste do módulo controlado.
*
*     OBS. O módulo de teste genérico faz esta inicialização sempre.
*
*  $EP Parâmetros
*     $P pArqLogParm - ponteiro para o arquivo log de teste. Não pode ser nulo.
*
***********************************************************************/

   void CED_InicializarControlador( void ) ;


/***********************************************************************
*
*  $FC Função: CED  &Alocar espaço malloc
*
*  $ED Descrição da função
*     Nos módulos controlados esta função instrumenta a função "malloc".
*     A instrumentação é transparente para o módulo controlado,
*     basta que inclua o módulo de definição CESPDIN.H.
*     Ou seja, se o módulo de definição "CESPDIN.H" for incluido,
*     a presente função será utilizada ao chamar "malloc" dentro de algum
*     módulo cliente. Não sendo incluido "CESPDIN.H", será chamada
*     diretamente a função "malloc" padrão de C.
*
*     O valor (parte útil do espaço alocado) é inicializado para o
*     caractere '|', permitindo a verificação de se o valor foi
*     corretamente inicializado ou não após à inicialização a ser realizada
*     pela função cliente.
*
*     Para assegurar transparência é definida a macro CED_MALLOC
*     que providencia os argumentos linha e arquivo fonte. Estes, por sua
*     vez, permitem identificar onde o espaço foi alocado, facilitando
*     resolver eventuais problemas de vazamento de memória
*     e/ou de uso incorreto de espaços.
*
*     A função aloca um "struct" contendo diversos controles além do
*     espaço útil (valor). O campo valor deste "struct" corresponde ao
*     espaço que seria alocado pela função de biblioteca "malloc".
*     O ponteiro retornado aponta para o campo valor, tornando
*     transparente o fato de se utilizar "malloc" ou esta função.
*     Cabe salientar que o comportamento anômalo com e sem sintrumentação
*     variam sensivelmente quando se instrumenta ou não as achamadas
*     a "malloc".
*
*     A função pode simular o esgotamento de memória. Para isto é necessário
*     estabelecer um limite através de uma das funções a isto destinadas.
*     Por exemplo, se o limite de espaço alocado for diferente de zero e o
*     total de espaço já alocado acrescido do espaço solicitado for maior
*     do que o limite estabelecido, a função não aloca espaço e retorna NULL,
*     simulando falta de memória.
*
*     A função pode simular esgotamento aleatório de memória. Para isto
*     é necessário estabelecer a freqüência de NULLs simulados, veja o
*     comando =limitarfrequencia em INTRPCED.C
*
*     Consulte as funções CED_Limitar... para detalhes das formas de
*     limite que podem ser estabelecidas.
*
*  $EP Parâmetros
*     $P Tamanho          - número de bytes do valor a ser alocado.
*     $P numLinhaFonte    - número da linha do código fonte onde foi
*                           chamada a função. Use __LINE__ como argumento
*                           da chamada.
*     $P NomeArquivoFonte - nome do arquivo fonte contendo a chamada.
*                           Use __FILE__ como argumento da chamada.
*
*  $FV Valor retornado
*     Ponteiro void para o espaço útil disponibilizado para a função
*     cliente. O espaço útil corresponde a o que seria retornado por malloc
*
*  $EIU Interface com usuário pessoa
*     Ao invés de utilizar a função CED_Malloc utilize a macro CED_MALLOC
*     uma vez que esta fornece automaticamente a identificação do local
*     em que foi realizada a chamada.
*
***********************************************************************/

   #define  CED_MALLOC( Tamanho ) \
               CED_Malloc( Tamanho , __LINE__ , __FILE__ )

   void * CED_Malloc( size_t Tamanho ,
                      int    numLinhaFonte ,
                      char * NomeArquivoFonte ) ;


/***********************************************************************
*
*  $FC Função: CED  &Alocar espaço todo zerado calloc
*
*  $ED Descrição da função
*     Aloca um espaço de Tamanho bytes todos iguais a \0.
*     Utiliza a função CED_Malloc. Veja a documentação desta.
*
***********************************************************************/

   #define  CED_CALLOC( Tamanho ) \
               CED_Calloc( Tamanho , __LINE__ , __FILE__ )

   void * CED_Calloc( size_t Tamanho ,
                      int    numLinhaFonte ,
                      char * NomeArquivoFonte ) ;


/***********************************************************************
*
*  $FC Função: CED  &Realocar espaço alocado realloc
*
*  $ED Descrição da função
*     A função realloc de C/C++ não é implementada pelo módulo CESPDIN.
*     De maneira geral também é recomendado que não seja utilizada,
*     uma vez que pode mudar o valor do ponteiro. Isso pode provocar
*     problemas caso um espaço seja referenciado por mais de um ponteiros.
*
*     A interface é similar à de CED_Malloc.
*
***********************************************************************/

   #define  CED_REALLOC( Tamanho ) \
               CED_Realloc( Tamanho , __LINE__ , __FILE__ )

   void * CED_Realloc( size_t Tamanho ,
                       int    numLinhaFonte ,
                       char * NomeArquivoFonte ) ;


/***********************************************************************
*
*  $FC Função: CED  &Desalocar espaço
*
*  $ED Descrição da função
*     Elimina o elemento da lista de controle. Todos os bytes são tornados
*     iguais a 'x'. O identificador do tipo do espaço também
*     é destruído, possibilitando verificar se ele ainda está alocado.
*
*     Ao usar esta função deve-se tomar o cuidado de nunca usar free para
*     desalocar um espaço controlado por este módulo.
*     Ou seja, evite o uso de malloc e free em módulos distintos em
*     que malloc está num módulo controlado e free não.
*     O mais seguro é ou ter todos os módulos que manipulam uma determinada
*     estrutura de dados sob controle, ou ter nenhum.
*
*  $EP Parâmetros
*     $P Ponteiro - ponteiro para o campo valor (espaço útil) do espaço
*                   a ser liberado
*
***********************************************************************/

   void CED_Free( void * Ponteiro ) ;


/***********************************************************************
*
*  $FC Função: CED  &Exibir conteúdo do espaço
*
*  $ED Descrição da função
*     Exibe o conteudo completo de um espaço de memória.
*     A parte útil é exibida em formato hexadecimal convencional
*
*  $EP Parâmetros
*     $P Ponteiro - ponteiro para o valor (parte útil) contido no espaço
*                   alocado, é o ponteiro retornado por CED_Malloc.
*
***********************************************************************/

   void CED_ExibirEspaco( void * Ponteiro ) ;


/***********************************************************************
*
*  $FC Função: CED  &Exibir todos os espaços
*
*  $ED Descrição da função
*     Exibe todos os espaços que satisfazem uma determinada regra.
*     Ver o tipo CED_tpModoExibir para uma explicação quanto às regras
*     de seleção de espaços a exibir.
*        CED_ExibirTodos     - exibe todos os espaços
*        CED_ExibirAtivos    - exibe os espaços marcados ativos
*        CED_ExibirInativos  - exibe os espaços marcados não ativos
*
***********************************************************************/

   void CED_ExibirTodosEspacos( CED_tpModoExibir Regra ) ;


/***********************************************************************
*
*  $FC Função: CED  &Definir tipo do espaço
*
*  $ED Descrição da função
*     Caso o tipo do espaço ainda não esteja definido, atribui o tipo.
*
*     O tipo do espaço é inicializado para CED_ID_TIPO_VALOR_NULO
*     quando da alocação com CED_Malloc. Posteriormente pode ser
*     alterado com a presente função.
*
*     A atribuição será realizada somente se uma das condições a seguir
*     for verdadeira:
*     - o tipo a ser atribuido é CED_ID_TIPO_VALOR_NULO
*     - ou o tipo do espaço é CED_ID_TIPO_VALOR_NULO
*
*     Para trocar o tipo de um espaço precisa-se primeiro torná-lo
*     igual a CED_ID_TIPO_VALOR_NULO para, depois, atribuir o novo tipo.
*
*     Uma vez definido o tipo do espaço, funções cliente podem controlar
*     a consistência entre o tipo implicitamente associado ao ponteiro
*     e o tipo que o espaço de fato contém.
*
*     Sugere-se que o desenvolvedor da aplicação crie uma tabela global
*     contendo um "enum" com os nomes de todos os tipos ('typedef struct')
*     que podem ser utilizados como declaradores de espaços dinâmicos.
*     O embrião da tabela existe em: arcabouc\tabelas\idTiposEspaco.def
*
*     O presente sistema de controle de tipos não prevê mecanismos para
*     tratamento de herança.
*
*  $EP Parâmetros
*     $P Ponteiro - ponteiro para o campo valor (espaço útil) do espaço
*                   a ser tipado
*     $P idTipo   - novo identificador de tipo a ser dado ao espaço.
*
*  $FV Valor retornado
*     TRUE   1    - se a atribuição foi bem sucedida
*     FALSE  0    - se não foi possível atribuir. Isto será o caso se
*                   o tipo do espáco já estava definido ao chamar a função,
*                   ou se o ponteiro for NULL.
*
***********************************************************************/

   int CED_DefinirTipoEspaco( void * Ponteiro , int idTipo ) ;


/***********************************************************************
*
*  $FC Função: CED  &Marcar ativo o espaço
*
*  $ED Descrição da função
*     Marca determinado espaço como ativo.
*
*     A marca ativo/não ativo é utilizada para apoiar o controle de
*     vazamento de memória. Siga o seguinte roteiro:
*     - Marque todos os espaços não ativos.
*     - A partir das âncoras, percorra as estruturas ativas marcando
*       ativos os espaços visitados. Este percorrimento é recursivo.
*       Retorna-se da chamada se encontrar um espaço já marcado ativo, ou
*       se não tiver para onde caminhar. Note que para poder fazer isto
*       é necessário conhecer-se o tipo dos dados contidos em cada um
*       dos espaços alocados.
*     - Percorra a lista de todos os espaços verificando se cada espaço
*       visitado é ou não inativo. Os não ativos provavelmente correspondem
*       a vazamento de memória.
*
*  $EP Parâmetros
*     $P Ponteiro - ponteiro para o campo valor (espaço útil) do espaço
*                   a ser marcado
*
***********************************************************************/

   void CED_MarcarEspacoAtivo( void * Ponteiro ) ;


/***********************************************************************
*
*  $FC Função: CED  &Marcar não ativo o espaço
*
*  $ED Descrição da função
*     Ver CED_MarcarEspacoAtivo
*
*  $EP Parâmetros
*     $P Ponteiro - ponteiro para o campo valor (espaço útil) do espaço
*                   a ser marcado
*
***********************************************************************/

   void CED_MarcarEspacoNaoAtivo( void * Ponteiro ) ;


/***********************************************************************
*
*  $FC Função: CED  &Marcar não ativos todos os espaços
*
*  $ED Descrição da função
*     Percorre todos os espaços alocados marcando-os inativos.
*
*     Veja: CED_MarcarEspacoAtivo
*
***********************************************************************/

   void CED_MarcarTodosEspacosInativos( ) ;


/***********************************************************************
*
*  $FC Função: CED  &Limitar memória disponível
*
*  $ED Descrição da função
*     Estabelece um limite de memória disponível.
*     A função CED_Malloc retornará NULL caso a adição do espaço solicitada
*     adicionada ao total útil já alocado ultrapassar o limite estabelecido.
*
*     Embora a função possa ser chamada a qualquer momento, recomenda-se
*     que seja chamada antes do primeiro caso de teste capaz de alocar
*     espaço de memória dinâmica por intermédio da função CED_Malloc
*     contida nesse módulo.
*
*  $EP Parâmetros
*     $P NovoLimiteMemoria - Estabelece o limite de memória disponível.
*                            Caso seja zero, o controle de limite não
*                            será efetuado por CED_Malloc.
*
***********************************************************************/

   void CED_LimitarMemoriaDisponivel( long NovoLimiteMemoria ) ;


/***********************************************************************
*
*  $FC Função: CED  &Limitar número de espaços alocados
*
*  $ED Descrição da função
*     Limita o número de espaços alocados. Esta função afeta o comportamento
*     de CED_Malloc, estabelecendo um limite para o número total de
*     espaços alocados em um dado momento.
*
*     Embora a função possa ser chamada a qualquer momento, recomenda-se
*     que seja chamada antes do primeiro caso de teste que provoque
*     alocação de espaço de memória.
*
*  $EP Parâmetros
*     $P numTotalEspacos - indica o limite de espaços que podem estar
*                          alocados a cada instante. Se zero, não
*                          será realizado o controle.
*
***********************************************************************/

   void CED_LimitarNumeroTotalEspacos( long numTotalEspacos ) ;


/***********************************************************************
*
*  $FC Função: CED  &Limitar com freqüência de geração de NULL
*
*  $ED Descrição da função
*     Esta função estabelece a freqüência com a qual não deverá ser
*     gerado NULL, o complemento a esta freqüência é a frequüência com
*     a qual deverá ser retornado NULL (não aloca) independentemente da
*     existência ou não de memória disponível.
*     A função visa apoiar a verificação de se o comportamento de um
*     programa leva em conta os retornos de falta de memória.
*
*  $EP Parâmetros
*     $P Frequencia - é um inteiro 10 <= Frequencia < 1000 e que indica
*                     a freqüência em cada 1000 alocações em que memória
*                     será alocada. Exemplo: para que 95% das vezes
*                     ocorra alocação, forneça o parâmetro 950, já se for
*                     desejado que ocorra em 99,9% das vezes, forneça
*                     999. Se zero, não será realizada a simulação de
*                     falta de memória.
*
*  $FV Valor retornado
*     TRUE   se tiver sido estabelecido o limite por freqüência
*     FALSE  se tiver ocorrido algum erro
*
***********************************************************************/

   int CED_LimitarFrequencia( int Frequencia ) ;


/***********************************************************************
*
*  $FC Função: CED  &Limitar, eliminar limites
*
*  $ED Descrição da função
*     Esta função elimina todos os limites de memória por ventura em vigor.
*
***********************************************************************/

   void CED_EliminarLimites( ) ;


/***********************************************************************
*
*  $FC Função: CED  &Obter o tipo do espaço de dados
*
*  $ED Descrição da função
*     Retorna o identificador do tipo do espaço apontado.
*
*  $EP Parâmetros
*     $P Ponteiro - ponteiro para o campo valor (espaço útil) do espaço
*                   do qual se deseja saber o tipo
*
*  $FV Valor retornado
*     Tipo do valor retornado, cajo tenha sido definido, ou um dos valores:
*
*        CED_ID_TIPO_ESPACO_NULO  se nunca foi atribuído um tipo ao espaço
*        CED_ID_TIPO_ILEGAL       se o espaço não corresponder a um espaço
*                                 controlado pelo módulo CESPDIN
*        CED_ID_TIPO_NULL         se o parâmetro Ponteiro for NULL
*
***********************************************************************/

   int CED_ObterTipoEspaco( void * Ponteiro ) ;


/***********************************************************************
*
*  $FC Função: CED  &Obter tamanho do valor contido no espaço
*
*  $ED Descrição da função
*     Retorna o número de bytes úteis do espaço
*
*  $EP Parâmetros
*     $P Ponteiro - ponteiro para a porção útil do espaço
*                   da qual se deseja saber o tamanho
*
***********************************************************************/

   int CED_ObterTamanhoValor( void * Ponteiro ) ;


/***********************************************************************
*
*  $FC Função: CED  &Obter tamanho do espaço alocado _msize
*
*  $ED Descrição da função
*     Implementa a função _msize disponível no Visual Studio C/C++
*
***********************************************************************/

   size_t CED_Msize( void * Ponteiro ) ;


/***********************************************************************
*
*  $FC Função: CED  &Obter número de espaços alocados
*
*  $ED Descrição da função
*     Retorna o número de elementos da lista de espaços alocados
*
***********************************************************************/

   int CED_ObterNumeroEspacosAlocados( ) ;


/***********************************************************************
*
*  $FC Função: CED  &Obter total de espaços alocados
*
*  $ED Descrição da função
*     Retorna o número total de vezes que foi chamada a função
*     CED_Malloc.
*
***********************************************************************/

   int CED_ObterTotalAlocacoes( ) ;


/***********************************************************************
*
*  $FC Função: CED  &Obter número total de NULLs gerados
*
*  $ED Descrição da função
*     Retorna o número de NULLs retornados em virtude de limitações impostas.
*     Veja as funções CED_LimitarXXX
*
***********************************************************************/

   long CED_ObterNumNULL( void ) ;


/***********************************************************************
*
*  $FC Função: CED  &Obter número de espaços segundo regra
*
*  $ED Descrição da função
*     Obtém o número de elementos contidos na lista de espaços alocados,
*     segundo a regra:
*        CED_ExibirTodos     - exibe todos os espaços
*        CED_ExibirAtivos    - exibe os espaços marcados ativos
*        CED_ExibirInativos  - exibe os espaços marcados não ativos
*
***********************************************************************/

   int CED_ObterNumeroEspacos( CED_tpModoExibir Regra ) ;


/***********************************************************************
*
*  $FC Função: CED  &Obter espaço total alocado
*
*  $ED Descrição da função
*     Obtém o total, em bytes, de espaços úteis alocados.
*
***********************************************************************/

   long CDE_ObterEspacoTotalAlocado( ) ;


/***********************************************************************
*
*  $FC Função: CED  &Iterador: iniciar iterador de espaços
*
*  $ED Descrição da função
*     Inicia o iterador com o primeiro espaço da lista de controle de espaços
*     dinâmicos.
*
*     O iterador pode ser utilizado para verificar a integridade dos
*     espaços de dados sob controle do módulo. Para isto a função de
*     verificação de espaços deve percorrer todos os espaços alocados
*     disparando o controle individual para cada um deles.
*
*     Pode-se também simular um garbage collector, percorrendo todos
*     os espaços e marcá-los inativos. Depois percorrer, a partir
*     das âncoras, todos os espaços adjacentes, marcando ativos os
*     espaços visitados. Evidentemente se já estiver marcado ativo
*     retorna-se para tratar do próximo estado pendente na pilha.
*     Ao final, os espaços que ainda estiverem marcados inativos
*     correspondem a vazamento de memória.
*
***********************************************************************/

   void CED_InicializarIteradorEspacos( ) ;


/***********************************************************************
*
*  $FC Função: CED  &Iterador: avançar para o próximo espaço
*
*  $ED Descrição da função
*     Avança o iterador para o próximo espaço.
*
*  $FV Valor retornado
*     TRUE  caso o espaco para o qual caminhou existe.
*     FALSE se não existe iterador ou chegou ao fim da lista de espaços
*
***********************************************************************/

   int CED_AvancarProximoEspaco( ) ;


/***********************************************************************
*
*  $FC Função: CED  &Iterador: obter referência para o espaço corrente
*
*  $ED Descrição da função
*     Obtém a referência para a parte útil do espaço corrente.
*
***********************************************************************/

   void * CED_ObterPonteiroEspacoCorrente( ) ;


/***********************************************************************
*
*  $FC Função: CED  &Iterador: existe espaço corrente
*
*  $ED Descrição da função
*     Verifica se o iterador referencia um espaço corrente.
*     Caso retorne FALSE, o iterador chegou ao final da lista de espaços
*     ou, então, ainda não foi inicializado.
*
***********************************************************************/

   int CED_ExisteEspacoCorrente( ) ;


/***********************************************************************
*
*  $FC Função: CED  &Iterador: terminar iterador
*
*  $ED Descrição da função
*     Desativa o iterador. A referência para o espaço corrente passa
*     a ser NULL.
*
***********************************************************************/

   void CED_TerminarIteradorEspacos( ) ;


/***********************************************************************
*
*  $FC Função: CED  &Iterador: excluir espaço corrente
*
*  $ED Descrição da função
*     Elimina o espaço corrente da lista de espaços controlados.
*     O espaço é liberado tornando ilegais todos os acessos realizados
*     por funções do usuário.
*     Na realidade esta função corresponde a um CED_Free.
*     Utilize esta função somente para para eliminar espaços sabidamente
*     inativos, ou para deturpar ao testar reações a anomalias em estruturas
*     de dados.
*
***********************************************************************/

   void CED_ExcluirEspacoCorrente( ) ;


/***********************************************************************
*
*  $FC Função: CED  &Verificar se espaço é ativo
*
*  $ED Descrição da função
*     Retorna TRUE se o espaço referenciado está marcado ativo
*
*  $EP Parâmetros
*     $P Ponteiro - ponteiro para o campo valor (espaço útil) do espaço
*                   do qual se deseja saber se é ativo ou não
*
***********************************************************************/

   int CED_EhEspacoAtivo( void * Ponteiro ) ;


/***********************************************************************
*
*  $FC Função: CED  &Verificar a integridade de um espaço de dados
*
*  $ED Descrição da função
*     Verifica a integridade de um determinado espaço de dados.
*     Retorna o número de falhas encontradas.
*     Esta função pode levar a um cancelamento do programa, caso o
*     espaço de dados esteja danificado, ou o ponteiro não aponte
*     para um espaço controlado.
*
*  $EP Parâmetros
*     $P Ponteiro para a porção útil do espaço. Este é o ponteiro
*                 retornado pela função CED_Malloc
*
*     $P int pVerificarValor( void * pValor )
*                 função fornecida pelo cliente para verificar a corretude
*                 do espaço útil. A função recebe como parâmetro um
*                 ponteiro para o espaço a verificar e deve retornar
*                      0  FALSE  caso o espaço não esteja correto e
*                   != 0  TRUE   se estiver correto.
*                 Todas as mensagens de erro devem ser dadas
*                 pela função de verificação. Procure utilizar as
*                 funções do módulo GENERICO para exibir as mensagens
*                 e realizar as comparações.
*                 Caso não deseje verificar o valor, forneça NULL como
*                 argumento.
*
*  $FV Valor retornado
*     TRUE   se espaço é válido ou o ponteiro for NULL
*     FALSE  se não for um espaço controlado pelo módulo CESPDIN, ou
*            se o conteúdo do espaço total não for válido
*
***********************************************************************/

   int CED_VerificarEspaco( void * Ponteiro ,
                            int ( * pVerificarValor )( void * pValor )) ;


/***********************************************************************
*
*  $FC Função: CED  &Verificar todos os espaços alocados
*
*  $ED Descrição da função
*     Percorre toda a lista, verificando um a um os espaços.
*     Caso um ou mais espaços estejam errados, pode ocorrer
*     cancelamento da execução em virtude de acesso ilegal a memória.
*
*  $EP Parâmetros
*     $P pVerificarValor - é o ponteiro para uma função que verifica o conteúdo
*                          do espaço útil. O parâmetro pValor dessa função
*                          apontará para o valor a ser controlado.
*                          Se NULL não verifica o espaço útil.
*
*  $FV Valor retornado
*     TRUE   se todos os espaços estão válidos
*     FALSE  caso contrário
*
***********************************************************************/

   int CED_VerificarTudo( int ( * pVerificarValor )( void * pValor )) ;

/***********************************************************************

/* Código final do módulo */

#if !defined( CESPDIN_OWN )
   #define    malloc     CED_MALLOC
   #define    calloc     CED_CALLOC
   #define    realloc    CED_REALLOC
   #define    free       CED_Free
   #define    _msize     CED_Msize
#endif

#undef CESPDIN_EXT

/********** Fim do módulo de definição: CED  Controlador de espaços de dados alocados dinamicamente **********/

#else
#endif
