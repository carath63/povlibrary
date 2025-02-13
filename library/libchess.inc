/******************************************************************************
 * File: libchess.inc
 * Description:
 *      This is a library of macros for defining and modifying the state of
 *      a chess board, as well as defining what a "chess set" includes that can
 *      be used to create a combined object consisting of the chess board
 *      and the pieces on that board in their current positions.
 ******************************************************************************/

#ifndef(Libchess_Inc_Temp)

#declare Libchess_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libchess.inc\n"
#end                     

//=============================================================================
// Includes
//

//-----------------------------------------------------------------------------
// Standard Includes
//

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//

// End My Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Pre-include Constants
//

// End Pre-include Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project Includes
//

// End Project Includes
//-----------------------------------------------------------------------------

// End Includes
//=============================================================================

//=============================================================================
// Chess Piece Declarations
//
// Constants used for easier access to elements of chess sets and board states
//
#declare Chess_player_white = 0;
#declare Chess_player_black = 1;
#declare Chess_player_names = array[2] {
    "White",
    "Black"
};
#declare Chess_player_abbrevs   = array[2] {
    "w",
    "b"
};        

#declare Chess_piece_type_none      = 0;
#declare Chess_piece_type_king      = 1;
#declare Chess_piece_type_queen     = 2;
#declare Chess_piece_type_bishop    = 3;
#declare Chess_piece_type_knight    = 4;
#declare Chess_piece_type_rook      = 5;
#declare Chess_piece_type_pawn      = 6;
#declare Chess_piece_type_names = array[7]{
    " ",
    "K",
    "Q",
    "B",
    "N",
    "R",
    "p"
}

#declare Chess_piece_side_king  = 0;
#declare Chess_piece_side_queen = 1;

// End Chess Piece Declarations
//=============================================================================

//=============================================================================
// Chess Set Definitions
//
// These macros are used to create a particular Chess Set.  
// Look at chess_sets/cs_simple.inc to see examples of calling these macros
// to define a new chess set.
//
// A Chess Set consists of a board (Chess_set_board*), and a set of white and
// black pieces, (Chess_set_player*)
//
// Utility macros have been provided for finding coordinates for board positions
// using various notations as well.
//

//-----------------------------------------------------------------------------
// Chess_set_board_create(BoardSize,SquareSize,BoardObj)
//
// Creates a board representation as a dictionary consisting of:
//      BoardSize: <x,y,z>
//          X - width of the board with white player in the -Z direction
//          Y - thickness of the board
//          Z - depth of the board with white player in the -Z direction
//      SquareSize: size of the sides of the board square
//      BoardObj: object{} for the board in the chess set.  It should be
//                resting on the Y=0 plane, and centered at <0,0> in the XZ plane
//
#macro Chess_set_board_create(BoardSize,SquareSize,BoardObj)
    #local _board   = dictionary {
        .board_size:    BoardSize,
        .square_size:   SquareSize,
        .board_obj:     BoardObj
    }
    ;
    
    _board
#end

// End Chess_set_board_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_board_loc(ChessSetBoard,Pos)
//
//      Returns a 3D vector <x,y,z> representing the center, resting position
//      for the given board and a position using column letter and row number
//      in standard chess notation.
//
//      ChessSetBoard: result of Chess_set_board_create() call
//      Pos: string in the format "a8" where "a" is the leftmost column (a-h) from the white player's view, and "8"
//      is the row number (1-8) from the white player's view.
// 
#macro Chess_set_board_loc(ChessSetBoard,Pos)
    #local _index   = Chess_board_pos_to_index(Pos);
    #local _loc     = <(-3.5 + _index.x)*ChessSetBoard.square_size, ChessSetBoard.board_size.y, (-3.5 + _index.y)*ChessSetBoard.square_size>;
    
    _loc
#end

// End Chess_set_board_loc
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_board_loc_index(ChessSetBoard,Index)
//
//      Returns a 3D vector <x,y,z> representing the center, resting position
//      for the given board and a position using a 2D vector as column and row numbers
//
//      ChessSetBoard: result of Chess_set_board_create() call
//      Index: 2D vector <x,y> where x is the column number (0-7) and y is the row number (0-7)
// 
#macro Chess_set_board_loc_index(ChessSetBoard,Index)
    #local _loc     = <(-3.5 + Index.x)*ChessSetBoard.square_size, ChessSetBoard.board_size.y, (-3.5 + Index.y)*ChessSetBoard.square_size>;
    
    _loc
