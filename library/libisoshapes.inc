/******************************************************************************
 * File: libisoshapes.inc
 * Description:
 *      This library contains macros for defining functions useful for
 *      creating isosurface objects.
 ******************************************************************************/

#ifndef(Libisoshapes_Inc_Temp)

#declare Libisoshapes_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libisoshapes.inc\n"
#end

#include "functions.inc"                     

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
// Iso utility functions
//
#declare Iso_length_2d  = function(x,y) {
    sqrt(x*x+y*y)
}

#declare Iso_length_3d = function(x,y,z) {
    sqrt(x*x+y*y+z*z)
}

#declare Iso_dot_2d = function(x1,y1,x2,y2) {
    x1*x2+y1*y2
}

#declare Iso_ndot_2d    = function(x1,y1,x2,y2) {
    x1*x2 - y1*y2
}    

#declare Iso_dot_3d = function(x1,y1,z1,x2,y2,z2) {
    x1*x2+y1*y2+z1*z2
}

#declare Iso_dot2_2d    = function(x,y) {
    Iso_dot_2d(x,y,x,y)
}                      

#declare Iso_dot2_3d    = function(x,y,z) {
    Iso_dot_3d(x,y,z,x,y,z)
}

#declare Iso_sign   = function(v) {
    select(v, -1, 1)
}

#declare Iso_clamp  = function(v,mn,mx) {
    max(mn,min(mx,v))
}        

// End Iso utility functions
//=============================================================================

//=============================================================================
// Iso Plane Functions
//
// Exact distance functions for spheres adapted 
// from: https://iquilezles.org/articles/distfunctions/
// Exact distance functions for spheres adapted 
// from: https://iquilezles.org/articles/distfunctions/
//
//float sdPlane( vec3 p, vec3 n, float h )
//{
//  // n must be normalized
//  return dot(p,n) + h;
//}

//-----------------------------------------------------------------------------
// Iso_plane()
//
#macro Iso_plane()
    function(px,py,pz,nx,ny,nz,h) {
        Iso_dot_3d(px,py,pz,nx,ny,nz) + h
    }
#end

// End Iso_plane
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_plane(Normal,H)
//
#macro Iso_fixed_plane(Normal,H)
    #local _ifp_plane_fn    = Iso_plane()
    
    #local _nrm = vnormalize(Normal);
    #local _nx  = _nrm.x;
    #local _ny  = _nrm.y;
    #local _nz  = _nrm.z;
    
    function(x,y,z) {
        _ifp_plane_fn(x,y,z,_nx,_ny,_nz,H)
    }
#end

// End Iso_fixed_plane
//-----------------------------------------------------------------------------

// End Iso Plane Functions
//=============================================================================

//=============================================================================
// Iso Sphere Functions
//

//-----------------------------------------------------------------------------
// Iso_sphere()
//      returns function(x,y,z,r)
//
#macro Iso_sphere()
    function(x,y,z,r) {
        Iso_length_3d(x,y,z) - r
    }
#end

// End Iso_sphere
//-----------------------------------------------------------------------------
 
//-----------------------------------------------------------------------------
// Iso_fixed_sphere(Radius)
//      returns function(x,y,z)
//
#macro Iso_fixed_sphere(Radius)
    #local _ifs_sphere_fn   = Iso_sphere()
    function(x,y,z) {
        _ifs_sphere_fn(x,y,z,Radius)
    }    
#end

// End Iso_fixed_sphere
//-----------------------------------------------------------------------------

// End Iso Sphere Functions
//=============================================================================

//=============================================================================
// Iso_box()
//      returns function(x,y,z,sx,sy,sz)
//
// Iso_fixed_box(Size)
//      returns function(x,y,z)
//
// These macros generate functions that take a point <x,y,z> and compute the
// distance from that point to the surface of a box of a variable or fixed
// size.  If the point is inside the box, the distance is negative, if it is
// on the surface it returns 0, otherwise it returns a positive distance.
//
// New versions adapted from: https://iquilezles.org/articles/distfunctions/
//

//-----------------------------------------------------------------------------
// Iso_box2()
//
#macro Iso_box2()
    function(x,y,z,sx,sy,sz) {
        select(abs(x) - sx,                                        
            select(abs(y) - sy,                                     // x is in
                select(abs(z) - sz,                                 // x is in, y is in
                    max(abs(x) - sx, abs(y) - sy, abs(z) - sz),     // x is in, y is in, z is in
                    abs(z) - sz                                     // x is in, y is in, z is out
                ),
                select(abs(z) - sz,                                 // x is in, y is out
                    abs(y) - sy,                                    // x is in, y is out, z is in
                    sqrt(pow(abs(z) - sz,2) + pow(abs(y) - sy,2))   // x is in, y is out, z is out
                )
            ),
            select(abs(y) - sy,                                     // x is out
                select(abs(z) - sz,                                 // x is out, y is in
                    abs(x) - sx,                                    // x is out, y is in, z is in
                    sqrt(pow(abs(x) - sx,2) + pow(abs(z) - sz,2))   // x is out, y is in, z is out
                ),
                select(abs(z) - sz,                                 // x is out, y is out
                    sqrt(pow(abs(x) - sx,2) + pow(abs(y) - sy,2)),  // x is out, y is out, z is in
                    sqrt(pow(abs(x) - sx,2) + pow(abs(y) - sy, 2) + pow(abs(z) - sz, 2)) // x is out, y is out, z is out
                )
            )
        )            
    }
#end

// End Iso_box2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_box2(Size)
//
#macro Iso_fixed_box2(Size)
    #local _sx  = Size.x/2;
    #local _sy  = Size.y/2;
    #local _sz  = Size.z/2;
    
    #local _ifb_box_fn  = Iso_box2()
    
    function(x,y,z) {
        _ifb_box_fn(x,y,z,_sx,_sy,_sz)
    }    
        
#end

// End Iso_fixed_box2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_box()
//
#macro Iso_box()
    #local _ib_box_fn   = function(x,y,z) {
        Iso_length_3d(max(x,0),max(y,0),max(z,0)) +
        min(max(x,y,z),0)
    }
    function(x,y,z,sx,sy,sz) {
        _ib_box_fn(abs(x)-sx,abs(y)-sy,abs(z)-sz)
    }
    
#end

// End Iso_box
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_box(Size)
//
#macro Iso_fixed_box(Size)
    #local _sx  = Size.x/2;
    #local _sy  = Size.y/2;
    #local _sz  = Size.z/2;
    
    #local _ifb_box_fn  = Iso_box()
    
    function(x,y,z) {
        _ifb_box_fn(x,y,z,_sx,_sy,_sz)
    }    
        
#end

// End Iso_fixed_box
//-----------------------------------------------------------------------------

// End Iso_box(), Iso_fixed_box()
//=============================================================================

//=============================================================================
// Iso_torus()
//      returns function(x,y,z,major,minor)
//
// Iso_fixed_torus(Major,Minor)
//      returns function(x,y,z)
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//

//-----------------------------------------------------------------------------
// Iso_torus()
//
#macro Iso_torus()
    #local _it_torus_fn = function(qx,qy,minor) {
        Iso_length_2d(qx,qy) - minor
    }
    function(x,y,z,major,minor) {
        _it_torus_fn(
            Iso_length_2d(x,z) - major,
            y,
            minor
        )
    }
#end

// End Iso_torus
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_torus(Major,Minor)
//
#macro Iso_fixed_torus(Major,Minor)
    #local _ift_torus_fn    = Iso_torus()
    function(x,y,z) {
        _ift_torus_fn(x,y,z,Major,Minor)
    }    
#end

// End Iso_fixed_torus
//-----------------------------------------------------------------------------

// End Iso_torus, Iso_fixed_torus
//=============================================================================

//=============================================================================
// Iso_infinite_cylinder()
//      returns function(x,y,z,r) for the distance of a point from an
// infinite cylinder along the y axis with radius r.
//
// Iso_fixed_infinite_cylinder(Radius)
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//

//-----------------------------------------------------------------------------
// Iso_infinite_cylinder()
//
#macro Iso_infinite_cylinder()
    function(x,y,z,r) {
        Iso_length_2d(x,z) - r
    }
#end

// End Iso_infinite_cylinder
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_infinite_cylinder(Radius)
//
#macro Iso_fixed_infinite_cylinder(Radius)
    #local _ific_cylinder_fn    = Iso_infinite_cylinder()
    function(x,y,z) {
        _ific_cylinder_fn(x,y,z,Radius)
    }    
#end

// End Iso_fixed_infinite_cylinder
//-----------------------------------------------------------------------------

// End Iso_infinite_cylinder, Iso_fixed_infinite_cylinder
//=============================================================================

//=============================================================================
// Iso_capped_cylinder()
//      returns function(x,y,z,r,h) for the distance of a point from a
// capped cylinder along the y axis with radius r and height 2*h centered
// at the origin.
//
// Iso_fixed_capped_cylinder(Radius,Height)
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//

//-----------------------------------------------------------------------------
// Iso_capped_cylinder()
//
#macro Iso_capped_cylinder()
    #local _icc_cyl_fn  = function(dx,dy) {
        min(max(dx,dy),0) +
        Iso_length_2d(max(dx,0),max(dy,0))
    }
    function(x,y,z,r,h) {
        _icc_cyl_fn(
            Iso_length_2d(x,z) - r,
            abs(y) - h
        )    
    }
    
#end

// End Iso_capped_cylinder
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_capped_cylinder(Radius,Height)
//
#macro Iso_fixed_capped_cylinder(Radius,Height)
    #local _ifcc_cyl_fn = Iso_capped_cylinder()
    #local _sh          = Height/2;
    function(x,y,z) {
        _ifcc_cyl_fn(x,y,z,Radius,_sh)
    }
#end

// End Iso_fixed_capped_cylinder
//-----------------------------------------------------------------------------

// End Iso_capped_cylinder, Iso_fixed_capped_cylinder
//=============================================================================

