/******************************************************************************
 * File: std_woodgrains.inc
 * Description:
 *      A set of predefined wood grain patterns and colormaps with tree
 *  parameters to use with the Wood_grain_cut macro
 ******************************************************************************/

#ifndef(Std_woodgrains_Inc_Temp)

#declare Std_woodgrains_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including std_woodgrains.inc\n"
#end                     

//=============================================================================
// Includes
//

//-----------------------------------------------------------------------------
// Standard Includes
//
#include "rand.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
#include "libwood.inc"

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
// White_oak
//
#declare White_oak_min_trunk_diameter   = Math_Scale(SCALE_FEET, 3);
#declare White_oak_max_trunk_diameter   = Math_Scale(SCALE_FEET, 4);
#declare White_oak_min_trunk_length     = Math_Scale(SCALE_FEET, 65);
#declare White_oak_max_trunk_length     = Math_Scale(SCALE_FEET, 85);
#declare White_oak_grain_size           = Math_Scale(SCALE_INCH, 8);
#declare White_oak_colormap_file        = "woodgrains/white-oak-colors.jpg";
#declare White_oak_colormap_start       = 0.0;
#declare White_oak_colormap_end         = 0.99;
#declare White_oak_colormap_entries     = 24;

//-----------------------------------------------------------------------------
// White_oak_grain(RStream)
//
#macro White_oak_grain(RStream)
    #local _s1      = Math_Scale(SCALE_INCH, 8);
    #local _grain   = pigment {
        wood
        color_map {
            [0.0 rgb 0]
            [1.0 rgb 1]
        }
        #local _s   = <1, 1, 0.0125>*White_oak_grain_size;
        scale _s warp { turbulence 0.2*(1+0.1*SRand(RStream)) lambda 12 omega 0.55 } scale 1/_s
        #local _s   = <0.1, 0.1, 0.01>;
        scale _s warp { turbulence 0.02*(1+0.1*SRand(RStream)) lambda 2 omega 0.6 octaves 8 } scale 1/_s
        scale <White_oak_grain_size, White_oak_grain_size, 1>
    }
    
    _grain
#end

// End White_oak_grain
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// White_oak_cmap_create()
//
#macro White_oak_cmap_create()
    Wood_cmap_create(White_oak_colormap_file,White_oak_colormap_start,White_oak_colormap_end,White_oak_colormap_entries)
#end    

// End White_oak_cmap_create
//-----------------------------------------------------------------------------

// End White_oak
//=============================================================================

//=============================================================================
// Jeffrey_pine
//
#declare Jeffrey_pine_min_trunk_diameter    = Math_Scale(SCALE_FEET, 2);
#declare Jeffrey_pine_max_trunk_diameter    = Math_Scale(SCALE_FEET, 4);
#declare Jeffrey_pine_min_trunk_length      = Math_Scale(SCALE_FEET, 100);
#declare Jeffrey_pine_max_trunk_length      = Math_Scale(SCALE_FEET, 165);
#declare Jeffrey_pine_grain_size            = Math_Scale(SCALE_INCH, 2+1/2)/3;
#declare Jeffrey_pine_colormap_file         = "woodgrains/ponderosa-pine-colors.jpg";
#declare Jeffrey_pine_colormap_start        = 0.0;
#declare Jeffrey_pine_colormap_end          = 0.99;
#declare Jeffrey_pine_colormap_entries      = 24;

//-----------------------------------------------------------------------------
// Jeffrey_pine_grain(RStream)
//
#macro Jeffrey_pine_grain(RStream)
    #local _grain   = pigment {
        wood
        color_map {
            [0.0 rgb 0]
            [1.0 rgb 1]
        }
        #local _s   = <1, 1, 0.0125>*Jeffrey_pine_grain_size;
        scale _s warp { turbulence 0.1*(1+0.1*SRand(RStream)) lambda 12 omega 0.55 } scale 1/_s
        #local _s   = <0.1, 0.1, 0.01>;
        scale _s warp { turbulence 0.05125*(1+0.1*SRand(RStream)) lambda 2 omega 0.6 octaves 8 } scale 1/_s
        rotate <1, 0, 0>
        scale <Jeffrey_pine_grain_size, Jeffrey_pine_grain_size, 1>*(1 + 0.1*SRand(RStream))
    }
    
    _grain

