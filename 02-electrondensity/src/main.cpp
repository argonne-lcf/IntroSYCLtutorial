#include "Field.hpp"
#include "WaveFunction.hpp"
#include "version.hpp"
#include "Timer.hpp"
#include <cstdlib>

int main(int argc, char *argv[]) {
  std::cout << "Version: " << PROJECT_VER << std::endl;
  std::cout << "Compilation Date: " << __DATE__ << "  " << __TIME__
            << std::endl;
  std::cout << "Git SHA1: " << GIT_SHA1 << std::endl;

  Wavefunction wf;
  if( argc != 4){
    std::cout << " We need more arguments try with:" << std::endl;
    std::cout << " ./" << argv[0] << " foo.wfx"  << " delta" << " rmin"<< std::endl;
    exit(EXIT_FAILURE);
  }

  wf.loadWF(argv[1]);
  float delta = std::stod(argv[2]);
  float rmin  = std::stod(argv[3]);

  Field field(wf, delta, rmin);

  Timer tgpu2;

  tgpu2.start();
  field.evalDensity();
  tgpu2.stop();
  std::cout << " Time = "<< tgpu2.getDuration() << " micro sec"<< std::endl;
  //wf.printWF();
  exit(EXIT_SUCCESS);
}
