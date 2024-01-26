/*
 *  35mm Camera Macros
 *
 *    Version 1.0 (16th March 2009)
 *
 *    Copyright 2009 Edouard Poor
 */

/*
Hi all.

Here is version 1.0 of my camera macros.

35mm Camera Macros Version 1.0
==============================

The 35mm Camera macros were created to allow POV users to place
cameras in their scenes with the properties of a standard 35mm film
(or full frame digital) camera.

The user can specify the lens (e.g. 50mm or 300mm) and the focal
ratio (e.g. f/1.4 or f/8), and the macros set the field of view and
amount of focal blur to approximate the effect seen in the real world.

The user can specify the focal plane in several ways:
  - Setting it to the look_at point
  - Specifying a seperate point in space that is in focus
  - Specifying a distance to the focal plane
  - Allowing the camera to autofocus on an object
  - Setting the camera to the maximal hyperfocal point

Multiple options can be set, such as the standard samples, variance and
confidence of POVs focal blur, as well as other options like the
circle of confusion for the hyperfocal calculations, the autofocus
samples and tightness, etc.

There is also a "stochastic" mode that allows the user to render
multiple frames (via the animation clock settings). The passes can
then be composited together later to produce an image with focal
blur. This mode turns POV's built-in focal blur off completely.

There are also two debugging variables that can be set to show
the focal plane, and the autofocus samples respectively.

Finally these macros output some detailed information about
the cameras being use.



Camera Macros
-------------

Camera35mm( cam_pos, look_pos, focal_length, fstop )

    Creates a camera with the focal plane at the same point as the
    "look_pos" point.

    Related Options:
        Camera35mm_SetFocalSamples( num_samples )
        Camera35mm_SetFocalVariance( variance )
        Camera35mm_SetFocalConfidence( confidence )


Camera35mm_Point( cam_pos, look_pos, focal_pos, focal_length, fstop )

    Creates a camera with the focal plane at the same distance as
    the "focal_pos" point.

    Related Options:
        Camera35mm_SetFocalSamples( num_samples )
        Camera35mm_SetFocalVariance( variance )
        Camera35mm_SetFocalConfidence( confidence )

Camera35mm_Distance( cam_pos, look_pos, focal_distance, focal_length, fstop )

    Creates a camera with the focal plane set at "focal_distance"
    from the camera.

    Related Options:
        Camera35mm_SetFocalSamples( num_samples )
        Camera35mm_SetFocalVariance( variance )
        Camera35mm_SetFocalConfidence( confidence )

Camera35mm_Hyperfocal( cam_pos, look_pos, focal_length, fstop )

    Creates a camera with focal plane set to the maximal
    hyperfocal distance (i.e. such that infinity is the
    furthest distance to still be considered in focus).

    Related Options:
        Camera35mm_SetCircleOfConfusion( coc_size )
        Camera35mm_SetFocalSamples( num_samples )
        Camera35mm_SetFocalVariance( variance )
        Camera35mm_SetFocalConfidence( confidence )


Camera35mm_Autofocus( cam_pos, look_pos, focal_length, fstop, obj )

    Creates a camera with the focal plane set by shooting test
    rays at the object "obj".

    The object needs to be declared and passed to the
    macro, as follows:

    #declare ball = sphere { 0, 20 pigment { rgb <1,0,0> } };
    object { ball }
    Camera35mm_Autofocus( cam_pos, look_pos, 50, 2.8, ball )

    Related Options:
        Camera35mm_SetAutofocusTightness( fraction_of_screen )
        Camera35mm_SetAutofocusSamples( num_samples )
        Camera35mm_SetFocalSamples( num_samples )
        Camera35mm_SetFocalVariance( variance )
        Camera35mm_SetFocalConfidence( confidence )


Camera35mm_NoBlur( cam_pos, look_pos, focal_length, fstop )

    Creates a camera with focal blur turned off. Useful for
    setting up or testing a scene.



Camera Modes
------------

By deault, the camera macros all use POV's built-in focal blur, but
the mode can be changed to use a stochastic rendering technique
that renders multiple images (via the animation clock settings)
that can later be composited together to give a scene with focal
blur.

Camera35mm_SetStochasticMode()

    Turns the stochastic rendering mode on (instead of POV's built-in
    focal blur).

    Related Options:
        Camera35mm_SetIrisImage( iris_image_png_filename )


Camera35mm_SetPovFocalBlurMode()

    Turns on POV's built-in focal blur mode.
    This is the default setting.



Camera Options
--------------

The following options may be set by the user

    Camera35mm_SetFocalSamples( n )

        Set the number of focal blur samples as per the standard
        POV camera feature.


    Camera35mm_SetFocalVariance( n )

        Sets the focal blur variance as per the standard
        POV camera feature.


    Camera35mm_SetFocalConfidence( n )

        Sets the focal blur confidence as per the standard
        POV camera featue.


    Camera35mm_SetFocalParameters( n_samples, n_variance, n_confidence )

        Sets the samples, variance and confidence all in one call.


    Camera35mm_SetIrisImage( s )

        Sets the iris image used for setting up each pass of the
        stochastic render rig.

        The image file has to be a grayscale PNG file currently.

        The system has a built-in iris image that is used if
        this macro isn't called.


    Camera35mm_SetCircleOfConfusion( n )

        Sets the circle of confusion for use in the hpyerfocal
        macro.


    Camera35mm_SetAutofocusSamples( n )

        Sets the number of samples shot into the scene to determine
        the optimal focal distance given an test object.

        The default value is 500.


    Camera35mm_SetAutofocusTightness( n )

        Sets the tightness of the autofocus area. 1.0 means use a
        circle the width of the image, 0.5 is half the image, and
        0.1 is 10% of the image. The circle is always centered
        in the image.

        The default value is 0.7.

        Wider values might be helped by using more samples, and
        narrower values could use less samples.



Debugging Variables
-------------------

There are two variables that can be set to debug a scene:

Camera35mm_DebugFocalPlane

    Draws a partially transparent plane through the focal plane.


Camera35mm_DebugAutofocus

    Draws spheres at every position where an autofocus sample
    hit the object being focused on.

    The sphere are purple, except for the one choosen to focus on,
    which is green.



Example Output
--------------

Autofocus Debug Output:
 Number of hits = 77 (out of 500)
 Autofocus tightness = 0.70
 Autofocus on = <14.769, 256.538, -37.084> (distance 1263.750mm)

35mm Camera Debug Output:
 Focal Length is 50mm
 Focal Ratio is f/1.4
 Focal Distance is 1259.1mm
 Field of View is 39.6 degrees

 Sensor size is 36mm by 18mm
 Sensor size is 700 by 350 (pixels)
 Circle of confusion is 0.051mm

 Hyperfocal distance is 34772.2mm
 Near Focus is 1216.7mm
 Far Focus is 1304.5mm
 Total distance in focus is 87.80mm
 In front of focal plane 42.37mm (48.3%)
 Behind focal plane 45.43mm (51.7%)

Focal Blur Debug Output:
 Focal blur samples is 37
 Focal blur variance is (Default)
 Focal blur confidence is (Default)

*/

