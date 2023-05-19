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
//         Created:  Sat, 15 Dec 2018 07:36:33 GMT
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

class MuonExercise1 : public edm::one::EDAnalyzer<edm::one::SharedResources>  {

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
    //
    // TFileService
    edm::Service<TFileService> fs;
     
    edm::EDGetTokenT<pat::MuonCollection> muonCollToken;
    edm::EDGetTokenT<pat::PackedGenParticleCollection> genCollToken;
    edm::EDGetTokenT<BXVector<l1t::Muon>> l1MuonCollToken;
    edm::EDGetTokenT<edm::TriggerResults> trigResultsToken;
    edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> trigObjCollToken;

    TH1D* h_genpt;
    TH1D* h_geneta;
    TH1D* h_genphi;
    TH1D* h_l1pt;
    TH1D* h_l1eta;
    TH1D* h_l1phi;
    TH1D* h_hltpt;
    TH1D* h_hlteta;
    TH1D* h_hltphi;
    TH1D* h_pt;
    TH1D* h_eta;
    TH1D* h_phi;
      
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
MuonExercise1::MuonExercise1(const edm::ParameterSet& iConfig)
{

  usesResource("TFileService");
  edm::InputTag theMuonLabel("slimmedMuons");
  edm::InputTag theGenMuonLabel("packedGenParticles");
  edm::InputTag theL1MuonLabel("gmtStage2Digis:Muon");
  edm::InputTag theTriggerLabel("TriggerResults", "", "HLT");
  edm::InputTag theTrigObjLabel("slimmedPatTrigger");

  muonCollToken = consumes<pat::MuonCollection>(theMuonLabel);
  genCollToken = consumes<pat::PackedGenParticleCollection>(theGenMuonLabel);
  l1MuonCollToken = consumes<BXVector<l1t::Muon>>(theL1MuonLabel);
  trigResultsToken = consumes<edm::TriggerResults>(theTriggerLabel);
  trigObjCollToken = consumes<pat::TriggerObjectStandAloneCollection>(theTrigObjLabel);

  h_pt = fs->make<TH1D>("pt", "RECO pt", 100, 0.0, 200.0);
  h_phi = fs->make<TH1D>("phi", "RECO phi", 100, -3.15, 3.15);
  h_eta = fs->make<TH1D>("eta", "RECO eta", 100, 0, 2.4);
  h_genpt = fs->make<TH1D>("genpt", "GEN pt", 100, 0.0, 200.0);
  h_genphi = fs->make<TH1D>("genphi", "GEN phi", 100, -3.15, 3.15);
  h_geneta = fs->make<TH1D>("geneta", "GEN eta", 100, 0, 2.4);
  h_l1pt = fs->make<TH1D>("l1pt", "L1 pt" , 100 , 0.0, 200.0);
  h_l1phi = fs->make<TH1D>("l1phi", "L1 phi", 100, -3.15, 3.15);
  h_l1eta = fs->make<TH1D>("l1eta", "L1 eta", 100, 0, 2.4);
  h_hltpt = fs->make<TH1D>("hltpt", "HLT pt" , 100 , 0.0, 200.0);
  h_hltphi = fs->make<TH1D>("hltphi", "HLT phi", 100, -3.15, 3.15);
  h_hlteta = fs->make<TH1D>("hlteta", "HLT eta", 100, 0, 2.4);

}


MuonExercise1::~MuonExercise1()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MuonExercise1::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;
  using namespace std;
  using namespace reco;
  using namespace pat;

  // save the current settings
  ios::fmtflags old_settings = cout.flags(); //save previous format flags
  int old_precision = cout.precision(); // save previous precision setting
  cout.setf(ios::fixed, ios::floatfield);
  cout << setprecision(3);

  //
  // RECO Muons
  //
  edm::Handle<vector<pat::Muon>> muons;
  iEvent.getByToken(muonCollToken, muons);
  cout << "Number of RECO muons: " << muons->size() << endl;

