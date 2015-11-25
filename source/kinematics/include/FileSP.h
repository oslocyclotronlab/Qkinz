#ifndef FILESP_H
#define FILESP_H

#include "StoppingPower.h"

#include "spline.h"

#include <string>

class Material;
class Particle;

class FileSP : public StoppingPower
{
public:
    FileSP(Material *material, Particle *particle) : StoppingPower(material, particle), fileSet( false ){ }

    ~FileSP(){ }

    double Evaluate(const double &E) const; //{ if (fileSet) return -stop(E);
                                            //        else return 0;}

    double Loss(const double &E, const int &points) const;

    double Loss(const double &E, const double &width, const int &points) const;

    void setFile(const std::string &file);

private:
    spline stop;
    
    //double refA;
    //int refZ;

    bool fileSet;

    bool next_line(std::istream &file, std::string &line);

};

#endif // FILESP_H
