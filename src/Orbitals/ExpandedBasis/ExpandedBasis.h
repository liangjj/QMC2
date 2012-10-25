/* 
 * File:   ExpandedBasis.h
 * Author: jorgmeister
 *
 * Created on October 9, 2012, 5:09 PM
 */

#ifndef EXPANDEDBASIS_H
#define	EXPANDEDBASIS_H

class ExpandedBasis : public Orbitals {
public:
    ExpandedBasis(GeneralParams & gp, Orbitals* basis, int m, std::string coeffPath);

    virtual double phi(const Walker* walker, int particle, int q_num);
    virtual double del_phi(const Walker* walker, int particle, int q_num, int d);
    virtual double lapl_phi(const Walker* walker, int particle, int q_num);

protected:
    int basis_size;

    arma::mat coeffs;

    Orbitals* basis;

};

#endif	/* EXPANDEDBASIS_H */

