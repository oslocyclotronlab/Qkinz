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


#ifndef EXCITATION_H
#define EXCITATION_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>

using namespace std;


//! The Excitation class
/*!
  Class to extract excitation energy for different nuclei.
 */

class Excitation
{
private:
    //! Mass number of the nucleus
    int A;
    //! Atomic number of the nucleus
    int Z;
    //! String to store the path to the file containing the data
    QString Filename;
    //! Vector to store excitation energy (in MeV)
    QVector<double> data;
    //! If excitation energy data excists
    bool Readdata();
    //! Result of Readdata()
    bool haveData;
public:
    //!  Constructor for a empty Excitation object
    Excitation();
    //! A construcor for a spesific nucleus
    /*!
     * \brief Excitation constructor
     * \param A Mass number of the nucleus(integer).
     * \param Z Atomic number of the nucleus(integer).
     */
    Excitation(int A, int Z);

    ~Excitation();

    //! Function to return the vector with the known excitation energy.
    QVector<double> asVector();
};


#endif // EXCITATION_H
