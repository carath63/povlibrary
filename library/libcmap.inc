/******************************************************************************
 * File: libcmap.inc
 * Description:
 *      Libcmap ".inc" file
 ******************************************************************************/

#ifndef(Libcmap_Inc_Temp)

#declare Libcmap_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libcmap.inc\n"
#end                     

//=============================================================================
// Includes
//

//-----------------------------------------------------------------------------
// Standard Includes
//
#include "functions.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
#include "libcolors.inc"

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
// Color Map Creation and Manipulation
//
#declare Color_type_rgb     = 0;
#declare Color_type_rgbf    = 1;
#declare Color_type_rgbt    = 2;
#declare Color_type_rgbft   = 3;
#declare Color_type_srgb    = 4;
#declare Color_type_srgbf   = 5;
#declare Color_type_srgbt   = 6;
#declare Color_type_srgbft  = 7;

#declare Color_type_labels  = array[8]{
    "rgb",
    "rgbf",
    "rgbt",
    "rgbft",
    "srgb",
    "srgbf",
    "srgbt",
    "srgbft"
}
;

//-----------------------------------------------------------------------------
// Cmap_entry_create(Index,Color,ColorType)
//
#macro Cmap_entry_create(Index,Color,ColorType)
    #if (Index < 0 | Index > 1)
        #error concat("Cmap_entry_create: Index out of range: ", str(Index, 0, 3), "\n")
    #end    
    #local _entry   = dictionary {
        .index: Index,
        .value: Color,
        .ctype: ColorType
    };
    
    _entry    
#end

// End Cmap_entry_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Cmap_entry_string(Entry,Precision)
//
#macro Cmap_entry_string(Entry,Precision)
    #local _es  = concat(
        "[",
        str(Entry.index, 0, Precision),
        ", ",
        Color_type_labels[Entry.ctype],
        " <",
        #switch(Entry.ctype)
            #case(Color_type_rgb)
            #case(Color_type_srgb)
                vstr(3, Entry.value, ",", 0, Precision),
                #break
            #case(Color_type_rgbf)
            #case(Color_type_rgbt)
            #case(Color_type_srgbf)
            #case(Color_type_srgbt)
                vstr(4, Entry.value, ",", 0, Precision),
                #break
            #case(Color_type_rgbft)
            #case(Color_type_srgbft)
                vstr(5, Entry.value, ",", 0, Precision),
                #break        
        #end
        ">]"
    );
    
    _es
#end

// End Cmap_entry_string
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Cmap_create()
//
#macro Cmap_create()
    #local _cmap    = dictionary {
        .num_entries:   0,
        .entries:   array
    };
    
    _cmap
#end

// End Cmap_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Cmap_debug_print(Cmap,Precision)
//
#macro Cmap_debug_print(Cmap,Precision)
    #debug "color_map {\n"
    #for (i, 0, Cmap.num_entries-1, 1)
        #debug concat("   ",Cmap_entry_string(Cmap.entries[i],Precision),"\n")
    #end
    #debug "}\n"
#end

// End Cmap_debug_print
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Cmap_generate(Cmap)
//
#macro Cmap_generate(Cmap)
    #local _cmap    = color_map {
        #for (i, 0, Cmap.num_entries-1, 1)
            [Cmap.entries[i].index, Cmap.entries[i].value]
        #end
    }
    
    _cmap
#end

// End Cmap_generate
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Cmap_add(Cmap,Entry)
//
#macro Cmap_add(Cmap,Entry)                    
    #for (i, Cmap.num_entries, 0, -1)
        #if (i = 0)
            #declare Cmap.entries[i] = Entry;
        #else
            #if (Cmap.entries[i-1].index <= Entry.index)
                #declare Cmap.entries[i] = Entry;
                #break
            #else
                #declare Cmap.entries[i] = Cmap.entries[i-1];
            #end    
        #end    
    #end        
    #declare Cmap.num_entries   = Cmap.num_entries + 1;
#end

// End Cmap_add
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Cmap_create_from_pigment(Pigment,ColorType,StartLoc,EndLoc,Entries)
//
#macro Cmap_create_from_pigment(Pigment,ColorType,StartLoc,EndLoc,Entries)
    #local _trajectory  = EndLoc - StartLoc;
    #local _length      = vlength(_trajectory);
    #local _dir         = vnormalize(_trajectory);
    #local _step_size   = _length/Entries;
    
    #local _cmap    = Cmap_create();
    #for (i, 0, Entries-1, 1)
        #local _p   = StartLoc+i*_dir*_step_size;
        #local _c   = eval_pigment(Pigment,_p);
        #local _e   = Cmap_entry_create(i/Entries, _c, ColorType);
        Cmap_add(_cmap,_e)
    #end
    
    _cmap
#end

// End Cmap_create_from_pigment
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Cmap_create_from_cosine_palette(C1,C2,C3,C4,ColorType,StepSize)
//
#macro Cmap_create_from_cosine_palette(C1,C2,C3,C4,ColorType,StepSize)
    #if (ColorType = Color_type_rgb | ColorType = Color_type_srgb)
        #local _cmap    = Cmap_create();
        #local _t       = 0.0;
        #while (_t <= 1.0)
            #local _c   = Cosine_color(C1,C2,C3,C4,_t);
            #local _e   = Cmap_entry_create(min(_t,1.0), _c, ColorType);
            Cmap_add(_cmap,_e)
            #local _t   = _t + StepSize;
        #end
    #else
        #error "Colors with transparency or filter are not yet supported\n"
    #end
    
    _cmap    
#end

// End Cmap_create_from_cosine_palette
//-----------------------------------------------------------------------------

/*
#declare Test_p = pigment {
    bozo
    color_map {
        [0.0 rgb <1, 0, 0>]
        [1.0 rgb <1, 1, 0>]
    }
}
#declare Test_cmap  = Cmap_create_from_pigment(Test_p,Color_type_rgb,<-1.0, 0.0, -1.0>,<1.0, 0.0, 1.0>,10)
Cmap_debug_print(Test_cmap,3)
        
#declare Test_cmap  = Cmap_create();
#declare Test_e1    = Cmap_entry_create(0.0, rgb <1,1,1>, Color_type_rgb);
#declare Test_e2    = Cmap_entry_create(0.5, rgb <0,0,0>, Color_type_rgb);
#declare Test_e3    = Cmap_entry_create(1.0 rgb 0.5, Color_type_rgbf);
Cmap_add(Test_cmap, Test_e3)
Cmap_debug_print(Test_cmap,3)
Cmap_add(Test_cmap, Test_e1)
Cmap_debug_print(Test_cmap,3)
Cmap_add(Test_cmap, Test_e2)
Cmap_debug_print(Test_cmap,3)

#local _cmap    = Cmap_generate(Test_cmap)
box {
    -1, 1
    pigment {
        gradient x
        color_map { _cmap }
    }
}
light_source {
    <0, 4, -4>,
    1
}
camera {
    perspective
    location <0, 4, -4>
    look_at <0, 0, 0>
}
*/    
                    


// End Color Map Creation and Manipulation
//=============================================================================

#version Libcmap_Inc_Temp;

#end // ifndef (Libcmap_Inc_Temp)
