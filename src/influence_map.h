#pragma once

#include <cstddef>

namespace influence_map {
    
    class InfluenceMap
    {
    public:
        enum ConnectionIndex {
            TopLeft     = 0,
            TopMiddle   = 1,
            TopRight    = 2,
            MiddleLeft  = 7,
            MiddleRight = 3,
            BottomLeft  = 6,
            BottomMiddle= 5,
            BottomRight = 4
        };
        
        static const size_t CONNECTIONS_ARRAY_LENGTH = 8;
        
        InfluenceMap(const size_t width, const size_t height, const bool clamp_values_to_0_1, const float initial_influence);
        InfluenceMap(const InfluenceMap&) = delete;
        InfluenceMap& operator=(const InfluenceMap&) = delete;
        ~InfluenceMap();
        
        size_t num_cells() const;
        size_t width() const;
        size_t height() const;
        
        float influence(const size_t x, const size_t y) const;
        void set_influence(const size_t x, const size_t y, const float influence);
        
        /**
         * Assumes that connections_array has space for 8 floats:
         *
         *      1 2 3
         *      8   4
         *      7 6 5
         * At the edges of the grid the out_of_bounds_value is used to fill any cell
         * positions which do not exist.
         *
         * Each influence value is multiplied by influence_weight before being stored.
         * This does not affect the values in the source cells.
         */
        void connections(const size_t x,
                         const size_t y,
                         float * const connections_array,
                         const float influence_weight,
                         const float out_of_bounds_value) const;
        
        /**
         * Similar to connections(), this fills the connections_array (which must be at least 8 floats in size)
         * with the surrounding influence by adding them to values already in those positions in the array.
         *
         * As it adds to the array you *must* have already seeded the connections_array with valid numeric values.
         *
         * Unlike the connections() function, out of bounds cells are simply ignored.
         */
        void add_connections(const size_t x,
                             const size_t y,
                             float * const connections_array,
                             const float influence_weight) const;

        void propagate(const float momentum, const float decay);
        
    private:
        const size_t _width;
        const size_t _height;
        const bool _clamp_values_to_0_1;
        
        float* _data;
        float* _copy;
        
        size_t coords_to_linear(const size_t x, const size_t y) const;
        float clamp_influence(const float influence) const;
    };
    
} // namespace influence_map

#include "influence_map.inl"