#end

// End Chess_set_board_loc_index
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_piece_create(Player,PieceType,PieceSize,PieceObj)
//
//      Creates a piece representation as a dictionary consisting of:
//
//      Player: One of the Chess_player_* constants (white or black)
//      PieceType: One of the Chess_piece_type_* constants
//      PieceSize: 3D vector <x,y,z> with the size of the piece in its
//          original facing
//      PieceObj: object{} for the piece, which should be resting on the Y=0 plane
//          and centered at <0,0> in the XZ plane
//      
#macro Chess_set_piece_create(Player,PieceType,PieceSize,PieceObj)
    #local _piece   = dictionary {
        .player:        Player,
        .piece_type:    PieceType,
        .piece_size:    PieceSize,
        .piece_obj:     PieceObj
    }
    ;
    
    _piece    
#end

// This is a constant Chess_set_piece representing an empty space on the board
//
#declare Chess_set_piece_none   = dictionary {
    .piece_type:    Chess_piece_type_none
}
;    

// End Chess_set_piece_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_player_create()
//
//  Creates a dictionary object representing the pieces for one player in a
//  Chess_set that initially has no pieces in it.  In this representation,
//      there is one king allowed, and all other pieces can have multiple copies
//      which allows for chess sets where there is variability between different
//      copies of the same piece.
//
//  Player: One of the Chess_player_* constants (white or black)
//
#macro Chess_set_player_create(Player)
    #local _player_pieces   = dictionary {
        .player:    Player,
        .king:      Chess_set_piece_none,
        .queens:    array,
        .num_queens:    0,
        .bishops:   array,
        .num_bishops:   0,
        .knights:   array,
        .num_knights:   0,
        .rooks:     array,
        .num_rooks:     0,
        .pawns:     array,
        .num_pawns: 0
    }
    
    _player_pieces
#end

// End Chess_set_player_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_create(Board)
//
//      Creates a chess set as a dictionary consisting of a Chess_set_board*
//  dictionary and an array of Chess_set_player* dictionaries, one for the white
//  and one for the black player.  Initially both player sets are empty.
//
//  Board: Result of a Chess_set_board_create() call
//
#macro Chess_set_create(Board)
    #local _pieces  = array[2];
    #local _pieces[Chess_player_white]  = Chess_set_player_create(Chess_player_white);
    #local _pieces[Chess_player_black]  = Chess_set_player_create(Chess_player_black);
    
    #local _set = dictionary {
        .board: Board,
        .pieces:    _pieces
    }
    ;
    
    _set
#end

// End Chess_set_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_player_add_piece(PlayerPieces,ChessSetPiece)
//
//      Adds a new piece to the given chess set player representation, automatically
//      appending it to the correct array for pieces other than the king.
//
//  PlayerPieces:   Given a chess set S (Chess_set_create()), this is either
//      S.pieces[Chess_player_white] or S.pieces[Chess_player_black]
//
//
// The easier use of this is the Chess_set_add_piece() macro below that finds the
// correct player pieces and calls this function with that.
//
#macro Chess_set_player_add_piece(PlayerPieces,ChessSetPiece)
    #switch(ChessSetPiece.piece_type)
        #case(Chess_piece_type_king)
            #declare PlayerPieces.king  = ChessSetPiece;
        #break
        #case(Chess_piece_type_queen)
            #declare PlayerPieces.queens[PlayerPieces.num_queens]   = ChessSetPiece;
            #declare PlayerPieces.num_queens                        = PlayerPieces.num_queens + 1;    
        #break
        #case(Chess_piece_type_bishop)
            #declare PlayerPieces.bishops[PlayerPieces.num_bishops] = ChessSetPiece;
            #declare PlayerPieces.num_bishops                       = PlayerPieces.num_bishops + 1;    
        #break
        #case(Chess_piece_type_knight)
            #declare PlayerPieces.knights[PlayerPieces.num_knights] = ChessSetPiece;
            #declare PlayerPieces.num_knights                       = PlayerPieces.num_knights + 1;    
        #break
        #case(Chess_piece_type_rook)
            #declare PlayerPieces.rooks[PlayerPieces.num_rooks]     = ChessSetPiece;
            #declare PlayerPieces.num_rooks                         = PlayerPieces.num_rooks + 1;    
        #break
        #case(Chess_piece_type_pawn)
            #declare PlayerPieces.pawns[PlayerPieces.num_pawns]     = ChessSetPiece;
            #declare PlayerPieces.num_pawns                         = PlayerPieces.num_pawns + 1;    
        #break
        #else
            #error concat("Chess_set_player_add_piece: Illegal piece type ", str(ChessSetPiece.piece_type, 0, 0), "\n")
        #break    
    #end
