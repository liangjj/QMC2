/* 
 * File:   AlphaHarmonicOscillator.h
 * Author: jorgehog
 *
 * Created on 26. juni 2012, 17:41
 */

#ifndef ALPHAHARMONICOSCILLATOR_H
#define	ALPHAHARMONICOSCILLATOR_H

/*!\brief Harmonic Oscillator single particle wave function class.
 * Uses the HarmonicOscillator BasisFunction subclasses auto-generated by SymPy
 * through the orbitalsGenerator tool.
 */
class AlphaHarmonicOscillator : public Orbitals {
public:
    AlphaHarmonicOscillator(GeneralParams &, VariationalParams &);

    /*!
     * Calculates the exponential term shared by all oscillator function once pr. particle
     * to save CPU-time.
     * \see Orbitals::set_qnum_indie_terms()
     */
    void set_qnum_indie_terms(const Walker * walker, int i) {
        *exp_factor = exp(-0.5 * (*k2) * walker->get_r_i2(i));
    }

    friend class ExpandedBasis;

protected:

    double w; //!< The oscillator frequency.

    double *alpha; //!< Pointer to the variational parameter alpha. Shared address with all the BasisFunction subclasses.
    double *k; //!< Pointer to sqrt(alpha*w). Shared address with all the BasisFunction subclasses.
    double *k2; //!< Pointer to alpha*w. Shared address with all the BasisFunction subclasses.
    double *w_over_a; //TRASH

    double *exp_factor; //!< Pointer to a factor precalculated by set_qnum_indie_terms(). Shared address with all the BasisFunction subclasses.

    /*!
     * Overridden superclass method implementing closed form expressions using Hermite polynomials.
     */
    double get_variational_derivative(const Walker* walker, int n);

    /*!
     * Calculates the quantum numbers of the oscillator and stores them in the matrix qnums.
     */
    void get_qnums();

    /*!
     * For Quantum Dots, closed form expressions for the matrix elements exist.
     */
    double get_coulomb_element(const arma::uvec & qnum_set);
    
    double get_sp_energy(int qnum) const;
    
    //! Method for calculating Hermite polynomials.
    /*!
     * @param n The degree of the Hermite polynomial.
     * @param x The argument for evaluating the polynomial.
     */
    double H(int n, double x) const;

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
        *k2 = parameter*w;
        *k = sqrt(*k2);
    }

};

#endif	/* ALPHAHARMONICOSCILLATOR_H */