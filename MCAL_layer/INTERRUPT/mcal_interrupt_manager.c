/* 
 * File:   mcal_interrupt_manger.c
 * Author: Honey
 *
 * Created on September 20, 2023, 5:17 PM
 */

#include "mcal_interrupt_manger.h"

static volatile uint8 RB4_flag = 1,
RB5_flag = 1,
RB6_flag = 1,
RB7_flag = 1 ;

#if PIRIORITY_ENABLE == PIRIORITY_ON
void __interrupt() interrupt_high_priority_manger()
{
     if(INTCONbits.INT0F == INTERRUPT_OCCUR && INTCONbits.INT0E  == INTERRUPT_OCCUR )
    {
         INT0_ISR();
    }
}
void __interrupt(low_priority) interrupt_low_priority_manger()
{
}

#else
void __interrupt() interrupt_manger()
{
    if(INTCONbits.INT0F == INTERRUPT_OCCUR && INTCONbits.INT0E  == INTERRUPT_ENABLE )
    {
         INT0_ISR();
    }
    if(INTCON3bits.INT1F == INTERRUPT_OCCUR && INTCON3bits.INT1E == INTERRUPT_ENABLE)
    {
         INT1_ISR();
    }
    if(INTCON3bits.INT2F == INTERRUPT_OCCUR && INTCON3bits.INT2E == INTERRUPT_ENABLE)
    {
        INT2_ISR();
    }
    
    /*RBx interrupt*/
    if(INTCONbits.RBIF == INTERRUPT_OCCUR && INTCONbits.RBIE == INTERRUPT_ENABLE && PORTBbits.RB4 == HIGH && RB4_flag == 1)
    {
        RB4_flag = 0;
        RB4_ISR(high_src);
    }
    if(INTCONbits.RBIF == INTERRUPT_OCCUR && INTCONbits.RBIE == INTERRUPT_ENABLE && PORTBbits.RB4 == LOW && RB4_flag == 0)
    {
        RB4_flag = 1;
        RB4_ISR(low_src);
    }
    if(INTCONbits.RBIF == INTERRUPT_OCCUR && INTCONbits.RBIE == INTERRUPT_ENABLE && PORTBbits.RB5 == HIGH && RB5_flag == 1)
    {
        RB5_flag = 0;
        RB5_ISR(high_src);
    }
    if(INTCONbits.RBIF == INTERRUPT_OCCUR && INTCONbits.RBIE == INTERRUPT_ENABLE && PORTBbits.RB5 == LOW && RB5_flag == 0)
    {
        RB5_flag = 1;        
        RB5_ISR(low_src);
    }
    if(INTCONbits.RBIF == INTERRUPT_OCCUR && INTCONbits.RBIE == INTERRUPT_ENABLE && PORTBbits.RB6 == HIGH && RB6_flag == 1)
    {
        RB6_flag = 0;
        RB6_ISR(high_src);
    }
    if(INTCONbits.RBIF == INTERRUPT_OCCUR && INTCONbits.RBIE == INTERRUPT_ENABLE && PORTBbits.RB6 == LOW && RB6_flag == 0)
    {
        RB6_flag = 1;
        RB6_ISR(low_src);
    }
    if(INTCONbits.RBIF == INTERRUPT_OCCUR && INTCONbits.RBIE == INTERRUPT_ENABLE && PORTBbits.RB7 == HIGH && RB7_flag == 1)
    {
        RB7_flag = 0;
        RB7_ISR(high_src);
    }
    if(INTCONbits.RBIF == INTERRUPT_OCCUR && INTCONbits.RBIE == INTERRUPT_ENABLE && PORTBbits.RB7 == LOW && RB7_flag == 0)
    {
        RB5_flag = 1;
        RB7_ISR(low_src);
    }
    /*ADC INTERRUPT*/
    if( PIR1bits.ADIF == INTERRUPT_OCCUR&& PIE1bits.ADIE == INTERRUPT_ENABLE )
    {
        ADC_ISR();
    }
    /*TIMER0 INTERRUPT*/
    if(INTCONbits.TMR0IF == INTERRUPT_OCCUR && INTCONbits.TMR0IE == INTERRUPT_ENABLE)
    {
        TIMER0_ISR();
    }
    /*TIMER1 INTERRUPT*/
    if(PIR1bits.TMR1IF == INTERRUPT_OCCUR &&PIE1bits.TMR1IE == INTERRUPT_ENABLE)
    {
         TIMER1_ISR();
    }
   /*TIMER2 INTERRUPT*/
    if(PIR1bits.TMR2IF == INTERRUPT_OCCUR &&PIE1bits.TMR2IE == INTERRUPT_ENABLE)
    {
         TIMER2_ISR();
    }
    /*TIMER3 INTERRUPT*/
    if(PIR2bits.TMR3IF == INTERRUPT_OCCUR &&PIE2bits.TMR3IE == INTERRUPT_ENABLE)
    {
         TIMER3_ISR();
    }
    /*CCPx interrupt*/
    if(PIR2bits.CCP2IF == INTERRUPT_OCCUR &&PIE2bits.CCP2IE == INTERRUPT_ENABLE)
    {
         CCP2_ISR();
    }
    if(PIR1bits.CCP1IF == INTERRUPT_OCCUR &&PIE1bits.CCP1IE== INTERRUPT_ENABLE)
    {
         CCP1_ISR();
    }
    /*EUSART interrupt */
    if(PIR1bits.RCIF ==INTERRUPT_OCCUR && PIE1bits.RCIE == INTERRUPT_ENABLE )
    {
        Rx_ISR();
    }
    if(PIR1bits.TXIF ==INTERRUPT_OCCUR && PIE1bits.TXIE == INTERRUPT_ENABLE )
    {
        Tx_ISR();
    }
    /*I2C Interrupt*/
    if(PIR1bits.SSPIF ==INTERRUPT_OCCUR && PIE1bits.SSPIE == INTERRUPT_ENABLE )
    {
        I2C_ISR();
    }
     if(PIR2bits.BCLIF ==INTERRUPT_OCCUR && PIE2bits.BCLIE == INTERRUPT_ENABLE )
    {
       I2C_BUS_COL_ISR();
    }
    
}
#endif
