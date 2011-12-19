EdgeSmooth
==========
EdgeSmooth is a plugin for 3dsmax which allows modelers to smooth models by 
defining edges as either "hard" of "soft". This is a more intuitive method 
than the standard smoothing groups.
The plugin operates on top of the standard smoothing groups system: a "soft" 
edge means that the faces on that edge share a common smoothing group. When 
there are no shared smoothing groups, the edge is "hard".

Supported 3dsmax versions
-------------------------
The project contains build configurations for 3dsmax2010, 2011 and 2012; win32 
and x64 platforms. It should also work with older 3dsmax versions, if you have 
the SDK. To add a new configuration, just create a copy of the max2010 config 
and adjust the SDK paths.  
To build all versions in one go, run build_all.bat. You may need to adjust the 
paths to the 3dsmax SDK in the C++ and Linker settings.  

Maxscript access
----------------
The main functions are exposed to maxscript through the EdgeSmooth interface:  

    <boolean> IsSoft <node>node <bitArray>edges  
    <boolean> IsHard <node>node <bitArray>edges  
    <void>    Apply  <boolean>soften node:<node> edges:<bitArray>  
    
    <boolean> MakeHard()  -- Action Interface, equiv. to Apply(false)  
    <boolean> MakeSoft()  -- Action Interface, equiv. to Apply(true)  
    
Example:  

    EdgeSmooth.Apply false $Sphere01 (polyop.getEdgeSelection $Sphere01)
  
See EdgeSmooth.h for details.  

License
-------
This project is licensed under the BSD license. 
For the complete license, see LICENSE.txt