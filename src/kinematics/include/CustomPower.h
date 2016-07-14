#ifndef CUSTOM_POWER_H
#define CUSTOM_POWER_H

#include <string>
#include <vector>
#include "spline.h"
#include "types.h"


class CustomPower
{
public:
    CustomPower(const std::string &str_file);

    ~CustomPower();

    double Evaluate(const double &E) const;

	double Loss(const double &E,
				const double &width,
				const int &points=1001) const;

    adouble Loss(adouble E, double width, int points=1001);
private:
    spline SP;
    double xmin;
    double xmax;

};

#endif // CUSTOM_POWER_H
