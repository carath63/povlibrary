/******************************************************************************
 * File: libskydome.inc
 * Description:
 *      This library is an adaptation of the LightSys-affiliated SkyDome code
 * turned into parameterized macros.
 *
 ******************************************************************************/

#ifndef(Lib_skydome_Inc_Temp)

#declare Lib_skydome_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libskydome.inc\n"
#end                     

#include "liblights.inc"
#include "libtime.inc"
#include "libsun.inc"
#include "libcolors.inc"

//#declare Sky_Test   = false;

//=============================================================================
// SkyConfig Declaration
//
#ifndef(North) #declare North=z; #end
#declare North  = vnormalize(North);
#ifndef (fiLuminous)
  #declare fiLuminous=finish{emission 1 diffuse 0.0 specular 0 phong 0 reflection 0 crand 0 irid{0}}
#end

#ifndef(Lightsys_Filter)
  #declare Lightsys_Filter=<1,1,1>;
#else  
  #declare Lightsys_Filter=Lightsys_Filter*<1,1,1>;
#end 
#ifndef(Lightsys_Brightness)
  #declare Lightsys_Brightness=1;
#end  

#declare Lightsys_FB = Lightsys_Filter*Lightsys_Brightness;

#declare SkyConfig_intensity_mult           = 0;
#declare SkyConfig_sun_weight               = 1;
#declare SkyConfig_zenith_weight            = 2;
#declare SkyConfig_horizon_weight           = 3;
#declare SkyConfig_under_horizon_correction = 4;
#declare SkyConfig_turbidity                = 5;
#declare SkyConfig_horizon_epsilon          = 6;
#declare SkyConfig_tesselation_trigger      = 7;
#declare SkyConfig_max_vertices             = 8;
#declare SkyConfig_min_vertices             = 9;
#declare SkyConfig_max_faces                = 10;
#declare SkyConfig_sun_light_intensity      = 11;
#declare SkyConfig_sun_area_light           = 12;
#declare SkyConfig_sun_parallel_light       = 13;

#declare SkyConfig_Turbidity_Pure               = 1.0;
#declare SkyConfig_Turbidity_ExceptionallyClear = 1.2;
#declare SkyConfig_Turbidity_VeryClear          = 1.8;
#declare SkyConfig_Turbidity_Clear              = 3.0;
#declare SkyConfig_Turbidity_LightHaze          = 8.0;
#declare SkyConfig_Turbidity_Haze               = 24.0;
#declare SkyConfig_Turbidity_ThinFog            = 64.0;

//-----------------------------------------------------------------------------
// SkyConfig_create()
//
// Creates a new SkyConfig array and initializes it with default values
//
#declare _SkyConfig_default_max_vertices    = 12+4*1247;
#declare _SkyConfig_default_min_vertices    = 12+60;
#declare _SkyConfig_default_max_faces       = 0; //20+4/3*(_SkyConfig_default_max_vertices-12)+1;
#macro SkyConfig_create()
    #local _result  = array[14]{
        0.75,       // intensity multiplier
        2.0,        // sun weight (was 2.0, but I am using 4.0)
        1.0,        // zenith weight
        1.0,        // horizon weight
        1,          // under horizon correction on
        SkyConfig_Turbidity_Clear,
        1e-3,       // horizon epsilon
        0.95,       // tesselation trigger
        _SkyConfig_default_max_vertices,
        _SkyConfig_default_min_vertices,
        _SkyConfig_default_max_faces,
        6,
        1,
        false
    };
_result    
#end