  for (auto it = muons->cbegin(); it != muons->cend(); ++it) {
    if ( it->pt() < 1.5 ) continue;
    cout << "Reco: " << "pt = " << setw(7) << it->pt()  << " " 
                     << "eta = " << setw(6) << it->eta() << " "
                     << "phi = " << setw(6) << it->phi() << " "
                     << "GLB = " << it->isGlobalMuon() << " "
                     << "Loose = " << it->isLooseMuon()<< endl;

    if ( not it->isGlobalMuon() ) continue;
    reco::TrackRef mu_g(it->globalTrack());
    reco::TrackRef mu_i(it->innerTrack());
    reco::TrackRef mu_o(it->outerTrack());
    // use the methods disribed in
    // https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId
    // to filter on the muon quality 
    h_pt->Fill(it->pt());
    h_eta->Fill(fabs(it->eta()));
    h_phi->Fill(it->phi());
  }

  //
  // GEN Muons
  //  
  edm::Handle <pat::PackedGenParticleCollection> genColl;
  iEvent.getByToken(genCollToken, genColl);
  int n = 0;
  for (auto it = genColl->cbegin(); it != genColl->cend(); ++it) if ( abs((*it).pdgId()) == 13 && fabs((*it).eta()) < 2.4 && (*it).pt() > 1.5 ) n++;
  cout << "Number of GEN muons: " << n << endl;

  for (auto it = genColl->cbegin(); it != genColl->cend(); ++it) {
    const pat::PackedGenParticle& mcMuon = (*it);
    if ( abs(mcMuon.pdgId()) != 13 ) continue;
    if ( fabs(mcMuon.eta()) > 2.4 || mcMuon.pt() < 1.5 ) continue;
    double pt =  mcMuon.pt();
    double phi = mcMuon.phi();
    double eta = mcMuon.eta();
    cout << "GEN: " << "pt = " << setw(7) <<  pt << " " 
                    << "eta = " << setw(6) << eta << " " 
                    << "phi = " << setw(6) << phi << endl;
    h_genpt->Fill(pt);
    h_geneta->Fill(fabs(eta));
    h_genphi->Fill(phi);

    TVector3 mu1Gen3V(mcMuon.px(), mcMuon.py(), mcMuon.pz());
    double mu1mass = mcMuon.mass();
    double mu1E = sqrt(mu1Gen3V.Mag2() + mu1mass*mu1mass);
    reco::Particle::LorentzVector mu1Gen4V(mcMuon.px(), mcMuon.py(), mcMuon.pz(), mu1E);
  }

  //
  // L1 Trigger Muons
  //
  edm::Handle<BXVector<l1t::Muon>> l1Coll;
  iEvent.getByToken(l1MuonCollToken, l1Coll);
  cout << "Number of L1 trigger muons: " << l1Coll->size() << endl;

  for (auto it = l1Coll->begin(); it != l1Coll->end(); ++it) {
    float pt    = (*it).pt();
    float eta   = (*it).eta();
    float phi   = (*it).phi();
    if (pt < 1.5) continue;
    //int charge  = (*it).charge();
    //bool valid_charge = muonCand.charge_valid();
    //if (!valid_charge) charge = 0;
    h_l1pt->Fill(pt);
    h_l1eta->Fill(fabs(eta));
    h_l1phi->Fill(phi);
    cout << "L1: " << "pt = " << setw(7) << pt  << " " 
                   << "eta = " << setw(6) << eta << " " 
                   << "phi = " << setw(6) << phi << endl;
  }

  //
  // HLT Muons
  //
  edm::Handle<edm::TriggerResults> triggerBits;
  iEvent.getByToken(trigResultsToken,triggerBits);

  const edm::TriggerNames& names = iEvent.triggerNames(*triggerBits);
  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects; 
  iEvent.getByToken(trigObjCollToken,triggerObjects);

  n = 0;
  for (auto it = triggerObjects->cbegin(); it != triggerObjects->cend(); ++it) {
    if ( (*it).hasTriggerObjectType(trigger::TriggerMuon) ) n++;
  }
  cout << "Number of HLT  muons: " << n << endl;

