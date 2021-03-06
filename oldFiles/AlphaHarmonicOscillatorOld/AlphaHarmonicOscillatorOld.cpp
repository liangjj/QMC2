/* 
 * File:   AlphaHarmonicOscillatorOld.cpp
 * Author: jorgehog
 * 
 * Created on 26. juni 2012, 17:41
 */

#include "../../QMCheaders.h"

AlphaHarmonicOscillatorOld::AlphaHarmonicOscillatorOld(GeneralParams & gP, VariationalParams & vP)
: Orbitals(gP.n_p, gP.dim) {

    this->alpha = new double();

    this->w = gP.systemConstant;
    set_parameter(vP.alpha, 0);

    basis_functions[0] = new HO_1(this->alpha, w);
    basis_functions[1] = new HO_2(this->alpha, w);
    basis_functions[2] = new HO_3(this->alpha, w);
    basis_functions[3] = new HO_4(this->alpha, w);
    basis_functions[4] = new HO_5(this->alpha, w);
    basis_functions[5] = new HO_6(this->alpha, w);
    basis_functions[6] = new HO_7(this->alpha, w);
    basis_functions[7] = new HO_8(this->alpha, w);
    basis_functions[8] = new HO_9(this->alpha, w);
    basis_functions[9] = new HO_10(this->alpha, w);
    basis_functions[10] = new HO_11(this->alpha, w);
    basis_functions[11] = new HO_12(this->alpha, w);
    basis_functions[12] = new HO_13(this->alpha, w);
    basis_functions[13] = new HO_14(this->alpha, w);
    basis_functions[14] = new HO_15(this->alpha, w);

    dell_basis_functions[0][0] = new dell_HO_1_0(this->alpha, w);
    dell_basis_functions[1][0] = new dell_HO_1_1(this->alpha, w);
    dell_basis_functions[0][1] = new dell_HO_2_0(this->alpha, w);
    dell_basis_functions[1][1] = new dell_HO_2_1(this->alpha, w);
    dell_basis_functions[0][2] = new dell_HO_3_0(this->alpha, w);
    dell_basis_functions[1][2] = new dell_HO_3_1(this->alpha, w);
    dell_basis_functions[0][3] = new dell_HO_4_0(this->alpha, w);
    dell_basis_functions[1][3] = new dell_HO_4_1(this->alpha, w);
    dell_basis_functions[0][4] = new dell_HO_5_0(this->alpha, w);
    dell_basis_functions[1][4] = new dell_HO_5_1(this->alpha, w);
    dell_basis_functions[0][5] = new dell_HO_6_0(this->alpha, w);
    dell_basis_functions[1][5] = new dell_HO_6_1(this->alpha, w);
    dell_basis_functions[0][6] = new dell_HO_7_0(this->alpha, w);
    dell_basis_functions[1][6] = new dell_HO_7_1(this->alpha, w);
    dell_basis_functions[0][7] = new dell_HO_8_0(this->alpha, w);
    dell_basis_functions[1][7] = new dell_HO_8_1(this->alpha, w);
    dell_basis_functions[0][8] = new dell_HO_9_0(this->alpha, w);
    dell_basis_functions[1][8] = new dell_HO_9_1(this->alpha, w);
    dell_basis_functions[0][9] = new dell_HO_10_0(this->alpha, w);
    dell_basis_functions[1][9] = new dell_HO_10_1(this->alpha, w);
    dell_basis_functions[0][10] = new dell_HO_11_0(this->alpha, w);
    dell_basis_functions[1][10] = new dell_HO_11_1(this->alpha, w);
    dell_basis_functions[0][11] = new dell_HO_12_0(this->alpha, w);
    dell_basis_functions[1][11] = new dell_HO_12_1(this->alpha, w);
    dell_basis_functions[0][12] = new dell_HO_13_0(this->alpha, w);
    dell_basis_functions[1][12] = new dell_HO_13_1(this->alpha, w);
    dell_basis_functions[0][13] = new dell_HO_14_0(this->alpha, w);
    dell_basis_functions[1][13] = new dell_HO_14_1(this->alpha, w);
    dell_basis_functions[0][14] = new dell_HO_15_0(this->alpha, w);
    dell_basis_functions[1][14] = new dell_HO_15_1(this->alpha, w);


    lapl_basis_functions[0] = new lapl_HO_1(this->alpha, w);
    lapl_basis_functions[1] = new lapl_HO_2(this->alpha, w);
    lapl_basis_functions[2] = new lapl_HO_3(this->alpha, w);
    lapl_basis_functions[3] = new lapl_HO_4(this->alpha, w);
    lapl_basis_functions[4] = new lapl_HO_5(this->alpha, w);
    lapl_basis_functions[5] = new lapl_HO_6(this->alpha, w);
    lapl_basis_functions[6] = new lapl_HO_7(this->alpha, w);
    lapl_basis_functions[7] = new lapl_HO_8(this->alpha, w);
    lapl_basis_functions[8] = new lapl_HO_9(this->alpha, w);
    lapl_basis_functions[9] = new lapl_HO_10(this->alpha, w);
    lapl_basis_functions[10] = new lapl_HO_11(this->alpha, w);
    lapl_basis_functions[11] = new lapl_HO_12(this->alpha, w);
    lapl_basis_functions[12] = new lapl_HO_13(this->alpha, w);
    lapl_basis_functions[13] = new lapl_HO_14(this->alpha, w);
    lapl_basis_functions[14] = new lapl_HO_15(this->alpha, w);

}

double AlphaHarmonicOscillatorOld::get_parameter(int n) {
    return *alpha;
}

void AlphaHarmonicOscillatorOld::set_parameter(double parameter, int n) {
    *alpha = parameter;
}

double AlphaHarmonicOscillatorOld::get_variational_derivative(const Walker* walker, int n){
    double dalpha = 0.0;

    for (int i = 0; i < n_p; i++) {
        dalpha -= 0.5 * w * walker->get_r_i2(i);
    }

    return dalpha;
}