// End SkyConfig_create()
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SkyConfig_dump(SkyConfig)
//
#macro SkyConfig_dump(SkyConfig)
    #debug "Sky_config:\n"
    #debug concat("\t_intensity_mult=", str(SkyConfig[SkyConfig_intensity_mult], 0, 3), "\n")
    #debug concat("\t_sun_weight=", str(SkyConfig[SkyConfig_sun_weight], 0, 3), "\n")
    #debug concat("\t_zenith_weight=", str(SkyConfig[SkyConfig_zenith_weight], 0, 3), "\n")
    #debug concat("\t_horizon_weight=", str(SkyConfig[SkyConfig_horizon_weight], 0, 3), "\n")
    #debug concat("\t_under_horizon_correction=", str(SkyConfig[SkyConfig_under_horizon_correction], 0, 3), "\n")
    #debug concat("\t_turbidity=", str(SkyConfig[SkyConfig_turbidity], 0, 3), "\n")
    #debug concat("\t_horizon_epsilon=", str(SkyConfig[SkyConfig_horizon_epsilon], 0, 3), "\n")
    #debug concat("\t_tesselation_trigger=", str(SkyConfig[SkyConfig_tesselation_trigger], 0, 3), "\n")
    #debug concat("\t_max_vertices=", str(SkyConfig[SkyConfig_max_vertices], 0, 3), "\n")
    #debug concat("\t_min_vertices=", str(SkyConfig[SkyConfig_min_vertices], 0, 3), "\n")
    #debug concat("\t_max_faces=", str(SkyConfig[SkyConfig_max_faces], 0, 3), "\n")
    #debug concat("\t_sun_light_intensity=", str(SkyConfig[SkyConfig_sun_light_intensity], 0, 3), "\n")
    #debug concat("\t_sun_area_light=", str(SkyConfig[SkyConfig_sun_area_light], 0, 0), "\n")
    #debug concat("\t_sun_parallel_light=", str(SkyConfig[SkyConfig_sun_parallel_light], 0, 3), "\n")
#end

// End SkyConfig_dump
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SkyConfig_copy(SkyConfig)
//
// Create a new SkyConfig copying the values from the given SkyConfig
//                                                                             
#macro SkyConfig_copy(SkyConfig)
    #local _result  = array[11];
    #for (i, 0, 10, 1)
        #local _result[i]   = SkyConfig[i];
    #end
_result    
#end

// End SkyConfig_copy
//-----------------------------------------------------------------------------

// End SkyConfig Declaration
//=============================================================================


//=============================================================================
// Sky_xyz_color(SkyConfig,v3Point,fIntensity,Ts,Norm_SolarPos)
//
// Borrowed from CIE_Skylight.inc and recast so the computations can be done
// from the M_Sun_Light_Color macro.
//
// ======================================================
// Preetham's skylight color equations
// ======================================================

// Should reorganise this to reduce the number of functions, macros
// maybe put some in external include file - Philippe

// Preetham's uses :
// Perez, Seals, J.M. and Ineichen model for sky luminance distribution
#declare PSJI_F=function(T, G, A, B, C, D, E){
  (1 + A*exp(B/cos(T)) ) * (1+ C*exp(D*G) + E*pow(cos(G),2) )
}
  // T: theta, angle in radians
  // G: gamma, angle in radians
  // A, B, C, D, E: distribution coefficients
  // ÀA: darkening or brightening of the horizon?
  // ÀB: luminance gradient near the horizon?
  // ÀC: realtive intensity of the circumsolar region?
  // ÀD: width of the circumsolar region?
  // ÀE: relative backscattered light?

    
#declare PSJI_Y=function(Yz, T, G, Ts, A, B, C, D, E){
  Yz * PSJI_F(T, G, A, B, C, D, E) / PSJI_F(0, Ts, A, B, C, D, E)
}
  // Yz: zenith luminance (or chromacity)
  // T:  theta, angle (in radians) from vertical to observed direction
  // G:  gamma, angle (in radians) between observed direction and sun
  // Ts: theta_s, angle (in radians) from vertical to sun direction
  // A, B, C, D, E: distribution coefficients

  
#macro _Sky_set_preetham_coefficients(Turbidity,Preetham_Y_ABCED,Preetham_x_ABCDE,Preetham_y_ABCDE)
    #declare Preetham_Y_ABCDE=
        <  0.1787, -0.3554, -0.0227,  0.1206, -0.0670 > * Turbidity
      + < -1.4630,  0.4275,  5.3251, -2.5771,  0.3703 >;
    
    // For x chromacity
    #declare Preetham_x_ABCDE=
        < -0.0193, -0.0665, -0.0004, -0.0641, -0.0033 > * Turbidity
      + < -0.2592,  0.0008,  0.2125, -0.8989,  0.0452 >;
    
    // For y chromacity
    #declare Preetham_y_ABCDE=
        < -0.0167, -0.0950, -0.0079, -0.0441, -0.0109 > * Turbidity
      + < -0.2608,  0.0092,  0.2102, -1.6537,  0.0529 >;
#end

// Preetham's zenith values

