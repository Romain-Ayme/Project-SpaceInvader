/**
*
* @file    main.cpp
*
* @authors Romain AYME
*
* @version V1.0
* @date    2/1/2020
*
* @brief   main funtion
*
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <utility>    // max()

#include "nsutil.h"

using namespace std;



typedef vector <string> CVString;

const char KEmpty               = ' ';  // utile
const char TOKEN_INVADER        = 'W';
const char TOKEN_PLAYER         = 'A';
const char TOKEN_TORPEDO        = '|';  // For me
const char TOKEN_MISSILE        = 'T';  // For Invaders
const char KEY_RIGHT            = '6';
const char KEY_LEFT             = '4';
const char KEY_SHOOT            = '5';

const unsigned int SIZE_INVADERS      = 4;
const unsigned int KInvadersMiddle    = SIZE_INVADERS / 2;
const unsigned int KMySize            = 1;
const unsigned int KMyMiddle          = KMySize / 2;
const unsigned int KSizeLine          = 10;
const unsigned int KSizeSpace         = 10;
const unsigned int KBegInvader        = 0;
const unsigned int KBegMe             = KSizeLine / 2;

const unsigned int KRatioMeInvaders = 4;

string KEmptyLine (KSizeLine, KEmpty);

const string KInvadersForm (SIZE_INVADERS, TOKEN_INVADER);
const string KMyForm       (KMySize,       TOKEN_PLAYER);

const string KInvadersColor (KYellow);
const string KMyColor (KGreen);

struct CMyParam {
    std::map <std::string, char> MapParamChar;
    std::map <std::string, unsigned int> MapParamUnsigned;
    std::map <std::string, std::string> MapParamString;
};

const std::map <std::string, std::string> KColor {
    {"KReset", "0"},
    {"KBlack", "30"},
    {"KRed", "31"},
    {"KGreen", "32"},
    {"KYellow", "33"},
    {"KBlue", "34"},
    {"KMAgenta", "35"},
    {"KCyan", "36"},

    {"KBBlack", "40"},
    {"KBRed", "41"},
    {"KBGreen", "42"},
    {"KBYellow", "43"},
    {"KBBlue", "44"},
    {"KBMAgenta", "45"},
    {"KBCyan", "46"},
};

/**
*
* @brief   Function for init parametres of game
*
*/
void InitParams (CMyParam &Param) {
    //Move Keys
    Param.MapParamChar["KEY_LEFT"] = 'w';
    Param.MapParamChar["KEY_RIGHT"] = 'x';
    Param.MapParamChar["KEY_SHOOT"] = 'c';

    //Player token
    Param.MapParamChar["TOKEN_PLAYER"] = 'A';

    //Torpedo token
    Param.MapParamChar["TOKEN_TORPEDO"] = '|';

    //Invader token
    Param.MapParamChar["TOKEN_INVADER"] = 'W';

    //Missile token
    Param.MapParamChar["TOKEN_MISSILE"] = 'T';

    //Size of grid — suppose to be a rectangle (or a square)
    Param.MapParamUnsigned["SPACE_SIZE"] = 10;

    //Display Colors ok
    Param.MapParamString["COLOR_PLAYER"] = KColor.find("KRed")->second ;
    Param.MapParamString["COLOR_INVADER"] = KColor.find("KGreen")->second ;

    Param.MapParamString["COLOR_TORPEDO"] = KColor.find("KBlue")->second ;
    Param.MapParamString["COLOR_MISSILE"] = KColor.find("KCyan")->second ;
}



void DownShift (CVString &Space, unsigned int CurrentLine) {
    for (unsigned int i (CurrentLine); i > 0; --i)
        Space[i] = Space[i - 1];
    Space[0] = KEmptyLine;
} // DownShift()

bool IsDead (const CVString &Space, unsigned int Line, unsigned int Column, char Who) {
    return Space[Line][Column] != Who && (Space[Line][Column] == TOKEN_INVADER || Space[Line][Column] == TOKEN_PLAYER);
} // IsDead()

bool Shoot (CVString &Space, unsigned int Line, unsigned int Middle, char Projectile, char Who) {
    if (IsDead(Space, Line, Middle, Who)) {
        Space[Line] = KEmptyLine;
        return true;
    }
    Space[Line][Middle] = KEmpty == Space[Line][Middle] ? Projectile : KEmpty;
    return false;
} // Shoot()

