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

#include "ame2012_masses.h"

#include <algorithm>
#include <stdio.h>
#include <string.h>
//#include <strings.h>

/*
namespace {
#include "ame2012_mass_tables.h"

    inline bool operator<(const ame2012_mass_t& m1, const ame2012_mass_t& m2)
    {
        return (m1.A < m2.A) || ((m1.A == m2.A) && (m1.Z < m2.Z));
    }

    const ame2012_mass_t& find_entry(int A, int Z)
    {
        const ame2012_mass_t search = { A, Z, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        const ame2012_mass_t *begin = ame2012_masses, *end = begin + ame2012_n_masses;
        const ame2012_mass_t *p = std::lower_bound(begin, end, search);
        // invalid entry with A=0 at the end, so if lower bound does
        // not find any match, return this one
        if( p->A == A && p->Z == Z )
            return *p;
        else
            return *end;
    }
}*/

bool operator<(const ame2012_mass_t& m1, const ame2012_mass_t& m2){
    return (m1.A < m2.A) || ((m1.A == m2.A) && (m1.Z < m2.Z));
}

const ame2012_mass_t& find_entry(int A, int Z){
    const ame2012_mass_t search = { A, Z, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const ame2012_mass_t *begin = ame2012_masses, *end = begin + ame2012_n_masses;
    const ame2012_mass_t *p = std::lower_bound(begin, end, search);
    if (p->A == A && p->Z == Z)
        return *p;
    else
        return *end;
}

const char* get_element_name(int Z) {
    if( Z>=0 && Z<element_count )
        return element_names[Z];
    else
        return "?";
}

double get_mass_amu(int A, int Z)
{
    return find_entry(A, Z).mass_u_amu / 1e6;
}

double get_Q_keV(int Ai1, int Zi1, int Ai2, int Zi2, int Ao1, int Zo1)
{
    const ame2012_mass_t& i1 = find_entry(Ai1, Zi1);
    const ame2012_mass_t& i2 = find_entry(Ai2, Zi2);
    const ame2012_mass_t& o1 = find_entry(Ao1, Zo1);
    const ame2012_mass_t& o2 = find_entry(Ai1+Ai2-Ao1, Zi1+Zi2-Zo1);

    if( i1.A && i2.A && o1.A && o2.A ) {
        return i1.mass_excess + i2.mass_excess - o1.mass_excess - o2.mass_excess;
    } else {
        return -1e22;
    }
}
