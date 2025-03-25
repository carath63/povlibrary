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
#include "libspline.inc"

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
//  object.  If it is not defined, then you must define the macro Chain_layout_link_obj_create(...)
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

//-----------------------------------------------------------------------------
// Chain_spec_create(LinkSpec)
//
#macro Chain_spec_create(LinkSpec)
    #local _link_specs      = array;
    #local _link_specs[0]   = LinkSpec;
    
    #local _spec    = dictionary {
        .link_spec_count:   1,
        .link_specs:        _link_specs,
    };
    
    _spec
#end

// End Chain_spec_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_spec_add_link_spec(ChainSpec,LinkSpec)
//
#macro Chain_spec_add_link_spec(ChainSpec,LinkSpec)
    #declare ChainSpec.link_specs[ChainSpec.link_spec_count]    = LinkSpec;
    #declare ChainSpec.link_spec_count  = ChainSpec.link_spec_count + 1;
#end

// End Chain_spec_add_link_spec
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
// Chain_layout_link_create(LinkSpec,optional LinkObj,LinkRot,BackLoc,FrontLoc)
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
#macro Chain_layout_link_create(LinkSpec, optional LinkObj,LinkRot,BackLoc,FrontLoc)
    #local _layout  = dictionary {
        .link_spec: LinkSpec,
        #ifdef(LinkObj)
        .link_obj:  LinkObj,
        #end
        .link_rot:  LinkRot,
        .link_back: BackLoc,
        .link_front:    FrontLoc,
    };
    
    _layout    
#end

// End Chain_layout_link_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_link_create_oriented(LinkSpec, optional LinkObj, LinkRot, BackLoc, Orientation)
//
//      This is a utility macro that calls Chain_layout_link_create() with a computed
//  FrontLoc based on the inner length from the LinkSpec rotated to the given Orientation
//  and then translated to the given BackLoc.
//
#macro Chain_layout_link_create_oriented(LinkSpec, optional LinkObj, LinkRot, BackLoc, Orientation)
    #local _front_trans = Reorient_Trans(z, Orientation)
    #local _link_front   = BackLoc + vtransform(<0, 0, LinkSpec.link_inner.y>, _front_trans);
    
    Chain_layout_link_create(LinkSpec, LinkObj, LinkRot, BackLoc, _link_front)
#end

// End Chain_layout_link_create_oriented
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_link_transform(LinkLayout)
// 
//  Creates the full transform{} for the given LinkLayout based on the
//  .link_rot and .link_back and .link_front information.
//
#macro Chain_layout_link_transform(LinkLayout)
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

// End Chain_layout_link_transform
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_link_object(LinkLayout,LinkNum)
//
//  Returns the actual object{} representing the link with the given
//  LinkLayout whose position in the chain is LinkNum.  It uses a cascading
//  method of determining the object:
//      1. If the LinkLayout has a .link_obj defined, use that
//      2. If the customized Chain_layout_link_obj_create(LinkLayout,LinkNum) macro is defined, call that
//      3. If the .link_spec in the LinkLayout has a default .link_obj defined for it, use that
//  Otherwise, if none of these are true, throw an error
//
#macro Chain_layout_link_object(LinkLayout,LinkNum)
    #ifdef (LinkLayout.link_obj)
        #local _link_obj    = object { LinkLayout.link_obj }
    #else
        #ifdef(Chain_layout_link_obj_create)
            #local _link_obj    = object { Chain_layout_link_obj_create(LinkLayout,LinkNum) }
        #else
            #ifdef (LinkLayout.link_spec.link_obj)
                #local _link_obj    = object { LinkLayout.link_spec.link_obj }
            #else
                #error concat("Chain_layout_link_object: No method to create link object\n")
            #end
        #end    
    #end 
    
    _link_obj   
#end

// End Chain_layout_link_object
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_link_render(LinkLayout,LinkNum)
//
#macro Chain_layout_link_render(LinkLayout,LinkNum)
    #local _link    = object {
        Chain_layout_link_object(LinkLayout,LinkNum)
        transform { Chain_layout_link_transform(LinkLayout) }
    }
    
    _link    
#end

