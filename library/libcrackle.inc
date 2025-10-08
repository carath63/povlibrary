/******************************************************************************
 * File: libcrackle.inc
 * Description:
 *      Library of utilities related to the crackle pattern
 ******************************************************************************/

#ifndef(Libcrackle_Inc_Temp)

#declare Libcrackle_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libcrackle.inc\n"
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
// Crackle Pavers
//
// These are macros for creating shape functions for an isosurface that
// defines a set of pavers whose shape is defined by a crackle pattern.
//

//-----------------------------------------------------------------------------
// Crackle_paver_shape_fn(CracklePigment,MaxHeight)
//
// Generates a function(x,y,z) that computes the height of a stone paver
// based on evaluating the given CracklePigment at (x,0,z).  The bounds of
// the shape should be <*, 0, *> to <*, MaxHeight, *>
//
#macro Crackle_paver_shape_fn(CracklePigment,MaxHeight)
    #local _cps_crackle_pfn = function {
        pigment { CracklePigment }
    }
    
    function(x,y,z) {
        y - _cps_crackle_pfn(x,0,z).gray*MaxHeight
    }    
#end

// End Crackle_paver_shape_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Crackle_paver_pigment_pattern(CracklePigment,optional CMap)
//
// Creates a function{} pigment pattern based on the given CracklePigment
// that is modified to be "solid" and have a color_map ranging from 0.0 to 1.0,
// unless the optional color_map { CMap } is supplied, in which case that
// is used instead of isomorphic mapping
//
#macro Crackle_paver_pigment_pattern(CracklePigment)
    #local _cppp_pfn    = function {
        pigment {
            CracklePigment
            solid
            color_map {
            #ifdef(CMap)
                CMap
            #else   
                [0.0 rgb 0]
                [1.0 rgb 1]
            #end       
            }
        }
    }
    
    #local _p   = pigment {
        function { _cppp_pfn(x,0,z).gray }
    }
    
    _p            
            
#end

// End Crackle_paver_pigment_pattern
//-----------------------------------------------------------------------------


// End Crackle Pavers
//=============================================================================

#version Libcrackle_Inc_Temp;

#end // ifndef (Libcrackle_Inc_Temp)
