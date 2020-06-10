# include <stdio.h>
# include <stdint.h>

# define PIECE_MASK 0b00001110

void board(void);

int main(int argc, char *argv[]){
    board();
    return 0;
}

void board(void) {
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

    static uint8_t full[8][8] = {
        {0x8b, 0xa8, 0xa7, 0xa4, 0xa0, 0xa6, 0xa8, 0x8b},
        {0xae, 0xae, 0xae, 0xae, 0xae, 0xae, 0xae, 0xae},
        {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
        {0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde},
        {0xcb, 0xd8, 0xd6, 0xd4, 0xd0, 0xd7, 0xd8, 0xcb}
    };
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) 
            printf("%d ",(full[i][j] & PIECE_MASK) >> 1);
        printf("\n");
    }
}