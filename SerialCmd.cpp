// This is:  D:\Russ\0000\Arduino\TestTerminal\Ver4\TestTerminal\TestTerminal.ino or a copy of it 
// convertered from my old pic routines, this uses the normal arduino libe.


// Purpose:
//    Recive a command string untill an end of command ( usually end of line, see .h file ) is recieved
//    and return the command
//    TestTerminal is a program showing its use.
//    Some add ons include
//      parsing out a number from the command
//      parsing out a series of numbers 


// stop still not working if followed by a crlf i think
// do not feel like spending time on this now  2015 Feb 21


#if defined(ARDUINO) && ARDUINO >= 100      // #  https://github.com/adafruit/DHT-sensor-library/issues/1   changed 2015 Feb 21
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "SerialCmd.h"

// ---------- begin constructor ----------
SerialCmd::SerialCmd(  )
{
    //pinMode(pin, OUTPUT);
    //_pin = pin;
}

// ---------- begin subroutine ----------
// add a char to the string
// not really a string until it has been ended with a 0
//
void   SerialCmd::buildCmd( unsigned char aNewChar )  {

    _cmd[ _cmdIx ] =  aNewChar;
    _cmdIx ++;
    if ( _cmdIx >= CMD_MAX ) {
        _cmdIx = CMD_MAX;
    }
}

// ---------- begin subroutine ----------
// backspace the command
void   SerialCmd::backCmd(  )  {
    if ( _cmdIx > 0 ) {
        _cmdIx --;
    }
    return;
}

// ---------- begin subroutine ----------
// reset the command to null, inline function good?
void SerialCmd::resetCmd(  )  {
    _cmdIx 			  = 0;
    gotCmd 		    = 0;
    _cmdStatus 		= CMD_STATUS_ADD;
}

 
 
// ---------- begin subroutine ----------
long   SerialCmd::parseCmdNbr(  const char* text )    {

  long  Nbr = 0;
  char  next;
  char  ix;
  int   sign = 1;

  for( ix = 1; ix < 16; ix++ )   {
        next = text[ix];
        switch ( next )  {
          case 0:  // end of string
              return Nbr * sign;
              break;
          case ' ':  // skip space
              break;
          case '+':  // skip +
              break;
          case '-':  // flip sign
              sign = -1 * sign;
              break;
          default:
            Nbr = ( Nbr * 10 ) + ( next - 48 );
            //Serial.print( "\n\r" );
            //Serial.println( F("") );
        } 
  }
  return Nbr * sign;
}

// ---------- begin subroutine ----------
bool   SerialCmd::doneNext(  )  {

  return done_getNext;
}



// ---------- begin subroutine working nov 2017 do new to manage negs, change to long ----------
// get next numeric value for now only pos ints, could peel off a string value and use libe to convert
// x11 22 333 4 5######
// x11 22 333 4 5 ######
// x 11 22 333 4 5###### ... are all valid note problems at begin and end  
// x with no numbers ???
// for now do not allow trailing blanks which would require look ahead which we could do 
long SerialCmd::getNext(  )  {
     long   val;
     char   a_char;
     val           = 0;
     int    sign   = 1;
     done_getNext  = false;
     while ( true ) {
         // do deal with 2 blanks need to know if we have started converting val == 0 not a good test add a flag??
         ix_getNext ++;
         a_char = cmdPrior[ ix_getNext ];
         if(  a_char == 0 ) {   // null we are done
             done_getNext  = true;
             //Serial.print( "done at ix_getNext = "  );
             //Serial.println( ix_getNext );
             return ( sign * val );
         }
         else if  ( a_char == CHAR_SP ) {
             // if ( val != 0 ) {
             // and go ahead then back up one 
             return (sign * val );   // if faster ??
         }
         else if  ( a_char == '-' ) {
             // if ( val != 0 ) {
             // and go ahead then back up one 
             sign = sign * -1;   // or just assign to -1, do not allow 2 - signs, not doe not process + sign 
         }
         else {
            val  = ( val * 10 ) + ( a_char - 48 );
         }
    }
    return (sign * val );
}

// ---------- begin subroutine ----------
// try to recieve some data
// may use as interrupt service routine in which case serial peek test not required
// for arduino call frequently
void SerialCmd::tryRecCmd ( ) {

    char dataIn;
    while ( Serial.available() > 0 ) {
        dataIn = Serial.read();

        if ( dataIn == CHAR_LF ) {
            // ignore this char
            continue;
        }
        // if ( data_in == CHAR_CR ) {
        // ignore this char
        //	continue;
        //  }

        if ( dataIn == STOP_CHAR )  {
            // stop no matter what else we are in the process of
            // do not log as command
            stopFlag = 1;
            // resetCmd(  );  // this resets the falg -- this is wrong, comment out may not fix -- still does not work at all
            continue;
        }
        if ( dataIn == CHAR_CMD_TERM ) {
            //if ( data_in == CHAR_CR ) {
            // marks end of command execute the command ( or set flag for same )
            // may want to save so can begin to build a new command
            // void strcpy( char *dst, const char *src )
            buildCmd( 0 );  // terminate the string
            strcpy( cmdPrior, _cmd );
            //serial.print( "cmdPrior" );
            //serial.print( cmdPrior );
            resetCmd(  );
            gotCmd = 1;
            done_getNext  = false;
            ix_getNext    = 0;    // this will skip the first char the command 
            continue;
        }
        if ( dataIn == CHAR_BS ) {
            //if (( data_in == CHAR_BS ) || ( data_in == CHAR_DEL )) {
            backCmd();
            continue;
        }
        if ( _cmdStatus == CMD_STATUS_ADD ) {
            buildCmd( dataIn );
        }
        // if here just drop the recieved char
        // are we too long in the interrup think not
    }
    return;
}
// ============ eof =============





















