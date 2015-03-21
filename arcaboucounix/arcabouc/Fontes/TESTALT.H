#if ! defined( TESTALT_ )
#define TESTALT_
/***************************************************************************
*
*  $MCD Módulo de definição: TALT Teste do gerador de números aleatórios
*
*  Arquivo gerado:              TESTALTnulo
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
*     Testa e exemplifica o uso do gerador de números aleatórios
*
***************************************************************************/
 
#if defined( TESTALT_OWN )
   #define TESTALT_EXT
#else
   #define TESTALT_EXT extern
#endif


/***********************************************************************
*
*  $FC Função: TALT &Efetuar o teste de função do módulo aleatorio
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste ) ;

#undef TESTALT_EXT

/********** Fim do módulo de definição: TALT Teste do gerador de números aleatórios **********/

#else
#endif
