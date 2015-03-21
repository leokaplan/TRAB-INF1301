#if ! defined( CONTA_ )
#define CONTA_
/***************************************************************************
*
*  $MCD Módulo de definição: CNT  Contadores de passagem
*
*  Arquivo gerado:              CONTA.h
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
*  $ED Descrição do módulo
*     Este módulo implementa funções para o controle de cobertura dos
*     testes de módulos. O controle de cobertura baseia-se
*     em contagem de passagens
*
*     Os contadores são identificados por nomes simbólicos (strings).
*     Para assegurar o controle efetivo, deve ser criado um arquivo
*     com todos os nomes de contadores definidos no módulo a testar.
*     Ver comando  =lercontadores  em INTRPCNT.C
*     Este arquivo deverá ser lido antes de iniciar os testes.
*     Ao final a tabela de símbolos pode ser examinada à procura de
*     contadores que não tenham sido contados (continuam comzero) durante
*     a execução do teste.
*
*     Os módulos a serem medidos devem ser marcados com chamadas para
*     a função de contagem disponibilizada pelo presente módulo:
*        CNT_Conta( <nome> ) ;
*     em que <nome> é um dos nomes que figura no arquivo de contagem.
*     Deve-se assegurar que todas as chamadas utilizem nomes diferentes
*     dos demais nomes utilizados em outras chamadas a esta função.
*
*     Os contadores podem ser inicializados com valores. Os seguintes
*     valores têm significado especial:
*        -1 - contador opcional. Caso o teste não passe pela correspondente
*             chamada à função de contagem, o sistema não acusará
*             erro de falta de cobertura.
*        -2 - contador proibido. Deve ser utilizado para marcar os
*             fragmentos de código que em uma execução correta normal
*             jamais devem ser executados. Estes fragmentos correspondem
*             tipicamente ao código de controle de integridade da execução.
*
*     Cada linha de texto do arquivo de declaração poderá ser um de:
*
*       // comentário - linha de comentário
*
*       linha em branco
*
*       linha de declaração, no formato:
*          nome\=numero  - definição de um contador.
*                          nome é o string que identifica o contador
*                          \= é o operador de inicialização, opcional
*                          numero é o valor inicial a ser atribuído
*                                 só pode ser fornecido se o operador \=
*                                 for fornecido
*
*     Os contadores podem ser acumulados em arquivos de totatilzação.
*     Isto permite controlar a cobertura dos testes quando forem
*     utilizadas várias massas (scripts) de teste.
*     O arquivo de totalização tem o mesmo formato que o de definição
*     de contadores.
*
*     Antes de iniciar a execuçãos dos testes deve ser lido o arquivo
*     de definição de contadores e, se for o caso, identificado
*     o arquivo de totalização.
*
*     Ao terminar os testes deve ser verificada a cobertura medida.
*     Caso esta não seja integral, as massas de teste devem ser
*     corrigidas de modo que a cobertura passe a ser integral.
*
*     Veja os comandos de teste
*        =inicializarcontadores
*        =lerarquivocontadores
*        =verificarcontagens
*        =terminarcontadores
*
*     definidos no módulo de interpretação de comandos de contagem
*        INTRPCNT
*
*  $EIU Interface com o usuário pessoa
*     Ao desenvolver módulos que deverão ser medidos siga as
*     seguintes instruções:
*
*     - Assegure que a função CNT_InicializarContadores seja
*       chamada antes de iniciar a medição
*
*     - Assegure que a função de declaração de contadores seja
*       lida antes de iniciar a medição
*
*     - Assegure que a função CNT_IniciarContagem seja chamada
*       ao iniciar a execução do código a ser medido
*
*     - Dependendo do modo de medir inclua no módulo a medir
*       fragmentos similares a
*          #ifdef _DEBUG
*             CNT_CONTAR( NomeContador ) ;
*          #endif
*       em todos os ligares em que deseje medir passagem de controle.
*
*     - Assegure que cada chamada a esta função referencie um
*       contador não referenciado pelas demais chamadas.
*
*     - Ao terminar a execução da porção de código a medir
*       chame a função CNT_PararContagem
*
*     - Antes de terminar a execução chame a função CNT_VerificarContagem
*       Esta função listará todos os contadores que contenham zero,
*       ou seja que não tenham figurado em uma chamada para
*       CNT_CONTAR no decorrer da execução.
*
*     Este processo pode ser modificado no caso de se querer realizar
*     outras formas de contagem.
*
***************************************************************************/
 
#if defined( CONTA_OWN )
   #define CONTA_EXT
