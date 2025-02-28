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
#include "libscale.inc"

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
        #debug concat("Lights_spotlight: PointAt=<", vstr(3, PointAt, ",", 0, 3), ">\n")
        #if (Cnt > 1)
            #local _ala1    = Sz*x;
            #local _ala2    = Sz*z;
            #local _len     = sqrt(PointAt.z*PointAt.z+PointAt.x*PointAt.x);
            #local _xrot    = #if(PointAt.y = 0 & _len = 0) 0; #else -degrees(atan2(_len,-PointAt.y)); #end
            #local _yrot    = #if(PointAt.x = 0 & PointAt.z = 0) 0; #else degrees(atan2(PointAt.x,PointAt.z)); #end
            
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

//=============================================================================
// Light Config Utilities
//
// A Light configuration defines everything that can be used to create a
// light_source object, and can be passed to the Light_source_create(Cfg)
// macro to actually create the light_source object.
//
// The usefulness of this is that it provides a number of utilities for
// defining different types of lights using the varous modifiers allowed
// within the light_source SDF definition.  It also takes advantage of code
// provided by the LightSys macros to deal with realistic light fading and
// color generation.
//
// A minimal Light Config requires a location (default <0,0,0>) and a color
//
#declare Light_color_type_simple    = 0;
#declare Light_color_type_lightsys  = 1; 

//-----------------------------------------------------------------------------
// Light_lightsys_config_create(optional Scale, optional Filter, optional Brightness)
//
#macro Light_lightsys_config_create(optional Scale, optional Filter, optional Brightness)
    #local _scale   = #ifdef(Scale) Scale; #else Math_Default_Scale; #end
    
    #local _ls_cfg  = dictionary {
        .scene_scale:   _scale,
        #ifdef(Filter)
        .filt:          Filter,
        #end
        #ifdef(Brightness)
        .bright:        Brightness
        #end
    }
    ;
    
    _ls_cfg
#end

// End Light_lightsys_config_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_color_config_create_simple(Color)
//
#macro Light_color_config_create_simple(Color)
    #local _color_cfg   = dictionary {
        .color_type:    Light_color_type_simple,
        .color_value:   Color
    }
    ;
    
    _color_cfg
#end

// End Light_color_config_create_simple
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_color_config_create_lightsys(Lct,Lmn,optional Flt)
//
#macro Light_color_config_create_lightsys(Lct,Lmn,optional Flt)
    #local _color_cfg   = dictionary {
        .color_type:    Light_color_type_lightsys,
        .color_value:   Lct,
        .color_lumens:  Lmn,
        #ifdef(Flt)
        .color_filter:  Flt
        #end
    }
    ;
    
    _color_cfg
#end

// End Light_color_config_create_lightsys
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_color_config_create_spectrum(Spectrum,Lmn,optional Flt)
//
#macro Light_color_config_create_spectrum(Spectrum,Lmn,optional Flt)
    #local _color_cfg   = dictionary {
        .color_type:    Light_color_type_lightsys,
        .color_value:   EmissiveSpectrum(Spectrum),
        .color_lumens:  Lmn,
        #ifdef(Flt)
        .color_filter:  Flt
        #end
    }
    ;
    
    _color_cfg
#end

// End Light_color_config_create_spectrum
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_area_config_create(Axis1,Axis2,Points1,Points2)
//
// Creates the basic components of an area light from fixed values for the
// axes and point counts.  Once the config is created, optional
// additions can be made by setting the following:
//
//      CFG.opt_adaptive {0,1,2,3,...}
//      CFG.opt_area_illumination { on,off }
//      CFG.opt_jitter
//      CFG.opt_circular
//      CFG.opt_orient
//
#macro Light_area_config_create(Axis1,Axis2,Points1,Points2)
    #local _area_cfg    = dictionary {
        .axis1: Axis1,
        .axis2: Axis2,
        .points1:   Points1,
        .points2:   Points2
    }
    ;
    
    _area_cfg
#end