#end

// End Chess_set_player_add_piece
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_add_piece(ChessSet,ChessSetPiece)
//
//      Adds a piece to the appropriate player pieces in the given chess set.
//
//      ChessSet: Result of Chess_set_create()
//      ChessSetPiece: Result of Chess_set_piece_create()
//
#macro Chess_set_add_piece(ChessSet,ChessSetPiece)
    Chess_set_player_add_piece(ChessSet.pieces[ChessSetPiece.player],ChessSetPiece)
#end

// End Chess_set_add_piece
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_player_complete(ChessSetPlayer)
//
//      Checks to ensure that a given player piece set in the chess set has
//      at least 1 of each piece.
//
//      This is a helper macro for Chess_set_complete() below and doesn't need to be
//      called directly.
//
#macro Chess_set_player_complete(ChessSetPlayer)
    #if (ChessSetPlayer.king.piece_type = Chess_piece_type_none)
        #error concat("Chess_set_player_complete: No ", Chess_player_names[ChessSetPlayer.player], " king\n")
    #end
    #if (ChessSetPlayer.num_queens = 0)
        #error concat("Chess_set_player_complete: No ", Chess_player_names[ChessSetPlayer.player], " queens\n")
    #end        
    #if (ChessSetPlayer.num_bishops = 0)
        #error concat("Chess_set_player_complete: No ", Chess_player_names[ChessSetPlayer.player], " bishops\n")
    #end        
    #if (ChessSetPlayer.num_knights = 0)
        #error concat("Chess_set_player_complete: No ", Chess_player_names[ChessSetPlayer.player], " knights\n")
    #end        
    #if (ChessSetPlayer.num_rooks = 0)
        #error concat("Chess_set_player_complete: No ", Chess_player_names[ChessSetPlayer.player], " rooks\n")
    #end        
    #if (ChessSetPlayer.num_pawns = 0)
        #error concat("Chess_set_player_complete: No ", Chess_player_names[ChessSetPlayer.player], " pawns\n")
    #end        
#end

// End Chess_set_player_complete
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_complete(ChessSet)
//
//      Checks to ensure that a given chess set has at least one of each piece
//      for each player.
//
//      ChessSet: Result of Chess_set_create()
#macro Chess_set_complete(ChessSet)
    Chess_set_player_complete(ChessSet.pieces[Chess_player_white])
    Chess_set_player_complete(ChessSet.pieces[Chess_player_black])        
#end

// End Chess_set_complete
//-----------------------------------------------------------------------------

// End Chess Set Definitions
//=============================================================================

//=============================================================================
// Chess Game Representations
//
// Orthogonal to the chess set is the representation of the state of the board.
// These macros create a state representation and manipulate it with macros
// for moving pieces on the board, adding them to the board directly, or
// removing them from the board.
//

//-----------------------------------------------------------------------------
// Chess_board_graveyard_create(Player)
//
//  Each player has a graveyard of pieces that are removed from the board.
//  The graveyard starts empty, and various operations can cause pieces to
//  be placed in the graveyard.
//
//  This macro creates a dictionary representation of the graveyard consisting
//  of the player (Chess_player_* constant), and an array of Chess_set_piece
//  objects and a count of those pieces.
//
//  Player: One of either Chess_player_white or Chess_player_black
//
#macro Chess_board_graveyard_create(Player)
    #local _graveyard   = dictionary {
        .player:    Player,
        .num_pieces:    0,
        .pieces:        array
    }
    ;
    
    _graveyard
