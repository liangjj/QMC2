/* 
 * File:   hydrogenicOrbitals.h
 * Author: jorgehog
 *
 * Created on 26. juni 2012, 17:41
 */

#ifndef HYDROGENICORBITALS_H
#define	HYDROGENICORBITALS_H

/*!\brief Hyrogen-like single particle wave function class.
 * Uses the hydrogenic BasisFunction subclasses auto-generated by SymPy
 * through the orbitalsGenerator tool.
 */
class hydrogenicOrbitals : public Orbitals {
public:
    hydrogenicOrbitals(GeneralParams &, VariationalParams &, int n_p);

    /*!
     * Calculates the exponential terms exp(-r/n) for all needed n once pr. particle
     * to save CPU-time. 
     * \see Orbitals::set_qnum_indie_terms()
     */
    void set_qnum_indie_terms(Walker * walker, int i);

    friend class ExpandedBasis;
    friend class DiAtomic;

protected:

    bool diatomic;
    int Z; //!< The charge of the core.

    double *alpha; //!< Pointer to the variational parameter alpha. Shared address with all the BasisFunction subclasses.
    double *k; //!< Pointer to alpha*Z. Shared address with all the BasisFunction subclasses.
    double *k2; //!< Pointer (alpha*Z)^2. Shared address with all the BasisFunction subclasses.

    double *exp_factor_n1; //!< Pointer to a factor precalculated by set_qnum_indie_terms(). Shared address with all the BasisFunction subclasses.
    double *exp_factor_n2; //!< Pointer to a factor precalculated by set_qnum_indie_terms(). Shared address with all the BasisFunction subclasses.
    double *exp_factor_n3; //!< Pointer to a factor precalculated by set_qnum_indie_terms(). Shared address with all the BasisFunction subclasses.
    double *exp_factor_n4; //!< Pointer to a factor precalculated by set_qnum_indie_terms(). Shared address with all the BasisFunction subclasses.

    double static minusPower(int n) {
        return -2 * (n % 2) + 1;
    }


    ///! Overridden superclass method implementing closed form expressions generated by SymPy.
    double get_variational_derivative(const Walker* walker, int n, const Walker* walker2 = NULL);
    
    
    //! Method for calculating a single particle wave functions variational derivative.
    /*!
     * @param i The particle number.
     */
    double get_dell_alpha_phi(const Walker* walker, int qnum, int i);

    double get_sp_energy(int qnum) const;
    double get_coulomb_element(const arma::uvec & qnum_set);

    void get_qnums();

    /*!
     * @return The variational parameter alpha.
     */
    double get_parameter(int n) {
        return *alpha;
    }

    /*!
     * Sets a new value for the alpha and updates all the pointer values.
     */
    void set_parameter(double parameter, int n) {
        *alpha = parameter;
        *k = parameter*Z;
        *k2 = (*k)*(*k);
    }

};

#endif	/* HYDROGENICORBITALS_H */