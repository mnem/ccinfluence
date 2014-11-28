#include "influence_map.h"

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
    
} // namespace influence_map


