/* 
 * File:   Fermions.cpp
 * Author: jorgmeister
 * 
 * Created on October 12, 2012, 2:44 PM
 */

#include "../../QMCheaders.h"

Fermions::Fermions(GeneralParams & gP, Orbitals* orbital)
: System(gP.n_p, gP.dim, orbital) {

    I = arma::zeros<arma::rowvec > (n2);

}

void Fermions::initialize(Walker* walker) {
    make_merged_inv(walker);
}

void Fermions::get_spatial_grad(Walker* walker, int particle) const {
    using namespace arma;

    int start = n2 * (particle >= n2);

    for (int i = start; i < n2 + start; i++) {
        walker->spatial_grad(i, span()) = strans(walker->dell_phi(i) * walker->inv(span(), i));
    }

}

void Fermions::make_merged_inv(Walker* walker) {
    using namespace arma;

    int end;
    for (int start = 0; start < n_p; start += n2) {
        end = n2 + start - 1;
        walker->inv(span(0, n2 - 1), span(start, end)) = arma::inv(walker->phi(span(start, end), span(0, n2 - 1)));
    }
}

double Fermions::get_spatial_wf(const Walker* walker) {
    using namespace arma;
    return arma::det(walker->phi(span(0, n2 - 1), span())) * arma::det(walker->phi(span(n2, n_p - 1), span()));
}

double Fermions::get_spatial_ratio(const Walker* walker_pre, const Walker* walker_post, int particle) const {
    int q_num;
    double s_ratio;

    s_ratio = 0;
    for (q_num = 0; q_num < n2; q_num++) {
        s_ratio += walker_post->phi(particle, q_num) * walker_pre->inv(q_num, particle);
    }

    return s_ratio;
}

void Fermions::update_inverse(const Walker* walker_old, Walker* walker_new, int particle) {
    int k, l, j, start;
    double sum;

    start = n2 * (particle >= n2);

    for (j = start; j < n2 + start; j++) {
        if (j == particle) {
            I(j - start) = walker_new->spatial_ratio;
        } else {
            sum = 0;
            for (l = 0; l < n2; l++) {
                sum += walker_new->phi(particle, l) * walker_old->inv(l, j);
            }
            I(j - start) = sum;
        }
    }
    //updating the part of the inverse with the same spin as particle i
    for (k = 0; k < n2; k++) {
        for (j = start; j < n2 + start; j++) {
            if (j == particle) {
                walker_new->inv(k, j) = walker_old->inv(k, particle) / walker_new->spatial_ratio;
            } else {
                walker_new->inv(k, j) = walker_old->inv(k, j) - walker_old->inv(k, particle) / walker_new->spatial_ratio * I(j - start);
            }
        }
    }
}

void Fermions::calc_for_newpos(const Walker* walker_old, Walker* walker_new, int particle) {
    update_inverse(walker_old, walker_new, particle);
}

double Fermions::get_spatial_lapl_sum(const Walker* walker) const {
    int i, j;
    double sum;

    sum = 0;
    for (i = 0; i < n_p; i++) {
        for (j = 0; j < n_p / 2; j++) {
            sum += orbital->lapl_phi(walker, i, j) * walker->inv(j, i);
        }
    }

    return sum;
}

void Fermions::update_walker(Walker* walker_pre, const Walker* walker_post, int particle) const {
    using namespace arma;
    
    int start = n2 * (particle >= n2);
    int end = start + n2 - 1;

    //Reseting the parts with the same spin as the moved particle
    walker_pre->inv(span(0, n2 - 1), span(start, end)) = walker_post->inv(span(0, n2 - 1), span(start, end));

}

void Fermions::copy_walker(const Walker* parent, Walker* child) const {
    child->inv = parent->inv;
}

void Fermions::reset_walker_ISCF(const Walker* walker_pre, Walker* walker_post, int particle) const {
    using namespace arma;
    
    int start = n2 * (particle >= n2);
    int end = start + n2 - 1;

    //Reseting the part of the inverse with the same spin as particle i
    walker_post->inv(span(0, n2 - 1), span(start, end)) = walker_pre->inv(span(0, n2 - 1), span(start, end));

}