#if ! defined( INTRPCNT_ )
#define INTRPCNT_
/***************************************************************************
*
*  $MCD Módulo de definição: ICNT Interpretador de contadores
*
*  Arquivo gerado:              INTRPCNT.H
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
*     4       avs   01/fev/2006 criar linguagem script simbólica
*     3       avs   08/dez/2004 uniformização dos exemplos
*     2       avs   07/jul/2003 unificação de todos os módulos em um só projeto
*     1       avs   16/abr/2003 início desenvolvimento
*
*  $ED Descrição do módulo
*     Contém o interpretador de comandos de contagem.
*     Este módulo deve ser referenciado no módulo de teste genérico
*     e a função de interpretação deve ser incorporada no
*     "daisy chain" que chama interpretadores sucessivos até que
*     um deles interprete o comando.
*
*  $EIU Interface com o usuário pessoa
*     Comandos de contagem sempre disponíveis
*
*     =inicializarcontadores   <nome arquivo contadores acumulado>
*          o nome do arquivo pode ser nulo.
*          Se o nome do arquivo for ".", será também tratado como nulo.
*
*     =terminarcontadores    <número contadores iguais a zero esperado>
*          Grava o arquivo de contadores acumulados, caso esteja
*          definido (nome não vazio).
*
*     =registraracumulador   <nome arquivo contadores acumulado>
*          substitui o nome registrado.
*          o nome do arquivo pode ser nulo.
*          Se o nome do arquivo for ".", será também tratado como nulo.
*
*     =lercontadores         <nome arquivo contadores>
*          leê o arquivo e atribui os valores iniciais somente se
*          o contador tiver sido lido de um arquivo acumulado.
*
*     =gravarcontadores      <nome arquivo contadores acumulado>
*          grava o arquivo de contadores acumulado registrado.
*          A gravação ocorre também ao terminar contadores
*
*     =zerartodoscontadores
*
*     =zerarcontador   <string nome do contador>
*
*     =iniciarcontagem
*          Ativa a contagem. Este comando precisa ser dado.
*          =inicializarcontadores não ativa a contagem, somente ativa
*          o módulo
*
*     =pararcontagem
*          Desativa a contagem
*
*     =contagemcontador   <número esperado>
*
*     =exibircontagem
*
*     =numcontadores      <número esperado>
*
*     =contagemtotal      <somatório das contagens esperado>
*
*     =verificarcontagens <número contadores zero iguais a zero esperado>
*
***************************************************************************/
 
#if defined( INTRPCNT_OWN )
   #define INTRPCNT_EXT
#else
   #define INTRPCNT_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

#include "TST_ESPC.H"


/***********************************************************************
*
*  $FC Função: ICNT &Interpretar comandos de contagem
*
*  $ED Descrição da função
*     Esta função interpreta todos os comandos específicos para
*     a contagem da passagem do controle da execução por determinado
*     ponto do código fonte.
*
*     Interpreta os comandos
*
*     =inicializarcontadores     <cond ret>
*     =terminarcontadores        <cond ret>
*     =registraracumulador       <nome arquivo acumulador>
*     =lercontadores             <nome arquivo contadores> <num erros esperados>
*     =zerartodoscontadores      <cond ret>
*     =zerarcontador             <nome contador> <cond ret>
*     =iniciarcontagem
*     =pararcontagem
*     =contagemcontador          <nome contador> <contagem esperada>
*     =numcontadores             <num esperado>
*     =contagemtotal             <Contagem esperada>
*     =verificarcontagens        <num contadores zero esperados>
*
***********************************************************************/

   TST_tpCondRet ICNT_EfetuarComadoContagem( char * ComandoTeste ) ;

#undef INTRPCNT_EXT

/********** Fim do módulo de definição: ICNT Interpretador de contadores **********/

#else
#endif
