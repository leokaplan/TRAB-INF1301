/***************************************************************************
*  $MCI Módulo de implementação: ALT  Gerador de números aleatórios
*
*  Arquivo gerado:              GERAALT.c
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
***************************************************************************/

#include <stdlib.h>
#include <time.h>

#define GERAALT_OWN
#include "GERAALT.H"
#undef GERAALT_OWN

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: ALT  &Inicializar o gerador de números aleatórios
*  ****/

   void ALT_Inicializar( unsigned int Base )
   {

      if ( Base == 0  )
      {
         srand( 3141592653 ) ;
      } else if ( Base == 1  )
      {
         srand( 3141592653 | ( unsigned int ) time( NULL )) ;
      } else
      {
         srand( Base ) ;
      } /* if */

   } /* Fim função: ALT  &Inicializar o gerador de números aleatórios */

/***************************************************************************
*
*  Função: ALT  &Gerar inteiro em uniformemente distribuído
*  ****/

   int ALT_GerarDistUniforme( int Inf , int Sup )
   {

      double x ;
      int i ;

      do
      {
         x  = rand( ) ;                 /* converte para flutuante */
         x /= RAND_MAX ;                /* 0.0000 <= x <= 1.0000     */
      } while ( x == 1.0 );             /* evitar valor == 1 */

      #pragma warning(disable:4244)
      i  = ( Sup - Inf + 1 ) * x ;      /* converte/trunca de volta para inteiro */
      #pragma warning(default:4244)
      return Inf + i ;

   } /* Fim função: ALT  &Gerar inteiro em uniformemente distribuído */

/***************************************************************************
*
*  Função: ALT  &Gerar distribuição dada por tabela de freqüências
*  ****/

   int ALT_GerarFrequencia( int numElem , int * vetElem , int Limite )
   {

      int i ,
          j ;

      i = ALT_GerarDistUniforme( 0 , Limite ) ;
      for( j = 0 ; j < numElem ; j++ )
      {
         if ( i <= vetElem[ j ] )
         {
            return j ;
         } /* if */
      } /* for */
      return numElem ;

   } /* Fim função: ALT  &Gerar distribuição dada por tabela de freqüências */

/***************************************************************************
*
*  Função: ALT  &Gerar permutação randômica uniformemente distribuída
*  ****/

   void ALT_GerarPermutacao( int   tamVetor ,
                             int * Vetor     )
   {

      int Temp ,
          inx  ,
          i     ;

      for( i = tamVetor - 1 ; i >= 1 ; i-- )
      {
         inx          = ALT_GerarDistUniforme( 0 , i ) ;
         Temp         = Vetor[ inx ] ;
         Vetor[ inx ] = Vetor[ i   ] ;
         Vetor[ i   ] = Temp ;
      } /* for */

   } /* Fim função: ALT  &Gerar permutação randômica uniformemente distribuída */

/********** Fim do módulo de implementação: ALT  Gerador de números aleatórios **********/

