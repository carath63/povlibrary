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
// Chess Piece Declarations
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

// End Chess Piece Declarations
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

#declare Chess_set_piece_none   = dictionary {
    .piece_type:    Chess_piece_type_none
}
;    

// End Chess_set_piece_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_player_create()
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
#macro Chess_set_add_piece(ChessSet,ChessSetPiece)
    Chess_set_player_add_piece(ChessSet.pieces[ChessSetPiece.player],ChessSetPiece)
#end

// End Chess_set_add_piece
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_set_player_complete(ChessSetPlayer)
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
            #if (_piece.piece = Chess_piece_type_none)
                #debug concat(" ", Chess_piece_type_names[_piece.piece],"|")
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
// Chess_board_layout(ChessSet,ChessBoardState)
//

//-----------------------------------------------------------------------------
// Chess_board_layout_place_piece(ChessBoardState,ChessSetPiece,BoardIndex)
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

//-----------------------------------------------------------------------------
// Chess_board_jitter_create(XZJitter,YRot)
//
#macro Chess_board_jitter_create(XZJitter,YRot)
    #local _jitter  = dictionary {
        .xz_jitter:     XZJitter,
        .yrot_jitter:   YRot
    }
    ;
    
    _jitter    
#end

#declare Chess_board_jitter_null    = Chess_board_jitter_create(<0,0>,0);

// End Chess_board_jitter_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_jitter_state_create()
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
#macro Chess_board_jitter_state_set_index(JitterState,Index,Jitter)
    #local JitterState[Index.y][Index.x]  = Jitter;
#end

// End Chess_board_jitter_state_set_index
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chess_board_jitter_state_set(JitterState,Pos,Jitter)
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
