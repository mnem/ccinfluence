#include "influence_map.h"
#include <cmath>

namespace influence_map {
    
    InfluenceMap::InfluenceMap(const size_t width, const size_t height, const bool clamp_values_to_0_1, const float initial_influence) :
        _width(width), _height(height), _clamp_values_to_0_1(clamp_values_to_0_1)
    {
        // Of course, if width * height overflows size_t, we're screwed. No sane
        // person would use values that large of course...
        _data = new float[num_cells()];
        _copy = new float[num_cells()];

        const float clamped_influence = clamp_influence(initial_influence);
        for (size_t i = 0; i < num_cells(); i++) {
            _data[i] = clamped_influence;
            _copy[i] = clamped_influence;
        }
    }
    
    InfluenceMap::~InfluenceMap()
    {
        delete[] _data;
        delete[] _copy;
    }
    
    void InfluenceMap::connections(const size_t x,
                                   const size_t y,
                                   float * const connections_array,
                                   const float influence_weight,
                                   const float out_of_bounds_value) const
    {
        connections_array[ConnectionIndex::TopLeft]      = out_of_bounds_value;
        connections_array[ConnectionIndex::TopMiddle]    = out_of_bounds_value;
        connections_array[ConnectionIndex::TopRight]     = out_of_bounds_value;
        connections_array[ConnectionIndex::MiddleRight]  = out_of_bounds_value;
        connections_array[ConnectionIndex::BottomRight]  = out_of_bounds_value;
        connections_array[ConnectionIndex::BottomMiddle] = out_of_bounds_value;
        connections_array[ConnectionIndex::BottomLeft]   = out_of_bounds_value;
        connections_array[ConnectionIndex::MiddleLeft]   = out_of_bounds_value;

        const size_t i = coords_to_linear(x, y);
        
        if (y > 0) {
            if (x > 0) connections_array[ConnectionIndex::TopLeft] = _data[i - _width - 1] * influence_weight;
            connections_array[ConnectionIndex::TopMiddle] = _data[i - _width] * influence_weight;
            if (x < _width - 1) connections_array[ConnectionIndex::TopRight] = _data[i - _width + 1] * influence_weight;
        }
        
        if (x > 0) connections_array[ConnectionIndex::MiddleLeft] = _data[i - 1] * influence_weight;
        if (x < _width - 1) connections_array[ConnectionIndex::MiddleRight] = _data[i + 1] * influence_weight;
        
        if (y < _height - 1) {
            if (x > 0) connections_array[ConnectionIndex::BottomLeft] = _data[i + _width - 1] * influence_weight;
            connections_array[ConnectionIndex::BottomMiddle] = _data[i + _width] * influence_weight;
            if (x < _width - 1) connections_array[ConnectionIndex::BottomRight] = _data[i + _width + 1] * influence_weight;
        }
    }

    void InfluenceMap::add_connections(const size_t x,
                                       const size_t y,
                                       float * const connections_array,
                                       const float influence_weight) const
    {
        const size_t i = coords_to_linear(x, y);
        
        if (y > 0) {
            if (x > 0) connections_array[ConnectionIndex::TopLeft] = _data[i - _width - 1] * influence_weight + connections_array[ConnectionIndex::TopLeft];
            connections_array[ConnectionIndex::TopMiddle] = _data[i - _width] * influence_weight + connections_array[ConnectionIndex::TopMiddle];
            if (x < _width - 1) connections_array[ConnectionIndex::TopRight] = _data[i - _width + 1] * influence_weight + connections_array[ConnectionIndex::TopRight];
        }
        
        if (x > 0) connections_array[ConnectionIndex::MiddleLeft] = _data[i - 1] * influence_weight + connections_array[ConnectionIndex::MiddleLeft];
        if (x < _width - 1) connections_array[ConnectionIndex::MiddleRight] = _data[i + 1] * influence_weight + connections_array[ConnectionIndex::MiddleRight];
        
        if (y < _height - 1) {
            if (x > 0) connections_array[ConnectionIndex::BottomLeft] = _data[i + _width - 1] * influence_weight + connections_array[ConnectionIndex::BottomLeft];
            connections_array[ConnectionIndex::BottomMiddle] = _data[i + _width] * influence_weight + connections_array[ConnectionIndex::BottomMiddle];
            if (x < _width - 1) connections_array[ConnectionIndex::BottomRight] = _data[i + _width + 1] * influence_weight + connections_array[ConnectionIndex::BottomRight];
        }
    }
    
    void InfluenceMap::propagate(const float momentum, const float decay)
    {
        float connections_array[InfluenceMap::CONNECTIONS_ARRAY_LENGTH];
        size_t x = 0;
        size_t y = 0;
        
        const float edge_distance = 1.0f;
        const float corner_distance = 1.414f;
        const float edge = expf(-edge_distance * decay);
        const float corner = expf(-corner_distance * decay);
        
        for (size_t i = 0; i < num_cells(); i++) {
            connections(x, y, connections_array, 1, 0);
            
            // Spread //////////////////////////////////////////////////////
            float max_influence = 0;
            float neighbour_influence;
            
            neighbour_influence = connections_array[ConnectionIndex::TopLeft] * corner;
            if (neighbour_influence > max_influence) max_influence = neighbour_influence;
            
            neighbour_influence = connections_array[ConnectionIndex::TopMiddle] * edge;
            if (neighbour_influence > max_influence) max_influence = neighbour_influence;
            
            neighbour_influence = connections_array[ConnectionIndex::TopRight] * corner;
            if (neighbour_influence > max_influence) max_influence = neighbour_influence;
            
            neighbour_influence = connections_array[ConnectionIndex::MiddleRight] * edge;
            if (neighbour_influence > max_influence) max_influence = neighbour_influence;
            
            neighbour_influence = connections_array[ConnectionIndex::BottomRight] * corner;
            if (neighbour_influence > max_influence) max_influence = neighbour_influence;
            
            neighbour_influence = connections_array[ConnectionIndex::BottomMiddle] * edge;
            if (neighbour_influence > max_influence) max_influence = neighbour_influence;
            
            neighbour_influence = connections_array[ConnectionIndex::BottomLeft] * corner;
            if (neighbour_influence > max_influence) max_influence = neighbour_influence;
            
            neighbour_influence = connections_array[ConnectionIndex::MiddleLeft] * edge;
            if (neighbour_influence > max_influence) max_influence = neighbour_influence;
            
            // lerp ////////////////////////////////////////////////////////
            const float cur_influence = influence(x, y);
            const float result = (max_influence - cur_influence) * momentum + cur_influence;
            _copy[i] = clamp_influence(result);
            
            // Move on
            if (++x >= _width) {
                x = 0;
                y++;
            }
        }
    
        // Swap the buffers
        float * const tmp = _copy;
        _copy = _data;
        _data = tmp;
    }

} // namespace influence_map


