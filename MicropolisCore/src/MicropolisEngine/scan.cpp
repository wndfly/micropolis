/* scan.cpp
 *
 * Micropolis, Unix Version.  This game was released for the Unix platform
 * in or about 1990 and has been modified for inclusion in the One Laptop
 * Per Child program.  Copyright (C) 1989 - 2007 Electronic Arts Inc.  If
 * you need assistance with this program, you may contact:
 *   http://wiki.laptop.org/go/Micropolis  or email  micropolis@laptop.org.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  You should have received a
 * copy of the GNU General Public License along with this program.  If
 * not, see <http://www.gnu.org/licenses/>.
 *
 *             ADDITIONAL TERMS per GNU GPL Section 7
 *
 * No trademark or publicity rights are granted.  This license does NOT
 * give you any right, title or interest in the trademark SimCity or any
 * other Electronic Arts trademark.  You may not distribute any
 * modification of this program using the trademark SimCity or claim any
 * affliation or association with Electronic Arts Inc. or its employees.
 *
 * Any propagation or conveyance of this program must include this
 * copyright notice and these terms.
 *
 * If you convey this program (or any modifications of it) and assume
 * contractual liability for the program to recipients of it, you agree
 * to indemnify Electronic Arts for any liability that those contractual
 * assumptions impose on Electronic Arts.
 *
 * You may not misrepresent the origins of this program; modified
 * versions of the program must be marked as such and not identified as
 * the original program.
 *
 * This disclaimer supplements the one included in the General Public
 * License.  TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, THIS
 * PROGRAM IS PROVIDED TO YOU "AS IS," WITH ALL FAULTS, WITHOUT WARRANTY
 * OF ANY KIND, AND YOUR USE IS AT YOUR SOLE RISK.  THE ENTIRE RISK OF
 * SATISFACTORY QUALITY AND PERFORMANCE RESIDES WITH YOU.  ELECTRONIC ARTS
 * DISCLAIMS ANY AND ALL EXPRESS, IMPLIED OR STATUTORY WARRANTIES,
 * INCLUDING IMPLIED WARRANTIES OF MERCHANTABILITY, SATISFACTORY QUALITY,
 * FITNESS FOR A PARTICULAR PURPOSE, NONINFRINGEMENT OF THIRD PARTY
 * RIGHTS, AND WARRANTIES (IF ANY) ARISING FROM A COURSE OF DEALING,
 * USAGE, OR TRADE PRACTICE.  ELECTRONIC ARTS DOES NOT WARRANT AGAINST
 * INTERFERENCE WITH YOUR ENJOYMENT OF THE PROGRAM; THAT THE PROGRAM WILL
 * MEET YOUR REQUIREMENTS; THAT OPERATION OF THE PROGRAM WILL BE
 * UNINTERRUPTED OR ERROR-FREE, OR THAT THE PROGRAM WILL BE COMPATIBLE
 * WITH THIRD PARTY SOFTWARE OR THAT ANY ERRORS IN THE PROGRAM WILL BE
 * CORRECTED.  NO ORAL OR WRITTEN ADVICE PROVIDED BY ELECTRONIC ARTS OR
 * ANY AUTHORIZED REPRESENTATIVE SHALL CREATE A WARRANTY.  SOME
 * JURISDICTIONS DO NOT ALLOW THE EXCLUSION OF OR LIMITATIONS ON IMPLIED
 * WARRANTIES OR THE LIMITATIONS ON THE APPLICABLE STATUTORY RIGHTS OF A
 * CONSUMER, SO SOME OR ALL OF THE ABOVE EXCLUSIONS AND LIMITATIONS MAY
 * NOT APPLY TO YOU.
 */

/** @file scan.cpp */

////////////////////////////////////////////////////////////////////////


#include "stdafx.h"


////////////////////////////////////////////////////////////////////////


/* comefrom: Simulate SpecialInit */
void Micropolis::FireAnalysis()
{
    /* Make firerate map from firestation map */

    register int x, y;

    SmoothFSMap();
    SmoothFSMap();
    SmoothFSMap();

    for (x = 0; x < SmX; x++) {
        for (y = 0; y < SmY; y++) {
            fireStationMapEffect[x][y] = fireStationMap[x][y];
        }
    }

    NewMapFlags[DYMAP] = NewMapFlags[FIMAP] = 1;
}


