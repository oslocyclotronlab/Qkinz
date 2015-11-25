#include "FileSP.h"

#include "Material.h"
#include "Particle.h"

#include <cstdlib>
#include <sstream>
#include <ostream>
#include <fstream>
#include <vector>
#include <iostream>

void FileSP::setFile(const std::string &file)
{
    std::ifstream input(file.c_str());
    std::string line;

    std::vector<double> E;
    std::vector<double> Sp;
    while ( next_line(input, line) ){
        std::stringstream icmd(line.c_str());
        double e, se, sn;
        icmd >> e;
        icmd >> se;
        icmd >> sn;
        E.push_back(e);
        Sp.push_back(se*1e-3 + sn*1e-3);
    }
    stop.set_points(E, Sp);
    fileSet = true;
    input.close();
}

double FileSP::Evaluate(const double &E) const
{
    if (!fileSet)
        return 0;
    return -stop(E);
}

double FileSP::Loss(const double &E, const int &points) const
{
    if (E < 0.011) return 0;
    double d = pMaterial->GetWidth(Material::mgcm2);
    double dx = d/(points-1);
    double e=E, R1, R2, R3, R4;
    for (int i = 0 ; i < points ; ++i){
        R1 = dx*Evaluate(e);
        if (e + 0.5*R1 < 0.011) return 0;
        R2 = dx*Evaluate(e + 0.5*R1);
        if (e + 0.5*R2 < 0.011) return 0;
        R3 = dx*Evaluate(e + 0.5*R2);
        if (e + R3 < 0.011) return 0;
        R4 = dx*Evaluate(e + R3);
        e += (R1 + 2*(R2 + R3) + R4)/6.0;
        if (e < 0.011) return 0;
    }
    return e;
}

double FileSP::Loss(const double &E, const double &width, const int &points) const
{
    if (E < 0.011) return 0;
    double dx = width/(points - 1);
    double e=E, R1, R2, R3, R4;
    for (int i = 0 ; i < points ; ++i){
        R1 = dx*Evaluate(e);
        if (e + 0.5*R1 < 0.011) return 0;
        R2 = dx*Evaluate(e + 0.5*R1);
        if (e + 0.5*R2 < 0.011) return 0;
        R3 = dx*Evaluate(e + 0.5*R2);
        if (e + R3 < 0.011) return 0;
        R4 = dx*Evaluate(e + R3);
        e += (R1 + 2*(R2 + R3) + R4)/6.0;
        if (e < 0.011) return 0;
    }
    return e;
}

bool FileSP::next_line(std::istream &in, std::string &cmd_line)
{
    cmd_line = "";
    std::string line;
    while ( getline(in, line) ){
        int ls = line.size();
        if ( ls == 0 ){
            break;
        } else if ( line[ls-1] != '\\' ) {
            cmd_line += line;
            break;
        } else {
            cmd_line += line.substr(0, ls-1);
        }
    }
    return in || !cmd_line.empty();
}
