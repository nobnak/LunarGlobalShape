#pragma once

#ifdef _WIN32
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#endif



#include <iostream>
#include <iterator>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Periodic_2_triangulation_traits_2.h>
#include <CGAL/Periodic_2_Delaunay_triangulation_2.h>
#include <CGAL/Timer.h>