#end

// End Chess_board_graveyard_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_graveyard_add(Graveyard,Piece)
//
//  Adds a chess set piece to a graveyard.
//
//  Graveyard: Result of Chess_board_graveyard_create()
//  Piece: Chess set piece (Chess_set_piece_create()) from the chess set
//
#macro Chess_board_graveyard_add(Graveyard,Piece)
    #declare Graveyard.pieces[Graveyard.num_pieces] = Piece;
    #declare Graveyard.num_pieces                   = Graveyard.num_pieces + 1;
#end

// End Chess_board_graveyard_add
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_create()
//
//  Creates a dictionary representation of a chess board state that consists of
//  a particular chess set, and a matrix of board position states holding
//  chess set pieces, and the two graveyard objects.  Initially, the matrix
//  of board states is set to Chess_set_piece_none, and the graveyards are empty.
//
//  ChessSet: Result of calling Chess_set_create()
//
#macro Chess_board_state_create(ChessSet)
    #local _board_state     = array[8][8];
    #for (r, 0, 7, 1)
        #for (c, 0, 7, 1)
            #local _board_state[r][c]  = Chess_set_piece_none;
        #end
    #end
    
    #local _graveyards  = array[2];
    #local _graveyards[Chess_player_white]  = Chess_board_graveyard_create(Chess_player_white);
    #local _graveyards[Chess_player_black]  = Chess_board_graveyard_create(Chess_player_black);
    
    #local _state   = dictionary {
        .chess_set:     ChessSet,
        .board_state:   _board_state,
        .graveyards:    _graveyards
    }
    ;
    
    _state            
#end

// End Chess_board_square_state_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_row_to_index(Row)
//
#macro Chess_board_row_to_index(Row)
    #if (Row < 1 | Row > 8)
        #error concat("Invalid chess board row ", str(Row, 0, 0), "\n")
    #end     
    (Row - 1)
#end

// End Chess_board_row_to_index
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_col_to_index(Col)
//
#macro Chess_board_col_to_index(Col)
    #local _col = strlwr(Col);
    #local _idx = asc(Col) - asc("a");
    #if (_idx < 0 | _idx > 7)
        #error concat("Invalid chess board column ", Col, "\n")
    #end    
    
    _idx
    
#end

// End Chess_board_col_to_index
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_pos_to_index(Pos)
//
#macro Chess_board_pos_to_index(Pos)
    #local _row = Chess_board_row_to_index(val(substr(Pos,2,1)));
    #local _col = Chess_board_col_to_index(substr(Pos,1,1));
    
    <_col,_row>
#end

// End Chess_board_pos_to_index
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_set(BoardState,Pos,Piece)
//
//  Sets the board state at a given position to the given chess set piece.
//
//  BoardState: Result of Chess_board_state_create()
//  Pos: string representation of a board position in column letter, row number format "a8"
//  Piece: Result of Chess_set_piece_create()
//
#macro Chess_board_state_set(BoardState,Pos,Piece)
    #local _index   = Chess_board_pos_to_index(Pos); 
    #declare BoardState.board_state[_index.y][_index.x] = Piece;
#end

// End Chess_board_state_set
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_get(BoardState,Pos)
//
//  Gets the chess set piece at the given board position.
//
//  BoardState: Result of Chess_board_state_create()
//  Pos: string representation of a board position in column letter, row number format "a8"
//
#macro Chess_board_state_get(BoardState,Pos)
    #local _index   = Chess_board_pos_to_index(Pos);
    #local _piece   = BoardState.board_state[_index.y][_index.x];
    
    _piece 
#end

// End Chess_board_state_get
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_get_index(BoardState,Index)
//
//  Gets the chess set piece at the given <col,row> index
//
//  BoardState: Result of Chess_board_state_create()
//  Index: <col,row> 0-based index of the column and row
//
#macro Chess_board_state_get_index(BoardState,Index)
    #local _piece   = BoardState.board_state[Index.y][Index.x];
    
    _piece 
