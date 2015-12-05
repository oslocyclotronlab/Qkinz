#ifndef TYPES
#define TYPES

#include <QVector>

//! List of avalible units.
enum Unit_t {
    mgcm2,  //! Width unit in mg/cm^2.
    gcm2,   //! Width unit in g/cm^2.
    um      //! Width unit in um.
};

//! Structure to contain the parameters for the beam.
typedef struct {
    int A;      //! Mass number of the beam.
    int Z;      //! Element number of the beam.
    double E;   //! Energy of the beam.
} Beam_t;

//! Structure to contain parameters needed for the target.
typedef struct {
    int A;          //! Mass number of the target isotope.
    int Z;          //! Element number of the target isotope.
    double width;   //! Width of the target (mg/cm^2).
    Unit_t unit;    //! Unit of the width parameter.
} Target_t;

//! Structure to contain parameters needed for the telescope.
typedef struct {
    struct Element_str {
        int Z;          //! Element number.
        double width;   //! Element width (um).
        Unit_t unit;    //! Unit of the width parameter.
    };

    Element_str dEdetector; //! dE detector element.

    Element_str Edetector;  //! E detector element.

    Element_str Absorber;   //! Absorber element.

    bool has_absorber;      //! True if absorber is present, false otherwise.

} Telescope_t;

//! Any extra elements that beam/fragment moves thru.
typedef struct {
    int A;              //! Mass number.
    int Z;              //! Element number.
    double width;       //! Width of the element.
    Unit_t unit;        //! Unit of the element.
    bool is_present;    //! If present or not.
} Extra_t;

//! Used to indicate data from what fragment.
enum Fragment_t {
    Proton,     //! Protons.
    Deutron,    //! Deutrons.
    Triton,     //! Tritons.
    Helium3,    //! Helium3s.
    Alpha       //! Alphas.
};

#endif // TYPES

