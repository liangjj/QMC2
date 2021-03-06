/* 
 * File:   VMC.h
 * Author: jorgmeister
 *
 * Created on October 12, 2012, 2:43 PM
 */

#ifndef VMC_H
#define	VMC_H

/*! \brief Implementation of the Variational Monte-Carlo Method.
 * Very little needs to be added when the QMC superclass holds all the
 * general functionality.
 */
class VMC : public QMC {
protected:

    int pop_tresh; //!< The amount of cycles between storing walkers for DMC.
    int offset; //!< The amount of cycles before starting to store walkers for DMC.
    int last_walker; //!< Count variable for the last walker stores for DMC.

    double vmc_E; //!< The VMC energy.

    Walker* original_walker; //!< The VMC walker.

    /*!
     * Sets the trial position for the single walker.
     */
    void set_trial_positions();

    //! Method for storing walkers for DMC.
    /*!
     * Stores the single VMC walker every pop_thresh cycle after offset cycles.
     */
    void store_walkers();

    //! Method for storing positional data for the Distribtuon.
    /*!
     * Stores the position data of the single VMC walker every dist_tresh cycle
     * after thermalization.
     */
    void save_distribution();

    /*!
     * In VMC, only the metropolis test is performed.
     */
    bool move_autherized(double A) {
        return metropolis_test(A);
    }

    void scale_values() {
        vmc_E /= (n_c * n_nodes);
    }

    /*
     * Collects the vmc energy from all processes.
     */
    void node_comm();


public:

    //! Constructor.
    /*
     * @param dist_out If true, matrices to hold positional data is initialized.
     */
    VMC(GeneralParams &, VMCparams &, SystemObjects &, ParParams &, int n_w, bool dist_out);
    ~VMC();

    void set_e(double E) {
        vmc_E = E;
    }

    double get_energy() const {
        return vmc_E;
    }

    void run_method();
    
    void output();

    friend class DMC;

    friend class Minimizer;
    friend class ASGD;

    friend class BlockingData;

};

#endif	/* VMC_H */
