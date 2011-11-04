#ifndef _CMGTools_StopNTupleProduction_MassSplittingSelector_h_
#define _CMGTools_StopNTupleProduction_MassSplittingSelector_h_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

namespace cmg
{
  class MassSplittingSelector : public edm::EDFilter {
  public:
	explicit MassSplittingSelector(const edm::ParameterSet&);
	~MassSplittingSelector();
	
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
	double minSplitting_;
	double maxSplitting_;
  };
}
#endif /*MassSplittingSelector_h_*/
