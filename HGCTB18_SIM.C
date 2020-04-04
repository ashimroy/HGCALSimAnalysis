#define HGCTB18_SIM_cxx
#include "HGCTB18_SIM.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

// User Class
#include "HGCalTBTopology.C"
#include "HGCalTestNumbering.C"
// Includes by User
#include <TMath.h>
#include <TMathBase.h>
#include "TFile.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "TLorentzVector.h"
#include "TStopwatch.h"

int DEBUG     = 0; // 0 for no print, 1 for print                                                                                      
using namespace std;

void HGCTB18_SIM::Loop(string outputfile, float mipCut, int ntotEvent, int nPrintEvent)
{
   TStopwatch sw;
   sw.Start();

   if (fChain == 0) return;

   cout<<"Output file : " << outputfile << endl;
   char *outfilename = const_cast<char*>(outputfile.c_str());
   TFile *fileout = new TFile(outfilename,"RECREATE");
   TTree *t1 = new TTree("SimTree","HGCalTB Sim Tree");

   Long64_t nentries = fChain->GetEntriesFast();

   std::cout << "Beam Momentum: " << pBeam << " GeV" << std::endl;
   std::cout << "Total entries: " << nentries << std::endl;

   if (ntotEvent >= 0) nentries = ntotEvent; // number of events you want to process..  
   std::cout << "Running on Total entries: " << nentries << std::endl;

   float MipToGeV200   = 57.0e-06; // 200 micron Si
   float MipToGeV300   = 85.5e-06; // 300 micron Si
   float MipToGeV      = MipToGeV300;
   //float GeVToMeV   = 1000;
   float GeVToMIP200   = 1.0/MipToGeV200;
   float GeVToMIP300   = 1.0/MipToGeV300;
   float GeVToMIP      = GeVToMIP300;

   // Tree Variables
   unsigned int   nLayer=40;
   float         sumEAll=0;
   vector<float> sumELayer;   sumELayer.clear();
   //vector<float> maxELayer;   maxELayer.clear();
   vector<float> E1Layer;     E1Layer.clear();
   vector<float> E7Layer;     E7Layer.clear();
   vector<float> E19Layer;    E19Layer.clear();
   vector<float> EAllLayer;    E19Layer.clear();

   t1->Branch("nLayer",&nLayer);
   t1->Branch("sumEAll",&sumEAll);
   t1->Branch("sumELayer",&sumELayer);
   //t1->Branch("maxELayer",&maxELayer);
   t1->Branch("E1Layer",&E1Layer);
   t1->Branch("E7Layer",&E7Layer);
   t1->Branch("E19Layer",&E19Layer);
   t1->Branch("EAllLayer",&EAllLayer);

   HexTopology ht1(false);
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if (jentry % nPrintEvent == 0) std::cout << "  " << jentry  << "  Events Processed... " << std::endl;

      float TotEdepEEMip = 0., TotEdepFHMip = 0.;


      // HGCAL EE
      if (DEBUG==1 && jentry % nPrintEvent == 0) cout<< "simHitLayEn2EE->size()" << simHitLayEn2EE->size() << endl;
      if (DEBUG==0 && jentry % nPrintEvent == 0) cout<< "simHitCellIdEE->size()" << simHitCellIdEE->size() << endl;
      if (DEBUG==0 && jentry % nPrintEvent == 0) cout<< "simHitCellEnEE->size()" << simHitCellEnEE->size() << endl;
      /*
      for (unsigned int icell = 0; icell < simHitCellIdEE->size(); icell++) {
	uint32_t id = simHitCellIdEE->at(icell);
	int subdet, z, layer, wafer, celltype, cellno;
	HGCalTestNumbering::unpackHexagonIndex(id, subdet, z, layer, wafer, celltype, cellno);
	cout << "id, subdet, z, layer, wafer, celltype, cellno:  " << id << " " << subdet << " "
	     << z << " " << layer << " " << wafer << " " << celltype << " " << cellno << endl;
      }
      */
      unsigned int icellEE_start = 0;
      for (unsigned int ilayer=0; ilayer<simHitLayEn2EE->size(); ilayer++){
	if (ilayer+1==27 || ilayer+1==28) {
	  GeVToMIP = GeVToMIP200;
	  MipToGeV = MipToGeV200;
	}
	unsigned int locMaxId = ht1.localMax( (*simHitCellIdEE), (*simHitCellEnEE), ilayer+1 );
	double clusterE1 = ht1.cluster( (*simHitCellIdEE), (*simHitCellEnEE), locMaxId, 0, mipCut*MipToGeV );
	double clusterE7 = ht1.cluster( (*simHitCellIdEE), (*simHitCellEnEE), locMaxId, 1, mipCut*MipToGeV );
	double clusterE19 = ht1.cluster( (*simHitCellIdEE), (*simHitCellEnEE), locMaxId, 2, mipCut*MipToGeV );
	//double clusterEAll = ht1.cluster( (*simHitCellIdEE), (*simHitCellEnEE), locMaxId, 7, mipCut*MipToGeV );
	double clusterEAll = 0.;
	for (unsigned int icell = icellEE_start; icell < simHitCellIdEE->size(); icell++) {
	  uint32_t id = simHitCellIdEE->at(icell);
	  int subdet, z, layer, wafer, celltype, cellno;
	  HGCalTestNumbering::unpackHexagonIndex(id, subdet, z, layer, wafer, celltype, cellno);
	  if (layer != ilayer+1) break;
	  if (DEBUG==1 && jentry % nPrintEvent == 0) cout<< "icellEE_start : layer : ilayer+1   "<< icellEE_start << " : " 
	      << layer << " : " << ilayer+1 << endl;
	  double CellE = simHitCellEnEE->at(icell);
	  if (CellE > mipCut*MipToGeV) clusterEAll+= CellE;
	  icellEE_start = icell+1;
	}

	double ilayerE = simHitLayEn2EE->at(ilayer) * GeVToMIP;
	E1Layer.push_back( clusterE1 * GeVToMIP );
	E7Layer.push_back( clusterE7 * GeVToMIP );
	E19Layer.push_back( clusterE19 * GeVToMIP );
	EAllLayer.push_back( clusterEAll * GeVToMIP );
	TotEdepEEMip += ilayerE;
	sumELayer.push_back(ilayerE);
      }

      // HGCAL FH
      if (DEBUG==1 && jentry % nPrintEvent == 0) cout<< "simHitLayEn2FH->size()" << simHitLayEn2FH->size() << endl;
      if (DEBUG==0 && jentry % nPrintEvent == 0) cout<< "simHitCellIdFH->size()" << simHitCellIdFH->size() << endl;
      if (DEBUG==0 && jentry % nPrintEvent == 0) cout<< "simHitCellEnFH->size()" << simHitCellEnFH->size() << endl;
      /*
      for (unsigned int icell = 0; icell < simHitCellIdFH->size(); icell++) {
        uint32_t id = simHitCellIdFH->at(icell);
        int subdet, z, layer, wafer, celltype, cellno;
	HGCalTestNumbering::unpackHexagonIndex(id, subdet, z, layer, wafer, celltype, cellno);
        cout << "id, subdet, z, layer, wafer, celltype, cellno:  " << id << " " << subdet << " "
             << z << " " << layer << " " << wafer << " " << celltype << " " << cellno << endl;
      }
      */
      unsigned int icellFH_start = 0;
      for (unsigned int ilayer=0; ilayer<simHitLayEn2FH->size(); ilayer++){
	unsigned int locMaxId = ht1.localMax( (*simHitCellIdFH), (*simHitCellEnFH), ilayer+1 );
        double clusterE1 = ht1.cluster( (*simHitCellIdFH), (*simHitCellEnFH), locMaxId, 0, mipCut*MipToGeV );
        double clusterE7 = ht1.cluster( (*simHitCellIdFH), (*simHitCellEnFH), locMaxId, 1, mipCut*MipToGeV );
        double clusterE19 = ht1.cluster( (*simHitCellIdFH), (*simHitCellEnFH), locMaxId, 2, mipCut*MipToGeV );
        //double clusterEAll = ht1.cluster( (*simHitCellIdFH), (*simHitCellEnFH), locMaxId, 18, mipCut*MipToGeV );
	double clusterEAll = 0.;
	for (unsigned int icell = icellFH_start; icell < simHitCellIdFH->size(); icell++) {
          uint32_t id = simHitCellIdFH->at(icell);
          int subdet, z, layer, wafer, celltype, cellno;
	  HGCalTestNumbering::unpackHexagonIndex(id, subdet, z, layer, wafer, celltype, cellno);
          if (layer != ilayer+1) break;
	  if (DEBUG==1 && jentry % nPrintEvent == 0) cout<< "icellFH_start : layer : ilayer+1   "<< icellFH_start << " : " 
	      << layer << " : " << ilayer+1 << endl;
	  double CellE = simHitCellEnFH->at(icell);
	  if (CellE > mipCut*MipToGeV) clusterEAll+= CellE;
          icellFH_start = icell+1;
        }

	double ilayerE = simHitLayEn2FH->at(ilayer) * GeVToMIP;
	E1Layer.push_back( clusterE1 * GeVToMIP );
	E7Layer.push_back( clusterE7 * GeVToMIP );
	E19Layer.push_back( clusterE19 * GeVToMIP );
	EAllLayer.push_back( clusterEAll * GeVToMIP );
	TotEdepFHMip += ilayerE;
	sumELayer.push_back(ilayerE);
      }

      /*
      // HGCAL BH
      if (DEBUG==0 && jentry % nPrintEvent == 0) cout<< "simHitCellIdBH->size()" << simHitCellIdBH->size() << endl;
      if (DEBUG==0 && jentry % nPrintEvent == 0) cout<< "simHitCellEnBH->size()" << simHitCellEnBH->size() << endl;

      for (unsigned int icell = 0; icell < simHitCellIdBH->size(); icell++) {
        uint32_t id = simHitCellIdBH->at(icell);
        int subdet, z, layer, wafer, celltype, cellno;
	HGCalTestNumbering::unpackHexagonIndex(id, subdet, z, layer, wafer, celltype, cellno);
        cout << "id, subdet, z, layer, wafer, celltype, cellno:  " << id << " " << subdet << " "
             << z << " " << layer << " " << wafer << " " << celltype << " " << cellno << endl;
      }
      */


      nLayer = simHitLayEn2EE->size() + simHitLayEn2FH->size();
      sumEAll = TotEdepEEMip + TotEdepFHMip;

      t1->Fill();
      sumELayer.clear();
      //maxELayer.clear();
      E1Layer.clear();
      E7Layer.clear();
      E19Layer.clear();
      EAllLayer.clear();
   } // Event Loop

   fileout->Write();
   fileout->Close();

   sw.Stop();
   std::cout << "RealTime : " << sw.RealTime() / 60.0 << " minutes" << std::endl;
   std::cout << "CPUTime  : " << sw.CpuTime()  / 60.0 << " minutes" << std::endl;

}
