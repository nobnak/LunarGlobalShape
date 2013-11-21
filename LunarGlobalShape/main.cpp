#include <iostream>
#include <iterator>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Periodic_2_triangulation_traits_2.h>
#include <CGAL/Periodic_2_Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Periodic_2_triangulation_traits_2<K> GT;
typedef CGAL::Periodic_2_Delaunay_triangulation_2<GT> PDT;

typedef PDT::Face_handle       Face_handle;
typedef PDT::Vertex_handle     Vertex_handle;
typedef PDT::Locate_type       Locate_type;
typedef PDT::Point             Point;
typedef PDT::Iso_rectangle     Iso_rectangle;

typedef K::Point_3 Point_3;

int main()
{
    Iso_rectangle domain(-1, -1, 2, 2); // The cube for the periodic domain
    
    std::istream_iterator< Point_3 >  input_begin( std::cin );
    std::istream_iterator< Point_3 >  input_end;
    std::ostream_iterator< Point >  output( std::cout, "\n" );
    
    //PDT T(input_begin, input_end, domain); // Put the domain with the constructor
    while (input_begin != input_end) {
        Point_3 p = *input_begin;
        *output = Point(p.x(), p.y());
        input_begin++;
    }
    
    return 0;
}