// End Light_area_config_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_fading_config_lightsys_create()
//
#macro Light_fading_config_lightsys_create()
    #local _cfg = dictionary {
        .lightsys_fading:   true
    };
    
    _cfg
#end        

// End Light_fading_config_lightsys_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_fading_config_create(Distance,Power)
//
#macro Light_fading_config_create(Distance,Power)
    #local _cfg = dictionary {
        .opt_fade_distance: Distance,
        .opt_fade_power:    Power
    }
    ;
    
    _cfg
#end

// End Light_fading_config_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_area_config_create_oriented(Radius,Points)
//
#macro Light_area_config_create_oriented(Radius,Points)
    #local _area_cfg    = Light_area_config_create(2*x*Radius,2*z*Radius,Points,Points);
    #declare _area_cfg.opt_circular = true;
    #declare _area_cfg.opt_orient   = true;
    
    _area_cfg
#end

// End Light_area_config_create_oriented
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_area_config_create_point_at(Cfg,Size1,Size2,Points1,Points2)
//
#macro Light_area_config_create_point_at(Cfg,Size1,Size2,Points1,Points2)
    #ifndef (Cfg.pointat)
        #error concat("No pointat configuration supplied\n")
    #end
        
    #local _ax1     = x*Size1;
    #local _ax2     = z*Size2;
    #local _dir     = vnormalize(Cfg.pointat - Cfg.loc);
    #local _xz_len  = sqrt(pow(_dir.x,2) + pow(_dir.z,2));
    #local _z_rot   = -degrees(atan2(_xz_len,_dir.y));
    #local _y_rot   = degrees(atan2(_dir.x,_dir.z));
    #local _ax1     = vrotate(vrotate(_ax1,<0,0,_z_rot>),<0,_y_rot,0>);
    #local _ax2     = vrotate(vrotate(_ax2,<0,0,_z_rot>),<0,_y_rot,0>);
    
    Light_area_config_create(_ax1,_ax2,Points1,Points2)
#end

// End Light_area_config_create_point_at
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_config_create(ColorCfg,optional Location, optional PointAt)
//
//      This is the basic point light configuration creation, which is also
//      reused for creating spotlights and cylindrical lights.
//
//      Once the configuration is created, additional capabilities can
//      be added with additional sub-configurations:
//
//      CFG.area_light_cfg          - See Light_area_config_create*
//      CFG.fading_cfg              - See Light_fading_config_create*
//      CFG.photons_cfg             - See Light_photons_config_create
//      CFG.opt_shadowless          - { true/false }
//      CFG.opt_parallel            - { true/false }
//      CFG.opt_looks_like          - { object } - only for point and spotlights
//      CFG.opt_projected_through   - { object }
//      CFG.opt_media_interaction   - { on/off }
//      CFG.opt_media_attenuation   - { on/off }
//
//      For spotlights and cylinder lights there are additional sub-configs:
//      CFG.spotlight_cfg
//      CFG.cylinder_cfg
//
#macro Light_config_create(ColorCfg, optional Location, optional PointAt)
    #local _loc = #ifdef(Location) Location; #else <0,0,0>; #end
    
    #local _cfg = dictionary {
        .loc:       _loc,
        .clr:       ColorCfg,
        #ifdef (PointAt)
        .pointat:   PointAt,
        #end
    }
    ;
    
    _cfg
#end

// End Light_config_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_config_create_spotlight(ColorCfg, optional Location, PointAt, Radius, optional FallOff, optional Tightness)
//
#macro Light_config_create_spotlight(ColorCfg, optional Location, PointAt, Radius, optional FallOff, optional Tightness)
    #local _cfg = Light_config_create(ColorCfg,Location,PointAt);
    #local _cfg.spotlight_cfg   = dictionary {
        .opt_radius:    Radius,
        #ifdef (FallOff)
        .opt_falloff:   FallOff,
        #end
        #ifdef (Tightness)
        .opt_tightness: Tightness,
        #end
    }
    ;
    
    _cfg   
#end

