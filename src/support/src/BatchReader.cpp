#include "BatchReader.h"

#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QVector>
//#include <algorithm>

const double PI = acos(-1);

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

BatchReader::BatchReader()
    : theBeam(new Beam_t)
    , theTarget(new Target_t)
    , theFront(new Extra_t)
    , theBack(new Extra_t)
    , theTelescope(new Telescope_t)
    , worker_set(false)
    , abs_set(false)
    , CustomPowerPro(false)
    , CustomPowerFrag(false)
    , angleIndices( 0 )
    , want_SiRi( true )
    , dir_siri( 'f' )
{
}

void BatchReader::Start(const QString &batchFile)
{
    if (readBatchFile(batchFile.toStdString())){
        Run();
    } else {
        // Add something that notifies the user that something went horrobly wrong.
    }

    emit FinishedAll();
}

BatchReader::~BatchReader()
{
    delete theBeam;
    delete theTarget;
    delete theFront;
    delete theBack;
    delete theTelescope;
    if (worker_set)
        delete worker;
}

void BatchReader::Run()
{
    std::ifstream inputAngle(anglefile.c_str());
    std::ofstream outputData(outfile.c_str());
    double x;
    int i, j, k=0;
    QVector<double> coef(4);
    outputData << "<index> <a0> <a1> <a2> <chiSq>\n";
    size_t length = count_line(inputAngle);
    if (want_SiRi){
        double angle;
        for (int i = 0 ; i < 8 ; ++i){
            angle = (i*2. + 40.)*PI/180.;
            if (dir_siri == 'b')
                angle = PI - angle;
            if (worker->getCoeff(angle, fragA, fragZ, coef)){
                outputData << i << " ";
                outputData << coef[0] << " ";
                outputData << coef[1] << " ";
                outputData << coef[2] << " ";
                outputData << coef[3] << "\n";
            } else {
                outputData << i << " 0 0 0 0\n";
            }
            emit curr_prog(100*double(i+1)/8.);
        }
    } else {
        while (inputAngle){
            if (angleIndices == 0){
                inputAngle >> x;
                std::cout << "Running angle: " << x << std::endl;
                if (worker->getCoeff(x, fragA, fragZ, coef)){
                    outputData << x << " ";
                    outputData << coef[0] << " ";
                    outputData << coef[1] << " ";
                    outputData << coef[2] << " ";
                    outputData << coef[3] << "\n";
                } else {
                    outputData << x << " 0 0 0 0\n";
                }
            } else if (angleIndices == 1){
                inputAngle >> i;
                inputAngle >> x;
                std::cout << "Running angle: " << x << std::endl;
                if (worker->getCoeff(x, fragA, fragZ, coef)){
                    outputData << i << " ";
                    outputData << coef[0] << " ";
                    outputData << coef[1] << " ";
                    outputData << coef[2] << " ";
                    outputData << coef[3] << "\n";
                } else {
                    outputData << i << " 0 0 0 0\n";
                }
            } else if (angleIndices == 2){
                inputAngle >> i >> j;
                inputAngle >> x;
                std::cout << "Running angle: " << x << std::endl;
                if (worker->getCoeff(x, fragA, fragZ, coef)){
                    outputData << i << " " << j << " ";
                    outputData << coef[0] << " ";
                    outputData << coef[1] << " ";
                    outputData << coef[2] << " ";
                    outputData << coef[3] << "\n";
                } else {
                    outputData << i << " " << j << " 0 0 0 0\n";
                }
            }
            ++k;
            emit curr_prog(100*double(k)/double(length));
        }
    }
    outputData.close();
    inputAngle.close();
    return;
}