// Zenith luminance
#declare Preetham_Yz=function(T, Ts){ // T turbidity, Ts angle vertical-sun
    (4.0453*T-4.9710)*tan((4/9-T/120)*(pi-2*Ts))
    -0.2455*T + 2.4192
}

// Zenith x chromacity
#declare Preetham_xz=function(T, Ts){ // T turbidity, Ts angle vertical-sun
     Ts*Ts*Ts *( 0.00166*T*T -0.02903*T +0.11693 ) 
    +Ts*Ts    *(-0.00375*T*T +0.06377*T -0.21196 )
    +Ts       *( 0.00209*T*T -0.03202*T +0.06052 )
    +          (              0.00394*T +0.25886 )
}

// Zenith y chromacity
#declare Preetham_yz=function(T, Ts){ // T turbidity, Ts angle vertical-sun
   Ts*Ts*Ts *( 0.00275*T*T -0.04214*T +0.15346 ) 
  +Ts*Ts    *(-0.00610*T*T +0.08970*T -0.26756 )
  +Ts       *( 0.00317*T*T -0.04153*T +0.06670 )
  +          (              0.00516*T +0.26688 )
}

// Get a point color,
// function of <0,0,0> -> v3Point direction
#macro Sky_xyz_color(SkyConfig,v3Point,fIntensity,Ts,Norm_SolarPos) // rename Pigment-> color
    #local v3Point2=vnormalize(v3Point);
    #local Theta=acos(min(1,vdot(y, v3Point2)));
    #local Horizon_Epsilon  = SkyConfig[SkyConfig_horizon_epsilon];
    #local Turbidity        = SkyConfig[SkyConfig_turbidity];
        
    #local Preetham_Y_ABCDE = <0,0,0,0,0>;
    #local Preetham_x_ABCDE = <0,0,0,0,0>; 
    #local Preetham_y_ABCDE = <0,0,0,0,0>;
    _Sky_set_preetham_coefficients(Turbidity,Preetham_Y_ABCDE,Preetham_x_ABCDE,Preetham_y_ABCDE)
    
    // Correction for what happens under the horizon
    #if ( (Theta>(pi/2-Horizon_Epsilon)) & SkyConfig[SkyConfig_under_horizon_correction] )
        #local Y_Mult=pow(sin(Theta),10);  // set a light attenuation 
        //#debug concat("Y_Mult=", str(Y_Mult, 0, 6), "\n")
        #local Theta=pi/2-Horizon_Epsilon; // keep horizon color
    #else
        #local Y_Mult=1; // else do nothing
    #end
    
    // last minute hack to avoid PoV warning messages - Ive nov-2003
    #local Gamma=acos(min(1,max(0.00001,vdot(Norm_SolarPos, v3Point2))));
    
    #local Current_Yz=Preetham_Yz(Turbidity,pi/2-Ts);
    #local Current_xz=Preetham_xz(Turbidity,pi/2-Ts);
    #local Current_yz=Preetham_yz(Turbidity,pi/2-Ts);
    
  Yxy_to_XYZ(
    <
     // Y luminance
     Y_Mult* // Correction for what happens under the horizon
     fIntensity* // Intensity correction to get vuable results
     PSJI_Y(
       Current_Yz,
       Theta,
       Gamma,
       Ts,
       Preetham_Y_ABCDE.red,
       Preetham_Y_ABCDE.green,
       Preetham_Y_ABCDE.blue,
       Preetham_Y_ABCDE.filter,
       Preetham_Y_ABCDE.transmit
     ),
     // x chorminance
     PSJI_Y(
       Current_xz,
       Theta,
       Gamma,
       Ts,
       Preetham_x_ABCDE.red,
       Preetham_x_ABCDE.green,
       Preetham_x_ABCDE.blue,
       Preetham_x_ABCDE.filter,
       Preetham_x_ABCDE.transmit
     ),
     // y chrominance
     PSJI_Y(
       Current_yz,
       Theta,
       Gamma,
       Ts,
       Preetham_y_ABCDE.red,
       Preetham_y_ABCDE.green,
       Preetham_y_ABCDE.blue,
       Preetham_y_ABCDE.filter,
       Preetham_y_ABCDE.transmit
     ),
    >  
  )
#end 

// End Sky_xyz_color
//=============================================================================

