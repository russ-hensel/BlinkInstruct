// Program description ( ):
//      Rewrite of classic blink example to inclued a serail command interface and some other "tricks "
//
//      Features
//           instructional also see readme.h
//          
//
// Author:      russ_hensel http://www.opencircuits.com/User:Russ_hensel
//
// ============================= use .h files for easy modification using #define 's ================================

#include "BlinkInstruct.h"    // #defines for this project
#include "SerialCmd.h"        // #defines for command interperter 

// ============================= globals variables ... ========

SerialCmd    serialCmd;        // my version of serial interface, use it or something else that is more standard 
long         blinkDelay;       // used to vary blink speed 

// ----------------------------------------------------------------
void rptHelp( void ) {  
//  help just a little intro for the program -- summary of commands and version 
    Serial.println( F( "" ) );
    Serial.print(   F( "Arduino:  " ) );  
    Serial.println( VERSION_ID  );   
    Serial.println( F( "c       Classic blink" ) );         // void classicBlink the origninal blink routine 
    Serial.println( F( "d nn    Set delay" ) );             // void setDelay( long delay )   {
    Serial.println( F( "f nn    Finite blink" ) );          // void finiteBlink( max_loop ) {
    Serial.println( F( "h       High speed blink" ) );      // void highSpeedBlink( ) {
    Serial.println( F( "n       New blink" ) );             // void newBlink( ) {
    Serial.println( F( "v       Version of software" ) );   // void rptVersion( )
    Serial.println( F( "?       Help" ) );                  // void rptHelp( )
}

// ---------- Main ----------   
void loop()   {
    
    unsigned char   cmdChar;
    long            cmdNbr;
    
    serialCmd.tryRecCmd( );

    if ( serialCmd.gotCmd  )   {

      cmdChar  = serialCmd.cmdPrior[0];                         // get the character for the command 
      cmdNbr   = serialCmd.parseCmdNbr( serialCmd.cmdPrior );   // get associated number if any 
      
      switch ( cmdChar )  {
     
        case 'c':  //     
            classicBlink();                 
            break;      
        case 'd':  //     
            setDelay( cmdNbr );           
            break;   
        case 'f':  //     
            finiteBlink( cmdNbr  );                
            break;   
        case 'h':  //     
            highSpeedBlink();
            break;   
         case 'n':  //                                                                   case 'h':  //     
            newBlink();
            break;       
         case 'v':   
            rptVersion();
            break;             
         case '?':  
            rptHelp();
            break;                                     
        default:
            Serial.print( F( "!Bad Command = " ) );
            Serial.println( char(cmdChar) );
        } 
        Serial.println( F( "ok"  ) );
        serialCmd.gotCmd    = 0;
    }           
}

// ------------------------------------------------------
void highSpeedBlink( ) {
//  fastest blink -- unless you get tricky
    while ( true ) { 
        digitalWrite( MY_LED_PIN, HIGH );   // turn the LED on (HIGH is the voltage level)                  
        digitalWrite( MY_LED_PIN, LOW );    // turn the LED off by making the voltage LOW       
    } 
}

// ------------------------------------------------------
void finiteBlink( long max_loop ) {
//  this loop runs for a finite time as specified in max_loop
//  speed set via blinkDelay  
    for ( int ix  = 1;   ix <= max_loop;  ix ++ ) { 
        digitalWrite( MY_LED_PIN, HIGH );   // turn the LED on (HIGH is the voltage level)
        delay( blinkDelay );                  
        digitalWrite( MY_LED_PIN, LOW );    // turn the LED off by making the voltage LOW
        delay( blinkDelay ); 
    }         
}

// ------------------------------------------------------
void newBlink( ) {
//  revised from the original blink example to let you set the speed at run time   
    while ( true ) { 
        digitalWrite( MY_LED_PIN, HIGH );   // turn the LED on (HIGH is the voltage level)
        delay( blinkDelay );                 
        digitalWrite( MY_LED_PIN, LOW );    // turn the LED off by making the voltage LOW
        delay( blinkDelay );  
    }              
}

// ------------------------------------------------------

void classicBlink( ) {
//  from the original blink example 
    while ( true ) { 
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(1000);                       // wait for a second
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        delay(1000);                       // wait for a second
    }
}

// ------------------------------------------------------
void setDelay( long delay )   {
//  sets the blink delay  
    blinkDelay  = delay;
    Serial.print( F( "delay = " ) );
    Serial.println( blinkDelay );
}

// ------------------------------------------------------
void rptVersion( )   {
    Serial.println( VERSION_ID  );
}

// ---------- begin setup ----------
void setup()   {
  
      Serial.begin( BAUD_RATE );
      serialCmd             = SerialCmd();
      serialCmd.resetCmd(  );
      delay( 1000 );     // setup time, some rummors it is benneficial 
      // initialize digital pin LED_BUILTIN as an output.
      pinMode( LED_BUILTIN, OUTPUT );
      pinMode( MY_LED_PIN, OUTPUT );   // initialize second pin ( which may be the same as LED_BUILTIN or not )
      blinkDelay    = DEFAULT_DELAY ;
      rptVersion();
      Serial.println( F( "ok"  ) );
}

// ======================= eof ==============================











