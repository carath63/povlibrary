/******************************************************************************
 * File: libspline.inc
 * Description:
 *      Library of spline utilities
 ******************************************************************************/

#ifndef(Libspline_Inc_Temp)

#declare Libspline_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libspline.inc\n"
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
// Spline_type_from_string(TypeString)
//
#macro Spline_type_from_string(TypeString)
    #switch(0)
        #case(strcmp(strlwr(TypeString),"linear_spline"))
        #case(strcmp(strlwr(TypeString),"linear"))
            linear_spline
        #break
        #case(strcmp(strlwr(TypeString),"natural_spline"))
        #case(strcmp(strlwr(TypeString),"natural"))
            natural_spline
        #break
        #case(strcmp(strlwr(TypeString),"quadratic_spline"))
        #case(strcmp(strlwr(TypeString),"quadratic"))
            quadratic_spline
        #break
        #case(strcmp(strlwr(TypeString),"cubic_spline"))
        #case(strcmp(strlwr(TypeString),"cubic"))
            cubic_spline
        #break
        #else
            #error concat("Unknown spline type ", TypeString, "\n")
        #break                    
    #end
    
#end

// End Spline_type_from_string
//=============================================================================

//=============================================================================
// Spline_create_distance_spline(Spline,TypeString,MinValue,MaxValue,Epsilon,optional NegValue, optional NegPoint, optional EndPoint, optional DistRet)
//
#macro Spline_create_distance_spline(Spline,TypeString,MinValue,MaxValue,Epsilon,optional NegValue, optional NegPoint, optional EndPoint, optional DistRet)

    spline {
        Spline_type_from_string(TypeString)
        #ifdef (NegPoint)
            #local _nv  = #ifdef(NegValue) NegValue; #else -1; #end
            _nv, NegPoint,
        #end
        #local _d   = 0;
        #local _p   = Spline(MinValue);
        _d, _p,
        #for (i, MinValue+Epsilon, MaxValue, Epsilon)
            #local _pn  = Spline(i);
            #local _d   = _d + vlength(_pn - _p);
            #local _p   = _pn;
            _d, _p,
        #end
        
        #ifdef(EndPoint)
            #local _d   = _d + vlength(EndPoint - _p);
            #local _p   = EndPoint;
            _d, _p
        #end
    }
    
    #ifdef (DistRet)
        #declare DistRet    = _d;
    #end    
    #debug concat("Spline_create_distance_spline: total distance = ", str(_d, 0, 6), "\n")
#end

// End Spline_create_distance_spline
//=============================================================================

//=============================================================================
// Spline transformation macros
//

//-----------------------------------------------------------------------------
// Spline_trans(Spline, Time, Foresight, optional Sky, optional Banking)
//
//      This is a replacement for transforms.inc: Spline_Trans(Spline,Time,Sky,Foresight,Banking)
//      that fixes an issue when the forward direction of the object based on the spline
//      results aligns with the Sky vector.  This version checks to see if
//      vlength(vcross(Sky,forward)) is 0, and if so, sets the right vector
//      to x, transformed with the transformation to align y to Sky.
//
#macro Spline_trans(Spline, Time, Foresight, optional Sky, optional Banking)
    #local _sky = #ifdef(Sky) Sky; #else y; #end
    #local _banking = #ifdef(Banking) Banking; #else 0; #end

    #local _location        = <0,0,0>+Spline(Time);
    #local _locationNext    = <0,0,0>+Spline(Time+Foresight);
    #local _locationPrev    = <0,0,0>+Spline(Time-Foresight);
    #local _forward         = vnormalize(_locationNext-_location);
    #if (vlength(vcross(Sky,_forward)) = 0)
        #local _ytrans  = Reorient_Trans(y, Sky)
        #local _right   = vtransform(x, _ytrans);
    #else
        #local _right   = VPerp_To_Plane(Sky,_forward);
    #end
    
    #local _up              = VPerp_To_Plane(_forward,_right);
    #local _matrix          = _matrix_Trans(_right,_up,_forward,_location)
    #local _bankingRotation =
        degrees(atan2(
                        VRotation(
                            VProject_Plane((_locationNext-_location),Sky),
                            VProject_Plane((_location-_locationPrev),Sky),
                            _up
                        )*_banking
                        ,1
        ));
        
    #local _t   = transform {
        rotate _bankingRotation*z
        transform _matrix
    }
    
    _t;
    
#end

// End Spline transformation macros
//=============================================================================

#version Libspline_Inc_Temp;

#end // ifndef (Libspline_Inc_Temp)