// End Chain_layout_link_render
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_link_dump(Prefix,LinkLayout)
//
#macro Chain_layout_link_dump(Prefix,LinkLayout)
    #debug concat(
        Prefix,
        ".link_back=<", vstr(3, LinkLayout.link_back, ",", 0, 3), "> ",
        ".link_front=<", vstr(3, LinkLayout.link_front, ",", 0, 3), "> ",
        ".link_rot=", str(LinkLayout.link_rot, 0, 3), "\n"
    )
#end

// End Chain_layout_link_dump
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_options_create(optional LinkSpecSelectFn, optional LinkXOffsetFn, optional LinkYOffsetFn, optional LinkZOffsetFn, optional LinkRotFn)
//
//      Creates a dictionary{} consisting of:
//          .link_spec_select_fn(N,x,y,z)
//          .link_x_offset_fn(N,x,y,z)
//          .link_y_offset_fn(N,x,y,z)
//          .link_z_offset_fn(N,x,y,z)
//          .link_z_rot_fn(N,x,y,z)
//
//      with default implementations if not provided:
//          .link_spec_select_fn(N,x,y,z) { mod(N, ChainSpec.link_spec_count) } (computed on the fly when needed)
//          .link_z_offset_fn(N,x,y,z) { 0 }
//          .link_z_rot_fn(N,x,y,z) { mod(N,2)*90 }
//
//      N - is the link number within the full chain layout
//      x,y,z is the position of the back location of the link
//
//
#declare Chain_layout_default_link_x_offset_fn  = function(N,x,y,z) { 0 }
#declare Chain_layout_default_link_y_offset_fn  = function(N,x,y,z) { 0 }
#declare Chain_layout_default_link_z_offset_fn  = function(N,x,y,z) { 0 }
#declare Chain_layout_default_link_z_rot_fn     = function(N,x,y,z) { mod(N,2) * 90 }
#macro Chain_layout_options_create(optional LinkSpecSelectFn, optional LinkXOffsetFn, optional LinkYOffsetFn, optional LinkZOffsetFn, optional LinkRotFn)
    #local _options = dictionary {
        #ifdef(LinkSpecSelectFn)
        .link_spec_select_fn:   LinkSpecSelectFn,
        #end
        .link_x_offset_fn:  #ifdef(LinkXOffsetFn) LinkXOffsetFn, #else Chain_layout_default_link_x_offset_fn, #end
        .link_y_offset_fn:  #ifdef(LinkYOffsetFn) LinkYOffsetFn, #else Chain_layout_default_link_y_offset_fn, #end
        .link_z_offset_fn:  #ifdef(LinkZOffsetFn) LinkZOffsetFn, #else Chain_layout_default_link_z_offset_fn, #end
        .link_z_rot_fn:     #ifdef(LinkRotFn) LinkRotFn, #else Chain_layout_default_link_z_rot_fn, #end
    }
    ;
    
    _options
#end

// End Chain_layout_options_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_select_link_spec(ChainLayout,LinkNumber,BackPos,optional Options)
//
#macro Chain_layout_select_link_spec(ChainLayout,LinkNumber,BackPos,optional Options)
    #local _options = #ifdef(Options) Options; #else ChainLayout.default_options; #end
    
    #if (ChainLayout.chain_spec.link_spec_count = 1)
        #local _link_spec   = ChainLayout.chain_spec.link_specs[0];
    #else
        #ifdef (_options.link_spec_select_fn)
            #local _link_spec_n = _options.link_spec_select_fn(LinkNumber,BackPos.x,BackPos.y,BackPos.z);
        #else                                                                                            
            #local _link_spec_n = mod(LinkNumber,ChainLayout.chain_spec.link_spec_count);
        #end                                                                             
        #local _link_spec   = ChainLayout.chain_spec.link_specs[_link_spec_n];
    #end                                                         
    
    _link_spec
#end

// End Chain_layout_select_link_spec
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_select_link_rot(ChainLayout,LinkNumber,BackPos,optional Options)
//
#macro Chain_layout_select_link_rot(ChainLayout,LinkNumber,BackPos,optional Options)
    #local _options = #ifdef(Options) Options; #else ChainLayout.default_options; #end
    
    #local _z_rot   = _options.link_z_rot_fn(LinkNumber,BackPos.x,BackPos.y,BackPos.z);
    
    _z_rot
#end

// End Chain_layout_select_link_rot
//-----------------------------------------------------------------------------
 
