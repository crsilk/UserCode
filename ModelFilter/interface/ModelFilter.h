#ifndef _CMGTools_StopNTupleProduction_ModelFilter_h_
#define _CMGTools_StopNTupleProduction_ModelFilter_h_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

  class ModelFilter : public edm::EDFilter 
  {
  public:
	explicit ModelFilter(const edm::ParameterSet&);
	~ModelFilter();
	
	static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
	std::vector<std::string> split(std::string fstring, std::string splitter);
	typedef std::vector<std::string>::const_iterator comments_const_iterator;
	
  private:
	virtual void beginJob() ;
	virtual bool filter(edm::Event&, const edm::EventSetup&);
	virtual void endJob() ;
    
	virtual bool beginRun(edm::Run&, edm::EventSetup const&);
	virtual bool endRun(edm::Run&, edm::EventSetup const&);
	virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
	virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

	edm::InputTag inputTagSource_;
	std::string modelTag_;
	std::vector<double> parameterMins_;
	std::vector<double> parameterMaxs_;
  };

#endif /*ModelFilter_h_*/