// End Light_config_create_spotlight
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_config_create_cylinder(ColorCfg, optional Location, PointAt, Radius, optional FallOff, optional Tightness)
//
#macro Light_config_create_cylinder(ColorCfg, optional Location, PointAt, Radius, optional FallOff, optional Tightness)
    #local _cfg = Light_config_create(ColorCfg,Location,PointAt);
    #local _cfg.cylinder_cfg   = dictionary {
        .opt_radius:    Radius,
        #ifdef (FallOff)
        .opt_falloff:   FallOff,
        #end
        #ifdef (Tightness)
        .opt_tightness: Tightness,
        #end
    }
    ;
    
    _cfg   
#end

// End Light_config_create_cylinder
//-----------------------------------------------------------------------------

// End Light Config Utilities
//=============================================================================

//=============================================================================
// Light_source_create(Cfg)
//
// Create a light_source defined by the values set in the given Cfg dictionary
//

//-----------------------------------------------------------------------------
// Light_source_color(ColorCfg)
//
#macro Light_source_color(ColorCfg)
    #switch(ColorCfg.color_type)
        #case(Light_color_type_simple)
            #local _c   = ColorCfg.color_value;
        #break
        
        #case(Light_color_type_lightsys)
            #ifdef (ColorCfg.color_filter)
                #local _c   = Light_Color_Filtered(ColorCfg.color_value,ColorCfg.color_lumens,ColorCfg.color_filter);
            #else
                #local _c   = Light_Color(ColorCfg.color_value,ColorCfg.color_lumens);
            #end
        #break
        
        #else
        #error concat("Unsupported color type ", str(ColorCfg.color_type, 0, 0), "\n")
        #break
    #end
    
    _c
#end

// End Light_source_color
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_source_area_light(Cfg)
//
#macro Light_source_area_light(Cfg)
    #local _verbose = #ifdef(Light_source_verbose) Light_source_verbose; #else 0; #end

    #if (_verbose) #debug concat("   area_light <", vstr(3,Cfg.area_light_cfg.axis1,",",0,3), ">,<", vstr(3,Cfg.area_light_cfg.axis2,",",0,3), ">,", str(Cfg.area_light_cfg.points1, 0, 0), ",", str(Cfg.area_light_cfg.points2, 0, 0), "\n") #end        
    area_light Cfg.area_light_cfg.axis1,Cfg.area_light_cfg.axis2,Cfg.area_light_cfg.points1,Cfg.area_light_cfg.points2
    #ifdef (Cfg.area_light_cfg.opt_adaptive)
    #if (_verbose) #debug concat("   adaptive ", str(Cfg.area_light_cfg.opt_adaptive, 0, 0), "\n") #end
    adaptive Cfg.area_light_cfg.opt_adaptive
    #end
    #ifdef (Cfg.area_light_cfg.opt_area_illumination)
    #if (_verbose) #debug concat("   area_illumination ", str(Cfg.area_light_cfg.opt_area_illumination, 0, 0), "\n") #end
    area_illumination Cfg.area_light_cfg.opt_area_illumination
    #end
    #ifdef (Cfg.area_light_cfg.opt_jitter)
    #if (_verbose) #debug "   jitter\n" #end
    jitter
    #end
    #ifdef (Cfg.area_light_cfg.opt_circular)
    #if (_verbose) #debug "   circular\n" #end
    circular
    #end
    #ifdef (Cfg.area_light_cfg.opt_orient)
    #if (_verbose) #debug "   orient\n" #end
    orient
    #end
#end

// End Light_source_area_light
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_source_fading(Cfg)
//
#macro Light_source_fading(Cfg)
    #local _verbose = #ifdef(Light_source_verbose) Light_source_verbose; #else 0; #end
    #ifdef(Cfg.fading_cfg.lightsys_fading)
    #if (_verbose) #debug "   Light_Fading()\n" #end
    Light_Fading()
    #else
        #ifdef (Cfg.fading_cfg.opt_fade_distance)
        #if (_verbose) #debug concat("   fade_distance ", str(Cfg.fading_cfg.opt_fade_distance, 0, 3), "\n") #end
        fade_distance Cfg.fading_cfg.opt_fade_distance
        #end
        #ifdef (Cfg.fading_cfg.opt_fade_power)
        #if (_verbose) #debug concat("   fade_power ", str(Cfg.fading_cfg.opt_fade_power, 0, 3), "\n") #end
        fade_power Cfg.fading_cfg.opt_fade_power
        #end
    #end
