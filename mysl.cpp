#define D51HIGHT     10
#define D51FUNNEL    7
#define D51LENGTH    83
#define D51PATTERNS  6
 
#define D51STR1  "      ====        ________                ___________ "
#define D51STR2  "  _D _|  |_______/        \\__I_I_____===__|_________| "
#define D51STR3  "   |(_)---  |   H\\________/ |   |        =|___ ___|   "
#define D51STR4  "   /     |  |   H  |  |     |   |         ||_| |_||   "
#define D51STR5  "  |      |  |   H  |__--------------------| [___] |   "
#define D51STR6  "  | ________|___H__/__|_____/[][]~\\_______|       |   "
#define D51STR7  "  |/ |   |-----------I_____I [][] []  D   |=======|__ "
 
#define D51WHL11 "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__ "
#define D51WHL12 " |/-=|___|=    ||    ||    ||    |_____/~\\___/        "
#define D51WHL13 "  \\_/      \\O=====O=====O=====O_/      \\_/            "
 
#define D51WHL21 "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__ "
#define D51WHL22 " |/-=|___|=O=====O=====O=====O   |_____/~\\___/        "
#define D51WHL23 "  \\_/      \\__/  \\__/  \\__/  \\__/      \\_/            "
 
#define D51WHL31 "__/ =| o |=-O=====O=====O=====O \\ ____Y___________|__ "
#define D51WHL32 " |/-=|___|=    ||    ||    ||    |_____/~\\___/        "
#define D51WHL33 "  \\_/      \\__/  \\__/  \\__/  \\__/      \\_/            "
 
#define D51WHL41 "__/ =| o |=-~O=====O=====O=====O\\ ____Y___________|__ "
#define D51WHL42 " |/-=|___|=    ||    ||    ||    |_____/~\\___/        "
#define D51WHL43 "  \\_/      \\__/  \\__/  \\__/  \\__/      \\_/            "
 
#define D51WHL51 "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__ "
#define D51WHL52 " |/-=|___|=   O=====O=====O=====O|_____/~\\___/        "
#define D51WHL53 "  \\_/      \\__/  \\__/  \\__/  \\__/      \\_/            "
 
#define D51WHL61 "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__ "
#define D51WHL62 " |/-=|___|=    ||    ||    ||    |_____/~\\___/        "
#define D51WHL63 "  \\_/      \\_O=====O=====O=====O/      \\_/            "
 
#define D51DEL   "                                                      "
 
#define COAL01 "                              "
#define COAL02 "                              "
#define COAL03 "    _________________         "
#define COAL04 "   _|                \\_____A  "
#define COAL05 " =|                        |  "
#define COAL06 " -|                        |  "
#define COAL07 "__|________________________|_ "
#define COAL08 "|__________________________|_ "
#define COAL09 "   |_D__D__D_|  |_D__D__D_|   "
#define COAL10 "    \\_/   \\_/    \\_/   \\_/    "
 
#define COALDEL "                              "
 
#define LOGOHIGHT        6
#define LOGOFUNNEL       4
#define LOGOLENGTH      84
#define LOGOPATTERNS     6
 
#define LOGO1  "     ++      +------ "
#define LOGO2  "     ||      |+-+ |  "
#define LOGO3  "   /---------|| | |  "
#define LOGO4  "  + ========  +-+ |  "
 
#define LWHL11 " _|--O========O~\\-+  "
#define LWHL12 "//// \\_/      \\_/    "
 
#define LWHL21 " _|--/O========O\\-+  "
#define LWHL22 "//// \\_/      \\_/    "
 
#define LWHL31 " _|--/~O========O-+  "
#define LWHL32 "//// \\_/      \\_/    "
 
#define LWHL41 " _|--/~\\------/~\\-+  "
#define LWHL42 "//// \\_O========O    "
 
#define LWHL51 " _|--/~\\------/~\\-+  "
#define LWHL52 "//// \\O========O/    "
 
#define LWHL61 " _|--/~\\------/~\\-+  "
#define LWHL62 "//// O========O_/    "
 
#define LCOAL1 "____                 "
#define LCOAL2 "|   \\@@@@@@@@@@@     "
#define LCOAL3 "|    \\@@@@@@@@@@@@@_ "
#define LCOAL4 "|                  | "
#define LCOAL5 "|__________________| "
#define LCOAL6 "   (O)       (O)     "
 
#define LCAR1  "____________________ "
#define LCAR2  "|  ___ ___ ___ ___ | "
#define LCAR3  "|  |_| |_| |_| |_| | "
#define LCAR4  "|__________________| "
#define LCAR5  "|__________________| "
#define LCAR6  "   (O)        (O)    "
 
#define DELLN  "                     "

#include <windows.h>
#include <stdio.h>
#include <string.h>
//#include "sl.h"
 
