#include "tablemakerhtml.h"

#include <cmath>

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
    } else if (what == Other){
        setResult(Ex, dE, d_dE, E, d_E, others);
    }
}

void TableMakerHTML::setCurve(QVector<double> ex, QVector<double> x, QVector<double> y, Particle_t what)
{
    if (what == Proton){
        protons_res = {ex, x, y};
    } else if (what == Deutron){
        deutrons_res = {ex, x, y};
    } else if (what == Triton){
        tritons_res = {ex, x, y};
    } else if (what == Helium3){
        He3s_res = {ex, x, y};
    } else if (what == Alpha){
        alphas_res = {ex, x, y};
    } else if (what == Other){
        others_res = {ex, x, y};
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
    } else if (what == Other){
        cOthers = Coeff;
    }
}

QString TableMakerHTML::getHTMLCode()
{
    QString value;
    value = QString("<html>\n<body>\n") + QString("<h1>Results:</h1>\n");
    if (protons.is_set){
        value += QString("<h2>Protons:</h2>\n");
        value += makeTable(protons);
        value += makeCurve(protons_res);
        if (!cProtons.empty())
            value += makeCoeff(cProtons);
    }
    if (deutrons.is_set){
        value += QString("<h2>Deutrons:</h2>\n");
        value += makeTable(deutrons);
        value += makeCurve(deutrons_res);
        if (!cDeutrons.empty())
            value += makeCoeff(cDeutrons);
    }
    if (tritons.is_set){
        value += QString("<h2>Tritons:</h2>\n");
        value += makeTable(tritons);
        value += makeCurve(tritons_res);
        if (!cTritons.empty())
            value += makeCoeff(cTritons);
    }
    if (He3s.is_set){
        value += QString("<h2>Helium-3:</h2>\n");
        value += makeTable(He3s);
        value += makeCurve(He3s_res);
        if (!cHe3s.empty())
            value += makeCoeff(cHe3s);
    }
    if (alphas.is_set){
        value += QString("<h2>Alphas:</h2>\n");
        value += makeTable(alphas);
        value += makeCurve(alphas_res);
        if (!cAlphas.empty())
            value += makeCoeff(cAlphas);
    }
    if (others.is_set){
        value += QString("<h2>Others:</h2>\n");
        value += makeTable(others);
        value += makeCurve(others_res);
        if (!cOthers.empty())
            value += makeCoeff(cOthers);
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
    output += "<td>Total particle energy (keV):</td>\n";
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
        output += "<td>";
        output += std::to_string((data.dE[i]+data.E[i])*1000).c_str();
        output += " ± ";
        output += std::to_string(sqrt(data.d_dE[i]*data.d_dE[i] + data.d_E[i]*data.d_E[i])*1000).c_str();
        output += "</td>\n";
        output += "</tr>\n";
    }
    output += "</table>\n";
    return output;
}

QString TableMakerHTML::makeCurve(Res_t data)
{
    QString output = "";
    output += "<table border=\"1\" style=\"width:100%\">\n";
    output += "<tr>\n";
    output += "<td><b>Excitation energy (keV):</b></td>\n";
    output += "<td>Energy dE-detector (keV):</td>\n";
    output += "<td>Energy E-detector (keV):</td>\n";
    output += "<td>Total particle energy (keV):</td>\n";
    output += "</tr>\n";
    for (int i = 0 ; i < data.ex.size() ; ++i){
        output += "<tr>\n";
        output += "<td>";
        output += std::to_string(data.ex[i]*1000).c_str();
        output += "</td>\n";
        output += "<td>";
        output += std::to_string(data.y[i]*1000).c_str();
        output += "</td>\n";
        output += "<td>";
        output += std::to_string(data.x[i]*1000).c_str();
        output += "</td>\n";
        output += "<td>";
        output += std::to_string((data.y[i]+data.x[i])*1000).c_str();
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
    output += "<p>chi-squared: ";
    output += std::to_string(coeff[3]).c_str();
    output += ", a0 = ";
    output += std::to_string(coeff[0]).c_str();
    output += " MeV, a1 = ";
    output += std::to_string(coeff[1]).c_str();
    output += ", a2 = ";
    output += std::to_string(coeff[2]).c_str();
    output += " MeV^-1</p>\n";
    return output;
}

QString TableMakerHTML::getTXT()
{
    QString value = "";

    value += "This file was generated by QKinz.\n\n\n";

    if (protons.is_set){
        value += "Protons:\n";
        value += makeTableTXT(protons);
        if (!cProtons.empty())
            value += makeCoeffTXT(cProtons);
        value += "\n\n\n";
    }
    if (deutrons.is_set){
        value += QString("Deutrons:\n");
        value += makeTableTXT(deutrons);
        if (!cDeutrons.empty())
            value += makeCoeffTXT(cDeutrons);
        value += "\n\n\n";
    }
    if (tritons.is_set){
        value += QString("Tritons:\n");
        value += makeTableTXT(tritons);
        if (!cTritons.empty())
            value += makeCoeffTXT(cTritons);
        value += "\n\n\n";
    }
    if (He3s.is_set){
        value += QString("Helium-3:\n");
        value += makeTableTXT(He3s);
        if (!cHe3s.empty())
            value += makeCoeffTXT(cHe3s);
        value += "\n\n\n";
    }
    if (alphas.is_set){
        value += QString("Alphas:\n");
        value += makeTableTXT(alphas);
        if (!cAlphas.empty())
            value += makeCoeffTXT(cAlphas);
        value += "\n\n\n";
    }
    if (others.is_set){
        value += QString("Others:\n");
        value += makeTableTXT(others);
        if (!cOthers.empty())
            value += makeCoeffTXT(cOthers);
        value += "\n\n\n";
    }

    return value;
}

QString TableMakerHTML::makeTableTXT(Result_t data)
{
    QString output = "";
    output += "Excitation energy (keV):     ";
    output += "Energy dE-detector (keV):     ";
    output += "dEnergy dE-detector (keV):     ";
    output += "Energy E-detector (keV):    ";
    output += "dEnergy dE-detector (keV):     ";
    output += "Total particle energy (keV):     \n";
    for (int i = 0 ; i < data.Ex.size() ; ++i){
        output += std::to_string(data.Ex[i]*1000).c_str();
        output += "     ";
        output += std::to_string(data.dE[i]*1000).c_str();
        output += "     ";
        output += std::to_string(data.d_dE[i]*1000).c_str();
        output += "     ";
        output += std::to_string(data.E[i]*1000).c_str();
        output += "     ";
        output += std::to_string(data.d_E[i]*1000).c_str();
        output += "     ";
        output += std::to_string(data.E[i]*1000 + data.dE[i]*1000).c_str();
        output += "\n";
    }
    return output;
}

QString TableMakerHTML::makeCoeffTXT(QVector<double> coeff)
{
    QString output = "";
    output += "Ex(e+de) = a0 + a1(e+de) + a2(e+de)^2\n";
    output += "chi-squared: ";
    output += std::to_string(coeff[3]).c_str();
    output += ", a0 = ";
    output += std::to_string(coeff[0]).c_str();
    output += "MeV, a1 = ";
    output += std::to_string(coeff[1]).c_str();
    output += ", a2 = ";
    output += std::to_string(coeff[2]).c_str();
    output += "(MeV)^-1\n";
    return output;
}
