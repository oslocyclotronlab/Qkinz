#include "Material.h"

#include "ziegler1985_table.h"
#include "ame2012_masses.h"

#include <iostream>

Material::Material()
    : Z( 0 )
    , A( 0 )
    , width( 0 )
    , currUnit( none )
    , retrive( false ){ }

Material::Material(const int &_Z, const int &_A, const double &_width, Unit _currUnit)
	: Z( _Z )
	, A( _A )
    , width( _width )
    , currUnit( _currUnit )
{	//! Variable telling if retrival of variable was successful or not.
	if (!retriveVariables()){
		std::cerr << "Error retriving data. Shure element no. " << Z << " excists?" << std::endl;
		retrive = false;
	} else
		retrive = true;
}

Material::Material(const Material &material)
    : Material(material.Z, material.A, material.width, material.currUnit){}

Material::~Material()
{
	if (retrive)
		delete[] pcoef;
}

Material &Material::operator=(const Material &material)
{
    this->setMaterial(material.Z, material.A, material.width, material.currUnit);
    return *this;
}

void Material::setMaterial(const int &_Z, const int &_A, const double &_width, Unit _currUnit)
{
	Z = _Z;
	A = _A;
    if (_width > 0){
        width = _width;
    }
    if (_currUnit != none)
        currUnit = _currUnit;

	if (!retriveVariables()){
		std::cerr << "Error retriving data. Shure element no. " << Z << " excists?" << std::endl;
		retrive = false;
	} else 
		retrive = true;
}

void Material::setWidth(const double &_width, Unit _currUnit)
{
    width = _width;
    currUnit = _currUnit;
}

bool Material::retriveVariables()
{
	mAMU = get_mass_amu(A, Z);
	mMeV = mAMU*931.494003;
	rho = Get_rho(Z);
	atrho = Get_atrho(Z);
	vfermi = Get_vfermi(Z);
	pcoef = new double[9];
	if (!Get_pcoef(Z, pcoef))
		Ziegler = false;
	else
		Ziegler = true;
    return true;
}
