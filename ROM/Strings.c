/*---------------------------------------------------------------------------------


	Simple shell idea based on 'hello world' demo from alekmaul
	--Jeremy

---------------------------------------------------------------------------------*/
#include <snes.h>
#include <stdio.h>

extern char snesfont;
static u8 _bgCnt, _iloc;
static char buf[32];
static char atBuf[4];
	
int curLine;
int col;
int vBlankTotal;

int curCmd;
int cmdRun;
	
void SetupBackGround(int consoleBgNumber)
{
	//Initialize text console with our font
	consoleInitText(0, 0, &snesfont);

	//Now Put in 16 color mode and disable Bgs except current
	setMode(BG_MODE1, 0);
	
	//Set the BG to 64x32
    bgSetMapPtr(consoleBgNumber, 0x800, SC_64x64);
	
	bgSetDisable(1);
	bgSetDisable(2); 
}
	
void DrawStartup()
{
	//Draw the screen header
	int verMaj = 0;
	int verMin = 1;
	
	sprintf(buf, "--------------------------------\n");
	consoleDrawText(0, 12, buf);
	sprintf(buf, "|SNES Shell Version %d.%d|\n", verMaj, verMin);
	consoleDrawText(4, 13, buf);
	sprintf(buf, "--------------------------------\n");
	consoleDrawText(0, 14, buf);
	
	consoleDrawText(19,27,"By - JRybicki");
}

void ClearStartup()
{
	//Clear out the header
	sprintf(buf, "                                 ");
	consoleDrawText(0, 12, buf);
	consoleDrawText(0, 14, buf);
	consoleDrawText(4, 13, buf);
	consoleDrawText(19,27,"             ");
	
	//Delay for a few NMI
	int vBlankCounter = 0;
	int vBlankDelay = 10;
	bool keepFade = true;
	
	while(keepFade)
	{
		if(vBlankCounter < vBlankDelay)
		{
			WaitForVBlank();
			vBlankCounter++;
		}
		else
		{
			keepFade = false;
		}
	}
}

bool SetConsoleMode(bool currentMode)
{
	bool oppositeMode;
	
	//Switch modes
	if(currentMode == true)
	{
		oppositeMode = false;
		consoleDrawText(0, 27, "Current Mode = Console");
	}
	else
	{
		oppositeMode = true;
		consoleDrawText(0, 27, "Current Mode = Command");
	}
	
	return oppositeMode;
}

void Menu()
{
    const int maxCmd = 2;
    
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
}

unsigned short pad0;
unsigned short pad1;
unsigned short prevPad;

void TextDemoRoutine()
{
    const int maxASCII = 0x256;
    unsigned short pad0Shift;
	
    pad0Shift = pad0 >> 4;
            
    //Let input settle out (When using emulator and AHK)
    if(prevPad != pad0)
    {
        //Draw text to the screen
        if(pad0Shift < maxASCII)
        {	
            //Draw the current line indicator
            if(pad0Shift != 0)
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

bool StartRoutine(bool isRunning)
{
    if(curCmd == 0)
    {
        if(isRunning == false)
        {  
            //Display the menu, default change nothing
            cmdRun = 0;
        }
    }
    else if(curCmd == 1)
    {
        if(isRunning == false)
        {  
            //Start text program and setup the screen
            isRunning = true;
            cmdRun = 1;
            
            curLine++;
            consoleDrawText(0, curLine, "->");
            //increment column indexer by 2
            col = 2;
        }
    }
    return isRunning;
}

void RunRoutine()
{
    switch (cmdRun) 
    {
        case 0:
            Menu();
            break;
        case 1:
            TextDemoRoutine();
            break;
        default:
            //nothing
            break;
    }
}

//---------------------------------------------------------------------------------
int main(void) 
{
	enum shellCMD
	{
		None     = 0,
		TextDemo = 1
	};	
    
    curLine = 0;
    col     = 0;
    vBlankTotal = 0;
    
    curCmd = 0;
    cmdRun = 0;

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
    bool isRoutineRunning = false;
	
	bool newLine = true;
	
	prevPad = 0;
	
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
       
		//Capture user inputs to start program
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
                isRoutineRunning = StartRoutine(isRoutineRunning);
			default:
				break;
		}
        
        //Run the subroutine
        RunRoutine();
        
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