//-----------------------------------------------------------------------------
// Chain_layout_select_link_xoffset(ChainLayout,LinkNumber,BackPos,optional Options)
//
#macro Chain_layout_select_link_xoffset(ChainLayout,LinkNumber,BackPos,optional Options)
    #local _options = #ifdef(Options) Options; #else ChainLayout.default_options; #end
    
    #local _x_offset   = _options.link_x_offset_fn(LinkNumber,BackPos.x,BackPos.y,BackPos.z);
    
    _x_offset
#end

// End Chain_layout_select_link_xoffset
//-----------------------------------------------------------------------------
 
//-----------------------------------------------------------------------------
// Chain_layout_select_link_yoffset(ChainLayout,LinkNumber,BackPos,optional Options)
//
#macro Chain_layout_select_link_yoffset(ChainLayout,LinkNumber,BackPos,optional Options)
    #local _options = #ifdef(Options) Options; #else ChainLayout.default_options; #end
    
    #local _y_offset   = _options.link_y_offset_fn(LinkNumber,BackPos.x,BackPos.y,BackPos.z);
    
    _y_offset
#end

// End Chain_layout_select_link_yoffset
//-----------------------------------------------------------------------------
 
//-----------------------------------------------------------------------------
// Chain_layout_select_link_zoffset(ChainLayout,LinkNumber,BackPos,optional Options)
//
#macro Chain_layout_select_link_zoffset(ChainLayout,LinkNumber,BackPos,optional Options)
    #local _options = #ifdef(Options) Options; #else ChainLayout.default_options; #end
    
    #local _z_offset   = _options.link_z_offset_fn(LinkNumber,BackPos.x,BackPos.y,BackPos.z);
    
    _z_offset
#end

// End Chain_layout_select_link_zoffset
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_select_link_offset(ChainLayout,LinkNumber,BackPos,optional Options)
//
#macro Chain_layout_select_link_offset(ChainLayout,LinkNumber,BackPos,optional Options)
    #local _options = #ifdef(Options) Options; #else ChainLayout.default_options; #end
    
    #local _offset  = <
        _options.link_x_offset_fn(LinkNumber,BackPos.x,BackPos.y,BackPos.z),
        _options.link_y_offset_fn(LinkNumber,BackPos.x,BackPos.y,BackPos.z),
        _options.link_z_offset_fn(LinkNumber,BackPos.x,BackPos.y,BackPos.z)
    >;
    
    _offset
#end

// End Chain_layout_select_link_offset
//-----------------------------------------------------------------------------
 
//-----------------------------------------------------------------------------
// Chain_layout_create(ChainSpec,optional ChainStartLoc, optional DefaultOptions)
//
//      Creates a dictionary{} containing:
//      .chain_spec:    ChainSpec,
//      .link_rot_fn:   LinkRotFn (optional)
//      .num_links:     0,
//      .links:         array // will contain instances of Chain_layout_link_create() in order
//
#macro Chain_layout_create(ChainSpec,optional ChainStartLoc, optional DefaultOptions)
    #local _chain_loc   = #ifdef(ChainStartLoc) ChainStartLoc; #else <0,0,0>; #end
    #local _layout  = dictionary {
        .chain_spec:    ChainSpec,
        .chain_loc:     _chain_loc,
        .default_options:   #ifdef(DefaultOptions) DefaultOptions, #else Chain_layout_options_create(,,,,), #end
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
    #local _verbose = #ifdef(Chain_layout_verbose) Chain_layout_verbose; #else 0; #end
    #if(_verbose > 2) Chain_layout_link_dump("Chain_layout_add_link: ", LinkLayout) #end
    #declare ChainLayout.links[ChainLayout.num_links]   = LinkLayout;
    #declare ChainLayout.num_links                      = ChainLayout.num_links+1;
#end

// End Chain_layout_add_link
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_render(ChainLayout)
//
#macro Chain_layout_render(ChainLayout)
    #if (ChainLayout.num_links = 0)
        #error concat("Chain_layout_render: No links in the chain layout\n")
    #elseif (ChainLayout.num_links = 1)
        #local _chain   = object { Chain_layout_link_render(ChainLayout.links[0],0) }
    #else
        #local _chain   = union {
            #for (i, 0, ChainLayout.num_links-1, 1)
            object { Chain_layout_link_render(ChainLayout.links[i],i) }
            #end
        }
    #end
    
    _chain    