#end

// End Chess_board_state_get_index
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_move(BoardState,PosFrom,PosTo)
//
//      Moves the piece on the board at position PosFrom to PosTo and
//      performs any other maintenance caused by that, (such as removing
//      the piece at PosTo if there is one.)
//
//  BoardState: Result of Chess_board_state_create()
//  PosFrom: Position of the piece to be moved in "a8" format
//  PosTo: Position to move the piece to in "a8" format
//
#macro Chess_board_state_move(BoardState,PosFrom,PosTo)
    #local _idx2    = Chess_board_pos_to_index(PosTo);
    
    #local _piece   = Chess_board_state_get(BoardState,PosFrom);
    #if (_piece.piece_type = Chess_piece_type_none)
        #error concat("There is no piece at ", PosFrom, " to be moved\n")
    #end
    Chess_board_state_set(BoardState,PosFrom,Chess_set_piece_none)
    
    #local _piece2  = Chess_board_state_get(BoardState,PosTo);
    #if (_piece2.piece_type != Chess_piece_type_none)
        #if (_piece2.player = _piece.player)
            #error concat("Illegal move from ", PosFrom, " to ", PosTo, ": player's piece on that spot\n")
        #end
        Chess_board_graveyard_add(BoardState.graveyards[_piece2.player],_piece2)    
    #end
    Chess_board_state_set(BoardState,PosTo,_piece)    
#end

// End Chess_board_state_move
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_pawn_promote(BoardState,PosFrom,PosTo,Piece)
//
//      Performs a pawn promition for the pawn at PosFrom to the position PosTo
//      replacing it with the given piece.  This macro uses Chess_board_state_move()
//      to move the pawn, so capturing a piece at PosTo is automatic.
//
//      The promoted pawn is added to the player's graveyard
//
//  BoardState: Result of Chess_board_state_create() 
//  PosFrom: Position of the piece to be moved in "a8" format
//  PosTo: Position to move the piece to in "a8" format
//  Piece: Result of Chess_set_piece_create() representing the piece the pawn is promoted to
//
#macro Chess_board_state_pawn_promote(BoardState,PosFrom,PosTo,Piece)
    #local _piece   = Chess_board_state_get(BoardState,PosFrom);
    #if (_piece.piece_type != Chess_piece_type_pawn)
        #error concat("Illegal pawn promotion: Piece at ", PosFrom, " is not a pawn.\n")
    #end
    Chess_board_state_move(BoardState,PosFrom,PosTo)
    Chess_board_graveyard_add(BoardState.graveyards[_piece.player],_piece)    
    Chess_board_state_set(BoardState,PosTo,Piece)
#end

// End Chess_board_state_pawn_promote
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_castle(BoardState,Player,Side)
//
//      Performs the castling move for the given player on the given side,
//      updating the locations of all of the affected pieces.
//
//  BoardState: Result of Chess_board_state_create()
//  Player: Chess_player_white or Chess_player_black
//  Side: Chess_piece_side_king or Chess_piece_side_queen
// 
#macro Chess_board_state_castle(BoardState,Player,Side)
    #if (Player = Chess_player_white)
        #local _king_from   = "e1";
        #if (Side = Chess_piece_side_king)
            #local _king_to     = "g1";
            #local _rook_from   = "h1";
            #local _rook_to     = "f1";
        #else 
            #local _king_to     = "c1";                         
            #local _rook_from   = "a1";
            #local _rook_to     = "d1";
        #end
    #else
        #local _king_from   = "e8";
        #if (Side = Chess_piece_side_king)
            #local _king_to     = "g8";
            #local _rook_from   = "h8";
            #local _rook_to     = "f8";
        #else
            #local _king_to     = "c8";                          
            #local _rook_from   = "a8";
            #local _rook_to     = "d8";
        #end
    #end
    
    #local _king    = Chess_board_state_get(BoardState,_king_from);
    #if (_king.piece_type != Chess_piece_type_king | _king.player != Player)
        #error concat("Illegal castle: piece at ", _king_from, " is not ", Chess_player_names[Player], "'s king\n")
    #end
    #local _rook    = Chess_board_state_get(BoardState,_rook_from);
    #if (_rook.piece_type != Chess_piece_type_rook | _rook.player != Player)
        #error concat("Illegal castle: piece at ", _rook_from, " is not ", Chess_player_names[Player], "'s rook\n")
    #end
    Chess_board_state_set(BoardState,_king_from,Chess_set_piece_none)
    Chess_board_state_set(BoardState,_king_to,_king)
    Chess_board_state_set(BoardState,_rook_from,Chess_set_piece_none)
    Chess_board_state_set(BoardState,_rook_to,_rook)        
