#include <iostream>
#include <vector>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <math.h>
using namespace std;

int MARK_WIDTH = 2, MARK_NUMBERS = 4;
int CARD_WIDTH = MARK_WIDTH * MARK_NUMBERS;
int CARD_HEIGHT = 6;
int CARDS_WIDTH = 6, CARDS_HEIGHT = 8;
int MESSAGE_LEFT_EDGE = 2, MESSAGE_WIDTH = 40;
int MESSAGE_TOP_EDGE, MESSAGE_HEIGTH;
int SPACE_BETWEEN_MESSAGE_AND_CARDSBOARD_X = 1;
int CARDSBOARD_LEFT_EDGE = MESSAGE_LEFT_EDGE + MESSAGE_WIDTH - 1 +
    SPACE_BETWEEN_MESSAGE_AND_CARDSBOARD_X;
int CARDSBOARD_TOP_EDGE;
int SPACE_BETWEEN_CARDS_X = 2, SPACE_BETWEEN_CARDS_Y = 1;

int cardsFrameX0;
int cardsFrameY0;
int yPerFrame;
int xPerFrame;
int cardsFrameX1;
int cardsFrameY1;
int deltaX0;
int deltaX1;
int deltaY0;
int deltaY1;

WORD DEFAULT_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE ;
WORD BRIGHT_GREEN_COLOR = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
// bright-red FG white BG
WORD UNSELECTED_COLOR = FOREGROUND_RED | FOREGROUND_INTENSITY |
    BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
// white FG red BG
WORD SELECTED_COLOR = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED |
    BACKGROUND_RED;
// bright-blue FG blue BG
WORD CARDBACK_COLOR = FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_BLUE |
    BACKGROUND_GREEN;
// White BG
WORD WHITEBACK_COLOR = BACKGROUND_INTENSITY | BACKGROUND_RED |
    BACKGROUND_BLUE | BACKGROUND_GREEN;
// black FG
WORD BLACKLETTER_COLOR = BACKGROUND_INTENSITY | BACKGROUND_RED |
    BACKGROUND_BLUE | BACKGROUND_GREEN;
// red FG
WORD REDLETTER_COLOR = FOREGROUND_RED | BACKGROUND_INTENSITY |
    BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
// blue FG
WORD BLUELETTER_COLOR = FOREGROUND_BLUE;

string FRAME_MARK_ARR[9] = {"שÝ", "שÞ", "שß", "שא", "שב", "שג", "שד", "שה", "שו"};
string basic_color[] = {"Spade", "Heart", "Club", "Diamond"};

struct INFORMATION_CARDS
{
    int xFrame, yFrame;
    int xCard, yCard;
    int framePos;

    string color;
    int num, number;
    bool ifEmpty = false;

    INFORMATION_CARDS(int _i, int _j, int _num)
    {
        color = basic_color[(int)floor(_num / 13)];
        num = _num % 13 + 1;
        number = _num;
        xCard = (xPerFrame + 2) * _i + SPACE_BETWEEN_CARDS_X;
        yCard = (yPerFrame + 1) * _j + SPACE_BETWEEN_CARDS_Y;
        xFrame = (xPerFrame + 2) * _i + cardsFrameX0 + CARDSBOARD_LEFT_EDGE;
        yFrame = (yPerFrame + 1) * _j + cardsFrameY0 + CARDSBOARD_TOP_EDGE;
    }
};

vector <vector<INFORMATION_CARDS> > stackCards;

