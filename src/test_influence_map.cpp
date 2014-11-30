#include "catch.hpp"
#include "influence_map.h"

#include <cmath>
#define CLOSE_ENOUGH(a, b, tolerance) fabs((a) - (b)) <= (tolerance)

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

TEST_CASE( "unclamped influence can be set and read", "[InfluenceMap]" ) {
    const size_t width = 3;
    const size_t height = 3;
    const bool clamped = false;
    const float initial_value = 0.0f;
    
    InfluenceMap map(width, height, clamped, initial_value);
    
    REQUIRE( map.num_cells() == 9 );
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            const float cell_value = map.influence(x, y);
            REQUIRE( cell_value == 0.0f );
        }
    }
    
    // Top row
    map.set_influence(0, 0, 1.0f);
    map.set_influence(1, 0, 2.0f);
    map.set_influence(2, 0, 3.0f);
    // Middle row
    map.set_influence(0, 1, 4.0f);
    map.set_influence(1, 1, 5.0f);
    map.set_influence(2, 1, 6.0f);
    // Bottom row
    map.set_influence(0, 2, 7.0f);
    map.set_influence(1, 2, 8.0f);
    map.set_influence(2, 2, 9.0f);
    
    REQUIRE( map.influence(0, 0) == 1.0f );
    REQUIRE( map.influence(1, 0) == 2.0f );
    REQUIRE( map.influence(2, 0) == 3.0f );
    REQUIRE( map.influence(0, 1) == 4.0f );
    REQUIRE( map.influence(1, 1) == 5.0f );
    REQUIRE( map.influence(2, 1) == 6.0f );
    REQUIRE( map.influence(0, 2) == 7.0f );
    REQUIRE( map.influence(1, 2) == 8.0f );
    REQUIRE( map.influence(2, 2) == 9.0f );
}

TEST_CASE( "clamped influence can be set and read", "[InfluenceMap]" ) {
    const size_t width = 3;
    const size_t height = 3;
    const bool clamped = true;
    const float initial_value = 0.0f;
    
    InfluenceMap map(width, height, clamped, initial_value);
    
    REQUIRE( map.num_cells() == 9 );
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            const float cell_value = map.influence(x, y);
            REQUIRE( cell_value == 0.0f );
        }
    }

    SECTION( "all influence within bounds" ) {
        // Top row
        map.set_influence(0, 0, 0.1f);
        map.set_influence(1, 0, 0.2f);
        map.set_influence(2, 0, 0.3f);
        // Middle row
        map.set_influence(0, 1, 0.4f);
        map.set_influence(1, 1, 0.5f);
        map.set_influence(2, 1, 0.6f);
        // Bottom row
        map.set_influence(0, 2, 0.7f);
        map.set_influence(1, 2, 0.8f);
        map.set_influence(2, 2, 0.9f);
        
        REQUIRE( map.influence(0, 0) == 0.1f );
        REQUIRE( map.influence(1, 0) == 0.2f );
        REQUIRE( map.influence(2, 0) == 0.3f );
        REQUIRE( map.influence(0, 1) == 0.4f );
        REQUIRE( map.influence(1, 1) == 0.5f );
        REQUIRE( map.influence(2, 1) == 0.6f );
        REQUIRE( map.influence(0, 2) == 0.7f );
        REQUIRE( map.influence(1, 2) == 0.8f );
        REQUIRE( map.influence(2, 2) == 0.9f );
    }
    SECTION( "some influence outwith bounds" ) {
        // Top row
        map.set_influence(0, 0,-0.1f);
        map.set_influence(1, 0, 0.2f);
        map.set_influence(2, 0, 1.3f);
        // Middle row
        map.set_influence(0, 1,-0.4f);
        map.set_influence(1, 1, 0.5f);
        map.set_influence(2, 1, 1.6f);
        // Bottom row
        map.set_influence(0, 2,-0.7f);
        map.set_influence(1, 2, 0.8f);
        map.set_influence(2, 2, 1.9f);
        
        REQUIRE( map.influence(0, 0) == 0.0f );
        REQUIRE( map.influence(1, 0) == 0.2f );
        REQUIRE( map.influence(2, 0) == 1.0f );
        REQUIRE( map.influence(0, 1) == 0.0f );
        REQUIRE( map.influence(1, 1) == 0.5f );
        REQUIRE( map.influence(2, 1) == 1.0f );
        REQUIRE( map.influence(0, 2) == 0.0f );
        REQUIRE( map.influence(1, 2) == 0.8f );
        REQUIRE( map.influence(2, 2) == 1.0f );
    }
}