//=============================================================================
// Iso_rounded_cylinder()
//      returns function(x,y,z,rr,rh,re) for the difference from a capped
// cylinder along the y axis with radius rr, edge radius re, and height 2*h 
// centered at the origin
//
// Iso_fixed_rounded_cylinder(Radius,Height,Rnd)
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//

//-----------------------------------------------------------------------------
// Iso_rounded_cylinder()
//
#macro Iso_rounded_cylinder()
    #local _irc_cyl_fn  = function(dx,dy,re) {
        min(max(dx,dy),0) +
        Iso_length_2d(max(dx,0),max(dy,0)) - re
    }
    function(x,y,z,rr,h,re) {
        _irc_cyl_fn(
            Iso_length_2d(x,z) - rr+re,
            abs(y) - h,
            re
        )
    }
    
#end

// End Iso_rounded_cylinder
//-----------------------------------------------------------------------------
 
//-----------------------------------------------------------------------------
// Iso_fixed_rounded_cylinder(Radius,Height,Rnd)
//
#macro Iso_fixed_rounded_cylinder(Radius,Height,Rnd)
    #local _ifrc_cyl_fn = Iso_rounded_cylinder()
    #local _h           = Height/2;
    function(x,y,z) {
        _ifrc_cyl_fn(x,y,z,Radius,_h-Rnd,Rnd)
    }
#end

// End Iso_fixed_rounded_cylinder
//-----------------------------------------------------------------------------

// End Iso_rounded_cylinder, Iso_fixed_rounded_cylinder
//=============================================================================

//=============================================================================
// Iso_cylinder()
//      returns function(x,y,z,r,sy)
//
// Iso_fixed_cylinder(Radius,Height)
//
// Cylinder shape functions for Y axis cylinders

//-----------------------------------------------------------------------------
// Iso_cylinder()
//
#macro Iso_cylinder()
    #local _ic_aux_fn   = function(x,y,z,r,sy,rd) {
        select(rd,
            max(rd, abs(y) - sy),
            select(abs(y) - sy,
                rd,
                sqrt(pow(rd,2) + pow(abs(y) - sy,2))
            )
        )        
    }
    
    function(x,y,z,r,sy) {
        _ic_aux_fn(x,y,z,r,sy,sqrt(x*x+z*z) - r)
    }
#end

// End Iso_cylinder
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_cylinder(Radius,Height)
//
#macro Iso_fixed_cylinder(Radius,Height)
    #local _sy  = Height/2;
    #local _ifc_cylinder_fn = Iso_cylinder()
    
    function(x,y,z) {
        _ifc_cylinder_fn(x,y,z,Radius,_sy)
    }
#end

// End Iso_fixed_cylinder
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_rounded_cylinder2()
//
// Cylinder shape functions for Y axis cylinders with rounded top and bottom
// edges
//
#macro Iso_rounded_cylinder2()

    #local _cyl_fn  = Iso_cylinder()

    function(x,y,z,r,sy,rnd) {
        min(
            _cyl_fn(x,y,z,r-rnd,sy),
            _cyl_fn(x,y,z,r,sy-rnd),
            f_torus(x,abs(y)-(sy-rnd),z,r-rnd,rnd)
        )
    }
#end

// End Iso_rounded_cylinder
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_rounded_cylinder(Radius,Height,Rnd)
//
#macro Iso_fixed_rounded_cylinder2(Radius,Height,Rnd)
    #local _sy  = Height/2;
    #local _ifrc_cylinder_fn    = Iso_rounded_cylinder2()
    
    function(x,y,z) {
        _ifrc_cylinder_fn(x,y,z,Radius,_sy,Rnd)
    }    
#end

// End Iso_fixed_rounded_cylinder
//-----------------------------------------------------------------------------

// End Iso_cylinder2, Iso_fixed_cylinder2
//=============================================================================

//=============================================================================
// Iso_rounded_box()
//      returns function(x,y,z,rnd,sx,sy,sz)
//
// Iso_fixed_rounded_box(Size,Rnd)
//      returns function(x,y,z)
//

//-----------------------------------------------------------------------------
// Iso_rounded_box2()
//
#macro Iso_rounded_box2()
    #local _irb_box_fn  = Iso_box()
    #local _irb_ycyl_fn = Iso_cylinder()
    #local _irb_xcyl_fn = function(x,y,z,r,sx) {
        _irb_ycyl_fn(y,x,z,r,sx)
    }
    #local _irb_zcyl_fn = function(x,y,z,r,sz) {
        _irb_ycyl_fn(x,z,y,r,sz)
    }
    
    function(x,y,z,rnd,sx,sy,sz) {
        min(
            _irb_box_fn(x,y,z,sx,sy-rnd,sz-rnd),
            _irb_box_fn(x,y,z-(sz-rnd/2),sx-rnd,sy-rnd,rnd/2),
            _irb_box_fn(x,y,z+(sz-rnd/2),sx-rnd,sy-rnd,rnd/2),
            _irb_box_fn(x,y-(sy-rnd/2),z,sx-rnd,rnd/2,sz-rnd),
            _irb_box_fn(x,y+(sy-rnd/2),z,sx-rnd,rnd/2,sz-rnd),
            _irb_ycyl_fn(x-(sx-rnd),y,z-(sz-rnd),rnd,sy-rnd),
            _irb_ycyl_fn(x+(sx-rnd),y,z-(sz-rnd),rnd,sy-rnd),
            _irb_ycyl_fn(x-(sx-rnd),y,z+(sz-rnd),rnd,sy-rnd),
            _irb_ycyl_fn(x+(sx-rnd),y,z+(sz-rnd),rnd,sy-rnd),
            _irb_xcyl_fn(x,y-(sy-rnd),z-(sz-rnd),rnd,sx-rnd),
            _irb_xcyl_fn(x,y+(sy-rnd),z-(sz-rnd),rnd,sx-rnd),
            _irb_xcyl_fn(x,y-(sy-rnd),z+(sz-rnd),rnd,sx-rnd),
            _irb_xcyl_fn(x,y+(sy-rnd),z+(sz-rnd),rnd,sx-rnd),
            _irb_zcyl_fn(x-(sx-rnd),y-(sy-rnd),z,rnd,sz-rnd),
            _irb_zcyl_fn(x+(sx-rnd),y-(sy-rnd),z,rnd,sz-rnd),
            _irb_zcyl_fn(x-(sx-rnd),y+(sy-rnd),z,rnd,sz-rnd),
            _irb_zcyl_fn(x+(sx-rnd),y+(sy-rnd),z,rnd,sz-rnd),
            f_sphere(x-(sx-rnd),y-(sy-rnd),z-(sz-rnd),rnd),
            f_sphere(x+(sx-rnd),y-(sy-rnd),z-(sz-rnd),rnd),
            f_sphere(x-(sx-rnd),y+(sy-rnd),z-(sz-rnd),rnd),
            f_sphere(x+(sx-rnd),y+(sy-rnd),z-(sz-rnd),rnd),
            f_sphere(x-(sx-rnd),y-(sy-rnd),z+(sz-rnd),rnd),
            f_sphere(x+(sx-rnd),y-(sy-rnd),z+(sz-rnd),rnd),
            f_sphere(x-(sx-rnd),y+(sy-rnd),z+(sz-rnd),rnd),
            f_sphere(x+(sx-rnd),y+(sy-rnd),z+(sz-rnd),rnd)
        )
    }
#end

// End Iso_rounded_box2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_rounded_box2(Size,Rnd)
//
#macro Iso_fixed_rounded_box2(Size,Rnd)
    #local _ifrb_rb_fn  = Iso_rounded_box2()
    #local _sx          = Size.x/2;
    #local _sy          = Size.y/2;
    #local _sz          = Size.z/2;
    
    function(x,y,z) {
        _ifrb_rb_fn(x,y,z,Rnd,_sx,_sy,_sz)
    }    
#end

// End Iso_fixed_rounded_box
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_rounded_box()
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
#macro Iso_rounded_box()
    #local _ib_length_fn    = function(x,y,z) {
        sqrt(x*x+y*y+z*z)
    }
    #local _ib_rbox_fn   = function(x,y,z,r) {
        _ib_length_fn(max(x,0),max(y,0),max(z,0)) +
        min(max(x,y,z),0) - r
    }
    function(x,y,z,r,sx,sy,sz) {
        _ib_rbox_fn(abs(x)-sx+r,abs(y)-sy+r,abs(z)-sz+r,r)
    }
    
#end

// End Iso_rounded_box
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_rounded_box(Size,Rnd)
//
#macro Iso_fixed_rounded_box(Size,Rnd)
    #local _ifrb_rb_fn  = Iso_rounded_box()
    #local _sx          = Size.x/2;
    #local _sy          = Size.y/2;
    #local _sz          = Size.z/2;
    
    function(x,y,z) {
        _ifrb_rb_fn(x,y,z,Rnd,_sx,_sy,_sz)
    }    
#end

// End Iso_fixed_rounded_box
//-----------------------------------------------------------------------------

// End Iso_rounded_Box, Iso_fixed_rounded_Box
//=============================================================================

//=============================================================================
// Egg Shapes
//

//-----------------------------------------------------------------------------
// Iso_egg()
//
// Note: The bounds for this object are:
//  <-1.055, -1, -1.055>, <1.055, 1, 1.055>
//  The max bulge is at y=0.7
//
#declare Iso_egg_lbounds    = <-1.055, -1, -1.055>;
#declare Iso_egg_ubounds    = <1.055, 1, 1.055>;
#macro Iso_egg()
    #local _egg_fn  = function(x,y) {
        (x*x + pow(pow(1.4,x)*y,2)) - 1
    }
    
    function(x,y,z) {
        _egg_fn(y, sqrt(x*x+z*z))
    }
#end

// End Iso_egg
//-----------------------------------------------------------------------------


// End Egg Shapes
//=============================================================================

