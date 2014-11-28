C++ Influence Map
=================

Simple C++ influence map with unit tests. Largely a rewrite from the
prototype I hacked together at http://bl.ocks.org/mnem/87e8998f389f1aa30227
but less pretty because no graphics.

Ignore the Xcode project, it was just a handy editor. The src should compile
on any C++11 or greater compiler. Or you can remove the `= delete` markers
in `influence_map.h` and shift the copy and assign constructors into
the private section and it should compile with earlier C++ compilers.