/* comefrom: Simulate SpecialInit */
void Micropolis::PopDenScan()
{
    /*  sets: populationDensityMap, , , comRateMap  */
    Quad Xtot, Ytot, Ztot;
    register short x, y, z;

    ClrTemArray();
    Xtot = 0;
    Ytot = 0;
    Ztot = 0;
    for (x = 0; x < WORLD_X; x++) {
        for (y = 0; y < WORLD_Y; y++) {
            z = map[x][y];
            if (z & ZONEBIT) {
                z = z & LOMASK;
                curMapX = x;
                curMapY = y;
                z = GetPDen(z) <<3;
                if (z > 254) {
                    z = 254;
                }
                tempMap1[x >>1][y >>1] = (Byte)z;
                Xtot += x;
                Ytot += y;
                Ztot++;
            }
        }
    }

    DoSmooth(); // tempMap1 -> tempMap2
    DoSmooth2(); // tempMap2 -> tempMap1
    DoSmooth(); // tempMap1 -> tempMap2

    for (x = 0; x < HWLDX; x++) {
        for (y = 0; y < HWLDY; y++) {
            populationDensityMap[x][y] = tempMap2[x][y] <<1;
        }
    }

    DistIntMarket();              /* set comRateMap w/ (/comMap) */

    if (Ztot > 0) {               /* Find Center of Mass for City */
        CCx = (short)(Xtot / Ztot);
        CCy = (short)(Ytot / Ztot);
    } else {
        CCx = HWLDX;                /* if pop=0 center of Map is CC */
        CCy = HWLDY;
    }

    CCx2 = CCx >>1;
    CCy2 = CCy >>1;
    NewMapFlags[DYMAP] = 1;
    NewMapFlags[PDMAP] = 1;
    NewMapFlags[RGMAP] = 1;
}


/* comefrom: PopDenScan */
int Micropolis::GetPDen(int Ch9)
{
    int pop;

    if (Ch9 == FREEZ) {
        pop = DoFreePop();
        return pop;
    }

    if (Ch9 < COMBASE) {
        pop = getResZonePop(Ch9);
        return pop;
    }

    if (Ch9 < INDBASE) {
        pop = (getComZonePop(Ch9) <<3);
        return pop;
    }

    if (Ch9 < PORTBASE) {
        pop = (getIndZonePop(Ch9) <<3);
        return pop;
    }

    return 0;
}


