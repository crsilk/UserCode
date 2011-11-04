#include "CMGTools/StopNTupleProduction/interface/DeltaPhi.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

using namespace reco;
using namespace std;
using namespace edm;
using namespace math;
DeltaPhi::DeltaPhi()
{
   deltaPhi_ = 0;
   deltaPt_ = 0;
   deltaVector_.SetCoordinates(0.0, 0.0, 0.0, 0.0);
}

DeltaPhi::DeltaPhi(const XYZTLorentzVector deltaVector)
{
   deltaVector_ = deltaVector;
   deltaPhi_ = fabs(deltaVector.phi());
   deltaPt_ = fabs(deltaVector.pt());
}
DeltaPhi::DeltaPhi(const double deltaPhi, const double deltaPt)
{
   deltaPhi_ = fabs(deltaPhi);
   deltaPt_ = fabs(deltaPt);
   deltaVector_.SetCoordinates(deltaPt*cos(deltaPhi), deltaPt*sin(deltaPhi), 
                               0.0, deltaPt);
}
XYZTLorentzVector DeltaPhi::deltaVector()
{
   return deltaVector_;
}
double DeltaPhi::deltaPhi()
{
   return deltaPhi_;
}
double DeltaPhi::deltaPt()
{
   return deltaPt_;
}
bool DeltaPhi::overlap( const Candidate & ) const 
{
  return false;
}

