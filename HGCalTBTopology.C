#include <TCanvas.h>
#include <TChain.h>
#include <TFile.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TPaveText.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TROOT.h>
#include <TStyle.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Compare plots with and without beamline counters
void PlotHistTBCompare(std::string infile1, std::string infile2, 
		       std::string text, std::string prefix, 
		       int maxMod=16, bool save=false);
// Plot energy distribution in a given layer
void PlotHistTBHitEn(std::string infile, std::string text, std::string prefix, 
		     int type, bool separate, int rebin, int maxMod=16, 
		     bool save=false);
// Plot general TB plots at SIM/Digi/Reco labels
void PlotHistSimDigRec(std::string fname="TBGenSimDigiReco.root", 
		       std::string dirnm="HGCalTBAnalyzer", int type=0,
		       std::string prefix="EL32", int maxMod=16,
		       bool save=false);
// Class to manipulate the Tree produced by the analysis code
// HGCTB l1(std::string infile, std::string outfile);
// l1.Loop()

class HexTopology {

public :
  HexTopology(bool fine);
  virtual ~HexTopology() {}

  double             cluster(const std::vector<unsigned int>& ids,
			     const std::vector<float>& energy, 
			     unsigned int id, int extent, float cutE);
  unsigned int       localMax(const std::vector<unsigned int>& ids,
			      const std::vector<float>& energy, 
			      unsigned int layer);
  std::vector<int>   neighbours(int cell, int extent);
  std::pair<int,int> findRowCol(int cell);
private :
  std::vector<int> leftCell, cells, addCell, subCell;
  unsigned int     nrows;
};

HexTopology::HexTopology(bool fine) {
  int cellCoarse[15] = {2,5,8,11,12,11,12,11,12,11,12,11,8,5,2};
  int addCoarse[15] =  {1,0,1, 0, 1, 0, 1, 0, 1, 0, 1, 0,1,0,1};
  int cellFine[20] = {3,6,9,12,15,16,15,16,15,16,15,16,15,16,15,14,11,8,5,2};
  int addFine[20]  = {0,1,0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,1,0,1};
  int fullCoarse(11), fullFine(15);
  
  int full(0);
  if (fine) {
    for (int k=0; k<20; ++k) {
      cells.push_back(cellFine[k]); addCell.push_back(addFine[k]);
    }
    full = fullFine;
  } else {
    for (int k=0; k<15; ++k) {
      cells.push_back(cellCoarse[k]); addCell.push_back(addCoarse[k]);
    }
    full = fullCoarse;
  }
  leftCell.push_back(0);
  nrows = cells.size();
  for (unsigned int k=0; k<nrows; ++k) {
    leftCell.push_back((leftCell[k]+cells[k]));
    int ii = (cells[k]-full -1)/2;
    subCell.push_back(ii);
  }
  /*
  std::cout << "Initialize Wafer of type " << fine << " with " << nrows
	    << " rows and " << leftCell[nrows] << " cells" << std::endl;
  for (unsigned int k=0; k<nrows; ++k) 
    std::cout << "Row[" << k << "] with " << cells[k] << ":" << addCell[k]
	      << " cells: leftmost " << leftCell[k] << " offset "
	      << subCell[k] << std::endl;
  */
}

std::pair<int,int> HexTopology::findRowCol(int cell) {
  int row(-1), col(-1);
  if (cell >= leftCell[0] && cell < leftCell[nrows]) {
    for (unsigned int i=0; i<=nrows; ++i) {
      if (cell < leftCell[i]) {
	row = i-1; col = cell-leftCell[i-1]-subCell[i-1]-addCell[i-1]; break;
      }
    }
  }
// std::cout << "Cell " << cell << " row " << row << " column " << col << std::endl;
  return std::pair<int,int>(row,col);
}

std::vector<int> HexTopology::neighbours(int cell, int extent) {
  int cols[11] = {0,-1,1,-2,2,-3,3,-4,4,-5,5};
  std::vector<int>   listCell;
  std::pair<int,int> rowCol = findRowCol(cell);
  if (rowCol.first >= 0) {
    int nadd = 2*extent + 1;
    int addc = addCell[rowCol.first];
    for (int i = 0; i <= extent; ++i) {
      for (int j = 0; j < nadd; ++j) {
	int row = rowCol.first + i;
	if (row >= 0 && row <= (int)(nrows)) {
	  int adc = (addc > addCell[row]) ? addc : addCell[row];
	  int col = rowCol.second+cols[j]+leftCell[row]+subCell[row]+adc;
	  if (col >= leftCell[row] && col < leftCell[row+1]) 
	    listCell.push_back(col);
	}
	if (i > 0) {
	  int row = rowCol.first - i;
	  if (row >= 0 && row <= (int)(nrows)) {
	    int adc = (addc > addCell[row]) ? addc : addCell[row];
	    int col = rowCol.second+cols[j]+leftCell[row]+subCell[row]+adc;
	    if (col >= leftCell[row] && col < leftCell[row+1]) 
	      listCell.push_back(col);
	  }
	}
      }
      nadd--;
    }
  }
  return listCell;
}

double HexTopology::cluster(const std::vector<unsigned int>& ids,
			    const std::vector<float>& energies, 
			    unsigned int id, int extent, float cutE) {
  double energy(0);
  int cell = id&0xFF;
  std::vector<int> listCell = neighbours(cell, extent);
  for (unsigned int k=0; k<ids.size(); ++k) {
    int idcell = ids[k]&0xFF;
    if ((std::find(listCell.begin(),listCell.end(),idcell) != listCell.end()) &&
	((id&0xFFFFFF00) == (ids[k]&0xFFFFFF00))  && energies[k] > cutE ) 
      energy += energies[k];
  }
  return energy;
}

unsigned int HexTopology::localMax(const std::vector<unsigned int>& ids,
				   const std::vector<float>& energy, 
				   unsigned int layer) {
  float        emax(0);
  unsigned int idmax(0);
  for (unsigned int k=0; k<ids.size(); ++k) {
    if (layer == ((ids[k]>>19)&0x7F)) {
      if (energy[k] > emax) {
	emax  = energy[k];
	idmax = ids[k];
      }
    }
  }
  return idmax;
}

