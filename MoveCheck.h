#ifndef MOVECHECK_H
#define MOVECHECK_H

bool validPos(const Vec2 pos, const bool throw)
{
    if(pos.x < 0 || pos.x >= 8 || pos.y < 0 || pos.y >= 8){
        if(throw){
            fwprintf(stderr, L"Error: %ls out of bounds\n", vec2Strify(pos));
            exit(EXIT_FAILURE);
        }else{
            return false;
        }
    }
    return true;
}

bool getValidAt(Valid moves, const Vec2 pos, const bool throw)
{
    validPos(pos, throw);
    return moves[pos.y][pos.x];
}

void setValidAt(Valid moves, const Vec2 pos, const bool type, const bool throw)
{
    validPos(pos, throw);
    moves[pos.y][pos.x] = type;
}

void resetValid(Valid moves)
{
    for(uint i = 0; i < 8; i++)
        memset(moves[i], false, sizeof(bool)*8);
}

uint validCount(Valid moves)
{
    uint total = 0;
    for(uint y = 0; y < 8; y++)
        for(uint x = 0; x < 8; x++)
            total += getValidAt(moves, (const Vec2){.x = x, .y = y}, true);

    return total;
}

uint cast(Board board, Valid moves, const Vec2 src, const Dir dir, const uint dist)
{
    if(!validPos(src, true))
        return 0;

    const Color srcColor = pieceColor(pieceAt(board, src));
    if(srcColor == C_NONE)
        return 0;

    uint total = 0;
    const uint dirDist = dist ? dist : 8;

    for(uint i = 1; i <= dirDist; i++){
        const Vec2 dst = shift(src, dir, i);

        if(!validPos(dst, false))
            return total;

        total++;
        setValidAt(moves, dst, true, true);

        if(colorInv(pieceColor(pieceAt(board, dst))) == srcColor)
            return total;
    }
    return total;
}

uint knightMoves(Board board, Valid moves, const Vec2 src)
{
    if(!validPos(src, true))
        return 0;

    const Piece srcPiece = pieceAt(board, src);
    if(srcPiece != P_KNIGHT_B && srcPiece != P_KNIGHT_W){
        fwprintf(stderr, L"Error: cannot get knightMoves for non knight at %ls\n", pos);
        exit(EXIT_FAILURE);
    }
    const Color srcColor = pieceColor(srcPiece);

    uint total = 0;
    for(Dir i = 0; i < 4; i++){
        Vec2 dst;
        Vec2 fork = shift(src, i, 1);

        if(validPos(dst = shift(fork,dirRoR(i),2), false)){
            if(pieceColor(pieceAt(board,dst)) != srcColor){
                setValidAt(moves, dst, true, true);
                total++;
            }
        }
        if(validPos(dst = shift(fork,dirRoL(i),2), false)){
            if(pieceColor(pieceAt(board,dst)) != srcColor){
                setValidAt(moves, dst, true, true);
                total++;
            }
        }

        fork = shift(fork, i, 1);

        if(validPos(dst = shift(fork,dirRoR(i),1), false)){
            if(pieceColor(pieceAt(board,dst)) != srcColor){
                setValidAt(moves, dst, true, true);
                total++;
            }
        }
        if(validPos(dst = shift(fork,dirRoL(i),1), false)){
            if(pieceColor(pieceAt(board,dst)) != srcColor){
                setValidAt(moves, dst, true, true);
                total++;
            }
        }
    }
    return total;
}

uint rookMoves(Board board, Valid moves, const Vec2 src)
{
    uint total = 0;
    const Piece srcPiece = pieceAt(board, src);
    if(srcPiece != P_ROOK_W && srcPiece != P_ROOK_B){
        fwprintf(stderr, L"Error: cannot get rookMoves for non rook at %ls\n", src);
        exit(EXIT_FAILURE);
    }

    for(uint i = 0, Dir d = D_U; i < 4; i++, d = dirRoR(d))
        total += cast(board, moves, src, d, 0);
    return total;
}

uint bishopMoves(Board board, Valid moves, const Vec2 src)
{
    uint total = 0;
    const Piece srcPiece = pieceAt(board, src);
    if(srcPiece != P_BISHOP_W && srcPiece != P_BISHOP_B){
        fwprintf(stderr, L"Error: cannot get bishopMoves for non bishop at %ls\n", src);
        exit(EXIT_FAILURE);
    }

    for(uint i = 0, Dir d = D_UR; i < 4; i++, d = dirRoR(d))
        total += cast(board, moves, src, d, 0);
    return total;
}