#end

// End Light_source_fading
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_source_spotlight(Cfg)
//
#macro Light_source_spotlight(Cfg)
    #local _verbose = #ifdef(Light_source_verbose) Light_source_verbose; #else 0; #end
    #if (_verbose) #debug "   spotlight\n" #end
    spotlight
    #if (_verbose) #debug concat("   point_at <", vstr(3, Cfg.pointat, ",", 0, 3), ">\n") #end
    point_at Cfg.pointat
    #if (_verbose) #debug concat("   radius ", str(Cfg.spotlight_cfg.opt_radius, 0, 3), "\n") #end 
    radius Cfg.spotlight_cfg.opt_radius
    #ifdef (Cfg.spotlight_cfg.opt_falloff)
    #if (_verbose) #debug concat("   falloff ", str(Cfg.spotlight_cfg.opt_falloff, 0, 3), "\n") #end 
    falloff Cfg.spotlight_cfg.opt_falloff
    #end
    #ifdef (Cfg.spotlight_cfg.opt_tightness)
    #if (_verbose) #debug concat("   tightness ", str(Cfg.spotlight_cfg.opt_tightness, 0, 3), "\n") #end 
    tightness Cfg.spotlight_cfg.opt_tightness
    #end
#end

// End Light_source_spotlight
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Light_source_cylinder(Cfg)
//
#macro Light_source_cylinder(Cfg)
    #local _verbose = #ifdef(Light_source_verbose) Light_source_verbose; #else 0; #end
    #if (_verbose) #debug "   cylinder\n" #end
    cylinder
    #if (_verbose) #debug concat("   point_at <", vstr(3, Cfg.pointat, ",", 0, 3), ">\n") #end
    point_at Cfg.pointat
    #if (_verbose) #debug concat("   radius ", str(Cfg.cylinder_cfg.opt_radius, 0, 3), "\n") #end 
    radius Cfg.cylinder_cfg.opt_radius
    #ifdef (Cfg.cylinder_cfg.opt_falloff)
    #if (_verbose) #debug concat("   falloff ", str(Cfg.cylinder_cfg.opt_falloff, 0, 3), "\n") #end 
    falloff Cfg.cylinder_cfg.opt_falloff
    #end
    #ifdef (Cfg.cylinder_cfg.opt_tightness)
    #if (_verbose) #debug concat("   tightness ", str(Cfg.cylinder_cfg.opt_tightness, 0, 3), "\n") #end 
    tightness Cfg.cylinder_cfg.opt_tightness
    #end
#end

// End Light_source_cylinder
//-----------------------------------------------------------------------------

