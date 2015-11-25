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


#ifndef ZIEGLER1985_TABLE_H
#define ZIEGLER1985_TABLE_H

extern const double p1[][8];
extern const double p2[][9];

int Get_minZ();
int Get_maxZ();

double Get_lfctr(int z);
double Get_rho(int z);
double Get_atrho(int z);
double Get_vfermi(int z);
double Get_mm2(int z);

bool Get_pcoef(int z, double *data);

#endif // ZIEGLER1985_TABLE_H
