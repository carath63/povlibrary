/******************************************************************************
 * File: libgeom.inc
 * Description:
 *      Useful macros for geometry computations
 ******************************************************************************/

#ifndef(Libgeom_Inc_Temp)

#declare Libgeom_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libgeom.inc\n"
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
    #if (Radius < 0 | Height < 0 | Height > Radius)
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

//=============================================================================
// Object testing macros
//

//-----------------------------------------------------------------------------
// f_overlaps(min1,max1,min2,max2)
// f_overlaps_2d(minx1,maxx1,miny1,maxy1,minx2,maxx2,miny2,maxy2)
// f_overlaps_3d(minx1,maxx1,miny1,maxy1,minz1,maxz1,minx2,maxx2,miny2,maxy2,minz2,maxz2)
//
// Returns 1 if the numeric range [min1,max1] overlaps with the range [min2,max2]
// else 0
//
#declare f_overlaps = function (min1,max1,min2,max2) {
    select(min1 - max2,
        select(min2 - max1,
            1,
            1,
            0
        ),
        1,
        0
    )
}

#declare f_overlaps_2d  = function(minx1,maxx1,miny1,maxy1,minx2,maxx2,miny2,maxy2) {
    f_overlaps(minx1,maxx1,minx2,maxx2) * f_overlaps(miny1,maxy1,miny2,maxy2)
}

#declare f_overlaps_3d  = function(minx1,maxx1,miny1,maxy1,minz1,maxz1,minx2,maxx2,miny2,maxy2,minz2,maxz2) {
    f_overlaps(minx1,maxx1,minx2,maxx2) * f_overlaps(miny1,maxy1,miny2,maxy2) * f_overlaps(minz1,maxz1,minz2,maxz2)
}

// End f_overlaps
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Overlaps(Obj1,Obj2)
//
#macro Overlaps(Obj1,Obj2)
    #local _min_ext1    = min_extent(Obj1);
    #local _max_ext1    = max_extent(Obj1);
    #local _min_ext2    = min_extent(Obj2);
    #local _max_ext2    = max_extent(Obj2);

    (f_overlaps_3d(_min_ext1.x,_max_ext1.x,_min_ext1.y,_max_ext1.y,_min_ext1.z,_max_ext1.z,_min_ext2.x,_max_ext2.x,_min_ext2.y,_max_ext2.y,_min_ext2.z,_max_ext2.z))
#end

// End Overlaps(Obj1,Obj2)
//-----------------------------------------------------------------------------

// End Object testing macros
//=============================================================================

//=============================================================================
// Bounding Box Macros
//
#declare Bounding_box_null  = dictionary { .dims: 0 };

//-----------------------------------------------------------------------------
// Bounding_box_create(dims,minex,maxex)
//
#macro Bounding_box_create(dims,minex,maxex)
    dictionary {
        .dims:      dims,
        .min_ext:   minex,
        .max_ext:   maxex
    }
#end

// End Bounding_box_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Bounding_box_from_obj(Obj)
//
#macro Bounding_box_from_obj(Obj)
    Bounding_box_create(3,min_extent(Obj),max_extent(Obj))
#end

// End Bounding_box_from_obj
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Bounding_box_size(BBox)
//
#macro Bounding_box_size(BBox)
    #if (BBox.dims = 0)
        #local _sz  = 0;
    #else    
        #local _sz  = (BBox.max_ext - BBox.min_ext);
    #end                                            
    
    _sz
#end

// End Bounding_box_size
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Bounding_box_union(BBox1,BBox2)
//
#macro Bounding_box_union(BBox1,BBox2)
    #if (BBox1.dims != BBox2.dims)
        #error concat("Bounding_box_union: mismatched dimensions: ", str(BBox1.dims,0,0), " and ", str(BBox2.dims,0,0), "\n")
    #end
    #switch(BBox1.dims)
        #case(2)
            #local _min = <min(BBox1.min_ext.x,BBox2.min_ext.x),min(BBox1.min_ext.y,BBox2.min_ext.y)>;
            #local _max = <max(BBox1.max_ext.x,BBox2.max_ext.x),max(BBox1.max_ext.y,BBox2.max_ext.y)>;
        #break
        
        #case(3)
            #local _min = <min(BBox1.min_ext.x,BBox2.min_ext.x),min(BBox1.min_ext.y,BBox2.min_ext.y),min(BBox1.min_ext.z,BBox2.min_ext.z)>;
            #local _max = <max(BBox1.max_ext.x,BBox2.max_ext.x),max(BBox1.max_ext.y,BBox2.max_ext.y),max(BBox1.max_ext.z,BBox2.max_ext.z)>;
        #break
    #end
    
    Bounding_box_create(BBox1.dims,_min,_max)    
