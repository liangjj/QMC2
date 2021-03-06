/* 
 * File:   Fokker_Planck.h
 * Author: jorgmeister
 *
 * Created on October 12, 2012, 2:37 PM
 */

#ifndef FOKKER_PLANCK_H
#define	FOKKER_PLANCK_H

/*! \brief Anisotropic diffusion by the Fokker-Planck equation.
 */
class Fokker_Planck : public Diffusion {
public:
    Fokker_Planck(int n_p, int dim, double timestep, seed_type random_seed, double D = 0.5);

    double get_g_ratio(const Walker* walker_post, const Walker* walker_pre) const;

    double get_new_pos(const Walker* walker, int i, int j) {
        return D * timestep * walker->qforce(i, j) + Diffusion::get_new_pos(walker, i, j);
    }

};

#endif	/* FOKKER_PLANCK_H */