// -- Include Files --------------------------------------------------------------------------------

#include "transforms.inc"
	// For: VRotateD()

#include "arrays.inc"
	// For: SortArray(), etc

#include "mathtools.inc"

// -- Constants ------------------------------------------------------------------------------------

#declare Camera35mm_PovFocalBlurMode = 1;
#declare Camera35mm_StochasticMode = 2;

#declare Camera35mm_FocalSamples = 0;
#declare Camera35mm_FocalVariance = 1;
#declare Camera35mm_FocalConfidence = 2;



// -- Internal Variables ---------------------------------------------------------------------------

#declare Camera35mm_RNG = seed( frame_number );

#declare Camera35mm_FocalParams = array[ 3 ];

// Variables that can be declared by user code:
//
// #declare Camera35mm_DebugFocalPlane = true;
// #declare Camera35mm_DebugAutofocus = true;



// -- Default Values -----------------------------------------------------------------------------

#declare Camera35mm_Mode = Camera35mm_PovFocalBlurMode;

#declare Camera35mm_DefaultLensFunction = 
	function {
		pigment {
			onion
			color_map {
				[ 0.00 color rgb 0.8 ]
				[ 0.45 color rgb 0.8 ]
				[ 0.46 color rgb 1 ]
				[ 0.48 color rgb 1 ]
				[ 0.50 color rgb 0.8 ]
				[ 0.50 color rgb 0 ]
				[ 1.00 color rgb 0 ]
			}
			translate <0.5, 0.5, 0>
		}
	};