#define COLS 20
#define LINES 50
int LOGO      = 0;
int FLY       = 0;
#define ERR  0
#define OK  1
HANDLE hOut ;
void gotoxy(HANDLE hOut, int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hOut, pos); /* 设置光标位置 */
}
 

/**在屏幕指定坐标显示字符串(curses方式)**/
int my_mvaddstr(int y, int x, char *str)
{
    for ( ; x < 0; ++x, ++str)
    {
        if (*str == '\0') 
            return ERR;
    }
 
    for ( ; *str != '\0'; ++str, ++x)
    {
        gotoxy(hOut,x,y);
        printf("%c",*str);
    }
 
    return OK;
}
/**增加一个人物在喊help**/
int add_man(int y, int x)
{
    static char *man[2][2] = {{"", "(O)"}, {"Help!", "\\O/"}};
    int i;
 
    for (i = 0; i < 2; ++i)
    {
        my_mvaddstr(y + i, x, man[(LOGOLENGTH + x) / 12 % 2][i]);
    }
 
    return OK;
}
 
/**增加火车头烟雾的动画**/
int add_smoke(int y, int x)
#define SMOKEPTNS   16
{
    static struct smokes
    {
        int y, x;
        int ptrn, kind;
    } S[1000];
     
    static int sum = 0;
 
    static char *Smoke[2][SMOKEPTNS]
    = {{"(   )", "(    )", "(    )", "(   )", "(  )",
        "(  )" , "( )"   , "( )"   , "()"   , "()"  ,
        "O"    , "O"     , "O"     , "O"    , "O"   ,
        " "                                          },
       {"(@@@)", "(@@@@)", "(@@@@)", "(@@@)", "(@@)",
        "(@@)" , "(@)"   , "(@)"   , "@@"   , "@@"  ,
        "@"    , "@"     , "@"     , "@"    , "@"   ,
        " "                                          }};
 
    static char *Eraser[SMOKEPTNS]
    =  {"     ", "      ", "      ", "     ", "    ",
        "    " , "   "   , "   "   , "  "   , "  "  ,
        " "    , " "     , " "     , " "    , " "   ,
        " "                                          };
 
    static int dy[SMOKEPTNS] = { 2,  1, 1, 1, 0, 0, 0, 0, 0, 0,
                 0,  0, 0, 0, 0, 0             };
 
    static int dx[SMOKEPTNS] = {-2, -1, 0, 1, 1, 1, 1, 1, 2, 2,
                 2,  2, 2, 3, 3, 3             };
    int i;
 
    if (x % 4 == 0)
    {
        for (i = 0; i < sum; ++i)
        {
            my_mvaddstr(S[i].y, S[i].x, Eraser[S[i].ptrn]);
            S[i].y    -= dy[S[i].ptrn];
            S[i].x    += dx[S[i].ptrn];
            S[i].ptrn += (S[i].ptrn < SMOKEPTNS - 1) ? 1 : 0;
            my_mvaddstr(S[i].y, S[i].x, Smoke[S[i].kind][S[i].ptrn]);
        }
 
        my_mvaddstr(y, x, Smoke[sum % 2][0]);
        S[sum].y = y;    
        S[sum].x = x;
        S[sum].ptrn = 0; 
        S[sum].kind = sum % 2;
        sum ++;
    }
 
    return OK;
}

 
void Cls(HANDLE hConsole)
{
    COORD coordScreen = {0, 0};
 
    BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
 
    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
 
    bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten);
    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
    bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
}
 

 
/**程序运行的参数选项**/
void option(char *str)
{
    extern int FLY, LOGO;   /**引用本文件全局变量ACCIDENT, FLY, LOGO**/
 
    while (*str != '\0')            /**顺序向后寻找字符串中的正确的选项字符   **/
    {
        switch (*str++) 
        {
            case 'F': 
                FLY      = 1; 
                break;
            case 'l': 
                LOGO     = 1; 
                break;
            default:
                break;
        }
    }
}
 
 
/**显示老式火车的字符填充方式**/
int add_sl(int x)
{
    static char *sl[LOGOPATTERNS][LOGOHIGHT + 1]
    = {{LOGO1, LOGO2, LOGO3, LOGO4, LWHL11, LWHL12, DELLN},
       {LOGO1, LOGO2, LOGO3, LOGO4, LWHL21, LWHL22, DELLN},
       {LOGO1, LOGO2, LOGO3, LOGO4, LWHL31, LWHL32, DELLN},
       {LOGO1, LOGO2, LOGO3, LOGO4, LWHL41, LWHL42, DELLN},
       {LOGO1, LOGO2, LOGO3, LOGO4, LWHL51, LWHL52, DELLN},
       {LOGO1, LOGO2, LOGO3, LOGO4, LWHL61, LWHL62, DELLN}};
 
    static char *coal[LOGOHIGHT + 1]
    = {LCOAL1, LCOAL2, LCOAL3, LCOAL4, LCOAL5, LCOAL6, DELLN};
 
    static char *car[LOGOHIGHT + 1]
    = {LCAR1, LCAR2, LCAR3, LCAR4, LCAR5, LCAR6, DELLN};
 
    int i, y, py1 = 0, py2 = 0, py3 = 0;
     
    if (x < - LOGOLENGTH) 
        return ERR;
 
    y = LINES/2 - 3;
 
    if (FLY == 1) 
    {
        y = (x / 6) + LINES - (COLS / 6) - LOGOHIGHT;
        py1 = 2; 
        py2 = 4; 
        py3 = 6;
    }
 
    for (i = 0; i <= LOGOHIGHT; ++i) 
    {
        printf(sl[(LOGOLENGTH + x) / 3 % LOGOPATTERNS][i]);
        my_mvaddstr(y + i, x, sl[(LOGOLENGTH + x) / 3 % LOGOPATTERNS][i]);
        my_mvaddstr(y + i + py1, x + 21, coal[i]);
        my_mvaddstr(y + i + py2, x + 42, car[i]);
        my_mvaddstr(y + i + py3, x + 63, car[i]);
    }
 
 
    add_man(y + 1, x + 14);
    add_man(y + 1 + py2, x + 45);
    add_man(y + 1 + py2, x + 53);
    add_man(y + 1 + py3, x + 66);
    add_man(y + 1 + py3, x + 74);
 
    add_smoke(y - 1, x + LOGOFUNNEL);
 
    return OK;
}
 
