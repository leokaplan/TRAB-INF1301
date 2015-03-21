#if ! defined( INTRPCED_ )
#define INTRPCED_
/***************************************************************************
*
*  $MCD Módulo de definição: ICED Interpretador controle de acesso a espacos de dados dinâmicos
*
*  Arquivo gerado:              INTRPCED.H
*  Letras identificadoras:      ICED
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
*  $EIU Interface com o usuário pessoa
*     Comandos de controle de espaços dinâmicos sempre disponíveis
*
*     =exibirtodosespacos  <regra>
*        <regra> pode ser uma de
*           1 - exibe todos os espaços
*           2 - exibe somente os marcados ativo
*           3 - exibe somente os marcados inativo
*
*     =verificartodosespacos  <num erros esperado>
*
*     =obternumeroespacosalocados <num espaços esperado>
*
*     =obternumerototalespacos <num total de alocações esperado>
*
*     =obternumeroregra  <regra>  <num esperado>
*        <regra> pode ser uma de
*           1 - exibe todos os espaços
*           2 - exibe somente os marcados ativo
*           3 - exibe somente os marcados inativo
*        <num esperado>  é o número esperado de espaços alocados e que
*                        satisfazem a regra
*
*     =obtertotalespacosalocado  <num total bytes alocados esperado>
*
*     =obternumeroNULLgerado  <num total de NULLs simulados esperado>
*
*     =exibirnumeroNULLgerado
*
*     =limitarmemoriadisponivel <limite do número de bytes disponíveis
*                                para a alocação dinâmica>
*
*     =limitarnumeroespacos     <limite do número de espaços alocados>
*
*     =limitarfrequenciaNULL    <freqüência com que allocações têm sucesso>
*           é um valor dado em milésimos:  999 indica 99,9% das vezes
*           CED_Malloc alocará memória
*
*     =eliminarlimitesespaco
*
***************************************************************************/
 
#if defined( INTRPCED_OWN )
   #define INTRPCED_EXT
#else
   #define INTRPCED_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

#include "TST_ESPC.H"


/***********************************************************************
*
*  $FC Função: ICED &Interpretar comandos controle de espaço
*
*  $ED Descrição da função
*     O módulo INTESPAC interpreta os comandos do arcabouço que controlam
*     os acessos a espaços de dados dinâmicos.
*     Não existe contexto para este interpretador
*     São seguintes os comandos interpretados:
*
*     =inicializarespacos
*         inicializa o sistema de controle de espaços.
*         Deve ser executado exatamente uma vez.
*
*     =exibirtodosespacos         <Regra>
*         exibe todos os espaços segundo a <Regra> de seleção dada,
*         que pode ser:
*             CED_ExibirTodos    = 1  Exibir todos os espaços
*             CED_ExibirAtivos   = 2  Exibir todos os espaços ativos
*             CED_ExibirInativos = 3  Exibir todos os espaços inativos
*
*     =verificartodosespacos      <numFalhasEsperado>
*         examina a validade de todo o espaço
*
*     =obternumeroespacosalocados <numAlocadosEsperado>
*         obtém o número de espaços controlados alocados no momento
*
*     =obternumerototalespacos    <numTotalEsperado>
*         obtém o número total de vezes em que foram alocados espaços controlados
*
*     =obternumeroregra           <regra>  <numEsperado>
*         obtém o número de espaços segundo <regra> de seleção dada (ver acima)
*
*     =obtertotalespacosalocado   <totalEsperado>
*         obtém o número de bytes úteis alocados no momento
*         se <totalEsperado> for < 0  exibe somente o número de bytes, sem controlar
*
*     =limitarmemoriadisponivel   <novoLimite>
*         estabelece um limite artificial de memória disponível
*
*     =limitarnumeroespacos       <novoLimite>
*         estabelece um limite artificial do néumero de espaços que podem
*         estar alocados em determinado momento
*
*     =eliminarlimitesespaco
*         retira os limites de alocação estabelecidos
*
*     As demais funçoes do módulo CESPDIN CED Controle de Espaço Dinâmico
*     estão disponíveis no arcabouço e podem ser utilizados livremente
*     por módulos de teste (ou outros) desenvolvidos pelo usuário do
*     arcabouço. Elas são testadas através do módulo de teste TESTCED
*     específico para este módulo
*
*  $EP Parâmetros
*     $P ComandoTeste - String contendo o comando
*
***********************************************************************/

   TST_tpCondRet ICED_InterpretarTesteEspaco( char * ComandoTeste ) ;

#undef INTRPCED_EXT

/********** Fim do módulo de definição: ICED Interpretador controle de acesso a espacos de dados dinâmicos **********/

#else
#endif
