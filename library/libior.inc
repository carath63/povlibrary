/******************************************************************************
 * File: libior.inc
 * Description:
 *      Lib_IOR ".inc" file
 ******************************************************************************/

#ifndef(Lib_IOR_Inc_Temp)

#declare Lib_IOR_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libior.inc\n"
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
// IOR Values
// Source: https://www.blauwfilms.com/ior-3d-database
//

//-----------------------------------------------------------------------------
// Metals
//
#declare IOR_metal_gold     = 0.470;
#declare IOR_metal_nickel   = 1.080;
#declare IOR_metal_copper   = 1.100;
#declare IOR_metal_bronze   = 1.180;
#declare IOR_metal_aluminum = 1.198;

// End Metals
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Gasses
//
#declare IOR_gas_air        = 1.000;
#declare IOR_gas_argon      = 1.000;
#declare IOR_gas_co2        = 1.000;
#declare IOR_gas_helium     = 1.000;
#declare IOR_gas_hydrogen   = 1.000;
#declare IOR_gas_nitrogen   = 1.000;
#declare IOR_gas_oxygen     = 1.000;
#declare IOR_gas_vacuum     = 1.000;
#declare IOR_gas_chlorine   = 1.001;

// End Gasses
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Liquids
//
#declare IOR_liquid_hydrogen    = 1.097; 
#declare IOR_liquid_co2         = 1.200;
#declare IOR_liquid_nitrogen    = 1.205;
#declare IOR_liquid_oxygen      = 1.221;
#declare IOR_liquid_cleaner     = 1.293;
#declare IOR_liquid_water_100c  = 1.318;
#declare IOR_liquid_alchohol    = 1.329;
#declare IOR_liquid_methanol    = 1.329;
#declare IOR_liquid_aqueous_humor   = 1.330;
#declare IOR_liquid_water_20c   = 1.333;
#declare IOR_liquid_carbonated  = 1.340;
#declare IOR_liquid_beer        = 1.345;
#declare IOR_liquid_milk        = 1.350;
#declare IOR_liquid_whisky      = 1.356;
#declare IOR_liquid_acetone     = 1.360;
#declare IOR_liquid_ethanol     = 1.360;
#declare IOR_liquid_white_rum   = 1.361;
#declare IOR_liquid_shampoo     = 1.362;
#declare IOR_liquid_vodka       = 1.363;
#declare IOR_liquid_chlorine    = 1.385;
#declare IOR_liquid_ccl4        = 1.460;
#declare IOR_liquid_vegetable_oil   = 1.470;
#declare IOR_liquid_turpentine  = 1.472;
#declare IOR_liquid_glycerine   = 1.473;
#declare IOR_liquid_glycerol    = 1.473;
#declare IOR_liquid_honey_21pct = 1.484;
#declare IOR_liquid_honey_17pct = 1.494; 
#declare IOR_liquid_benzene     = 1.501;
#declare IOR_liquid_honey_13pct = 1.504;
#declare IOR_liquid_shower_gel  = 1.510;
#declare IOR_liquid_oil_of_wintegreen   = 1.536;
#declare IOR_liquid_PETg                = 1.570;
#declare IOR_liquid_mercury             = 1.620;
#declare IOR_liquid_carbon_disulfide    = 1.628;
#declare IOR_liquid_bromine             = 1.661;

