/*---------------------------------------------------------------------------------


	Simple console based on the 'hello world' demo from alekmaul
	--Jeremy

---------------------------------------------------------------------------------*/
#include <snes.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) 
{
	enum shellCMD
	{
		None     = 0,
		TextDemo = 1
	};
	
	unsigned short pad0;
	unsigned short pad1;
	
	unsigned short pad0Shift;
	unsigned short prevPad;
	
	const int maxASCII = 0x256;
	
    //initialize SNES
	consoleInit();
    
	//Set background modes and ascii tiles
	SetupBackGround(0);
	
	//Draw and turn on the screen
	DrawStartup();
	setScreenOn();
	
	//Wait for some input
	//TODO uncomment: while (!padsCurrent(0));
	setFadeEffect(FADE_OUT);
	WaitForVBlank();
	
	//Blank the screen and Fade it back in
	ClearStartup();
	setFadeEffect(FADE_IN);
	
	//Reset the counter and init console draw vars
	int vBlankCounter = 0;
	int vBlankTotal = 0;
	int curLine = 0;
	int curCmd = 0;
    const int maxCmd = 2;
	
	bool newLine = true;
	
	int col = 0;
	prevPad = 0;
	
	bool newChar      = false;
	int  newCharFrame = 0;
	static const int numFrameSettle = 2;
	
	
	//Init line indicator with information
	curLine++;
	consoleDrawText(col, curLine, "->Welcome to Demo");
    curLine++;
    consoleDrawText(col, curLine, "->Please use L/R to start");
    curLine++;
    consoleDrawText(col, curLine, "->");
	//increment column indexer by 2
	col = col + 2;
	
	//Show current user mode
	bool inputMode = true;
	consoleDrawText(0, 27, "Current Mode = Command");
	
	u16 scrX=0, scrY=0;
	u16 move;
	
	while(true) 
	{
		//Get current #0 pad
		pad0 = 0;
		pad0 = padsCurrent(0);
		/**
		//Switch modes
		if((pad0 == KEY_B) && (prevPad != KEY_B))
		{
			inputMode = SetConsoleMode(inputMode);
		}
		else if(inputMode == true)
		{
		**/
        
		//Scroll screen up and down
		switch (pad0) 
		{
			case KEY_DOWN: 
				scrY+=2; 
				bgSetScroll(0, scrX,scrY);
				break;
			case KEY_UP: 
				scrY-=2; 
				bgSetScroll(0, scrX,scrY);
				break;
            case KEY_L:
                if(prevPad != pad0)
                {
                    curCmd--;
                }
                break;
            case KEY_R:
                if(prevPad != pad0)
                {
                    curCmd++;
                }
                break;
            case KEY_START:
                //Start routine
                StartRoutine(curCmd);
			default:
				break;
		}
        
        //Bounds check the switch
        if(curCmd >= maxCmd)
        {
            curCmd = 0;
        }
        else if (curCmd < 0)
        {
            curCmd = (maxCmd - 1);
        }
        switch(curCmd)
        {
            case 0:
                //Wait for next input
                consoleDrawText(col, curLine, "NULL    ");
                break;
            case 1:
                consoleDrawText(col, curLine, "TextDemo");
                //List Folders
                break;
            default:
                consoleDrawText(col, curLine, "        ");
                //Do nothing
                break;
        }
		
        /**
		else if(inputMode == false)
		{
			pad0Shift = pad0 >> 4;
					
			//Let input settle out (When using emulator and AHK)
			if(prevPad != pad0)
			{
				newChar = true;
				newCharFrame = vBlankTotal;
			}
			
			if(newChar == true)
			{
				//Input not changing
				if((vBlankTotal - newCharFrame) > numFrameSettle)
				{
					newChar = false;
					
					//Draw text to the screen
					if(pad0Shift < maxASCII)
					{	
						//Draw the current line indicator
						if(pad0Shift == 0xA)
						{
							//erase previous line
							consoleDrawText(0, curLine, "  ");
							curLine++;
							consoleDrawText(0, curLine, "->");
							//increment column indexer by 2
							col = 2;
						}
						else if(pad0Shift != 0)
						{
							sprintf(buf, "%c", pad0Shift);
							consoleDrawText(col, curLine, buf);
							col++;
						}
				
						//Debugging statements
						//Record the short value
						sprintf(buf, "val = %u  ", pad0Shift);
						consoleDrawText(23, 25, buf);

						//Record the ascii value
						sprintf(buf, "asc = %c", pad0Shift);
						consoleDrawText(23, 26, buf);
					}
					
				}
			}
		}
		**/
		prevPad = pad0;
		padsClear(0);
		
		//Draw the current frame
		consoleDrawText(23,27,"Frm = %03d",vBlankCounter);
		WaitForVBlank();
		vBlankTotal++;
		if(vBlankCounter == 1000)
		{
			//Reset Counter (3 Digits)
			vBlankCounter = 0;
		}
		else
		{
			vBlankCounter++;	
		}
	}
	
	return 0;
}