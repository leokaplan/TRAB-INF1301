#if ! defined( GENERICO_ )
#define GENERICO_
/***************************************************************************
*
*  $MCD Módulo de definição: TSTG Controlador de teste genérico
*
*  Arquivo gerado:              GENERICO.h
*  Letras identificadoras:      TST
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
*     O módulo de teste genérico contém um conjunto de funções de
*     coordenação, apoio e controle da execução de testes de módulos
*     quaisquer.
*
*     Embora na prática a função de teste específica seja uma extensão da
*     função de teste genérica, a função de teste específica encontra-se em
*     outro módulo. Desta forma as alterações necessárias para cada
*     módulo a testar concentram-se exclusivamente naquele módulo.
*
*  $EIU Interface com o usuário pessoa
*     Os seguintes comandos genéricos estão definidos:
*
*     linha em branco - faz nada
*
*     //      - comentário, é impresso no LOG
*
*     ==      - título de caso de teste, o título é impresso no LOG
*
*     =recuperar
*             - Caso o comando imediatamente anterior ao comando
*               =recuperar retorne TST_CondRetOK, obviemanete o comando
*               executou corretamente, portanto o comando =recuperar
*               reportará uma falha no script de teste, uma vez que não
*               foi encontrada a falha esperada ao executar o comando.
*             - Caso o comando antecessor retorne TST_CondRetErro,
*               ou TST_CondRetParm (erro de parâmetros do comando)
*               a falha reportada será desconsiderada, uma vez que
*               era esperada. O comando =recuperar destina-se a
*               apoiar o teste da corretude dos validadores de
*               parâmetros dos comandos e das funções por eles
*               ativadas.
*             - caso o comando antecessor retorne outras condições, o
*               comando =recuperar será tratado como falha.
*
*     =bkpt   - permite interagir com o depurador utilizado para
*               desenvolver os módulos sob teste. O comando =bkpt
*               executa um código "dummy" no qual pode ser inserido
*               um "breakpoint" do depurador. Desta forma o teste pode
*               prosseguir sem interrupção até o comando =bkpt.
*               Para isto
*               - apague ou desative todos os breakpoints do depurador
*               - insira o comando =bkpt no script pouco antes do ponto
*                 de interrupção desejado
*               - usando o depurador, estabeleça um breakpoint na linha de
*                 código: "Dummy = 0 ;" contida no módulo de teste
*                 genérico. Esta linha pode ser encontrada
*                 procurando por "BKPT_CMD"
*               - dê partida no programa e espere até chegar ao breakpoint
*                 associado ao comando "dummy". Agora reative ou instale
*                 os breakpoints requeridos pela a sessão de depuração.
*                 e continue depurando
*
*     =cancelar
*             - termina a interpretação dos comandos de teste.
*               Sempre sinaliza falha.
*
*     =AceitaFalhasAcumuladas  numFalhas
*             - caso o contador de falhas registre numFalhas, ele será
*               tornado zero. Este comando é similar a =recuperar.
*               Difere deste por ser executado desde que o comando
*               anterior não tenha reportado erro, tal como acontece
*               com todos os comandos, exceto =recuperar.
*               O comando permite recuperar falhas sinalizadas por
*               módulos de apoio, tais como, leitor de parâmetros,
*               contador de passagens e controle de espaço dinâmico.
*               Caso o contador de número de falhas contenha um valor
*               diferente de numFalhas, será reportado um erro e
*               o contador é incrementado de 1.
*
*     Os demais comandos são passados para as funções de teste específico.
*     Primeiro para as do arcabouço, depois para a específica do módulo
*     a testar. A última função de teste específico deve retornar a condição
*     TST_CondRetNaoConhec caso não consiga interpretar a diretiva.
*
*  $EIM Interface com outros módulos
*     A primeira função do arcabouço ("framework") a ser chamada deve a
*     sempre ser TST_InicializarTeste. Caso não o seja, ocorrerão erros
*     de execução (assert).
*
*     Após abra o arquivo de script executando a função
*     TST_AbrirArquivoScript. Se for desejado gerar um log abra
*     o arquivo corresponente com a função TST_AbrirArquivoLog.
*
*     Depois de abrir os arquivos, execute os testes chamando a função
*     TST_RealizarTeste. A função de teste genérico lerá todo o arquivo
*     de script, retornando após concluir o teste.
*
*     Após, termine o teste através da função TST_TerminarTeste.
*     Esta função exibe as estatísticas do teste e, caso solicitado,
*     acumula essas estatísticas em um arquivo acumulador (ver
*     parâmetro /a do programa principal.
*
*     As funções "TST_ObterXXX" podem ser utilizadas antes de fechar o
*     módulo de teste para exibir o resumo do teste no módulo cliente do
*     módulo de controle genérico.
*
*     O programa cliente do módulo controle de teste genérico pode
*     efetuar as operações acima repetidas vezes, permitindo, assim o
*     teste envolvendo diversos arquivos scripts de teste. A saída
*     de cada teste deve ser dirigida para um log específico, ou para
*     stdout, uma vez que, ao abrir um arquivo log, o seu conteúdo será
*     sempre destruído.
*
*     O módulo de teste genérico também implementa diversas funções
*     de comparação "TST_CompararXXX" use-as no módulo de controle de
*     teste específico. Desta forma será assegurado o tratamento uniforme
*     de todas as falhas possivelmente encontradas.
*
*     O módulo disponibiliza ainda a função TST_ExibirPrefixo
*     que gera uma linha de saída obedecendo aos padrões de interface
*     estabelecidos. Sendo desejado pode-se adicionar textos à linha
*     usando a funcao fprintf( TST_ObterArqLog( ) , ... ). Um exemplo da
*     utilidade disso é quando se deseja imprimir informação complementar
*     ao interpretar uma determinada diretiva. A função TST_ObterArqLog( )
*     retorna a referência ao arquivo log em uso no momento.
*
*     Finalmente, o módulo disponibiliza a função TST_NotificarFalha
*     que incrementa o contador de falhas além de emitir uma mensagem
*     de erro.
*
***************************************************************************/
 