#declare Camera35mm_AutofocusTightness = 0.7;
#declare Camera35mm_AutofocusSamples = 500;

#declare Camera35mm_FocalParams[ Camera35mm_FocalSamples ] = 7.0;



// -- Mode of Operation --------------------------------------------------------------------------

#macro Camera35mm_SetPovFocalBlurMode()
	#declare Camera35mm_Mode = Camera35mm_PovFocalBlurMode;
#end

#macro Camera35mm_SetStochasticMode()
	#declare Camera35mm_Mode = Camera35mm_StochasticMode;
#end



// -- Utility Functions -----------------------------------------------------------------------------

#macro RatioAlongLineToClosestPointOnLine( line_start, line_end, p )
	#local line_length = vlength( line_end - line_start );
	
	#local r =  ( ( ( p.x - line_start.x ) * ( line_end.x - line_start.x ) ) +
				( ( p.y - line_start.y ) * ( line_end.y - line_start.y ) ) +
				( ( p.z - line_start.z ) * ( line_end.z - line_start.z ) ) ) /
				( line_length * line_length );

	r
#end


#macro ClosestPointOnLine( line_start, line_end, p )
	#local r = RatioAlongLineToClosestPointOnLine( line_start, line_end, p );

	#local point_on_line = line_start + r * (line_end - line_start);

	point_on_line
#end



// -- Statistical & RNG Functions ------------------------------------------------------------------

#macro hqrand( s )
	#local prime = 67;
	#local i = 0;
	#local n = 0.5791349;
	#while( i < prime )
		#local n = n + rand( s );
		#local i = i + 1;
	#end
	n - int( n )
#end


#macro halton( index, base )
	#local out = 0.0;
	#local fraction = 1.0 / base;
	#local i = index;
	#while( i > 0 )
		#local remainder = mod( i, base );
		#local out = out + (fraction * remainder);
		#local i = int(i / base);
		#local fraction = fraction / base;
	#end

	out
#end


#macro halton3D( n )
	#local baseX = 2;
	#local baseY = 3;
	#local baseZ = 5;
	
	< halton( n, baseX ), halton( n, baseY ), halton( n, baseZ ) >
#end


#macro Gauss(RSR)
	sqrt(-2*log(hqrand(RSR))/log(2.718281828459))*cos(2*pi* hqrand(RSR))
#end



// -- Override array sort code ----------------------------------------------------------------------

#macro Sort_Compare(Array, IdxA, IdxB)
	(Array[IdxA][0] < Array[IdxB][0]) 
#end


#macro Sort_Swap_Data(Array, IdxA, IdxB)
   #local Tmp0 = Array[IdxA][0];
   #local Tmp1 = Array[IdxA][1];
   #local Tmp2 = Array[IdxA][2];
   #local Tmp3 = Array[IdxA][3];
   #declare Array[IdxA][0] = Array[IdxB][0];
   #declare Array[IdxA][1] = Array[IdxB][1];
   #declare Array[IdxA][2] = Array[IdxB][2];
   #declare Array[IdxA][3] = Array[IdxB][3];
   #declare Array[IdxB][0] = Tmp0;
   #declare Array[IdxB][1] = Tmp1;
   #declare Array[IdxB][2] = Tmp2;
   #declare Array[IdxB][3] = Tmp3;
#end



// -- Global Parameters Functions ----------------------------------------------------------------

#macro Camera35mm_SetFocalSamples( n )
	#declare Camera35mm_FocalParams[ Camera35mm_FocalSamples ] = n;
#end


#macro Camera35mm_SetFocalVariance( n )
	#declare Camera35mm_FocalParams[ Camera35mm_FocalVariance ] = n;
#end


