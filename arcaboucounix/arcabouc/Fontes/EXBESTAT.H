#if ! defined( Exbestat_ )
#define Exbestat_
/***************************************************************************
*
*  $MCD Módulo de definição: EXES Exibir estatisticas de teste acumuladas
*
*  Arquivo gerado:              Exbestatnulo
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
*     Exibe o conteúdo do arquivo acumulador de estatísticas de massas de teste.
*
*  $EIU Interface com o usuário pessoa
*     Deve ser fornecido um parâmetro /a<nome>
*     no qual <nome> é o nome do arquivo de acumulação de estatísticas
*     gerado com apoio do arcabouço.
*
***************************************************************************/
 
#if defined( Exbestat_OWN )
   #define Exbestat_EXT
#else
   #define Exbestat_EXT extern
#endif


/***********************************************************************
*
*  $FC Função: EXES &Exibir conteúdo do arquivo estatisticas
*
***********************************************************************/

   int main( int NumParm , char ** vtValorParm ) ;

#undef Exbestat_EXT

/********** Fim do módulo de definição: EXES Exibir estatisticas de teste acumuladas **********/

#else
#endif