#end

// End Chess_board_state_castle
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_init(BoardState)
//
//      Sets the board state to the standard board set up for a game of chess.
//
//  BoardState: Result of Chess_board_state_create()
//
#macro Chess_board_state_init(BoardState)
    Chess_set_complete(BoardState.chess_set)
    
    Chess_board_state_set(BoardState,"a1",BoardState.chess_set.pieces[Chess_player_white].rooks[0])
    Chess_board_state_set(BoardState,"b1",BoardState.chess_set.pieces[Chess_player_white].knights[0])
    Chess_board_state_set(BoardState,"c1",BoardState.chess_set.pieces[Chess_player_white].bishops[0])
    Chess_board_state_set(BoardState,"d1",BoardState.chess_set.pieces[Chess_player_white].queens[0])
    Chess_board_state_set(BoardState,"e1",BoardState.chess_set.pieces[Chess_player_white].king)
    Chess_board_state_set(BoardState,"f1",BoardState.chess_set.pieces[Chess_player_white].bishops[mod(1, BoardState.chess_set.pieces[Chess_player_white].num_bishops)])
    Chess_board_state_set(BoardState,"g1",BoardState.chess_set.pieces[Chess_player_white].knights[mod(1, BoardState.chess_set.pieces[Chess_player_white].num_knights)])
    Chess_board_state_set(BoardState,"h1",BoardState.chess_set.pieces[Chess_player_white].rooks[mod(1, BoardState.chess_set.pieces[Chess_player_white].num_rooks)])
    
    Chess_board_state_set(BoardState,"a8",BoardState.chess_set.pieces[Chess_player_black].rooks[0])
    Chess_board_state_set(BoardState,"b8",BoardState.chess_set.pieces[Chess_player_black].knights[0])
    Chess_board_state_set(BoardState,"c8",BoardState.chess_set.pieces[Chess_player_black].bishops[0])
    Chess_board_state_set(BoardState,"d8",BoardState.chess_set.pieces[Chess_player_black].queens[0])
    Chess_board_state_set(BoardState,"e8",BoardState.chess_set.pieces[Chess_player_black].king)
    Chess_board_state_set(BoardState,"f8",BoardState.chess_set.pieces[Chess_player_black].bishops[mod(1, BoardState.chess_set.pieces[Chess_player_black].num_bishops)])
    Chess_board_state_set(BoardState,"g8",BoardState.chess_set.pieces[Chess_player_black].knights[mod(1, BoardState.chess_set.pieces[Chess_player_black].num_knights)])
    Chess_board_state_set(BoardState,"h8",BoardState.chess_set.pieces[Chess_player_black].rooks[mod(1, BoardState.chess_set.pieces[Chess_player_black].num_rooks)])
    
    #local _a   = asc("a");
    
    #for (i, 1, 8, 1)
        #local _pos1    = concat(chr(_a+i-1),"2");
        #local _pos2    = concat(chr(_a+i-1),"7");
        Chess_board_state_set(BoardState,_pos1,BoardState.chess_set.pieces[Chess_player_white].pawns[mod((i-1), BoardState.chess_set.pieces[Chess_player_white].num_pawns)])
        Chess_board_state_set(BoardState,_pos2,BoardState.chess_set.pieces[Chess_player_black].pawns[mod((i-1), BoardState.chess_set.pieces[Chess_player_black].num_pawns)])
    #end
    
#end

