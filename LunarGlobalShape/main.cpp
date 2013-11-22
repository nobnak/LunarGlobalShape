#include "stdafx.h"

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
    std::istream_iterator< Point_3 >  input_begin( std::cin );
    std::istream_iterator< Point_3 >  input_end;
    std::ostream_iterator< Point >  output( std::cout, "\n" );

    CGAL::Timer t;
	std::vector<Point> points;
	int counter = 0;
    t.start();
    while (input_begin != input_end) {
        Point_3 p = *(input_begin++);
		points.push_back( Point(p.x() / 360.0, (p.y() + 90.0) / 180.0) );
		if ((++counter % 1000) == 0)
			std::cout << counter << std::endl;
    }
    t.stop();
	std::cout << "Data load : " << t.time() << "sec" << std::endl;

	PDT T; // Put the domain with the constructor
    t.start();
	T.insert(points.begin(), points.end(), true);
    t.stop();
    std::cout << "Triangulation : " << t.time() << "sec" << std::endl;
    
    return 0;
}
