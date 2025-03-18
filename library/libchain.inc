/******************************************************************************
 * File: libchain.inc
 * Description:
 *      Generic library for laying out chains with a defined type of link.
 ******************************************************************************/

#ifndef(Libchain_Inc_Temp)

#declare Libchain_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libchain.inc\n"
#end                     

//=============================================================================
// Includes
//

//-----------------------------------------------------------------------------
// Standard Includes
//
#include "transforms.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
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
// Chain Link Definitions
//
//  A Chain link is a dictionary that contains metrics about the types of link(s)
//  that are contained within a given chain and it used to lay out the links
//  for a given layout pattern.  Chain links are defined to point along the
//  positive Z axis, and should be defined such that the point <0,0,0> lies
//  on the attachment point for the "previous" link in a chain, and the link
//  is lying flat, but centered on the y=0 plane. 
//
//  The link definition includes the following:
//      .link_name:     unique string identifier
//      .link_size:     3D <x,y,z> bounding box size of the link
//      .link_inner:    2D <x,z> inner length (z axis) and width (x axis) of the link
//      .link_obj:   (optional) fixed object representing a chaing link
//
//  If .link_obj is defined, then all links in the chain use the same
//  object.  If it is not defined, then you must define the macro Chain_link_obj_create(...)
//  to create a new object for each link in the chain.  The parameters define the position
//  in the chain and its 3D position in space that can be used to define variants.
//

//-----------------------------------------------------------------------------
// Chain_link_spec_create(LinkName,LinkSize,LinkInner,optional LinkObject)
//
#macro Chain_link_spec_create(LinkName,LinkSize,LinkInner,optional LinkObject)
    #local _spec    = dictionary {
        .link_name: LinkName,
        .link_size: LinkSize,
        .link_inner:    LinkInner,
        #ifdef(LinkObject)
        .link_obj:   LinkObject,
        #end
    };
    
    _spec
#end

// End Chain_link_spec_create
//-----------------------------------------------------------------------------

// End Chain Link Definitions
//=============================================================================

//=============================================================================
// Chain Definitions
//
//  A Chain is defined by one or more Chain Link specifications and a function
//  for selecting among those definitions when adding a new link to a chain.
//
//  A Chain is a dictionary that contains the following:
//
//      .link_spec_count:   number of link specs for the chain
//      .link_specs:        array of Chain_link_specs
//      .link_select_fn:    optional function(N) that selects the Chain_link_spec to use when adding multiple links
//      .link_count:        optional total number of links available in the chain

//-----------------------------------------------------------------------------
// Chain_spec_create(LinkSpec,optional LinkCount)
//
#macro Chain_spec_create(LinkSpec,optional LinkCount)
    #local _link_specs  = array;
    #local _link_specs[0]   = LinkSpec;
    
    #local _spec    = dictionary {
        .link_spec_count:   1,
        .link_specs:        _link_specs,
        #ifdef(LinkCount)
        .link_count:        LinkCount,
        #end
    };
    
    _spec
#end

// End Chain_spec_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_spec_add_link_spec(ChainSpec,LinkSpec,optional SelectFn)
//
#macro Chain_spec_add_link_spec(ChainSpec,LinkSpec,optional SelectFn)
    #declare ChainSpec.link_specs[ChainSpec.link_spec_count]    = LinkSpec;
    #declare ChainSpec.link_spec_count  = ChainSpec.link_spec_count + 1;
    #ifdef (SelectFn)
    #declare ChainSpec.link_select_fn   = SelectFn;
    #end
#end

// End Chain_spec_add_link_spec
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_spec_select_link_spec(ChainSpec,LinkNumber)
//
#macro Chain_spec_select_link_spec(ChainSpec,LinkNumber)
    #if (ChainSpec.link_spec_count = 1)
        #local _link_spec   = ChainSpec.link_specs[0];
    #else
        #ifdef (ChainSpec.link_select_fn)
            #local _link_spec_n = ChainSpec.link_select_fn(LinkNumber);
            #if (_link_spec_n >= ChainSpec.link_spec_count)
                #warning concat("Chain_spec_select_link_spec: Select function returned link spec number ", str(_link_spec_n, 0, 0), " which is out of bounds: using ", str(ChainSpec.link_spec_count-1, 0, 0), "\n")
                #local _link_spec_n = ChainSpec.link_spec_count-1;
            #end
        #else
            #local _link_spec_n = mod(LinkNumber, ChainSpec.link_spec_count);
        #end
        #local _link_spec   = ChainSpec.link_specs[_link_spec_n];    
    #end 
    
    _link_spec   