#end

// End Jeffrey_pine_grain
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Jeffrey_pine_cmap_create()
//
#macro Jeffrey_pine_cmap_create()
    Wood_cmap_create(Jeffrey_pine_colormap_file,Jeffrey_pine_colormap_start,Jeffrey_pine_colormap_end,Jeffrey_pine_colormap_entries)
#end    

// End Jeffrey_pine_cmap_create
//-----------------------------------------------------------------------------

// End Jeffrey_pine
//=============================================================================

//=============================================================================
// Red_pine
//
#declare Red_pine_min_trunk_diameter    = Math_Scale(SCALE_FEET, 2);
#declare Red_pine_max_trunk_diameter    = Math_Scale(SCALE_FEET, 3);
#declare Red_pine_min_trunk_length      = Math_Scale(SCALE_FEET, 65);
#declare Red_pine_max_trunk_length      = Math_Scale(SCALE_FEET, 100);
#declare Red_pine_grain_size            = Math_Scale(SCALE_INCH, 3);
#declare Red_pine_colormap_file         = "woodgrains/ponderosa-pine-colors.jpg";
#declare Red_pine_colormap_start        = 0.0;
#declare Red_pine_colormap_end          = 0.99;
#declare Red_pine_colormap_entries      = 24;

//-----------------------------------------------------------------------------
// Red_pine_grain(RStream)
//
#macro Red_pine_grain(RStream)
    #local _grain   = pigment {
        wood
        color_map {
            [0.0 rgb 0]
            [1.0 rgb 1]
        }
        #local _s   = 10*<1, 1, 0.0125>*Red_pine_grain_size;
        scale _s warp { turbulence 0.1*(1+0.1*SRand(RStream)) lambda 12 omega 0.55 } scale 1/_s
        #local _s   = <1, 1, 0.1>;
        scale _s warp { turbulence 0.02*(1+0.1*SRand(RStream)) lambda 2 omega 0.6 octaves 8 } scale 1/_s
        rotate <0.25, 0, 0>
        scale <Red_pine_grain_size, Red_pine_grain_size, 1>*(1 + 0.1*SRand(RStream))
    }
    
    _grain

#end

// End Red_pine_grain
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Red_pine_cmap_create()
//
#macro Red_pine_cmap_create()
    Wood_cmap_create(Red_pine_colormap_file,Red_pine_colormap_start,Red_pine_colormap_end,Red_pine_colormap_entries)
#end    

// End Red_pine_cmap_create
//-----------------------------------------------------------------------------

// End Red_pine 
//=============================================================================

//=============================================================================
// Black Ash
//
#declare Black_ash_min_trunk_diameter    = Math_Scale(SCALE_FEET, 1);
#declare Black_ash_max_trunk_diameter    = Math_Scale(SCALE_FEET, 2);
#declare Black_ash_min_trunk_length      = Math_Scale(SCALE_FEET, 50);
#declare Black_ash_max_trunk_length      = Math_Scale(SCALE_FEET, 65);
#declare Black_ash_grain_size            = Math_Scale(SCALE_INCH, 2);   // Based on the width of the colormap
#declare Black_ash_colormap_file        = "woodgrains/black-ash-colors.jpg";
#declare Black_ash_colormap_start       = 0.0;
#declare Black_ash_colormap_end         = 0.99;
#declare Black_ash_colormap_entries     = 24; 


