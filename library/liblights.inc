/******************************************************************************
 * File: liblights.inc
 * Description:
 *      FILL IN DESCRIPTION HERE
 ******************************************************************************/

#ifndef(Liblights_Inc_Temp)

#declare Liblights_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including liblights.inc\n"
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
#include "CIE.inc"
#include "Lightsys.inc"
#include "lightsys_constants.inc"

// End Required Include Files
//=============================================================================

//=============================================================================
// Lightsys Utilities
//

//-----------------------------------------------------------------------------
// Lights_spotlight(Lct,Lm,Sz,Cnt,PointAt,Radius,Falloff,Tightness)
//
// Creates a spotlight that may also be an area light, specifying the area
// light axes as X*Sz and Z*Sz with the same light count in each direction
// and rotated to the PointAt, which should be relative to the light position
//
#macro Lights_spotlight(Lct,Lm,Sz,Cnt,PointAt,Radius,Falloff,Tightness)
    #local _light   = light_source {
        0
        Light_Color(Lct,Lm)
        spotlight radius Radius falloff Falloff tightness Tightness point_at PointAt
        //Light_Spot_Cos_Falloff(PointAt)
        #if (Cnt > 1)
            #local _ala1    = Sz*x;
            #local _ala2    = Sz*z;
            #local _len     = sqrt(PointAt.z*PointAt.z+PointAt.x*PointAt.x);
            #local _xrot    = #if(PointAt.y = 0 & _len = 0) 0; #else -degrees(atan2(_len,-PointAt.y)); #end
            #local _yrot    = #if(PointAt.x = 0 & PointAt.z = 0) 0; #else degrees(atan2(PointAt.x,PointAt.z)); #end
            
            #debug concat("Lights_spotlight: PointAt=<", vstr(3, PointAt, ",", 0, 3), ">\n")
            #debug concat("Lights_spotlight: _xrot=", str(_xrot, 0, 3), "; _yrot=", str(_yrot, 0, 3), "\n")
            
            #local _ala1    = vrotate(_ala1, <_xrot, _yrot, 0>);
            #local _ala2    = vrotate(_ala2, <_xrot, _yrot, 0>);
            #debug concat("Lights_Spotlight: _ala1=<", vstr(3, _ala1, ",", 0, 3), ">\n")
            #debug concat("Lights_Spotlight: _ala2=<", vstr(3, _ala2, ",", 0, 3), ">\n")
            
            // TODO: Rotate the axes so the normal of the plane defining
            // the area light points in the right direction
            area_light _ala1,_ala2,Cnt,Cnt
            jitter
            adaptive 1
        #end
        Light_Fading()
    }
    
    _light
#end

// End Supporting Library Component
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_config_create(Lct,Lm)
//
#macro Light_config_create(Lct,Lm)
    #local _cfg = dictionary {
        .light_color:   Lct,
        .light_lumens:  Lm,
        .area_axis1:    x,
        .area_axis2:    z,
        .area_size1:    0,
        .area_size2:    0,
        .area_adaptive: 0,
        .area_jitter:   false,
        .area_orient:   false,
        .area_circular: false,
        .sine_falloff:  false
    }
    ;
    
    _cfg
#end

// End Light_config_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_advanced(Cfg)
//
#macro Light_advanced(Cfg)
    #local _light   = light_source {
        0,
        Light_Color(Cfg.light_color,Cfg.light_lumens)
        
        #if (Cfg.sine_falloff)
        Light_Spot_Cos_Falloff(vcross(Ala1,Ala2))
        #end
        
        // area light?
        #if (Cfg.area_size1+Cfg.area_size2>0)
            area_light Cfg.area_axis1,Cfg.area_axis2,Cfg.area_size1,Cfg.area_size2
            #if (Cfg.area_jitter)
            jitter
            #end
            #if (Cfg.area_adaptive >= 0)
            adaptive Cfg.area_adaptive
            #end
            #if (Cfg.area_circular)
            circular
            #end
            #if (Cfg.area_orient)
            orient
            #end
        #end

        Light_Fading()
    }
    
    _light
#end

// End Light_advanced
//-----------------------------------------------------------------------------

// End Lightsys Utilities
//=============================================================================

#version Liblights_Inc_Temp;
#end // ifndef(Liblights_Inc_Temp)