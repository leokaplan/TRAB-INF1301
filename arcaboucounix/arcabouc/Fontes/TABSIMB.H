#if ! defined( TABSIMB_ )
#define TABSIMB_
/***************************************************************************
*
*  $MCD Módulo de definição: TBS  Tabela de símbolos
*
*  Arquivo gerado:              TABSIMB.h
*  Letras identificadoras:      TBS
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
*     Implementa tabelas de símbolos genéricas, i.e. que contém dados
*        de qualquer tipo.
*     Podem existir n tabelas simultaneamente.
*     Cada tabela é homogênea quanto ao tipo dos dados que armazena.
*     Cada dado contém o símbolo e o correspondente valor.
*     Para a tabela de símbolos é irrelevante a estrutura dos dados
*        armazenados, ou seja não é necessário conhecer o tipo específico.
*     A tabela não se preocupa com possível ordenação dos símbolos
*        contidos nos dados.
*     Os ponteiros para os dados são copiados para dentro da tabela.
*        Não é copiado o valor apontado por estes ponteiros.
*     Os dados passam a pertencer à tabela, ou seja, caso um símbolo seja
*        excluído, será destruído o correspondente dado.
*     Cada tabela referencia uma função que determina como devem
*        ser acessados os símbolos contidos nos dados nela contidos.
*     A função de obtenção do símbolo contido em um dado é dependente do tipo
*        deste dado. Conseqüentemente a tabela passa a ser implicitamente
*        tipada com o tipo correspondente aos dados que contém. Um uso
*        incorreto dos tipos pode provocar erro de execução (cancelamento
*        da execução por acesso a endereços - ponteiros - ilegais)
*     Cada tabela referencia uma função que determina como devem ser
*        desalocados os dados nela contidos.
*        Isto permite armazenar dados compostos que contenham ponteiros
*        para espaços que devam ser desalocados. Evita-se, assim, o
*        vazamento de memória.
*     A função de liberação de espaços de elementos deve assegurar a
*        liberação de todos os espaços de um elemento. Esta função
*        é chamada antes de se desalocar o espaço. Caso não seja
*        necessário desalocar espaços referenciados pelo dado, o ponteiro
*        para esta função de liberação poderá ser NULL .
*
***************************************************************************/
 
#if defined( TABSIMB_OWN )
   #define TABSIMB_EXT
#else
   #define TABSIMB_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

#ifdef _DEBUG
   #include "GENERICO.H"
#endif

typedef struct TBS_tagTabela * TBS_tppTabela ;


/***********************************************************************
*
*  $TC Tipo de dados: TBS Condições de retorno
*
*
*  $ED Descrição do tipo
*     Condições de retorno das funções da tabela de símbolos
*
***********************************************************************/

   typedef enum {

         TBS_CondRetOK ,
               /* Concluiu corretamente */

         TBS_CondRetSimboloExiste ,
               /* Símbolo já existe */

         TBS_CondRetSimboloNaoExiste
               /* Símbolo não existe */

   } TBS_tpCondRet ;


/***********************************************************************
*
*  $FC Função: TBS  &Criar tabela de símbolos
*
*  $ED Descrição da função
*     Cria uma tabela de símbolos que armazenará símbolos de determinado
*     tipo. Os possíveis tipos são desconhecidos a priori. A tipagem
*     é implicita. Não existe identificador de tipo associado à tabela.
*
*  $EP Parâmetros
*     tamVetor  - tamanho do vetor de randomização. Procure sempre
*                 escolher um número primo. Isto tende a otimizar a
*                 randomização.
*
*     ObterSimbolo - ponteiro para função que retorna um ponteiro
*                    para o string do símbolo contido na estrutura de dados
*                    fornecida por parâmetro
*                 Protótipo da função :
*
*                       char * ObterSimbolo( void * pDado )
*
*                 A função dever retornar:
*                    ponteiro para o string contido em dado
*
*                 A função não realiza controle de existência do string.
*                 Como o tipo dos dados é indefinido (void), este poderá ser
*                 qualquer estrutura tão complexa quanto se queira.
*                 No entanto, qualquer símbolo será sempre um string
*                 terminado em zero.
*
*     LiberarDado  - ponteiro para função que libera os espaços apontados
*                    pelo dado referenciado por um elemento da tabela de
*                    símbolos. Caso não existam tais ponteirospode-se
*                    passar o argumento NULL.
*
*  $FV Valor retornado
*     Se executou corretamente retorna o ponteiro para a tabela de
*     símbolos criada.
*     Este ponteiro deve ser utilizado pelas funções de manipulação de
*     símbolos e da própria tabela.
*
*     Se ocorreu algum erro, por exemplo falta de memória ou dados errados,
*     a função retornará NULL. Não será feita qualquer discriminação quanto
*     ao tipo de problema ocorrido.
*
***********************************************************************/

   TBS_tppTabela TBS_CriarTabela(
             int tamVetor                               ,
             char * ( * ObterSimbolo ) ( void * pDado ) ,
             void   ( * LiberarDado  ) ( void * pDado )  ) ;


