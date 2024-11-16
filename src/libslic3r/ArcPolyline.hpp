#ifndef SLIC3R_ARCPOLYLINE_HPP_
#define SLIC3R_ARCPOLYLINE_HPP_

#include "Geometry/ArcWelder.hpp"
#include "Line.hpp"
#include "MultiPoint.hpp"
#include <vector>

namespace Slic3r {

class ArcPolyline
{
protected:
    Geometry::ArcWelder::Path m_path;
    bool m_only_strait = true;

    static Geometry::ArcWelder::Path _from_polyline(const Points &poly);
    static Geometry::ArcWelder::Path _from_polyline(std::initializer_list<Point> poly);

public:
    ArcPolyline() = default;
    ArcPolyline(const ArcPolyline &) = default;
    ArcPolyline(ArcPolyline &&) = default;
    ArcPolyline(const Polyline &other) : m_path(_from_polyline(other.points)) {}
    ArcPolyline(const Points &other) : m_path(_from_polyline(other)) {}
    ArcPolyline(const Geometry::ArcWelder::Path &other);
    ArcPolyline &operator=(const ArcPolyline &) = default;
    ArcPolyline &operator=(ArcPolyline &&) = default;

    void append(const Point &point) { m_path.emplace_back(Geometry::ArcWelder::Segment{point, 0.f, Geometry::ArcWelder::Orientation::Unknown}); }
    void append_before(const Point &point) { m_path.insert(m_path.begin(), Geometry::ArcWelder::Segment{point, 0.f, Geometry::ArcWelder::Orientation::Unknown}); }
    void append(const Points &src);
    void append(Points &&src);
    void append(const Points::const_iterator &begin, const Points::const_iterator &end);
    void append(const ArcPolyline &src);
    void append(ArcPolyline &&src);
    void clear() { m_path.clear(); }
    void swap(ArcPolyline &other) { m_path.swap(other.m_path); this->m_only_strait = other.m_only_strait; assert(is_valid()); }
    void reverse() { Geometry::ArcWelder::reverse(m_path); }

    const Point &front() const { return m_path.front().point; }
    const Point &middle() const { return m_path[m_path.size() / 2].point; }
    const Point &back() const { return m_path.back().point; }
    bool empty() const { return m_path.empty(); }
    bool is_valid() const;
    bool is_closed() const { return this->m_path.front().point == this->m_path.back().point; }

    bool has_arc() const;
    size_t size() const { return m_path.size(); }
    const Geometry::ArcWelder::Path &get_arc() const { return m_path; }
    const Point &get_point(size_t i) const { return m_path[i].point; }
    const Geometry::ArcWelder::Segment &get_arc(size_t i) const { return m_path[i]; }

    bool split_at_index(const size_t index, ArcPolyline &p1, ArcPolyline &p2) const;
    void pop_front();
    void pop_back();
    void set_front(const Point &p);
    void set_back(const Point &p);
    int find_point(const Point &point, coordf_t epsilon) const;

    coordf_t length() const { return Geometry::ArcWelder::path_length<coordf_t>(m_path); }
    bool at_least_length(coordf_t length) const;
    std::pair<int, Point> foot_pt(const Point &pt) const;
    void split_at(Point &point, ArcPolyline &p1, ArcPolyline &p2) const;
    void split_at(coordf_t distance, ArcPolyline &p1, ArcPolyline &p2) const;
    void clip_start(coordf_t dist);
    void clip_end(coordf_t dist);
    Polyline to_polyline(coord_t deviation = 0) const;
    void translate(const Vector &vector);
    void rotate(double angle);
    Point get_point_from_begin(coord_t distance) const;
    Point get_point_from_end(coord_t distance) const;

    void make_arc(ArcFittingType with_fitting_arc, coordf_t tolerance, double fit_percent_tolerance);
    int simplify_straits(coordf_t min_tolerance, coordf_t min_point_distance, coordf_t mean_dist_per_line, const int buffer_size, const int buffer_init);
    void simplify_straits(const coordf_t min_tolerance, const coordf_t min_point_distance);
    bool normalize();
};

} // namespace Slic3r

#endif // SLIC3R_ARCPOLYLINE_HPP_