//=============================================================================
// Iso_cone()
//      returns function(x,y,z,h,r) that determines the distance of a point
// from the edge of the cone along the Y axis whose base has a radius of r
// and has height h, with the point at the origin
//
// Iso_fixed_cone(Radius,Height)
//
// Adapted from: https://iquilezles.org/articles/distfunctions/

//-----------------------------------------------------------------------------
// Iso_cone()
//
#macro Iso_cone()
    #local _ic_cone_fn  = function(d,s) {
        sqrt(d)*Iso_sign(s)
    }
    #local _ic_d_fn = function(ax,ay,bx,by) {
        min(Iso_dot2_2d(ax,ay), Iso_dot2_2d(bx,by))
    }
    #local _ic_s_fn = function(k,wx,wy,r,h) {
        max(
            k*(wx*-h-wy*r),
            k*(wy+h)
        )    
    }
    #local _ic_wqr_fn   = function(wx,wy,h,r) {
        Iso_clamp(
            Iso_dot_2d(wx,wy,r,h)/Iso_dot2_2d(r,h),
            0.0,
            1.0
        )    
    }
    #local _ic_aux2_fn  = function(x,y,z,h,r,wx,wy,wqr) {
        _ic_cone_fn(
            _ic_d_fn(
                wx - r*wqr,
                wy - h*wqr,
                wx - r*Iso_clamp(wx/r,0.0,1.0),
                wy + h
            ),
            _ic_s_fn(
                Iso_sign(-h),
                wx,
                wy,
                r,
                h
            )
        )    
    }
    #local _ic_aux1_fn  = function(x,y,z,h,r,wx,wy) {
        _ic_aux2_fn(x,y,z,h,r,wx,wy,_ic_wqr_fn(wx,wy,h,r))
    }
    
    function(x,y,z,h,r) {
        _ic_aux1_fn(x,y,z,h,r,
            Iso_length_2d(x,z),
            y
        )    
    }
#end

// End Iso_cone
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_cone(Height,Radius)
//
#macro Iso_fixed_cone(Height,Radius)
    #local _ifc_cone_fn = Iso_cone()
    function(x,y,z) {
        _ifc_cone_fn(x,y,z,Height,Radius)
    }    
#end

// End Iso_fixed_cone
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_cone_inexact()
//
#macro Iso_cone_inexact()
    #local _ic_aux_fn1  = function(x,y,z,h,r,a) {
        max(
            Iso_dot_2d(cos(a),sin(a),Iso_length_2d(x,z),y),
            -h-y
        )    
    }
    function(x,y,z,h,r) {
        _ic_aux_fn1(x,y,z,h,r,atan2(r,h))
    }
#end

// End Iso_cone_inexact
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_cone_inexact(Height,Radius)
//
#macro Iso_fixed_cone_inexact(Height,Radius)
    #local _ifc_cone_fn = Iso_cone_inexact()
    function(x,y,z) {
        _ifc_cone_fn(x,y,z,Height,Radius)
    }    
#end

// End Iso_fixed_cone_inexact
//-----------------------------------------------------------------------------

// End Iso_cone, Iso_fixed_cone
//=============================================================================

//=============================================================================
// Iso_capped_cone()
//      returns function(x,y,z,h,r1,r2) that finds the distance from the surface
// of a truncated cone with the given top and bottom radii and height on the
// Y axis.
//
// Iso_fixed_capped_cone(Height,Radius1,Radius2)
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//
//float sdCappedCone( vec3 p, float h, float r1, float r2 )
//{
//  vec2 q = vec2( length(p.xz), p.y );
//  vec2 k1 = vec2(r2,h);
//  vec2 k2 = vec2(r2-r1,2.0*h);
//  vec2 ca = vec2(q.x-min(q.x,(q.y<0.0)?r1:r2), abs(q.y)-h);
//  vec2 cb = q - k1 + k2*clamp( dot(k1-q,k2)/dot2(k2), 0.0, 1.0 );
//  float s = (cb.x<0.0 && ca.y<0.0) ? -1.0 : 1.0;
//  return s*sqrt( min(dot2(ca),dot2(cb)) );
//}

//-----------------------------------------------------------------------------
// Iso_capped_cone()
//
#macro Iso_capped_cone()
    #local _icc_cone_fn = function(s,cax,cay,cbx,cby) {
        s*sqrt(min(Iso_dot2_2d(cax,cay),Iso_dot2_2d(cbx,cby)))
    }
    
    #local _icc_s_fn    = function(cax,cay,cbx,cby) {
        select(cbx,
            select(cay,
                -1.0,
                1.0
            ),
            1.0
        )        
    }
    
    #local _icc_aux3_fn = function(cax,cay,cbx,cby) {
        _icc_cone_fn(
            _icc_s_fn(cax,cay,cbx,cby),
            cax,
            cay,
            cbx,
            cby
        )
    }
    
    #local _icc_kdq_fn  = function(y,lxz,k1x,k1y,k2x,k2y) {
        Iso_clamp(
            Iso_dot_2d(k1x-lxz,k1y-y,k2x,k2y)/Iso_dot2_2d(k2x,k2y),
            0.0,
            1.0
        )    
    }
    
    #local _icc_aux2_fn = function(x,y,z,k1x,k1y,k2x,k2y,lxz,kdq,h,r1,r2) {
        _icc_aux3_fn(
            lxz - min(lxz,select(y,r1,r2)),
            abs(y) - h,
            lxz - k1x + k2x*kdq,
            y - k1y + k2y*kdq
        )    
    }        
    
    #local _icc_aux1_fn = function(x,y,z,k1x,k1y,k2x,k2y,lxz,h,r1,r2) {
        _icc_aux2_fn(x,y,z,k1x,k1y,k2x,k2y,lxz,_icc_kdq_fn(y,lxz,k1x,k1y,k2x,k2y),h,r1,r2)
    }
    
    function(x,y,z,h,r1,r2) {
        _icc_aux1_fn(x,y,z,r2,h,r2-r1,2*h,Iso_length_2d(x,z),h,r1,r2)
    }
#end

// End Iso_capped_cone
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_capped_cone(Height,Radius1,Radius2)
//
#macro Iso_fixed_capped_cone(Height,Radius1,Radius2)
    #local _ifcc_cone_fn    = Iso_capped_cone()
    
    function(x,y,z) {
        _ifcc_cone_fn(x,y,z,Height,Radius1,Radius2)
    }
#end

// End Iso_fixed_capped_cone
//-----------------------------------------------------------------------------

// End Iso_capped_cone, Iso_fixed_capped_cone
//=============================================================================

//=============================================================================
// Iso_rounded_cone()
//      returns function(x,y,z,h,r1,r2) that measures the distance from the surface
// of a rounded cone whose height is h, radius is r1, and rounding radius is r2
// along the Y axis.
//
// Iso_fixed_rounded_cone(Height,Radius1,Radius2)
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//float sdRoundCone( vec3 p, float r1, float r2, float h )
//{
//  // sampling independent computations (only depend on shape)
//  float b = (r1-r2)/h;
//  float a = sqrt(1.0-b*b);

//  // sampling dependant computations
//  vec2 q = vec2( length(p.xz), p.y );
//  float k = dot(q,vec2(-b,a));
//  if( k<0.0 ) return length(q) - r1;
//  if( k>a*h ) return length(q-vec2(0.0,h)) - r2;
//  return dot(q, vec2(a,b) ) - r1;
//}
//

//-----------------------------------------------------------------------------
// Iso_rounded_cone()
//
#macro Iso_rounded_cone()
    #local _irc_cone_fn = function(k,lxz,y,a,b,h,r1,r2) {
        select(k,
            Iso_length_2d(lxz,y) - r1,
            select(a*h-k,
                Iso_length_2d(lxz,y-h) - r2,
                Iso_dot_2d(lxz,y,a,b) - r1
            )
        )        
    }
    
    #local _irc_aux2_fn = function(x,y,z,lxz,a,b,h,r1,r2) {
        _irc_cone_fn(
            Iso_dot_2d(lxz,y,-b,a),
            lxz,
            y,
            a,
            b,
            h,
            r1,
            r2
        )    
    }
    
    #local _irc_aux1_fn  = function(x,y,z,lxz,b,h,r1,r2) {
        _irc_aux2_fn(x,y,z,lxz,sqrt(1.0-b*b),b,h,r1,r2)
    }
    
    function(x,y,z,h,r1,r2) {
        _irc_aux1_fn(x,y,z,Iso_length_2d(x,z),(r1-r2)/h,h,r1,r2)
    }
#end

// End Iso_rounded_cone
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_rounded_cone(Height,Radius1,Radius2)
//
#macro Iso_fixed_rounded_cone(Height,Radius1,Radius2)
    #local _ifrc_cone_fn    = Iso_rounded_cone()
    function(x,y,z) {
        _ifrc_cone_fn(x,y,z,Height,Radius1,Radius2)
    }    
#end

// End Iso_fixed_rounded_cone
//-----------------------------------------------------------------------------

// End Iso_rounded_cone, Iso_fixed_rounded_cone
//=============================================================================

//=============================================================================
// Iso_ellipsoid()
//      returns function(x,y,z,rx,ry,rz) that measures the distance from the
// surface of an ellipsoid with the given 3 radii centered at the origin.
//
// Iso_fixed_ellipsoid(RX,RY,RZ)
//      returns function(x,y,z) that measures the distance from the surface
// of an ellipsoid with fixed radii RX,RY,RZ centered at the origin.
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//float sdEllipsoid( vec3 p, vec3 r )
//{
//  float k0 = length(p/r);
//  float k1 = length(p/(r*r));
//  return k0*(k0-1.0)/k1;
//}

//-----------------------------------------------------------------------------
// Iso_ellipsoid()
//
#macro Iso_ellipsoid()
    #local _ie_ellipsoid_fn = function(k0,k1) {
        k0*(k0-1.0)/k1
    }
    
    function(x,y,z,rx,ry,rz) {
        _ie_ellipsoid_fn(
            Iso_length_3d(x/rx,y/ry,z/rz),
            Iso_length_3d(x/(rx*rx),y/(ry*ry),z/(rz*rz))
        )
    }
