# include <stdio.h>
# include <stdint.h>
# include <string.h>

# define M_PIECE 0b00001110 //bits which id piece type
# define M_W_ATK 0b00100000 //flip bit specifying white attack
# define M_B_ATK 0b00010000 //flip bit specifying black attack

# define BOARD_SIZE 8 //assumes a square board

# define ASCII_A 65
# define ASCII_a 97
# define ASCII_0 48

static const char *FILES = "abcdefgh";
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
# define PIECE  0b10000000 //0x80
# define BLACK  0b01000000 //0x40
# define WHITE  0b00000000  //completely uncessary but just to be less confusing later

# define PAWN           0b111 //0x7
# define ROOK           0b101 //0x5
# define KNIGHT         0b100 //0x4
# define BISHOP         0b011 //0x3
# define QUEEN          0b010 //0x2
# define KING           0b000 //0x0
# define CASTLED_KING   0b001 //0x1

# define W_R_C  PIECE | WHITE | ROOK | 1
# define W_R    PIECE | WHITE | ROOK
# define W_N    PIECE | WHITE | KNIGHT
# define W_B_L  PIECE | WHITE | BISHOP
# define W_B_D  PIECE | WHITE | BISHOP | 1
# define W_Q    PIECE | WHITE | QUEEN
# define W_K_C  PIECE | WHITE | KING
# define W_K    PIECE | WHITE | CASTLED_KING
# define W_P    PIECE | WHITE | PAWN
# define W_P_E  PIECE | WHITE | PAWN | 1

# define B_R_C  PIECE | BLACK | ROOK | 1
# define B_R    PIECE | BLACK | ROOK
# define B_N    PIECE | BLACK | KNIGHT
# define B_B_L  PIECE | BLACK | BISHOP
# define B_B_D  PIECE | BLACK | BISHOP | 1
# define B_Q    PIECE | BLACK | QUEEN
# define B_K_C  PIECE | BLACK | KING
# define B_K    PIECE | BLACK | CASTLED_KING
# define B_P    PIECE | BLACK | PAWN
# define B_P_E  PIECE | BLACK | PAWN | 1

/*
# define W_R_C  0b10001011  //white rook, can be castled
# define W_R    0b10001010  //white rook, can't be castled
# define W_N    0b10001000  //white knight
# define W_B_L  0b10000110  //white light square bishop
# define W_B_D  0b10000111  //white dark square bishop
# define W_Q    0b10000100  //white queen
# define W_K_C  0b10000000  //white king, can be castled
# define W_K    0b10000010  //white kind, can't be castled
# define W_P    0b10001110  //white pawn
# define W_P_E  0b10001111  //white pawn, can be captured e.p.

# define B_R_C  0b11001011  //black rook, can be castled
# define B_R    0b11001010  //black rook, can't be castled
# define B_N    0b11001000  //black knight
# define B_B_L  0b11000110  //black light square bishop
# define B_B_D  0b11000111  //black dark square bishop
# define B_Q    0b11000100  //black queen
# define B_K_C  0b11000000  //black king, can be castled
# define B_K    0b11000010  //black kind, can't be castled
# define B_P    0b11001110  //black pawn
# define B_P_E  0b11001111  //black pawn, can be captured e.p.
*/

# define EMPTY  0b00000000


void board(void);
void getMove(uint8_t boardState[BOARD_SIZE][BOARD_SIZE], uint8_t logicalStateChange[BOARD_SIZE]);

int main(int argc, char *argv[]){
    board();
    return 0;
};

void board(void) {
    /**
     * Initialised full chessboard such that [0][0] is a1, [0][1] is a2, ... [7][7] is h8
     * Use macros for each piece, and set the "attacked" flag as necessary using a bitwise OR
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
    /**
     * store the status of each row as a single byte, such that the 1's bit is the A file, 2's bit is the B file
     * 4's bit is the C file etc. not ideal in that the visual representation of it is "backwards", but it means
     * we can do bitwise operations such as 
     *      logical[2] & (1 << 7)
     * which will return TRUE if there is a piece on h2
     */
    static uint8_t logical[8] = {
        0b11111111, //1st rank
        0b11111111, //2nd rank
        0b00000000, //3rd rank
        0b00000000, //4th rank
        0b00000000, //5th rank
        0b00000000, //6th rank
        0b11111111, //7th rank
        0b11111111  //8th rank
    };

    static uint8_t stateChangeTest[8] = {
        0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x1, 0x0 //should be 1...a7
        //0x2, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0 //should be 1.Na3
    };

    getMove(full, stateChangeTest);
};

