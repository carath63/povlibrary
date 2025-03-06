/******************************************************************************
 * File: libtext.inc
 * Description:
 *      This library contains various macros and utilities for creating
 *  text objects that I have found to be useful.
 ******************************************************************************/

#ifndef(Libtext_Inc_Temp)

#declare Libtext_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libtext.inc\n"
#end                     

//=============================================================================
// Includes
//

//-----------------------------------------------------------------------------
// Standard Includes
//

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//

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
// Text Scaling and Justification Utilities
//

//-----------------------------------------------------------------------------
// Text_font_type(FontTypeStr)
//
#macro Text_font_type(FontTypeStr)
    #switch(0)
        #case(strcmp(strlwr(FontTypeStr),"ttf"))
            ttf
        #break
        #case(strcmp(strlwr(FontTypeStr),"ttc"))
            ttf
        #break
        #else
            #error concat("Unsupported font type ", FontTypeStr)
        #break            
    #end
#end

// End Text_font_type
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Text_font_type_string_from_name(FontName)
//
#macro Text_font_type_string_from_name(FontName)
    #local _len     = strlen(FontName);
    #local _last    = _len;
    #local _dot     = 0;
    #local _ext     = "ttf";
    
    #while (_last > 0 & _dot <= 0)
        #if (substr(FontName,_last,1) = ".")
            #local _dot = _last;
            #if (_dot < _len)
                #local _ext = substr(FontName,_dot+1,(_len - _dot));
            #end    
        #else
            #local _last    = _last - 1;    
        #end
    #end
    _ext
#end

// End Text_font_type_string_from_name
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Text_metrics_get(FontName,Text,optional Depth,optional Offset,optional FontTypeStr)
//
//  Returns a dictionary{} containing:
//      .font_type:     FontType,
//      .font_name:     FontName,
//      .text_str:      Text,
//      .text_depth:    Depth,
//      .text_offset:   Offset,
//      .text_max:      max_extent()
//      .text_min:      min_extent()
//      .text_size:     max_extent() - min_extent()
//      .text_center:   (max_extent() + min_extent())/2;
//      .text_obj:      text {}
//      .text_scale:    <1,1,1>
//
//  The extents/size are used in other macros for scaling, centering, etc.
//  The text{} object is stored so it can be reused if desired
//
#macro Text_metrics_get(FontName,Text,optional Depth,optional Offset,optional FontTypeStr)
    #local _depth       = #ifdef(Depth) Depth; #else 1; #end
    #local _offset      = #ifdef(Offset) Offset; #else 0; #end
    #local _font_type   = #ifdef(FontTypeStr) FontTypeStr; #else Text_font_type_string_from_name(FontName); #end
    
    #local _text_obj    = text { Text_font_type(_font_type) FontName Text _depth, _offset }
    #local _text_max    = max_extent(_text_obj);
    #local _text_min    = min_extent(_text_obj);
    
    #local _metrics = dictionary {
        .font_type:     _font_type,
        .font_name:     FontName,
        .text_str:      Text,
        .text_depth:    _depth,
        .text_offset:   _offset,
        .text_max:      _text_max,
        .text_min:      _text_min,
        .text_size:     _text_max - _text_min,
        .text_obj:      _text_obj,
        .text_scale:    <1,1,1>
    };
    
    _metrics
#end

// End Text_metrics_get
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Text_metrics_scale_to_width(TextMetrics,Width)
//
//  Returns a scaling factor <x,y,z> that maintains the height/width ratio for the
//  given text metrics to fit the given width, without changing the depth
//
#macro Text_metrics_scale_to_width(TextMetrics,Width)
    #local _wscale  = Width/TextMetrics.text_size.x;
    #local _scale   = <_wscale, _wscale, 1>;
    
    _scale
#end

// End Text_metrics_scale_to_width
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Text_metrics_scale_to_height(TextMetrics,Height)
//
//  Returns a scaling factor <x,y,z> that maintains the height/width ratio for the
//  given text metrics to fit the given height, without changing the depth
//
#macro Text_metrics_scale_to_height(TextMetrics,Height)
    #local _hscale  = Height/TextMetrics.text_size.y;
    #local _scale   = <_hscale, _hscale, 1>;
    
    _scale
#end

// End Text_metrics_scale_to_height
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Text_metrics_scale(TextMetrics,Scale)
//
//  Returns a new Text_metrics dictionary{} where everything is scaled by
//  the given Scaling factor
//
#macro Text_metrics_scale(TextMetrics,Scale)
    #local _s   = Scale*<1,1,1>;
    #local _sm  = dictionary {
        .font_type: TextMetrics.font_type,
        .font_name: TextMetrics.font_name,
        .text_str:  TextMetrics.text_str,
        .text_depth:    TextMetrics.text_depth*_s.z,
        .text_offset:   TextMetrics.text_offset*_s,
        .text_max:      TextMetrics.text_max*_s,
        .text_min:      TextMetrics.text_min*_s,
        .text_size:     TextMetrics.text_size*_s,
        .text_obj:      object { TextMetrics.text_obj scale _s },
        .text_scale:    TextMetrics.text_scale * _s
    }
    ;
    
    _sm
