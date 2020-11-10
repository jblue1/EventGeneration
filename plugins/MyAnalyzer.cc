// -*- C++ -*-
//
// Package:    analyzer/MyAnalyzer
// Class:      MyAnalyzer
//
/**\class MyAnalyzer MyAnalyzer.cc analyzer/MyAnalyzer/plugins/MyAnalyzer.cc

 Description: Save 4-momenta and related information for partons, genJets and recoJets

 Implementation:
     Saving gen particles with pythia status codes 70-79 as partons to later be clustered into jets
*/
//
//   Created:  Tue, 20 Oct 2020 03:57:13 GMT


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"


#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TTree.h"
#include "TFile.h"

using reco::TrackCollection;

class MyAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit MyAnalyzer(const edm::ParameterSet&);
      ~MyAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<reco::PFJetCollection> pfJetsToken_;
      edm::EDGetTokenT<reco::PFJetCollection> pfJetsCHSToken_;
      edm::EDGetTokenT<reco::GenJetCollection> genJetsToken_;
      edm::EDGetTokenT<std::vector<reco::GenParticle>> partonsToken_;

      TTree* eventTree;

      std::vector<Float_t> pfJetPt;
      std::vector<Float_t> pfJetEta;
      std::vector<Float_t> pfJetPhi;
      std::vector<Float_t> pfJetPx;
      std::vector<Float_t> pfJetPy;
      std::vector<Float_t> pfJetPz;
      std::vector<Float_t> pfJetE;
      std::vector<Float_t> pfJetPhotonEnergy;
      std::vector<Float_t> pfJetElectronEnergy;
      std::vector<Float_t> pfJetMuonEnergy;

      std::vector<Float_t> pfJetCHSPt;
      std::vector<Float_t> pfJetCHSEta;
      std::vector<Float_t> pfJetCHSPhi;
      std::vector<Float_t> pfJetCHSPx;
      std::vector<Float_t> pfJetCHSPy;
      std::vector<Float_t> pfJetCHSPz;
      std::vector<Float_t> pfJetCHSE;

      std::vector<Float_t> genJetPt;
      std::vector<Float_t> genJetEta;
      std::vector<Float_t> genJetPhi;
      std::vector<Float_t> genJetPx;
      std::vector<Float_t> genJetPy;
      std::vector<Float_t> genJetPz;
      std::vector<Float_t> genJetE;

      std::vector<Float_t> partonPt;
      std::vector<Float_t> partonEta;
      std::vector<Float_t> partonPhi;
      std::vector<Float_t> partonPx;
      std::vector<Float_t> partonPy;
      std::vector<Float_t> partonPz;
      std::vector<Float_t> partonE;
      std::vector<Int_t> partonPdgId;
      std::vector<Int_t> partonStatus;

      std::vector<Float_t> hadronPt;
      std::vector<Float_t> hadronEta;
      std::vector<Float_t> hadronPhi;
      std::vector<Float_t> hadronPx;
      std::vector<Float_t> hadronPy;
      std::vector<Float_t> hadronPz;
      std::vector<Float_t> hadronE;
      std::vector<Int_t> hadronPdgId;
      std::vector<Int_t> hadronStatus;

};


