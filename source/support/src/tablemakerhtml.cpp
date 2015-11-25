#include "tablemakerhtml.h"

TableMakerHTML::TableMakerHTML()
{

}

void TableMakerHTML::setData(QVector<double> Ex, QVector<double> dE, QVector<double> d_dE, QVector<double> E, QVector<double> d_E, Particle_t what)
{
    if (what == Proton){
        setResult(Ex, dE, d_dE, E, d_E, protons);
    } else if (what == Deutron){
        setResult(Ex, dE, d_dE, E, d_E, deutrons);
    } else if (what == Triton){
        setResult(Ex, dE, d_dE, E, d_E, tritons);
    } else if (what == Helium3){
        setResult(Ex, dE, d_dE, E, d_E, He3s);
    } else if (what == Alpha){
        setResult(Ex, dE, d_dE, E, d_E, alphas);
    }
}

void TableMakerHTML::setCoeff(QVector<double> Coeff, Particle_t what)
{
    if (what == Proton){
        cProtons = Coeff;
    } else if (what == Deutron){
        cDeutrons = Coeff;
    } else if (what == Triton){
        cTritons = Coeff;
    } else if (what == Helium3){
        cHe3s = Coeff;
    } else if (what == Alpha){
        cAlphas = Coeff;
    }
}

QString TableMakerHTML::getHTMLCode()
{
    QString value;
    value = QString("<html>\n<body>\n") + QString("<h1>Results:</h1>\n");
    if (protons.is_set){
        value += QString("<h2>Protons:</h2>\n");
        value += makeTable(protons);
        if (!cProtons.empty())
            value += makeCoeff(cProtons);
    }
    if (deutrons.is_set){
        value += QString("<h2>Deutrons:</h2>\n");
        value += makeTable(deutrons);
        if (!cDeutrons.empty())
            value += makeCoeff(cDeutrons);
    }
    if (tritons.is_set){
        value += QString("<h2>Tritons:</h2>\n");
        value += makeTable(tritons);
        if (!cTritons.empty())
            value += makeCoeff(cTritons);
    }
    if (He3s.is_set){
        value += QString("<h2>Helium-3:</h2>\n");
        value += makeTable(He3s);
        if (!cHe3s.empty())
            value += makeCoeff(cHe3s);
    }
    if (alphas.is_set){
        value += QString("<h2>Alphas:</h2>\n");
        value += makeTable(alphas);
        if (!cAlphas.empty())
            value += makeCoeff(cAlphas);
    }
    value += "</body>\n";
    value += "</html>";
    return value;
}

QString TableMakerHTML::makeTable(Result_t data)
{
    QString output = "";
    output += "<table border=\"1\" style=\"width:100%\">\n";
    output += "<tr>\n";
    output += "<td><b>Excitation energy (keV):</b></td>\n";
    output += "<td>Energy dE-detector (keV):</td>\n";
    output += "<td>Energy E-detector (keV):</td>\n";
    output += "</tr>\n";
    for (int i = 0 ; i < data.Ex.size() ; ++i){
        output += "<tr>\n";
        output += "<td>";
        output += std::to_string(data.Ex[i]*1000).c_str();
        output += "</td>\n";
        output += "<td>";
        output += std::to_string(data.dE[i]*1000).c_str();
        output += " ± ";
        output += std::to_string(data.d_dE[i]*1000).c_str();
        output += "</td>\n";
        output += "<td>";
        output += std::to_string(data.E[i]*1000).c_str();
        output += " ± ";
        output += std::to_string(data.d_E[i]*1000).c_str();
        output += "</td>\n";
        output += "</tr>\n";
    }
    output += "</table>\n";
    return output;
}

QString TableMakerHTML::makeCoeff(QVector<double> coeff)
{
    QString output = "";
    output += "<b>Ex(e+de) = a0 + a1(e+de) + a2(e+de)^2</b>\n";
    output += "<p>a0 = ";
    output += std::to_string(coeff[0]*1000).c_str();
    output += " keV, a1 = ";
    output += std::to_string(coeff[1]*1000).c_str();
    output += ", a2 = ";
    output += std::to_string(coeff[2]*1000).c_str();
    output += " keV^-1</p>\n";
    return output;
}
