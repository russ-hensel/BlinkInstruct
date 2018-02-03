// SerialCmd.h
// updated with F( "SerialCmdMaster with Blink Ver4 2017 11 17.777" )
#ifndef SerialCmd_h

#define SerialCmd_h

//#include "WProgram.h"
// #  https://github.com/adafruit/DHT-sensor-library/issues/1   changed 2015 Feb 21
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#define   CMD_SUPER_MAX           35                    // storage set aside for command string 
#define 	CMD_MAX        	        CMD_SUPER_MAX - 3			// maximum number of characters in a command

// add to command string
#define 	CMD_STATUS_ADD 	        1
// not ready
#define 	CMD_STATUS_NR 	        2

#define CHAR_LF  		0x0A
#define	CHAR_CR  		0x0D

// just one of the next  -- and check what char are ignored on recieve.
//#define		CHAR_CMD_TERM  		'*'
#define	CHAR_CMD_TERM  		0x0D       //0D is cr 0a is lf
//#define		CHAR_CMD_TERM  		'#'    //

#define CHAR_SP       32     // space 32
#define CHAR_BS  		0x08     // is this the backspace char
#define CHAR_DEL 		0x7F
#define	STOP_CHAR 		'!'

class SerialCmd  {
public:
    SerialCmd();
    void   buildCmd( unsigned char aNewChar );
    void   backCmd(  );
    void   resetCmd(  );
    long   parseCmdNbr(  const char* text );        
    bool   doneNext(  );      // or just an instance reference 
    long   getNext(  );
    void   tryRecCmd ( );
    byte   stopFlag;
    byte   gotCmd;        // true when we have a command
    char 	 cmdPrior[CMD_SUPER_MAX];
    bool   done_getNext;
    int    ix_getNext;       // at class level 
private:
    byte 	  _cmdIx;
    char 	  _cmd[CMD_SUPER_MAX];   // can init with = "ss" but cannot reassign later
    byte  	_cmdStatus;
    byte    _dataIn;
};

#endif

// ====================== eof ====================







