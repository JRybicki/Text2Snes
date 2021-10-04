#include <iostream>
#include <vector>

/**
Register    Serial     Default
Bit         Transfer   Purpose
Number______Order______(Joypads)_____
15          1st        B Button         (1=Low=Pressed)
14          2nd        Y Button
13          3rd        Select Button
12          4th        Start Button
11          5th        Up
10          6th        Down
9           7th        Left
8           8th        Right
7           9th        A Button
6           10th       X Button
5           11th       L Button
4           12th       R Button
3           13th       0 (High)
2           14th       0 (High)
1           15th       0 (High)
0           16th       0 (High)

**/

static const int maxAsciiBits = 7;
static const int maxLineSize = 20;
char inputsL[] = { '1', '2', '3', '4', '5', '6', '7' }; //w, enter, tab
char inputsR[] = { '7', '6', '5', '4', '3', '2', '1' };

static const char keySet    = '1';
static const char spaceChar = ' ';

std::string binary(unsigned x);

int main()
{
    unsigned int charAscii[maxLineSize];
    char binChars[maxLineSize][maxAsciiBits];

    std::vector<std::string> digitVector;
    std::vector<std::string> asciiVector;

    std::string strMsg = "Hello World! \ntest new line";
    
    //Calculate ascii and create binary strings
    for (unsigned int i = 0; i < strMsg.length(); i++)
    {
        charAscii[i] = (int)strMsg[i];

        std::string digitsStr = binary(charAscii[i]);
        digitVector.push_back(digitsStr);

        std::cout << strMsg[i] << " " << std::hex << charAscii[i] << " " << digitsStr << std::endl;
    }

    //Create ascii string from binary string
    for (auto it = std::begin(digitVector); it != std::end(digitVector); ++it)
    {
        std::string asciiStr;

        for (int i = 0; i < it->length(); i++)
        {
            //check if bit it set
            if (it->at(i) == keySet)
            {
                asciiStr.push_back(inputsR[i]);
                asciiStr.push_back(spaceChar);
            }
        }
        std::cout << "Keys to Press: " << asciiStr << std::endl;
        asciiVector.push_back(asciiStr);
    }

    for (auto it = std::begin(asciiVector); it != std::end(asciiVector); ++it)
    {
        char buffer[200];
        sprintf_s(buffer, "C:\\snesdev\\snes-roms\\Strings\\input.exe %s", it->c_str());

        std::system(buffer);
    }


}

std::string binary(unsigned x)
{
    std::string s;
    int counter = 0;
    do
    {
        s.push_back('0' + (x & 1));
        counter++;
    } while (x >>= 1);
    //Fillout at least 7 chars for single byte
    while (counter < 7)
    {
        s.push_back('0');
        counter++;
    }

    std::reverse(s.begin(), s.end());
    return s;
}