#include "StoppingPower.h"

#include "Material.h"
#include "Particle.h"


StoppingPower::StoppingPower(Material *material, Particle *particle)
    : AbstractFunction()
    , pMaterial( material )
	, pParticle( particle ){ }


void StoppingPower::setMaterial(Material *material) 
{
    pMaterial = material;
}

void StoppingPower::setParticle(Particle *particle) 
{
    pParticle= particle;
}