bool BatchReader::next_command(const std::string &cmd)
{
    std::istringstream icmd(cmd.c_str());

    std::string name;

    icmd >> name;

    if (name == "output"){
        icmd >> outfile;
        return true;
    } else if (name == "telescope"){
        std::string tmp1, tmp2;
        icmd >> tmp1;
        if (tmp1 == "dE"){
            icmd >> dEZ;
            icmd >> dEW;
            icmd >> tmp2;
            if (tmp2 == "um"){
                dEU = Unit_t::um;
            } else if (tmp2 == "gcm2"){
                dEU = Unit_t::gcm2;
            } else if (tmp2 == "mgcm2"){
                dEU = Unit_t::mgcm2;
            }
            return true;
        } else if (tmp1 == "E"){
            icmd >> EZ;
            icmd >> EW;
            icmd >> tmp2;
            if (tmp2 == "um"){
                EU = Unit_t::um;
            } else if (tmp2 == "gcm2"){
                EU = Unit_t::gcm2;
            } else if (tmp2 == "mgcm2"){
                EU = Unit_t::mgcm2;
            }
            return true;
        } else if (tmp1 == "A"){
            icmd >> AZ;
            icmd >> AW;
            if (tmp2 == "um"){
                AU = Unit_t::um;
            } else if (tmp2 == "gcm2"){
                AU = Unit_t::gcm2;
            } else if (tmp2 == "mgcm2"){
                AU = Unit_t::mgcm2;
            }
            abs_set = true;
            return true;
        }
    } else if (name == "custom"){
        std::string tmp1, tmp2;
        icmd >> tmp1;
        if (tmp1 == "projectile"){
            icmd >> tmp2;
            tStopPro = new CustomPower(tmp2);
            CustomPowerPro = true;
            return true;
        } else if (tmp1 == "fragment"){
            icmd >> tmp2;
            tStopFrag = new CustomPower(tmp2);
            CustomPowerFrag = true;
            return true;
        }
    } else if (name == "projectile"){
        icmd >> proA;
        icmd >> proZ;
        icmd >> proE;
        return true;
    } else if (name == "fragment"){
        icmd >> fragA;
        icmd >> fragZ;
        return true;
    } else if (name == "target"){
        icmd >> tA;
        icmd >> tZ;
        icmd >> tW;
        std::string tmp;
        icmd >> tmp;
        if (tmp == "um"){
            tU = Unit_t::um;
        } else if (tmp == "gcm2"){
            tU = Unit_t::gcm2;
        } else if (tmp == "mgcm2"){
            tU = Unit_t::mgcm2;
        }
        return true;
    } else if (name == "angle"){
        std::string tmp;
        icmd >> tmp;
        if (tmp == "siri"){
            icmd >> dir_siri;
            want_SiRi = true;
            if (dir_siri != 'f' && dir_siri != 'b'){
                return false;
            }
            return true;
        } else if (tmp == "custom"){
            icmd >> angleIndices;
            icmd >> anglefile;
            want_SiRi = false;
            return true;
        }
    } else {
        return false;
    }
    return false;
}

bool BatchReader::readBatchFile(const std::string &batchFile)
{
    std::ifstream batch_file(batchFile.c_str());
    std::string batch_line;
    while (next_commandline(batch_file, batch_line)){
        if (batch_line.size() == 0 || batch_line[0] == '#')
            continue;
        if (!next_command(batch_line)){
            std::cout << "Cannot understand line '" << batch_line << "'" << std::endl;
            return false;
        }
    }
    theBeam->A = proA;
    theBeam->Z = proZ;
    theBeam->E = proE;

    theTarget->A = tA;
    theTarget->Z = tZ;
    theTarget->width = tW;
    theTarget->unit = tU;

    theTelescope->Absorber.Z = AZ;
    theTelescope->Absorber.width = AW;
    theTelescope->Absorber.unit = AU;
    theTelescope->Edetector.Z = EZ;
    theTelescope->Edetector.width = EW;
    theTelescope->Edetector.unit = EU;
    theTelescope->dEdetector.Z = EZ;
    theTelescope->dEdetector.width = dEW;
    theTelescope->dEdetector.unit = dEU;
    theTelescope->has_absorber = abs_set;

    theFront->A = 1;
    theFront->Z = 1;
    theFront->width = 100;
    theFront->unit = Unit_t::mgcm2;
    theFront->is_present = false;
    theBack->A = 1;
    theBack->Z = 1;
    theBack->width = 100;
    theBack->unit = Unit_t::mgcm2;
    theBack->is_present = false;

    worker = new Worker(theBeam, theTarget, theFront, theBack, theTelescope);
    worker_set = true;
    if (CustomPowerPro && CustomPowerFrag){
        worker->setCustomTarget(tStopPro, tStopFrag);
    }
    return true;
}

bool BatchReader::next_commandline(std::istream &in, std::string &cmd_line)
{
    cmd_line = "";
    std::string line;

    while ( getline(in, line) ){
        size_t ls = line.size();
        if ( ls == 0 ){
            break;
        } else if ( line[ls-1] != '\\'){
            cmd_line += line;
            break;
        } else {
            cmd_line += line.substr(0, ls-1);
        }
    }
    return in || !cmd_line.empty();
}