#end

// End Chain_layout_render
//-----------------------------------------------------------------------------

// End Chain Layout Macros
//=============================================================================

//=============================================================================
// Chain Layout Construction Macros

//-----------------------------------------------------------------------------
// Chain_layout_add_link_to_end(ChainLayout,BackOffset,LinkOrientation,optional Options,optional LinkSpec, optional LinkObj)
//
//      This macro creates a new Chain_layout_link dictionary object that
//      is located at the current end of the chain and is pointing in the
//      given LinkOrientation.
//
//      If LinkSpec is not defined, it is determined using the link spec
//      selection function defined in Options, or ChainLayout.default_options,
//      or the final default of iterating over them with successive links
//
//      The back location for the new link is the front loc of the last link,
//      or the chain position if no links have been created yet.  The offset
//      from that back location is calculated using Options.link_z_offset_fn
//      or ChainLayout.default_options.link_z_offset_fn.
//
//      The Z rotation of the link is calculated using Options.link_z_rot_fn
//      or ChainLayout.default_options.link_z_rot_fn.
//
//      The resulting ChainLink object is created using Chain_layout_link_create_oriented
//      with the selected LinkSpec, back location, Z rotation, and LinkOrientation.
//
//      If LinkObj is supplied, it is used as well, otherwise it will default
//      using the rules defined in Chain_layout_link_create_oriented.
//
#macro Chain_layout_add_link_to_end(ChainLayout,BackOffset,LinkOrientation,optional Options, optional LinkSpec, optional LinkObj)
    #local _verbose         = #ifdef(Chain_layout_verbose) Chain_layout_verbose; #else 0; #end
    #local _link_num        = ChainLayout.num_links;
    #local _last_loc        = #if (ChainLayout.num_links = 0) ChainLayout.chain_loc; #else ChainLayout.links[ChainLayout.num_links-1].link_front; #end
    #local _back_loc        = _last_loc + BackOffset;
    #local _link_spec       = #ifdef(LinkSpec) LinkSpec; #else Chain_layout_select_link_spec(ChainLayout,_link_num,_back_loc,Options); #end
    #local _link_rot        = Chain_layout_select_link_rot(ChainLayout,_link_num,_back_loc,Options);
    
    #local _link            = Chain_layout_link_create_oriented(_link_spec, LinkObj, _link_rot, _back_loc, LinkOrientation);
    
    Chain_layout_add_link(ChainLayout,_link)
#end      

// End Chain_layout_add_link_to_end
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_compute_abs_offset(Forward,RelOffset)
//
#macro Chain_layout_compute_abs_offset(Forward,RelOffset)
    #local _rel_offset  = <1,1,1>*RelOffset;
    #if (vlength(vcross(y,Forward)) = 0)
        #local _right   = x;
    #else
        #local _right   = VPerp_To_Plane(y,Forward);
    #end
    #local _up          = VPerp_To_Plane(Forward,_right);
    #local _offset      = _rel_offset.x*_right + _rel_offset.y*_up + _rel_offset.z*Forward;
    
    _offset
#end

// End Chain_layout_compute_abs_offset
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_last_link_orientation(ChainLayout)
//
#macro Chain_layout_last_link_orientation(ChainLayout)
    #local _orientation = #if(ChainLayout.num_links = 0) z; #else vnormalize(ChainLayout.links[ChainLayout.num_links-1].link_front - ChainLayout.links[ChainLayout.num_links-1].link_back); #end
    
    _orientation
#end