#end

// End Chain_spec_select_link_spec
//-----------------------------------------------------------------------------

// End Chain Definitions
//=============================================================================

//=============================================================================
// Chain Layout Macros
//
//  These macros create a Chain_layout object that can be rendered into a
//  union{} with Chain_layout_render().
//

//-----------------------------------------------------------------------------
// Chain_link_obj_create(LinkSpec,LinkNumber,BackLoc,FrontLoc)
//
//      If any Chain_link_spec does not contain a defined .link_obj, then this
//  macro must be defined to create an instance of the chain link in its
//  standard position and orientation.  The supplied LinkSpec can be used to
//  determine what type of link to create, the LinkNumber and LinkLayout info
//  can be used to add variations that could depend on which link in the chain
//  is being rendered, or random variations using the location of the chain.
//
//  The macro should return an object{} for the link whose BackLoc is <0,0,0>
//  and whose FrontLoc is BackLoc + <0, 0, LinkSpec.link_inner.y>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_link_layout_create(LinkObj,LinkRot,BackLoc,FrontLoc)
//
//      Creates a dictionary{} consisting of:
//      .link_obj:  LinkObj,
//      .link_rot:  LinkRot,
//      .link_back: BackLoc,
//      .link_front:    FrontLoc,
//
//  This is the smallest unit of a Chain_layout representing a single
//  link in the chain.  It is generally created by other Chain_layout macros.
//
#macro Chain_link_layout_create(LinkObj,LinkRot,BackLoc,FrontLoc)
    #local _layout  = dictionary {
        .link_obj:  LinkObj,
        .link_rot:  LinkRot,
        .link_back: BackLoc,
        .link_front:    FrontLoc,
    };
    
    _layout    
#end

// End Chain_link_layout_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_link_layout_trans(LinkLayout)
// 
//  Creates the full transform{} for the given LinkLayout based on the
//  .link_rot and .link_back and .link_front information.
//
#macro Chain_link_layout_trans(LinkLayout)
    #local _forward = vnormalize(LinkLayout.link_front - LinkLayout.link_back);
    #local _right   = #if (vlength(vcross(y,_forward)) = 0) x; #else VPerp_To_Plane(y,_forward); #end
    #local _up      = VPerp_To_Plane(_forward,_right);
    #local _matrix  = Matrix_Trans(_right,_up,_forward,LinkLayout.link_back);
    
    #local _t   = transform {
        rotate z*LinkLayout.link_rot
        transform { _matrix }
    }
    
    _t    
#end

// End Chain_link_layout_trans
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_link_layout_render(LinkLayout)
//
#macro Chain_link_layout_render(LinkLayout)
    #local _link    = object {
        LinkLayout.link_obj
        transform { Chain_link_layout_trans(LinkLayout) }
    }
    
    _link    
#end

// End Chain_link_layout_render
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_create(ChainSpec,optional LinkRotFn)
//
//      Creates a dictionary{} containing:
//      .chain_spec:    ChainSpec,
//      .link_rot_fn:   LinkRotFn (optional)
//      .num_links:     0,
//      .links:         array // will contain instances of Chain_link_layout_create() in order
//
#macro Chain_layout_create(ChainSpec,optional LinkRotFn)
    #local _layout  = dictionary {
        .chain_spec:    ChainSpec,
        #ifdef (LinkRotFn)
        .link_rot_fn:   LinkRotFn,
        #end
        .num_links:     0,
        .links:         array
    }
    ;
    
    _layout
#end

// End Chain_layout_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_add_link(ChainLayout,LinkLayout)
//
#macro Chain_layout_add_link(ChainLayout,LinkLayout)
    #declare ChainLayout.links[ChainLayout.num_links]   = LinkLayout;
    #declare ChainLayout.num_links                      = ChainLayout.num_links+1;
#end

// End Chain_layout_add_link
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_create_link_obj(ChainLayout,LinkNum,BackLoc,FrontLoc)
//
//      This function can be used to create a link object{} using the Chain_spec
//  associated with this Chain_layout to select the appropriate link spec,
//  and then using that link spec to create the object.
// 
#macro Chain_layout_create_link_obj(ChainLayout,LinkNum,BackLoc,FrontLoc)
    #local _link_spec   = Chain_spec_select_link_spec(ChainLayout.chain_spec,LinkNum);
    
    #ifdef(Chain_link_obj_create)
        #local _link_obj    = object { Chain_link_obj_create(_link_spec,LinkNum,BackLoc,FrontLoc) }
    #else
        #ifndef(_link_spec.link_obj)
            #error concat("Chain_layout_create_link_obj: Chain_link_spec ", _link_spec.link_name, " does not have a link_obj and no Chain_link_obj_create macro is defined\n")
        #end
        #local _link_obj    = object { _link_spec.link_obj }
    #end                                                    
    
    _link_obj