//=============================================================================
// Sky_sun_color(SkyConfig,DateTime,LAT,LONG)
//
// Computes the rgb color of light emitted by the sun using the given SkyConfig for
// the date and time in the DateTime at the given LAT/LONG location.
//
#macro Sky_sun_color(SkyConfig,DateTime,LAT,LONG)
    #local _sunpos          = Sun_position(DateTime,LAT,LONG,100);
    #local _sunaxis         = Sun_axis(DateTime,LAT,LONG); 
    #local _currentts       = radians(-_sunaxis.x);
    #local _normsunpos      = vnormalize(_sunpos);
    #local _suncolorXYZ     = Sky_xyz_color(SkyConfig,_sunpos,1,_currentts,_normsunpos);
    #local _zenithcolor     = Sky_xyz_color(SkyConfig,y,1,_currentts,_normsunpos);
    #local _horizoncolor    = Sky_xyz_color(SkyConfig,vrotate(-z,_sunaxis.x*x),1,_currentts,_normsunpos); 

    #local flIntensity      = SkyConfig[SkyConfig_intensity_mult] /(
        SkyConfig[SkyConfig_sun_weight]*_suncolorXYZ.y + 
        SkyConfig[SkyConfig_zenith_weight]*_zenithcolor.y + 
        SkyConfig[SkyConfig_horizon_weight]*_horizoncolor.y
    )*( SkyConfig[SkyConfig_sun_weight] + SkyConfig[SkyConfig_zenith_weight] + SkyConfig[SkyConfig_horizon_weight] );
    
    #local _suncolor        = xyz2RGB(Sky_xyz_color(SkyConfig,_sunpos,flIntensity,_currentts,_normsunpos));
    #local _suncolor        = Clip(_suncolor);
    
_suncolor
#end

// End Sky_sun_color
//=============================================================================

//=============================================================================
// Sky_dome(SkyConfig,DateTime,DomeSize,LAT,LNG)
//
#macro _Sky_init_dome(SkyConfig)
    #declare IcoNumFaces    = 0;
    #declare IcoVtxIndex    = 0;
    #declare IcoMeshIndex   = 0;
    
    #local _min_vertices    = SkyConfig[SkyConfig_min_vertices];
    #local _max_vertices    = max(SkyConfig[SkyConfig_max_vertices],_min_vertices);
    #local _max_faces       = SkyConfig[SkyConfig_max_faces];
    #if (_max_faces <= 0)
        #local _max_faces   = 20+4/3*(_max_vertices-12)+1;
        #declare SkyConfig[SkyConfig_max_faces] = _max_faces;
    #end    
    
    #declare IcoVtx         = array[_max_vertices][2];
    #declare IcoMesh        = array[_max_faces][3];
    #declare IcoMeshFlag    = array[_max_faces];
#end

#macro _Sky_add_point_and_col_to_mesh(vtx,col)
  #declare IcoVtx[IcoVtxIndex][0]=vtx;
  #declare IcoVtx[IcoVtxIndex][1]=col;
  #declare IcoVtxIndex=IcoVtxIndex+1;
#end

#macro _Sky_add_point_to_mesh(SkyConfig,vtx,flIntensity,Current_Ts,Norm_SolarPos)
  _Sky_add_point_and_col_to_mesh(vtx,Sky_xyz_color(SkyConfig,vtx,flIntensity,Current_Ts,Norm_SolarPos))
#end

#macro _Sky_add_triangle_to_mesh(idx1,idx2, idx3)
  #declare IcoMesh[IcoMeshIndex][0]=idx1;
  #declare IcoMesh[IcoMeshIndex][1]=idx2;
  #declare IcoMesh[IcoMeshIndex][2]=idx3;
  #declare IcoMeshFlag[IcoMeshIndex]=true;
  #declare IcoNumFaces=IcoNumFaces+1;
  #declare IcoMeshIndex=IcoMeshIndex+1;
#end

#macro _Sky_clipped_xyz2RGB(xyzv)
    #local _clipped = Clip(xyz2RGB(xyzv));
    //#debug concat("_clipped=<", vstr(3, _clipped, ",", 0, 6), ">\n")
_clipped
#end

