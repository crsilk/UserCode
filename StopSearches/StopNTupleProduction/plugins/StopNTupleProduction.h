#ifndef __CMGTools_StopNTupleProduction_StopNtupleProduction_h_
#define __CMGTools_StopNTupleProduction_StopNtupleProduction_h_

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"
#include "CMGTools/StopNTupleProduction/interface/DeltaPhiFactory.h"
#include "CMGTools/StopNTupleProduction/plugins/ModelSelector.h"

typedef cmg::ModelSelector ModelSelector;
typedef PhysicsObjectProducer<cmg::DeltaPhiFactory> DeltaPhiPOProducer;


#endif /*StopNTupleProduction*/
