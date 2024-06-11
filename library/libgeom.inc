/******************************************************************************
 * File: libgeom.inc
 * Description:
 *      Useful macros for geometry computations
 ******************************************************************************/

#ifndef(Libgeom_Inc_Temp)

#declare Libgeom_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including lib_template.inc\n"
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

// End Required Include Files
//=============================================================================

//=============================================================================
// Circles, chords, and arcs
//

//-----------------------------------------------------------------------------
// ChordArc2Radius(ChordLength,ArcHeight)
//
// Computes the radius of the circle that contains a chord of the given length
// with the given arc height.
//
#macro ChordHeight2Radius(ChordLength,ArcHeight)
    ((ChordLength*ChordLength)/(8*ArcHeight) + (ArcHeight/2))
#end

// Deprecated
#macro ChordArc2Radius(ChordLength,ArcHeight)
    ChordHeight2Radius(ChordLength,ArcHeight)
#end

// End ChordArc2Radius
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ChordRadius2Height(ChordLength,Radius)
//
// Computes the arc height for a chord of the given length in a circle with
// the given radius.
//
#macro ChordRadius2Height(ChordLength,Radius)
    #local _a   = ChordLength/2;
    (Radius - sqrt(pow(Radius,2)-pow(_a,2)))
#end

// End ChordRadius2Height
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// RadiusHeight2Chord(Radius,Height)
//
// Computes the length of the chord with the given arc height in a circle
// with the given radius.
//
#macro RadiusHeight2Chord(Radius,Height)
    #if (Radius < 0 | Height < 0 | Height > 2*Radius)
        #error concat("RadiusHeight2Chord(", str(Radius,0,5), ",", str(Height,0,5), "): Invalid Height\n")
    #end    
    2*(sqrt(2*Radius*Height - Height*Height))
#end

// End RadiusHeight2Chord
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chords2Radius(ChordLength1,ChordLength2,Distance)
//
// Given two chords of different lengths and the vertical
// distance between them, compute the radius of the circle
// that contains both chords
#macro Chords2Radius(ChordLength1,ChordLength2,Distance)
    #local _chord1_offset   = (Distance*Distance - ChordLength1*ChordLength1 + ChordLength2*ChordLength2)/(2*Distance);
    #local _radius          = sqrt(ChordLength1*ChordLength2 + _chord1_offset*_chord1_offset);
    
    (_radius)
#end

// End Chords2Radius
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ChordRadius2Arc(ChordLength,Radius)
//
// Computes the arc length in radians for a chord of the given length in a
// circle with the given radius.
//
#macro ChordRadius2Arc(ChordLength,Radius)
    2*asin((ChordLength/2)/Radius)
#end

// End ChordRadius2Arc
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ArcRadius2Chord(ArcRadians,Radius)
//
// Computes the length of the chord covering the given ArcRadians in a circle
// with the given Radius.
//
#macro ArcRadius2Chord(ArcRadians,Radius)
    sqrt(
        2*Radius*Radius*(1 - cos(ArcRadians))
    )
#end

// End ArcRadius2Chord
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ArcRadius2Height(ArcRadians,Radius)
//
// Computes the arc height of the chord covering the given ArcRadians in a circle
// with the given Radius.
//
#macro ArcRadius2Height(ArcRadians,Radius)
    Radius*(1 - cos(ArcRadians/2))
#end

// End ArcRadius2Height
//-----------------------------------------------------------------------------

// End Circles, chords, and arcs
//=============================================================================

//=============================================================================
// Non-standard rotation order macros
//

//-----------------------------------------------------------------------------
// XZY_rotate(Pt,Rotation)
//
// Rotate Pt first around the X axis, then the Z axis, then the Y axis.
// Useful for rotating objects that lie along the X axis and have full freedom
// of movement.
//
#macro XZY_rotate(Pt,Rotation)
    #local _r   = <1,1,1>*Rotation;
    #local _p   = vrotate(
        vrotate(
            Pt,
            <_r.x,0,_r.z>
        ),
        <0, _r.y, 0>
    );
    
    _p        
#end

// End XZY_rotate
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// XZY_rotation_transform(Rotation)
//
// Creates a transform{} that rotates around the X axis, then the Z axis, then
// the Y axis.
//
#macro XZY_rotation_transform(Rotation)
    #local _r   = <1,1,1>*Rotation;
    #local _t   = transform {
        #if (_r.x != 0 | _r.z != 0)
            rotate <_r.x, 0, _r.z>
        #end 
        #if (_r.y != 0)
            rotate <0, _r.y, 0>
        #end    
    }
    
    _t
#end

// End XZY_rotation_transform
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ZXY_rotate(Pt,Rotation)
//
// Rotate Pt first around the X axis, then the Z axis, then the Y axis.
// Useful for rotating objects that lie along the X axis and have full freedom
// of movement.
//
#macro ZXY_rotate(Pt,Rotation)
    #local _r   = <1,1,1>*Rotation;
    #local _p   = vrotate(
        vrotate(
            Pt,
            <0,0,_r.z>
        ),
        <_r.x, _r.y, 0>
    );
    
    _p        
#end

// End ZXY_rotate
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ZXY_rotation_transform(Rotation)
//
// Creates a transform{} that rotates around the X axis, then the Z axis, then
// the Y axis.
//
#macro ZXY_rotation_transform(Rotation)
    #local _r   = <1,1,1>*Rotation;
    #local _t   = transform {
        #if (_r.z != 0)
            rotate <0, 0, _r.z>
        #end 
        #if (_r.y != 0 | _r.x != 0)
            rotate <_r.x, _r.y, 0>
        #end    
    }
    
    _t
#end

// End ZXY_rotation_transform
//-----------------------------------------------------------------------------


// End Non-standard rotation order macros
//=============================================================================

#version Libgeom_Inc_Temp;
#end // ifndef(Libgeom_Inc_Temp)