#macro Camera35mm_SetFocalConfidence( n )
	#declare Camera35mm_FocalParams[ Camera35mm_FocalConfidence ] = n;
#end


#macro Camera35mm_SetFocalParameters( n_samples, n_variance, n_confidence )
	#declare Camera35mm_FocalParams[ Camera35mm_FocalSamples ] = n_samples;
	#declare Camera35mm_FocalParams[ Camera35mm_FocalVariance ] = n_variance;
	#declare Camera35mm_FocalParams[ Camera35mm_FocalConfidence ] = n_confidence;
#end


#macro Camera35mm_SetIrisImage( s )
	#declare Camera35mm_StochasticIrisImage = s;
#end


#macro Camera35mm_SetCircleOfConfusion( n )
	#declare Camera35mm_CircleOfConfusion = n;
#end


#macro Camera35mm_SetAutofocusSamples( n )
	#declare Camera35mm_AutofocusSamples = n;
#end


#macro Camera35mm_SetAutofocusTightness( n )
	#declare Camera35mm_AutofocusTightness = n;
#end



// -- Debugging/Information --------------------------------------------------------------------------

#macro Camera35mm_PrintInfo( cam_pos, look_pos, focal_length, fstop, focal_pos )
	#local film_width = Math_Scale(SCALE_MM, 36);
	#local fov = degrees( atan( (film_width / 2) / focal_length ) * 2 );
	#local apertureDiameter = focal_length / fstop;

	#local focal_point_on_line = ClosestPointOnLine( cam_pos, look_pos, focal_pos);
	#local focal_distance = vlength( focal_point_on_line - cam_pos);

	#if( defined ( Camera35mm_CircleOfConfusion ) )
		#local circle_of_confusion = Camera35mm_CircleOfConfusion;
	#else
		#local circle_of_confusion = film_width / image_width;
	#end
	
	#local hyperfocal_distance = ((focal_length * focal_length) / (fstop * circle_of_confusion)) + focal_length;

	#local dofNear = ((hyperfocal_distance - focal_length) * focal_distance) / (hyperfocal_distance + focal_distance - (2* focal_length));
	// Prevent 'divide by zero' when calculating far distance.
	#if ( (hyperfocal_distance - focal_distance) <= 0.00001)
		#local dofFar = 100000000.0;
	#else
		#local dofFar = ((hyperfocal_distance-focal_length) * focal_distance) / (hyperfocal_distance - focal_distance);
	#end
	// Calculate percentage of DOF in front and behind the subject.
	#local dofNearPercent = (focal_distance - dofNear)/(dofFar-dofNear) * 100.0;
	#local dofFarPercent = (dofFar - focal_distance)/(dofFar-dofNear) * 100.0;
	#local dofTotal = dofFar - dofNear;

	#debug "35mm Camera Debug Output: \n"
	#debug concat( "\tFocal Length is ", str( focal_length, 0, 0 ), "mm\n" )
	#debug concat( "\tFocal Ratio is f/", str( fstop, 0, 1 ), "\n" )
	#debug concat( "\tFocal Distance is ", str( focal_distance, 0, 1 ), "mm\n" )
	#debug concat( "\tField of View is ", str( fov, 0, 1 ), " degrees\n" )
	#debug concat( "\tAperture Diameter is ", str( apertureDiameter, 0, 3), "mm\n")
	
	#debug "\n"
	#debug concat( "\tSensor size is ", str( film_width, 0, 0 ), "mm by ", str( film_width * image_height / image_width, 0, 0 ), "mm\n" )
	#debug concat( "\tSensor size is ", str( image_width, 0, 0 ), " by ", str( image_height, 0, 0 ), " (pixels)\n" )
	#debug concat( "\tCircle of confusion is ", str( circle_of_confusion, 0, 3 ), "mm\n" )
	
	#debug "\n"
	#debug concat( "\tHyperfocal distance is ", str( hyperfocal_distance, 0, 1 ), "mm\n" )
	#debug concat( "\tNear Focus is ", str( dofNear, 0, 1 ), "mm\n" )
	#debug concat( "\tFar Focus is ", str( dofFar, 0, 1 ), "mm\n" )
	#debug concat( "\tTotal distance in focus is ", str( dofTotal, 0, 2 ), "mm\n" )
	#debug concat( "\tIn front of focal plane ", str( focal_distance - dofNear, 0, 2 ), "mm (", str( dofNearPercent, 0, 1 ), "%)\n" )
	#debug concat( "\tBehind focal plane ", str( (focal_distance - dofFar) * -1, 0, 2 ), "mm (", str( dofFarPercent, 0, 1 ), "%)\n" )