bool ManageInvader (int &Increment, unsigned int &CurrentLine, unsigned int &Beg, bool &ToShoot, bool &Win, CVString &Space) {
    /*    */
    if ((Increment > 0 && (Beg + SIZE_INVADERS == KSizeLine || Space[CurrentLine][Beg + SIZE_INVADERS] != KEmpty)) || (Increment < 0 && (Beg  == 0 || Space[CurrentLine][Beg - 1] != KEmpty))) {
        ++CurrentLine;
        for (unsigned int j (Beg); j < Beg + SIZE_INVADERS; ++j) {
            if (TOKEN_TORPEDO == Space[CurrentLine][j]) {
                Win = true;
                Space[CurrentLine] = KEmptyLine;
                return false;
            }
        }
        Increment *= -1;
        DownShift(Space, CurrentLine);
        if (Space.size() - 1 <= CurrentLine)
            return true;
    }
    else {
        string ToShift(Space[CurrentLine].substr(Increment > 0 ? 0 : 1, KSizeLine - 1));
        Space[CurrentLine] = (Increment > 0) ? ' ' + ToShift : ToShift + ' ';
    }
    Beg +=Increment;
    ToShoot = ! ToShoot;
    /*    */
    if (ToShoot)
        return Shoot(Space, CurrentLine + 1, Beg + KInvadersMiddle, TOKEN_MISSILE, TOKEN_INVADER);
    /*    */

    return false;

} // ManageInvaders()

void Remove (CVString &Space, unsigned int Line, unsigned int Column) {
    char Form = Space[Line][Column];
    for (unsigned int j (Column); j < KSizeLine && Space[Line][j] == Form; ++j)
        Space[Line][j] = KEmpty;
    for (unsigned int j (Column); j-- > 0 && Space[Line][j] == Form; )
        Space[Line][j] = KEmpty;

} // Remove()

void RecomputeSpace (CVString &Space, bool &Win, bool &Lost) {
    /*    */
    for (unsigned int i (0); i < KSizeSpace; ++i) {
        for (unsigned int j (0); j < KSizeLine; ++j) {
            if (TOKEN_TORPEDO == Space[i][j]) {
                if (i != 0) {
                    if (TOKEN_INVADER == Space[i - 1][j]) {
                        Remove(Space, i - 1, j);
                        Win = true;
                    }
                    else if (TOKEN_MISSILE == Space[i - 1][j])
                        Space[i - 1][j] = KEmpty;
                    else
                        Space[i - 1][j] = TOKEN_TORPEDO;
                    Space[i][j]     = KEmpty;
                }
                else
                    Space[i][j] = KEmpty;
            }
            else if (TOKEN_MISSILE == Space[i][j]) {
                /* le probleme est de ne pas faire avancer le missile de plus d'un */
                if (i != KSizeSpace - 1) {
                    if (TOKEN_PLAYER == Space[i + 1][j]) {
                        Remove(Space, i + 1, j);
                        Lost = true;
                        Space[i][j]     = KEmpty;
                    }
                    else if (TOKEN_TORPEDO == Space[i + 1][j]) {
                        Space[i + 1][j] = KEmpty;
                        Space[i][j]     = KEmpty;
                    }
                    else {
                        if (i > 0 && TOKEN_MISSILE == Space[i - 1][j])
                            Space[i - 1][j] = KEmpty;
                        else
                            Space[i + 1][j] = TOKEN_MISSILE;
                    }
                }
                else if (TOKEN_MISSILE == Space[i - 1][j])

                    Space[i - 1][j] = KEmpty;
                else
                    Space[i][j] = KEmpty;
            }
        }
    }
    /*    */

} // RecomputeSpace()

void DisplaySpace (const CVString &Space, const CMyParam &myParams) {
    ClearScreen ();
    for (string Line : Space) {
        cout << "|";
        for (unsigned int Col (0); Col < Line.size(); ++Col) {

            // Affichage joueur
            if (Line [Col] == myParams.MapParamChar.find("TOKEN_PLAYER")->second)
                Color(myParams.MapParamString.find("COLOR_PLAYER")->second);

            // Affichage invader
            else if (Line [Col] == myParams.MapParamChar.find("TOKEN_INVADER")->second)
                Color(myParams.MapParamString.find("COLOR_INVADER")->second);

            // Affichage Torpedo
            else if (Line [Col] == myParams.MapParamChar.find("TOKEN_TORPEDO")->second)
                Color(myParams.MapParamString.find("COLOR_TORPEDO")->second);

            // Affichage Missile
            else if (Line [Col] == myParams.MapParamChar.find("TOKEN_MISSILE")->second)
                Color(myParams.MapParamString.find("COLOR_MISSILE")->second);

            cout << Line [Col];
            Color(KReset);
        }
        cout << "|" << endl;
    }

} // DisplaySpace()