// End Liquids
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Minerals
//
#declare IOR_mineral_almondine  = 1.244; 
#declare IOR_mineral_cryolite   = 1.338;
#declare IOR_mineral_apatite    = 1.420;
#declare IOR_mineral_flourite   = 1.433;
#declare IOR_mineral_opal       = 1.450;
#declare IOR_mineral_natrolite  = 1.480;
#declare IOR_mineral_sodalite   = 1.483;
#declare IOR_mineral_calcite    = 1.486;
#declare IOR_mineral_onyx       = 1.486;
#declare IOR_mineral_hauyne     = 1.490;
#declare IOR_mineral_ulexite    = 1.490;
#declare IOR_mineral_cancrinite = 1.491;
#declare IOR_mineral_tugtupite  = 1.496;
#declare IOR_mineral_chrysocolla    = 1.500;
#declare IOR_mineral_lapis_lazuli_min   = 1.500;
#declare IOR_mineral_lapis_lazuli_max   = 1.550;
#declare IOR_mineral_hauynite           = 1.502;
#declare IOR_mineral_petalite           = 1.502;
#declare IOR_mineral_dolamite           = 1.503;
#declare IOR_mineral_leucite            = 1.509;
#declare IOR_mineral_chalk              = 1.510;
#declare IOR_mineral_magnesite          = 1.515;
#declare IOR_mineral_rock_salt          = 1.516;
#declare IOR_mineral_gaylussite         = 1.517; 
#declare IOR_mineral_moonstone_min      = 1.518;
#declare IOR_mineral_moonstone_max      = 1.526;
#declare IOR_mineral_gypsum             = 1.519;
#declare IOR_mineral_ammolite           = 1.520;
#declare IOR_mineral_stichtite          = 1.520;
#declare IOR_mineral_iolite             = 1.522;
#declare IOR_mineral_sanidine           = 1.522;
#declare IOR_mineral_feldspar_albite    = 1.525;
#declare IOR_mineral_feldspar_amazonite = 1.525;
#declare IOR_mineral_feldspar_microcline    = 1.525;
#declare IOR_mineral_feldspar_orthoclase    = 1.525;
#declare IOR_mineral_moonstone_adularia     = 1.525;
#declare IOR_mineral_peristerite            = 1.525;
#declare IOR_mineral_aragonite              = 1.530;
#declare IOR_mineral_meerschaum             = 1.530;
#declare IOR_mineral_pearl                  = 1.530;
#declare IOR_mineral_amethyst               = 1.532;
#declare IOR_mineral_citrine                = 1.532;
#declare IOR_mineral_elaeolite              = 1.532;
#declare IOR_mineral_feldspar_adventurine   = 1.532;
#declare IOR_mineral_feldspar_chrysoprase   = 1.534;
#declare IOR_mineral_moonstone_albite       = 1.535;
#declare IOR_mineral_apophyllite            = 1.536;
#declare IOR_mineral_amber_min              = 1.539;
#declare IOR_mineral_amber_max              = 1.546;
#declare IOR_mineral_feldspar_oligoclase    = 1.539;
#declare IOR_mineral_steatite               = 1.539;
#declare IOR_mineral_cordierite             = 1.540; 
#declare IOR_mineral_jasper                 = 1.540;
#declare IOR_mineral_prase                  = 1.540;
#declare IOR_mineral_prasiolite             = 1.540;
#declare IOR_mineral_scapolite              = 1.540;
#declare IOR_mineral_agate                  = 1.544;
#declare IOR_mineral_chalcedony_min         = 1.544;
#declare IOR_mineral_chalcedony_max         = 1.553;
#declare IOR_mineral_quartz_min             = 1.544;
#declare IOR_mineral_quartz_max             = 1.644;
#declare IOR_mineral_nacl_min               = 1.544;
#declare IOR_mineral_nacl_max               = 1.644;
#declare IOR_mineral_tiger_eye              = 1.544; 
#declare IOR_mineral_agalmatolite           = 1.550;
#declare IOR_mineral_variscite              = 1.550;
#declare IOR_mineral_beryllonite            = 1.553;
#declare IOR_mineral_hambergite             = 1.559;
#declare IOR_mineral_emerald_min            = 1.560;
#declare IOR_mineral_emerald_max            = 1.605;
#declare IOR_mineral_labradorite_min        = 1.560;
#declare IOR_mineral_labradorite_max        = 1.572;
#declare IOR_mineral_oregon_sunstone_min    = 1.560;
#declare IOR_mineral_oregon_sunstone_max    = 1.572;
#declare IOR_mineral_serpentine             = 1.560;
#declare IOR_mineral_feldspar_labrodorite   = 1.565;
#declare IOR_mineral_aquamarine             = 1.567;
#declare IOR_mineral_beryl_min              = 1.570;
#declare IOR_mineral_beryl_max              = 1.600;
#declare IOR_mineral_anhydrite              = 1.571;
#declare IOR_mineral_augelite               = 1.574;
#declare IOR_mineral_vivianite              = 1.580;
#declare IOR_mineral_morganite              = 1.585;
#declare IOR_mineral_colemanite             = 1.586;
#declare IOR_mineral_howlite                = 1.586;
#declare IOR_mineral_wardite                = 1.590;
#declare IOR_mineral_ekanite                = 1.600;
#declare IOR_mineral_jade_nephrite_min      = 1.600;
#declare IOR_mineral_jade_nephrite_max      = 1.641;
#declare IOR_mineral_nephrite               = 1.600;
#declare IOR_mineral_rhodochrosite          = 1.600;
#declare IOR_mineral_tremolite              = 1.600;
#declare IOR_mineral_brazilianite           = 1.603;
#declare IOR_mineral_tourmaline_min         = 1.603;
#declare IOR_mineral_tourmaline_max         = 1.655;
#declare IOR_mineral_topaz_min              = 1.607;
#declare IOR_mineral_topaz_max              = 1.627;
#declare IOR_mineral_smaragdite             = 1.608;
#declare IOR_mineral_chrome_tourmaline_min  = 1.610;
#declare IOR_mineral_chrome_tourmaline_max  = 1.640;
#declare IOR_mineral_prehnite               = 1.610;
#declare IOR_mineral_turquoise_min          = 1.610;
#declare IOR_mineral_turquoise_max          = 1.650;
#declare IOR_mineral_amblygonite            = 1.611;
#declare IOR_mineral_hemimorphite           = 1.614;
#declare IOR_mineral_lazulite               = 1.615;
#declare IOR_mineral_actinolite             = 1.618;
#declare IOR_mineral_smithsonite            = 1.621;
#declare IOR_mineral_celestite              = 1.622;
#declare IOR_mineral_clinohymite            = 1.625;
#declare IOR_mineral_danburite_min          = 1.627;
#declare IOR_mineral_danburite_max          = 1.641;
#declare IOR_mineral_andalusite_min         = 1.629;
#declare IOR_mineral_andalusite_max         = 1.650;
#declare IOR_mineral_chalybite              = 1.630;
#declare IOR_mineral_barite                 = 1.636;
#declare IOR_mineral_jade_jadeite_min       = 1.640;
#declare IOR_mineral_jade_jadeite_max       = 1.667;
#declare IOR_mineral_phenakite              = 1.650;
#declare IOR_mineral_spodumene              = 1.650;
#declare IOR_mineral_euclase                = 1.652;
#declare IOR_mineral_hiddenite              = 1.655;
#declare IOR_mineral_malachite              = 1.655;
#declare IOR_mineral_sillimanite            = 1.658;
#declare IOR_mineral_kunzite_min            = 1.660;
#declare IOR_mineral_kunzite_max            = 1.676;
#declare IOR_mineral_enstatite              = 1.663;
#declare IOR_mineral_aluminum_oxide         = 1.665;
#declare IOR_mineral_jadeite                = 1.665;
#declare IOR_mineral_kornerupine            = 1.665;
#declare IOR_mineral_hypersthene            = 1.670;
#declare IOR_mineral_olivine                = 1.670;
#declare IOR_mineral_axenite_min            = 1.674;
#declare IOR_mineral_axenite_max            = 1.704;
#declare IOR_mineral_axinite                = 1.675;
#declare IOR_mineral_diopside               = 1.680;
#declare IOR_mineral_barytocalcite          = 1.684;
#declare IOR_mineral_dumortierite           = 1.686;
#declare IOR_mineral_rhodizite              = 1.690;
#declare IOR_mineral_tanzanite_min          = 1.692;
#declare IOR_mineral_tanzanite_max          = 1.700;
#declare IOR_mineral_sinhalite              = 1.699;
#declare IOR_mineral_cobalt_violet          = 1.710;
#declare IOR_mineral_spinel_min             = 1.712;
#declare IOR_mineral_spinel_max             = 1.717;
#declare IOR_mineral_idocrase               = 1.713;
#declare IOR_mineral_kyanite                = 1.715;
#declare IOR_mineral_garnet_grossular_min   = 1.720;
#declare IOR_mineral_garnet_grossular_max   = 1.800;
#declare IOR_mineral_taaffeite              = 1.720;
#declare IOR_mineral_clinozoisite           = 1.724;
#declare IOR_mineral_azurite                = 1.730;
#declare IOR_mineral_garnet_pyrope_min      = 1.730;
#declare IOR_mineral_garnet_pyrope_max      = 1.760;
#declare IOR_mineral_epidote                = 1.733;
#declare IOR_mineral_rhodonite              = 1.735;
#declare IOR_mineral_garnet_tsavorite_min   = 1.739;
#declare IOR_mineral_garnet_tsavorite_max   = 1.744;
#declare IOR_mineral_staurolite             = 1.739;
#declare IOR_mineral_cobalt_blue            = 1.740;
#declare IOR_mineral_garnet_rhodolite_min   = 1.740;
#declare IOR_mineral_garnet_rhodolite_max   = 1.770;
#declare IOR_mineral_garnet_uvarovite_min   = 1.740;
#declare IOR_mineral_garnet_uvarovite_max   = 1.870;
#declare IOR_mineral_periclase              = 1.740;
#declare IOR_mineral_pyrope                 = 1.740;
#declare IOR_mineral_chrysoberyl            = 1.745;
#declare IOR_mineral_garnet_hessonite       = 1.745;
#declare IOR_mineral_alexandrite_min        = 1.746;
#declare IOR_mineral_alexandrite_max        = 1.755;
#declare IOR_mineral_crysoberyl_min         = 1.746;
#declare IOR_mineral_crysoberyl_max         = 1.755;
#declare IOR_mineral_benitoite              = 1.757;
#declare IOR_mineral_ruby_min               = 1.757;
#declare IOR_mineral_ruby_max               = 1.779;
#declare IOR_mineral_garnet_almondine       = 1.760;
#declare IOR_mineral_padparadja_min         = 1.760;
#declare IOR_mineral_padparadja_max         = 1.773;
#declare IOR_mineral_star_ruby_min          = 1.760;
#declare IOR_mineral_star_ruby_max          = 1.773;
#declare IOR_mineral_star_sapphire_min      = 1.760;
#declare IOR_mineral_star_sapphire_max      = 1.773;
#declare IOR_mineral_corundum               = 1.766;
#declare IOR_mineral_ceylanite              = 1.770;
#declare IOR_mineral_zircon_min             = 1.777;
#declare IOR_mineral_zircon_max             = 1.987;
#declare IOR_mineral_lead_nitrate           = 1.782;
#declare IOR_mineral_painite                = 1.787;
#declare IOR_mineral_garnet_almandite       = 1.790;
#declare IOR_mineral_garnet_mandarin_min    = 1.790;
#declare IOR_mineral_garnet_mandarin_max    = 1.800;
#declare IOR_mineral_spessartite_min        = 1.790;
#declare IOR_mineral_spessartite_max        = 1.810;
#declare IOR_mineral_cerussite              = 1.804;
#declare IOR_mineral_garnet_spessartite     = 1.810;
#declare IOR_mineral_pyrite                 = 1.810;
#declare IOR_mineral_garnet_andradite       = 1.820;
#declare IOR_mineral_purpurite              = 1.840;
#declare IOR_mineral_uvarovite              = 1.870;
#declare IOR_mineral_garnet_demantiod_min   = 1.880;
#declare IOR_mineral_garnet_demantiod_max   = 1.900;
#declare IOR_mineral_sphene                 = 1.885; 
#declare IOR_mineral_scheelite              = 1.920;
#declare IOR_mineral_sulphur                = 1.960;
#declare IOR_mineral_cobalt_green           = 1.970;
#declare IOR_mineral_cassiterite            = 1.997;
#declare IOR_mineral_lead                   = 2.010;
#declare IOR_mineral_zincite                = 2.010;
#declare IOR_mineral_tin_Iodide             = 2.106;
#declare IOR_mineral_phsogenite             = 2.117;
#declare IOR_mineral_cubic_zirconia_min     = 2.150;
#declare IOR_mineral_cubic_zirconia_max     = 2.180;
#declare IOR_mineral_cromite                = 2.160;
#declare IOR_mineral_tantalite              = 2.240;
#declare IOR_mineral_wulfenite              = 2.300;
#declare IOR_mineral_chrome_yellow          = 2.310;
#declare IOR_mineral_crocoite               = 2.310;
#declare IOR_mineral_sphalerite             = 2.368;
#declare IOR_mineral_chromium_green         = 2.400;
#declare IOR_mineral_strontium_titanate     = 2.410;
#declare IOR_mineral_diamond                = 2.418;
#declare IOR_mineral_chrome_red             = 2.420;
#declare IOR_mineral_anatase                = 2.490;
#declare IOR_mineral_moissanite_min         = 2.650;
#declare IOR_mineral_moissanite_max         = 2.690;
#declare IOR_mineral_aluminum_chloride      = 2.700;
#declare IOR_mineral_chromium_oxide         = 2.705;
#declare IOR_mineral_copper_oxide           = 2.705;
#declare IOR_mineral_proustite              = 2.790;
#declare IOR_mineral_cuprite                = 2.850;
#declare IOR_mineral_amorphous_selenium     = 2.920;
#declare IOR_mineral_hematite               = 2.940;
#declare IOR_mineral_iron                   = 2.950;
#declare IOR_mineral_chromium               = 2.970;
#declare IOR_mineral_cinnabar               = 3.020;
#declare IOR_mineral_iodine_crystal         = 3.340;
#declare IOR_mineral_gallium_phosphide      = 3.500;
#declare IOR_mineral_gallium_arsenide       = 3.927;
#declare IOR_mineral_silicon                = 4.010;