/**显示普通火车的字符填充方式**/
int add_D51(int x)
{
    int y, i, dy = 0;
     
    /**显示火车头不同运动状态的车身的各个部分的字符串数组，一个火车头共11行字**/
    /**符串D51STR1-7为车身，D51WHLX1-X3为车轮，D51DEL为车底部空行，运动状态共**/
    /**6种区别在于轮子的运动状态，所以分为6个数组                            **/
    static char *d51[D51PATTERNS][D51HIGHT + 1]
    = {{D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
        D51WHL11, D51WHL12, D51WHL13, D51DEL},
       {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
        D51WHL21, D51WHL22, D51WHL23, D51DEL},
       {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
        D51WHL31, D51WHL32, D51WHL33, D51DEL},
       {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
        D51WHL41, D51WHL42, D51WHL43, D51DEL},
       {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
        D51WHL51, D51WHL52, D51WHL53, D51DEL},
       {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
        D51WHL61, D51WHL62, D51WHL63, D51DEL}};
 
    /**显示火车车厢的数组，共11行，为固定数组                              **/
    static char *coal[D51HIGHT + 1]
    = {COAL01, COAL02, COAL03, COAL04, COAL05,
       COAL06, COAL07, COAL08, COAL09, COAL10, COALDEL};
 
    /**火车头的显示宽度共83列                                              **/
    if (x < - D51LENGTH) 
    {
        return ERR;
    }
 
    /**显示高度为"(终端窗体的高度)/2-5"                                      **/
    y = LINES/2 - 5;
 
    if (FLY == 1) 
    {
        y = (x / 7) + LINES - (COLS / 7) - D51HIGHT;
        dy = 1;
    }
 
    /**绘制火车的动画造型                                                     **/
    for (i = 0; i <= D51HIGHT; ++i)
    {
        my_mvaddstr(y + i, x, d51[(D51LENGTH + x) % D51PATTERNS][i]);
        my_mvaddstr(y + i + dy, x + 53, coal[i]);
    }
 
    /**增加火车中呼救的人的动画                                              **/
    add_man(y + 2, x + 43);
    add_man(y + 2, x + 47);
 
    /**增加火车头的冒烟动画                                                    **/
    add_smoke(y - 1, x + D51FUNNEL);
 
    return OK;
}
 

/**主函数**/
void main(int argc, char *argv[])
{
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    int x, i;
 
    for (i = 1; i < argc; ++i)       /**对程序带的参数做出判断处理            **/
    {
        if ('-' == *argv[i]) 
        {
            option(argv[i] + 1);    /**获取操作选项                           **/
        }
    }
 
    gotoxy(hOut,1, 1);
    for (x = COLS - 1;;--x)         /**根据终端上的列数进行循环             **/
    {
        if (LOGO == 0)
        {
            if (add_D51(x) == ERR)  /**填充终端的每一行的显示字符内容      **/
                break;
        } 
        else
        {
            if (add_sl(x) == ERR)   /**填充终端的每一行的显示字符内容      **/
                break;
        }
        Sleep(40);
     }
                /**终端关闭curses模式                 **/
}