bool ManageMe (CVString &Space, unsigned int &Pos, bool &Lost, const CMyParam &myParams) {
    char C;
    cin >> C;

    unsigned int Line = Space.size() - 1;
    string NewLastLine = Space[Line];

    const char LEFT  = myParams.MapParamChar.find("KEY_LEFT")->second;
    const char RIGHT = myParams.MapParamChar.find("KEY_RIGHT")->second;
    const char SHOOT = myParams.MapParamChar.find("KEY_SHOOT")->second;


    if (C == LEFT) {
        if (Pos > 0) {
            --Pos;
            if (IsDead(Space, Line, Pos, TOKEN_PLAYER)) {
                NewLastLine = KEmptyLine;
                Lost = true;
            }
            else {
                NewLastLine = Space[Line].substr (0, Pos) +
                        Space[Line].substr (Pos + 1) + ' ';
            }
        }
    }
    else if (C == RIGHT) {
        if (Pos < KSizeLine - 1) {
            ++Pos;
            if (IsDead(Space, Line, Pos, TOKEN_PLAYER)) {
                NewLastLine = KEmptyLine;
                Lost = true;
            }
            else
                NewLastLine = Space[Line].substr (0, Pos - 1) + ' ' + Space[Line].substr(Pos - 1, KSizeLine - Pos);
        }
    }
    else if (C == SHOOT) {
        return Shoot(Space, Line - 1, Pos, TOKEN_TORPEDO, TOKEN_PLAYER);
    }
    Space[Line] = NewLastLine;

    /*    */
    return false;

} // ManageMe()

// Ne marche pas car case ne prend pas en compte les maps
bool ManageMe_old (CVString &Space, unsigned int &Pos, bool &Lost) {
    char C;
    cin >> C;

    unsigned int Line = Space.size() - 1;
    string NewLastLine = Space[Line];

    switch (C) {
    case KEY_LEFT    :
        if (Pos > 0) {
            --Pos;
            if (IsDead(Space, Line, Pos, TOKEN_PLAYER)) {
                NewLastLine = KEmptyLine;
                Lost = true;
            }
            else {
                NewLastLine = Space[Line].substr (0, Pos) +
                        Space[Line].substr (Pos + 1) + ' ';
            }
        }
        break;
    case KEY_RIGHT   :
        if (Pos < KSizeLine - 1) {
            ++Pos;
            if (IsDead(Space, Line, Pos, TOKEN_PLAYER)) {
                NewLastLine = KEmptyLine;
                Lost = true;
            }
            else
                NewLastLine = Space[Line].substr (0, Pos - 1) + ' ' + Space[Line].substr(Pos - 1, KSizeLine - Pos);
        }
        break;
    case KEY_SHOOT :
        return Shoot(Space, Line - 1, Pos, TOKEN_TORPEDO, TOKEN_PLAYER);
    }
    Space[Line] = NewLastLine;

    /*    */
    return false;

} // ManageMe()

void InitSpace (CVString &Space, unsigned int Size) {
    Space.resize (Size);
    for (string &Line : Space)
        Line = KEmptyLine;
    Space[0] = KEmptyLine.substr (0, KBegInvader) + KInvadersForm + KEmptyLine.substr(0, KSizeLine - KBegInvader - SIZE_INVADERS);
    Space[Space.size() - 1] = KEmptyLine.substr (0, KBegMe) + KMyForm + KEmptyLine.substr(0, KSizeLine - KBegMe - KMySize);
} // InitSpace()

void SpaceInvaders (void) {
    CVString Space;
    CMyParam myParams;
    InitParams(myParams);
    InitSpace(Space, KSizeSpace);
    int Direction (1);
    unsigned int CurrentLine (0);
    unsigned int PosInvader (KBegInvader);
    unsigned int PosMe      (KBegMe);

    bool IWin           (false);
    bool ILoose         (false);
    bool InvadersShoots (false);

    DisplaySpace(Space, myParams);
    GotoXY(7, 12);

    for (;;) {
        for (unsigned int i (max (2u, KRatioMeInvaders)); i-- > 0; ) {
            if (!ManageMe(Space, PosMe, ILoose, myParams) && ! ILoose)
                RecomputeSpace(Space, IWin, ILoose);
            DisplaySpace(Space, myParams);
            GotoXY(7, 12);
            if (ILoose || IWin) break;
        }
        if (ILoose || IWin) break;
        ILoose = ManageInvader(Direction, CurrentLine, PosInvader, InvadersShoots, IWin, Space);
        if (! IWin && ! ILoose)
            RecomputeSpace(Space, IWin, ILoose);
        DisplaySpace(Space, myParams);
        GotoXY(7, 12);
        if (ILoose || IWin) break;
    }
    if (IWin)
        cout << "J'ai gagné" << endl;
    else
        cout << "J'ai perdu" << endl;
} // SpaceInvaders()

