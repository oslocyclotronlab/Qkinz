/*******************************************************************************
 * Copyright (C) 2015 University of Oslo                                       *
 * Author: Vetle Wegner Ingeberg, v.w.ingeberg@fys.uio.no                      *
 *                                                                             *
 * --------------------------------------------------------------------------- *
 * This program is free software; you can redistribute it and/or modify it     *
 * under the terms of the GNU General Public License as published by the       *
 * Free Software Foundation; either version 3 of the license, or (at your      *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but         *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                  *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General   *
 * Public License for more details.                                            *
 *                                                                             *
 * You should have recived a copy of the GNU General Public License along with *
 * the program. If not, see <http://www.gnu.org/licenses/>.                    *
 *                                                                             *
 *******************************************************************************/


#include "excitation.h"

#include <iostream>
#include <sstream>
#include <string>
Excitation::Excitation()
{

}

Excitation::Excitation(int iA, int iZ){
    A = iA; Z = iZ;

    stringstream ss(stringstream::in | stringstream::out);
    ss << ":/Excitation/z";
    ss << Z;

    Filename = ss.str().c_str();
    if (!Readdata()){ // If data not pressent
        haveData = false;
        data = QVector<double>(11); // Set excitation energies from 0 to 10 MeV.
        for (int i = 0 ; i < 11 ; i++){
            data[i] = (double)i;
        }
    } else {
        haveData = true;
    }
}

Excitation::~Excitation()
{
}

bool Excitation::Readdata(){
     QFile file(Filename);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){ // Check if file exsists
         return false;
     }

     QTextStream lines(&file);
     QString line = lines.readLine();
     QStringList elements = line.split(" ");

     int Amin = elements[0].toInt(); // Lowest A with values
     int Amax = elements[1].toInt(); // Highest A with values

     if ((A > Amin)||(A < Amax)){ // Do not read all lines if file does not contain data with mass number A
         int Ai = Amin; // Line with data for mass number Ai
         while (!lines.atEnd()){ // Reading lines
             line = lines.readLine();
             if (A == Ai){
                 if (line == ""){ return false; } // If line is empty, the no data exsists
                 elements = line.split(" ");
                 data = QVector<double>(elements.size() + 1);
                 data[0] = 0.0; // Ground state
                 for (int i = 0 ; i < elements.size() ; i++){
                     data[i+1] = elements[i].toDouble()/1000.0;
                 }
                 file.close();
                 return true;
             }
             Ai += 1; // Next A
         }
     }
     file.close();
     return false;
}

QVector<double> Excitation::asVector()
{
    return data;
}