void gotoxy(int x, int y)
{
  COORD pos;
  pos.X = x;
  pos.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

string GetMove()
{
	int c = _getch();
    if (c == 13)
        return "enter";
	if (c == 224)
    {
		c = _getch();
        switch (c)
        {
            case 72:
                return "up";
            case 80:
                return "down";
            case 75:
                return "left";
            case 77:
                return "right";
        }
	}
	return "null";
}

void ShowACard(int x0, int y0, bool isFront, string color = "", int num = 0)
{
    int x = x0 + CARDSBOARD_LEFT_EDGE + 1;
    int y = y0 + CARDSBOARD_TOP_EDGE;
    if (isFront)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            WHITEBACK_COLOR);
        for (int i = 0; i < CARD_HEIGHT; i++)
        {
            gotoxy(x, y + i);
            for (int j = 0; j < MARK_NUMBERS; j++)
                cout << "  ";
        }
        if(color == "Spade" || color == "Club")
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                BLACKLETTER_COLOR);
        else if(color == "Heart" || color == "Diamond")
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                REDLETTER_COLOR);
        gotoxy(x, y);
        cout<<color;
        gotoxy(x, y + 1);
        cout<<num;
        gotoxy(x + CARD_WIDTH - color.size(), y + CARD_HEIGHT - 1);
        cout<<color;
        gotoxy(x + CARD_WIDTH - (num/10 + 1) , y + CARD_HEIGHT - 2);
        cout<<num;
    }
    else
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                CARDBACK_COLOR);
        for (int i = 0; i < CARD_HEIGHT; i++)
        {
            gotoxy(x, y + i);
            for (int j = 0; j < MARK_NUMBERS; j++)
                cout << "¡»";
        }
	}
}

void EmptyACard(int x0 , int y0)
{
    int x = x0 + CARDSBOARD_LEFT_EDGE + 1;
    int y = y0 + CARDSBOARD_TOP_EDGE;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                DEFAULT_COLOR);
    for (int i = 0; i < CARD_HEIGHT; i++)
    {
        gotoxy(x, y + i);
        for (int j = 0; j < MARK_NUMBERS; j++)
            cout << "  ";
    }
}

void ShowAFrame(int x0, int y0, int width, int height, bool ifSelected = false)
{
    if (ifSelected)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            BRIGHT_GREEN_COLOR);
    else
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            DEFAULT_COLOR);
    string frameCorner[4]= {"שÝ", "שß", "שד", "שו"};

    //left
    for ( int i = y0 + 1 ; i < y0 + height ; i++ )
    {
        gotoxy( x0, i );
        cout << "שר";
    }
    //right
    for ( int i = y0 + 1 ; i < y0 + height - 1 ; i++ )
    {
        gotoxy( x0 + width - 1, i );
        cout << "שר";
    }
    // up
    gotoxy( x0, y0 );
    cout << frameCorner[0];
    for ( int i = x0 ; i < x0 + width - 2 ; i++)
        cout << "שש";
    cout << frameCorner[1];
    //down
    gotoxy( x0, y0 + height - 1 );
    cout << frameCorner[2];
    for ( int i = x0 ; i < x0 + width - 2  ; i++)
        cout << "שש";
    cout << frameCorner[3];
}

void ShowLevelRules(int x0, int y0)
{
    gotoxy(x0, y0);
    cout << "           Memory Game            ";
    gotoxy(x0, y0 + 2);
    cout << " Please choose size of cardboard. ";
}

void ShowGameRules(int x0, int y0)
{
    string gameRules[] = {
        "           Memory Game         ",
        "Use arrow keys and Enter key to",
        "select cards.",
        "Eliminate cards by turning over",
        "any two matching cards.",
        "Win the game by eliminating all",
        "of cards!"};
    for(int i = 0; i < 7; i++){
        gotoxy(x0, y0 + i * 2);
        cout << gameRules[i] ;
    }
}

void Clean()
{
    int top = CARDSBOARD_TOP_EDGE + cardsFrameY1 + yPerFrame - 3;
    int left =  CARDSBOARD_LEFT_EDGE + cardsFrameX1 + xPerFrame - 3;
    for (int i = 0; i < top; i++)
    {
        gotoxy(0, 0 + i);
        for (int j = 0; j < left; j++)
            cout << " ";
    }
}