// End Chess_board_state_init
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_dump(BoardState,Graveyards)
//
//      Writes the current board state to the debug stream
//
//  BoardState: Result of Chess_board_state_create()
//  Graveyards: if true, dump the graveyard information, else skip it
//
#macro Chess_board_state_dump(BoardState,Graveyards)
    #if (Graveyards & BoardState.graveyards[Chess_player_black].num_pieces > 0)
        #for(i, 0, BoardState.graveyards[Chess_player_black].num_pieces-1, 1)
            #debug concat(Chess_piece_type_names[BardState.graveyards[Chess_player_black].pieces[i].piece_type],",")
        #end
    #end
    #for (i, 7, 0, -1)
        #debug "|"
        #for (j, 0, 7, 1)
            #local _piece   = BoardState.board_state[i][j];
            #if (_piece.piece_type = Chess_piece_type_none)
                #debug concat(" ", Chess_piece_type_names[_piece.piece_type],"|")
            #else                                                       
                #debug concat(Chess_player_abbrevs[_piece.player], Chess_piece_type_names[_piece.piece_type],"|")
            #end
        #end
        #debug "\n"
    #end
    #if (Graveyards & BoardState.graveyards[Chess_player_white].num_pieces > 0)
        #for(i, 0, BoardState.graveyards[Chess_player_white].num_pieces-1, 1)
            #debug concat(Chess_piece_type_names[BardState.graveyards[Chess_player_white].pieces[i].piece_type],",")
        #end
    #end
#end

// End Chess_board_state_dump
//-----------------------------------------------------------------------------

// End Chess Game Representations
//=============================================================================

//=============================================================================
// Chess_board_layout(ChessBoardState)
//
//  This macro creates the combination of the chess board and the pieces on it
//  based on the current state.
//
//  ChessBoardState: Result of Chess_board_state_create()
//

//-----------------------------------------------------------------------------
// Chess_board_layout_place_piece(ChessBoardState,ChessSetPiece,BoardIndex)
//
//      This is a utility macro to place a piece on the board, allowing for
//      customization of the piece location.  It is called for each board location
//      by Chess_board_layout()
//
//  ChessBoardState: Result of Chess_board_state_create() 
//  ChessSetPiece: Result of Chess_set_piece_create()
//  BoardIndex: <col,row> where col and row are 0-based indices
//
#macro Chess_board_layout_place_piece(ChessBoardState,ChessSetPiece,BoardIndex)
    #ifdef (Chess_board_layout_place_piece_custom)
        #local _object  = object {
            Chess_board_layout_place_piece_custom(ChessBoardState,ChessSetPiece,BoardIndex)
        }    
    #else
        #local _object  = object {
            ChessSetPiece.piece_obj
            translate Chess_set_board_loc_index(ChessBoardState.chess_set.board,BoardIndex)
        }    
    #end
    
    _object
#end

// End Chess_board_layout_place_piece
//-----------------------------------------------------------------------------

#macro Chess_board_layout(ChessBoardState)
    #local _layout  = union {
        object { ChessBoardState.chess_set.board.board_obj }
        #for (i, 0, 7, 1)
            #for (j, 0, 7, 1)
                #local _loc_piece   = Chess_board_state_get_index(ChessBoardState, <i,j>);
                #if (_loc_piece.piece_type != Chess_piece_type_none)
                    object { Chess_board_layout_place_piece(ChessBoardState,_loc_piece,<i,j>) }    
                #end
            #end
        #end    
    }
    
    _layout
#end

// End Chess_board_layout
//=============================================================================

//=============================================================================
// Standard Chess Board Jitter Macros
//
//  These macros provide a common customization for placing pieces on the
//  board that generate and track the jitter offset for a piece at each board
//  location.  This is important for animations where subsequent clocks may
//  result in changes to randomization between runs that could occur if the
//  jitter was assigned when the layout was created.

//-----------------------------------------------------------------------------
// Chess_board_jitter_create(XZJitter,YRot)
//
//      Creates a dictionary representation of a location jitter consisting
//      of a rotation around the Y axis for the piece, and an offset in the
//      XZ plane from the center of the board square.
//
//  XZJitter:   <x,z> distance from the center of the square
//  YRot: rotation (0-360) around the Y axis for the piece
//
#macro Chess_board_jitter_create(XZJitter,YRot)
    #local _jitter  = dictionary {
        .xz_jitter:     XZJitter,
        .yrot_jitter:   YRot
    }
    ;
    
    _jitter    
