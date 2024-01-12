/******************************************************************************
 * File: brickwall_mortar.inc
 * Description:
 *      This file contains declarations for defining the mortar between bricks
 *      in a brick wall.
 ******************************************************************************/

#ifndef(Brickwall_mortar_Inc_Temp)

#declare Brickwall_mortar_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including brickwall_mortar.inc\n"
#end                     

//=============================================================================
// Required Include Files
//
// All files required for the library to work should be included here.  There
// should be no assumption that any given include file is included prior to
// the library.inc file.
//
// Further, to preserve this library as something that can be packaged up to
// export to news.povray.org, only include standard POV-Ray include files or
// other files in this library directory.
//
#include "libgeom.inc"

// End Required Include Files
//=============================================================================

//=============================================================================
// Mortar Block
//      A Mortar_block is a dictionary that contains the origin and size
//      as well as the shape function for a block of mortar.
//

//-----------------------------------------------------------------------------
// Mortar_block_create(Origin,Size,ShapeFn)
//
#macro Mortar_block_create(Origin,Size,ShapeFn)
    #local _block   = dictionary {
        .origin:    Origin,
        .sz:        Size,
        .shape_fn:  ShapeFn
    }
    ;
    
    _block
#end

// End Mortar_block_create
//-----------------------------------------------------------------------------

// End Mortar Block
//=============================================================================

//=============================================================================
// Mortar Joints
//
//      A mortar joint defines the depth of mortar between two bricks.  It
//      consists of a function that defines the shape of the joint and a depth
//      value, such that positive depth values indicate joints that cut into
//      flush mortar, and negative depth values extrude out of flush mortar.
//
//      A joint depth of 0 is always flush
//

//-----------------------------------------------------------------------------
// Mortar_joint_create(Depth,ShapeFn)
//
//      Creates a Mortar_joint, which is a dictionary containing the depth
//      and the shape of the joint.
//
//      If Depth < 0, the joint extends out from the mortar
//      If Depth = 0, the joint is flush
//      If Depth > 0, the joint digs into the mortar
//
//      ShapeFn is a function(x,y,z,xoff,yoff,zoff) where x, y, and z are the
//      absolute position of the point, and xoff, yoff, and zoff are the offset
//      within the joint.  Usually, the absolute positions are used to add
//      noise and texture to the joint, while the xoff, yoff, and zoff are
//      used to define the shape.
//
//      ShapeFn should define the joint for mortar centered on the y=0 plane,
//      extending along the x axis infinitely, and cutting into or extruding
//      backwards from the z=0 plane.
//
#macro Mortar_joint_create(Depth,ShapeFn)
    #local _joint   = dictionary {
        .depth:     Depth,
        .shape_fn:  ShapeFn
    }
    ;
    
    _joint
#end

// End Mortar_joint_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mortar_joint_flush_fn()
//
#macro Mortar_joint_flush_fn()
    function(x,y,z,xoff,yoff,zoff) { 0 }
#end    

// End Mortar_joint_flush_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mortar_joint_rake_fn(Depth,MortarSize)
//
#macro Mortar_joint_rake_fn(Depth,MortarSize)
    function(x,y,z,xoff,yoff,zoff) {
        max(
            abs(yoff) - MortarSize/2,
            zoff - Depth
        )    
    }
#end

// End Mortar_joint_rake_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mortar_joint_weather_fn(Depth,MortarSize)
//
#macro Mortar_joint_weather_fn(Depth,MortarSize)
    #local _slope   = Depth/MortarSize;
    function(x,y,z,xoff,yoff,zoff) {
        zoff - (yoff + MortarSize/2)*_slope
    }
#end

// End Mortar_joint_weather_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mortar_joint_struck_fn(Depth,MortarSize)
//
#macro Mortar_joint_struck_fn(Depth,MortarSize)
    #local _slope   = Depth/MortarSize;
    function(x,y,z,xoff,yoff,zoff) {
        zoff - (Depth - (yoff + MortarSize/2)*_slope)
    }
#end

// End Mortar_joint_struck_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mortar_joint_vee_fn(Depth,MortarSize)
//
#macro Mortar_joint_vee_fn(Depth,MortarSize)
    #local _slope   = 2*Depth/MortarSize;
    function(x,y,z,xoff,yoff,zoff) {
        zoff - (Depth - abs(yoff)*_slope)
    }
#end

// End Mortar_joint_vee_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mortar_joint_concave_fn(Depth,MortarSize)
//
#macro Mortar_joint_concave_fn(Depth,MortarSize)
    #local _cyl_radius      = ChordHeight2Radius(MortarSize,Depth);
    #local _cyl_z_offset    = Depth - _cyl_radius;
    
    function(x,y,z,xoff,yoff,zoff) {
        sqrt(yoff*yoff + pow(zoff - _cyl_z_offset,2)) - _cyl_radius
    }    
#end

// End Mortar_joint_concave_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mortar_joint_extruded_fn(Depth,MortarSize)
//
#macro Mortar_joint_extruded_fn(Depth,MortarSize)
    #local _cyl_radius      = ChordHeight2Radius(MortarSize,Depth);
    #local _cyl_z_offset    = _cyl_radius - Depth;
    
    function(x,y,z,xoff,yoff,zoff) {
        max(
            zoff,
            sqrt(yoff*yoff + pow(zoff - _cyl_z_offset,2)) - _cyl_radius
        )    
    }    
#end

// End Mortar_joint_extruded_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mortar_joint_grapevine_fn(Depth)
//
#macro Mortar_joint_grapevine_fn(Depth)
    function(x,y,z,xoff,yoff,zoff) {
        sqrt(yoff*yoff + zoff*zoff) - Depth
    }    
#end

// End Mortar_joint_grapevine_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mortar_joint_beaded_fn(Depth)
//
#macro Mortar_joint_beaded_fn(Depth)
    function(x,y,z,xoff,yoff,zoff) {
        max(
            zoff,
            sqrt(yoff*yoff + zoff*zoff) - Depth
        )    
    }    
#end

// End Mortar_joint_beaded_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mortar_joint_spec_create(BedJoint,HeaderJoint,FaceJoint)
//      Creates a dictionary containing joint specifications for the bed,
//      header, and face mortar in a brick wall.
//
#macro Mortar_joint_spec_create(BedJoint,HeaderJoint,FaceJoint)
    #local _spec    = dictionary {
        .bed:       BedJoint,
        .header:    HeaderJoint,
        .face:      FaceJoint,
        .back_joints:   true
    }
    ;
    
    _spec    
#end

// End Mortar_joint_spec_create
//-----------------------------------------------------------------------------

// End Mortar Joints
//=============================================================================

#version Brickwall_mortar_Inc_Temp;
#end // ifndef(Brickwall_mortar_Inc_Temp)