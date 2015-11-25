#ifndef TYPES
#define TYPES

#include <QVector>

typedef struct {
    int A;      //! Mass number of the beam.
    int Z;      //! Element number of the beam.
    double E;   //! Energy of the beam.
} Beam_t;

typedef struct {
    int A;          //! Mass number of the target isotope.
    int Z;          //! Element number of the target isotope.
    double width;   //! Width of the target (mg/cm^2).
} Target_t;

typedef struct {
    struct Element_str {
        int Z;          //! Element number.
        double width;   //! Element width (um).
    };

    Element_str dEdetector; //! dE detector element.

    Element_str Edetector;  //! E detector element.

    Element_str Absorber;   //! Absorber element.

} Telescope_t;

//! Used to indicate data from what fragment.
enum Fragment_t {
    Proton,     //! Protons.
    Deutron,    //! Deutrons.
    Triton,     //! Tritons.
    Helium3,    //! Helium3s.
    Alpha       //! Alphas.
};

typedef QVector<double> QVec;
//qRegisterMetaType<QVec>("QVec");

#endif // TYPES