#end



// -- Camera Macros ----------------------------------------------------------------------------------

#macro Camera35mm_NoBlur( cam_pos, look_pos, focal_length, f_ratio )
	#local film_width = Math_Scale(SCALE_MM, 36);
	#local fov = degrees( atan( (film_width / 2) / focal_length ) * 2 );

	// Camera35mm_PrintInfo( cam_pos, look_pos, focal_length, f_ratio, look_pos )

	camera {
		perspective	
		location cam_pos
		look_at look_pos
		angle fov
		right x * 1
		up y * image_height/image_width
	}
#end


#macro Camera35mm_Internal( cam_pos, look_pos, focal_length, fstop, focal_pos )
	#local film_width = Math_Scale(SCALE_MM, 36);
	#local fov = degrees( atan( (film_width / 2) / focal_length ) * 2 );
	#local apertureDiameter = focal_length / fstop;
	#local focal_point_on_line = ClosestPointOnLine( cam_pos, look_pos, focal_pos );
	
	Camera35mm_PrintInfo( cam_pos, look_pos, focal_length, fstop, focal_pos )

	#if( defined( Camera35mm_DebugFocalPlane ) )
		plane {
			cam_pos - look_pos, 0
			translate focal_point_on_line
			pigment { rgbt <0,1,0,0.8> }
			finish { diffuse 0 ambient 1 }
			no_reflection
			no_shadow			
		}
	#end

	#if( Camera35mm_Mode = Camera35mm_PovFocalBlurMode )
		#debug "\nFocal Blur Debug Output:\n"
		#if( defined( Camera35mm_FocalParams[ Camera35mm_FocalSamples ] ) )
			#debug concat( "\tFocal blur samples is ", str( Camera35mm_FocalParams[ Camera35mm_FocalSamples ], 0, 0 ), "\n" )
		#else
			#debug concat( "\tFocal blur samples is (UNSET)\n" )
		#end
		#if( defined( Camera35mm_FocalParams[ Camera35mm_FocalVariance ] ) )
			#debug concat( "\tFocal blur variance is ", str( Camera35mm_FocalParams[ Camera35mm_FocalVariance ], 0, 3 ), "\n" )
		#else
			#debug concat( "\tFocal blur variance is (Default)\n" )
		#end
		#if( defined( Camera35mm_FocalParams[ Camera35mm_FocalConfidence ] ) )
			#debug concat( "\tFocal blur confidence is ", str( Camera35mm_FocalParams[ Camera35mm_FocalConfidence ], 0, 3 ), "\n" )
		#else
			#debug concat( "\tFocal blur confidence is (Default)\n" )
		#end

		camera {
			perspective
	
			focal_point focal_point_on_line
			aperture (apertureDiameter ) * 2  // Magic Number
	
			blur_samples Camera35mm_FocalParams[ Camera35mm_FocalSamples ] 
			#if( defined( Camera35mm_FocalParams[ Camera35mm_FocalVariance ] ) )
				variance Camera35mm_FocalParams[ Camera35mm_FocalVariance ]
			#end
			#if( defined( Camera35mm_FocalParams[ Camera35mm_FocalConfidence ] ) )
				confidence Camera35mm_FocalParams[ Camera35mm_FocalConfidence ]
			#end
			
			location cam_pos
			look_at look_pos
			angle fov
			right x * 1
			up y * image_height/image_width
		}
	#else
		Camera35mm_Stochastic( cam_pos, look_pos, focal_length, fstop, focal_pos )
	#end
#end


#macro Camera35mm( cam_pos, look_pos, focal_length, fstop )
	Camera35mm_Internal( cam_pos, look_pos, focal_length, fstop, look_pos )
