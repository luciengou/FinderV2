#include "ionRF24LE1.h"

void main( void )
{
    unsigned int itmp;
	
    P0DIR = 0;                                  //Set P0s as output
    P0 |= ( 1<<0 );

    while( 1 )
    {
        //P0 = 0xFF;
        for( itmp = 0; itmp < 50000; itmp ++ );
        for( itmp = 0; itmp < 50000; itmp ++ );
        for( itmp = 0; itmp < 50000; itmp ++ );
        for( itmp = 0; itmp < 50000; itmp ++ );
		
        P0 &= ~( 1<<1 );               //turn on 
        P0 |= ( 1<<2 );              //turn off 


        for( itmp = 0; itmp < 50000; itmp ++ );
        for( itmp = 0; itmp < 50000; itmp ++ );
        for( itmp = 0; itmp < 50000; itmp ++ );
        for( itmp = 0; itmp < 50000; itmp ++ );
		
        P0 &= ~( 1<<2 );                        //turn on 
        P0 |= ( 1<<1 );              //turn off
    }
	
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif