/* 
 * File:   ASGD.cpp
 * Author: jorgehog
 *
 * Created on 23. August 2012, 16:52
 */

#include "../../QMCheaders.h"

ASGD::ASGD(VMC* vmc, MinimizerParams & mP, const ParParams & pp)
: Minimizer(vmc, pp, mP.alpha, mP.beta) {
    using namespace arma;

    this->n_c = mP.n_c;
    this->thermalization = mP.therm;
    this->n_c_SGD = mP.n_c_SGD;
    this->SGDsamples = mP.SGDsamples;
    this->n_walkers = mP.n_w;

    this->a = mP.a;
    this->A = mP.A;
    this->f_min = mP.f_min;
    this->f_max = mP.f_max;
    this->w = mP.omega;

    this->max_step = mP.max_step;

    walkers = new Walker*[n_walkers];
    trial_walkers = new Walker*[n_walkers];
    
    for (int i = 0; i < n_walkers; i++) {
        walkers[i] = new Walker(vmc->n_p, vmc->dim);
        trial_walkers[i] = new Walker(vmc->n_p, vmc->dim);
    }
 
    
    gradient = zeros(1, Nparams);
    gradient_local = zeros(1, Nparams);

    gradient_old = zeros(1, Nparams);
    gradient_tot = zeros(1, Nparams);

    t_prev = A;
}

void ASGD::get_variational_gradients(Walker* walker, double e_local) {

    for (int alpha = 0; alpha < Nspatial_params; alpha++) {

        double dalpha = vmc->get_orbitals_ptr()->get_variational_derivative(walker, alpha);

        gradient_local(alpha) += e_local * dalpha;
        gradient(alpha) += dalpha;

    }

    for (int beta = 0; beta < Njastrow_params; beta++) {

        double dbeta = vmc->get_jastrow_ptr()->get_variational_derivative(walker, beta);

        gradient_local(Nspatial_params + beta) += e_local*dbeta;
        gradient(Nspatial_params + beta) += dbeta;

    }

}

void ASGD::get_total_grad() {
    int scale = n_walkers * n_c_SGD;

#ifdef MPI_ON
    MPI_Allreduce(MPI_IN_PLACE, &E, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    E /= n_nodes;
#endif
    E /= scale;

    gradient_tot = 2 * (gradient_local - gradient * E)/scale;
    
    for (int i = 0; i < Nparams; i++) {
        error_estimators.at(i)->update_data(gradient_tot(i));
    }

#ifdef MPI_ON
    MPI_Allreduce(MPI_IN_PLACE, gradient_tot.memptr(), Nparams, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    gradient_tot /= n_nodes;
#endif

    
}

void ASGD::minimize() {
    
    thermalize_walkers();
    
    for (sample = 1; sample <= SGDsamples; sample++) {
    
        E = 0;
        gradient = arma::zeros(1, Nparams);
        gradient_local = arma::zeros(1, Nparams);

        for (int k = 0; k < n_walkers; k++) {
            vmc->get_sampling_ptr()->set_trial_states(walkers[k]);
            vmc->get_system_ptr()->initialize(walkers[k]);
            vmc->get_jastrow_ptr()->get_dJ_matrix(walkers[k]);
            vmc->get_sampling_ptr()->get_necessities(walkers[k]);

            for (int cycle = 0; cycle < n_c_SGD; cycle++) {
                
                vmc->diffuse_walker(walkers[k], trial_walkers[k]);

                vmc->calculate_energy_necessities(walkers[k]);
                double e_local = vmc->calculate_local_energy(walkers[k]);
                E += e_local;

                get_variational_gradients(walkers[k], e_local);

            }
        }

        get_total_grad();

        update_parameters();

        dump_output();

        output_cycle();

    }

    output("Finished minimizing. Final parameters:");
    finalize_output();

    error_output();

    vmc->accepted = 0;
}

void ASGD::update_parameters() {

    double x = -arma::dot(gradient_tot, gradient_old);

    t = t_prev + f(x);
    if (t < 0) {
        t = 0;
    }


    for (int param = 0; param < Nspatial_params; param++) {

        step = a / (t + A) * gradient_tot(param);
        if (fabs(step) > max_step) {
            step *= max_step / fabs(step);
        }

        double alpha = vmc->get_orbitals_ptr()->get_parameter(param);
        vmc->get_orbitals_ptr()->set_parameter(fabs(alpha - step), param);

    }

    for (int param = 0; param < Njastrow_params; param++) {

        step = a / (t + A) * gradient_tot(Nspatial_params + param);
        if (step * step > max_step * max_step) {
            step *= max_step / fabs(step);
        }

        double beta = vmc->get_jastrow_ptr()->get_parameter(param);
        vmc->get_jastrow_ptr()->set_parameter(fabs(beta - step), param);

    }

    t_prev = t;
    gradient_old = gradient_tot;
}

void ASGD::output_cycle() {
    if ((sample % 100) == 0) {
        output("cycle:", sample);
        s << gradient_tot << std::endl;
        s << E << std::endl;
        std_out->cout(s);
    }
}

void ASGD::thermalize_walkers() {
    int k = 0;
    vmc->get_sampling_ptr()->set_trial_pos(vmc->original_walker);
    vmc->copy_walker(vmc->original_walker, vmc->trial_walker);
    for (int cycle = 1; cycle <= thermalization + n_walkers * n_c; cycle++) {
        vmc->diffuse_walker(vmc->original_walker, vmc->trial_walker);

        if ((cycle > thermalization) && (cycle % n_c) == 0) {
            vmc->copy_walker(vmc->original_walker, walkers[k]);
            vmc->copy_walker(vmc->trial_walker, trial_walkers[k]);
            k++;
        }
    }
}