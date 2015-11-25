#include "Particle.h"

#include "ziegler1985_table.h"
#include "ame2012_masses.h"

#include <iostream>

Particle::Particle()
    : Z( 0 )
    , A( 0 ){}

Particle::Particle(const int &_Z, const int &_A)
	: Z( _Z )
	, A( _A )
{
	Ziegler = retriveVariables();
	if (!Ziegler)
		std::cerr << "Error retriving variables. Shure element(A,Z) (" << A << ", " << Z << ") is a valid isotope?" << std::endl;
}

Particle::Particle(const Particle &particle)
    : Particle(particle.Z, particle.A){}

Particle::~Particle() { }

Particle &Particle::operator=(const Particle &particle)
{
    this->setParticle(particle.Z, particle.A);
    return *this;
}

void Particle::setParticle(const int &_Z, const int &_A)
{
	Z = _Z;
	A = _A;
	Ziegler = retriveVariables();
	if (!Ziegler)
		std::cerr << "Error retriving variables. Shure element(A,Z) (" << A << ", " << Z << ") is a valid isotope?" << std::endl;
}

bool Particle::retriveVariables()
{
	mAMU = get_mass_amu(A, Z);
	mMeV = mAMU*931.494003;
	lfctr = Get_lfctr(Z);
	if (lfctr >= 0)
		return true;
	return false;
}
