#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <Material.h>
#include <Particle.h>

TEST_CASE( "Particle", "[Particle]" ) {
    SECTION("Look-up") {
        Particle particle(1, 1);
        REQUIRE(particle.GetM_AMU() == Approx(1.0078250322));
    }
}

TEST_CASE( "Material", "[Material]" ) {
    SECTION("Setting up") {
        Material material(14, 28, 200, Material::um);
        REQUIRE(material.Getvfermi() == Approx(0.97411));
    }
}