#end

// The jitter map is initialized with no jitter for all locations
//
#declare Chess_board_jitter_null    = Chess_board_jitter_create(<0,0>,0);

// End Chess_board_jitter_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_jitter_state_create()
//
//      Creates a matrix[8][8] representation of the jitter state of the whole
//      board for storing the individual location jitter values.
//
#macro Chess_board_jitter_state_create()
    #local _jitter_state    = array[8][8];
    
    #for (i, 0, 7, 1)
        #for (j, 0, 7, 1)
            #local _jitter_state[i][j]  = Chess_board_jitter_null;
        #end
    #end
    
    _jitter_state        
#end

// End Chess_board_jitter_state_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_jitter_state_set_index(JitterState,Index,Jitter)
//
//      Sets the jitter at a given <col,row> index
//
//  JitterState:    result of calling Chess_board_jitter_state_create()
//  Index:  <col,row> to set
//  Jitter: result of calling Chess_board_jitter_create()
//
#macro Chess_board_jitter_state_set_index(JitterState,Index,Jitter)
    #local JitterState[Index.y][Index.x]  = Jitter;
#end

// End Chess_board_jitter_state_set_index
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_jitter_state_set(JitterState,Pos,Jitter)
// 
//      This is equivalent to Chess_board_jitter_state_set_index() but uses
//      the "a8" format to represent the position.
//
//  JitterState:    result of calling Chess_board_jitter_state_create()
//  Pos:  position in "a8" format to set
//  Jitter: result of calling Chess_board_jitter_create()
//
#macro Chess_board_jitter_state_set(JitterState,Pos,Jitter)
    #local _index   = Chess_board_pos_to_index(Pos);
    #local JitterState[_index.y][_index.x]    = Jitter; 
#end

// End Chess_board_jitter_state_set
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_layout_place_piece_jitter(ChessBoardState,ChessSetPiece,BoardIndex)
//
//      This macro can be used in an implementation of Chess_board_layout_place_piece_custom()
//      for use with Chess_board_layout() to use the jitter system defined here.
//
//    Usage:
//    #local _board_state   = Chess_board_state_create(...);
//    #local _board_state.jitter_state    = Chess_board_jitter_state_create();
//    (perform board state operations... and jitter setting operations)
//
//    #macro Chess_board_layout_place_piece_custom(ChessBoardState,ChessSetPiece,BoardIndex)
//        #local _object  = object { Chess_board_layout_place_piece_jitter(ChessBoardState,ChessSetPiece,BoardIndex) }
//        _object
//    #end
//    #local _board = Chess_board_layout(_board_state)
//
#macro Chess_board_layout_place_piece_jitter(ChessBoardState,ChessSetPiece,BoardIndex)
    #local _board_loc   = Chess_set_board_loc_index(ChessBoardState.chess_set.board,BoardIndex);
    #ifndef (ChessBoardState.jitter_state)
        #declare ChessBoardState.jitter_state    = Chess_board_jitter_state_create();
    #end
    #local _jitter                          = ChessBoardState.jitter_state[BoardIndex.y][BoardIndex.x];
    //#debug concat("_jitter[", vstr(2,BoardIndex,",",0,0), "]={ .xz_jitter: ", vstr(2, _jitter.xz_jitter, ",", 0, 3), ", .yrot_jitter: ", str(_jitter.yrot_jitter, 0, 3), "}\n") 
    #local _yrot    = _jitter.yrot_jitter;
    #local _xz      = _jitter.xz_jitter;
    #local _object  = object {
        ChessSetPiece.piece_obj
        rotate <0, _yrot, 0>
        translate _board_loc + <_xz.x, 0, _xz.y>
    }                 
    
    _object
#end

// End Chess_board_layout_place_piece_jitter
//-----------------------------------------------------------------------------


// End Standard Chess Board Jitter Macros
//=============================================================================

#version Libchess_Inc_Temp;

#end // ifndef (Libchess_Inc_Temp)
