#include "Includes.h"

int main(void)
{
    setlocale(LC_ALL, "en_US.utf8");

    wc board[8][8] = {
        {L'♜',L'♞',L'♝',L'♛',L'♚',L'♝',L'♞',L'♜'},
        {L'♟',L'♟',L'♟',L'♟',L'♟',L'♟',L'♟',L'♟'},
        {L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '},
        {L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '},
        {L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '},
        {L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '},
        {L'♙',L'♙',L'♙',L'♙',L'♙',L'♙',L'♙',L'♙'},
        {L'♖',L'♘',L'♗',L'♕',L'♔',L'♗',L'♘',L'♖'}
    };
    GameState state = S_NEUTRAL;
    Color turn = C_WHITE;
    while(state != S_MATE_W){
        const Move move = getColorsMove(board, turn);
        applyMove(board, move);
        turn = nextTurnColor(turn);
    }

    return 0;
}