// End Chain_layout_last_link_orientation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_add_segment(ChainLayout,EndPoint,optional Options)
//
//      This macro adds links to the given ChainLayout pointing towards EndPoint
//  such that the back location of the last link added precedes the EndPoint,
//  and the front location is at or beyond it.  To determine where the actual
//  end of the chain is located, check ChainLayout.links[ChainLayout.num_links-1]
//
//  The LinkSpec is selected using the link spec selection function in Options
//  or ChainLayout.default_options.  The LinkObject will be created using Chain_layout_link_obj_create
//  or the default object in the selected LinkSpec.
//
//  The LinkOrientation is the direction from the current endpoint (or chain_loc)
//  of the ChainLayout to the EndPoint.
//
#macro Chain_layout_add_segment(ChainLayout,EndPoint,optional Options)
    #local _verbose         = #ifdef(Chain_layout_verbose) Chain_layout_verbose; #else 0; #end
    #local _link_num        = ChainLayout.num_links;
    #local _first_loc       = #if (_link_num = 0) ChainLayout.chain_loc; #else ChainLayout.links[_link_num-1].link_front; #end
    
    #local _orientation     = Chain_layout_last_link_orientation(ChainLayout);
    #local _rel_offset      = Chain_layout_select_link_offset(ChainLayout,ChainLayout.num_links,_first_loc,Options);
    #local _offset          = Chain_layout_compute_abs_offset(_orientation,_rel_offset);
    #local _first_loc       = _first_loc + _offset;
    #local _last_loc        = _first_loc;
    
    // Since the entire segment follows the same orientation vector, we calculate the coordinate system once
    // and use it to calculate the offset values.
    //
    #local _orientation = vnormalize(EndPoint - _last_loc);
    #local _length      = vlength(EndPoint - _first_loc);
    #local _cur_length  = 0;
    
    #while (_cur_length < _length)
        #local _rel_offset  = Chain_layout_select_link_offset(ChainLayout,ChainLayout.num_links,_last_loc,Options);
        #local _offset      = Chain_layout_compute_abs_offset(_orientation,_rel_offset);
        #if (_verbose > 2) #debug concat("Chain_layout_add_segment: _offset=<", vstr(3, _offset, ",", 0, 3), ">\n") #end
        Chain_layout_add_link_to_end(ChainLayout,_offset,_orientation,Options,,)
        #local _cur_length  = vlength(ChainLayout.links[ChainLayout.num_links-1].link_front - _first_loc);
        #local _last_loc    = ChainLayout.links[ChainLayout.num_links-1].link_front;
    #end
    
#end

// End Chain_layout_add_segment
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_add_spline_segment(ChainLayout,Spline,Length,optional Options)
//
//  This macro adds links to the given ChainLayout, finding the front location
//  by querying the given Spline at a distance reflecting the links position
//  in the spline segment and its internal length.  The spline should be defined
//  to return the last position in the current chain at distance 0.
//
//  The LinkSpec is selected using the link spec selection function in Options
//  or ChainLayout.default_options.  The LinkObject will be created using Chain_layout_link_obj_create
//  or the default object in the selected LinkSpec.
//
#macro Chain_layout_add_spline_segment(ChainLayout,Spline,Length,optional Options)
    #local _last_loc    = #if (ChainLayout.num_links = 0) ChainLayout.chain_loc; #else ChainLayout.links[ChainLayout.num_links-1].link_front; #end
    
    #local _cur_len     = 0;
    
    #while (_cur_len < Length)
        #local _link_spec   = Chain_layout_select_link_spec(ChainLayout,ChainLayout.num_links,_last_loc,Options);
        #local _rel_offset  = Chain_layout_select_link_offset(ChainLayout,ChainLayout.num_links,_last_loc,Options);
        #local _last_orient = Chain_layout_last_link_orientation(ChainLayout);
        #local _offset      = Chain_layout_compute_abs_offset(_last_orient,_rel_offset);
        
        #local _back_loc    = _last_loc + _offset;

        #local _cur_len     = _cur_len + _link_spec.link_inner.y; // + _rel_offset.z;
        #local _link_front   = Spline(_cur_len) + _offset;
        #local _orientation = vnormalize(_link_front - _back_loc);
        
        Chain_layout_add_link_to_end(ChainLayout,_offset,_orientation,Options,,)
        
        #local _last_loc    = ChainLayout.links[ChainLayout.num_links-1].link_front;
    #end
#end