#end


#macro Camera35mm_Point( cam_pos, look_pos, focal_pos, focal_length, fstop )
	Camera35mm_Internal( cam_pos, look_pos, focal_length, fstop, focal_pos )
#end


#macro Camera35mm_Distance( cam_pos, look_pos, focal_distance, focal_length, fstop )
	#local look_distance = vlength( look_pos - cam_pos );
	#local look_pos = cam_pos + focal_distance * (look_pos - cam_pos) / look_distance;

	Camera35mm_Point( cam_pos, look_pos, look_pos, focal_length, fstop )
#end


#macro Camera35mm_Hyperfocal( cam_pos, look_pos, focal_length, fstop )
	#local film_width = Math_Scale(SCALE_MM, 36);
	#local fov = degrees( atan( (film_width / 2) / focal_length ) * 2 );
	#local apertureDiameter = focal_length / fstop;
	#local look_distance = vlength( look_pos - cam_pos);
	#local circle_of_confusion = film_width / image_width;
	#local hyperfocal_distance = ((focal_length * focal_length) / (fstop * circle_of_confusion)) + focal_length;
	
	Camera35mm_Distance( cam_pos, look_pos, hyperfocal_distance, focal_length, fstop )
#end


#macro Camera35mm_Autofocus( cam_pos, look_pos, focal_length, fstop, obj )
	#local film_width = Math_Scale(SCALE_MM, 36);
	#local fov = degrees( atan( (film_width / 2) / focal_length ) * 2 );
	#local apertureDiameter = focal_length / fstop;
	#local look_dir = look_pos - cam_pos;
	#local focal_distance = vlength( look_dir );
	#local rot_angle = VAngleD(z, look_dir );
	
	#local autofocus_array = array[ Camera35mm_AutofocusSamples ][ 4 ];
	#local array_used = 0;
	#local total_points_tested = 0;

	#local h = 20;
	#local i = 0;
	#while( i < Camera35mm_AutofocusSamples )
	
		// Next Halton Point 
		#local found = false;
		#while ( found = false )
			#local test_dir = < halton( h, 2 ), halton( h, 3 ), 0> - <0.5,0.5,0>;
			#local h = h + 1;
			#if ( vlength( test_dir ) <= 0.5 & abs( test_dir.y ) <= 0.5*(image_height/image_width)/Camera35mm_AutofocusTightness )
				#local found = true;
			#end
		#end

		// Scale test_dir
		#local test_dir = (test_dir * (fov * Camera35mm_AutofocusTightness ) / 60) + <0,0,1>;

		// Get distance to point on object vis trace()
		#local dir = vaxis_rotate(test_dir, vcross(look_dir, z), -rot_angle);
		#local total_points_tested = total_points_tested + 1;
		#local test=<0,0,0>;
		#local Inter= trace( obj, cam_pos, dir, test );
		#if ( vlength( test ) !=0 )
			#local autofocus_array[ array_used ][0] = vlength( cam_pos - Inter );
			#local autofocus_array[ array_used ][1] = Inter.x;
			#local autofocus_array[ array_used ][2] = Inter.y;
			#local autofocus_array[ array_used ][3] = Inter.z;
			
			#local array_used  = array_used + 1;
		#end			

		#declare i = i + 1;
	#end
	
	Sort_Partial_Array(autofocus_array, 0, array_used-1)
	#local m = floor( array_used * 0.25 );  // 25th percentile
	#local percentile25_focus_point = < autofocus_array[ m ][1], autofocus_array[ m ][2], autofocus_array[ m ][3] >;
	#local distance_to_focus_point = autofocus_array[ m ][0];

	#if( defined( Camera35mm_DebugAutofocus ) )
		#local i = 0;
		#while( i < array_used )
			sphere { 
				<autofocus_array[ i ][1], autofocus_array[ i ][2], autofocus_array[ i ][3]>, 0.05 * fov
				#if ( i = m )
					pigment { rgb <0,1,0> }
				#else
					pigment { rgb <1,0,1> }
				#end
				finish { ambient 1 diffuse 0 }
			}
			#local i = i +1;
		#end
	#end

	#debug "Autofocus Debug Output:\n"
	#debug concat( "\tNumber of hits = ", str( array_used, 0, 0 ), " (out of ", str( total_points_tested, 0, 0 ), ")\n" )
	#debug concat( "\tAutofocus tightness = ", str( Camera35mm_AutofocusTightness, 0, 2 ), "\n" )
	#debug concat( "\tAutofocus on = <", str( percentile25_focus_point.x, 0, 3 ), ", ", str( percentile25_focus_point.y, 0, 3 ), ", ", str( percentile25_focus_point.z, 0, 3 ), "> (distance ", str( distance_to_focus_point, 0, 3), "mm)\n" )
	#debug "\n"

	Camera35mm_Point( cam_pos, look_pos, percentile25_focus_point, lens, f_ratio )