// End Minerals
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Glass (plastic)
//
#declare IOR_glass_fused_quartz     = 1.458;
#declare IOR_glass_fused_silica     = 1.459;
#declare IOR_glass_fused_quartz2    = 1.460; 
#declare IOR_glass_plastic          = 1.460;
#declare IOR_glass_pyrex            = 1.474;
#declare IOR_glass_crown_impure     = 1.485;
#declare IOR_glass_obsidian         = 1.486;
#declare IOR_glass_albite           = 1.489;
#declare IOR_glass_acrylic          = 1.490;
#declare IOR_glass_lucite           = 1.495;
#declare IOR_glass_crown_pure_min   = 1.500;
#declare IOR_glass_crown_pure_max   = 1.540;
#declare IOR_glass                  = 1.500;
#declare IOR_glass_modavite         = 1.500;
#declare IOR_glass_crown_zinc       = 1.517;
#declare IOR_glass_crown            = 1.520;
#declare IOR_glass_flint_impure_min = 1.523;
#declare IOR_glass_flint_impure_max = 1.925;
#declare IOR_glass_flint_29pct      = 1.569;
#declare IOR_glass_flint_pure_min   = 1.600;
#declare IOR_glass_flint_pure_max   = 1.620;
#declare IOR_glass_flint_heavy      = 1.650;
#declare IOR_glass_flint_dense      = 1.660;
#declare IOR_glass_flint_55pct      = 1.669;
#declare IOR_glass_flint_71pct      = 1.805;
#declare IOR_glass_flint_heaviest   = 1.890;
#declare IOR_glass_crystal          = 2.00; 
#declare IOR_glass_arsenic_trisulfide   = 2.040;