uint queenMoves(Board board, Valid moves, const Vec2 src)
{
    uint total = 0;
    const Piece srcPiece = pieceAt(board, src);
    if(srcPiece != P_QUEEN_W && srcPiece != P_QUEEN_B){
        fwprintf(stderr, L"Error: cannot get queenMoves for non queen at %ls\n", src);
        exit(EXIT_FAILURE);
    }

    for(uint i = 0, Dir d = D_U; i < 4; i++, d = dirRoR(d))
        total += cast(board, moves, src, d, 0) + cast(board, moves, src, dirRor(d), 0);
    return total;
}

uint kingMoves(Board board, Valid moves, const Vec2 src)
{
    uint total = 0;
    const Piece srcPiece = pieceAt(board, src);
    if(srcPiece != P_KING_W && srcPiece != P_KING_B){
        fwprintf(stderr, L"Error: cannot get kingMoves for non king at %ls\n", src);
        exit(EXIT_FAILURE);
    }

    for(uint i = 0, Dir d = D_U; i < 4; i++, d = dirRoR(d))
        total += cast(board, moves, src, d, 1) + cast(board, moves, src, dirRor(d), 1);
    return total;
}

uint pawnMoves(Board board, Valid moves, const Vec2 src)
{
    const Piece srcPiece = pieceAt(board, src);
    const Color srcColor = pieceColor(srcPiece);
    bool first;
    Dir fDir;
    if(srcPiece == P_PAWN_W){
        fDir = D_D;
        first = src.y == 1;
    }else if(srcPiece == P_PAWN_B){
        fDir = D_U;
        first = src.y == 6;
    }else{
        fwprintf(stderr, L"Error: cannot get pawnMoves for non pawn at %ls\n", pos);
        exit(EXIT_FAILURE);
    }

    uint total = 0;
    const Vec2 dst = shift(src, fDir, 1);
    if(!validPos(dst))
        return total;
    if(pieceAt(board, dst) == L' '){
        setValidAt(moves, dst, M_VALID);
        total++;
        if(first){
            const Vec2 fDst = shift(src, fDir, 2);
            if(validPos(fDst) && pieceAt(board, fDst) == L' '){
                setValidAt(moves, fDst, M_VALID);
                total++;
            }
        }
    }

    Vec2 cap = shift(dst, dirRoL(fDir), 1);
    if(validPos(cap) && pieceColor(pieceAt(board, cap)) == colorInv(srcColor)){
        setValidAt(moves, cap, M_CAPTURE);
        total++;
    }

    cap = shift(dst, dirRoR(fDir), 1);
    if(validPos(cap) && pieceColor(pieceAt(board, cap)) == colorInv(srcColor)){
        setValidAt(moves, cap, M_CAPTURE);
        total++;
    }

    return total;
}

uint validMovesStateless(Board board, Moves moves, const Vec2 pos)
{
    resetMoves(moves);
    const Piece srcPiece = pieceAt(board, pos);
    const Color srcColor = pieceColor(srcPiece);
    if(srcColor == C_NONE){
        fwprintf(stderr, L"Error: cannot get validMoves for blank square\n");
        exit(EXIT_FAILURE);
    }

    switch(srcPiece){
        case P_PAWN_B:
        case P_PAWN_W:
            return pawnMoves(board, moves, pos);
            break;
        case P_ROOK_B:
        case P_ROOK_W:
            return rookMoves(board, moves, pos);
            break;
        case P_KNIGHT_B:
        case P_KNIGHT_W:
            return knightMoves(board, moves, pos);
            break;
        case P_BISHOP_B:
        case P_BISHOP_W:
            return bishopMoves(board, moves, pos);
            break;
        case P_QUEEN_B:
        case P_QUEEN_W:
            return queenMoves(board, moves, pos);
            break;
        case P_KING_B:
        case P_KING_W:
            return kingMoves(board, moves, pos);
            break;
        case P_EMPTY:
        default:
            return 0;
    }
    return 0;
}

Turn* turnPosChanged(Turn *game, const Vec2 pos)
{
    while(game){
        if(eqVec2(game->move.src.pos, pos) || eqVec2(game->move.dst.pos, pos))
            return game;
        game = game->next;
    }
    return NULL;
}

#endif /* end of include guard: MOVECHECK_H */