/**
 * Use the knowlege of pieces and the logical state of the board to recognise which move has been made
 * Currently will only work with normal, legal piece moves, and not with captures, castles, en passant etc.
 */
void getMove(uint8_t boardState[BOARD_SIZE][BOARD_SIZE], uint8_t logicalStateChange[BOARD_SIZE]) {
    /**
     * we store each participant square as a single uint8, with the 4 MSBs representing the rank (1,2,3,...)
     * and the 4 LSBs representing the file (A,B,C,...). 
     * Since only 3 bits are needed to represent a range of 8, the highest is always 0, i.e:
     *      0010 0111
     *      |    |_____ 0111 = 7 --> 8 (0..7 maps to 1..8)
     *      |__________ 0010 = 2 --> b (0..7 maps to A..B)
     * Which means 00100111 represents the square B8.
     * 
     * Also, the most squares which can participate in a move is 4, for castling, so we only need an array of size 4.
     */
    uint8_t squares[4] = {0, 0, 0, 0}; //initialise to all 0
    /**
     * the pieces get stored as a bit with the 3 LSBs indicating the piece type as outlined above,
     * and the MSB getting set HIGH if there is a piece, or LOW if it's empty.
     */
    uint8_t pieces[4] = {0, 0, 0, 0}; //initialised to 0 representing an empty square
    uint8_t found = 0; //number of active squares found
    // 1. find coordinates of logical state change squares
    for (int rank = 0; rank < BOARD_SIZE; rank++) {
        for (int file = 0; file < BOARD_SIZE; file++) {
            if ( logicalStateChange[rank] & (1 << file) ) { //if there is a piece on the square
                squares[found] = file + ( rank << 4 ); //set to the binary representation outlined above
                uint8_t contents = boardState[rank][file]; //i swear you couldn't pass 2d arrays like this wtf
                if (contents != EMPTY) { //if empty, don't need to do anything
                    pieces[found] = 1 << 7  | ( (contents & M_PIECE) >> 1 );
                    //..............|...........|______________________________ get the piece id and shift to LSBs
                    //...............\_________________________________________ MSB set high for piece id
                }
                found++;
            };
        };
    };
    // 4 LSBs store the piece type, 4 MSBs store the id of the square in squares[] array
    uint8_t pieceBuffer = 0;
    char moveString[7+1]; // "Qh1xg2+" is a longest possible move, assuming no !/?? type commentary
    char charBuffer[1+1]; // general purpose buffer for handinling single characters

    // 2. verify that theres only 2
    if (found == 2) {
    // 3. verify that one is empty
        if ( !(pieces[1] && pieces[2]) ) {
    // 4. get the piece type of the non-empty square used
            if (pieces[0]) {
                pieceBuffer = ( pieces[0] & ~PIECE ) >> 1 | 0 << 4; //clear the piece flag and get the piece id, then set the index
            } else if (pieces[1]) {
                pieceBuffer = ( pieces[1] & ~PIECE ) >> 1 | 1 << 4; //same as above, but this time the index is non-zero
            };
    // 5. form a move made
            switch (pieceBuffer & 0xf) { //select the LSBs
                case PAWN:
                    /*//Nothing gets added for a pawn move, but this code is needed for pawn captures
                    snprintf(charBuffer, 1, "%d", ASCII_a + squares[1 - (pieceBuffer >> 4)]);
                    strncpy(moveString, charBuffer, 1); //1-x selects the other piece (since there's
                    // ... only 2 pieces maximum in this case), then extracts the index and used that to get the file (>>4 for MSBs)
                    */
                break;
                case ROOK:
                    //!!!TODO check for conflicts here to see if specifying which piece is needed
                    strncpy(moveString, "R", 1);
                break;
                case KNIGHT:
                    //!!!TODO check for conflicts here to see if specifying which piece is needed
                    strncpy(moveString, "N", 1);
                break;
                case BISHOP:
                    strncpy(moveString, "B", 1);
                break;
                case QUEEN:
                    //!!!TODO check for conflicts here to see if specifying which piece is needed
                    strncpy(moveString, "Q", 1);
                break;
                case KING:
                case CASTLED_KING:
                    strncpy(moveString, "K", 1);
                break;
            };

            snprintf(charBuffer, 1, "%d", ASCII_a + squares[pieceBuffer >> 4]); // extract the file from squares
            strncat(moveString, charBuffer, 1);// add alpha component

            snprintf(charBuffer, 1, "%d", ASCII_0 + 1 + squares[(pieceBuffer >> 4)]); // extract rank from squares
            strncat(moveString, charBuffer, 1); // add numeric component

        };
    };

    printf("%s", moveString);
};