/***********************************************************************
*
*  $FC Função: TBS  &Destruir tabela de símbolos
*
*  $ED Descrição da função
*     Destrói a tabela fornecida. O ponteiro para a tabela não é modificado.
*     Se ocorrer algum erro durante a destruição, a tabela resultará
*     estruturalmente incorreta. OBS. não existe previsão para possíveis
*     falhas de execução.
*
*  $FV Valor retornado
*     TBS_CondRetOK    - destruiu sem problemas
*
***********************************************************************/

   void TBS_DestruirTabela( TBS_tppTabela pTabela ) ;


/***********************************************************************
*
*  $FC Função: TBS  &Validar tabela de símbolos
*
*  $ED Descrição da função
*     Valida a estrutura da tabela. A função verifica todas as
*     assertivas estruturais da tabela de símbolos
*
*     Esta função depende da existência do arcabouço de teste.
*     Será compilada se e somente se o nome _DEBUG estiver definido.
*
*  $EP Parâmetros
*     pTabela - ponteiro para a tabela a verificar
*
*  $FV Valor retornado
*     TST_CondRetOK   - se a estrutura está correta
*     TST_CondRetErro - se estiver incorreta
*
***********************************************************************/

#ifdef _DEBUG

   TST_tpCondRet TBS_ValidarTabela( TBS_tppTabela pTabela ) ;

#endif


/***********************************************************************
*
*  $FC Função: TBS  &Inserir símbolo
*
*  $ED Descrição da função
*     Insere um dado contendo um símbolo na tabela dada.
*     É inserido o ponteiro, não a cópia do dado. Caso o elemento da
*     tabela seja destrído, o correspondente dado também o será.
*     No entanto, caso o dado contenha referências, estas não serão
*     processadas ao destruir.
*
*     Limitações:
*     - símbolos devem sempre ser strings terminados em zero.
*     - as funções internas são sensíveis à caixa e à existeência
*       de caracteres diacríticos.
*     - o dado contendo o símbolo passa a ser controlado pela tabela.
*       Não é feita cópia dele ao inserir.
*     - dados não podem conter ponteiros para outros espaços, exceto no
*       caso de nem a tabela nem elementos individuais jamais serem
*       destruídos durante a execução do programa.
*     - caso a função não consiga inserir o dado (o símbolo contido
*       no dado já existe na tabela), cabe à função cliente destruir
*       a estrutura de dados enviada. Caso isto não seja feito, poderá
*       ocorrer vazamento de memória.
*
*  $EP Parâmetros
*     pTabela - ponteiro para a tabela onde deve ser inserido o símbolo
*     pDado   - ponteiro para o dado que contém o símbolo. O tipo do
*               dado deve ser consistente com o tipo registrado na tabela
*
*  $FV Valor retornado
*     TBS_CondRetOK            - inseriu um símbolo novo
*     TBS_CondRetSímboloExiste - se não inseriu por já existir
*
***********************************************************************/

   TBS_tpCondRet TBS_InserirSimbolo( TBS_tppTabela pTabela ,
                                     void * pDado           ) ;


/***********************************************************************
*
*  $FC Função: TBS  &Procurar símbolo
*
*  $ED Descrição da função
*     Procura o símbolo dado
*
*  $EP Parâmetros
*     pTabela  - ponteiro para a tabela onde procura
*     pSimbolo - ponteiro para um string (símbolo) a procurar
*
*  $FV Valor retornado
*     Se encontrou: ponteiro para o dado correspondente ao símbolo
*                   o ponteiro será consistente com o tipo registrado
*                   na tabela. Exceto o atributo símbolo, qualquer outro
*                   atributo desta estrutura poderá ser alterado
*                   externamente à tabela de símbolos
*     Se não encontrou: NULL
*
***********************************************************************/

   void * TBS_ProcurarSimbolo( TBS_tppTabela pTabela ,
                               char * pSimbolo         ) ;


/***********************************************************************
*
*  $FC Função: TBS  &Excluir símbolo
*
*  $ED Descrição da função
*     Exclui o símbolo dado da tabela dada.
*
*  $EP Parâmetros
*     pTabela    - ponteiro para a tabela onde deve excluir.
*     pSimbolo   - ponteiro para o simbolo correspondente ao dado
*                  a ser eliminado da tabela
*
*  $FV Valor retornado
*     TBS_CondRetOK               - se foi excluído
*     TBS_CondRetSimboloNaoExiste - se o símbolo já não existia na tabela
*
*  $DE Observações
*     Note que o dado contido na tabela será destruído e que não é
*     feita cópia ao inseri-lo na tabela.
*
***********************************************************************/

   TBS_tpCondRet TBS_ExcluirSimbolo( TBS_tppTabela pTabela ,
                                     char * pSimbolo        ) ;

#undef TABSIMB_EXT

/********** Fim do módulo de definição: TBS  Tabela de símbolos **********/

#else
#endif
