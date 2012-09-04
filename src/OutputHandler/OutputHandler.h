/* 
 * File:   OutputHandler.h
 * Author: jorgehog
 *
 * Created on 3. sept 2012, 13:17
 */

#ifndef OUTPUTHANDLER_H
#define	OUTPUTHANDLER_H

class OutputHandler {
protected:
    bool is_vmc;
    bool is_dmc;
    
    bool parallel;
    int my_rank;
    int num_procs;
    
    std::string filename;
    std::string path;

    std::ofstream file;

    QMC* qmc;
    DMC* dmc;
    VMC* vmc;

public:

    OutputHandler();
    OutputHandler(std::string filename,
            std::string path,
            bool parallel,
            int my_rank,
            int num_procs
            );

    virtual void dump() = 0;
    virtual void finalize();
    
    void set_qmc_ptr(QMC* qmc);

};


#endif	/* OUTPUTHANDLER_H */