// End Glass
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Other Solid Compounds
//
#declare IOR_solid_water            = 1.309;
#declare IOR_solid_vitreous_humor   = 1.340;
#declare IOR_solid_teflon           = 1.350;
#declare IOR_solid_eye_cornea       = 1.380;
#declare IOR_solid_eye_lens         = 1.410;
#declare IOR_solid_parafin_wax      = 1.446;
#declare IOR_solid_formica          = 1.470;
#declare IOR_solid_coral            = 1.486;
#declare IOR_solid_styrene          = 1.519;
#declare IOR_solid_nylon            = 1.530;
#declare IOR_solid_shell            = 1.530;
#declare IOR_solid_ivory            = 1.540;
#declare IOR_solid_polystyrene      = 1.550;
#declare IOR_solid_PET              = 1.575;
#declare IOR_solid_polycarbonate    = 1.584;
#declare IOR_solid_styrofoam        = 1.595;
#declare IOR_solid_asphalt          = 1.635;
#declare IOR_solid_mylar            = 1.650;
#declare IOR_solid_ebonite          = 1.660;
#declare IOR_solid_jet              = 1.660; 

// End Other Solid Compounds
//-----------------------------------------------------------------------------

// End IOR Values
//=============================================================================

#version Lib_IOR_Inc_Temp;

#end // ifndef (Lib_IOR_Inc_Temp)