TEST_CASE( "connecting influences can be read", "[InfluenceMap]" ) {
    const size_t width = 3;
    const size_t height = 3;
    const bool clamped = true;
    const float initial_value = 0.0f;
    float connections[InfluenceMap::CONNECTIONS_ARRAY_LENGTH];
    
    InfluenceMap map(width, height, clamped, initial_value);
    
    // Top row
    map.set_influence(0, 0, 0.1f);
    map.set_influence(1, 0, 0.2f);
    map.set_influence(2, 0, 0.3f);
    // Middle row
    map.set_influence(0, 1, 0.4f);
    map.set_influence(1, 1, 0.5f);
    map.set_influence(2, 1, 0.6f);
    // Bottom row
    map.set_influence(0, 2, 0.7f);
    map.set_influence(1, 2, 0.8f);
    map.set_influence(2, 2, 0.9f);
    
    SECTION( "around top left" ) {
        map.connections(0, 0, connections, 1, 0);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 0.0f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 0.0f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 0.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 0.2f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 0.5f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 0.4f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 0.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 0.0f );
    }
    SECTION( "around top middle" ) {
        map.connections(1, 0, connections, 1, 0);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 0.0f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 0.0f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 0.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 0.3f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 0.6f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 0.5f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 0.4f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 0.1f );
    }
    SECTION( "around top right" ) {
        map.connections(2, 0, connections, 1, 0);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 0.0f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 0.0f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 0.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 0.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 0.0f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 0.6f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 0.5f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 0.2f );
    }
    SECTION( "around middle left" ) {
        map.connections(0, 1, connections, 1, 0);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 0.0f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 0.1f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 0.2f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 0.5f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 0.8f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 0.7f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 0.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 0.0f );
    }
    SECTION( "around middle middle" ) {
        map.connections(1, 1, connections, 1, 0);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 0.1f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 0.2f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 0.3f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 0.6f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 0.9f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 0.8f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 0.7f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 0.4f );
    }
    SECTION( "around middle right" ) {
        map.connections(2, 1, connections, 1, 0);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 0.2f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 0.3f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 0.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 0.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 0.0f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 0.9f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 0.8f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 0.5f );
    }
    SECTION( "around bottom left" ) {
        map.connections(0, 2, connections, 1, 0);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 0.0f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 0.4f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 0.5f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 0.8f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 0.0f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 0.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 0.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 0.0f );
    }
    SECTION( "around bottom middle" ) {
        map.connections(1, 2, connections, 1, 0);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 0.4f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 0.5f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 0.6f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 0.9f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 0.0f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 0.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 0.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 0.7f );
    }
    SECTION( "around bottom right" ) {
        map.connections(2, 2, connections, 1, 0);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 0.5f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 0.6f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 0.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 0.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 0.0f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 0.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 0.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 0.8f );
    }
    SECTION( "around middle middle with correct weighting" ) {
        map.connections(1, 1, connections, 10, 0);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 1.0f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 2.0f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 3.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 6.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 9.0f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 8.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 7.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 4.0f );
    }
}

TEST_CASE( "connecting influences can be added", "[InfluenceMap]" ) {
    const size_t width = 3;
    const size_t height = 3;
    const bool clamped = true;
    const float initial_value = 0.0f;
    
    InfluenceMap map(width, height, clamped, initial_value);
    
    // Top row
    map.set_influence(0, 0, 0.1f);
    map.set_influence(1, 0, 0.2f);
    map.set_influence(2, 0, 0.3f);
    // Middle row
    map.set_influence(0, 1, 0.4f);
    map.set_influence(1, 1, 0.5f);
    map.set_influence(2, 1, 0.6f);
    // Bottom row
    map.set_influence(0, 2, 0.7f);
    map.set_influence(1, 2, 0.8f);
    map.set_influence(2, 2, 0.9f);

    float connections[InfluenceMap::CONNECTIONS_ARRAY_LENGTH] = {1, 1, 1, 1, 1, 1, 1, 1};
    
    SECTION( "unweighted" ) {
        map.add_connections(1, 1, connections, 1);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 1.1f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 1.2f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 1.3f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 1.6f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 1.9f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 1.8f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 1.7f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 1.4f );
    }
    
    SECTION( "weighted" ) {
        map.add_connections(1, 1, connections, 100);
        
        const float tolerance = 0.00001f;
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::TopLeft], 11.0f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::TopMiddle], 21.0f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::TopRight], 31.0f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::MiddleRight], 61.0f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::BottomRight], 91.0f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::BottomMiddle], 81.0f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::BottomLeft], 71.0f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::MiddleLeft], 41.0f, tolerance) );
    }
}

