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
#include <cmath>

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

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"


#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TTree.h"
#include "TFile.h"

using reco::TrackCollection;


/**
 * Recursively traverse through the mother of each particle until 
 * you find a parton, or a particle has no mother. Save the found
 * partons in a set.
 *
 * @param original - current particle in the traversal
 * @param partonSet - set to save all of the found partons to
 */
void findPartonMother(const reco::Candidate *original, std::set<const reco::Candidate*> &partonSet) {
    int numMoms = original->numberOfMothers();
    if (numMoms == 0) return;

    for (int j=0; j < numMoms; j++) {
        const reco::Candidate *mom = original->mother(j);
        int status = mom->status();
        if (status < 70 || status > 80) {
            findPartonMother(mom, partonSet);
        }
        else {
            partonSet.insert(mom);
        }

    }
    return;
}

/**
 * Overload of above function for different particle type.
 *
 * Looping through the gen particles in the event yields
 * objects of the type reco::GenParticle. However, using the
 * .mother() function returns objects of the type
 * const reco::Candidate*, which necessitates the need for
 * these two functions.
 */
void findPartonMother(reco::GenParticle original, std::set<const reco::Candidate*> &partonSet) {
    int numMoms = original.numberOfMothers();
    if (numMoms == 0) return;

    for (int j=0; j < numMoms; j++) {
        const reco::Candidate *mom = original.mother(j);
        int status = mom->status();
        if (status < 70 || status > 80) {
            findPartonMother(mom, partonSet);
        }
        else {
            partonSet.insert(mom);
        }
    }
    return;
}


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
      edm::EDGetTokenT<reco::PFCandidateCollection> pfCandsToken_;

      // define tree
      TTree* eventTree;

      // define containers to be used for tree branches
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

      std::vector<Float_t> pfCandPt;
      std::vector<Float_t> pfCandEta;
      std::vector<Float_t> pfCandPhi;
      std::vector<Float_t> pfCandPx;
      std::vector<Float_t> pfCandPy;
      std::vector<Float_t> pfCandPz;
      std::vector<Float_t> pfCandE;

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
      };


// constructors and destructor
MyAnalyzer::MyAnalyzer(const edm::ParameterSet& iConfig) :
    pfJetsToken_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("ak4PFJets"))),
    pfJetsCHSToken_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("ak4PFJets"))),
    genJetsToken_(consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("ak4GenJets"))),
    partonsToken_(consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticles"))),
    pfCandsToken_(consumes<reco::PFCandidateCollection>(iConfig.getParameter<edm::InputTag>("particleFlow")))


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

    pfCandPt.clear();
    pfCandEta.clear();
    pfCandPhi.clear();
    pfCandPx.clear();
    pfCandPy.clear();
    pfCandPz.clear();
    pfCandE.clear();

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

    using namespace edm;
    edm::Handle<reco::PFJetCollection> pfJetH;
    iEvent.getByToken(pfJetsToken_, pfJetH);

    edm::Handle<reco::PFCandidateCollection> pfCandsH;
    iEvent.getByToken(pfCandsToken_, pfCandsH);

    edm::Handle<reco::GenJetCollection> genJetH;
    iEvent.getByToken(genJetsToken_, genJetH);

    edm::Handle<std::vector<reco::GenParticle>> genParticlesH;
    iEvent.getByToken(partonsToken_, genParticlesH);
   
    
    // loop through reco jets and save 4-momenta info
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

    // loop through PF Candidates and save 4-momenta info
    for (reco::PFCandidateCollection::const_iterator particle=pfCandsH->begin(); particle != pfCandsH->end(); ++particle) {
        pfCandPt.push_back(particle->pt());
        pfCandEta.push_back(particle->eta());
        pfCandPhi.push_back(particle->phi());
        pfCandPx.push_back(particle->px());
        pfCandPy.push_back(particle->py());
        pfCandPz.push_back(particle->pz());
        pfCandE.push_back(particle->energy());
    }


    // loop through gen jets and save 4-momenta info
    for (reco::GenJetCollection::const_iterator jet=genJetH->begin(); jet != genJetH->end(); ++jet) {
        genJetPt.push_back(jet->pt());
        genJetEta.push_back(jet->eta());
        genJetPhi.push_back(jet->phi());
        genJetPx.push_back(jet->px());
        genJetPy.push_back(jet->py());
        genJetPz.push_back(jet->px());
        genJetE.push_back(jet->energy());
    }
    

    std::set<const reco::Candidate*> partonSet;

    // loop through stable gen particles, recursivly backtrack through the
    // event and save the found partons to a set
    for (std::vector<reco::GenParticle>::const_iterator particle = genParticlesH->begin(); particle != genParticlesH->end(); particle++) {
        const reco::GenParticle stableParticle = *particle;
        if (stableParticle.status() == 1) {
            findPartonMother(stableParticle, partonSet);
        }
    }

    // iterate through the set of partons and save 4-momenta information
    for (std::set<const reco::Candidate*>::iterator it = partonSet.begin(); it != partonSet.end(); it++){
        const reco::Candidate *parton = *it;
        partonPt.push_back(parton->pt());
        partonEta.push_back(parton->eta());
        partonPhi.push_back(parton->phi());
        partonPx.push_back(parton->px());
        partonPy.push_back(parton->py());
        partonPz.push_back(parton->pz());
        partonE.push_back(parton->energy());
        partonPdgId.push_back(parton->pdgId());
        partonStatus.push_back(parton->status());

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


    eventTree->Branch("pfCandPt", &pfCandPt);
    eventTree->Branch("pfCandEta", &pfCandEta);
    eventTree->Branch("pfCandPhi", &pfCandPhi);
    eventTree->Branch("pfCandPx", &pfCandPx);
    eventTree->Branch("pfCandPy", &pfCandPy);
    eventTree->Branch("pfCandPz", &pfCandPz);
    eventTree->Branch("pfCandE", &pfCandE);
    
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
