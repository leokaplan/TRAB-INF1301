#if ! defined( Tst_Espc_ )
#define Tst_Espc_
/***************************************************************************
*
*  $MCD Módulo de definição: TSTE Interface de teste específico
*
*  Arquivo gerado:              Tst_Espc.h
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
*     Este módulo consta somente do módulo de definição de todos os módulos
*     de teste específicos a serem desenvolvidos, e também da especificação
*     do tipo de condições de retorno deste módulo.
*
*     Ele estabelece a interface a ser utilizada pelo módulo de teste
*     genérico com todos os módulos de teste específico a serem
*     desenvolvidos.
*
*     Os módulos de teste específico devem sempre incluir este módulo
*     de definição e, idealmente, não devem adicionar mais itens
*     à interface.
*
*     O módulo de teste específico contém a função de teste de um
*     determinado módulo ou conjunto de módulos. Mais especificamente,
*     contém a função que interpreta os comandos de teste que exercitarão
*     as funções do(s) módulo(s) em teste.
*
*     Poderá conter também diversas funções auxiliares requeridas para
*     realizar os testes, vide "contexto" a seguir.
*
*     O contexto é formado por variáveis e estruturas auxiliares necessárias
*     para que se possa efetuar todos os comandos de teste.
*     Por exemplo pode-se desejar testar várias instâncias de uma
*     mesma estrutura. Uma forma de resolver isto é criar um vetor
*     global static
*     de n elementos em que cada elemento aponta para a cabeça de
*     uma determinada instância dessa estrutura. Para realizar um
*     teste relativo a uma determinada instância, torna-se necessário
*     selecioná-la através de um índice contido no comando de teste.
*     Este parâmetro, embora seja um índice, é na realidade o identificador
*     da instância da estrutura a ser manipulada (idEstrutura).
*
*     Caso o módulo em teste requeira um contexto, este deverá ser
*     estabelecido no presente módulo. Para tal devem ser declaradas
*     variáveis e estruturas encapsuladas e devem ser incluídos comandos
*     de teste capazes de manipular o contexto.
*
*     Uma das formas para estabelecer o contexto seria a de incluir
*     no mçodulo de teste específico comando semelhantes a:
*
*       =iniciaXXX  que inicializa o contexto do módulo sob teste para
*                   vazio. Este comando deve ser utilizado uma única vez
*                   logo ao inciar a execução do teste.
*       =reset      restaura o contexto para vazio. Este comando usualmente
*                   assumirá que o contexto a esvaziar esteja correto.
*                   O comando deverá liberar os espaços alocados aos
*                   elementos, anular as referências a ele, inicializar
*                   valores de vetores de referências, etc.
*                   O comando tem utilidade especial ao segmentar um teste
*                   em partes estanques e ao controlar vazamento de
*                   memória.
*       =terminaXXX este comando destrói todo o contexto e não o
*                   reinicializa. O objetivo é poder verificar vazamento
*                   de memória.
*
*     Evidentemente, outra forma seria a de alterar as funções de
*     inicialização e término do módulo de teste genérico o que é pouco
*     recomendável pois viola o princípio de um mesmo módulo genérico
*     servir para testar todos os construtos.
*
*     Para cada função do módulo a ser testado deve existir pelo menos
*     um comando de teste. Em alguns casos pode ser interessante criar
*     comandos que armazenem, alterem ou consultem variáveis de
*     estado do próprio módulo de teste. Ver os comentários sobre
*     contexto acima. Ou seja, podem existir comandos de teste que
*     têm o propósito de agir sobre o contexto e, não, o de efetuar
*     um determinado teste. Alguns desses comandos podem inclusive
*     realizar verificações entre um resultado esperado e existente
*     no contexto.
*
*     Todos os controles de consistência dos parâmetros dos comandos
*     de teste devem ser realizados no módulo de teste específico.
*     Recomenda-se que seja sempre retornado TST_CondRetParm,
*     sinalizando erro de sintaxe do comando de teste ou valores
*     não permitidos. Deve ser sempre assumido que os parâmetros
*     podem estar em erro. Ou seja, o controle da integridade dos
*     comando é mandatória.
*
*     Caso seja necessário emitir mensagens  especiais,
*     deve ser utilizada a função genérica "TST_ExibirPrefixo".
*     Caso seja desejado acrescentar um texto a alguma mensagem já exibida,
*     (prefixo, ou comparar), utilize a função
*        fprintf( TST_ObterArqLog( ) , "..." , ... )
*     na qual não deve ocorrer avanço de linha ou nova linha ("\n" ou "\r")
*     no parâmetro indicador de formato (2o. parâmetro). Essa comvenção
*     assegura a consistência do aspecto das mensagens com o restante do
*     programa de teste.
*
*     Cada comando deve ser completamente interpretado pelo seu
*     respectivo fragmento de interpretação. Ou seja, não deverá ser
*     retornado para o módulo genérico qualquer dado adicional à
*     condição de retorno da função de teste específico. Veja
*     "TST_ExibirPrefixo" acima, caso seja desejado adicionar detalhes
*     à mensagen já exibida.
*
*     A função TST_NotificarFalha sempre registra uma falha, enquanto que
*     a função TST_ExibirPrefixo pode ser usada para emitir mensagens
*     de informação ou mesmo de advertência. Caso a função TST_ExibirPrefixo
*     seja utilizada para sinalizar uma falha, é necessário chamar
*     também a função TST_ContarFalha( ) para que seja notificada a falha.
*     Isto não é necessário no caso da função TST_NotificarFalha, pois
*     esta sempre conta falhas.
*
*     As funções a testar que retornem um valor sempre devem ter
*     este valor verificado, mesmo que seja uma condição de retorno.
*     Utilize para isto uma das funções " Comparar" do arcabouço.
*     Caso uma função retorne dois ou mais valores, incluindo
*     a condição de retorno, utilize um código semelhante ao
*     descrito a seguir.
*
*     Seja
*        FuncATestar( p1, p2, p3 ... ) => v1, v2, ...
*     a função a ser testada e, v1, v2, ... os dados retornados de
*     alguma forma (valor retornado, parâmetro por referência, variável
*     global). Assumindo que v1 seja o valor retornado.
*
*        v1Obtido = FuncATestar( ...
*        CondRet  = TST_CompararXXX( v1Obtido , v1Esperado , "mensagem 1" ) ;
*        if ( CondRet != TST_CondRetOK )
*        {
*           return CondRet ;
*        }
*
*        CondRet  = TST_CompararXXX( v2Obtido , v2Esperado , "mensagem 2" ) ;
*        if ( CondRet != TST_CondRetOK )
*        {
*           return CondRet ;
*        }
*        ...
*
*        TST_CompararXXX( vnObtido , vnEsperado , "mensagem n" ) ;
*
***************************************************************************/
 