#end

// End Chain_layout_create_link_obj
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_add_link_auto(ChainLayout,BackLoc,FrontLoc,optional LinkRotFn)
//
#macro Chain_layout_add_link_auto(ChainLayout,BackLoc,FrontLoc,optional LinkRotFn)
    #local _link_rot    =
    #ifdef (LinkRotFn)
        LinkRotFn(ChainLayout.num_links,BackLoc.x,BackLoc.y,BackLoc.z);
    #else
        #ifdef (ChainLayout.link_rot_fn)
            ChainLayout.link_rot_fn(ChainLayout.num_links,BackLoc.x,BackLoc.y,BackLoc.z);
        #else   
            90*mod(ChainLayout.num_links,2);
        #end
    #end
     
    #ifdef (Chain_layout_verbose) #debug concat("_link_rot=", str(_link_rot, 0, 3), "\n") #end
    #ifdef (Chain_layout_verbose) #debug concat("vlength(FrontLoc-BackLoc)=", str(vlength(FrontLoc - BackLoc), 0, 3), "\n") #end
    
    #local _link_layout = Chain_link_layout_create(Chain_layout_create_link_obj(ChainLayout,ChainLayout.num_links,BackLoc,FrontLoc),_link_rot,BackLoc,FrontLoc);
    Chain_layout_add_link(ChainLayout,_link_layout)
#end

// End Chain_layout_add_link_auto
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_add_segment(ChainLayout,EndPoint,optional LinkOffsetFn, optional LinkRotFn)
//
//      Adds links to the current chain, starting either at <0,0,0> or the end
//      of the last link in the chain in a straight line towards EndPoint until
//      the front end of the last link added is beyond that EndPoint.
//
#macro Chain_layout_add_segment(ChainLayout,EndPoint, optional LinkOffsetFn, optional LinkRotFn)
    #if (ChainLayout.num_links = 0)
        #local _back_loc    = <0,0,0>;
    #else
        #local _back_loc    = ChainLayout.links[ChainLayout.num_links-1].link_front;
    #end
    #ifdef (LinkOffsetFn)
        #local _back_loc    = _back_loc + <0, 0, LinkOffsetFn(ChainLayout.num_links, _back_loc.x, _back_loc.y, _back_loc.z)>;
    #end    
    
    #local _segment_dir = vnormalize(EndPoint - _back_loc);
    #debug concat("_segment_dir=<", vstr(3, _segment_dir, ",", 0, 3), ">\n")
    #local _segment_len = vlength(EndPoint - _back_loc);
    #local _cur_len     = 0;
    #while (_cur_len < _segment_len)
        #local _link_spec   = Chain_spec_select_link_spec(ChainLayout.chain_spec,ChainLayout.num_links);
        #local _link_len    = _link_spec.link_inner.y;
        #local _front_loc   = _back_loc + _segment_dir*_link_len;
        Chain_layout_add_link_auto(ChainLayout,_back_loc,_front_loc,LinkRotFn)
        #local _back_loc    = _front_loc;
        #local _cur_len     = _cur_len + _link_len;
    #end
            
#end

// End Chain_layout_add_segment
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_add_spline_segment(ChainLayout,Spline,Length,optional LinkOffsetFn, optional LinkRotFn)
//
//  Adds a length of chain beginning at <0,0,0> or the link_front of the last
//  link currently in the chain that follows the given Spline for the given
//  length.  Links are selected automatically fro the set of link specs in
//  the chain.  This function does not allow for lax chains.
//
//  The Spline should be defined such that the values reflect the distance
//  from the last point in the chain to each link.  Use libspline.inc: Spline_create_distance_spline
//  to get accurate distance values in your spline.
//
//  Spline(0) should be the link_back of the first new link, which is also
//  the link_front of the last link in the layout or <0,0,0>.
//
//  Spline(Length) will represent the link_front of the last new link added to
//  the layout,
// 
#macro Chain_layout_add_spline_segment(ChainLayout,Spline,Length,optional LinkOffsetFn, optional LinkRotFn)
    #if (ChainLayout.num_links = 0)
        #local _back_loc    = <0,0,0>;
    #else
        #local _back_loc    = ChainLayout.links[ChainLayout.num_links-1].link_front;
    #end
    #ifdef (LinkOffsetFn)
        #local _back_loc    = _back_loc + <0, 0, LinkOffsetFn(ChainLayout.num_links, _back_loc.x, _back_loc.y, _back_loc.z)>;
    #end    
    
    #local _cur_len = 0;
    #while (_cur_len < Length)
        #local _link_spec   = Chain_spec_select_link_spec(ChainLayout.chain_spec,ChainLayout.num_links);
        #local _link_len    = _link_spec.link_inner.y;
        #local _front_loc   = Spline(_cur_len + _link_len);
        Chain_layout_add_link_auto(ChainLayout,_back_loc,_front_loc,LinkRotFn)
        #local _back_loc    = _front_loc;
        #local _cur_len     = _cur_len + _link_len;        
    #end
    
