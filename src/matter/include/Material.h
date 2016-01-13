#ifndef MATERIAL_H
#define MATERIAL_H


#include <cmath>

//! Class to contain information about an material.
/*! When initilized, this class will retrive all the
 *  data required for both calculations using Ziegler
 *  or the Bethe-Block calculations.
 */
class Material
{
public:
    typedef enum {
        none,   //! If width not set.
        gcm2,   //! Unit of width is [g/cm²].
        mgcm2,  //! Unit of width is [mg/cm²].
        um,     //! Unit of width is [µm].
    } Unit;

    //! Empty constructor.
    Material();

	//! Constructor.
    Material(const int &_Z,             /*!< Element number of material.        */
             const int &_A,             /*!< Mass number of material.           */
             const double &_width = -1, /*!< Width of the material in [unit].   */
             Unit _currUnit=none        /*!< Unit of the width parameter.       */);

    //! Copy constructor.
    Material(const Material &material);

	//! Destructor.
	~Material();

    //! Assign operator.
    Material &operator=(const Material &material);

	//! Set/change material.
    void setMaterial(const int &_Z, /*!< Element number of material.                    */
                     const int &_A,	/*!< Mass number of material.                       */
                     const double &_width = -1, /*!< Width of the material in [unit].   */
                     Unit _currUnit=none        /*!< Unit of the width parameter.       */);

    //! Change width.
    void setWidth(const double &_width, /*!< New width of the material.     */
                  Unit _currUnit        /*!< Unit of the new width.         */);

	// Functions to get the different values needed for performing of stopping-power calculations.

	//! Get element number.
	/*! \return element number. */
	int GetZ() const;

	//! Get mass number.
	/*! \return mass number if set, -1 if not. */
	int GetA() const;

	//! Get mass.
	/*! \return mass in [AMU]. */
	double GetM_AMU() const;

	//! Get mass.
	/*! \return mass in [MeV]. */
	double GetM_MeV() const;

	//! Get density.
	/*! \return density in [g/cm³]. */
	double Getrho() const;

	//! Get density.
	/*! \return density in [1e22/cm³]. */
	double Getatrho() const;

	//! Get vfermi.
	/*! \return vfermi in [Bohr v]. */
	double Getvfermi() const;

    //! Get mean excitation potential.
    /*! \return Mean excitation potential in [MeV].
     */
    double GetMeanEx() const;

	//! Get pcoef.
	/*! \return proton stopping cross section coefficients. */
	double Getpcoef(const int &i) const;

    //! Get pointer to pcoef.
    /*! \return proton stopping cross section coefficients. */
    double *Getpcoef_ptr() const;

    //! Get the width of the material.
    /*! \return Width of material in [g/cm²].
     */
    double GetWidth(Unit what=um /*!< What kind of units to return.  */) const;


private:
	//! Variable to contain the element number.
	int Z;

	//! Variable to contain the element mass number. If -1, assume average isotopic composition.
	int A;

	//! Variable to contain the weight of the material in [AMU].
	double mAMU;

	//! Variable to contain the weight of the material in [MeV/c²].
	double mMeV;

	//! Variable to contain the density of the material [g/cm³].
	double rho;

	//! Variable to contain the density of the material [1e22/cm³].
	double atrho;

	//! Variable to contain the vfermi of the material [Bohr v].
	double vfermi;

	//! Variable to contain the proton stopping cross section coefficients.
	double *pcoef;

	//! Variable telling if data needed for Ziegler1985 excists.
	bool Ziegler;

    //! Variable to store the width of the target.
    double width;

    //! Variable to tell what units are used.
    Unit currUnit;

    //! Variable telling if retrival of variable was successful or not.
    bool retrive;

	//! Helper function to retrive variables.
	/*! \return true if successful, false otherwise.
	 */
	bool retriveVariables();
};

inline int Material::GetZ() const { return Z; }

inline int Material::GetA() const { return A; }

inline double Material::GetM_AMU() const { return mAMU; }

inline double Material::GetM_MeV() const { return mMeV; }

inline double Material::Getrho() const { return rho; }

inline double Material::Getatrho() const { return atrho; }

inline double Material::Getvfermi() const { return vfermi; }

inline double Material::GetMeanEx() const
{
    if (Z < 13)
        return (12*Z + 7)*1e-6;
    else
        return Z*(9.76 + 58.8*pow(Z, -1.19))*1e-6;
}

inline double Material::Getpcoef(const int &i) const { return pcoef[i]; }
inline double *Material::Getpcoef_ptr() const { return pcoef; }

inline double Material::GetWidth(Unit what) const
{
    if (what == currUnit || what == none)
        return width;
    else if (what == gcm2 && currUnit == mgcm2)
        return width*1e-3;
    else if (what == mgcm2 && currUnit == gcm2)
        return width*1e3;
    else if (what == gcm2 && currUnit == um)
        return width*rho*1e-4;
    else if (what == um && currUnit == gcm2)
        return width/(rho*1e-4);
    else if (what == mgcm2 && currUnit == um)
        return width*rho/10.;
    else if (what == um && currUnit == mgcm2)
        return width*10./rho;
    else
        return -1;
}

#endif // MATERIAL_H