#if defined( Tst_Espc_OWN )
   #define Tst_Espc_EXT
#else
   #define Tst_Espc_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

#define     SINALIZA_ERRO       ">>>"
#define     SINALIZA_RECUPERA   "<<<"
#define     SINALIZA_COMENTARIO "!!!"


/***********************************************************************
*
*  $TC Tipo de dados: TST Cond ret módulo de teste específico
*
*
*  $ED Descrição do tipo
*     Este tipo enumera todas as condições de retorno que podem ser
*     geradas pela função de teste específica.
*
***********************************************************************/

   typedef enum {

         TST_CondRetOK ,
               /* Comando de teste executou corretamente */

         TST_CondRetErro ,
               /* Comando de teste encontrou uma ou mais falhas
               *
               *$ED Descrição
               *   Esta condição sinaliza que um ou mais resultados da execução
               *   da função sob teste resultou em um valor diferente do esperado. */

         TST_CondRetParm ,
               /* Comando de teste está com a sintaxe em erro
               *
               *$ED Descrição
               *   A presente condição de retorno deve ser retornada caso a sintaxe
               *   ou os valores do comando de teste estiverem em erro.
               *
               *   Todos os fragmentos de interpretação devem:
               *   - realizar a análise dos parâmetros do comando
               *   - verificar se todos os parâmetros foram lidos
               *   - verificar se os valores dos parâmetros estão corretos
               *     por exemplo: verificar se os índices de acesso a elementos
               *     estão dentro dos limites dos correspondentes vetores.
               *   - efetuar a função a testar
               *   - verificar se os resultados retornados por esta função estão
               *     corretos */

         TST_CondRetNaoExecutou ,
               /* Comando de teste não se destina ao presente interpretador
               *
               *$ED Descrição
               *   Esta condição deve ser retornada por todas as funções de interpretação
               *   de comandos de teste relativas a módulos do próprio arcabouço,
               *   caso não tenha sido encontrado um comando válido. Isso permite
               *   estabelecer um "daisy chain" de interpretadores em que cada qual
               *   tenha finalidade específica no contexto do arcabouço. */

         TST_CondRetNaoConhec ,
               /* Comando de teste não é conhecido
               *
               *$ED Descrição
               *   Esta condição deve ser retornada por todas as funções de interpretação
               *   de casos de teste específico visando determinado módulo em teste,
               *   sempre que esse interpretador não consiga identificar o comando.
               *
               *   Cabe salientar a diferença entre TST_CondRetNaoExecutou e
               *   TST_CondRetNaoConhec. A primeira corresponde à não interpretação
               *   de um comando possivelmente válido em interpretadores subsequentes.
               *   A segunda corresponde a comandos não válidos considerando todos
               *   os interpretadores. */

         TST_CondRetNaoImplementado ,
               /* Comando de teste não implementado
               *
               *$ED Descrição
               *   Esta condição deve ser retornada ao interpretar um comando de teste
               *   específico ainda não implementado.
               *
               *   Recomenda-se que o desenvolvimento dos módulos sob teste e do
               *   correpondente módulo de teste específico se dê incrementalmente.
               *   Consequentemente diversos comandos não estarão implementados
               *   no início do desenvolvimento. Esta incompleteza poderá ser
               *   sinalizada com esta condição de retorno. */

         TST_CondRetMemoria
               /* Faltou memória para executar o comando
               *
               *$ED Descrição
               *   Esta condição deve ser retornada caso tenha ocorrido algum erro
               *   quando o módulo de teste específico tentar alocar memória ou
               *   outro recurso. */

   } TST_tpCondRet ;


