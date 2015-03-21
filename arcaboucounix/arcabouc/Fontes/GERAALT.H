#if ! defined( GERAALT_ )
#define GERAALT_
/***************************************************************************
*
*  $MCD Módulo de definição: ALT  Gerador de números aleatórios
*
*  Arquivo gerado:              GERAALT.h
*  Letras identificadoras:      ALT
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
*     Implementa um pequeno conjunto de geradores de números aleatórios.
*
*  $EIM Interface com outros módulos
*     Antes de gerar qualquer número deve ser chamada a função
*     ALT_Inicializar.
*
***************************************************************************/
 
#if defined( GERAALT_OWN )
   #define GERAALT_EXT
#else
   #define GERAALT_EXT extern
#endif


/***********************************************************************
*
*  $FC Função: ALT  &Inicializar o gerador de números aleatórios
*
*  $ED Descrição da função
*     Inicializa o gerador de números aleatórios.
*     Esta função deve ser chamada antes de qualquer outra do módulo GERAALT
*
*  $EP Parâmetros
*     Base - se == 0 utiliza a semente normal (PI 3141592653 )
*                  isto gerará a mesma seqüência de números aleatórios
*                  a seguir da inicialização. Esta semente é tida como
*                  uma boa semente, a distribuição é virtualmente uniforme
*                  e a cardinalidade é a maior possível.
*            se == 1 utiliza a PI | time( )
*                  isto gera uma seqüência de números aleatórios
*                  diferente a cada inicialização
*            outros - utiliza o valor fornecido como semente
*                  deve ser tomado cuidado pois uma semente mal escolhida
*                  pode levar à geração de uma seqüência não aleatória,
*                  ou de cardinalidade limitada.
*
*  $ER Requisitos
*     Esta função deve ser chamada uma única vez e isso antes de gerar
*     o primeiro número aleatório
*
***********************************************************************/

   void ALT_Inicializar( unsigned int Base ) ;


/***********************************************************************
*
*  $FC Função: ALT  &Gerar inteiro em uniformemente distribuído
*
*  $ED Descrição da função
*     Gerar um número inteiro uniformemente distribuído em um
*     intervalo dado
*
*  $EP Parâmetros
*     $P Inf - limite inferior, inclusive
*     $P Sup - limite superior, inclusive
*
*  $FV Valor retornado
*     Inf <= x <= Sup
*
*  $EAE Assertivas de entradas esperadas
*     Inf <= Sup
*     Dados errados provocarão um comportamento errático no programa.
*
***********************************************************************/

   int ALT_GerarDistUniforme( int Inf , int Sup ) ;


/***********************************************************************
*
*  $FC Função: ALT  &Gerar distribuição dada por tabela de freqüências
*
*  $ED Descrição da função
*     Gerar distribuição segundo tabela de freqüências
*
*  $EP Parâmetros
*     $P numElem - número de elementos (intervalos) da tabela
*                  o número de intervalos é na realidade igual a numElem + 1
*     $P vetElem - vetor de elementos da tabela
*                  Os valores dos elementos são números entre 0 e Limite - 1
*                  Os elementos devem estar em ordem crescente.
*                  Os elementos correspondem a uma função cumulativa
*                  Exemplo, se forem desejados as freqüências
*                     10% 30% e 60%
*                  devem ser fornecidos os dados 2 {10 , 40} 100
*     $P Limite  - é o limite superior da distribuição de freqüências
*                  Permite chegar a freqüências muito pequenas, por exemplo
*                  para uma distribuição de freqüências 9999/10000 e
*                  1/10000 forneça os parâmetros
*                  1 9999 10000
*
*  $FV Valor retornado
*     O índice da freqüência 0 <= indice <= numElem, com uma probabilidade
*     igual ao correspondente intervalo.
*
*     A probabilidade x é dada por
*         x = ( vetElem[ i ] - vetElem[ i - 1 ] ) / Limite
*         para todos i , 0 <= i <= numElem
*            sendo que vetElem[ -1      ] é assumido igual a 0
*            sendo que vetElem[ numElem ] é assumido igual a Limite
*
*  $EAE Assertivas de entradas esperadas
*     numElem >= 1
*     vetElem[ numElem - 1 ] < Limite
*     para todos i, 0 <= i < numElem : vetElem[ i - 1 ] < vetElem[ i ]
*             sendo que vetElem[ -1 ] é considerado igual a 0
*     Dados errados provocarão um comportamento errático no programa.
*
***********************************************************************/

   int ALT_GerarFrequencia( int numElem , int * vetElem , int Limite ) ;


/***********************************************************************
*
*  $FC Função: ALT  &Gerar permutação randômica uniformemente distribuída
*
*  $ED Descrição da função
*     Gera uma permutação de n elementos uniformemente distribuída.
*     O algoritmo utilizado é o "Knuth shuffle" descrito na Wikipedia.
*
*  $EP Parâmetros
*     $P tamVetor - número de elementos a serem embaralhados
*     $P    Vetor - elementos a serem embaralhados. O vetor deve conter
*                   pelo menos tamVetor elementos. Caso não contenha, os
*                   resultados serão impreditíveis, possivelmente
*                   desastrosos.
*                   O vetor deve estar inicializado com os elementos a
*                   serem permutados. Por exemplo, no caso de um baralho,
*                   o vetor deve ser inicializado com a seqüência de
*                   índices das cartas (0 a 51).
*
***********************************************************************/

   void ALT_GerarPermutacao( int   tamVetor ,
                             int * Vetor     ) ;

#undef GERAALT_EXT

/********** Fim do módulo de definição: ALT  Gerador de números aleatórios **********/

#else
#endif