// End Chain_layout_add_spline_segment
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_add_slack_segment(ChainLayout,EndPt,SlackLength,optional Options)
//
#macro Chain_layout_add_slack_segment(ChainLayout,EndPt,SlackLength,optional Options)
    // Create a quadratic spline between the front of the lat link in the chain layout and the EndPt,
    // with an estimated sag point in the middle to cause the length of the chain to be SlackLength.
    //
    #local _pt0             = #if (ChainLayout.num_links = 0) ChainLayout.chain_loc; #else ChainLayout.links[ChainLayout.num_links-1].link_front; #end
    #local _pt0_sdist       = 0;
    
    #local _pt0_end         = EndPt - _pt0;
    #local _pt0_end_dist    = vlength(_pt0_end);
    
    #local _slack_len       = max(SlackLength,_pt0_end_dist);
    #local _slack_a         = _slack_len/2;
    #local _slack_h         = vlength(_pt0_end*<1,0,1>);
    #local _slack_v         = 1.1*sqrt(_slack_a*_slack_a - _slack_h*_slack_h/4);    // 1.1 mult accounts for curvature
    
    #if (_pt0.y < EndPt.y)
    #local _mid             = EndPt.y - _pt0_end/4;
    #else                                          
    #local _mid             = _pt0 + _pt0_end/4;
    #end
    #local _ptmid           = _mid - <0, (_slack_v - abs(_pt0_end.y))/2, 0>;
    #local _ptmid_dist      = vlength(_ptmid - _pt0); 
    
    // Use the a distance adjusted version of the spline to add a spline segment of SlackLength to the
    // chain
    //
    #ifdef(Chain_layout_verbose)
        #debug concat("Chain_layout_add_slack_segment:\n")
        #debug concat("   ", str(_pt0_sdist, 0, 3), ", <", vstr(3, _pt0, ",", 0, 3), ">\n")
        #debug concat("   ", str(_ptmid_dist, 0, 3), ", <", vstr(3, _ptmid, ",", 0, 3), ">\n")
        #debug concat("   ", str(_slack_len, 0, 3), ", <", vstr(3, EndPt, ",", 0, 3), ">\n")
    #end
    #local _spline  = spline {
        quadratic_spline
        _pt0_sdist, _pt0,
        _ptmid_dist, _ptmid,
        _slack_len, EndPt
    }
    #local _dspline_len = 0;
    #local _dspline     = Spline_create_distance_spline(_spline,"quadratic_spline",0.0,_slack_len+1,0.05,,,,_dspline_len)
    Chain_layout_add_spline_segment(ChainLayout,_dspline,_dspline_len,Options)
#end

// End Chain_layout_add_slack_segment
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_add_catenary_segment(ChainLayout,EndPt,optional CatenaryA, optional Options)
//
//  Parts of this are borrowed from the RopeSys (rope_sys.inc) implementation
//  of a catenary and adapted for use with chain links.
//
#macro Chain_layout_compute_catenary_a(Length,Distance,Tolerance)
    #local _cur_a       = 1.0;
    #local _cur_delta   = 2*Tolerance;
    
    #while (abs(_cur_delta) > Tolerance)
        #local _cur_L       = 2*_cur_a*sinh(Distance/(2*_cur_a));
        #local _cur_delta   = Length - _cur_L;
        #if (abs(_cur_delta) > Tolerance)
            #if (_cur_delta < 0)
                #local _cur_a   = 1.5*_cur_a;
            #else                            
                #local _cur_a   = _cur_a/2;
            #end
        #end
    #end
    
    _cur_a
    
#end