/* comefrom: Simulate SpecialInit */
void Micropolis::PTLScan()
{
    /* Does pollution, terrain, land value */
    Quad ptot, LVtot;
    register int x, y, z, dis;
    int Plevel, LVflag, loc, zx, zy, Mx, My, pnum, LVnum, pmax;

    // tempMap3 is a map of development density, smoothed into terrainMap.
    for (x = 0; x < QWX; x++) {
        for (y = 0; y < QWY; y++) {
            tempMap3[x][y] = 0;
        }
    }

    LVtot = 0;
    LVnum = 0;

    for (x = 0; x < HWLDX; x++) {
        for (y = 0; y < HWLDY; y++) {
            Plevel = 0;
            LVflag = 0;
            zx = x <<1;
            zy = y <<1;

            for (Mx = zx; Mx <= zx + 1; Mx++) {
                for (My = zy; My <= zy + 1; My++) {
                    loc = (map[Mx][My] & LOMASK);
                    if (loc) {
                        if (loc < RUBBLE) {
                            tempMap3[x >>1][y >>1] += 15; /* inc terrainMem */
                            continue;
                        }
                        Plevel += GetPValue(loc);
                        if (loc >= ROADBASE) {
                            LVflag++;
                        }
                    }
                }
            }

/* XXX ??? This might have to do with the radiation tile returning -40.
            if (Plevel < 0) {
                Plevel = 250;
            }
*/

            if (Plevel > 255) {
                Plevel = 255;
            }

            tempMap1[x][y] = Plevel;

            if (LVflag) {                     /* LandValue Equation */
                dis = 34 - GetDisCC(x, y);
                dis = dis <<2;
                dis += (terrainDensityMap[x >>1][y >>1] );
                dis -= (pollutionMap[x][y]);
                if (crimeMap[x][y] > 190) {
                    dis -= 20;
                }
                if (dis > 250) {
                    dis = 250;
                }
                if (dis < 1) {
                    dis = 1;
                }
                landValueMap[x][y] = dis;
                LVtot += dis;
                LVnum++;
            } else {
                landValueMap[x][y] = 0;
            }
        }
    }

    if (LVnum) {
        landValueAverage = (short)(LVtot / LVnum);
    } else {
        landValueAverage = 0;
    }

    DoSmooth(); // tempMap1 -> tempMap2
    DoSmooth2(); // tempMap2 -> tempMap1

    pmax = 0;
    pnum = 0;
    ptot = 0;

    for (x = 0; x < HWLDX; x++) {
        for (y = 0; y < HWLDY; y++)  {
            z = tempMap1[x][y];
            pollutionMap[x][y] = z;

            if (z) { /*  get pollute average  */
                pnum++;
                ptot += z;
                /* find max pol for monster  */
                if (z > pmax || (z == pmax && (getRandom16() & 3) == 0)) {
                    pmax = z;
                    PolMaxX = x <<1;
                    PolMaxY = y <<1;
                }
            }
        }
    }
    if (pnum) {
        pollutionAverage = (short)(ptot / pnum);
    } else {
        pollutionAverage = 0;
    }

    SmoothTerrain();

    NewMapFlags[DYMAP] = 1;
    NewMapFlags[PLMAP] = 1;
    NewMapFlags[LVMAP] = 1;
}


/**
 * Return pollution of a tile value
 * @param loc Tile character
 * @return Value of the pollution (0..255, bigger is worse)
 */
int Micropolis::GetPValue(int loc)
{
    if (loc < POWERBASE) {

        if (loc >= HTRFBASE) {
            return /* 25 */ 75;     /* heavy traf  */
        }

        if (loc >= LTRFBASE) {
            return /* 10 */ 50;     /* light traf  */
        }

        if (loc <  ROADBASE) {

            if (loc > FIREBASE) {
                return /* 60 */ 90;
            }

            /* XXX: Why negative pollution from radiation? */
            if (loc >= RADTILE) {
                return /* -40 */ 255; /* radioactivity  */
            }

        }
        return 0;
    }

    if (loc <= LASTIND) {
        return 0;
    }

    if (loc < PORTBASE) {
        return 50;        /* Ind  */
    }

    if (loc <= LASTPOWERPLANT) {
        return /* 60 */ 100;      /* prt, aprt, cpp */
    }

    return 0;
}


/**
 * Compute Manhattan distance from given position to (#CCx2, #CCy2).
 * @param x X coordinate of given position.
 * @param y Y coordinate of given position.
 * @return Manhattan distance (\c dx+dy ) between both positions.
 * @note For long distances (> 32), value 32 is returned.
 */
int Micropolis::GetDisCC(int x, int y)
{
    int xDis, yDis;

    if (x > CCx2) {
        xDis = x - CCx2;
    } else {
        xDis = CCx2 - x;
    }

    if (y > CCy2) {
        yDis = y - CCy2;
    } else {
        yDis = CCy2 - y;
    }

    return min(xDis + yDis, 32);
}