#end

// End Chain_layout_add_spline_segment
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_add_coil_segment(ChainLayout,CoilAxis,CoilPt,Revolutions,optional Offset,optional OffsetAbsolute, optional LinkRotFn)
//
//      Given a 3D CoilAxis vector and a CoilPt on that axis (defines the translation from <0,0,0>),
//  and a number of full and fractional revolutions around that axis, this function will add
//  links to the chain that revolve around that axis at a radius defined by the front of
//  the last chain link in the layout.  The chain will coil by the given Offset from the starting
//  perpendicular point of the axis, (default is the width of the chain link).
//
#macro Chain_layout_add_coil_segment(ChainLayout,CoilAxis,CoilPt,Revolutions,optional Offset,optional OffsetAbsolute, optional LinkRotFn)
    // Compute the perpendicular point on CoilAxis from the front of the last link
    // in the current chain, (or <0,0,0> if there are no links) translated by CoilPt
    //
    #local _offset_abs  = #ifdef(OffsetAbsolute) OffsetAbsolute; #else false; #end
    #local _coil_axis   = vnormalize(CoilAxis);
    
    #if (ChainLayout.num_links = 0)
        #local _back_loc   = <0,0,0>;
    #else
        #local _back_loc   = ChainLayout.links[ChainLayout.num_links-1].link_front;
    #end
    
    #local _coil_perpendicular_pt   = CoilPt + VProject_Axis(_back_loc - CoilPt, _coil_axis);
    #local _coil_radius             = vlength(_coil_perpendicular_pt - _back_loc);
    
    #local _max_rot                 = 360*Revolutions;
    #local _cur_rot                 = 0;
    #while (_cur_rot <= _max_rot)
        #local _link_spec   = Chain_spec_select_link_spec(ChainLayout.chain_spec,ChainLayout.num_links);
        #local _link_len    = _link_spec.link_inner.y;
        #local _link_arc    = degrees(_link_len/_coil_radius);
        #local _link_arc    = degrees(ChordRadius2Arc(_link_len,_coil_radius));
        #if (_offset_abs)
            #local _link_offset = #ifdef(Offset) Offset; #else _link_spec.link_size.x; #end
        #else
            #local _link_offset = #ifdef(Offset) Offset*_link_spec.link_size.x; #else _link_spec.link_size.x; #end
        #end    
        
        #local _front_loc   = CoilPt + _coil_axis*_link_offset*_link_arc/360 + vaxis_rotate(_back_loc - CoilPt, _coil_axis, _link_arc);
        #debug concat("_back_loc=<", vstr(3, _back_loc, ",", 0, 2), ">; _front_loc=<", vstr(3, _front_loc, ",", 0, 2), ">\n")
        Chain_layout_add_link_auto(ChainLayout,_back_loc,_front_loc,LinkRotFn)
        #local _back_loc    = _front_loc;
        #local _cur_rot     = _cur_rot + _link_arc;
    #end
        
#end

// End Chain_layout_add_coil_segment
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_render(ChainLayout)
//
#macro Chain_layout_render(ChainLayout)
    #if (ChainLayout.num_links = 0)
        #error concat("Chain_layout_render: No links in the chain layout\n")
    #elseif (ChainLayout.num_links = 1)
        #local _chain   = object { Chain_link_layout_render(ChainLayout.links[0]) }
    #else
        #local _chain   = union {
            #for (i, 0, ChainLayout.num_links-1, 1)
            object { Chain_link_layout_render(ChainLayout.links[i]) }
            #end
        }
    #end
    
    _chain    
#end

// End Chain_layout_render
//-----------------------------------------------------------------------------

// End Chain Layout Macros
//=============================================================================

#version Libchain_Inc_Temp;

#end // ifndef (Libchain_Inc_Temp)