/***********************************************************************
*
*  $FC Função: TSTE &Efetuar comandos de teste específicos
*
*  $ED Descrição da função
*     Efetua os diversos comandos de teste específicos para o módulo
*     sendo testado.
*
*     Cada módulo de teste específico deve desenvolver esta função.
*     Poderá conter diversas outras conforme necessário. No entanto
*     estas devem todas ser funções encapsuladas no módulo (static)
*
*     Freqüentemente é necessário desenvolver um contexto para o teste.
*     Por exemplo, pode ser necessário tratar n elementos disponibilizados
*     pelo módulo sob teste. Neste caso deveria ser criado um vetor
*     global com referências para estas elemento. Ao iniciar os teste
*     este vetor deverá ser inicializado. Ao terminar, os elementos do
*     vetor referenciando algum elemento devem ser destruídos.
*
*     Todos os controles de consistência devem ser realizados no
*     módulo de teste específico.
*
*     A função TST_EfetuarComando( ) deve retornar as condições de retorno
*     identificadas no tipo TST_tpCondRet.
*
*     As funções do módulo sob teste que retornam algum valor sempre
*     devem sempre ter este valor verificado, mesmo sendo uma condição de
*     retorno. Para fazer a vereficação da corretude, recomenda-se
*     utilizar as funções TST_CompararXXX definidas no módulo genérico.
*     Assegura-se assim a uniformidade da saída gerada durante o teste.
*
*     Para realizar a análise de um comando de teste recomenda-se
*     utilizar as funções definidas no módulo LerParm. Elas, além
*     de realizarem a análise léxica, permitem o uso intercambiável
*     de parâmetros simbólicos ou literais. Também realizam a
*     verificação da corretude dos tipos dos parâmetros, contribuindo
*     assim para a redução de erros de redação de scripts de teste
*     que possam passar despercebidos.
*
*     As funções a serem testadas que requeiram algum argumento, devem
*     ser chamadas com valores (parâmetros) extraídos do comando de
*     teste sendo executado. Use sempre as funções do módulo LerParm
*     para realizar a análise sintática.
*
*  $EP Parâmetros
*     $P ComandoTeste - String contendo o comando
*
*  $FV Valor retornado
*     TST_CondRetXXX       - veja a descriÓcão dos possíveis valores
*                            retornados na especificação do correspondente
*                            enum
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste ) ;

#undef Tst_Espc_EXT

/********** Fim do módulo de definição: TSTE Interface de teste específico **********/

#else
#endif
