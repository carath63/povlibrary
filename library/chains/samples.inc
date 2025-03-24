/******************************************************************************
 * File: samples.inc
 * Description:
 *      Simple sample chain and link definitions for use with libchain.inc
 ******************************************************************************/

#ifndef(Samples_Inc_Temp)

#declare Samples_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including samples.inc\n"
#end                     

//=============================================================================
// Includes
//

//-----------------------------------------------------------------------------
// Standard Includes
// 
#include "rc3metal.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
#include "libchain.inc"

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
// Simple_silver_chain
//

//-----------------------------------------------------------------------------
// Simple_silver_chain_link_shape(Length,Width,Radius)
//
//  Creates an un-textured link object with the given dimensions.
//
#macro Simple_silver_chain_link_shape(Length,Width,Radius)
    #local _t_major = Width/2 - Radius;
    #local _c_len   = Length - 2*(_t_major + Radius);
    
    #local _link_obj    = union {
        difference {
            torus {
                _t_major Radius
            }
            plane { z, 0 }
            translate <0, 0, _c_len/2>
        }
        difference {
            torus {
                _t_major Radius
            }
            plane { -z, 0 }
            translate <0, 0, -_c_len/2>
        }
        difference {
            cylinder {
                <0, 0, -_c_len/2>,
                <0, 0, _c_len/2>,
                Radius
            }
            box {
                <-(Radius+1), -(Radius+1), -0.025>,
                <(Radius+1), (Radius+1), 0.025>
            }        
            translate <_t_major, 0, 0>
        }
        cylinder {
            <0, 0, -_c_len/2>,
            <0, 0, _c_len/2>,
            Radius
            translate <-_t_major, 0, 0>
        }                
        translate <0, 0, Length/2 - 2*Radius>    
    }
    
    _link_obj
    
#end

// End Simple_silver_chain_link_shape
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Simple_silver_chain_spec_create(Length,Width,Radius)
// 
//  Creates a Chain_link_spec for a simple chain with torus ends and cylindrical
//  sides.  The Torus and Cylinder radius is given by Radius.  The Width is the
//  outside width (X-axis) of a single link of the chain, and the Length is
//  the outside length (Z-axis) of a single link.
//
//  The link spec is created with an instance of Simple_silver_chain_link_shape()
//  and applies a metallic silver material to it.
//
//  If you want to use other materials, define the macro Chain_layout_link_obj_create(LinkLayout,LinkNum).
//  You can call Simple_silver_chain_spec_create(LinkLayout.link_size.z,LinkLayout.link_size.x,LinkLayout.link_size.y/2 - or LinkLayout.link_radius)
//  to create the link object
//
#macro Simple_silver_chain_spec_create(Length,Width,Radius)
    #local _link_obj    = object {
        Simple_silver_chain_link_shape(Length,Width,Radius)
        material {
            texture { RC3Metal(RC3M_C_SILVER, 0.6, 0.4) }
            interior { ior 1.5 }
        }
    }
    
    #local _ir          = <Width - 4*Radius, Length - 4*Radius>;
    #local _link_spec   = Chain_link_spec_create("Link",max_extent(_link_obj) - min_extent(_link_obj),_ir,_link_obj);
    #local _link_spec.link_radius   = Radius;
    #local _chain_spec  = Chain_spec_create(_link_spec,);
    
    _chain_spec
#end

// End Simple_silver_chain_spec_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Simple_silver_chain_rest_angle(LinkSpec)
//
//      Given a LinkSpec for the Simple_silver_chain, compute the resting
//  angle of a vertical link with a horizontal link.  If the chain is resting
//  on a solid flat surface, the z_rot_function should be rest_angle/2 for
//  the horizontal links and -rest_angle/2 for vertical links.
//
#macro Simple_silver_chain_rest_angle(LinkSpec)                 
    #local _radius      = #ifdef(LinkSpec.link_radius) LinkSpec.link_radius; #else LinkSpec.link_size.y/2; #end
    #local _rest_angle  = 90 - degrees(atan2(LinkSpec.link_inner.x/2+_radius,_radius));
    
    (_rest_angle)
#end

// End Simple_silver_chain_rest_angle
//-----------------------------------------------------------------------------

// End Simple_silver_chain
//=============================================================================

#version Samples_Inc_Temp;

#end // ifndef (Samples_Inc_Temp)