//-----------------------------------------------------------------------------
// Black_ash_grain(RStream)
//
#macro Black_ash_grain(RStream)
    #local _grain   = pigment {
        wood
        color_map {
            [0.0 rgb 0]
            [1.0 rgb 1]
        }
        #local _s   = Math_Scale(SCALE_CM, <0.1, 0.1, 0.0025>);
        scale _s warp { turbulence 0.05*(1+0.1*SRand(RStream))*<1,1,1> lambda 3 omega 0.6 octaves 8 } scale 1/_s
        #local _s   = 4*Math_Scale(SCALE_CM, <1, 1, 0.0125>);
        scale _s warp { turbulence 0.2*(1+0.1*SRand(RStream)) lambda 24 omega 1.1 } scale 1/_s
        rotate <0.6+0.1*SRand(RStream), 0, 0>
        scale <Black_ash_grain_size, Black_ash_grain_size, 1>*(1 + 0.1*SRand(RStream))
    }
    
    _grain

#end

// End Black_ash_grain
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Black_ash_cmap_create()
//
#macro Black_ash_cmap_create()
    Wood_cmap_create(Black_ash_colormap_file,Black_ash_colormap_start,Black_ash_colormap_end,Black_ash_colormap_entries)
#end    

// End Black_ash_cmap_create
//-----------------------------------------------------------------------------

// End Black Ash
//=============================================================================

//=============================================================================
// Aromatic Red Cedar
//

#declare Aromatic_red_cedar_min_trunk_diameter      = Math_Scale(SCALE_FEET, 1.5);
#declare Aromatic_red_cedar_max_trunk_diameter      = Math_Scale(SCALE_FEET, 2.5);
#declare Aromatic_red_cedar_min_trunk_length        = Math_Scale(SCALE_FEET, 50);
#declare Aromatic_red_cedar_max_trunk_length        = Math_Scale(SCALE_FEET, 65);
#declare Aromatic_red_cedar_grain_size              = Math_Scale(SCALE_INCH, 2);   // Based on the width of the colormap
#declare Aromatic_red_cedar_colormap_file           = "woodgrains/aromatic-red-cedar-colors.jpg";
#declare Aromatic_red_cedar_colormap_start          = 0.0;
#declare Aromatic_red_cedar_colormap_end            = 0.99;
#declare Aromatic_red_cedar_colormap_entries        = 24;

//-----------------------------------------------------------------------------
// Aromatic_red_cedar_grain(RStream)
//
#macro Aromatic_red_cedar_grain(RStream)
    #local _grain   = pigment {
        wood
        color_map {
            [0.0 rgb 0]
            [1.0 rgb 1]
        }
        #local _s   = <0.1, 0.1, 0.0025>;
        scale _s warp { turbulence 0.1*(1+0.1*SRand(RStream))*<1,1,1> lambda 3 omega 0.6 octaves 4 } scale 1/_s
        #local _s   = 4*<1, 1, 0.0125>;
        scale _s warp { turbulence 0.1*(1+0.1*SRand(RStream)) lambda 6 omega 0.7 } scale 1/_s
        rotate <1+0.1*SRand(RStream), 0, 0>
        scale <Aromatic_red_cedar_grain_size, Aromatic_red_cedar_grain_size, 1>*(1 + 0.1*SRand(RStream))
    }
    
    _grain

#end

// End Aromatic_red_cedar_grain
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Aromatic_red_cedar_cmap_create()
//
#macro Aromatic_red_cedar_cmap_create()
    Wood_cmap_create(Aromatic_red_cedar_colormap_file,Aromatic_red_cedar_colormap_start,Aromatic_red_cedar_colormap_end,Aromatic_red_cedar_colormap_entries)
#end    

// End Aromatic_red_cedar_cmap_create
//-----------------------------------------------------------------------------

// End Aromatic Red Cedar
//=============================================================================

//=============================================================================
// Black_cherry
//
#declare Black_cherry_min_trunk_diameter    = Math_Scale(SCALE_FEET, 3);
#declare Black_cherry_max_trunk_diameter    = Math_Scale(SCALE_FEET, 5);
#declare Black_cherry_min_trunk_length      = Math_Scale(SCALE_FEET, 50);
#declare Black_cherry_max_trunk_length      = Math_Scale(SCALE_FEET, 100);
#declare Black_cherry_grain_size            = Math_Scale(SCALE_INCH, 2);
#declare Black_cherry_colormap_file         = "woodgrains/prunus-serotina-colors.jpg";
#declare Black_cherry_colormap_start        = 0.0;
#declare Black_cherry_colormap_end          = 0.99;
#declare Black_cherry_colormap_entries      = 24;