#end

// End Text_metrics_scale
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Text_metrics_spacing_get(TextMetrics)
//
//  Returns a dictionary{} containing the intra-character spacing and character offsets
//  for a text string using the given Font and allowing for additional optional Offsets
//
//      {
//          .text_metrics:  TextMetrics,
//          .text_len:      strlen(Text),
//          .num_spaces:    strlen(Text)-1
//          .spaces:        array of inter-character spacings,
//          .offsets:       array of inter-character offsets,
//          .char_metrics:  array of Text_metrics computed for each individual character
//      }
//
#macro Text_metrics_spacing_get(TextMetrics)
    #local _tlen        = strlen(TextMetrics.text_str);
    #local _num_spaces  = _tlen-1;
    #local _spaces      = array[_num_spaces];
    #local _offsets     = array[_num_spaces];
    #local _char_metrics    = array[_tlen];
    
    #local _c1          = substr(TextMetrics.text_str,1,1);
    #local _char_metrics[0]     = Text_metrics_scale(Text_metrics_get(TextMetrics.font_name,_c1,TextMetrics.text_depth,TextMetrics.text_offset,TextMetrics.font_type),TextMetrics.text_scale); 
    #for (i, 1, _tlen-1, 1)
        #local _c2              = substr(TextMetrics.text_str,i+1,1);
        #local _char_metrics[i] = Text_metrics_scale(Text_metrics_get(TextMetrics.font_name,_c2,TextMetrics.text_depth,TextMetrics.text_offset,TextMetrics.font_type),TextMetrics.text_scale);
        #local _pair            = concat(_c1,_c2);
        #local _pair_metrics    = Text_metrics_scale(Text_metrics_get(TextMetrics.font_name,_pair,TextMetrics.text_depth,_offset,_font_type),TextMetrics.text_scale);
        
        #local _spaces[i-1]     = _pair_metrics.text_size.x - (_char_metrics[i-1].text_size.x + _char_metrics[i].text_size.x);
        #local _offsets[i-1]    = _pair_metrics.text_size.x - _char_metrics[i].text_size.x;
        
        #local _c1              = _c2;
    #end
    
    #local _spacing = dictionary {
        .text_metrics:  TextMetrics,
        .text_len:      _tlen,
        .num_spaces:    _num_spaces,
        .spaces:        _spaces,
        .offsets:       _offsets,
        .char_metrics:  _char_metrics
    }
    ;
    
    _spacing
#end

// End Text_metrics_spacing_get
//-----------------------------------------------------------------------------

// End Text Scaling Utilities
//=============================================================================

//=============================================================================
// Text Layout Utilities
//

//-----------------------------------------------------------------------------
// Text_metrics_layout_cylindrical(TextMetrics,Radius)
//
//  Returns a union{} of the individual text characters included in the given
//  TextMetrics object such that the first character is placed at <0, 0, -Radius>
//  and subsequent characters are rotated from that position around the Y axis.
//  The arc-distance between characters is determined by using Text_metrics_spacing_get()
//  so that the natural kerning for the font is preserved.
//
#macro Text_metrics_layout_cylindrical(TextMetrics,Radius)
    #local _cyl_circ    = 2*pi*Radius;
    #local _spacing     = Text_metrics_spacing_get(TextMetrics);
    #local _depth       = TextMetrics.text_size.z;

    #local _text    = union {
        object { _spacing.char_metrics[0].text_obj translate <0, 0, -(Radius + _depth)> }
        #local _cur_rot = 0;
        #for (i, 1, _spacing.text_len-1, 1)
            #local _cur_rot = _cur_rot + 360*(_spacing.offsets[i-1]/_cyl_circ);
            object {
                _spacing.char_metrics[i].text_obj
                translate <0, 0, -(Radius + _depth)>
                rotate <0, -_cur_rot, 0>
            }    
        #end
    }
    
    _text
    
#end

// End Text_metrics_layout_cylindrical
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Text_metrics_layout_helical(TextMetrics,Radius,Rise,optional ZRotate)
//
//  Returns a union{} of the individual text characters included in the given
//  TextMetrics object such that the first character is placed at <0, 0, -Radius>
//  and subsequent characters are rotated from that position around the Y axis.
//  The arc-distance between characters is determined by using Text_metrics_spacing_get()
//  so that the natural kerning for the font is preserved.
//
#macro Text_metrics_layout_helical(TextMetrics,Radius,Rise,optional ZRotate)
    #local _cyl_circ    = 2*pi*Radius;
    #local _spacing     = Text_metrics_spacing_get(TextMetrics);
    #local _depth       = TextMetrics.text_size.z;
    #local _zrotate     = #ifdef(ZRotate) ZRotate; #else false; #end
    #if (_zrotate)
        #local _z_rotation_angle    = degrees(atan2(Rise,(2*pi*Radius)));
    #end    

    #local _text    = union {
        object { _spacing.char_metrics[0].text_obj translate <0, 0, -(Radius + _depth)> }
        #local _cur_rot = 0;
        #for (i, 1, _spacing.text_len-1, 1)
            #local _cur_rot = _cur_rot + 360*(_spacing.offsets[i-1]/_cyl_circ);
            #local _cur_y   = Rise*(_cur_rot/360);
            object {
                _spacing.char_metrics[i].text_obj
                #if (_zrotate)
                    rotate <0, 0, _z_rotation_angle>
                #end
                
                translate <0, _cur_y, -(Radius + _depth)>
                rotate <0, -_cur_rot, 0>
            }    
        #end
    }
    
    _text
    
