#include <vector>
#include <iostream>
#include <algorithm>

int whichSciFi(std::vector<int> scifiBoards, UInt_t bID[], UChar_t tofID[], UInt_t nhits, bool isTestBeam=true, int minTofpetHits=64);


void showerStart(int runN, int energy, int nwalls, int files=20){
    // Add desired TTrees to TChain (from test beam)
    TChain* event = new TChain("data");
    for (int j=0; j<10 && j<files; j++){
        event->Add(Form("/eos/experiment/sndlhc/raw_data/testbeam_23/all/run_%i/data_000%d.root",runN,j));
    }
    if (files>10) {
        for (int j=10; j<files; j++){
            event->Add(Form("/eos/experiment/sndlhc/raw_data/testbeam_23/all/run_%i/data_00%d.root",runN,j));
        }  
    }
    // Add desired TTrees to TChain (from TI18)
    /*
    for (int j=0; j<10 && j<files; j++){
        event->Add(Form("/eos/experiment/sndlhc/raw_data/physics/2022/run_00%i/data_000%d.root",runN,j));
    }
    if (files>10) {
        for (int j=10; j<files; j++){
            event->Add(Form("/eos/experiment/sndlhc/raw_data/physics/2022/run_00%i/data_00%d.root",runN,j));
        }  
    }
    */
    // Scifi boards TB: {scifi1X, scifi1Y, scifi2x, ...}
    std::vector<int> scifiBoardsTB = {24,53,56,54,45,33,37,38};
    // Scifi boards TI18: {scifi1X1, scifi1X2, scifi1X3, scifi1Y1, scifi1Y2, scifi1Y3, scifi2X1,...}
    std::vector<int> scifiBoardsTI18 = {11,17,28,29,3,30,23,2,25,16,14,18,22,27,4,15,9,5,8,50,49,46,40,20,21,10,6,19,13,36};
    // Output ROOT file
    TFile* outf = new TFile(Form("scifi_%i_%iW.root",energy,nwalls),"recreate");
    // Histogram with shower position
    TH1I* hShower = new TH1I("Start of shower wall TB","Start of shower wall TB;wall;entries",5,-1,4);
    //TH1I* hShower = new TH1I("Start of shower wall TI18","Start of shower wall TI18;wall;entries",6,-1,5);

    // Variables used to store data event by event
    UInt_t bID[5000];
    UInt_t hits;
    UChar_t tofID[5000];

    event->SetBranchAddress("board_id",bID);
    event->SetBranchAddress("tofpet_id",tofID);
    event->SetBranchAddress("n_hits",&hits);

    // Loop over all the events
    for(int j=0; j<event->GetEntries(); j++) {
        event->GetEntry(j);
        hShower->Fill(whichSciFi(scifiBoardsTB,bID, tofID, hits));
	}

    hShower->Write();
    outf->Close();
    std::cout<<Form("DONE %i %i\n",energy,nwalls);
} 


int whichSciFi(std::vector<int> scifiBoards, UInt_t bID[], UChar_t tofID[], UInt_t nhits, bool isTestBeam=true, int minTofpetHits=64) {
    if (isTestBeam) {
        std::vector<int> tofPetHits(64, 0);  // (nelements, value)
        for (int i=0; i<nhits; i++) {
            // check which hits are scifi
            auto it = std::find(scifiBoards.begin(), scifiBoards.end(), bID[i]);
            if (it != scifiBoards.end()) {
                int index = std::distance(scifiBoards.begin(), it);
                for (int j=0; j<8; j++) {
                    if (tofID[i]==j) {
                        tofPetHits[index*8 + j]++;
                        break;
                    }
                }

            }
        }
        for (int k=0; k<tofPetHits.size(); k++) {
            if (tofPetHits[k]>=minTofpetHits) {
                return (int) k/16;  // in TB first scifi=0 -> first wall = 1
            }
        }
        return -1; //if no shower is present
    }
    else {
        std::vector<int> tofPetHits(240, 0);  // (nelements, value)
        for (int i=0; i<nhits; i++) {
            // check which hits are scifi
            auto it = std::find(scifiBoards.begin(), scifiBoards.end(), bID[i]);
            if (it != scifiBoards.end()) {
                int index = std::distance(scifiBoards.begin(), it);
                for (int j=0; j<8; j++) {
                    if (tofID[i]==j) {
                        tofPetHits[index*8 + j]++;
                        break;
                    }
                }

            }
        }
        for (int k=0; k<tofPetHits.size(); k++) {
            if (tofPetHits[k]>=minTofpetHits) {
                return (int) k/48;  // In TI18 first scifi=0 -> first wall = 0
            }
        }
        return -1; //if no shower is present
    }
}