#end

// End Iso_ellipsoid
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_ellipsoid(RX,RY,RZ)
//
#macro Iso_fixed_ellipsoid(RX,RY,RZ)
    #local _ie_ellipsoid_fn = function(k0,k1) {
        k0*(k0-1.0)/k1
    }
    
    #local _rx2 = RX*RX;
    #local _ry2 = RY*RY;
    #local _rz2 = RZ*RZ;
    
    function(x,y,z) {
        _ie_ellipsoid_fn(
            Iso_length_3d(x/RX,y/RY,z/RZ),
            Iso_length_3d(x/_rx2,y/_ry2,z/_rz2)
        )
    }
#end

// End Iso_ellipsoid
//-----------------------------------------------------------------------------

// End Iso_ellipsoid, Iso_fixed_ellipsoid
//=============================================================================

//=============================================================================
// Iso_rhombus()
//      returns function(x,y,z,la,lb,h,ra) that measures the distance from the
// surface of a rhombus with rounded corners centered at the origin with the
// x-axis scale l1, z-axis scale lb, y scale h, and corner rounding radius ra.
//
// Note: The resulting shape extends the la/lb dimensions by ra
//
// Iso_fixed_rhombus(LA,LB,H,RA)
//      returns function(x,y,z) that measures the distance from the surface of
// a rhombus with rounded corners centered at the origin wiht the given fixed
// scale values.
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//float sdRhombus( vec3 p, float la, float lb, float h, float ra )
//{
//  p = abs(p);
//  vec2 b = vec2(la,lb);
//  float f = clamp( (ndot(b,b-2.0*p.xz))/dot(b,b), -1.0, 1.0 );
//  vec2 q = vec2(length(p.xz-0.5*b*vec2(1.0-f,1.0+f))*sign(p.x*b.y+p.z*b.x-b.x*b.y)-ra, p.y-h);
//  return min(max(q.x,q.y),0.0) + length(max(q,0.0));
//}

//-----------------------------------------------------------------------------
// Iso_rhombus()
//
#macro Iso_rhombus()
    #local _ir_rhombus_fn   = function(qx,qy) {
        min(max(qx,qy),0.0) + Iso_length_2d(max(qx,0),max(qy,0))
    }
    
    #local _ir_f_fn = function(x,y,z,la,lb) {
        Iso_clamp(
            Iso_ndot_2d(la,lb,la-2*x,lb-2*z)/Iso_dot2_2d(la,lb),
            -1.0,
            1.0
        )    
    }
    
    #local _ir_aux2_fn   = function(x,y,z,la,lb,h,ra,f) {
        _ir_rhombus_fn(
            Iso_length_2d(
                x-0.5*la*(1 - f),
                z-0.5*lb*(1 + f)
            ) * Iso_sign(x*lb+z*la-la*lb) - ra,
            y-h
        )
    }
    
    #local _ir_aux1_fn  = function(x,y,z,la,lb,h,ra) {
        _ir_aux2_fn(x,y,z,la,lb,h,ra,_ir_f_fn(x,y,z,la,lb))
    }    
    
    function(x,y,z,la,lb,h,ra) {
        _ir_aux1_fn(abs(x),abs(y),abs(z),la,lb,h,ra)
    }
#end

// End Iso_rhombus
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_rhombus(LA,LB,H,RA)
//
// Note: This version fixes the shape to account for RA such that the resulting
// rhombus fits within +/-<LA,H,LB> by subtracting RA from LA and LB 
#macro Iso_fixed_rhombus(LA,LB,H,RA)
    #local _ifr_rhombus_fn  = Iso_rhombus()
    #local _la  = LA - RA;
    #local _lb  = LB - RA;
    function(x,y,z) {
        _ifr_rhombus_fn(x,y,z,_la,_lb,H,RA)
    }
#end

// End Iso_fixed_rhombus
//-----------------------------------------------------------------------------

// End Iso_rhombus
//=============================================================================

//=============================================================================
// Iso_capsule()
//      Returns function(x,y,z,h,r) that computes the distance from the surface
// of a cylinder whose ends are capped with spheres, with a length of h
// (not counting the caps) and radius r, on the Y axis.  The bottom cap
// lies at y=0, and the top cap lies at y=h
//
// Iso_fixed_capsule(H,R)
//      Returns function(x,y,z) that computes the distance from the surface of
// a cylinder whose ends are capped with spheres, with a fixed length of H
// (counting the caps) and fixed radius R.  This function centers the
// capsule at y=0
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//float sdVerticalCapsule( vec3 p, float h, float r )
//{
//  p.y -= clamp( p.y, 0.0, h );
//  return length( p ) - r;
//}

//-----------------------------------------------------------------------------
// Iso_capsule()
//
#macro Iso_capsule()
    #local _ic_capsule_fn   = function(x,y,z,r) {
        Iso_length_3d(x,y,z) - r
    }
    
    function(x,y,z,h,r) {
        _ic_capsule_fn(x, y - Iso_clamp(y, 0, h), z, r)
    }
#end

// End Iso_capsule
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_capsule(H,R)
//
#macro Iso_fixed_capsule(H,R)
    #local _h   = H - 2*R;
    #local _ifc_capsule_fn  = Iso_capsule()
    function(x,y,z) {
        _ifc_capsule_fn(x,y+_h/2,z,_h,R)
    }
#end

// End Iso_fixed_capsule
//-----------------------------------------------------------------------------

// End Iso_capsule,Iso_fixed_capsule
//=============================================================================

//=============================================================================
// Iso_octahedron()
//      Returns function(x,y,z,s) that computes the distance from the surface
// of a regular octahedron with a "radius" of size s, centered at the origin.
//
// Iso_fixed_octahedron(S)
//      Returns function(x,y,z) that computes the distance from the surface
// of a regular octahedron with a fixed "radius" of size S, centered at the
// origin.
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//float sdOctahedron( vec3 p, float s )
//{
//  p = abs(p);
//  float m = p.x+p.y+p.z-s;
//  vec3 q;
//       if( 3.0*p.x < m ) q = p.xyz;
//  else if( 3.0*p.y < m ) q = p.yzx;
//  else if( 3.0*p.z < m ) q = p.zxy;
//  else return m*0.57735027;
//    
//  float k = clamp(0.5*(q.z-q.y+s),0.0,s); 
//  return length(vec3(q.x,q.y-s+k,q.z-k)); 
//}

//-----------------------------------------------------------------------------
// Iso_octahedron()
//
#macro Iso_octahedron()
    #local _io_octahedron_fn    = function(qx,qy,qz,s,k) {
        Iso_length_3d(qx,qy-s+k,qz-k)
    }
    
    #local _io_k_fn = function(qz,qy,s) {
        Iso_clamp(0.5*(qz-qy+s), 0.0, s)
    }
    
    #local _io_m_fn = function(x,y,z,s) {
        x+y+z-s
    }
    
    #local _io_aux3_fn  = function(qx,qy,qz,s) {
        _io_octahedron_fn(qx,qy,qz,s,_io_k_fn(qz,qy,s))
    }
    
    #local _io_aux2_fn  = function(x,y,z,s,m) {
        select(3*x-m,
            _io_aux3_fn(x,y,z,s),
            select(3*y-m,
                _io_aux3_fn(y,z,x,s),
                select(3*z-m,
                    _io_aux3_fn(z,x,y,s),
                    m*0.57735027
                )
            )
        )            
    }    
    
    #local _io_aux1_fn  = function(x,y,z,s) {
        _io_aux2_fn(x,y,z,s,_io_m_fn(x,y,z,s))
    }
    
    function(x,y,z,s) {
        _io_aux1_fn(abs(x),abs(y),abs(z),s)
    }
#end

// End Iso_octahedron
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_octahedron(S)
//
#macro Iso_fixed_octahedron(S)
    #local _ifo_octahedron_fn   = Iso_octahedron()
    function(x,y,z) {
        _ifo_octahedron_fn(x,y,z,S)
    }    
#end

// End Iso_fixed_octahedron
//-----------------------------------------------------------------------------

// End Iso_octahedron,Iso_fixed_octahedron
//=============================================================================

//=============================================================================
// Iso_pyramid()
//      Returns function(x,y,z,h) that computes the distance from the surface
// of a regular pyramid of height h.
//
// Iso_fixed_pyramid(H)
//      Returns function(x,y,z) that computes the distance from the surface
// of a regular pyramid of fixed height H.
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//float sdPyramid( vec3 p, float h )
//{
//  float m2 = h*h + 0.25;
//    
//  p.xz = abs(p.xz);
//  p.xz = (p.z>p.x) ? p.zx : p.xz;
//  p.xz -= 0.5;
//
//  vec3 q = vec3( p.z, h*p.y - 0.5*p.x, h*p.x + 0.5*p.y);
//   
//  float s = max(-q.x,0.0);
//  float t = clamp( (q.y-0.5*p.z)/(m2+0.25), 0.0, 1.0 );
//    
//  float a = m2*(q.x+s)*(q.x+s) + q.y*q.y;
//  float b = m2*(q.x+0.5*t)*(q.x+0.5*t) + (q.y-m2*t)*(q.y-m2*t);
//    
//  float d2 = min(q.y,-q.x*m2-q.y*0.5) > 0.0 ? 0.0 : min(a,b);
//    
//  return sqrt( (d2+q.z*q.z)/m2 ) * sign(max(q.z,-p.y));
//}