//-----------------------------------------------------------------------------
// Black_cherry_grain(RStream)
//
#macro Black_cherry_grain(RStream)
    #local _s1      = Math_Scale(SCALE_INCH, 8);
    #local _grain   = pigment {
        wood
        color_map {
            [0.0 rgb 0]
            [1.0 rgb 1]
        }
        //scale 1/_s1 warp { turbulence 0.03+0.01*SRand(RStream) lambda 2+SRand(RStream) } scale _s1       
        #local _s   = 0.5*<1, 1, 0.0125>*Black_cherry_grain_size;
        scale _s warp { turbulence 0.2*(1+0.1*SRand(RStream)) lambda 15 omega 0.7 } scale 1/_s
        #local _s   = <0.1, 0.1, 0.01>;
        scale _s warp { turbulence 0.02*(1+0.1*SRand(RStream)) lambda 2 omega 0.6 octaves 8 } scale 1/_s
        scale <Black_cherry_grain_size, Black_cherry_grain_size, 1>
    }
    
    _grain
#end

// End Black_cherry_grain
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Black_cherry_cmap_create()
//
#macro Black_cherry_cmap_create()
    Wood_cmap_create(Black_cherry_colormap_file,Black_cherry_colormap_start,Black_cherry_colormap_end,Black_cherry_colormap_entries)
#end    

// End Black_cherry_cmap_create
//-----------------------------------------------------------------------------

// End Black_cherry
//=============================================================================

//=============================================================================
// Pencil_wood
//
#declare Pencil_wood_min_trunk_diameter     = Math_Scale(SCALE_FEET, 3);
#declare Pencil_wood_max_trunk_diameter     = Math_Scale(SCALE_FEET, 5);
#declare Pencil_wood_min_trunk_length       = Math_Scale(SCALE_FEET, 50);
#declare Pencil_wood_max_trunk_length       = Math_Scale(SCALE_FEET, 100);
#declare Pencil_wood_grain_size             = Math_Scale(SCALE_INCH, 1);
#declare Pencil_wood_colormap_file          = "woodgrains/pencil-wood-colors.jpg";
#declare Pencil_wood_colormap_start         = 0.0;
#declare Pencil_wood_colormap_end           = 0.99;
#declare Pencil_wood_colormap_entries       = 24;

//-----------------------------------------------------------------------------
// Pencil_wood_grain(NSeed)
//
#macro Pencil_wood_grain(NSeed)
    #local _grain   = pigment {
        wood
        color_map {
            [0.0 rgb 0]
            [1.0 rgb 1]
        }
        #local _s   = <0.1, 0.1, 0.0025>;
        //scale _s warp { turbulence 0.1*(1+0.1*SRand(NSeed))*<1,1,1> lambda 3 omega 0.6 octaves 4 } scale 1/_s
        #local _s   = 4*<1, 1, 0.0125>;
        scale _s warp { turbulence 0.2*(1+0.1*SRand(NSeed)) lambda 6 omega 0.75 } scale 1/_s
        rotate <0.5+0.1*SRand(NSeed), 0, 0>
        scale <Aromatic_red_cedar_grain_size, Aromatic_red_cedar_grain_size, 1>*(1 + 0.1*SRand(NSeed))
    }
    
    _grain

#end

// End Pencil_wood_grain
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Pencil_wood_cmap_create()
//
#macro Pencil_wood_cmap_create()
    Wood_cmap_create(Pencil_wood_colormap_file,Pencil_wood_colormap_start,Pencil_wood_colormap_end,Pencil_wood_colormap_entries)
#end    

// End Pencil_wood_cmap_create
//-----------------------------------------------------------------------------

// End Pencil_wood
//=============================================================================

#version Std_woodgrains_Inc_Temp;

#end // ifndef (Std_woodgrains_Inc_Temp)
