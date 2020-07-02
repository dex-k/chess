# include <stdio.h>
# include <stdint.h>

# define M_PIECE 0b00001110 //bits which id piece type
# define M_W_ATK 0b00100000 //flip bit specifying white attack
# define M_B_ATK 0b00010000 //flip bit specifying black attack

# define BOARD_SIZE 8 //assumes a square board

# define ASCII_A 65
# define ASCII_a 97
# define ASCII_0 48

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

# define W_R_C  PIECE | WHITE | (ROOK << 1)     | 1
# define W_R    PIECE | WHITE | (ROOK << 1)
# define W_N    PIECE | WHITE | (KNIGHT << 1)
# define W_B_L  PIECE | WHITE | (BISHOP << 1)
# define W_B_D  PIECE | WHITE | (BISHOP << 1)   | 1
# define W_Q    PIECE | WHITE | (QUEEN << 1)
# define W_K_C  PIECE | WHITE | (KING << 1)
# define W_K    PIECE | WHITE | (CASTLED_KING << 1)
# define W_P    PIECE | WHITE | (PAWN << 1)
# define W_P_E  PIECE | WHITE | (PAWN << 1)     | 1

# define B_R_C  PIECE | BLACK | (ROOK << 1)     | 1
# define B_R    PIECE | BLACK | (ROOK << 1)
# define B_N    PIECE | BLACK | (KNIGHT << 1)
# define B_B_L  PIECE | BLACK | (BISHOP << 1)
# define B_B_D  PIECE | BLACK | (BISHOP << 1)   | 1
# define B_Q    PIECE | BLACK | (QUEEN << 1)
# define B_K_C  PIECE | BLACK | (KING << 1)
# define B_K    PIECE | BLACK | (CASTLED_KING << 1)
# define B_P    PIECE | BLACK | (PAWN << 1)
# define B_P_E  PIECE | BLACK | (PAWN << 1)     | 1


# define EMPTY  0b00000000

/**
 * needs so be changed to better soln.
 * currently, it just holds the board array to avoid it being a global variable.
 */
void board(void);

/**
 * takes in the initial board with piece descriptors, and the changes which have happened, and uses
 * that info to figure out what move was made, and if the move was valid
 */
void getMove(uint8_t boardState[BOARD_SIZE][BOARD_SIZE], uint8_t logicalStateChange[BOARD_SIZE]);

/**
 * takes in a board with piece descriptors and prints it using letters for pieces
 */
void printFullBoard(uint8_t board[BOARD_SIZE][BOARD_SIZE]);

/**
 * takes a logical board state (if there is or isn't pieces) and prints an 8x8 grid of ./x for each piece
 */
void printLogicalBoard(uint8_t logical[BOARD_SIZE]);

/**
 * dangerous function that appends a single character to the end of a string in place of the terminating 
 * NUL character, and adds a terminating NUL after.
 * 
 * !!! assumes the string is big enough and makes no checks
 */
void strcat_c(char *str, char c);

const char *PIECE_ID = "KKQBNR-P"; //global access to the letters for each piece

int main(int argc, char *argv[]){
    board();
    printf("\n");
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
        // 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x1, 0x0 //should be 1...a7
        0x2, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0 //should be 1.Na3
        // 0x10, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0 //not allowed move, is technicaly 1.Ka3
    };

    printFullBoard(full);
    printLogicalBoard(stateChangeTest);
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
     *      |    |_____ 0111 = 7 --> 8 (0..7 maps to 1..8) rank
     *      |__________ 0010 = 2 --> b (0..7 maps to A..B) file
     * Which means 00100111 represents the square b8.
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
    
    /**
     * find coordinates of squares which underwent a logical state change
     */
    for (uint8_t rank = 0; rank < BOARD_SIZE; rank++) {
        for (uint8_t file = 0; file < BOARD_SIZE; file++) {

            if ( logicalStateChange[rank] & (1 << file) ) { //if the square was part of a move

                squares[found] = rank + ( file << 4 ); //set to the binary representation outlined above

                if (boardState[rank][file] & PIECE) { //if the square isn't empty, the MSB will be set to 1
                    pieces[found] = 1 << 7  | ( (boardState[rank][file] & M_PIECE) >> 1 );
                    //..............|...........|______________________________ get the piece id and shift to LSBs
                    //...............\_________________________________________ MSB set high for piece id
                }

                found++;

            };

        };
    };
    // 4 LSBs store the piece type, 4 MSBs store the id of the square in squares[] array
    uint8_t pieceBuffer = 0;

    // ={} ensures bulk initialisation to NULL https://stackoverflow.com/a/60535875
    char moveString[7+1] = {}; // "Qh1xg2+" is a longest possible move, assuming no !/?? type commentary

    /**
     * if theres only 2 squares, the move was either a normal move or capture
     */
    if (found == 2) {
        /**
         * If one piece is empty, then we know it was a normal move
         */
        if ( !(pieces[1] && pieces[2]) ) {
            /**
             * Extract the piece type of the square which wasn't empty, 
             * i.e. the piece that moved
             */
            if (pieces[0]) {
                pieceBuffer = (pieces[0] & 0xf) | 0<<4; //piece id stored in 3 LSBs, so select them, then add the index (0) in MSBs
            } else if (pieces[1]) {
                pieceBuffer = (pieces[1] & 0xf) | 1<<4; //same as above, but the index placed in MSBs is nonzero
            };


            /**
             * use this info to form the move that was made
             */
            if ( (pieceBuffer & 0xf) != PAWN) { //only but a piece identifier in if the move wasn't made by a pawn
                // strncpy(moveString, &PIECE_ID[pieceBuffer & 0xf], 1);
                *moveString = PIECE_ID[pieceBuffer & 0xf]; //since we're only setting the first character, guarenteed.
            }
    
            strcat_c(moveString, (char)(ASCII_a + (squares[1 - (pieceBuffer >> 4)] >> 4)));//add file of the EMPTY square...
            // ... by using squares[1-x] which is guarenteed to return the other piece because we only have two peices atm
            strcat_c(moveString, (char)(ASCII_0 + 1 + (squares[1 - (pieceBuffer >> 4)] & 0xf))); //add rank
        };
    };

    printf("%s", moveString);
};


void printFullBoard(uint8_t board[BOARD_SIZE][BOARD_SIZE]){
    for (int8_t rank = BOARD_SIZE-1; rank >= 0; rank--) { //start at the borrom because we need to print black first
        for (int8_t file = 0; file < BOARD_SIZE; file++){
            if (board[rank][file] & PIECE) {
                if (board[rank][file] & BLACK) {
                    printf("%c ", PIECE_ID[(board[rank][file] & M_PIECE) >> 1] + 32); //make it lowercase
                } else {
                    printf("%c ", PIECE_ID[(board[rank][file] & M_PIECE) >> 1] );
                }
            } else {
                printf(". ");
            }
        };
        printf("\n");
    };
    printf("\n");
}

void printLogicalBoard(uint8_t logical[BOARD_SIZE]) {
    for (int8_t rank = BOARD_SIZE-1; rank >= 0; rank--) { //start at the borrom because we need to print black first
        for (int8_t file = 0; file < BOARD_SIZE; file++){
            // printf("%d ", (logical[rank] >> file) & 1);
            printf("%c ", (logical[rank] >> file) & 1 ? 'x' : '.');
        };
        printf("\n"); //line break
    };
    printf("\n"); //trailing newline
}

void strcat_c(char *str, char c) {
    //loop through the string incrementing pointer until we find the NUL
    for (;*str;str++); // note the terminating semicolon here. 
    *str++ = c; //append char
    *str++ = '\0'; //append NUL
}