//-----------------------------------------------------------------------------
// Iso_pyramid()
//
#macro Iso_pyramid()
    #local _ip_pyramid_fn   = function(y,qz,d2,m2) {
        sqrt((d2+qz*qz)/m2) * Iso_sign(max(qz,-y))
    }
    
    #local _ip_d2_fn    = function(qx,qy,m2,a,b) {
        select(min(qy, -qx*m2-qy*0.5),
            min(a,b),
            min(a,b),
            0.0
        )    
    }
    
    #local _ip_a_fn     = function(qx,qy,m2,s) {
        m2*(qx+s)*(qx+s) + qy*qy
    }
    
    #local _ip_b_fn     = function(qx,qy,m2,tv) {
        m2*(qx+0.5*tv)*(qx+0.5*tv) + (qy-m2*tv)*(qy-m2*tv)
    }
    
    #local _ip_t_fn     = function(qy,z,m2) {
        Iso_clamp((qy-0.5*z)/(m2+0.25), 0.0, 1.0)
    }
    
    #local _ip_aux4_fn  = function(y,m2,qx,qy,qz,s,tv) {
        _ip_pyramid_fn(y,qz,_ip_d2_fn(qx,qy,m2,_ip_a_fn(qx,qy,m2,s),_ip_b_fn(qx,qy,m2,tv)),m2)
    }
    
    #local _ip_aux3_fn  = function(x,y,z,m2,qx,qy,qz) {
        _ip_aux4_fn(y,m2,qx,qy,qz,max(-qx,0),_ip_t_fn(qy,z,m2))
    }
    
    #local _ip_aux2_fn  = function(x,y,z,h,m2) {
        _ip_aux3_fn(x,y,z,m2,
            z,
            h*y - 0.5*x,
            h*x + 0.5*y
        )    
    }
    
    #local _ip_aux1_fn  = function(x,y,z,h,m2) {
        select(x-z,
            _ip_aux2_fn(z-0.5,y,x-0.5,h,m2),
            _ip_aux2_fn(x-0.5,y,z-0.5,h,m2)
        )    
    }
    
    function(x,y,z,h) {
        _ip_aux1_fn(abs(x),y,abs(z),h,h*h+0.25)
    }
#end

// End Iso_pyramid
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_pyramid(H)
//
#macro Iso_fixed_pyramid(H)
    #local _ifp_pyramid_fn  = Iso_pyramid()
    function(x,y,z) {
        _ifp_pyramid_fn(x,y,z,H)
    }    
#end

// End Iso_fixed_pyramid
//-----------------------------------------------------------------------------

// End Iso_pyramid, Iso_fixed_pyramid
//=============================================================================

//=============================================================================
// Iso_hexagonal_prism()
//      Returns function(x,y,z,r,th) that measures the distance from the surface
// of a hexagonal prism centered at the origin with depth (th) along the Z axis
// and distance from center to perpendicular edge r
//
// Iso_fixed_hexagonal_prism(R,TH)
//      Returns function(x,y,z) that measures the distance from the surface of
// a hexagonal prism centered at the origin with fixed radius (R) and depth (TH).
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//float sdHexPrism( vec3 p, vec2 h )
//{
//  const vec3 k = vec3(-0.8660254, 0.5, 0.57735);
//  p = abs(p);
//  p.xy -= 2.0*min(dot(k.xy, p.xy), 0.0)*k.xy;
//  vec2 d = vec2(
//       length(p.xy-vec2(clamp(p.x,-k.z*h.x,k.z*h.x), h.x))*sign(p.y-h.x),
//       p.z-h.y );
//  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
//} 

//-----------------------------------------------------------------------------
// Iso_hexagonal_prism()
//
#macro Iso_hexagonal_prism()
    #local _kx  = -0.8660254;
    #local _ky  = 0.5;
    #local _kz  = 0.57735;
    
    #local _ihp_hexp_fn = function(dx,dy) {
        min(max(dx,dy),0.0) + Iso_length_2d(max(dx,0),max(dy,0))
    }
    
    #local _ihp_aux3_fn = function(x,y,z,r,th) {
        _ihp_hexp_fn(
            Iso_length_2d(
                x-Iso_clamp(x, -_kz*r, _kz*r),
                y-r
            ) * Iso_sign(y-r),
            z-th
        )
    }
    
    #local _ihp_aux2_fn = function(x,y,z,r,th,dkp) {
        _ihp_aux3_fn(x - 2*dkp*_kx, y - 2*dkp*_ky,z,r,th)
    }
    
    #local _ihp_aux1_fn = function(x,y,z,r,th) {
        _ihp_aux2_fn(x,y,z,r,th,min(Iso_dot_2d(_kx,_ky,x,y),0.0))
    }
    
    function(x,y,z,r,th) {
        _ihp_aux1_fn(abs(x),abs(y),abs(z),r,th)
    }
#end 

// End Iso_hexagonal_prism
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_hexagonal_prism(R,TH)
//
#macro Iso_fixed_hexagonal_prism(R,TH)
    #local _ifhp_prism_fn   = Iso_hexagonal_prism()
    function(x,y,z) {
        _ifhp_prism_fn(x,y,z,R,TH)
    }    
#end

// End Iso_fixed_hexagonal_prism
//-----------------------------------------------------------------------------


// End Iso_hexagonal_prism, Iso_fixed_hexagonal_prism
//=============================================================================

//=============================================================================
// Iso_triangular_prism()
//      Returns function(x,y,z,h,l) that measures the distance from the surface
// of a triangular prism of height h and length l lying along the Z axis.
//
// Iso_fixed_triangular_prism(H,L)
//      Returns function(x,y,z) that measures the distance from the surface
// of a triangular prism of fixed height H and fixed length L lying along the
// Z axis.
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//float sdTriPrism( vec3 p, vec2 h )
//{
//  vec3 q = abs(p);
//  return max(q.z-h.y,max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5);
//}

//-----------------------------------------------------------------------------
// Iso_triangular_prism()
//
#macro Iso_triangular_prism()
    #local _itp_prism_fn    = function(qx,qy,qz,h,l) {
        max(
            qz-l,
            max(qx*0.866025+qy*0.5, -qy)-0.5*h
        )    
    }
    
    function(x,y,z,h,l) {
        _itp_prism_fn(abs(x),abs(y),abs(z),h,l)
    }
#end

// End Iso_triangular_prism
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_triangular_prism(H,L)
//
#macro Iso_fixed_triangular_prism(H,L)
    #local _itp_prism_fn    = function(qx,qy,qz) {
        max(
            qz-L,
            max(qx*0.866025+qy*0.5, -qy)-0.5*H
        )    
    }
    
    function(x,y,z) {
        _itp_prism_fn(abs(x),y,abs(z))
    }
#end

// End Iso_fixed_triangular_prism
//-----------------------------------------------------------------------------


// End Iso_triangular_prism, Iso_fixed_triangular_prism
//=============================================================================

//=============================================================================
// Helix Functions
//

//-----------------------------------------------------------------------------
// Iso_helix1(Dir)
//
#declare Iso_helix_dir_clockwise        = 0;
#declare Iso_helix_dir_counterclockwise = 1;
#macro Iso_helix1(Dir)
    #local _ih_circle_fn    = function(qx,qy,r) {
        Iso_length_2d(qx,qy) - r
    }
    
    #local _ih_qy_fn    = function(y,rise) {
        y - 
        (
            select(y,
                int(y/rise - 0.5)*rise,
                int(y/rise + 0.5)*rise
            )
        )        
    }
    
    #switch(Dir)
        #case(Iso_helix_dir_clockwise)
            #local _ih_sy_fn    = function(x,z,rise) {
                rise*f_normalized_atan2(x,z)/(2*pi)
            }    
        #break
        #case(Iso_helix_dir_counterclockwise)
            #local _ih_sy_fn    = function(x,z,rise) {
                rise*f_normalized_atan2(z,x)/(2*pi)
            }    
        #break
        #else
            #error concat("Iso_helix1: Invalid Dir ", str(Dir, 0, 3), "\n")
        #break    
    #end
    
    #local _ih_fn2  = function(x,y,z,r1,r2) {
        _ih_circle_fn(
            Iso_length_2d(x,z) - r1,
            y,
            r2
        )    
    }
    
    function(x,y,z,r1,r2,rise) {
        _ih_fn2(x,_ih_qy_fn(y-_ih_sy_fn(x,z,rise),rise),z,r1,r2)
    }    
#end

// End Iso_helix1
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_helix1(Radius1,Radius2,Rise,Dir)
//
#macro Iso_fixed_helix1(Radius1,Radius2,Rise,Dir)
    #local _ifh_helix_fn    = Iso_helix1(Dir)
    
    function(x,y,z) {
        _ifh_helix_fn(x,y,z,Radius1,Radius2,Rise)
    }
#end

// End Iso_fixed_helix1
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_helix_extrusion(Dir,SDF)
//
// Create a helix SDF(x,y) in the given direction using the provided SDF as the
// 2D shape to be extruded along the path of the helix.
//
// Iso_helix1 is equivalent to Iso_helix_extrusion(Dir,Iso_fixed_2d_circle)
//
#macro Iso_helix_extrusion(Dir,SDF)
    #local _ih_qy_fn    = function(y,rise) {
        y - 
        (
            select(y,
                int(y/rise - 0.5)*rise,
                int(y/rise + 0.5)*rise
            )
        )        
    }
    
    #switch(Dir)
        #case(Iso_helix_dir_clockwise)
            #local _ih_sy_fn    = function(x,z,rise) {
                rise*f_normalized_atan2(x,z)/(2*pi)
            }    
        #break
        #case(Iso_helix_dir_counterclockwise)
            #local _ih_sy_fn    = function(x,z,rise) {
                rise*f_normalized_atan2(z,x)/(2*pi)
            }    
        #break
        #else
            #error concat("Iso_helix1: Invalid Dir ", str(Dir, 0, 3), "\n")
        #break    
    #end
    
    #local _ih_fn2  = function(x,y,z,r1) {
        SDF(
            Iso_length_2d(x,z) - r1,
            y
        )    
    }
    
    function(x,y,z,r1,rise) {
        _ih_fn2(x,_ih_qy_fn(y-_ih_sy_fn(x,z,rise),rise),z,r1)
    }    
#end

