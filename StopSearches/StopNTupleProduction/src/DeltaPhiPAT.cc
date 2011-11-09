#include "CMGTools/StopNTupleProduction/interface/DeltaPhiPAT.h"

using namespace pat;
using namespace math;

DeltaPhi::DeltaPhi(): 
   PATObject<reco::DeltaPhi>(reco::DeltaPhi())
{
}
DeltaPhi::DeltaPhi(const reco::DeltaPhi & deltaPhi): 
   PATObject<reco::DeltaPhi>(reco::DeltaPhi(deltaPhi))
{
}
DeltaPhi::DeltaPhi(const edm::Ptr<reco::DeltaPhi> & deltaPhi): 
   PATObject<reco::DeltaPhi>(reco::DeltaPhi(deltaPhi))
{
}
DeltaPhi::~DeltaPhi()
{
}
XYZTLorentzVector DeltaPhi::deltaVector() const
{
   return deltaVector_;
}
double DeltaPhi::deltaPhi() const
{
   return deltaPhi_;
}
double DeltaPhi::deltaPt() const
{
   return deltaPt_;
}

