#ifndef RECONSTRUCTION
#define RECONSTRUCTION
#include <CommonInclude.h>

class Reconstruction{
public:
    Reconstruction();
    Mat PaperTemplate;
};

Reconstruction::Reconstruction(){
    FileStorage fs1("PaperTemplate.yml", FileStorage::READ);
    fs1["R29B18"]>>PaperTemplate;
    fs1.release();
}

#endif // RECONSTRUCTION