int ChooseLevel()
{
    int selectedLevel = 0;
    int levelNamePosX = 33;
    ShowAFrame(20, 3, MESSAGE_WIDTH, MESSAGE_HEIGTH+5,false);
    ShowLevelRules(24, 4);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SELECTED_COLOR);
    gotoxy(levelNamePosX, 10);
    cout << "    small    ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), UNSELECTED_COLOR);
    gotoxy(levelNamePosX, 13);
    cout << "    medium   ";
	gotoxy(levelNamePosX, 16);
	cout << "    large    ";
    gotoxy(43, 10);
	while (true)
	{
		string dir = GetMove();
        if (dir == "enter")
            break;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), UNSELECTED_COLOR);
		if (dir == "down" && selectedLevel == 0)
        {
            selectedLevel = 1;
            gotoxy(levelNamePosX, 10);
            cout << "    small    ";
            gotoxy(levelNamePosX, 13);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SELECTED_COLOR);
            cout << "    medium   ";
        }
		else if (dir == "down" && selectedLevel == 1)
		{
            selectedLevel = 2;
            gotoxy(levelNamePosX, 13);
            cout << "    medium   ";
            gotoxy(levelNamePosX, 16);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SELECTED_COLOR);
            cout << "    large    ";
        }
		else if (dir == "up" && selectedLevel == 1)
		{
            selectedLevel = 0;
            gotoxy(levelNamePosX, 13);
            cout << "    medium   ";
            gotoxy(levelNamePosX, 10);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SELECTED_COLOR);
            cout << "    small    ";
        }
		else if (dir == "up" && selectedLevel == 2)
		{
            selectedLevel = 1;
            gotoxy(levelNamePosX, 16);
            cout << "    large    ";
            gotoxy(levelNamePosX, 13);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SELECTED_COLOR);
            cout << "    medium   ";
        }
	}
    xPerFrame = SPACE_BETWEEN_CARDS_X + CARD_WIDTH;
    yPerFrame = SPACE_BETWEEN_CARDS_Y + CARD_HEIGHT;
    switch (selectedLevel)
    {
        case 0:
            CARDS_WIDTH = 4;
            CARDS_HEIGHT = 2;
            MESSAGE_TOP_EDGE = 0;
            MESSAGE_HEIGTH = yPerFrame;
            CARDSBOARD_TOP_EDGE = 0;
            break;
        case 1:
            CARDS_WIDTH = 4;
            CARDS_HEIGHT = 3;
            MESSAGE_TOP_EDGE = 2;
            MESSAGE_HEIGTH = yPerFrame*3/2;
            CARDSBOARD_TOP_EDGE = 0;
            break;
        case 2:
            CARDS_WIDTH = 4;
            CARDS_HEIGHT = 4;
            MESSAGE_TOP_EDGE = 2;
            MESSAGE_HEIGTH = yPerFrame * 2;
            CARDSBOARD_TOP_EDGE = 0;
            break;
        default:
            CARDS_WIDTH = 0;
            CARDS_HEIGHT = 0;
            break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
    gotoxy(levelNamePosX, 10);
    cout << "             ";
    gotoxy(levelNamePosX, 13);
    cout << "             ";
	gotoxy(levelNamePosX, 16);
	cout << "             ";
    cardsFrameX0 = floor((float)SPACE_BETWEEN_CARDS_X / 2);
    cardsFrameY0 = floor((float)SPACE_BETWEEN_CARDS_Y / 2);
    cardsFrameX1 = CARDS_WIDTH * xPerFrame + cardsFrameX0;
    cardsFrameY1 = CARDS_HEIGHT * yPerFrame + cardsFrameY0;
    deltaX0 = CARDSBOARD_LEFT_EDGE ;
    deltaX1 = CARDSBOARD_LEFT_EDGE + xPerFrame;
    deltaY0 = CARDSBOARD_TOP_EDGE;
    deltaY1 = CARDSBOARD_TOP_EDGE + yPerFrame;
	return selectedLevel;
}

