#ifndef TYPES_H
#define TYPES_H

typedef wchar_t wc;
typedef unsigned int uint;

typedef enum{S_NEUTRAL, S_CHECK_W, S_CHECK_B, S_MATE_W, S_MATE_B}GameState;
typedef enum{C_NONE, C_WHITE, C_BLACK}Color;
typedef enum{M_INVALID, M_VALID, M_CAPTURE}MoveType;
typedef enum{D_U, D_R, D_D, D_L}Dir;
typedef enum{P_DEFAULT, P_COLORS, P_MOVES}PrintType;

typedef union{
    int arr[2];
    struct{
        int x;
        int y;
    };
    struct{
        int col;
        int row;
    };
    struct{
        int w;
        int h;
    };
}Pairs;

typedef union{
    uint arr[2];
    struct{
        uint x;
        uint y;
    };
    struct{
        uint col;
        uint row;
    };
    struct{
        uint w;
        uint h;
    };
}Pairu;

typedef struct{
    Pairu selected;
    Pairu targeted;
}Move;

void clearTerm(void)
{
    wprintf(__extension__(L"\e[1;1H\e[2J"));
}

bool eqPairu(const Pairu p1, const Pairu p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

bool eqPairs(const Pairs p1, const Pairs p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

bool dirAlgnX(const Dir dir)
{
    return dir == D_L || dir == D_R;
}

bool dirAlgnY(const Dir dir)
{
    return dir == D_U || dir == D_D;
}

Dir dirInv(const Dir dir)
{
    return dir^(1u<<1);
}

Dir dirRor(const Dir dir)
{
    return (dir+1)%4;
}

Dir dirRol(const Dir dir)
{
    return (dir+3)%4;
}

Color colorInv(const Color color)
{
    if(color == C_WHITE)
        return C_BLACK;
    if(color == C_BLACK)
        return C_WHITE;
    return C_NONE;
}

#endif /* end of include guard: TYPES_H */