TEST_CASE( "unclamped propagation", "[InfluenceMap]" ) {
    const size_t width = 3;
    const size_t height = 3;
    const bool clamped = false;
    const float initial_value = 0.0f;
    
    InfluenceMap map(width, height, clamped, initial_value);
    
    map.set_influence(1, 1, 1000);
    
    SECTION( "with no decay or momentum" ) {
        const float momentum = 0.0f;
        const float decay = 0.0f;
        
        map.propagate(momentum, decay);
        
        REQUIRE(map.influence(1, 1) == 1000.0f);
        
        float connections[InfluenceMap::CONNECTIONS_ARRAY_LENGTH];
        map.connections(1, 1, connections, 1, -1);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 0.0f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 0.0f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 0.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 0.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 0.0f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 0.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 0.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 0.0f );
    }
    
    SECTION( "with 1.0 decay and 0.5 momentum" ) {
        const float momentum = 0.5f;
        const float decay = 1.0f;
        
        map.propagate(momentum, decay);
        
        REQUIRE(map.influence(1, 1) == 500.0f);
        
        float connections[InfluenceMap::CONNECTIONS_ARRAY_LENGTH];
        map.connections(1, 1, connections, 1, -1);
        
        const float tolerance = 0.1f;
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::TopLeft], 121.5f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::TopMiddle], 183.9f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::TopRight], 121.5f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::MiddleRight], 183.9f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::BottomRight], 121.5f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::BottomMiddle], 183.9f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::BottomLeft], 121.5f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::MiddleLeft], 183.9f, tolerance) );
    }
}

TEST_CASE( "clamped propagation", "[InfluenceMap]" ) {
    const size_t width = 3;
    const size_t height = 3;
    const bool clamped = true;
    const float initial_value = 0.0f;
    
    InfluenceMap map(width, height, clamped, initial_value);
    
    map.set_influence(1, 1, 1000);
    
    SECTION( "with no decay or momentum" ) {
        const float momentum = 0.0f;
        const float decay = 0.0f;
        
        map.propagate(momentum, decay);
        
        REQUIRE(map.influence(1, 1) == 1.0f);
        
        float connections[InfluenceMap::CONNECTIONS_ARRAY_LENGTH];
        map.connections(1, 1, connections, 1, -1);
        
        REQUIRE( connections[     InfluenceMap::ConnectionIndex::TopLeft] == 0.0f );
        REQUIRE( connections[   InfluenceMap::ConnectionIndex::TopMiddle] == 0.0f );
        REQUIRE( connections[    InfluenceMap::ConnectionIndex::TopRight] == 0.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::MiddleRight] == 0.0f );
        REQUIRE( connections[ InfluenceMap::ConnectionIndex::BottomRight] == 0.0f );
        REQUIRE( connections[InfluenceMap::ConnectionIndex::BottomMiddle] == 0.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::BottomLeft] == 0.0f );
        REQUIRE( connections[  InfluenceMap::ConnectionIndex::MiddleLeft] == 0.0f );
    }
    
    SECTION( "with 1.0 decay and 0.5 momentum" ) {
        const float momentum = 0.5f;
        const float decay = 1.0f;
        
        map.propagate(momentum, decay);
        
        REQUIRE(map.influence(1, 1) == 0.5f);
        
        float connections[InfluenceMap::CONNECTIONS_ARRAY_LENGTH];
        map.connections(1, 1, connections, 1, -1);
        
        const float tolerance = 0.0001f;
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::TopLeft], 0.1215f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::TopMiddle], 0.1839f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::TopRight], 0.1215f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::MiddleRight], 0.1839f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::BottomRight], 0.1215f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::BottomMiddle], 0.1839f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::BottomLeft], 0.1215f, tolerance) );
        REQUIRE( CLOSE_ENOUGH(connections[InfluenceMap::ConnectionIndex::MiddleLeft], 0.1839f, tolerance) );
    }
}

