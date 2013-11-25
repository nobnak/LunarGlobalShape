#include "stdafx.h"

template < class GT, class Vb = CGAL::Triangulation_vertex_base_3<GT> >
class My_vertex_base : public Vb {
public:
  typedef typename Vb::Vertex_handle  Vertex_handle;
  typedef typename Vb::Cell_handle    Cell_handle;
  typedef typename Vb::Point          Point;

  template < class TDS2 >
  struct Rebind_TDS {
    typedef typename Vb::template Rebind_TDS<TDS2>::Other  Vb2;
    typedef My_vertex_base<GT, Vb2>                        Other;
  };

  My_vertex_base() {}
  My_vertex_base(const Point& p) : Vb(p) {}
  My_vertex_base(const Point& p, Cell_handle c) : Vb(p, c) {}

  Vertex_handle   vh;
  Cell_handle     ch;
  int i;
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel			K;
typedef CGAL::Triangulation_data_structure_3<My_vertex_base<K> >    Tds;
typedef CGAL::Triangulation_3<K, Tds>			Triangulation;
typedef Triangulation::Point					Point;
typedef Triangulation::Facet					Facet;
typedef Triangulation::Cell_handle				Cell_handle;
typedef Triangulation::Vertex_handle			Vertex_handle;
typedef Triangulation::Cell_iterator			Cell_iterator;
typedef Triangulation::Facet_iterator			Facet_iterator;

double LUNAR_RADIUS = 1737.4;
double R_LUNAR_RADIUS = 1.0 / LUNAR_RADIUS;
double ANGLE2RAD = 0.01745329251;

int main()
{
    std::istream_iterator<Point>  input_begin( std::cin );
    std::istream_iterator<Point>  input_end;

    CGAL::Timer t;
    std::vector<Point> lalt;
	int counter = 0;
	int heartbeat = 1000;
    t.start();
    while (input_begin != input_end) {
        Point p3 = *(input_begin++);
        lalt.push_back(p3);
		if (++counter % heartbeat == 0)
			std::cout << "Data load : " << counter << std::endl;
    }
    t.stop();
	std::cout << "Took " << t.time() << "sec" << std::endl;

	std::cout << "Shuffle data : ";
	t.start();
	std::random_shuffle(lalt.begin(), lalt.end());
	t.stop();
	std::cout << "Took " << t.time() << "sec" << std::endl;

	std::vector<Point> points;
	points.push_back(Point(0, 0, 0));
	counter = 0;
	t.start();
	for (std::vector<Point>::iterator it = lalt.begin(); it != lalt.end(); it++) {
		Point p3 = *it;
		double theta = p3.x() * ANGLE2RAD;
		double phi = p3.y() * ANGLE2RAD;
		double cosphi = cos(phi);
		Point pSphere = Point(cosphi * sin(theta), sin(phi), cosphi * cos(theta));
		points.push_back(pSphere);
		if (++counter % heartbeat == 0)
			std::cout << "Parse data : " << counter << std::endl;
	}
	t.stop();
	std::cout << "Took " << t.time() << "sec" << std::endl;

	Triangulation T;
	counter = 0;
    t.start();
	for (std::vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
		Vertex_handle vh = T.insert(*it);
		vh->i = counter;

		if (++counter % heartbeat == 0)
			std::cout << "Triangulation : " << counter << std::endl;
	}
    t.stop();
    std::cout << "Took " << t.time() << "sec" << std::endl;

	std::vector<Cell_handle> cells;
	T.incident_cells(T.infinite_vertex(), std::back_inserter(cells));
	std::vector<Facet> facets;
	for (std::vector<Cell_handle>::iterator it = cells.begin(); it != cells.end(); it++) {
		Cell_handle ch = *it;
		Facet f = Facet(ch, ch->index(T.infinite_vertex()));
		facets.push_back(f);
	}

	std::ofstream oFileT("output.obj", std::ios::out);
    oFileT << "v 0 0 0" << std::endl;
	for (int i = 1; i < points.size(); i++) {
		Point p = points[i];
        Point pLalt = lalt[i-1];
        double r = (LUNAR_RADIUS + pLalt.z()) * R_LUNAR_RADIUS;
        p = Point(p.x() * r, p.y() * r, p.z() * r);
		oFileT << "v " << p << std::endl;
	}
	for (std::vector<Facet>::iterator it = facets.begin(); it != facets.end(); it++) {
		Facet f = *it;
		Cell_handle ch = f.first;
		
		int vi = f.second;
		if (vi % 2 == 0) {
			oFileT << "f " << (1 + ch->vertex((vi + 3) % 4)->i)
				<< " " << (1 + ch->vertex((vi + 2) % 4)->i)
				<< " " << (1 + ch->vertex((vi + 1) % 4)->i) << std::endl;
		} else {
			oFileT << "f " << (1 + ch->vertex((vi + 1) % 4)->i)
				<< " " << (1 + ch->vertex((vi + 2) % 4)->i)
				<< " " << (1 + ch->vertex((vi + 3) % 4)->i) << std::endl;
		}
	}
    
    return 0;
}