#end

// End Text_metrics_layout_helical
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Text_metrics_layout_spline(TextMetrics,Spline,RotateY,RotateZ,DOffset)
//  
//  Returns a union{} of the individual text characters included in the given
//  TextMetrics object such that the first character is placed at Spline(0),
//  and subsequent characters are placed along the spline based on the offset
//  values from Text_metrics_spacing_get() call.  If RotateY is true, the
//  letters are rotated first around the Y axis based on the position of the
//  previous character.  (Spline(-1) should return a position that sets
//  the rotation of the first character.
//
//  If RotateZ is true, the character is also rotated on the Z axis based on
//  the difference in Y position of the previous character and this character.
//  DOffset specifies the translation of the character along the Z axis. 
//                                                                      
#macro Text_metrics_layout_spline(TextMetrics,Spline,RotateY,RotateZ,optional DOffset,optional Epsilon)
    #local _doffset = #ifdef(DOffset) DOffset; #else 0; #end
    #local _epsilon = #ifdef(Epsilon) Epsilon; #else 1e-4; #end
    
    #local _spacing     = Text_metrics_spacing_get(TextMetrics);
    #local _offset_locs = array[_spacing.text_len];
    #local _offset_locs[0]  = 0;
    #for (i, 0, _spacing.num_spaces-1, 1)
        #local _offset_locs[i+1]    = _offset_locs[i] + _spacing.offsets[i];
    #end
    
    #local _cdist   = 0.0;
    
    #local _text    = union {
        #for (i, 0, _spacing.text_len-1, 1)
            object {
                _spacing.char_metrics[i].text_obj
                translate <0, 0, _doffset>
                
                #if (RotateY | RotateZ)
                    #local _p1      = Spline(_offset_locs[i] - _epsilon);
                    #local _p2      = Spline(_offset_locs[i] + _epsilon);
                    #local _p_dir   = vnormalize(_p2 - _p1);
                    #if (RotateZ)
                        #local _z_rotate    = degrees(atan2(_p_dir.y, sqrt(_p_dir.x*_p_dir.x+_p_dir.z*_p_dir.z)));
                        rotate <0, 0, _z_rotate>
                    #end
                    
                    #if (RotateY)
                        #local _y_rotate    = -degrees(atan2(_p_dir.z,_p_dir.x));
                        rotate <0, _y_rotate, 0>                 
                    #end
                    
                #end
                
                #local _pos = Spline(_offset_locs[i]);
                #ifdef (Text_metrics_layout_spline_verbose)
                #debug concat(_spacing.char_metrics[i].text_str, ": offset=", str(_offset_locs[i], 0, 3), "; pos=<", vstr(3, _pos, ",", 0, 3), ">")
                #ifdef (_prev_pos)
                    #local _dist    = vlength(_pos - _prev_pos);
                    #local _cdist   = _cdist + _dist;
                    #debug concat("; distance=", str(vlength(_pos - _prev_pos), 0, 3), "; cum distance=", str(_cdist, 0, 3), "\n")
                #else
                    #debug "\n"
                #end
                #end 
                translate _pos
                
                #local _prev_pos    = _pos;
            }    
        #end
    }
    
    _text
    
#end

// End Text_metrics_layout_spline
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Text_metrics_layout_circular(TextMetrics,Radius)
//
//  Returns a union{} of individual text characters included in the given
//  TextMetrics object such that the first character is placed at <0,0,0> and
//  subsequent characters are placed rotated around the axis z=Radius using
//  the Text_metrics_spacing_get() results.
//
#macro Text_metrics_layout_circular(TextMetrics,Radius)
    #local _circ        = 2*pi*Radius;
    #local _spacing     = Text_metrics_spacing_get(TextMetrics);
    
    #local _text    = union {
        #local _cur_rot = 0;
        object { _spacing.char_metrics[0].text_obj translate <0, -Radius, 0> }
        #for (i, 1, _spacing.text_len-1, 1)
            #local _cur_rot = _cur_rot + 360*(_spacing.offsets[i-1]/_circ);
            object {
                _spacing.char_metrics[i].text_obj
                translate <0, -Radius, 0>
                rotate <0, 0, _cur_rot>
            }    
        #end
    }
    
    _text
#end

// End Text_metrics_layout_circular
//-----------------------------------------------------------------------------

// End Text Layout Utilities
//=============================================================================

#version Libtext_Inc_Temp;

#end // ifndef (Libtext_Inc_Temp)
