#ifndef DTNtuple_DTNtupleProducer_h
#define DTNtuple_DTNtupleProducer_h

/** \class DTNtupleProducer DTNtupleProducer.h DTDPGAnalysis/DTNtuples/src/DTNtupleProducer.h
 *  
 * Steering class: the edm::EDAnalyzer for DTNtuple prdouction
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h"
#include "DTDPGAnalysis/DTNtuples/src/DTNtupleConfig.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"

#include <memory>

class DTNtupleProducer : public edm::EDAnalyzer // CB change it to appropriate class
{
 public:

  /// Constructor
  DTNtupleProducer(const edm::ParameterSet &);
  
  /// Fill ntuples event by event
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

  /// Configure event setup
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);

  /// General "una tantum" configuration
  virtual void beginJob();

  /// Write ntuples to file
  virtual void endJob();
  
 private:
  
  /// Pointer to the TTree
  std::shared_ptr<TTree> m_tree;

  /// Ponter to the configuration 
  std::shared_ptr<DTNtupleConfig> m_config;

  /// The container with all the fillers
  std::vector<std::unique_ptr<DTNtupleBaseFiller>> m_fillers;

};

#endif