#macro Chain_layout_add_catenary_segment(ChainLayout,EndPt,optional CatenaryA, optional Options)
    #local _catenary_a  = #ifdef(CatenaryA) CatenaryA; #else 1.0; #end
    #local _verbose     = #ifdef(Chain_layout_verbose) Chain_layout_verbose; #else 0; #end
    
    #local _start_pos       = #if(ChainLayout.num_links = 0) ChainLayout.chain_loc; #else ChainLayout.links[ChainLayout.num_links-1].link_front; #end
    #local _start_hproj     = _start_pos*<1,0,1>;
    #local _segment_disp    = EndPt - _start_pos;
    #local _horizontal_disp = vlength(_segment_disp*<1,0,1>);
    #local _vertical_disp   = abs(_segment_disp.y);
    #local _horizontal_dir  = vnormalize(_segment_disp*<1,0,1>);
    
    // See rope_sys.inc: Rope_Catenary for an explanation of the iterative method
    // for determining the best fit along a catenary with the computed characteristics
    // between _start_pos and EndPt.
    //
    #local _chain_x         = 0;
    #local _chain_y         = _catenary_a*(cosh((_chain_x+_horizontal_disp)/_catenary_a)-cosh(_chain_x/_catenary_a));
    #if (_verbose > 1) #debug concat("Chain_layout_add_catenary_segment: _chain_x=", str(_chain_x, 0, 3), "; _chain_y=", str(_chain_y, 0, 3), "\n") #end    
    #local _chain_dir       = 1;
    #if (_chain_y > _vertical_disp) #local _chain_dir = -_chain_dir; #end
    #local _chain_accuracy  = 0.001;
    #local _chain_delta     = _horizontal_disp/10;
    
    #while (abs(_chain_y - _vertical_disp) > _chain_accuracy)
        #local _chain_x = _chain_x + _chain_delta*_chain_dir;
        #local _chain_y = _catenary_a*(cosh((_chain_x+_horizontal_disp)/_catenary_a)-cosh(_chain_x/_catenary_a));
        #if (_verbose > 1) #debug concat("Chain_layout_add_catenary_segment: _chain_x=", str(_chain_x, 0, 3), "; _chain_y=", str(_chain_y, 0, 3), "\n") #end    
        #if ((_chain_y > _vertical_disp & _chain_dir>0) | (_chain_y<_vertical_disp & _chain_dir < 0))
            #local _chain_dir   = -_chain_dir;
            #local _chain_delta = _chain_delta/10;
        #end
    #end
    
    
    // Traverse the horizontal line between the two points by the internal length of each link in sequence
    // and find the catenary point, then adjust to ensure the new point is exactly the internal length
    // of the link and project the resulting point back up onto the horizontal line to compute the next point.
    //
    #if (_start_pos.y > EndPt.y)
        #local _chain_x = -(_horizontal_disp+_chain_x);
    #end
    #local _chain_y = _catenary_a*(cosh(_chain_x/_catenary_a)-1);
    #local _last_loc    = _start_pos;

    #if (_verbose)
        #debug concat("Chain_layout_add_catenary_segment: _chain_x=", str(_chain_x, 0, 3), "; _chain_y=", str(_chain_y, 0, 3), "\n")
    #end    
    
    #local _chain_xdisp = 0;
    #local _links_added = 0;
    #while (_chain_xdisp < _horizontal_disp)
        #local _link_spec   = Chain_layout_select_link_spec(ChainLayout,ChainLayout.num_links,_last_loc,Options);
        #local _rel_offset  = Chain_layout_select_link_offset(ChainLayout,ChainLayout.num_links,_last_loc,Options);
        #local _last_orient = Chain_layout_last_link_orientation(ChainLayout);
        #local _offset      = Chain_layout_compute_abs_offset(_last_orient,_rel_offset);
        
        #local _link_xdisp  = _chain_xdisp+_link_spec.link_inner.y + _rel_offset.z;
        #local _link_yval   = _catenary_a*(cosh((_chain_x+_link_xdisp)/_catenary_a)-1);
        #local _link_ydisp  = _link_yval-_chain_y;
        #local _link_front  = _start_pos + _horizontal_dir*_link_xdisp + y*_link_ydisp + _offset;
        #local _link_dir    = vnormalize(_link_front - _last_loc);
        
        // If we really wanted to be fancy, we could do some iterative recalculation over _chain_xdisp
        // values until we got close enough to the _link_length matching the internal length of the
        // selected link spec.  For now, I won't bother.
        //
        #if (_verbose > 1)
            #debug concat("Chain_layout_add_catenary_segment: disp: ", str(_chain_xdisp,0,3), " link_back=<", vstr(3,_last_loc,",", 0, 3), ">; link_dir=<", vstr(3, _link_dir, ",", 0, 3), ">\n")
        #end
        
        #local _offset      = Chain_layout_compute_abs_offset(_link_dir,_rel_offset);
        #if (_verbose > 2) #debug concat("Chain_layout_add_segment: _offset=<", vstr(3, _offset, ",", 0, 3), ">\n") #end
        
        Chain_layout_add_link_to_end(ChainLayout,_offset,_link_dir,Options,,)
        #local _links_added = _links_added + 1;
        
        // See where the link actually ended
        //
        #local _last_loc    = ChainLayout.links[ChainLayout.num_links-1].link_front;
        #local _last_hproj  = _last_loc*<1,0,1>;
        #local _chain_xdisp = vlength(_last_hproj - _start_hproj);
        
    #end
    
    #if (_verbose) #debug concat("Chain_layout_add_catenary_segment: added ", str(_links_added, 0, 0), " links.\n") #end    

