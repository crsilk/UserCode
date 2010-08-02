#ifndef PFAnalyses_VBFHTauTau_FWLiteTSelector_h
#define PFAnalyses_VBFHTauTau_FWLiteTSelector_h
// -*- C++ -*-
//
// Class  : FWLiteTSelector    
// 
/**\class FWLiteTSelector 

 Description: A ROOT TSelector which accesses data using an edm::Event
              Version updated and adapted to FWLiteAnalysis framework.

*/
//
// Original Author:  Chris Jones
//         Created:  Tue Jun 27 16:37:27 EDT 2006
// Updated by Artur Kalinowski
//                   Fri Sep 18 13:54:16 CEST 2009
//       
//


#include <string>

// system include files
#include "TSelector.h"

// user include files
#include "boost/shared_ptr.hpp"

// forward declarations
class TFile;
class TList;
class TTree;
class TProofOutputFile;

class FWLiteTreeAnalyzer;

#include "DataFormats/FWLite/interface/ChainEvent.h"

namespace pfAnalyses{

class FWLiteTSelector : public TSelector
{

   public:
      FWLiteTSelector();
      virtual ~FWLiteTSelector();
      
   private:
      FWLiteTSelector(const FWLiteTSelector&); // stop default

      const FWLiteTSelector& operator=(const FWLiteTSelector&); // stop default

      virtual void        Begin(TTree *) ;
      virtual void        SlaveBegin(TTree *);
      virtual void        Init(TTree*);
      virtual Bool_t      Notify() ;
      virtual Bool_t      Process(Long64_t /*entry*/) ;
      virtual void        SlaveTerminate();
      virtual void        Terminate();
      virtual Int_t Version() const { return 1; }
      
      void setupNewFile(TFile&);
      // ---------- member data --------------------------------
      int tmp_;
      int lastEntry_;
      std::string sampleName_;
      std::string fileName_;
      bool everythingOK_;
      boost::shared_ptr<fwlite::ChainEvent> event_;
      TTree *tree_;

      FWLiteTreeAnalyzer *fwLiteTreeAnalyzer_;
      TProofOutputFile *fProofFile_;
      TFile *fFile_;

      
};
}

#endif