/* comefrom: Simulate SpecialInit */
void Micropolis::CrimeScan()
{
    short numz;
    Quad totz;
    register short x, y, z;
    short cmax;

    SmoothPSMap();
    SmoothPSMap();
    SmoothPSMap();

    totz = 0;
    numz = 0;
    cmax = 0;

    for (x = 0; x < HWLDX; x++) {
        for (y = 0; y < HWLDY; y++) {
            z = landValueMap[x][y];
            if (z) {
                ++numz;
                z = 128 - z;
                z += populationDensityMap[x][y];
                if (z > 300) {
                    z = 300;
                }
                z -= policeStationMap[x >>2][y >>2];
                z = clamp(z, (short)0, (short)250);
                crimeMap[x][y] = (Byte)z;
                totz += z;

                // Update new crime hot-spot
                if (z > cmax || (z == cmax && (getRandom16() & 3) == 0)) {
                    cmax = z;
                    CrimeMaxX = x <<1;
                    CrimeMaxY = y <<1;
                }

            } else {
                crimeMap[x][y] = 0;
            }
        }
    }

    if (numz > 0) {
        crimeAverage = (short)(totz / numz);
    } else {
        crimeAverage = 0;
    }

    for (x = 0; x < SmX; x++) {
        for (y = 0; y < SmY; y++) {
            policeStationMapEffect[x][y] = policeStationMap[x][y];
        }
    }

    NewMapFlags[DYMAP] = 1;
    NewMapFlags[CRMAP] = 1;
    NewMapFlags[POMAP] = 1;
}


/* comefrom: PTLScan */
void Micropolis::SmoothTerrain()
{
    if (DonDither & 1) {
        int x, y = 0, z = 0, dir = 1;

        for (x = 0; x < QWX; x++) {
            for (; y != QWY && y != -1; y += dir) {
                z +=
                    tempMap3[(x == 0) ? x : (x - 1)][y] +
                    tempMap3[(x == (QWX - 1)) ? x : (x + 1)][y] +
                    tempMap3[x][(y == 0) ? (0) : (y - 1)] +
                    tempMap3[x][(y == (QWY - 1)) ? y : (y + 1)] +
                    (tempMap3[x][y] <<2);
                terrainDensityMap[x][y] = (unsigned char)(((unsigned)z) >>3);
                z &= 0x7;
            }
            dir = -dir;
            y += dir;
        }
    } else {
        short x, y, z;

        for (x = 0; x < QWX; x++) {
            for (y = 0; y < QWY; y++) {
                z = 0;
                if (x > 0) {
                    z += tempMap3[x - 1][y];
                }
                if (x < (QWX - 1)) {
                    z += tempMap3[x + 1][y];
                }
                if (y > 0) {
                    z += tempMap3[x][y - 1];
                }
                if (y < (QWY - 1)) {
                    z += tempMap3[x][y + 1];
                }
                terrainDensityMap[x][y] = (unsigned char)((z >>2) + tempMap3[x][y]) >>1;
            }
        }
    }
}


/* comefrom: PopDenScan */
void Micropolis::DoSmooth()
{
    /* smooths data in tempMap1[x][y] into tempMap2[x][y]  */
    if (DonDither & 2) {
        register int x, y = 0, z = 0, dir = 1;

        for (x = 0; x < HWLDX; x++) {
            for (; y != HWLDY && y != -1; y += dir) {
                z +=
                    tempMap1[(x == 0) ? x : (x - 1)][y] +
                    tempMap1[(x == (HWLDX - 1)) ? x : (x + 1)][y] +
                    tempMap1[x][(y == 0) ? (0) : (y - 1)] +
                    tempMap1[x][(y == (HWLDY - 1)) ? y : (y + 1)] +
                    tempMap1[x][y];
                tempMap2[x][y] = (unsigned char)(((unsigned int)z) >>2);
                z &= 3;
            }
            dir = -dir;
            y += dir;
        }
    } else {
        register short x,y,z;

        for (x = 0; x < HWLDX; x++) {
            for (y = 0; y < HWLDY; y++) {
                z = 0;
                if (x > 0) {
                    z += tempMap1[x - 1][y];
                }
                if (x < (HWLDX - 1)) {
                    z += tempMap1[x + 1][y];
                }
                if (y > 0) {
                    z += tempMap1[x][y - 1];
                }
                if (y < (HWLDY - 1)) {
                    z += tempMap1[x][y + 1];
                }
                z = (z + tempMap1[x][y]) >>2;
                if (z > 255) {
                    z = 255;
                }
                tempMap2[x][y] = (unsigned char)z;
            }
        }
    }
}


