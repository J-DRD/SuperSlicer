///|/ Copyright (c) Prusa Research 2016 - 2023 Vojtěch Bubník @bubnikv
///|/ Copyright (c) Slic3r 2013 - 2015 Alessandro Ranellucci @alranel
///|/ Copyright (c) 2014 Petr Ledvina @ledvinap
///|/
///|/ PrusaSlicer is released under the terms of the AGPLv3 or higher
///|/
#include "SurfaceCollection.hpp"
#include "BoundingBox.hpp"
#include "SVG.hpp"

#include <map>

namespace Slic3r {

void SurfaceCollection::simplify(double tolerance)
{
    Surfaces ss;
    for (Surfaces::const_iterator it_s = this->surfaces.begin(); it_s != this->surfaces.end(); ++it_s) {
        ExPolygons expp;
        it_s->expolygon.simplify(tolerance, &expp);
        for (ExPolygons::const_iterator it_e = expp.begin(); it_e != expp.end(); ++it_e) {
            Surface s = *it_s;
            s.expolygon = *it_e;
            ss.push_back(s);
        }
    }
    this->surfaces = ss;
}

/* group surfaces by common properties */
void SurfaceCollection::group(std::vector<SurfacesPtr> *retval) const
{
    for (const Surface &surface : this->surfaces) {
        // find a group with the same properties
        SurfacesPtr *group = nullptr;
        for (std::vector<SurfacesPtr>::iterator git = retval->begin(); git != retval->end(); ++git)
            if (! git->empty() && surfaces_could_merge(*git->front(), surface)) {
                group = &*git;
                break;
            }
        // if no group with these properties exists, add one
        if (group == nullptr) {
            retval->resize(retval->size() + 1);
            group = &retval->back();
        }
        // append surface to group
        group->push_back(&surface);
    }
}

SurfacesConstPtr SurfaceCollection::filter_by_type(const SurfaceType type) const
{
    SurfacesConstPtr ss;
    for (const Surface & surface : this->surfaces) {
        if (surface.surface_type == type)
            ss.push_back(&surface);
    }
    return ss;
}

SurfacesConstPtr SurfaceCollection::filter_by_type_flag(const SurfaceType allowed, const SurfaceType not_allowed) const
{
    SurfacesConstPtr ss;
    for (const Surface & surface : this->surfaces) {
        if ((surface.surface_type & allowed) == allowed && (surface.surface_type & not_allowed) == 0) ss.push_back(&surface);
    }
    return ss;
}

SurfacesConstPtr SurfaceCollection::filter_by_types(std::initializer_list<SurfaceType> types) const
{
    SurfacesConstPtr ss;
    for (const Surface &surface : this->surfaces)
        if (std::find(types.begin(), types.end(), surface.surface_type) != types.end())
            ss.push_back(&surface);
    return ss;
}

void SurfaceCollection::filter_by_type(const SurfaceType type, Polygons *polygons) const
{
    for (const Surface & surface : this->surfaces) {
        if (surface.surface_type == type) {
            Polygons pp = to_polygons(surface.expolygon);
            polygons->insert(polygons->end(), pp.begin(), pp.end());
        }
    }
}
void
SurfaceCollection::filter_by_type_flag(Polygons* polygons, const SurfaceType flags_needed, const SurfaceType flags_not_allowed) const
{
    for (const Surface & surface : this->surfaces) {
        if ((surface.surface_type & flags_needed) == flags_needed && (surface.surface_type & flags_not_allowed)==0) {
            Polygons pp = to_polygons(surface.expolygon);
            polygons->insert(polygons->end(), pp.begin(), pp.end());
        }
    }
}

void SurfaceCollection::keep_type(const SurfaceType type)
{
    size_t j = 0;
    for (size_t i = 0; i < surfaces.size(); ++i) {
        if (surfaces[i].surface_type == type) {
            if (j < i)
                std::swap(surfaces[i], surfaces[j]);
            ++j;
        }
    }
    if (j < surfaces.size())
        surfaces.erase(surfaces.begin() + j, surfaces.end());
}

void SurfaceCollection::keep_type_flag(const SurfaceType type_to_keep, const SurfaceType type_to_remove)
{
    size_t j = 0;
    for (size_t i = 0; i < surfaces.size(); ++i) {
        if ((surfaces[i].surface_type & type_to_keep) == type_to_keep && (surfaces[i].surface_type & type_to_remove) == 0) {
            if (j < i)
                std::swap(surfaces[i], surfaces[j]);
            ++j;
        }
    }
    if (j < surfaces.size())
        surfaces.erase(surfaces.begin() + j, surfaces.end());
}

void SurfaceCollection::keep_types(std::initializer_list<SurfaceType> types)
{
    size_t j = 0;
    for (size_t i = 0; i < surfaces.size(); ++ i)
        if (std::find(types.begin(), types.end(), surfaces[i].surface_type) != types.end()) {
            if (j < i)
                std::swap(surfaces[i], surfaces[j]);
            ++ j;
        }
    if (j < surfaces.size())
        surfaces.erase(surfaces.begin() + j, surfaces.end());
}

void SurfaceCollection::keep_types_flag(const SurfaceType types_to_keep, const SurfaceType type_to_remove)
{
    size_t j = 0;
    for (size_t i = 0; i < surfaces.size(); ++i) {
        if ((surfaces[i].surface_type & types_to_keep) != 0 && (surfaces[i].surface_type & type_to_remove) == 0) {
            if (j < i)
                std::swap(surfaces[i], surfaces[j]);
            ++j;
        }
    }
    if (j < surfaces.size())
        surfaces.erase(surfaces.begin() + j, surfaces.end());
}

void SurfaceCollection::remove_type(const SurfaceType type)
{
    size_t j = 0;
    for (size_t i = 0; i < surfaces.size(); ++ i) {
        if (surfaces[i].surface_type != type) {
            if (j < i)
                std::swap(surfaces[i], surfaces[j]);
            ++ j;
        }
    }
    if (j < surfaces.size())
        surfaces.erase(surfaces.begin() + j, surfaces.end());
}

void SurfaceCollection::remove_type(const SurfaceType type, ExPolygons *polygons)
{
    size_t j = 0;
    for (size_t i = 0; i < surfaces.size(); ++ i) {
        if (Surface &surface = surfaces[i]; surface.surface_type == type) {
            polygons->emplace_back(std::move(surface.expolygon));
        } else {
            if (j < i)
                std::swap(surfaces[i], surfaces[j]);
            ++ j;
        }
    }
    if (j < surfaces.size())
        surfaces.erase(surfaces.begin() + j, surfaces.end());
}

void SurfaceCollection::remove_types(std::initializer_list<SurfaceType> types)
{
    size_t j = 0;
    for (size_t i = 0; i < surfaces.size(); ++ i)
        if (std::find(types.begin(), types.end(), surfaces[i].surface_type) == types.end()) {
            if (j < i)
                std::swap(surfaces[i], surfaces[j]);
            ++ j;
        }
    if (j < surfaces.size())
        surfaces.erase(surfaces.begin() + j, surfaces.end());
}

void SurfaceCollection::export_to_svg(const char *path, bool show_labels) 
{
    BoundingBox bbox;
    for (Surfaces::const_iterator surface = this->surfaces.begin(); surface != this->surfaces.end(); ++surface)
        bbox.merge(get_extents(surface->expolygon));
    Point legend_size = export_surface_type_legend_to_svg_box_size();
    Point legend_pos(bbox.min(0), bbox.max(1));
    bbox.merge(Point(std::max(bbox.min(0) + legend_size(0), bbox.max(0)), bbox.max(1) + legend_size(1)));

    SVG svg(path, bbox);
    const float transparency = 0.5f;
    for (Surfaces::const_iterator surface = this->surfaces.begin(); surface != this->surfaces.end(); ++surface) {
        svg.draw(surface->expolygon, surface_type_to_color_name(surface->surface_type), transparency);
        if (show_labels) {
            int idx = int(surface - this->surfaces.begin());
            char label[64];
            sprintf(label, "%d", idx);
            svg.draw_text(surface->expolygon.contour.points.front(), label, "black");
        }
    }
    export_surface_type_legend_to_svg(svg, legend_pos);
    svg.Close();
}

}
