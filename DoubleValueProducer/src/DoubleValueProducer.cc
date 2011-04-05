// -*- C++ -*-
//
// Package:    DoubleValueProducer
// Class:      DoubleValueProducer
// 
/**\class DoubleValueProducer DoubleValueProducer.cc UserCode/DoubleValueProducer/src/DoubleValueProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Christopher Silkworth
//         Created:  Tue Apr  5 10:15:40 CDT 2011
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


using namespace std;
using namespace edm;
//
// class declaration
//

class DoubleValueProducer : public edm::EDProducer {
   public:
      explicit DoubleValueProducer(const edm::ParameterSet&);
      ~DoubleValueProducer();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      double inputDouble1_;
      double inputDouble2_;
      
      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
DoubleValueProducer::DoubleValueProducer(const edm::ParameterSet& iConfig)
{
   inputDouble1_ = iConfig.getUntrackedParameter<double>("value1");
   inputDouble2_ = iConfig.getUntrackedParameter<double>("value2");
   produces<vector<double> >( );
}


DoubleValueProducer::~DoubleValueProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
DoubleValueProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   
   vector<double> values;
   values.push_back(inputDouble1_);
   values.push_back(inputDouble2_);

   auto_ptr<vector<double> > valuesPointer(new vector<double>(values) );
   
   iEvent.put(valuesPointer);
      
   
 
}

// ------------ method called once each job just before starting event loop  ------------
void 
DoubleValueProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DoubleValueProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(DoubleValueProducer);
