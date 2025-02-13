/******************************************************************************
 * File: scene-template.pov
 * Description:
 *      Template for rendering scenes
 ******************************************************************************/

#version 3.8;

#ifndef (use_radiosity)
    #declare use_radiosity  = 0;
#end

//=============================================================================
// Includes
//
#declare View_POV_Include_Stack=true;

//-----------------------------------------------------------------------------
// Standard Includes
//
#include "camera35mm.inc"
#include "Lightsys.inc"
#include "rand.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
// 
#include "libchess.inc"
#include "chess_sets/cs_simple.inc"

// End My Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project Includes
//

// End Project Includes
//-----------------------------------------------------------------------------

// End Includes
//=============================================================================

global_settings {
    assumed_gamma 1.0
    max_trace_level 10
}

#default {
    finish { ambient 0 diffuse 0.8 }
}        

//=============================================================================
// Scene default configuration
//

// Set the default values, which can be adjusted based on the scene view
//
#declare Scene_seed             = seed(2025021305);

#ifndef (Scene_View)
#declare Scene_View = 1.1;
#end

// End Scene configuration
//=============================================================================

//=============================================================================
// Camera default configuration
//

#declare camera_location        = <0, Math_Scale(SCALE_INCH, 18), -Math_Scale(SCALE_INCH, 18)>;
#declare camera_lookat          = <0, 0, 0>;
#declare camera_blur_focus      = camera_lookat;
#declare camera_use_blur        = false;
#declare camera_focal_length    = Math_Scale(SCALE_MM, 50);
#declare camera_blur_fstop      = 8;

// End Camera configuration
//=============================================================================

//=============================================================================
// Scene object declarations
//

//-----------------------------------------------------------------------------
// Scene_camera_light()
//
// This creates a light located at the camera position as a default
// light source
//
#macro Scene_camera_light()
    #local _l   = light_source {
        camera_location, 1
    }
    
    _l    
#end

// End Scene_camera_light
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_chess_board()
//
#macro Scene_chess_board()
    // Create the simple chess set defined in chess_sets/cs_simple.inc and create
    // an initial board state from that chess set
    #local _chess_set   = CS_simple_chess_set_create();
    #local _board_state = Chess_board_state_create(_chess_set);
    
    // For this sample we are using jitter to place the pieces slightly off-center
    // from the squares for a more random look.  The Chess_board_jitter_state_create()
    // creates the initial <0,0> jitter values for each board position.
    //
    #local _board_state.jitter_state    = Chess_board_jitter_state_create();
    
    // Declaring this macro defines how objects get placed on the board.  Since we are using jitter
    // we use the Chess_board_layout_place_piece_jitter which uses the jitter state created above.
    // 
    #macro Chess_board_layout_place_piece_custom(ChessBoardState,ChessSetPiece,BoardIndex)
        #local _object  = object { Chess_board_layout_place_piece_jitter(ChessBoardState,ChessSetPiece,BoardIndex) }
        _object
    #end
    
    // Put all of the pieces on the board as for the beginning of a chess game
    Chess_board_state_init(_board_state)
    // Move the pawn on e2 to e4 and set a jitter value for the ending position 
    Chess_board_state_move(_board_state,"e2","e4")
    Chess_board_jitter_state_set(_board_state.jitter_state,"e4",Chess_board_jitter_create(0.25*<SRand(Scene_seed),SRand(Scene_seed)>, 15*SRand(Scene_seed)))
    
    Chess_board_state_move(_board_state,"e7","e5")
    Chess_board_jitter_state_set(_board_state.jitter_state,"e5",Chess_board_jitter_create(0.25*<SRand(Scene_seed),SRand(Scene_seed)>, 15*SRand(Scene_seed)))
    Chess_board_state_move(_board_state,"d2","d3")
    Chess_board_jitter_state_set(_board_state.jitter_state,"d3",Chess_board_jitter_create(0.25*<SRand(Scene_seed),SRand(Scene_seed)>, 15*SRand(Scene_seed)))
    Chess_board_state_move(_board_state,"d7","d6")
    Chess_board_jitter_state_set(_board_state.jitter_state,"d6",Chess_board_jitter_create(0.25*<SRand(Scene_seed),SRand(Scene_seed)>, 15*SRand(Scene_seed)))
    Chess_board_state_move(_board_state,"b1","c3")
    Chess_board_jitter_state_set(_board_state.jitter_state,"c3",Chess_board_jitter_create(0.25*<SRand(Scene_seed),SRand(Scene_seed)>, 15*SRand(Scene_seed)))
    Chess_board_state_move(_board_state,"b8","c6")
    Chess_board_jitter_state_set(_board_state.jitter_state,"c6",Chess_board_jitter_create(0.25*<SRand(Scene_seed),SRand(Scene_seed)>, 15*SRand(Scene_seed)))
    
    // Dump the current state of the board
    Chess_board_state_dump(_board_state,true)
    
    // Use Chess_board_layout, which creates a combined object consisting of the board from the chess set with all of the pieces
    // currently on the board in their jittered position
    //
    #local _board   = object {
        Chess_board_layout(_board_state)
    }    
    
    _board       
#end

// End Scene_chess_board
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_object_create()
//
// Redefine this to create the scene.  It will be instantiated below
#macro Scene_object_create()
    #local _scene   = union {
        object { Scene_chess_board() }
        object { Scene_camera_light() }
    }
    
    _scene
#end

// End Scene_object_create
//-----------------------------------------------------------------------------


// End Scene object declarations
//=============================================================================

//=============================================================================
// View-specific configuration overrides
//

#switch(val(str(Scene_View,0,2)))
    #case(1.1)
    #break
    
    #else
        #warning concat("Unknown Scene_View: ", str(Scene_View, 0, 2), "\n")
    #break
#end // switch(Scene_View)

// End View-specific configuration overrides
//=============================================================================

//=============================================================================
// Scene declaration
//
#ifndef(Scene_object)
#declare Scene_object    = object {
    Scene_object_create()
} 
#end

// End Scene declaration
//=============================================================================

//=============================================================================
// Camera
//        
#ifdef (camera_blur_samples)
    Camera35mm_SetFocalSamples(camera_blur_samples)
#end
#ifdef (camera_blur_variance)
    Camera35mm_SetFocalVariance(camera_blur_variance)
#end
#ifdef (camera_blur_confidence)
    Camera35mm_SetFocalConfidence(camera_blur_confidence)
#end

#if (camera_use_blur)
    #ifdef (camera_blur_focus)
        Camera35mm_Point( camera_location, camera_lookat, camera_blur_focus, camera_focal_length, camera_blur_fstop )
    #else
        Camera35mm( camera_location, camera_lookat, camera_focal_length, camera_blur_fstop )
    #end
#else
    Camera35mm_NoBlur(camera_location, camera_lookat, camera_focal_length, camera_blur_fstop )
#end    
Camera35mm_PrintInfo( camera_location, camera_lookat, camera_focal_length, camera_blur_fstop, camera_blur_focus )

// End Camera
//=============================================================================

//=============================================================================
// Scene Instantiation
//

object { Scene_object }

// End Scene Instantiation
//=============================================================================
    