// constructors and destructor
MyAnalyzer::MyAnalyzer(const edm::ParameterSet& iConfig) :
    pfJetsToken_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("ak4PFJets"))),
    pfJetsCHSToken_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("ak4PFJets"))),
    genJetsToken_(consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("ak4GenJets"))),
    partonsToken_(consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticles")))

{
   //now do what ever initialization is needed

}


MyAnalyzer::~MyAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

    // clear all vectors before processing each event
    pfJetPt.clear();
    pfJetEta.clear();
    pfJetPhi.clear();
    pfJetPx.clear();
    pfJetPy.clear();
    pfJetPz.clear();
    pfJetE.clear();
    pfJetPhotonEnergy.clear();
    pfJetElectronEnergy.clear();
    pfJetMuonEnergy.clear();

    pfJetCHSPt.clear();
    pfJetCHSEta.clear();
    pfJetCHSPhi.clear();
    pfJetCHSPx.clear();
    pfJetCHSPy.clear();
    pfJetCHSPz.clear();
    pfJetCHSE.clear();

    genJetPt.clear();
    genJetEta.clear();
    genJetPhi.clear();
    genJetPx.clear();
    genJetPy.clear();
    genJetPz.clear();
    genJetE.clear();

    partonPt.clear();
    partonEta.clear();
    partonPhi.clear();
    partonPx.clear();
    partonPy.clear();
    partonPz.clear();
    partonE.clear();
    partonPdgId.clear();
    partonStatus.clear();

    hadronPt.clear();
    hadronEta.clear();
    hadronPhi.clear();
    hadronPx.clear();
    hadronPy.clear();
    hadronPz.clear();
    hadronE.clear();
    hadronPdgId.clear();
    hadronStatus.clear();


    using namespace edm;
    edm::Handle<reco::PFJetCollection> pfJetH;
    iEvent.getByToken(pfJetsToken_, pfJetH);

    edm::Handle<reco::PFJetCollection> pfJetCHSH;
    iEvent.getByToken(pfJetsCHSToken_, pfJetCHSH);

    edm::Handle<reco::GenJetCollection> genJetH;
    iEvent.getByToken(genJetsToken_, genJetH);

    edm::Handle<std::vector<reco::GenParticle>> partonsH;
    iEvent.getByToken(partonsToken_, partonsH);
   
    for (reco::PFJetCollection::const_iterator jet=pfJetH->begin(); jet != pfJetH->end(); ++jet) {
        pfJetPt.push_back(jet->pt());
        pfJetEta.push_back(jet->eta());
        pfJetPhi.push_back(jet->phi());
        pfJetPx.push_back(jet->px());
        pfJetPy.push_back(jet->py());
        pfJetPz.push_back(jet->px());
        pfJetE.push_back(jet->energy());
        pfJetPhotonEnergy.push_back(jet->photonEnergy());
        pfJetElectronEnergy.push_back(jet->electronEnergy());
        pfJetMuonEnergy.push_back(jet->muonEnergy());
    }

    for (reco::PFJetCollection::const_iterator jet=pfJetCHSH->begin(); jet != pfJetCHSH->end(); ++jet) {
        pfJetCHSPt.push_back(jet->pt());
        pfJetCHSEta.push_back(jet->eta());
        pfJetCHSPhi.push_back(jet->phi());
        pfJetCHSPx.push_back(jet->px());
        pfJetCHSPy.push_back(jet->py());
        pfJetCHSPz.push_back(jet->px());
        pfJetCHSE.push_back(jet->energy());
    }

    for (reco::GenJetCollection::const_iterator jet=genJetH->begin(); jet != genJetH->end(); ++jet) {
        genJetPt.push_back(jet->pt());
        genJetEta.push_back(jet->eta());
        genJetPhi.push_back(jet->phi());
        genJetPx.push_back(jet->px());
        genJetPy.push_back(jet->py());
        genJetPz.push_back(jet->px());
        genJetE.push_back(jet->energy());
    }

    std::set<std::vector<Float_t>> hadrons;
    std::set<const reco::Candidate*> hadronPointers;
    int numHadrons = 0;
    for (std::vector<reco::GenParticle>::const_iterator particle = partonsH->begin(); particle != partonsH->end(); particle++) {
        if (particle->status() > 69 && particle->status() < 80) {
            partonPt.push_back(particle->pt());
            partonEta.push_back(particle->eta());
            partonPhi.push_back(particle->phi());
            partonPx.push_back(particle->px());
            partonPy.push_back(particle->py());
            partonPz.push_back(particle->pz());
            partonE.push_back(particle->energy());
            partonPdgId.push_back(particle->pdgId());
            partonStatus.push_back(particle->status());
            int numDaught = particle->numberOfDaughters();
            for (int j=0; j < numDaught; j++) {
                const reco::Candidate* d = particle->daughter(j);
                hadronPointers.insert(d);
                numHadrons++;
            }
        }
    }
    std::cout << "Num Hadrons 1: " << hadrons.size() << std::endl;
    std::cout << "Num Hadrons 2: " << hadronPointers.size() << std::endl;

    for (auto it = hadronPointers.begin(); it != hadronPointers.end(); it++) {
        hadronPt.push_back((*it)->pt());
        hadronEta.push_back((*it)->eta());
        hadronPhi.push_back((*it)->phi());
        hadronPx.push_back((*it)->px());
        hadronPy.push_back((*it)->py());
        hadronPz.push_back((*it)->pz());
        hadronE.push_back((*it)->energy());
        hadronPdgId.push_back((*it)->pdgId());
        hadronStatus.push_back((*it)->status());
    }

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
    
    eventTree->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void
MyAnalyzer::beginJob() {
    edm::Service<TFileService> fs;
    eventTree = fs->make<TTree>("eventTree", "eventTree");

    eventTree->Branch("pfJetPt", &pfJetPt);
    eventTree->Branch("pfJetEta", &pfJetEta);
    eventTree->Branch("pfJetPhi", &pfJetPhi);
    eventTree->Branch("pfJetPx", &pfJetPx);
    eventTree->Branch("pfJetPy", &pfJetPy);
    eventTree->Branch("pfJetPz", &pfJetPz);
    eventTree->Branch("pfJetE", &pfJetE);
    eventTree->Branch("pfJetPhotonEnergy", &pfJetPhotonEnergy);
    eventTree->Branch("pfJetElectronEnergy", &pfJetElectronEnergy);
    eventTree->Branch("pfJetMuonEnergy", &pfJetMuonEnergy);
    


    eventTree->Branch("pfJetCHSPt", &pfJetCHSPt);
    eventTree->Branch("pfJetCHSEta", &pfJetCHSEta);
    eventTree->Branch("pfJetCHSPhi", &pfJetCHSPhi);
    eventTree->Branch("pfJetCHSPx", &pfJetCHSPx);
    eventTree->Branch("pfJetCHSPy", &pfJetCHSPy);
    eventTree->Branch("pfJetCHSPz", &pfJetCHSPz);
    eventTree->Branch("pfJetCHSE", &pfJetCHSE);

    eventTree->Branch("genJetPt", &genJetPt);
    eventTree->Branch("genJetEta", &genJetEta);
    eventTree->Branch("genJetPhi", &genJetPhi);
    eventTree->Branch("genJetPx", &genJetPx);
    eventTree->Branch("genJetPy", &genJetPy);
    eventTree->Branch("genJetPz", &genJetPz);
    eventTree->Branch("genJetE", &genJetE);

    eventTree->Branch("partonPt", &partonPt);
    eventTree->Branch("partonEta", &partonEta);
    eventTree->Branch("partonPhi", &partonPhi);
    eventTree->Branch("partonPx", &partonPx);
    eventTree->Branch("partonPy", &partonPy);
    eventTree->Branch("partonPz", &partonPz);
    eventTree->Branch("partonE", &partonE);
    eventTree->Branch("partonPdgId", &partonPdgId);
    eventTree->Branch("partonStatus", &partonStatus);

    eventTree->Branch("hadronPx", &hadronPx);
    eventTree->Branch("hadronPy", &hadronPy);
    eventTree->Branch("hadronPz", &hadronPz);
    eventTree->Branch("hadronE", &hadronE);
    eventTree->Branch("hadronPt", &hadronPt);
    eventTree->Branch("hadronEta", &hadronEta);
    eventTree->Branch("hadronPhi", &hadronPhi);
    eventTree->Branch("hadronPdgId", &hadronPdgId);
    eventTree->Branch("hadronStatus", &hadronStatus);
}

// ------------ method called once each job just after ending the event loop  ------------
void
MyAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MyAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MyAnalyzer);