// End Iso_helix_extrusion
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_helix_extrusion(Radius,Rise,Dir,SDF)
//
#macro Iso_fixed_helix_extrusion(Radius,Rise,Dir,SDF)
    #local _ifhe_helix_fn   = Iso_helix_extrusion(Dir,SDF)
    
    function(x,y,z) {
        _ifhe_helix_fn(x,y,z,Radius,Rise)
    }
#end

// End Iso_fixed_helix_extrusion
//-----------------------------------------------------------------------------

// End Helix Functions
//=============================================================================

//=============================================================================
// Iso_extrusion(Primitive2DFn)
//      Returns function(x,y,z,h) that computes the distance from the surface of
// a 3D object created by extruding the given 2D shape function along the Z axis
// scaled out by h in the positive and negative directions
//
// Iso_fixed_extrusion(Primitive2DFn,Height)
//      Returns function(x,y,z) that computes the distance from the surface of
// a 3D object created by extruding the given 2D shape function along the Z axis
// scaled out by a fixed Height/2 in the positive and negative directions
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//float opExtrusion( in vec3 p, in sdf2d primitive, in float h )
//{
//    float d = primitive(p.xy)
//    vec2 w = vec2( d, abs(p.z) - h );
//    return min(max(w.x,w.y),0.0) + length(max(w,0.0));
//}

//-----------------------------------------------------------------------------
// Iso_extrusion(Primitive2DFn)
//
#macro Iso_extrusion(Primitive2DFn)
    #local _ie_extrusion_fn = function(wx,wy) {
        min(max(wx,wy),0.0) + Iso_length_2d(max(wx,0),max(wy,0))
    }
    function(x,y,z,h) {
        _ie_extrusion_fn(Primitive2DFn(x,y),abs(z) - h)
    }
#end

// End Iso_extrusion
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_extrusion(Primitive2DFn,Height)
//
#macro Iso_fixed_extrusion(Primitive2DFn,Height)
    #local _h               = Height/2;
    #local _ie_extrusion_fn = function(wx,wy) {
        min(max(wx,wy),0.0) + Iso_length_2d(max(wx,0),max(wy,0))
    }
    function(x,y,z) {
        _ie_extrusion_fn(Primitive2DFn(x,y),abs(z) - _h)
    }
#end

// End Iso_fixed_extrusion
//-----------------------------------------------------------------------------


// End Iso_extrusion(Primitive2DFn)
//=============================================================================
 
//=============================================================================
// Iso_rotation(Primitive2DFn)
//      Returns a function(x,y,z) that computes the distance from the surface of
// revolution generated by rotating the given Primitive2DFn around the Y axis.
//
// Adapted from: https://iquilezles.org/articles/distfunctions/
//float opRevolution( in vec3 p, in sdf2d primitive, float o )
//{
//    vec2 q = vec2( length(p.xz) - o, p.y );
//    return primitive(q)
//}
#macro Iso_rotation(Primitive2DFn)
    function(x,y,z,o) {
        Primitive2DFn(Iso_length_2d(x,z)-o,y)
    }    
#end

#macro Iso_fixed_rotation(Primitive2DFn,Radius)
    #local _ifr_fn  = Iso_rotation(Primitive2DFn)
    function(x,y,z) {
        _ifr_fn(x,y,z,Radius)
    }    
#end

// End Iso_rotation
//=============================================================================

//=============================================================================
// Iso_rounding(Primitive3DFn)
//      Returns a function(x,y,z,r) that computes the distance from the surface of
// the 3D object defined by the given exact distance function after applying a
// rounding value to the edges defined by r.
//
// Iso_fixed_rounding(Primitive3DFn,Rnd)
//      Returns a function(x,y,z) that computes the distance from the surface of
// the 3D object defined by the given exact distance function after applying a
// fixed rounding value Rnd.
//

//-----------------------------------------------------------------------------
// Iso_rounding(Primitive3DFn)
//
#macro Iso_rounding(Primitive3DFn)
    function(x,y,z,r) {
        Primitive3DFn(x,y,z) - r
    }    
#end

// End Iso_rounding
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_rounding(Primitive3DFn,Rnd)
//
#macro Iso_fixed_rounding(Primitive3DFn,Rnd)
    function(x,y,z) {
        Primitive3DFn(x,y,z) - Rnd
    }    
#end

// End Iso_fixed_rounding
//-----------------------------------------------------------------------------

// End Iso_rounding,Iso_fixed_rounding
//=============================================================================

//=============================================================================
// Iso Shape Combining Functions
//
#declare Iso_union  = function(d1,d2) {
    min(d1,d2)
}

#declare Iso_subtraction    = function(d1,d2) {
    max(-d1,d2)
}

#declare Iso_intersection   = function(d1,d2) {
    max(d1,d2)
}

#declare Iso_xor    = function(d1,d2) {
    max(min(d1,d2),-max(d1,d2))
}

#macro Iso_smooth_union()
    #local _isu_union_fn    = function(d1,d2,h,k) {
        min(d1,d2) - 0.25*h*h/k
    }
    function(d1,d2,k) {
        _isu_union_fn(d1,d2,max(k-abs(d1-d2),0.0),k)
    }
#end

#macro Iso_fixed_smooth_union(K)
    #local _isu_union_fn    = function(d1,d2,h) {
        min(d1,d2) - 0.25*h*h/K
    }
    function(d1,d2) {
        _isu_union_fn(d1,d2,max(K-abs(d1-d2),0.0))
    }
#end

#macro Iso_smooth_subtraction()
    #local _iss_union_fn    = Iso_smooth_union()
    function(d1,d2,k) {
        -_iss_union_fn(d1, -d2, k)
    }    
#end

#macro Iso_fixed_smooth_subtraction(K)
    #local _iss_union_fn    = Iso_fixed_smooth_union(K)
    function(d1,d2) {
        -_iss_union_fn(d1, -d2)
    }    
#end

#macro Iso_smooth_intersection()
    #local _iss_union_fn    = Iso_smooth_union()
    function(d1,d2,k) {
        -_iss_union_fn(-d1,-d2,k)
    }
#end

#macro Iso_fixed_smooth_intersection(K)
    #local _iss_union_fn    = Iso_fixed_smooth_union(K)
    function(d1,d2) {
        -_iss_union_fn(-d1, -d2)
    }    
#end

#macro Iso_smooth_union2(MixFn)
    function(d1,d2,k) {
        MixFn(d1,d2,k)
    }    
#end

#macro Iso_fixed_smooth_union2(MixFn,K)
    function(d1,d2) {
        MixFn(d1,d2,K)
    }    
#end

#macro Iso_smooth_subtraction2(MixFn)
    #local _iss_union_fn    = Iso_smooth_union2(MixFn)
    function(d1,d2,k) {
        -_iss_union_fn(d1, -d2, k)
    }    
#end

#macro Iso_fixed_smooth_subtraction2(MixFn,K)
    #local _iss_union_fn    = Iso_fixed_smooth_union2(MixFn,K)
    function(d1,d2) {
        -_iss_union_fn(d1, -d2)
    }    
#end

#macro Iso_smooth_intersection2(MixFn)
    #local _iss_union_fn    = Iso_smooth_union2(MixFn)
    function(d1,d2,k) {
        -_iss_union_fn(-d1,-d2,k)
    }
#end

#macro Iso_fixed_smooth_intersection2(MixFn,K)
    #local _iss_union_fn    = Iso_fixed_smooth_union2(MixFn,K)
    function(d1,d2) {
        -_iss_union_fn(-d1, -d2)
    }    
#end

#macro Iso_root_mix()
    #local _kmult   = 2.0;
    #local _irm_mix_fn  = function(d1,d2,k,h) {
        0.5*(d1+d2-sqrt(h*h+k*k))
    }                           
    #local _irm_aux_fn  = function(d1,d2,k) {
        _irm_mix_fn(d1,d2,k,d2-d1)
    }
    function(d1,d2,k) {
        _irm_aux_fn(d1,d2,k*_kmult)
    }
#end

#macro Iso_quadratic_mix()
    #local _kmult   = 4.0;
    #local _iqm_mix_fn  = function(d1,d2,k,h) {
        min(d1,d2) - 0.25*h*h*k
    }
    #local _iqm_aux_fn  = function(d1,d2,k) {
        _iqm_mix_fn(d1,d2,k,max(k-abs(d1-d2), 0.0)/k)
    }
    function(d1,d2,k) {
        _iqm_aux_fn(d1,d2,_kmult*k)
    }        
#end

#macro Iso_cubic_mix()
    #local _kmult   = 6.0;
    #local _icm_mix_fn  = function(d1,d2,k,h) {
        min(d1,d2) - h*h*h*k/6.0
    }
    #local _icm_aux_fn  = function(d1,d2,k) {
        _icm_mix_fn(d1,d2,k,max(k-abs(d1-d2), 0.0)/k)
    }
    function(d1,d2,k) {
        _icm_aux_fn(d1,d2,_kmult*k)
    }        
#end

#macro Iso_quartic_mix()
    #local _kmult   = 16.0/3.0;
    #local _iqm_mix_fn  = function(d1,d2,k,h) {
        min(d1,d2) - h*h*h*(4.0-h)*k/16.0
    }
    #local _iqm_aux_fn  = function(d1,d2,k) {
        _iqm_mix_fn(d1,d2,k,max(k-abs(d1-d2), 0.0)/k)
    }
    function(d1,d2,k) {
        _iqm_aux_fn(d1,d2,_kmult*k)
    }        
#end

// End Iso Shape Combining Functions
//=============================================================================

//=============================================================================
// 2D Extrusion Functions
//

//-----------------------------------------------------------------------------
// Iso_2d_circle()
//
#macro Iso_2d_circle()
    function(x,y,r) {
        Iso_length_2d(x,y) - r
    }
#end

// End Iso_2d_circle
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_2d_circle(Radius)
//
#macro Iso_fixed_2d_circle(Radius)
    #local _if2c_circle_fn  = Iso_2d_circle()
    function(x,y) {
        _if2c_circle_fn(x,y,Radius)
    }
