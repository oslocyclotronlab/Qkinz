#ifndef BATCHREADER_H
#define BATCHREADER_H

#include <QObject>
#include <QThread>
#include <QString>
#include <string>
#include "types.h"
#include "worker.h"
#include "CustomPower.h"

class BatchReader : public QObject
{
    Q_OBJECT
public:
    BatchReader();

	~BatchReader();

public slots:
    void Start(const QString &batchFile);

signals:
    void FinishedAll();
    void curr_prog(double);

private:
    bool readBatchFile(const std::string &batchFile);
	void Run();

	bool next_commandline(std::istream &in, std::string &cmd_line);
    bool next_command(const std::string &line);

	 //! The current beam.
    Beam_t *theBeam;

    //! The current target.
    Target_t *theTarget;

    //! Fronting of the target.
    Extra_t *theFront;

    //! Backing of the target.
    Extra_t *theBack;

    //! The current telescope.
    Telescope_t *theTelescope;

    Worker *worker;
    bool worker_set;

    int dEZ, EZ, AZ;
    double dEW, EW, AW;
    Unit_t dEU, EU, AU;
    bool abs_set;

    int tZ, tA;
    double tW;
    Unit_t tU;

    int proA, proZ;
    double proE;
    int fragA, fragZ;

    int angleIndices;

    bool want_SiRi;
    char dir_siri;

    CustomPower *tStopPro;
    CustomPower *tStopFrag;
    bool CustomPowerPro, CustomPowerFrag;

    std::string anglefile;
    std::string outfile;
};

#endif // BATCHREADER
