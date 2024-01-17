The .pov files in this directory are all example scenes that construct brick walls using the Brickwall library files located in povlibrary/library.  The scenes
and libraries were developed with the beta 2 POVRay 3.8.  Your mileage with earlier versions of povray will vary.

To render these scenes, you will need to have povlibrary/library in your Library_Path.  You can leave everything else as a default.  For the scenes that
have multiple options, you can define Scene_View=N.M on the povray command line to render the other views.  (i.e. Declare=Scene_View=1.2).

There are also a number of other options to define other parameters for the scene views.  You can do this from the command line for simple values,
(e.g. Declare=Scene_wall_courses=12 to set the number of brick courses to 12 instead of the default of 10).  For the more complex options found in "common.inc"
you may find it easier to create a copy of one of the example scene files and define overrides for the Common_ values in that file before including "common.inc".