#else
   #define CONTA_EXT extern
#endif


/***********************************************************************
*
*  $TC Tipo de dados: CNT Condições de retorno
*
*
***********************************************************************/

   typedef enum {

         CNT_CondRetOK = 0 ,
               /* CNT retornou normal */

         CNT_CondRetErro = -11 ,
               /* CNT Encontrou erro */

         CNT_CondRetNaoContador = -12 ,
               /* CNT Contador não existe */

         CNT_CondRetOpcional = -13 ,
               /* CNT Contador é opcional */

         CNT_CondRetProibido = -14 ,
               /* CNT Contador não pode ser contado */

         CNT_CondRetInicializado = -15 ,
               /* CNT Módulo contador já inicializado */

         CNT_CondRetNaoInicializado = -16 ,
               /* CNT Módulo contador não inicializado */

         CNT_CondRetNaoIterador = -17
               /* CNT Iterador lista ordenada não incializado */

   } CNT_tpCondRet ;


/***********************************************************************
*
*  $FC Função: CNT  &Inicializar contadores
*
*  $ED Descrição da função
*     Esta função inicializa uma seção de contagem.
*     Uma aplicação pode conter diversas seções de contagem mutuamente
*     independentes.
*     Esta função deve ser chamada exatamente uma vez no início de
*     cada seção.
*     Ao final da seção deve ser chamada a função
*        CNT_TerminarContadores
*
*  $EP Parâmetros
*     $P NomeArquivo - ponteiro para o nome do arquivo de contagem
*                   acumulada. Se NULL ou se não conseguir abrir,
*                   não existe arquivo de contagem acumulada.
*                   Sempre deve ser lido o arquivo de definição de
*                   contadores. Ao ler não serão alterados os valores
*                   de contagem porventura inicializados.
*                   Ao final da seção de contagem será gerado o arquivo
*                   de contagem acumulada, caso o parâmetro tenha sido
*                   diferente de NULL.
*
***********************************************************************/

   CNT_tpCondRet CNT_InicializarContadores( char * NomeArquivo ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Terminar contadores
*
*  $ED Descrição da função
*     Termina a seção de contagem.
*     Grava o arquivo de contagem acumulada caso tenha sido definido.
*
*     Veja CNT_InicializarContadores para mais detalhes.
*
***********************************************************************/

   CNT_tpCondRet CNT_TerminarContadores( ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Iniciar a contagem
*
*  $ED Descrição da função
*     Inicia a contagem. Enquanto esta função não tiver sido chamada,
*     não é realizada a contagem ao passar pela função CNT_Contar
*
*     Contagem pode ser parada com CNT_PararContagem
*
***********************************************************************/

   void CNT_IniciarContagem( ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Parar contagem
*
*  $ED Descrição da função
*     Para a contagem. Após este comando todas as chamadas da função
*     CNT_Contar realizam nada.
*     A contagem somente será iniciada ou retomada com uma chamada
*     da função CNT_IniciarContagem
*
***********************************************************************/

   void CNT_PararContagem( ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Registrar arquivo acumulador
*
*  $ED Descrição da função
*     Registra ou substitui o nome do arquivo acumulador registrado no
*     módulo contador
*
***********************************************************************/

   void CNT_RegistrarAcumulador( char * NomeArquivo ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Ler arquivo de definição de contadores
*
*  $FV Valor retornado
*     Retorna o número de erros de leitura reportado.
*     O formato do arquivo está descrito no cabeçalho do módulo.
*
***********************************************************************/

   int CNT_LerContadores( char * NomeArquivoDefinicao ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Gravar arquivo de contagem acumulada
*
*  $ED Descrição da função
*     Grava os contadores e respectivas contagens em um arquivo ASCII.
*     Pode ser utilizado para gerar arquivos contendo totais parciais.
*     O Arquivo de acumulação é definido ao inicializar o módulo contador
*     e é gravado ao terminar este módulo. Ver comandos:
*        =inicializarcontadores
*        =terminarcontadores
*     em INTRPCNT
*
***********************************************************************/

   int CNT_GravarContadores( char * NomeArquivo ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Contar
*
*  $ED Descrição da função
*     Adiciona 1 ao contador dado.
*     Se o contador continha -1 - atribui 1 a ele.
*     Se o contador continha -2 - emite uma mensagem de erro acusando
*           a execução de um fragmento de código proibido.
*
*     No módulo a medir devem ser inseridas chamadas para esta função em
*     todos os lugares em que interessa contar conforme a estratégia de
*     contagem utilizada.
*
*  $EP Parâmetros
*     $P NomeContador - é um string terminado em zero. Não pode
*                       conter o string "\=", uma vez que este sinaliza
*                       início de inicialização do contador.
*                       Não pode ser NULL.
*     $P numLinha     - o número da linha do código fonte em que foi
*                       chamada a função, use a constante __LINE__
*                       ou, entao, use a macro CNT_CONTAR
*
*  $FV Valor retornado
*     CNT_CondRetOK       se processou normal
*     CNT_CondRetProibido se tentou contar um contador marcado proibido,
*                         i.e. contagem inicializada para -2
*     CNT_CondRetNaoContador   se tentou contar um contador que não consta
*                         da tabela de contadores lida de arquivo
*                         ou se o contador contém um valor ilegal ( <= -3 )
*     CNT_CondRetNaoInicializado   se o módulo ainda não foi inicializado
*
*  $EIU Interface com usuário pessoa
*     Sinaliza erro na saída se
*        Nome do contador não existe
*        Se for tentado contar um contador proibido
*
*     As mensagens indicam a causa e o número da linha de código fonte
*     que contém a chamada para a função CNT_Contar
*
***********************************************************************/

   #define CNT_CONTAR( NomeContador )  \
           CNT_Contar( NomeContador , __LINE__ )

   CNT_tpCondRet CNT_Contar( char * NomeContador , int numLinha ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Verificar contagem
*
*  $ED Descrição da função
*     Verifica se existe algum contador com contagem igual a zero
*     (nunca foi contado). Emite uma mensagem para cada contador
*     zero encontrado.
*     Retorna o número de erros reportados.
*       -1 se ainda não incializado
*
***********************************************************************/

   int CNT_VerificarContagem( ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Obter valor de contagem
*
*  $ED Descrição da função
*     Retorna CNT_CondRetNaoContador se o contador não existe
*     Obs. o valor de todas as condições de retorno são negativas.
*
***********************************************************************/

   long CNT_ObterContagem( char * NomeContador ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Obter número de contadores
*
***********************************************************************/

   int CNT_ObterNumeroContadores( ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Obter total de contagem
*
***********************************************************************/

   long CNT_ObterContagemTotal( ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Iterador: obter contagem corrente
*
*  $ED Descrição da função
*     Retorna a contagem associada com o contador corrente do iterador
*
*     Retorna CNT_CondRetNaoIterador caso este não tenha sido iniciado
*     ou já tenha percorrido todos os contadores
*
***********************************************************************/

   long CNT_ObterContagemCorrente( ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Iterator: obter nome de contador corrente
*
*  $FV Valor retornado
*     Retorna o ponteiro para o nome do contador.
*     Jamais altere o nome pois o contador se tornaria inacessível.
*     Retorna NULL se o iterador não está ativo.
*
***********************************************************************/

   char * CNT_ObterContadorCorrente( ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Iterador: avançar para o próximo contador
*
***********************************************************************/

   void CNT_IrProximoContador( ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Iterator: iniciar percorrimento da lista em ordem alfabética
*
*  $ED Descrição da função
*     Inicializa ou reinicializa o iterador que percorre a lista ordenada.
*     A qualquer momento no máximo um um iterador estará ativo.
*
*     O iterador pode ser utilizado para pecorrer toda a lista de
*     contadores.
*
***********************************************************************/

   void CNT_IniciarIterador( ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Iterator: verificar se o iterador está ativo
*
*  $FV Valor retornado
*     TRUE  se estiver ativo
*     FALSE se o módulo não estiver inicializado ou o iterador já tiver
*           chegado ao final da lista
*
***********************************************************************/

   int CNT_EhAtivoIterador( ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Zerar todos contadores
*
***********************************************************************/

   CNT_tpCondRet CNT_ZerarContadores( ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Zerar contador dado
*
***********************************************************************/

   CNT_tpCondRet CNT_ZerarContador( char * NomeContador ) ;


/***********************************************************************
*
*  $FC Função: CNT  &Verificar a estrutura dos contadores
*
*  $ED Descrição da função
*     Verifica a integridade da estrutura de dados do módulo contador
*
*  $FV Valor retornado
*     Retorna o número de erros estruturais encontrados. Este número
*     não corresponde à totalidade de erros, uma vez que o processamento
*     pode ser interrompido dependendo da severidade do erro encontrado.
*
***********************************************************************/

#ifdef _DEBUG

   int CNT_VerificarEstruturaContadores( ) ;

#endif

#undef CONTA_EXT

/********** Fim do módulo de definição: CNT  Contadores de passagem **********/

#else
#endif