COORD ChooseACard(COORD pos0)
{
    int i = pos0.X;
    int j = pos0.Y;
    int iTemp = pos0.X;
    int jTemp = pos0.Y;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), REDLETTER_COLOR);
    gotoxy(stackCards[i][j].xFrame, stackCards[i][j].yFrame);
    ShowAFrame(stackCards[i][j].xFrame, stackCards[i][j].yFrame,
        xPerFrame + 1, yPerFrame + 1, true);
    while (true)
	{
		string dir = GetMove();
		iTemp = i;
		jTemp = j;
        if ( dir == "enter" )
        {
            COORD pos;
            pos.X = i;
            pos.Y = j;
            return pos;
        }
		else if ( dir == "up" && j != 0)
        {
            jTemp = j;
            j --;
		}
        else if ( dir == "down" && j != CARDS_HEIGHT - 1)
        {
            jTemp = j;
            j ++;
        }
        else if ( dir == "left" && i != 0)
        {
            iTemp = i;
            i --;
        }
        else if ( dir == "right" && i != CARDS_WIDTH - 1)
        {
            iTemp = i;
            i ++;
        }
        ShowAFrame(stackCards[iTemp][jTemp].xFrame,
                stackCards[iTemp][jTemp].yFrame,
                xPerFrame + 1,
                yPerFrame + 1,
                false);
        ShowAFrame(stackCards[i][j].xFrame,
                stackCards[i][j].yFrame,
                xPerFrame + 1,
                yPerFrame + 1,
                true);
	}
}
vector<int> ShuffleCards(int width, int height)
{
    int shuffleTimes = 100;
    vector<int> cards;
    vector<int> allCardsPairs;
    for (int i = 0; i < 26; i++)
        allCardsPairs.push_back(i);
    for (int i = 0; i < 26 - width * height / 2; i++)
        allCardsPairs.erase(allCardsPairs.begin() + rand() % allCardsPairs.size());
    for (int i = 0; i < width * height / 2; i++)
    {
        cards.push_back(allCardsPairs[i]);
        cards.push_back(allCardsPairs[i] + 26);
    }
    for (int i = 0; i < shuffleTimes; i++) {
        int card1 = rand() % cards.size();
        cards.push_back(cards[card1]);
        cards.erase(cards.begin() + card1);
    }
    return cards;
}

void InitGameData()
{
    vector<int> cards;
    cards = ShuffleCards(CARDS_WIDTH, CARDS_HEIGHT);
    int k = 0;
    for ( int i = 0 ; i < CARDS_WIDTH; i++ )
    {
        vector<INFORMATION_CARDS> temp;
        for ( int j = 0 ; j < CARDS_HEIGHT; j++ )
        {
            temp.push_back(INFORMATION_CARDS(i, j, cards[k]));
            k++;
        }
        stackCards.push_back(temp);
    }
}

void InitGameUI() {
    Clean();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
    //set message frame
    ShowAFrame(MESSAGE_LEFT_EDGE, MESSAGE_TOP_EDGE, MESSAGE_WIDTH,
        MESSAGE_HEIGTH, false);
    ShowAFrame(MESSAGE_LEFT_EDGE, MESSAGE_TOP_EDGE + MESSAGE_HEIGTH,
        MESSAGE_WIDTH, 15, false);
    ShowGameRules(MESSAGE_LEFT_EDGE + 5,
        MESSAGE_TOP_EDGE + MESSAGE_HEIGTH + 1);
    for (int i = 0; i < CARDS_WIDTH; i++)
        for (int j = 0; j < CARDS_HEIGHT; j++)
        {
            //set cards
            ShowACard(stackCards[i][j].xCard, stackCards[i][j].yCard, false);

            //set cards frame
            ShowAFrame(stackCards[i][j].xFrame, stackCards[i][j].yFrame,
                xPerFrame + 1, yPerFrame + 1, false);
        }
}