#if defined( GENERICO_OWN )
   #define GENERICO_EXT
#else
   #define GENERICO_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

#include "TST_ESPC.H"


/***********************************************************************
*
*  $FC Função: TSTG &Controlar uma sessão de teste
*
*  $ED Descrição da função
*     Esta função é o ponto de entrada para a realização do teste.
*     Permite que se adicione o arcabouço de teste a um sistema qualquer.
*
*  $EP Parâmetros
*     $P NomeConstrutoParm
*     $P NomeArqScriptParm - deve ser não nulo
*     $P NomeArqLogParm    - se for nulo, gera a saída na console
*     $P NomeArqAcumParm   - se não for nulo, acumula as estatísticas no
*                            arquivo denominado
*
*  $FV Valor retornado
*     0 - se o teste ocorreu OK
*     4 - se foram observados erros de dados ou falhas nos testes
*
***********************************************************************/

   int TST_ControlarSessaoTeste( char * NomeConstrutoParm ,
                                 char * NomeArqScriptParm ,
                                 char * NomeArqLogParm    ,
                                 char * NomeArqAcumParm    ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Comparar booleano
*
*  $ED Descrição da função
*     Compara valor booleano esperado com valor booleano obtido,
*     reportamdo uma falha caso não sejam iguais.
*     Note que "!= 0" corresponde a TRUE e "== 0" corresponde a FALSE
*
*  $EP Parâmetros
*     ValorEsperado  - é um valor fornecido como parâmetro do comando
*                      de teste
*     ValorObtido    - é o valor retornado pela função em teste
*     pMensagem      - mensagem explanatória da falha encontrada
*
*  $FV Valor retornado
*     TST_CondRetErro  se o valor esperado não for consistente com o obtido
*     TST_CondRetOK    se os valores forem consistentes
*
***********************************************************************/

   TST_tpCondRet TST_CompararBool( int    ValorEsperado ,
                                   int    ValorObtido   ,
                                   char * pMensagem      ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Comparar caractere
*
*  $ED Descrição da função
*     Compara um caractere esperado com um caractere obtido, emitindo mensagem de
*     erro caso não sejam iguais.
*
*  $EP Parâmetros
*     ValorEsperado  - é um valor fornecido como parâmetro do comando
*                      de teste
*     ValorObtido    - é o valor retornado pela função em teste
*     pMensagem      - mensagem explanatória da falha encontrada
*
*  $FV Valor retornado
*     TST_CondRetErro  se o valor esperado não for igual ao obtido
*     TST_CondRetOK    se os valores forem iguais
*
***********************************************************************/

   TST_tpCondRet TST_CompararChar( char   ValorEsperado ,
                                   char   ValorObtido   ,
                                   char * pMensagem       ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Comparar inteiro
*
*  $ED Descrição da função
*     Compara valor inteiro long esperado com valor inteiro obtido,
*     reportamdo uma falha caso não sejam iguais.
*
*  $EP Parâmetros
*     ValorEsperado  - é um valor fornecido como parâmetro do comando
*                      de teste
*     ValorObtido    - é o valor retornado pela função em teste
*     pMensagem      - mensagem explanatória da falha encontrada
*
*  $FV Valor retornado
*     TST_CondRetErro  se o valor esperado não for igual ao obtido
*     TST_CondRetOK    se os valores forem iguais
*
***********************************************************************/

   TST_tpCondRet TST_CompararInt( long   ValorEsperado ,
                                  long   ValorObtido   ,
                                  char * pMensagem       ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Comparar flutuante
*
*  $ED Descrição da função
*     Compara valor flutuante esperado com valor flutuante obtido,
*     reportando uma falha caso
*     1 - Tolerancia > esperado / obtido ou
*     1 + Tolerancia < esperado / obtido
*
*  $EP Parâmetros
*     ValorEsperado  - é um valor fornecido como parâmetro do comando
*                      de teste
*     ValorObtido    - é o valor retornado pela função em teste
*     Tolerância     - indica os limites da relação entre esperado e obtido.
*                      Deve ser um valor positivo, menor do que 1.
*                      Use um valor do tipo 10 ** -n, no qual n é o
*                      número de dígitos significativos desejado.
*     pMensagem      - mensagem explanatória da falha encontrada
*
*  $FV Valor retornado
*     TST_CondRetErro  se o valor obtido não estiver dentro do domínio da
*                      tolerância em torno do valor esperado
*     TST_CondRetOK    se os valores estiverem dentro da tolerância
*
***********************************************************************/

   TST_tpCondRet TST_CompararFloat( double   ValorEsperado ,
                                    double   ValorObtido   ,
                                    double   Tolerancia    ,
                                    char *   pMensagem      ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Comparar string
*
*  $ED Descrição da função
*     Compara string esperado com string obtido, reportamdo uma falha
*     caso não sejam iguais. Ambos os strings devem obedecer o
*     padrão C, terminando em zero.
*
*  $EP Parâmetros
*     ValorEsperado  - é um string fornecido como parâmetro do comando
*                      de teste.
*     ValorObtido    - é o string retornado pela função em teste
*     pMensagem      - mensagem explanatória da falha encontrada
*
*  $FV Valor retornado
*     TST_CondRetErro  se o valor esperado não for igual ao obtido
*     TST_CondRetOK    se os valores forem iguais
*
***********************************************************************/

   TST_tpCondRet TST_CompararString( char * ValorEsperado ,
                                     char * ValorObtido   ,
                                     char * pMensagem       ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Comparar espaço
*
*  $ED Descrição da função
*     Compara espaços de dados, reportando uma falha caso não sejam iguais.
*     Esta função pode ser utilizada para comparar qualquer coisa,
*     inclusive valores do tipo  "struct"
*     O tipo do valor é indefinido e os valores serão exibidos em
*     formato hexadecimal, caso não sejam iguais
*
*  $EP Parâmetros
*     ValorEsperado  - ponteiro para o espaço que contém o valor esperado
*     ValorObtido    - ponteiro para o espaço que contém o valor obtido
*     TamEspaco      - tamanho do espaço a ser comparado
*     pMensagem      - mensagem explanatória da falha encontrada
*
*  $FV Valor retornado
*     TST_CondRetErro  se o valor esperado não for igual ao obtido
*     TST_CondRetOK    se os valores forem iguais
*
***********************************************************************/

   TST_tpCondRet TST_CompararEspaco( void * ValorEsperado ,
                                     void * ValorObtido   ,
                                     int    TamEspaco     ,
                                     char * pMensagem       ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Comparar ponteiro
*
*  $ED Descrição da função
*     Compara dois ponteiros emitindo uma mensagem caso não sejam iguais
*
*  $FV Valor retornado
*     TST_CondRetErro  se os dois ponteiros forem diferentes
*     TST_CondRetOK    se forem iguais
*
***********************************************************************/

   TST_tpCondRet TST_CompararPonteiro( void * PonteiroEsperado ,
                                       void * PonteiroObtido   ,
                                       char * pMensagem       ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Comparar ponteiro nulo
*
*  $ED Descrição da função
*     Verifica
*       - quando ModoEsperado == 0 se o PonteiroObtido é nulo
*       - quando ModoEsperado != 0 se o PonteiroObtido é não nulo
*
*  $FV Valor retornado
*     TST_CondRetErro  se os dois ponteiros forem diferentes
*     TST_CondRetOK    se forem iguais
*
***********************************************************************/

   TST_tpCondRet TST_CompararPonteiroNulo( int    ModoEsperado ,
                                           void * PonteiroObtido   ,
                                           char * pMensagem       ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Assertiva controlada
*
*  $ED Descrição da função
*     Esta função substitui a função assert de C.
*     É utilizada no contexto do arcabouço de teste.
*     Ela imprime mensagens mais visíveis tanto no log como na
*     janela de comando.
*
*     Recomenda-se, no entanto, utilizar a função assert de C em aplicações.
*
*  $FV Valor retornado
*     Termina a execução do programa com código de retorno == 4
*
***********************************************************************/

   #define  TST_ASSERT( Expressao ) \
      TST_Assert( Expressao , __LINE__ , __FILE__ )

   void TST_Assert( int Expressao , int Linha , char * NomeArq ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Exibir prefixo da mensagem
*
*  $ED Descrição da função
*     Imprime o prefixo de mensages de falha ou de informação.
*     Não incrementa o contador de falhas.
*     Esta função estabelece o padrão de saída das mensagens geradas
*     durante os testes.
*
*  $EP Parâmetros
*     $P Indicador - informa a natureza da mensagem,
*                    Utilize preferenciamente as constantes definidas em
*                    TST_ESPEC.H:
*                       SINALIZA_ERRO       ">>>"
*                       SINALIZA_RECUPERA   "<<<"
*                       SINALIZA_COMENTARIO "!!!"
*     $P pMensagem - é a mensagem prefixo a ser gerada.
*
*  $EIU Interface com usuário pessoa
*     Podem ser acrescentadas mensagens ao prefixo. A função
*     TST_ObterArqLog retorna o ponteiro para o arquivo log em uso
*     (stdout ou definido pelo usuário)
*
***********************************************************************/

   void TST_ExibirPrefixo( char * Indicador , char * pMensagem ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Notificar falha
*
*  $ED Descrição da função
*     Exibe uma mensagem de falha no formato padrão do arcabouço.
*     A função não exibe os valores esperado e obtido.
*
*  $EP Parâmetros
*     $P pMensagem - mensdagem explicativa da falha.
*
*  $FV Valor retornado
*     TST_CondRetErro  sempre
*
***********************************************************************/

   TST_tpCondRet TST_NotificarFalha( char * pMensagem ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Incrementar contador de falhas
*
*  $ED Descrição da função
*     Esta função deve ser usada quando, por alguma razão, precisar-se
*     utilizar TST_ExibirPrefixo para exibir uma mensagem de falha.
*     As outras funções (comparação, TST_NotificarFalha, etc.)
*     incrementam o contador automaticamente.
*
***********************************************************************/

   void TST_ContarFalhas( ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Obter número de casos de teste efetuados
*
*  $ED Descrição da função
*     Retorna o número de casos de teste lidos.
*     Cada caso de teste inicia com o comando "== <nome do caso de teste>".
*
***********************************************************************/

   int TST_ObterNumCasosTeste( ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Obter número de comandos específicos corretamente interpretados
*
*  $ED Descrição da função
*     Retorna o número de comandos de teste específicos lidos e
*     corretamente interpretados.
*     Não conta comandos que retornem não OK independentemento do motivo,
*        inclusive os comandos saltados após uma falha.
*     Também não conta comandos recuperar.
*
***********************************************************************/

   int TST_ObterNumComandosExecutados( void ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Obter número de falhas encontradas
*
*  $ED Descrição da função
*     Retorna o número de falhas encontrados.
*     O módulo de teste específico sinaliza falhas através de
*     uma condição de retorno diferente de TST_CondRetOK.
*     Além de comparações em erro, são tratados como falhas:
*       - comandos de teste em erro,
*       - comandos de teste de um dado caso de teste ignorados (saltados)
*       - erros encontrados nos instrumentos
*       - erros de sintaxe dos comandos de teste
*       - qualquer chamada às funções TST_NotificarFalha, ou TST_ContarFalha.
*     Portanto, é possível que o número de falhas venha a ser maior do
*     que o número de casos de teste existente no script.
*
***********************************************************************/

   int TST_ObterNumFalhas( void ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Obter origem do nome da extensão de arquivo
*
*  $ED Descrição da função
*     Procura o início do nome da extensão contido no nome do arquivo
*     dado por parâmetro
*
*     Evite o uso de nomes de arquivos com dois ou mais caracteres ".".
*     Isto pode confundir no caso de se desejar a adição de algum nome
*     de extensão padrão.
*
*  $EP Parâmetros
*     $P NomeArqParm - nome de arquivo a ser analisado
*
*  $FV Valor retornado
*     Ponteiro para o caractere '.' que separa a extensão.
*     Será NULL caso o nome do arquivo não contenha extensão.
*
***********************************************************************/

   char * TST_ObterInicioExtensao( char * NomeArqParm ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Obter ponteiro para arquivo LOG
*
*  $ED Descrição da função
*     Retorna o ponteiro para o FILE do log em uso no momento.
*     Será stdout caso não seja definido explicitamente ao chamar a funcao
*     TST_ControlarSessaoTeste
*
*  $FV Valor retornado
*     Ponteiro válido para um arquivo de texto de saída
*
***********************************************************************/

   FILE * TST_ObterArqLog( void ) ;


/***********************************************************************
*
*  $FC Função: TSTG &Exibir um espaço qualquer em formato hexadecimal e char
*
*  $ED Descrição da função
*     Exibe o conteúdo de um espaço de tamEspaco bytes. A saída contém
*     uma coluna em hexadecimal e outra em ASCII
*     São exibidos 16 bytes por linha.
*
*  $EP Parâmetros
*     $P tamEspaco - tamanho em bytes do espaço a exibir
*     $P Ponteiro  - ponteiro para o primeiro byte do espaço a exibir
*
***********************************************************************/

   void TST_ExibirEspacoHexa( int    tamEspaco ,
                              void * Ponteiro   ) ;

#undef GENERICO_EXT

/********** Fim do módulo de definição: TSTG Controlador de teste genérico **********/

#else
#endif
