#include "Application.h"

#include "Objects.h"


/// Workflow
/// Direct renderer: Camera, object, rays, intersections
///     Create benchmark
///     Cleanup intersect, choose optimal
///     Intersect info class
///     Material class. What exactly will it do?
///     After figuring out the closest intersect object
/// Photon maps: a lot of photons divided in blocks
/// Photon mapping: light source will emit photons to photon maps
/// Upon intersection camera rays will gather information
///     from photon maps they hit

/// FIXME
/// -O2 -ffast-math -mtune=native

/// TODO
/// Environmental map
/// Check previous version for obvious improvement points
/// Browse another implementations, see what mine is lacking
/// Compare low precision to high precision
/// Add option to lock camera on center
///



int main(int, char**) {
    std::string name;
    std::getline(std::cin, name);
    objectTest();
    Application app;
    app.benchmark();
    //app.start();
    return 0;
}