#end

// End Iso_fixed_2d_circle
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_2d_ellipse()
//
#macro Iso_2d_ellipse()
    function(x,y,a,b) {
        Iso_length_2d(x/a,y/b) - 1
    }
#end

// End Iso_2d_ellipse
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_2d_ellipse(Radius)
//
#macro Iso_fixed_2d_ellipse(Radius)
    #local _if2c_ellipse_fn  = Iso_2d_ellipse()
    function(x,y) {
        _if2c_ellipse_fn(x,y,Radius)
    }
#end

// End Iso_fixed_2d_ellipse
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_2d_box()
//      Returns function(x,y,xs,ys) that computes the distance from the edges of
// a 2D box scaled by xs and ys.
//
#macro Iso_2d_box()
    #local _ib_box_fn   = function(x,y) {
        Iso_length_2d(max(x,0),max(y,0)) +
        min(max(x,y),0)
    }
    function(x,y,sx,sy) {
        _ib_box_fn(abs(x)-sx,abs(y)-sy)
    }
    
#end

// End Iso_2d_box
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_2d_box(Size)
//
#macro Iso_fixed_2d_box(Size)
    #local _sx  = Size.x/2;
    #local _sy  = Size.y/2;
    
    #local _if2_box_fn  = Iso_2d_box()
    function(x,y) {
        _if2_box_fn(x,y,_sx,_sy)
    }    
#end

// End Iso_fixed_2d_box
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_2d_rhombus()
//      Returns function(x,y,xs,ys) that computes the distance from the edges of
//   2D rhombus scaled by xs and ys
//
#macro Iso_2d_rhombus()
    #local _ir_rhombus_fn   = function(x,y,sx,sy,d) {
        d*Iso_sign(x*sy+y*sx - sx*sy)
    }
    
    #local _ir_h_fn = function(ax,ay,sx,sy) {
        Iso_clamp(Iso_ndot_2d(sx-2.0*ax,sy-2.0*ay,sx,sy)/Iso_dot2_2d(sx,sy), -1.0, 1.0)
    }
    
    #local _ir_d_fn = function(ax,ay,sx,sy,h) {
        Iso_length_2d(ax-0.5*sx*(1.0-h),ay-0.5*sy*(1.0+h))
    }
    
    #local _ir_fn2  = function(ax,ay,sx,sy) {
        _ir_rhombus_fn(ax,ay,sx,sy,_ir_d_fn(ax,ay,sx,sy,_ir_h_fn(ax,ay,sx,sy)))
    }
    
    function(x,y,sx,sy) {
        _ir_fn2(abs(x),abs(y),sx,sy)
    }
    
#end

// End Iso_2d_rhombus
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_2d_rhombus(SX,SY)
//
#macro Iso_fixed_2d_rhombus(SX,SY)
    #local _ifr_rhombus_fn  = Iso_2d_rhombus()
    
    function(x,y) {
        _ifr_rhombus_fn(x,y,SX,SY)
    }
#end

// End Iso_fixed_2d_rhombus(SX,SY)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_2d_isosceles_trapezoid()
//
#macro Iso_2d_isosceles_trapezoid()
    #local _it_trap_fn  = function(cax,cay,cbx,cby,s) {
        s*sqrt(min(Iso_dot2_2d(cax,cay),Iso_dot2_2d(cbx,cby)))
    }
    
    #local _it_s_fn = function(cbx,cay) {
        select(cbx,
            select(cay,
                -1,
                1
            ),
            1
        )        
    }    
    
    #local _it_cax_fn   = function(px,py,r1,r2) {
        px -
            min(
                px,
                select(py,r1,r2)
            )     
    }
    
    #local _it_cay_fn   = function(py,h) {
        abs(py) - h
    }
    
    #local _it_cbx_fn   = function(px,py,k1x,k1y,k2x,k2y) {
        px - k1x + k2x*Iso_clamp(Iso_dot_2d(k1x-px,k1y-py,k2x,k2y)/Iso_dot2_2d(k2x,k2y), 0.0, 1.0)
    }
    #local _it_cby_fn   = function(px,py,k1x,k1y,k2x,k2y) {
        py - k1y + k2y*Iso_clamp(Iso_dot_2d(k1x-px,k1y-py,k2x,k2y)/Iso_dot2_2d(k2x,k2y), 0.0, 1.0)
    }
    #local _it_fn2  = function(cax,cay,cbx,cby) {
        _it_trap_fn(cax,cay,cbx,cby,_it_s_fn(cbx,cay))
    }
    
    function(x,y,r1,r2,h) {
        _it_fn2(
            _it_cax_fn(abs(x),y,r1,r2),
            _it_cay_fn(y,h),
            _it_cbx_fn(abs(x),y,r2,h,r2-r1,2.0*h),
            _it_cby_fn(abs(x),y,r2,h,r2-r1,2.0*h)
        )    
    }
#end

// End Iso_2d_isosceles_trapezoid
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_2d_isosceles_trapezoid(R1,R2,H)
//
#macro Iso_fixed_2d_isosceles_trapezoid(R1,R2,H)
    #local _ifit_fn = Iso_2d_isosceles_trapezoid()
    
    function(x,y) {
        _ifit_fn(x,y,R1,R2,H)
    }
#end

// End Iso_fixed_2d_isosceles_trapezoid
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_2d_parallelogram()
//
#macro Iso_2d_parallelogram()
    #local _ip_fn7 = function(dx,dy) {
        sqrt(dx)*Iso_sign(-dy)
    }
    
    #local _ip_fn6  = function(dx,dy,vx,vy,wi,he,s) {
        _ip_fn7(
            min(dx,Iso_dot2_2d(vx,vy)),    
            min(dy, wi*he-abs(s))
        )
    }
    
    #local _ip_fn5  = function(px,py,wi,he,sk,dx,dy,s) {
        _ip_fn6(dx,dy,
            px - wi - sk*Iso_clamp(Iso_dot_2d(px-wi,py,sk,he)/Iso_dot2_2d(sk,he),-1.0,1.0),
            py - he*Iso_clamp(Iso_dot_2d(px-wi,py,sk,he)/Iso_dot2_2d(sk,he),-1.0,1.0),
            wi,
            he,
            s
        )    
    }
    
    #local _ip_fn4  = function(px,py,wi,he,sk,dx,dy,s) {
        _ip_fn5(
            select(s,-px,px),
            select(s,-py,py),
            wi,he,sk,dx,dy,s
        )    
    }
    
    #local _ip_fn3  = function(px,py,wi,he,sk,dx,dy) {
        _ip_fn4(px,py,wi,he,sk,dx,dy,
            px*he - py*sk
        )    
    }
    
    #local _ip_fn2  = function(px,py,wi,he,sk,wx,wy) {
        _ip_fn3(px,py,wi,he,sk,
            Iso_dot2_2d(wx,wy),
            -wy
        )    
    }
    
    #local _ip_fn1  = function(px,py,wi,he,sk) {
        _ip_fn2(px,py,wi,he,sk,
            (px-sk-Iso_clamp(px-sk,-wi,wi)),
            (py-he)
        )    
    }
    
    function(x,y,wi,he,sk) {
        _ip_fn1(
            select(y,-x,x),
            select(y,-y,y),
            wi,
            he,
            sk
        )    
    }
    
#end

// End Iso_2d_parallelogram
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_2d_parallelogram(WS,HS,SKEW)
//
#macro Iso_fixed_2d_parallelogram(WS,HS,SKEW)
    #local _ifp_fn  = Iso_2d_parallelogram()
    
    function(x,y) {
        _ifp_fn(x,y,WS,HS,SKEW)
    }
#end

