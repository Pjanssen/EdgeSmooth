EdgeSmooth
==========
EdgeSmooth is a plugin for 3dsmax which allows modelers to smooth models by 
defining edges as either "hard" of "soft". This is a more intuitive method 
than the standard smoothing groups.
The plugin operates on top of the standard smoothing groups system: a "soft" 
edge means that the faces on that edge share a common smoothing group. When 
there are no shared smoothing groups, the edge is "hard".

Supported versions
------------------
During development I'll only use 3dsmax 2010 64bit, but in general it should 
be possible to recompile it for any reasonably recent version.

Exposed functions
-----------------
See EdgeSmooth.h for the functions that are exposed to maxscript.

License
-------
This project is licensed under the BSD license. 
For the complete license, see LICENSE.txt