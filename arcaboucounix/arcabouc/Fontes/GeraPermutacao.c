/* Gerar uma permutação aleatória de um vetor contendo os n 
*  primeiros inteiros
*  Modo de uso
*     GerarPermutacao n
*       2 < n <= 100
*/


#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <math.h>
#include    <time.h>

#include "GERAALT.H"

#define     DIM_VET           100 
#define     DIM_PARM_BUFFER   100 

int main( int numParm , char ** vtParm ) 
{
   char ParmBuffer[ DIM_PARM_BUFFER ] ;
   int  numGera  = -1 ,
	     numPermut = -1 ,
        numLidos = -1 ,
        Erro     =  0 ;
   int  Vetor[ DIM_VET ] ;
   int  i ;
   int conta ;
/* Exibir logo do programa */

   printf( "\n\nLES - Laboratorio de Engenharia de Software DI/PUC-Rio" ) ;
   printf( "\n      Gerador de permutacoes aleatorias (c) 2007\n" ) ;

/* Inicializar contexto */

   if ( numParm < 3 )
   {
      strcpy( ParmBuffer , "Falta parametro." ) ;
      Erro = 1 ;
   } else
	{
      strcpy( ParmBuffer , vtParm[ 1 ] ) ;
      numLidos = sscanf( ParmBuffer , " %d" , &numGera ) ;
      if ( ( numLidos != 1 )
        || ( numGera   < 2 )
        || ( numGera  >= DIM_VET ))
      {
         Erro = 1 ;
      } /* if */

		strcpy( ParmBuffer , vtParm[ 2 ] ) ;
      numLidos = sscanf( ParmBuffer , " %d" , &numPermut ) ;
      if ( ( numLidos != 1 )
        || ( numPermut < 1 ))
      {
         Erro = 1 ;
      } /* if */
   } /* if */
	
	if ( Erro != 0 )
   {
      printf( "\nNúmero de elementos a permutar errado: %s" , ParmBuffer ) ;
      printf( "\nModo de usar:  gerapermutacao  n  np em que 2 <= n < %d e nP >= 1" , DIM_VET ) ;
      return 4 ;
   } /* if */
         
   srand( 3141592653 ^ ((( unsigned ) time( NULL )) << 8 )) ;

/* Embaralhar vetor */

   for( i = 0 ; i < numGera ; i++ )
   {
      Vetor[ i ] = i + 1 ;
   } /* for */


for ( conta = 0 ; conta < numPermut ; conta ++ )
{
   ALT_GerarPermutacao( numGera , Vetor ) ;

/* Exibir Permutação */

   printf( "\n\nPermutacao" ) ;

   for( i = 0 ; i < numGera ; i ++ )
   {
      if ( i % 10 == 0 )
      {
         printf( "\n" ) ;
      } /* if */
      printf( " %4d" , Vetor[ i ] ) ;
   } /* for */
} /* for */
   printf( "\n" ) ;

   return 0 ;

} /* main */