void LoadParams(CMyParam &Param)
{
    ifstream ifs ("../Project-SpaceInvader/settings.yml");
    if ( !ifs.is_open()){
        cerr << "error : file are missing or can't be open" << endl;
        exit(-1);
    }
    string cle;
    for (unsigned int i (0); i < 4; ++i) {
        ifs >> cle;
        char tmp;
        ifs >> tmp; //pour le ":"
        unsigned int val;
        ifs >> val;
        Param.MapParamUnsigned[cle] = val;
    }
    for (unsigned int i (0); i < 2; ++i) {
        ifs >> cle;
        char tmp;
        ifs >> tmp; //pour le ":"
        string val;
        ifs >> val;
        Param.MapParamString[cle] = val;
    }
    /*// C'est un test, je ne sais pas si ça marche
    while (ifs.eof() == false) {
        ifs >> cle;
        char tmp;
        ifs >> tmp; //pour le ":"

        if ((cle.substr(0,4) == "KEY_") || (cle.substr(0,6) == "TOKEN_")) {
            char val;
            ifs >> val;
            Param.MapParamChar[cle] = val;
        }
    }*/
} // LoadParams()

struct AuthorizedKey {
    /** List of authorized key for the type char in a struct CMyParam */
    const vector<string> VParamChar {"KEY_LEFT", "KEY_RIGHT", "KEY_SHOOT", "TOKEN_PLAYER", "TOKEN_INVADER", "TOKEN_TORPEDO", "TOKEN_MISSILE"};

    /** List of authorized key for the type string in a struct CMyParam */
    const vector<string> VParamString {"COLOR_PLAYER", "COLOR_INVADER", "COLOR_TORPEDO", "COLOR_MISSILE"};

    /** List of authorized key for the type unsigned in a struct CMyParam */
    const vector<string> VParamUnsigned {"SPACE_SIZE"};
};

//---------------------------------

void StartMenu () {
    /*ifstream ifs ("../Project-SpaceInvader/Invader.txt");
    if (!ifs.is_open())
        cerr << "error : file are missing or can't be open" << endl;
    else {
        // Affiche le logo
        unsigned int Cpt (5);
        while (!ifs.eof()) {
            string Line;
            getline(ifs, Line);
            GotoXY(33, Cpt);
            cout << Line;
            ++Cpt;
        }
        ifs.close();

        ifstream ifs ("../Project-SpaceInvader/SpaceInvadertext.txt");
        if (!ifs.is_open())
            cerr << "error : file are missing or can't be open" << endl;
        else {
            // Affiche le Texte
            unsigned int Cpt (14);
            while (!ifs.eof()) {
                string Line;
                getline(ifs, Line);
                GotoXY(11, Cpt);
                cout << Line;
                ++Cpt;
            }
            GotoXY(29,22);
            cout << "Press enter to continue" << endl;
            //Delay(2000);
            getchar();
            GotoXY(29,22);
            cout << "                       " ;
            //            ClearScreen();
        }
    }*/

    DisplaysDoc("../Project-SpaceInvader/assets/Invader.txt", 33, 5);
    DisplaysDoc("../Project-SpaceInvader/assets/SpaceInvadertext.txt", 11, 14);
    GotoXY(29,22);
    cout << "Press enter to continue" << endl;
    getchar();
    GotoXY(29,22);
    cout << "                       " ;

    // Menu
    int X (4);
    int Y (18);
    GotoXY(X,++Y);
    cout << "1 : Start" << endl;
    GotoXY(X,++Y);
    cout << "2 : Show config" << endl;
    GotoXY(X,++Y);
    cout << "3 : Show regle" << endl;
    GotoXY(X,++Y);
    cout << "4 : Credits" << endl;
    GotoXY(X,++Y);
    cout << "5 : Exit" << endl;

    char C;
    cin >> C;

    switch (C) {
    case '1'    :
        SpaceInvaders();
        break;
    case '2'   :
        cout << "Config";
        break;

    case '3'   :
        cout << "Config 3";
        break;
    case '4'   :
        ClearScreen();
        DisplaysDoc("../Project-SpaceInvader/assets/Credit.txt");
        break;
    case '5'   :
        ClearScreen();
        DisplaysDoc("../Project-SpaceInvader/assets/End.txt", 20, 10);
        GotoXY(0,23);
        exit(-1);
        break;

    case 'r' | 'R'  :
        cout << "R mode";
        break;
    }

}
int main () {
    StartMenu();
    //SpaceInvaders();
    return 0;
} // main()
