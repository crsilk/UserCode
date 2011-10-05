
/** \file def.cpp
   - Project:     SOFTSUSY
   - Author:      Ben Allanach 
   - Manual:      hep-ph/0104145, Comp. Phys. Comm. 143 (2002) 305 
   - Webpage:     http://hepforge.cedar.ac.uk/softsusy/

   \brief switches (options) and parameters such as default fermion masses,
   required accuracy etc 
*/

#include "def.h"

/// global variable declaration
/// no quark mixing (dominant third family approx), and no verbose output
namespace softsusy {
int MIXING = 1, PRINTOUT = 0;
/// fractional accuracy required
double TOLERANCE = 1.0e-3;
/// decay constant of muon
double GMU = 1.16637e-5; 
/// there are two possible conventions: if QEWSB > MZ, its value is assumed
/// in GeV and used as a constant MSUSY. Otherwise, it MULTIPLIES the usual 
/// MSUSY value, of root(mstop1 mstop2)
double QEWSB = 1.0; 
/// Do we include 2-loop RGEs of *all* scalar masses and A-terms, or only the
/// scalar mass Higgs parameters? (Other quantities all 2-loop anyway): the
/// default in SOFTSUSY 3 is to include all 2-loop terms
bool INCLUDE_2_LOOP_SCALAR_CORRECTIONS = true;
/// number of loops used to calculate Higgs mass and tadpoles. They should be
/// identical for a consistent calculation
int numHiggsMassLoops = 2, numRewsbLoops = 2;
/// global pole mass of MZ in GeV - MZCENT is defined in def.h
double MZ = MZCENT;
}
/// end of global variable declaration
