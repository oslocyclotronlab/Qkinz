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

#ifndef AME2012_MASSES_H
#define AME2012_MASSES_H

#include "ame2012_mass_tables.h"

const char* get_element_name(int Z);
double get_mass_amu(int A, int Z);
double get_Q_keV(int Ai1, int Zi1, int Ai2, int Zi2, int Ao1, int Zo1);

/** Ask for a Z and accept either a number or a name */
void ask_par_Z(const char* ask, int* Z);

bool operator<(const ame2012_mass_t& m1, const ame2012_mass_t& m2);
const ame2012_mass_t& find_entry(int A, int Z);


#endif // AME2012_MASSES_H