#end

// End Bounding_box_union
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Bounding_box_intersection(BBox1,BBox2)
//
#macro Bounding_box_intersection(BBox1,BBox2)
    #if (BBox1.dims != BBox2.dims)
        #error concat("Bounding_box_intersection: mismatched dimensions: ", str(BBox1.dims,0,0), " and ", str(BBox2.dims,0,0), "\n")
    #end
    
    #local _intersection    = Bounding_box_null;
    #if (BBox2.max_ext.x >= BBox1.min_ext.x & BBox2.min_ext.x <= BBox1.max_ext.x)
        #if (BBox2.max_ext.y >= BBox1.min_ext.y & BBox2.min_ext.y <= BBox1.max_ext.y)
            #if (BBox1.dims > 2)
                #if (BBox2.max_ext.z >= BBox1.min_ext.z & BBox2.min_ext.z <= BBox1.max_ext.z)
                    #local _min = <max(BBox1.min_ext.x,BBox2.min_ext.x),max(BBox1.min_ext.y,BBox2.min_ext.y),max(BBox1.min_ext.z,BBox2.min_ext.z)>;
                    #local _max = <min(BBox1.max_ext.x,BBox2.max_ext.x),min(BBox1.max_ext.y,BBox2.max_ext.y),min(BBox1.max_ext.z,BBox2.max_ext.z)>;
                    #local _intersection    = Bounding_box_create(BBox1.dims,_min,_max);
                #end
            #else
                #local _min = <max(BBox1.min_ext.x,BBox2.min_ext.x),max(BBox1.min_ext.y,BBox2.min_ext.y)>;
                #local _max = <min(BBox1.max_ext.x,BBox2.max_ext.x),min(BBox1.max_ext.y,BBox2.max_ext.y)>;
                #local _intersection    = Bounding_box_create(BBox1.dims,_min,_max);
            #end
        #end
    #end
    
    _intersection
    
#end

// End Bounding_box_intersection
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Bounding_box_subtraction(BBox1,BBox2)
//
#macro Bounding_box_subtraction(BBox1,BBox2)
    #if (BBox1.dims != BBox2.dims)
        #error concat("Bounding_box_subtraction: mismatched dimensions: ", str(BBox1.dims,0,0), " and ", str(BBox2.dims,0,0), "\n")
    #end
    
    #local _intersection    = Bounding_box_intersection(BBox1,BBox2);
    #if (_intersection.dims = 0)
        #local _subtraction = BBox1;
    #else                           
        #local _dx_min  = _intersection.min_ext.x - BBox1.min_ext.x;
        #local _dx_max  = BBox1.max_ext.x - _intersection.max_ext.x;
        #if (_dx_min >= _dx_max)
            #local _sx_min  = BBox1.min_ext.x;
            #local _sx_max  = _intersection.min_ext.x;
        #else                                         
            #local _sx_min  = _intersection.max_ext.x;
            #local _sx_max  = BBox1.max_ext.x;
        #end
        
        #local _dy_min  = _intersection.min_ext.y - BBox1.min_ext.y;
        #local _dy_max  = BBox1.max_ext.y - _intersection.max_ext.y;
        #if (_dy_min >= _dy_max)
            #local _sy_min  = BBox1.min_ext.y;
            #local _sy_max  = _intersection.min_ext.y;
        #else                                         
            #local _sy_min  = _intersection.max_ext.y;
            #local _sy_max  = BBox1.max_ext.y;
        #end
        
        #if (BBox1.dims > 2)
            #local _dz_min  = _intersection.min_ext.z - BBox1.min_ext.z;
            #local _dz_max  = BBox1.max_ext.z - _intersection.max_ext.z;
            #if (_dz_min >= _dz_max)
                #local _sz_min  = BBox1.min_ext.z;
                #local _sz_max  = _intersection.min_ext.z;
            #else                                         
                #local _sz_min  = _intersection.max_ext.z;
                #local _sz_max  = BBox1.max_ext.z;
            #end
            #local _subtraction = Bounding_box_create(BBox1.dims,<_sx_min,_sy_min,_sz_min>,<_sx_max,_sy_max,_sz_max>);
        #else
            #local _subtraction = Bounding_box_create(BBox1.dims,<_sx_min,_sy_min>,<_sx_max,_sy_max>);
        #end
    #end
    
    _subtraction
#end

// End Bounding_box_subtraction
//-----------------------------------------------------------------------------

// End Bounding Box Macros
//=============================================================================

#version Libgeom_Inc_Temp;
#end // ifndef(Libgeom_Inc_Temp)