/* comefrom: PopDenScan */
void Micropolis::DoSmooth2()
{
    /* smooths data in tempMap2[x][y] into tempMap1[x][y]  */
    if (DonDither & 4) {
        int x, y = 0, z = 0, dir = 1;

        for (x = 0; x < HWLDX; x++) {
            for (; y != HWLDY && y != -1; y += dir) {
                z +=
                    tempMap2[(x == 0) ? x : (x - 1)][y] +
                    tempMap2[(x == (HWLDX - 1)) ? x : (x + 1)][y] +
                    tempMap2[x][(y == 0) ? (0) : (y - 1)] +
                    tempMap2[x][(y == (HWLDY - 1)) ? y : (y + 1)] +
                    tempMap2[x][y];
                tempMap1[x][y] = (unsigned char)(((unsigned char)z) >>2);
                z &= 3;
            }
            dir = -dir;
            y += dir;
        }
    } else {
        short x, y, z;

        for (x = 0; x < HWLDX; x++) {
            for (y = 0; y < HWLDY; y++) {
                z = 0;
                if (x > 0) {
                    z += tempMap2[x - 1][y];
                }
                if (x < (HWLDX - 1)) {
                    z += tempMap2[x + 1][y];
                }
                if (y > 0) {
                    z += tempMap2[x][y - 1];
                }
                if (y < (HWLDY - 1)) {
                    z += tempMap2[x][y + 1];
                }
                z = (z + tempMap2[x][y]) >>2;
                if (z > 255) {
                    z = 255;
                }
                tempMap1[x][y] = (unsigned char)z;
            }
        }
    }
}


/* comefrom: PopDenScan */
void Micropolis::ClrTemArray()
{
    short x, y;

    for (x = 0; x < HWLDX; x++) {
        for (y = 0; y < HWLDY; y++) {
            tempMap1[x][y] = (Byte)0;
        }
    }
}


/* comefrom: FireAnalysis */
void Micropolis::SmoothFSMap()
{
    short x, y, edge;

    for (x = 0; x < SmX; x++) {
        for (y = 0; y < SmY; y++) {
            edge = 0;
            if (x > 0) {
                edge += fireStationMap[x - 1][y];
            }
            if (x < (SmX - 1)) {
                edge += fireStationMap[x + 1][y];
            }
            if (y > 0) {
                edge += fireStationMap[x][y - 1];
            }
            if (y < (SmY - 1)) {
                edge += fireStationMap[x][y + 1];
            }
            edge = (edge >>2) + fireStationMap[x][y];
            tempMap4[x][y] = edge >>1;
        }
    }

    for (x = 0; x < SmX; x++) {
        for (y = 0; y < SmY; y++) {
            fireStationMap[x][y] = tempMap4[x][y];
        }
    }
}


/* comefrom: CrimeScan */
void Micropolis::SmoothPSMap()
{
    int x, y, edge;

    for (x = 0; x < SmX; x++) {
        for (y = 0; y < SmY; y++) {
            edge = 0;
            if (x > 0) {
                edge += policeStationMap[x - 1][y];
            }
            if (x < (SmX - 1)) {
                edge += policeStationMap[x + 1][y];
            }
            if (y> 0) {
                edge += policeStationMap[x][y - 1];
            }
            if (y < (SmY - 1)) {
                edge += policeStationMap[x][y + 1];
            }
            edge = (edge >>2) + policeStationMap[x][y];
            tempMap4[x][y] = edge >>1;
        }
    }

    for (x = 0; x < SmX; x++) {
        for (y = 0; y < SmY; y++) {
            policeStationMap[x][y] = tempMap4[x][y];
        }
    }
}


/**
 * Compute distance to city center for the entire map.
 * @see comRateMap
 */
void Micropolis::DistIntMarket()
{
    short x, y, z;

    for (x = 0; x < SmX; x++) {
        for (y = 0; y < SmY; y++) {
            z = GetDisCC(x * 4,y * 4); // 0..32
            z = z * 4;  // 0..128
            z = 64 - z; // 64..-64
            comRateMap[x][y] = z;
        }
    }
}


////////////////////////////////////////////////////////////////////////
