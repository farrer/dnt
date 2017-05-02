/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _moller_overlaps_h
#define _moller_overlaps_h

/*! Verify if two triangles overlaps
 * vertices of triangle 1: V0,V1,V2
 * vertices of triangle 2: U0,U1,U2
 * \return -> 1 if the triangles intersect, otherwise 0 */
int NoDivTriTriIsect(float V0[3],float V1[3],float V2[3],
                     float U0[3],float U1[3],float U2[3]);


int intersect_triangle(float orig[3], float dir[3], 
                       float vert0 [3], float vert1 [3], float vert2 [3],
                       float* t, float* u, float* v);


#endif