#end

// End Chain_layout_add_catenary_segment
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_add_catenary_segment2(ChainLayout,EndPt,ChainLength,optional Tolerance, optional Options)
//
//  Utility version of catenary code that automatically determines the A
//  parameter for the catenary based on a chain length that is hanging between the
//  two end points.
//
#macro Chain_layout_add_catenary_segment2(ChainLayout,EndPt,ChainLength,optional Tolerance, optional Options)
    #local _start_pos       = #if(ChainLayout.num_links = 0) ChainLayout.chain_loc; #else ChainLayout.links[ChainLayout.num_links-1].link_front; #end 
    #local _tolerance       = #ifdef(Tolerance) Tolerance; #else 0.001; #end
    #local _distance        = vlength(EndPt - _start_pos);
    #local _chain_length    = max(ChainLength,_distance);
    #local _catenary_a      = Chain_layout_compute_catenary_a(ChainLength,_distance,_tolerance);
    
    Chain_layout_add_catenary_segment(ChainLayout,EndPt,_catenary_a,Options)
#end

// End Chain_layout_add_catenary_segment2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Chain_layout_add_coil_segment(ChainLayout,CoilAxis,CoilPt,Revolutions,optional Offset, optional Options)
//
//  Offset is a signed multiple of the width of a chain link.  If abs(Offset) < 1.0 and Revolutions > 1.0
//  you will get overlaps of links.
//
#macro Chain_layout_add_coil_segment(ChainLayout,CoilAxis,CoilPt,Revolutions,optional Offset, optional Options)
    #local _offset_mult             = #ifdef(Offset) Offset; #else 1.0; #end
    #local _back_loc                = #if (ChainLayout.num_links = 0) ChainLayout.chain_loc; #else ChainLayout.links[ChainLayout.num_links-1].link_front; #end
    
    #local _coil_axis               = vnormalize(CoilAxis);
    #local _coil_perpendicular_pt   = CoilPt + VProject_Axis(_back_loc - CoilPt, _coil_axis);
    #local _coil_radius             = vlength(_coil_perpendicular_pt - _back_loc);
    
    #local _max_rot                 = 360*Revolutions;
    #local _cur_rot                 = 0;
    #while (_cur_rot <= _max_rot)
        #local _link_spec   = Chain_layout_select_link_spec(ChainLayout,ChainLayout.num_links,_back_loc,Options);
        #local _rel_offset  = Chain_layout_select_link_offset(ChainLayout,ChainLayout.num_links,_back_loc,Options);
        #local _last_orient = Chain_layout_last_link_orientation(ChainLayout);          
        
        // TODO: Using this as the offset for adding to the end of the link is incorrect.  It needs to be
        // computed outside of that to set the back loc and front loc correctly.
        //
        #local _offset      = Chain_layout_compute_abs_offset(_last_orient,_rel_offset);

        #local _link_len    = _link_spec.link_inner.y;
        #local _link_arc    = degrees(_link_len/_coil_radius);
        #local _link_arc    = degrees(ChordRadius2Arc(_link_len,_coil_radius));
        #local _link_offset = _offset_mult * _link_spec.link_size.x;
        
        #local _link_front   = CoilPt + _coil_axis*_link_offset*_link_arc/360 + vaxis_rotate(_back_loc - CoilPt, _coil_axis, _link_arc);
        #local _orientation = vnormalize(_link_front - _back_loc);
        #ifdef (Chain_layout_verbose) #debug concat("Chain_layout_add_coil_segment: _back_loc=<", vstr(3, _back_loc, ",", 0, 2), ">; _link_front=<", vstr(3, _link_front, ",", 0, 2), ">\n") #end
        Chain_layout_add_link_to_end(ChainLayout,_offset,_orientation,Options,,)
        
        #local _back_loc    = _link_front;
        #local _cur_rot     = _cur_rot + _link_arc;
    #end
        
#end

// End Chain_layout_add_coil_segment
//-----------------------------------------------------------------------------

// End Chain Layout Construction Macros
//=============================================================================

#version Libchain_Inc_Temp;

#end // ifndef (Libchain_Inc_Temp)
