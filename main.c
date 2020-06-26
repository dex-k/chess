# include <stdio.h>
# include <stdint.h>

# define M_PIECE 0b00001110 //bits which id piece type
# define M_W_ATK 0b00100000 //flip bit specifying white attack
# define M_B_ATK 0b00010000 //flip bit specifying black attack


/**
 * Types of chess pieces:
 *  0x0 000 King.castle:    (in check, not in check)    1 bit   0=false
 *  0x1 001 King.!castle:   (in check, not in check)    1 bit   0=false
 *  0x2 010 Queen:                                      0 bits
 *  0x3 011 Bishop:         (Dark square, light square) 1 bit   0=white
 *  0x4 100 Knight:                                     0 bits
 *  0x5 101 Rook:           (Can/can't be castled)      1 bit   0=false
 *  0x6 110  
 *  0x7 111 Pawn:           (Can be captured e.p.)      1 bit   0=false
 * Byte encoding
 *  [ ][ ][ ][ ][ ][ ][ ][ ]
 *  [1][1][1][1][   3   ][1]
 *   |  |  |  |     |     |
 *   |  |  |  |     |     *-- Piece modifier
 *   |  |  |  |     *-------- Piece specifier
 *   |  |  |  *-------------- Attacked by black
 *   |  |  *----------------- Attacked by white
 *   |  *-------------------- Color specifier 0=white
 *   *----------------------- IsPiece         0=false
 */
# define W_R_C  0b10001011  //can be castled
# define W_R    0b10001010  //can't be castled
# define W_N    0b10001000
# define W_B_L  0b10000110  //light square
# define W_B_D  0b10000111  //dark square
# define W_Q    0b10000100
# define W_K_C  0b10000000  //can be castled
# define W_K    0b10000010  //can't be castled
# define W_P    0b10001110
# define W_P_E  0b10001111 //can be captured e.p.

# define B_R_C  0b11001011  //can be castled
# define B_R    0b11001010  //can't be castled
# define B_N    0b11001000
# define B_B_L  0b11000110  //light square
# define B_B_D  0b11000111  //dark square
# define B_Q    0b11000100
# define B_K_C  0b11000000  //can be castled
# define B_K    0b11000010  //can't be castled
# define B_P    0b11001110
# define B_P_E  0b11001111 //can be captured e.p.

# define EMPTY  0b00000000


void board(void);

int main(int argc, char *argv[]){
    board();
    return 0;
}

void board(void) {
    /**
     * Initialised full chessboard such that [0][0] is a1, [0][1] is a2, ... [7][7] is h8
     */
    static uint8_t full[8][8] = {
        {W_R_C,           W_N   | M_W_ATK, W_B_D | M_W_ATK, W_Q   | M_W_ATK, W_K_C | M_W_ATK, W_B_L | M_W_ATK, W_N   | M_W_ATK, W_R_C},
        {W_P   | M_W_ATK, W_P   | M_W_ATK, W_P   | M_W_ATK, W_P   | M_W_ATK, W_P   | M_W_ATK, W_P   | M_W_ATK, W_P   | M_W_ATK, W_P   | M_W_ATK},
        {EMPTY | M_W_ATK, EMPTY | M_W_ATK, EMPTY | M_W_ATK, EMPTY | M_W_ATK, EMPTY | M_W_ATK, EMPTY | M_W_ATK, EMPTY | M_W_ATK, EMPTY | M_W_ATK},
        {EMPTY,           EMPTY,           EMPTY,           EMPTY,           EMPTY,           EMPTY,           EMPTY,           EMPTY},
        {EMPTY,           EMPTY,           EMPTY,           EMPTY,           EMPTY,           EMPTY,           EMPTY,           EMPTY},
        {EMPTY | M_B_ATK, EMPTY | M_B_ATK, EMPTY | M_B_ATK, EMPTY | M_B_ATK, EMPTY | M_B_ATK, EMPTY | M_B_ATK, EMPTY | M_B_ATK, EMPTY | M_B_ATK},
        {B_P   | M_B_ATK, B_P   | M_B_ATK, B_P   | M_B_ATK, B_P   | M_B_ATK, B_P   | M_B_ATK, B_P   | M_B_ATK, B_P   | M_B_ATK, B_P   | M_B_ATK},
        {B_R_C,           B_N   | M_B_ATK, B_B_L | M_B_ATK, B_Q   | M_B_ATK, B_K_C | M_B_ATK, B_B_D | M_B_ATK, B_N   | M_B_ATK, B_R_C}
    };

    static uint8_t logical[8] = {
        0b11111111,
        0b11111111,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b11111111,
        0b11111111
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) 
            printf("%x ", full[i][j] - full[i][j] );
        printf("\n");
    }
}