#macro Sky_dome(SkyConfig,DateTime,DomeSize,LAT,LNG)
    #local _sunpos          = Sun_position(DateTime,LAT,LNG,DomeSize-1);
    #local _sunaxis         = Sun_axis(DateTime,LAT,LNG);
    #debug concat("_sunpos=<", vstr(3, _sunpos, ",", 0, 3), ">\n")
    #debug concat("_sunaxis=<", vstr(3, _sunaxis, ",", 0, 3), ">\n")
    #local _az              = _sunaxis.y;
    #local _al              = -_sunaxis.x; 
    #local _currentts       = radians(-_sunaxis.x); 
    #debug concat("_currentts=", str(_currentts,0,3), "\n")
    #local _currentsolarpos = vrotate(z,-_al*x);
    #debug concat("_currentsolarpos=<", vstr(3, _currentsolarpos, ",", 0, 3), ">\n")
    #local _normsunpos      = vnormalize(_currentsolarpos);
    #debug concat("_normsunpos=<", vstr(3, _normsunpos, ",", 0, 3), ">\n")
    
    #local _suncolorXYZ     = Sky_xyz_color(SkyConfig,_currentsolarpos,1,_currentts,_normsunpos);
    #local _zenithcolor     = Sky_xyz_color(SkyConfig,y,1,_currentts,_normsunpos);
    #local _horizoncolor    = Sky_xyz_color(SkyConfig,vrotate(-z,_sunaxis.x*x),1,_currentts,_normsunpos);

    #debug concat("_suncolorXYZ=<", vstr(3, _suncolorXYZ, ",", 0, 3), ">\n")     
    #debug concat("_zenithcolor=<", vstr(3, _zenithcolor, ",", 0, 3), ">\n") 
    #debug concat("_horizoncolor=<", vstr(3, _horizoncolor, ",", 0, 3), ">\n") 

    #local flIntensity      = SkyConfig[SkyConfig_intensity_mult] /(
        SkyConfig[SkyConfig_sun_weight]*_suncolorXYZ.y + 
        SkyConfig[SkyConfig_zenith_weight]*_zenithcolor.y + 
        SkyConfig[SkyConfig_horizon_weight]*_horizoncolor.y
    )*( SkyConfig[SkyConfig_sun_weight] + SkyConfig[SkyConfig_zenith_weight] + SkyConfig[SkyConfig_horizon_weight] );
    #debug concat("flIntensity=", str(flIntensity, 0, 3), "\n")
    

    #local phi=(sqrt(5)+1)/2;
    #local BringToHorizonAngle=degrees(atan(phi));
    
    _Sky_init_dome(SkyConfig)
    
    #local _max_faces           = SkyConfig[SkyConfig_max_faces];
    #local _min_vertices        = SkyConfig[SkyConfig_min_vertices];
    #local _max_vertices        = max(SkyConfig[SkyConfig_max_vertices],_min_vertices);
    #local _tesselation_trigger = SkyConfig[SkyConfig_tesselation_trigger];

    _Sky_add_point_to_mesh(SkyConfig,vrotate(vnormalize(<    0,    phi,  1>),<BringToHorizonAngle-_al,0,0>),flIntensity,_currentts,_normsunpos) //  0
    _Sky_add_point_to_mesh(SkyConfig,vrotate(vnormalize(<    0,    phi, -1>),<BringToHorizonAngle-_al,0,0>),flIntensity,_currentts,_normsunpos) //  1
    _Sky_add_point_to_mesh(SkyConfig,vrotate(vnormalize(<    0,   -phi,  1>),<BringToHorizonAngle-_al,0,0>),flIntensity,_currentts,_normsunpos) //  2
    _Sky_add_point_to_mesh(SkyConfig,vrotate(vnormalize(<    0,   -phi, -1>),<BringToHorizonAngle-_al,0,0>),flIntensity,_currentts,_normsunpos) //  3
    _Sky_add_point_to_mesh(SkyConfig,vrotate(vnormalize(<  phi,    1,    0>),<BringToHorizonAngle-_al,0,0>),flIntensity,_currentts,_normsunpos) //  4
    _Sky_add_point_to_mesh(SkyConfig,vrotate(vnormalize(<  phi,   -1,    0>),<BringToHorizonAngle-_al,0,0>),flIntensity,_currentts,_normsunpos) //  5
    _Sky_add_point_to_mesh(SkyConfig,vrotate(vnormalize(< -phi,    1,    0>),<BringToHorizonAngle-_al,0,0>),flIntensity,_currentts,_normsunpos) //  6
    _Sky_add_point_to_mesh(SkyConfig,vrotate(vnormalize(< -phi,   -1,    0>),<BringToHorizonAngle-_al,0,0>),flIntensity,_currentts,_normsunpos) //  7
    _Sky_add_point_to_mesh(SkyConfig,vrotate(vnormalize(<    1,    0,  phi>),<BringToHorizonAngle-_al,0,0>),flIntensity,_currentts,_normsunpos) //  8
    _Sky_add_point_to_mesh(SkyConfig,vrotate(vnormalize(<   -1,    0,  phi>),<BringToHorizonAngle-_al,0,0>),flIntensity,_currentts,_normsunpos) //  9
    _Sky_add_point_to_mesh(SkyConfig,vrotate(vnormalize(<    1,    0, -phi>),<BringToHorizonAngle-_al,0,0>),flIntensity,_currentts,_normsunpos) // 10
    _Sky_add_point_to_mesh(SkyConfig,vrotate(vnormalize(<   -1,    0, -phi>),<BringToHorizonAngle-_al,0,0>),flIntensity,_currentts,_normsunpos) // 11
    
    _Sky_add_triangle_to_mesh( 0, 8, 9)
    _Sky_add_triangle_to_mesh( 9, 8, 2)
    _Sky_add_triangle_to_mesh( 0, 8, 4)
    _Sky_add_triangle_to_mesh( 0, 9, 6)
    _Sky_add_triangle_to_mesh( 8, 2, 5)
    _Sky_add_triangle_to_mesh( 8, 4, 5)
    _Sky_add_triangle_to_mesh( 9, 2, 7)
    _Sky_add_triangle_to_mesh( 9, 6, 7)
    _Sky_add_triangle_to_mesh( 0, 1, 4)
    _Sky_add_triangle_to_mesh( 0, 1, 6)
    _Sky_add_triangle_to_mesh( 2, 3, 5)
    _Sky_add_triangle_to_mesh( 2, 3, 7)
    _Sky_add_triangle_to_mesh( 1,10,11)
    _Sky_add_triangle_to_mesh(10,11, 3)
    _Sky_add_triangle_to_mesh( 1, 4,10)
    _Sky_add_triangle_to_mesh( 4,10, 5)
    _Sky_add_triangle_to_mesh( 5,10, 3)
    _Sky_add_triangle_to_mesh( 1, 11,6)
    _Sky_add_triangle_to_mesh(11, 6, 7)
    _Sky_add_triangle_to_mesh( 7,11, 3)

    // Tesselation loop
    // This produce many duplicated vertices (probably 1/2)
    #local ite0=0;
    #while ( (ite0<IcoMeshIndex) & (IcoMeshIndex<(_max_faces-3)) & (IcoVtxIndex<(_max_vertices-3)) )
      // Prepare tesselation test :
      // If linear interpolation (as performed by the mesh)
      // isn't correct enough (Tesselation_Trigger), tesselate ;
      // else validate current triangle (=do nothing)
      
      // Data for existing vertices
      #local idx1=IcoMesh[ite0][0];
      #local idx2=IcoMesh[ite0][1];
      #local idx3=IcoMesh[ite0][2];
      
      #local vtx1=IcoVtx[idx1][0];
      #local vtx2=IcoVtx[idx2][0];
      #local vtx3=IcoVtx[idx3][0];
      
      #local col1=IcoVtx[idx1][1];
      #local col2=IcoVtx[idx2][1];
      #local col3=IcoVtx[idx3][1];
      
      // Potential vertices for tesselation
      #local vtx12=vnormalize((vtx1+vtx2)/2);
      #local vtx23=vnormalize((vtx2+vtx3)/2);
      #local vtx31=vnormalize((vtx3+vtx1)/2);
      
      // What colors the model gives them
      #local col12=Sky_xyz_color(SkyConfig,vtx12,flIntensity,_currentts,_normsunpos);
      #local col23=Sky_xyz_color(SkyConfig,vtx23,flIntensity,_currentts,_normsunpos);
      #local col31=Sky_xyz_color(SkyConfig,vtx31,flIntensity,_currentts,_normsunpos);
      
      // What colors the interpolation gives them
      #local colM12=(col1+col2)/2;
      #local colM23=(col2+col3)/2;
      #local colM31=(col3+col1)/2;
      
      // Compute color differences using CMC equation
      #local colM12=DeltaE_CMC(col12, colM12);// *sqrt(vlength(vtx2-vtx1)/VtxLength);
      #local colM23=DeltaE_CMC(col23, colM23);// *sqrt(vlength(vtx3-vtx2)/VtxLength);
      #local colM31=DeltaE_CMC(col31, colM31);// *sqrt(vlength(vtx1-vtx3)/VtxLength);
      
      
      // Test if >= Tesselation_Trigger
      // if that is the case, tesselate
      // Tesselates also if not enough vertices
      #local colM12=( colM12>=_tesselation_trigger ) ;
      #local colM23=( colM23>=_tesselation_trigger ) ;
      #local colM31=( colM31>=_tesselation_trigger ) ;
      #if (colM12 | colM23 | colM31 | (IcoVtxIndex<_min_vertices) )
        // Tesselate
        // Invalidate the tesselated triangle
        #declare IcoMeshFlag[ite0]=false;
        #declare IcoNumFaces=IcoNumFaces-1;
        // Add 4 new triangles
        #local idx12=IcoVtxIndex;
        _Sky_add_point_and_col_to_mesh(vtx12,col12)
        #local idx23=IcoVtxIndex;
        _Sky_add_point_and_col_to_mesh(vtx23,col23)
        #local idx31=IcoVtxIndex;
        _Sky_add_point_and_col_to_mesh(vtx31,col31)
        _Sky_add_triangle_to_mesh(idx1, idx12, idx31)
        _Sky_add_triangle_to_mesh(idx2, idx23, idx12)
        _Sky_add_triangle_to_mesh(idx3, idx31, idx23)
        _Sky_add_triangle_to_mesh(idx12, idx23, idx31)
        
        #if (IcoMeshIndex>=(_max_faces-3))
          #debug "Warning: Tesselation ended while tesselation wasn't complete : maximum number of faces is met.\n"
        #end
        #if (IcoVtxIndex>=(_max_vertices-3))
          #debug "Warning: Tesselation ended while tesselation wasn't complete : maximum number of vertices is met.\n"
        #end
          
      /*
      #else // This is useless : triangle is validated by _Sky_add_triangle_to_mesh()
            // to guarantee that triangles that aren't tested because we've run
            // out of vertices are traced.
        #declare IcoMeshFlag[ite0]=true;
      */
      #end // end tesselation test 
      #local ite0=ite0+1;
    #end // #while (ite0<IcoVtxIndex)
    
    #debug concat("Sky_dome faces generated=", str(IcoMeshIndex,0,0), "\n")
    
    
    // Build mesh
    //#declare IcoDome=
    mesh2{
      // Vertices
      vertex_vectors {
        IcoVtxIndex
        #local ite0=0;
        #while (ite0<IcoVtxIndex)
          , IcoVtx[ite0][0]
          #local ite0=ite0+1;
        #end // #while (ite0<IcoVtxIndex)
      }
     
      // Vertices' colors
      texture_list {
        IcoVtxIndex
        #local ite0=0;
        #while (ite0<IcoVtxIndex)
           //, texture{pigment{rgb xyz2RGB(IcoVtx[ite0][1])*Lightsys_FB} finish{fiLuminous}}
           , texture{pigment{rgb _Sky_clipped_xyz2RGB(IcoVtx[ite0][1])*Lightsys_FB} finish{fiLuminous}}
          #local ite0=ite0+1;
        #end // #while (ite0<IcoVtxIndex)
      }
      
      // Faces
      face_indices{
      IcoNumFaces
        #local ite0=0;
        #while (ite0<IcoMeshIndex)
          #if(IcoMeshFlag[ite0])
            ,<IcoMesh[ite0][0], IcoMesh[ite0][1], IcoMesh[ite0][2]>
            , IcoMesh[ite0][0], IcoMesh[ite0][1], IcoMesh[ite0][2]
          #end
          #local ite0=ite0+1;
        #end // #while (ite0<IcoVtxIndex)
      }
      no_shadow
      scale DomeSize
      rotate (_az+degrees(atan2( North.x, North.z)))*y
    } // mesh2
      
#end

// End Sky_dome
//=============================================================================

#version Lib_skydome_Inc_Temp;
#end // ifndef(Lib_skydome_Inc_Temp)