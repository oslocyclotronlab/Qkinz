#include "CustomPower.h"


#include <fstream>
#include <vector>

static size_t count_line(std::istream &is)
{
    // skip when bad
    if( is.bad() ) return 0;
    // save state
    std::istream::iostate state_backup = is.rdstate();
    // clear state
    is.clear();
    std::istream::streampos pos_backup = is.tellg();

    is.seekg(0);
    size_t line_cnt;
    size_t lf_cnt = std::count(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>(), '\n');
    line_cnt = lf_cnt;
    // if the file is not end with '\n' , then line_cnt should plus 1
    is.unget();
    if( is.get() != '\n' ) { ++line_cnt ; }

    // recover state
    is.clear() ; // previous reading may set eofbit
    is.seekg(pos_backup);
    is.setstate(state_backup);

    return line_cnt;
}

CustomPower::CustomPower(const std::string &str_file)
{
    std::ifstream inputData(str_file.c_str());
    size_t length = count_line(inputData);
    std::vector<double> xval(length), yval(length);
    double x, y;
    size_t i = 0;
    while (inputData){
        inputData >> x >> y;
        xval[i] = x;
        yval[i] = y;
        ++i;
    }
    inputData.close();
    xmin = 1000;
    xmax = -1000;
    for (size_t i = 0 ; i < length ; ++i){
        if (xval[i] < xmin)
            xmin = xval[i];
        if (xval[i] > xmax)
            xmax = xval[i];
    }

    SP.set_points(xval, yval);

}

double CustomPower::Evaluate(const double &E) const
{
    if (E < xmin)
        return 0;
    if (E > xmax)
        return -SP(xmax - 25.);
    return -SP(E);
}

CustomPower::~CustomPower(){ }

double CustomPower::Loss(const double &E, const double &width, const int &points) const
{
    double dx = width/double(points - 1);
    double e = E;
    double K1, K2, K3, K4;
    for (int i = 0 ; i < points ; ++i){
        K1 = dx*Evaluate(e);
        K2 = dx*Evaluate(e + 0.5*K1);
        K3 = dx*Evaluate(e + 0.5*K2);
        K4 = dx*Evaluate(e + K3);
        e += (K1 + 2*(K2 + K3) + K4)/6.0;
        if (e != e || e < 0)
            return 0;
    }
    return e;
}

adouble CustomPower::Loss(adouble E, double width, int points)
{
    double dx = width/double(points - 1);
    adouble e = E;
    adouble R1(E.size()), R2(E.size()), R3(E.size()), R4(E.size());
    for (int i = 0 ; i < points ; ++i){
        for (int j = 0 ; j < E.size() ; ++j){
            R1[j] = dx*Evaluate(e[j]);
            R2[j] = dx*Evaluate(e[j] + 0.5*R1[j]);
            R3[j] = dx*Evaluate(e[j] + 0.5*R2[j]);
            R4[j] = dx*Evaluate(e[j] + R3[j]);
        }
        e += (R1 + 0.5*(R2 + R3) + R4)/6.0;
    }
    e[e<0] = 0.0;
    e[e!=e] = 0.0;
    return e;
}