#macro Light_source_create(Cfg)
    #local _verbose = #ifdef(Light_source_verbose) Light_source_verbose; #else 0; #end
    // Set up any LightSys configuration parameters
    //
    #ifdef (Cfg.lightsys_cfg)
        #if (_verbose) #debug concat("#declare Lightsys_Scene_Scale = ", str(Cfg.lightsys_cfg.scene_scale, 0, 6), "\n") #end
        #declare Lightsys_Scene_Scale   = Cfg.lightsys_cfg.scene_scale;
        #ifdef (Cfg.lightsys_cfg.filt)
        #if (_verbose) #debug concat("#declare Lightsys_Filter = <", vstr(3, Cfg.lightsys_cfg.filt, ",", 0, 6), "\n") #end
        #declare Lightsys_Filter        = Cfg.lightsys_cfg.filt;
        #end
        #ifdef (Cfg.lightsys_cfg.bright)
        #if (_verbose) #debug concat("#declare Lightsys_Brightness = ", str(Cfg.lightsys_cfg.bright, 0, 6), "\n") #end
        #declare Lightsys_Brightness    = Cfg.lightsys_cfg.bright;
        #end
    #end
    
    #if (_verbose) #debug concat("light_source {\n") #end
    #local _ls  = light_source {
        #if (_verbose) #debug concat("   <", vstr(3, Cfg.loc, ",", 0, 6), ">, <", vstr(3, Light_source_color(Cfg.clr), ",", 0, 3), ">\n") #end
        Cfg.loc, Light_source_color(Cfg.clr)
        
        // Set up shadowless lighting options before any spotlight or cylinder
        // options
        #ifdef (Cfg.opt_shadowless)
            #if (Cfg.opt_shadowless)
                #if (_verbose) #debug "   shadowless\n" #end
                shadowless
            #end
        #end
        
        // Parallel light options apply to any light source
        //
        #ifdef (Cfg.opt_parallel)
            #if (Cfg.opt_parallel)
                #ifndef (Cfg.pointat)
                    #error concat("Parallel lights require a point_at value\n")
                #end
                #ifdef (Cfg.opt_looks_like)
                    #error concat("Parallel lights cannot have a looks_like value\n")
                #end
                #if (_verbose) #debug "   parallel\n" #end    
                parallel
            #end            
        #end
        
        // Set up for spotlights and cylindrical lights
        //
        #ifdef (Cfg.spotlight_cfg)         
            Light_source_spotlight(Cfg)
        #else
            #ifdef (Cfg.cylinder_cfg)
                Light_source_cylinder(Cfg)
            #end
        #end    
        
        // Put the point_at declaration here
        //
        
        // Looks like options apply to anything except parallel lights
        //
        #ifdef (Cfg.opt_looks_like)
            #if (_verbose) #debug "   looks_like { Cfg.opt_looks_like }\n" #end
            looks_like { Cfg.opt_looks_like }
        #end
        
        // Projected through applies to any light source
        //
        #ifdef (Cfg.opt_projected_through)
            #if (_verbose) #debug "   projected_through { Cfg.opt_projected_through }\n" #end
            projected_through { Cfg.opt_projected_through }
        #end        
            
        
        // Area light options apply to any light source
        //
        #ifdef (Cfg.area_light_cfg)
        Light_source_area_light(Cfg)
        #end
        
        // Fading options apply to any light source
        //
        #ifdef (Cfg.fading_cfg)
        Light_source_fading(Cfg)
        #end
        
        // Media interaction options apply to any light source
        //
        #ifdef (Cfg.opt_media_interaction) 
            #if (_verbose) #debug concat("   media_interaction ", str(Cfg.opt_media_interaction, 0, 0), "\n") #end
            media_interaction Cfg.opt_media_interaction
        #end
        #ifdef (Cfg.opt_media_attenuation)
            #if (_verbose) #debug concat("   media_attenuation ", str(Cfg.opt_media_attenuation, 0, 0), "\n") #end
            media_attenuation Cfg.opt_media_attenuation
        #end        
        
        // Photon options apply to any light source
        //
        #ifdef (Cfg.photons_cfg)
            #if (_verbose) #debug "   photons {\n" #end
            photons {
                #ifdef (Cfg.photons_cfg.opt_refraction)
                #if (_verbose) #debug concat("      refraction ", str(Cfg.photons_cfg.opt_refraction, 0, 0), "\n") #end
                refraction Cfg.photons_cfg.opt_refraction
                #end
                #ifdef (Cfg.photons_cfg.opt_reflection)
                #if (_verbose) #debug concat("      reflection ", str(Cfg.photons_cfg.opt_reflection, 0, 0), "\n") #end
                reflection Cfg.photons_cfg.opt_reflection
                #end
                #ifdef (Cfg.photons_cfg.opt_area_light)
                #if (_verbose) #debug "      area_light\n" #end
                area_light
                #end    
            }
            #if (_verbose) #debug "   }\n" #end
        #end 
    }
    #if (_verbose) #debug "}\n" #end
    
    _ls
#end

// End Light_source_create
//=============================================================================

#version Liblights_Inc_Temp;
#end // ifndef(Liblights_Inc_Temp)