#end



#macro Camera35mm_Stochastic( cam_pos, look_pos, lens, f_ratio, focal_pos )
	#local eye_direction = look_pos - cam_pos;
	#local rot_angle = VAngleD(z, eye_direction);
	#local focal_point_on_line = ClosestPointOnLine( cam_pos, look_pos, focal_pos );
	#local focal_plane_distance = vlength( focal_point_on_line - cam_pos);
	
	#local film_width = Math_Scale(SCALE_MM, 36);
	#local fov = degrees( atan( (film_width / 2) / lens ) * 2 );

	#local aa_scale = (focal_plane_distance * fov) / (image_width * 54.77);
	
	/*
	#local rot = hqrand(s) * 360;
	#local g = Gauss( s ) / 2;
	#declare offset_vec = < sin( rot ) * g, cos( rot ) * g, 0 >;
	*/
	#local offset_vec = < hqrand( Camera35mm_RNG ), hqrand( Camera35mm_RNG ), 0 > - <0.5,0.5,0>;
	#if ( rot_angle = 0 )
		#local aa_offset = offset_vec;
	#else
		#local aa_offset = vaxis_rotate(offset_vec, vcross(eye_direction, z), -rot_angle);
	#end

	#if( defined( Camera35mm_StochasticIrisImage  ) )
		#local lens_shape = function { pigment { image_map { png Camera35mm_StochasticIrisImage once } } };
	#else
		#local lens_shape = Camera35mm_DefaultLensFunction;
	#end

	#local h = frame_number * 7;
	#local found = false;
	#while ( found = false )
		#local test_pos = halton3D( h ) * <1,1,0>;
		#local h = h + 1;
		#local test_brightness = lens_shape( test_pos.x, test_pos.y, test_pos.z ).red;
		#if ( test_brightness >= 0.1 )
			#local lens_offset = test_pos - <0.5,0.5,0>;
			#local lens_brightness = test_brightness;
			#local found = true;
		#end		
	#end
	
	#local apertureDiameter = lens / f_ratio;
	#local dof_scale = (apertureDiameter / focal_plane_distance);

	/*
	#debug "\n"
	#debug concat( "\taa_scale = ", str( aa_scale, 0, 6 ), "\n" )
	#debug concat( "\tdof_scale = ", str( dof_scale, 0, 6 ), "\n" )
	*/
	
	#debug concat( "Stochastic Debug Output:\n" )
	#debug concat( "\tLens brightness = ", str( lens_brightness, 0, 2 ), "\n" )

	plane {
		cam_pos - look_pos, 0
		translate cam_pos - (look_pos * 0.001)
		pigment { rgbt <0,0,0,lens_brightness> }
		finish { diffuse 0 ambient 1 }
		no_reflection
		no_shadow			
	}

	#local rx = lens_offset.x * dof_scale;
	#local ry = lens_offset.y * dof_scale;
	
	camera {
		perspective
		transform {
			translate look_pos * -1
			Reorient_Trans( eye_direction, z )
			matrix <1, 0, 0,  0, 1, 0,   rx, ry, 1,   0, 0, 0>
			Reorient_Trans( z, eye_direction )
			translate look_pos
		}
		location cam_pos // + (aa_offset * aa_scale)
		look_at look_pos + (aa_offset * aa_scale)
		angle fov
		right x * 1
		up y * image_height/image_width
	}
#end