bool IfTheyArePairs(COORD pos1, COORD pos2)
{
    int n1 = stackCards[pos1.X][pos1.Y].number;
    int n2 = stackCards[pos2.X][pos2.Y].number;
    if (n1 > n2)
        return n1 - n2 == 26;
    else if (n2 > n1 )
        return n2 - n1 == 26;
    else
        return false;
}

void PlayGame()
{
    COORD pos;
    pos.X = 0;
    pos.Y = 0;
    bool isOver = false;
    COORD tempCard1, tempCard2;
    int pairNumbers = 0;
    int k_line = 0;
    while(!isOver)
    {
        tempCard1.X = -1;
        tempCard2.X = -1;
        while(true)
        {
            pos = ChooseACard(pos);
            if(stackCards[pos.X][pos.Y].ifEmpty)
                continue;
            ShowACard(stackCards[pos.X][pos.Y].xCard, stackCards[pos.X][pos.Y].yCard,
                true, stackCards[pos.X][pos.Y].color, stackCards[pos.X][pos.Y].num);
            if (tempCard1.X == -1)
                tempCard1 = pos;
            else if (tempCard1.X != -1)
            {
                tempCard2 = pos;
                Sleep(1500);
                if (IfTheyArePairs(tempCard1, tempCard2))
                {
                    pairNumbers ++;
                    gotoxy(MESSAGE_LEFT_EDGE + 1 , MESSAGE_TOP_EDGE + 1 + k_line);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        DEFAULT_COLOR);
                    cout << "Eliminate "
                         << stackCards[tempCard1.X][tempCard1.Y].color << " "
                         << stackCards[tempCard1.X][tempCard1.Y].num
                         << " and "
                         << stackCards[tempCard2.X][tempCard2.Y].color << " "
                         << stackCards[tempCard2.X][tempCard2.Y].num <<"!";
                    k_line++;
                    stackCards[tempCard1.X][tempCard1.Y].ifEmpty = true;
                    stackCards[tempCard2.X][tempCard2.Y].ifEmpty = true;
                    EmptyACard(stackCards[tempCard1.X][tempCard1.Y].xCard,
                        stackCards[tempCard1.X][tempCard1.Y].yCard);
                    EmptyACard(stackCards[tempCard2.X][tempCard2.Y].xCard,
                        stackCards[tempCard2.X][tempCard2.Y].yCard);
                    gotoxy(MESSAGE_LEFT_EDGE + 1 , MESSAGE_TOP_EDGE + 4);
                    if(pairNumbers == CARDS_WIDTH * CARDS_HEIGHT / 2)
                    {
                        gotoxy(MESSAGE_LEFT_EDGE + 1 , MESSAGE_TOP_EDGE + 1 + k_line);
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            DEFAULT_COLOR);
                        cout << "Victory!" ;
                        isOver = true;
                        Sleep(1500);
                    }
                }
                else
                {
                    ShowACard(stackCards[tempCard1.X][tempCard1.Y].xCard,
                        stackCards[tempCard1.X][tempCard1.Y].yCard, false);
                    ShowACard(stackCards[tempCard2.X][tempCard2.Y].xCard,
                        stackCards[tempCard2.X][tempCard2.Y].yCard, false);
                }
                break;
            }
        }
    }
}

void EndGame()
{
    Clean();
    gotoxy(37, (CARDSBOARD_TOP_EDGE + cardsFrameY1 + yPerFrame) / 2);
    cout << "VICTORY";
    gotoxy(37 + 25,
           (CARDSBOARD_TOP_EDGE + cardsFrameY1 + yPerFrame) / 2 + 1);
    cout << "Author: JackyTsai\n";
    system("Pause");
}

int main()
{
	srand(time(NULL));
    ChooseLevel();
    InitGameData();
    system("mode con cols=93 lines=40");
    InitGameUI();
    PlayGame();
    EndGame();
    return 0;
}
