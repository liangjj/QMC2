/* 
 * File:   AtomCore.cpp
 * Author: jorgmeister
 * 
 * Created on October 12, 2012, 2:42 PM
 */


#include "../../QMCheaders.h"


AtomCore::AtomCore(GeneralParams & gP)
: Potential(gP.n_p, gP.dim) {

    this->Z = (int) gP.systemConstant;
    name = "Core";
}


double AtomCore::get_pot_E(const Walker* walker) const {

    double e_potential = 0;

    for (int i = 0; i < n_p; i++) {
        e_potential -= Z / walker->get_r_i(i);
    }

    return e_potential;
}
