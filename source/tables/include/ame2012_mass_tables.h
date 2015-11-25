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

#ifndef AME_2012_MASSES_TABLES_H
#define AME_2012_MASSES_TABLES_H

typedef struct {
    const int A, Z, origin;
    const double mass_excess, Dmass_excess;
    const double binding_energy, Dbinding_energy;
    const int beta_decay_type;
    const double beta_decay_energy, Dbeta_decay_energy;
    const double mass_u_amu, Dmass_u_amu;
    const bool mass_extrapolated, beta_decay_energy_extrapolated;
} ame2012_mass_t;

extern const ame2012_mass_t ame2012_masses[];

extern const int ame2012_n_masses;
extern const int element_count;

extern const char *element_names[];
extern const char *origins[];

#endif //AME_2012_MASSES_TABLES_H
