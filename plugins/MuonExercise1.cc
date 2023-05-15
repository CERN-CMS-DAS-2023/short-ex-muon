// -*- C++ -*-
//
// Package:    CMSDASExercises/MuonExercise1
// Class:      MuonExercise1
// 
/**\class MuonExercise1 MuonExercise1.cc CMSDASExercises/MuonExercise1/plugins/MuonExercise1.cc

 Description: Short Muon exercise for CMSDAS

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Norbert Neumeister
//         Created:  Thu, 14 Dec 2017 09:31:13 GMT
//
//

// system include files
#include <memory>
#include <iomanip>

#include <TLorentzVector.h>
#include <TVector3.h>
#include <TH1.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "Math/GenVector/VectorUtil.h"

//
// class declaration
//

class MuonExercise1 : public edm::one::EDAnalyzer<edm::one::SharedResources> {

  public:

    explicit MuonExercise1(const edm::ParameterSet&);
    ~MuonExercise1();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:

    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
    //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    // ----------member data ---------------------------
     
    // TFileService
    edm::Service<TFileService> fs;
  
    edm::EDGetTokenT<pat::MuonCollection> muonCollToken;
    //edm::EDGetTokenT<pat::PackedGenParticleCollection> genCollToken;
    edm::EDGetTokenT<reco::GenParticleCollection> genCollToken;

    // Histograms
    TH1D* h_ngen;  // number of generated muons
    TH1D* h_nrec;  // number of reconstructed muons
    TH1D* h_genpt; // pt of generated muons 
    TH1D* h_recpt; // pt of reconstructed muons
      
};

//
// constructors and destructor
//
MuonExercise1::MuonExercise1(const edm::ParameterSet& iConfig) {

  usesResource("TFileService");

  edm::InputTag muonTag("slimmedMuons");
  //edm::InputTag genPartTag("packedGenParticles");
  edm::InputTag genPartTag("prunedGenParticles");

  muonCollToken = consumes<pat::MuonCollection>(muonTag);
  //genCollToken = consumes<pat::PackedGenParticleCollection>(genPartTag);
  genCollToken = consumes<reco::GenParticleCollection>(genPartTag);

  h_ngen  = fs->make<TH1D>("ngen", "Number of GEN muons", 10, 0.0, 10.0);
  h_nrec  = fs->make<TH1D>("nrec", "Number of RECO muons", 10, 0.0, 10.0);
  h_recpt = fs->make<TH1D>("pt", "RECO pt", 100, 0.0, 200.0);
  h_genpt = fs->make<TH1D>("genpt", "GEN pt", 100, 0.0, 200.0);

}


MuonExercise1::~MuonExercise1() {
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void MuonExercise1::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;
  using namespace std;
  using namespace reco;
  using namespace pat;


  size_t ngen(0), nrec(0);

  //
  // RECO Muons
  //
  edm::Handle<vector<pat::Muon>> muonColl;
  iEvent.getByToken(muonCollToken, muonColl);
  nrec = muonColl->size();
  cout << "Number of RECO muons: " << nrec << endl;

  for (auto it = muonColl->cbegin(); it != muonColl->cend(); ++it) {
    // put your code here
    
  }

  //
  // GEN Muons
  //  
  //edm::Handle <pat::PackedGenParticleCollection> genColl;
  edm::Handle <reco::GenParticleCollection> genColl;
  iEvent.getByToken(genCollToken, genColl);
  for (auto&& genPart : *(genColl.product())) {
    if ( genPart.status() == 1 && std::abs(genPart.pdgId()) == 13 && fabs(genPart.eta()) < 2.4 && genPart.pt() > 1.5 ) ngen++;
  }
  cout << "Number of GEN muons: " << ngen << endl;

  for (auto&& genPart : *(genColl.product())) {
    if ( genPart.status() != 1 || std::abs(genPart.pdgId()) != 13 || fabs(genPart.eta()) > 2.4 || genPart.pt() < 1.5 ) continue;
    // put your code here
    
  }

}


// ------------ method called once each job just before starting event loop  ------------
void MuonExercise1::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void MuonExercise1::endJob() {
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MuonExercise1::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MuonExercise1);
