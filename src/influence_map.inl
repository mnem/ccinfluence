#include "xassert.h"

namespace influence_map {
        
    inline size_t InfluenceMap::coords_to_linear(const size_t x, const size_t y) const
    {
        XASSERT(x < _width, "x is greater than map width");
        XASSERT(y < _height, "y is greater than map height");
        
        return _width * y + x;
    }
    
    inline float InfluenceMap::clamp_influence(const float influence) const
    {
        if (!_clamp_values_to_0_1 || (influence >= 0.0f && influence <= 1.0f)) {
            return influence;
        } else if (influence > 1.0f) {
            return 1.0f;
        } else {
            return 0.0f;
        }
    }
    
    inline size_t InfluenceMap::num_cells() const
    {
        return _width * _height;
    }
    
    inline size_t InfluenceMap::width() const
    {
        return _width;
    }
    
    inline size_t InfluenceMap::height() const
    {
        return _height;
    }

    inline bool InfluenceMap::values_clamped() const
    {
        return _clamp_values_to_0_1;
    }

    inline float InfluenceMap::influence(const size_t x, const size_t y) const
    {
        return _data[coords_to_linear(x, y)];
    }

    inline void InfluenceMap::set_influence(const size_t x, const size_t y, const float influence)
    {
        _data[coords_to_linear(x, y)] = clamp_influence(influence);
    }
    
} // namespace influence_map