// End Iso_fixed_2d_parallelogram
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_2d_triangle(P0,P1,P2)
//
// Adapted from: https://iquilezles.org/articles/distfunctions2d/
//float sdTriangle( in vec2 p, in vec2 p0, in vec2 p1, in vec2 p2 )
//{
//    vec2 e0 = p1-p0, e1 = p2-p1, e2 = p0-p2;
//    vec2 v0 = p -p0, v1 = p -p1, v2 = p -p2;
//    vec2 pq0 = v0 - e0*clamp( dot(v0,e0)/dot(e0,e0), 0.0, 1.0 );
//    vec2 pq1 = v1 - e1*clamp( dot(v1,e1)/dot(e1,e1), 0.0, 1.0 );
//    vec2 pq2 = v2 - e2*clamp( dot(v2,e2)/dot(e2,e2), 0.0, 1.0 );
//    float s = sign( e0.x*e2.y - e0.y*e2.x );
//    vec2 d = min(min(vec2(dot(pq0,pq0), s*(v0.x*e0.y-v0.y*e0.x)),
//                     vec2(dot(pq1,pq1), s*(v1.x*e1.y-v1.y*e1.x))),
//                     vec2(dot(pq2,pq2), s*(v2.x*e2.y-v2.y*e2.x)));
//    return -sqrt(d.x)*sign(d.y);
//}
#macro Iso_2d_triangle(P0,P1,P2)
    #local _p0x = P0.x;
    #local _p0y = P0.y;
    #local _p1x = P1.x;
    #local _p1y = P1.y;
    #local _p2x = P2.x;
    #local _p2y = P2.y;
    
    #local _e0      = P1 - P0;
    #local _e0x     = _e0.x;
    #local _e0y     = _e0.y;
    #local _e0d2    = Iso_dot2_2d(_e0x,_e0y);
    #local _e1      = P2 - P1;             
    #local _e1x     = _e1.x;
    #local _e1y     = _e1.y;
    #local _e1d2    = Iso_dot2_2d(_e1x,_e1y);
    #local _e2      = P0 - P2;             
    #local _e2x     = _e2.x;
    #local _e2y     = _e2.y;
    #local _e2d2    = Iso_dot2_2d(_e2x,_e2y);
    
    #local _s   = Iso_sign(_e0.x*_e2.y - _e0.y*_e2.x);
    
    // _v0x = x - p0.x; _v0y = y - p0.y
    // _v1x = x - p1.x; _v1y = y - p1.y
    // _v2x = x - p2.x; _v2y = y - p2.y 
    
    #local _it_triangle_fn  = function(dx,dy) {
        -sqrt(dx)*Iso_sign(dy)
    }
    
    #local _it_dx_fn    = function(pq0x,pq0y,pq1x,pq1y,pq2x,pq2y) {
        min(
            Iso_dot2_2d(pq0x,pq0y),
            Iso_dot2_2d(pq1x,pq1y),
            Iso_dot2_2d(pq2x,pq2y)
        )    
    }
    
    #local _it_dy_fn    = function(v0x,v0y,v1x,v1y,v2x,v2y) {
        min(
            _s*(v0x*_e0y-v0y*_e0x),
            _s*(v1x*_e1y-v1y*_e1x),
            _s*(v2x*_e2y-v2y*_e2x)
        )
    }
    
    #local _it_pqx_fn   = function(vx,vy,ex,ey,ed2) {
        vx - ex*Iso_clamp(Iso_dot_2d(vx,vy,ex,ey)/ed2), 0.0, 1.0)
    }
    
    #local _it_pqy_fn   = function(vx,vy,ex,ey,ed2) {
        vy - ey*Iso_clamp(Iso_dot_2d(vx,vy,ex,ey)/ed2), 0.0, 1.0)
    }
    
    function(x,y) {
        _it_triangle_fn(
            _it_dx_fn(
                _it_pqx_fn(x-_p0x,y-_p0y,_e0x,_e0y,_e0d2),
                _it_pqy_fn(x-_p0x,y-_p0y,_e0x,_e0y,_e0d2),
                _it_pqx_fn(x-_p1x,y-_p1y,_e1x,_e1y,_e1d2),
                _it_pqy_fn(x-_p1x,y-_p1y,_e1x,_e1y,_e1d2),
                _it_pqx_fn(x-_p2x,y-_p2y,_e2x,_e2y,_e2d2),
                _it_pqy_fn(x-_p2x,y-_p2y,_e2x,_e2y,_e2d2)
            ),
            _it_dy_fn(
                x-_p0x,
                y-_p0y,
                x-_p1x,
                y-_p0y,
                x-_p2x,
                y-_p2y
            )
        )
    }
    
#end

// End Iso_2d_triangle
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_2d_equilateral_triangle(R)
//
// Adapted from: https://iquilezles.org/articles/distfunctions2d/
//float sdEquilateralTriangle( in vec2 p, in float r )
//{
//    const float k = sqrt(3.0);
//    p.x = abs(p.x) - r;
//    p.y = p.y + r/k;
//    if( p.x+k*p.y>0.0 ) p = vec2(p.x-k*p.y,-k*p.x-p.y)/2.0;
//    p.x -= clamp( p.x, -2.0*r, 0.0 );
//    return -length(p)*sign(p.y);
//}
#macro Iso_2d_equilateral_triangle(R)
    #local _k   = sqrt(3.0);
    #local _ty  = R/_k;
    
    #local _i2et_triangle_fn    = function(px,py) {
        -Iso_length_2d(px,py)*Iso_sign(py)
    }
    
    #local _i2et_tmp2_fn    = function(px,py) {
        _i2et_triangle_fn(
            px - Iso_clamp(px, -2.0*R, 0.0),
            py
        )    
    }
    
    #local _i2et_tmp1_fn    = function(px,py) {
        select(-(px+_k*py),
            _i2et_tmp2_fn((px-_k*py)/2,(-_k*px-py)/2),
            _i2et_tmp2_fn(px,py)
        )    
    }
    
    function(x,y) {
        //_i2et_tmp1_fn(abs(x) - R, y + R/_k)
        _i2et_tmp1_fn(abs(x) - R, y)
    }
    
#end

// End Iso_2d_equilateral_triangle
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_2d_isosceles_triangle(QX,QY)
//
// Adapted from: https://iquilezles.org/articles/distfunctions2d/
//float sdTriangleIsosceles( in vec2 p, in vec2 q )
//{
//    p.x = abs(p.x);
//    vec2 a = p - q*clamp( dot(p,q)/dot(q,q), 0.0, 1.0 );
//    vec2 b = p - q*vec2( clamp( p.x/q.x, 0.0, 1.0 ), 1.0 );
//    float s = -sign( q.y );
//    vec2 d = min( vec2( dot(a,a), s*(p.x*q.y-p.y*q.x) ),
//                  vec2( dot(b,b), s*(p.y-q.y)  ));
//    return -sqrt(d.x)*sign(d.y);
//}
#macro Iso_2d_isosceles_triangle(QX,QY)
    #local _qd2 = Iso_dot2_2d(QX,QY);
    #local _s   = -Iso_sign(QY);
    
    #local _i2it_triangle_fn    = function(dx,dy) {
        -sqrt(dx)*Iso_sign(dy)
    }
    
    #local _i2it_dx_fn  = function(ax,ay,bx,by) {
        min(Iso_dot2_2d(ax,ay), Iso_dot2_2d(bx,by))
    }
    
    #local _i2it_dy_fn  = function(px,py) {
        min(
            _s*(px*QY-py*QX),
            _s*(py-QY)
        )    
    }
    
    #local _i2it_ax_fn  = function(px,py) {
        px - QX*Iso_clamp(Iso_dot_2d(px,py,QX,QY)/_qd2, 0.0, 1.0)
    }
    #local _i2it_ay_fn  = function(px,py) {
        py - QY*Iso_clamp(Iso_dot_2d(px,py,QX,QY)/_qd2, 0.0, 1.0)
    }                                                            
    
    #local _i2it_bx_fn  = function(px,py) {
        px - QX*Iso_clamp(px/QX, 0.0, 1.0)
    }
    #local _i2it_by_fn  = function(px,py) {
        py - QY
    }
    
    #local _i2it_tmp2_fn    = function(px,py,ax,ay,bx,by) {
        _i2it_triangle_fn(
            _i2it_dx_fn(ax,ay,bx,by),
            _i2it_dy_fn(px,py)
        )    
    }
    
    #local _i2it_tmp1_fn    = function(px,py) {
        _i2it_tmp2_fn(
            px,
            py,
            _i2it_ax_fn(px,py),
            _i2it_ay_fn(px,py),
            _i2it_bx_fn(px,py),
            _i2it_by_fn(px,py)
        )    
    }
    
    function(x,y) {
        _i2it_tmp1_fn(abs(x),QY-y)
    }
    
#end

// End Iso_2d_isosceles_triangle
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_2d_uneven_capsule()
//
#macro Iso_2d_uneven_capsule()
    
    #local _uc_fn4  = function(px,py,r1,r2,h,b,a,k) {
        select(k,
            Iso_length_2d(px,py) - r1,
            select(a*h - k,
                Iso_length_2d(px,py-h) - r2,
                Iso_dot_2d(px,py,a,b) - r1
            )
        )        
    }
    
    #local _uc_fn3  = function(px,py,r1,r2,h,b,a) {
        _uc_fn4(px,py,r1,r2,h,b,a,
            Iso_dot_2d(px,py,-b,a)
        )    
    }
    
    #local _uc_fn2  = function(px,py,r1,r2,h,b) {
        _uc_fn3(px,py,r1,r2,h,b,
            sqrt(1-b*b)
        )        
    }
    
    #local _uc_fn1  = function(px,py,r1,r2,h) {
        _uc_fn2(px,py,r1,r2,h,(r1-r2)/h)
    }
    
    function(x,y,r1,r2,h) {
        _uc_fn1(abs(x),y,r1,r2,h)
    }
#end                       

// End Iso_2d_uneven_capsule
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_2d_uneven_capsule(R1,R2,HS)
//
#macro Iso_fixed_2d_uneven_capsule(R1,R2,HS)
    #local _ifuc_fn = Iso_2d_uneven_capsule()
    function (x,y) {
        _ifuc_fn(x,y,R1,R2,HS)
    }
#end

// End Iso_fixed_2d_uneven_capsule
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_2d_regular_pentagon()
//
#macro Iso_2d_regular_pentagon()
    #local _kx  = 0.809016994;
    #local _ky  = 0.587785252;
    #local _kz  = 0.726542528; 
    
    #local _rp_fn4  = function(px,py) {
        Iso_length_2d(px,py)*Iso_sign(py)
    }
    
    #local _rp_fn3  = function(px,py,r) {
        _rp_fn4(
            px - Iso_clamp(px,-r*_kz,r*_kz),
            py - r
        )    
    }
    
    #local _rp_fn2  = function(px,py,r) {
        _rp_fn3(
            px - 2.0*min(Iso_dot_2d(_kx,_ky,px,py),0.0)*_kx,
            py - 2.0*min(Iso_dot_2d(_kx,_ky,px,py),0.0)*_ky,
            r
        )    
    }
    
    #local _rp_fn1  = function(px,py,r) {
        _rp_fn2(
            px - 2.0*min(Iso_dot_2d(-_kx,_ky,px,py),0.0)*(-_kx),
            py - 2.0*min(Iso_dot_2d(-_kx,_ky,px,py),0.0)*_ky,
            r
        )    
    }
    
    function(x,y,r) {
        _rp_fn1(abs(x),y,r)
    }
#end

// End Iso_2d_regular_pentagon
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_2d_regular_pentagon(R)
//
#macro Iso_fixed_2d_regular_pentagon(R)
    #local _frp_fn  = Iso_2d_regular_pentagon()
    function(x,y) {
        _frp_fn(x,y,R)
    }    
#end

// End Iso_fixed_2d_regular_pengagon
//-----------------------------------------------------------------------------

// End 2D Extrusion Functions
//=============================================================================

#version Libisoshapes_Inc_Temp;
#end // ifndef(Libisoshapes_Inc_Temp)