  // loop over selected trigger objects
  for (pat::TriggerObjectStandAlone obj : *triggerObjects) {
    obj.unpackPathNames(names);
    if ( not obj.hasTriggerObjectType(trigger::TriggerMuon)) continue;
    if ( not obj.hasPathName("HLT_Mu*") && not obj.hasPathName("HLT_IsoMu*") ) continue;
    if ( obj.pt() < 1.5) continue;
    cout << "HLT: " << "pt = " << setw(7) << obj.pt() << " " 
                    << "eta = " << setw(6) << obj.eta() << " " 
                    << "phi = " << setw(6) << obj.phi() << " "
                    << obj.collection() <<  endl;
    h_hltpt->Fill(obj.pt());
    h_hlteta->Fill(fabs(obj.eta()));
    h_hltphi->Fill(obj.phi());
  }

  //
  // Matching
  //
  for (auto it1 = muons->cbegin(); it1 != muons->cend(); ++it1) {
    if ( not it1->isGlobalMuon() ) continue;
    int idx_G = -1;
    int idx_L1 = -1;
    int idx_HLT = -1;

    // match gen
    for (auto it2 = genColl->cbegin(); it2 != genColl->cend(); ++it2) {
      const pat::PackedGenParticle& mcMuon = (*it2);
      if ( abs(mcMuon.pdgId()) != 13 ) continue;
      if ( fabs(mcMuon.eta()) > 2.4 ) continue;
      double dR = deltaR(*it2, *(it1->innerTrack()));
      if ( dR < 0.1 ) { 
        idx_G = std::distance(genColl->cbegin(), it2);
        break;}
    }

    // match trigger 
    int n=0;
    for (pat::TriggerObjectStandAlone obj : *triggerObjects) {
      obj.unpackPathNames(names);
      if ( not obj.hasTriggerObjectType(trigger::TriggerMuon) && not obj.hasTriggerObjectType(trigger::TriggerL1Mu) ) { n++; continue;}
      if ( obj.pt() < 1.5 ) { n++; continue; }
      double dR = deltaR(obj, *(it1->innerTrack()));
      // L1
      if ( obj.hasTriggerObjectType(trigger::TriggerL1Mu) && dR < 0.25 ) {
        idx_L1 = n;
      }
      // HLT
      if ( obj.hasTriggerObjectType(trigger::TriggerMuon) && dR < 0.1 ) {
        idx_HLT = n;
      }
      n++;
    }

    cout << "Reco muon: " << "pt = "  << setw(7) << it1->pt() << " " 
                          << "eta = " << setw(6) << it1->eta() << " " 
                          << "phi = " << setw(6) << it1->phi() << " "
                          << " is matched to: " <<  it1->triggered("HLT_IsoMu*") << endl;

    if (idx_G > -1 )   cout << " GEN muon: " << "pt = " << setw(7) << genColl->at(idx_G).pt() << " "
                                    << "eta = " << setw(6) << genColl->at(idx_G).eta() << " " 
                                    << "phi = " << setw(6) << genColl->at(idx_G).phi() << " "
                                    << endl;
    if (idx_L1 > -1 ) cout << " L1  muon: " << "pt = " << setw(7) << triggerObjects->at(idx_L1).pt() << " "
                                    << "eta = " << setw(6) << triggerObjects->at(idx_L1).eta() << " "
                                    << "phi = " << setw(6) << triggerObjects->at(idx_L1).phi() << " "
                                    << endl;
    if (idx_HLT > -1 ) cout << " HLT muon: " << "pt = " << setw(7) << triggerObjects->at(idx_HLT).pt() << " "
                                    << "eta = " << setw(6) << triggerObjects->at(idx_HLT).eta() << " "
                                    << "phi = " << setw(6) << triggerObjects->at(idx_HLT).phi() << " "
                                    << endl;
  }
 
  // restore output format flags and precision
  cout.flags(old_settings);
  cout.precision(old_precision);

}


// ------------ method called once each job just before starting event loop  ------------
void 
MuonExercise1::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MuonExercise1::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
MuonExercise1::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
MuonExercise1::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
MuonExercise1::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
MuonExercise1::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MuonExercise1::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MuonExercise1);
