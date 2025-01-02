/******************************************************************************
 * File: libchess.inc
 * Description:
 *      Libchess ".inc" file
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
// Chess Piece and Board Representations
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

#declare Chess_piece_side_none  = 0;
#declare Chess_piece_side_king  = 1;
#declare Chess_piece_side_queen = 2;

//-----------------------------------------------------------------------------
// Chess_piece_create(Player,Piece,optional Side)
//
#macro Chess_piece_create(Player,Piece,optional Side)

    #ifdef(Side)
        #local _side    = Side;
    #else
        #local _side    = Chess_piece_side_none;
    #end        
    #local _piece   = dictionary {
        .player:    Player,
        .piece:     Piece,
        .side:      _side
    }
    ;
    
    _piece
#end

#declare Chess_piece_none   = dictionary {
    .piece: Chess_piece_type_none
}
;    

// End Chess_piece_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_pawn_create(Player,Column)
//
#macro Chess_pawn_create(Player,Column)
    #local _pawn    = dictionary {
        .player:    Player,
        .piece:     Chess_piece_type_pawn,
        .column:    Column
    }
    ;
    
    _pawn
#end

// End Chess_pawn_create
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
// Chess_board_graveyard_create(Player)
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
#macro Chess_board_graveyard_add(Graveyard,Piece)
    #declare Graveyard.pieces[Graveyard.num_pieces] = Piece;
    #declare Graveyard.num_pieces                   = Graveyard.num_pieces + 1;
#end

// End Chess_board_graveyard_add
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_create()
//
#macro Chess_board_state_create()
    #local _states  = array[8][8];
    #for (i, 0, 7, 1)
        #for (j, 0, 7, 1)
            #local _states[i][j]    = Chess_piece_none;
        #end
    #end        
    #local _board   = dictionary {
        .white_graveyard:   Chess_board_graveyard_create(Chess_player_white),
        .black_graveyard:   Chess_board_graveyard_create(Chess_player_black),
        .board_state:       _states
    }
    ;
    
    _board
#end

// End Chess_board_state_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_set(BoardState,Pos,Piece)
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
#macro Chess_board_state_get_index(BoardState,Index)
    #local _piece   = BoardState.board_state[Index.y][Index.x];
    
    _piece 
#end

// End Chess_board_state_get_index
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_move(BoardState,PosFrom,PosTo)
//
#macro Chess_board_state_move(BoardState,PosFrom,PosTo)
    #local _idx2    = Chess_board_pos_to_index(PosTo);
    
    #local _piece   = Chess_board_state_get(BoardState,PosFrom);
    #if (_piece.piece = Chess_piece_type_none)
        #error concat("There is no piece at ", PosFrom, " to be moved\n")
    #end
    Chess_board_state_set(BoardState,PosFrom,Chess_piece_none)
    
    #local _piece2  = Chess_board_state_get(BoardState,PosTo);
    #if (_piece2.piece != Chess_piece_type_none)
        #if (_piece2.player = _piece.player)
            #error concat("Illegal move from ", PosFrom, " to ", PosTo, ": player's piece on that spot\n")
        #end    
        #if (_piece2.player = Chess_player_white)
            Chess_board_graveyard_add(BoardState.white_graveyard,_piece2)
        #else                                                            
            Chess_board_graveyard_add(BoardState.black_graveyard,_piece2)
        #end
    #end
    Chess_board_state_set(BoardState,PosTo,_piece)    
#end

// End Chess_board_state_move
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_pawn_promote(BoardState,PosFrom,PosTo,Piece)
//
#macro Chess_board_state_pawn_promote(BoardState,PosFrom,PosTo,Piece)
    #local _piece   = Chess_board_state_get(BoardState,PosFrom);
    #if (_piece.piece != Chess_piece_type_pawn)
        #error concat("Illegal pawn promotion: Piece at ", PosFrom, " is not a pawn.\n")
    #end
    Chess_board_state_move(BoardState,PosFrom,PosTo)
    #if (_piece.player = Chess_player_white)
        Chess_board_graveyard_add(BoardState.white_graveyard,_piece)
    #else                                                            
        Chess_board_graveyard_add(BoardState.black_graveyard,_piece)
    #end
    Chess_board_state_set(BoardState,PosTo,Piece)
        
#end

// End Chess_board_state_pawn_promote
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_castle(BoardState,Player,Side)
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
    #if (_king.piece != Chess_piece_type_king | _king.player != Player)
        #error concat("Illegal castle: piece at ", _king_from, " is not ", Chess_player_names[Player], "'s king\n")
    #end
    #local _rook    = Chess_board_state_get(BoardState,_rook_from);
    #if (_rook.piece != Chess_piece_type_rook | _rook.player != Player)
        #error concat("Illegal castle: piece at ", _rook_from, " is not ", Chess_player_names[Player], "'s rook\n")
    #end
    Chess_board_state_set(BoardState,_king_from,Chess_piece_none)
    Chess_board_state_set(BoardState,_king_to,_king)
    Chess_board_state_set(BoardState,_rook_from,Chess_piece_none)
    Chess_board_state_set(BoardState,_rook_to,_rook)        
#end

// End Chess_board_state_castle
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_init(BoardState)
//
#macro Chess_board_state_init(BoardState)
    Chess_board_state_set(BoardState,"a1",Chess_piece_create(Chess_player_white,Chess_piece_type_rook,Chess_piece_side_queen))
    Chess_board_state_set(BoardState,"b1",Chess_piece_create(Chess_player_white,Chess_piece_type_knight,Chess_piece_side_queen))
    Chess_board_state_set(BoardState,"c1",Chess_piece_create(Chess_player_white,Chess_piece_type_bishop,Chess_piece_side_queen))
    Chess_board_state_set(BoardState,"d1",Chess_piece_create(Chess_player_white,Chess_piece_type_queen,))
    Chess_board_state_set(BoardState,"e1",Chess_piece_create(Chess_player_white,Chess_piece_type_king,))
    Chess_board_state_set(BoardState,"f1",Chess_piece_create(Chess_player_white,Chess_piece_type_bishop,Chess_piece_side_king))
    Chess_board_state_set(BoardState,"g1",Chess_piece_create(Chess_player_white,Chess_piece_type_knight,Chess_piece_side_king))
    Chess_board_state_set(BoardState,"h1",Chess_piece_create(Chess_player_white,Chess_piece_type_rook,Chess_piece_side_king))
    
    Chess_board_state_set(BoardState,"a8",Chess_piece_create(Chess_player_black,Chess_piece_type_rook,Chess_piece_side_queen))
    Chess_board_state_set(BoardState,"b8",Chess_piece_create(Chess_player_black,Chess_piece_type_knight,Chess_piece_side_queen))
    Chess_board_state_set(BoardState,"c8",Chess_piece_create(Chess_player_black,Chess_piece_type_bishop,Chess_piece_side_queen))
    Chess_board_state_set(BoardState,"d8",Chess_piece_create(Chess_player_black,Chess_piece_type_queen,))
    Chess_board_state_set(BoardState,"e8",Chess_piece_create(Chess_player_black,Chess_piece_type_king,))
    Chess_board_state_set(BoardState,"f8",Chess_piece_create(Chess_player_black,Chess_piece_type_bishop,Chess_piece_side_king))
    Chess_board_state_set(BoardState,"g8",Chess_piece_create(Chess_player_black,Chess_piece_type_knight,Chess_piece_side_king))
    Chess_board_state_set(BoardState,"h8",Chess_piece_create(Chess_player_black,Chess_piece_type_rook,Chess_piece_side_king))
    
    #local _a   = asc("a");
    
    #for (i, 1, 8, 1)
        #local _pos1    = concat(chr(_a+i-1),"2");
        #local _pos2    = concat(chr(_a+i-1),"7");
        Chess_board_state_set(BoardState,_pos1,Chess_pawn_create(Chess_player_white,i)) 
        Chess_board_state_set(BoardState,_pos2,Chess_pawn_create(Chess_player_black,i)) 
    #end
    
#end

// End Chess_board_state_init
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_state_dump(BoardState,Graveyards)
//
#macro Chess_board_state_dump(BoardState,Graveyards)
    #if (Graveyards & BoardState.black_graveyard.num_pieces > 0)
        #for(i, 0, BoardState.black_graveyard.num_pieces-1, 1)
            #debug concat(Chess_piece_type_names[BardState.black_graveyard.pieces[i].piece],",")
        #end
    #end
    #for (i, 7, 0, -1)
        #debug "|"
        #for (j, 0, 7, 1)
            #local _piece   = BoardState.board_state[i][j];
            #if (_piece.piece = Chess_piece_type_none)
                #debug concat(" ", Chess_piece_type_names[_piece.piece],"|")
            #else                                                       
                #debug concat(Chess_player_abbrevs[_piece.player], Chess_piece_type_names[_piece.piece],"|")
            #end
        #end
        #debug "\n"
    #end
    #if (Graveyards & BoardState.white_graveyard.num_pieces > 0)
        #for(i, 0, BoardState.white_graveyard.num_pieces-1, 1)
            #debug concat(Chess_piece_type_names[BardState.white_graveyard.pieces[i].piece],",")
        #end
    #end
#end

// End Chess_board_state_dump
//-----------------------------------------------------------------------------

// End Chess Piece and Board Representations
//=============================================================================

//=============================================================================
// Chess Set Definitions
//

//-----------------------------------------------------------------------------
// Chess_set_board_create(BoardSize,SquareSize,BoardObj)
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
#macro Chess_set_board_loc_index(ChessSetBoard,Index)
    #local _loc     = <(-3.5 + Index.x)*ChessSetBoard.square_size, ChessSetBoard.board_size.y, (-3.5 + Index.y)*ChessSetBoard.square_size>;
    
    _loc
#end

// End Chess_set_board_loc_index
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_piece_create(PieceSize,Piece,PieceObj)
//
#macro Chess_set_piece_create(PieceSize,Piece,PieceObj)
    #local _piece   = dictionary {
        .piece_size:    PieceSize,
        .piece:         Piece,
        .piece_obj:     PieceObj
    }
    ;
    
    _piece    
#end

#declare Chess_set_piece_none   = dictionary {
    .piece: Chess_piece_none
}
;    

// End Chess_set_piece_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_create(Board)
//
#macro Chess_set_create(Board)
    #local _set = dictionary {
        .board: Board,
        .white_pieces:  array,
        .white_count:   0,
        .black_pieces:  array,
        .black_count:   0
    }
    ;
    
    _set
#end

// End Chess_set_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_add_piece(ChessSet,ChessSetPiece)
//
#macro Chess_set_add_piece(ChessSet,ChessSetPiece)
    #if (ChessSetPiece.piece.player = Chess_player_white)
        #declare ChessSet.white_pieces[ChessSet.white_count]    = ChessSetPiece;
        #declare ChessSet.white_count                           = ChessSet.white_count + 1;
    #else
        #declare ChessSet.black_pieces[ChessSet.black_count]    = ChessSetPiece;
        #declare ChessSet.black_count                           = ChessSet.black_count + 1;
    #end
#end

// End Chess_set_add_piece
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_find_piece(ChessSet,Piece,Remove)
//
#macro Chess_set_find_piece(ChessSet,Piece,Remove)
    #macro _find(PieceList,PieceCount,Remove)
        #local _found   = Chess_set_piece_none;
        #local _i       = 0;
        #while (_i < PieceCount & (_found.piece.piece = Chess_piece_type_none))
            #local _p   = PieceList[_i];
            #if (_p.piece.piece = Piece.piece)
                #local _found   = _p;
                #if (Remove)
                    #while (_i+1 < PieceCount)
                        #declare PieceList[_i]  = PieceList[_i+1];
                        #local _i   = _i + 1;
                    #end
                #end
                
            #else
                #local _i   = _i + 1;    
            #end
        #end 
        _found
    #end
    
    #if (Piece.player = Chess_player_white)
        #local _piece   = _find(ChessSet.white_pieces,ChessSet.white_count,Remove);
        #if (Remove & _piece.piece.piece != Chess_piece_type_none)
            #declare ChessSet.white_count   = ChessSet.white_count - 1;
        #end    
    #else
        #local _piece   = _find(ChessSet.black_pieces,ChessSet.black_count,Remove);
        #if (Remove & _piece.piece.piece != Chess_piece_type_none)
            #declare ChessSet.black_count   = ChessSet.black_count - 1;
        #end    
    #end
    
    _piece
#end

// End Chess_set_find_piece
//-----------------------------------------------------------------------------

// End Chess Set Definitions
//=============================================================================

//=============================================================================
// Chess_board_layout(ChessSet,ChessBoardState)
//

//-----------------------------------------------------------------------------
// Chess_board_layout_place_piece(SetPiece,BoardLoc)
//
#macro Chess_board_layout_place_piece(SetPiece,BoardLoc)
    #ifdef (Chess_board_layout_place_piece_custom)
        #local _object  = object {
            Chess_board_layout_place_piece_custom(SetPiece,BoardLoc)
        }    
    #else
        #local _object  = object {
            SetPiece.piece_obj
            translate BoardLoc
        }    
    #end
    
    _object
#end

// End Chess_board_layout_place_piece
//-----------------------------------------------------------------------------

#macro Chess_board_layout(ChessSet,ChessBoardState)
    #local _layout  = union {
        object { ChessSet.board.board_obj }
        #for (i, 0, 7, 1)
            #for (j, 0, 7, 1)
                #local _loc_piece   = Chess_board_state_get_index(ChessBoardState, <i,j>);
                #if (_loc_piece.piece != Chess_piece_type_none)
                    #local _set_piece   = Chess_set_find_piece(ChessSet,_loc_piece,true);
                    #if (_set_piece.piece.piece = Chess_piece_type_none)
                        #error concat("Could not find a ", Chess_player_names[_loc_piece.player], " ", Chess_piece_type_names[_loc_piece.piece], " in the chess set\n")
                    #end
                    #local _board_loc   = Chess_set_board_loc_index(ChessSet.board,<i,j>);
                    object { Chess_board_layout_place_piece(_set_piece,_board_loc) }    
                #end
            #end
        #end    
    }
    
    _layout
#end

// End Chess_board_layout
//=============================================================================

#version Libchess_Inc_Temp;

#end // ifndef (Libchess_Inc_Temp)
