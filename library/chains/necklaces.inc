/******************************************************************************
 * File: necklaces.inc
 * Description:
 *      Collection of chain types used for necklaces, usually parameterized
 *  for different sizings, and with utility functions for using them
 *  with libchain.
 *
 *  All of the chain objects defined here are un-textured so you can apply
 *  any texture/material you want with them.
 ******************************************************************************/

#ifndef(Necklaces_Inc_Temp)

#declare Necklaces_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including necklaces.inc\n"
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
#include "libchain.inc"
#include "libgeom.inc"

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
// Cable Chains
//
//  Cable chains have a single link type, either round, oval, or oblong with
//  alternating links at 90 degrees from each other.
//

//-----------------------------------------------------------------------------
// Cable_chain_torus_spec_create(Major,Minor,optional LinkObj)
//

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Cable_chain_torus_link_spec_create(Major,Minor,optional LinkObj)
//
#macro Cable_chain_torus_link_spec_create(Major,Minor,optional LinkObj)
    #local _link_size   = <2*(Major+Minor), 2*Minor, 2*(Major+Minor)>;
    #local _link_inner  = <2*(Major-Minor), 2*(Major-Minor)>; 
    #local _link_obj    = object { #ifdef(LinkObj) LinkObj #else Cable_chain_torus_link_object_create(Major,Minor) #end } 
    
    #local _link_spec   = Chain_link_spec_create("Cable_chain_torus",_link_size,_link_inner,_link_obj);
    #local _link_spec.cable_major   = Major;
    #local _link_spec.cable_minor   = Minor;
    
    _link_spec
#end

// End Cable_chain_torus_link_spec_create
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Cable_chain_torus_link_object_create(Major,Minor)
//
#macro Cable_chain_torus_link_object_create(Major,Minor)
    #local _link    = torus {
        Major
        Minor
        translate <0, 0, (Major - Minor)>
    }
    
    _link    
#end

// End Cable_chain_torus_link_object_create
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#macro Cable_chain_torus_spec_create(Major,Minor,optional LinkObj)
    #local _link_spec   = Cable_chain_torus_link_spec_create(Major,Minor,LinkObj);
    #local _chain_spec  = Chain_spec_create(_link_spec,);
    
    _chain_spec
#end
// End Cable_chain_torus_spec_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Cable_chain_rolo_spec_create(Major,Minor,optional LinkObj)
//

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Cable_chain_rolo_link_spec_create(Major,Minor,optional LinkObj)
//
#macro Cable_chain_rolo_link_spec_create(Major,Minor,optional LinkObj)
    #local _link_size   = <2*(Major+Minor), 2*Minor, 2*(Major+Minor)>;
    #local _offset      = ChordRadius2Height(2*Minor,Major);
    #local _link_inner  = <2*(Major-_offset), 2*(Major-_offset)>;
    #local _link_obj    = object { #ifdef(LinkObj) LinkObj #else Cable_chain_rolo_link_object_create(Major,Minor) #end }
    
    #local _link_spec   = Chain_link_spec_create("Cable_chain_rolo",_link_size,_link_inner,_link_obj);
    #local _link_spec.rolo_major    = Major;
    #local _link_spec.rolo_minor    = Minor;
    #local _link_spec.rolo_offset   = _offset;
    
    _link_spec
#end

// End Cable_chain_rolo_link_spec_create
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Cable_chain_rolo_link_object_create(Major,Minor)
//
#macro Cable_chain_rolo_link_object_create(Major,Minor)
    #local _offset  = ChordRadius2Height(2*Minor,Major);
    #local _link    = difference {
        torus {
            Major
            Minor
        }
        cylinder {
            <0, -(Minor+1), 0>,
            <0, (Minor+1), 0>,
            Major
        }
        translate <0, 0, (Major - _offset)>        
    }
    
    _link
#end

// End Cable_chain_rolo_link_object_create
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#macro Cable_chain_rolo_spec_create(Major,Minor,optional LinkObj)
    #local _link_spec   = Cable_chain_rolo_link_spec_create(Major,Minor,LinkObj);
    #local _chain_spec  = Chain_spec_create(_link_spec,);
    
    _chain_spec
#end

// End Cable_chain_rolo_spec_create
//-----------------------------------------------------------------------------

// End Cable Chains
//=============================================================================

#version Necklaces_Inc_Temp;

#end // ifndef (Necklaces_Inc_Temp)
