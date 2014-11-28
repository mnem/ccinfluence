#include "catch.hpp"
#include "influence_map.h"

using namespace influence_map;

TEST_CASE( "Unclamped influence maps can be created", "[InfluenceMap]" ) {
    const size_t width = 3;
    const size_t height = 5;
    const bool clamped = false;
    const float initial_value = 100.0f;
    
    InfluenceMap map(width, height, clamped, initial_value);
    
    REQUIRE( map.width() == 3 );
    REQUIRE( map.height() == 5 );
    REQUIRE( map.num_cells() == 15 );
    
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            const float cell_value = map.influence(x, y);
            REQUIRE( cell_value == 100.0f );
        }
    }
}

TEST_CASE( "Clamped influence maps can be created", "[InfluenceMap]" ) {
    const size_t width = 3;
    const size_t height = 5;
    const bool clamped = true;
    
    SECTION( "initial value within bounds" ) {
        const float initial_value = 0.5f;
        InfluenceMap map(width, height, clamped, initial_value);
        
        REQUIRE( map.width() == 3 );
        REQUIRE( map.height() == 5 );
        REQUIRE( map.num_cells() == 15 );
        
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                const float cell_value = map.influence(x, y);
                REQUIRE( cell_value == 0.5f );
            }
        }
    }
    
    SECTION( "initial value above bounds" ) {
        const float initial_value = 1.5f;
        InfluenceMap map(width, height, clamped, initial_value);
        
        REQUIRE( map.width() == 3 );
        REQUIRE( map.height() == 5 );
        REQUIRE( map.num_cells() == 15 );
        
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                const float cell_value = map.influence(x, y);
                REQUIRE( cell_value == 1.0f );
            }
        }
    }
    
    SECTION( "initial value below bounds" ) {
        const float initial_value = -1.5f;
        InfluenceMap map(width, height, clamped, initial_value);
        
        REQUIRE( map.width() == 3 );
        REQUIRE( map.height() == 5 );
        REQUIRE( map.num_cells() == 15 );
        
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                const float cell_value = map.influence(x, y);
                REQUIRE( cell_value == 0.0f );
            }
        }
    }
}
