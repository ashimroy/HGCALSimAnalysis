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


class HGCalTestNumbering {
public:
  static const int kHGCalCellSOffset = 0;
  static const int kHGCalCellSMask = 0xFFFF;
  static const int kHGCalSectorSOffset = 16;
  static const int kHGCalSectorSMask = 0x7F;
  static const int kHGCalSubSectorSOffset = 23;
  static const int kHGCalSubSectorSMask = 0x1;
  static const int kHGCalLayerSOffset = 24;
  static const int kHGCalLayerSMask = 0x7F;
  static const int kHGCalZsideSOffset = 31;
  static const int kHGCalZsideSMask = 0x1;

  static const int kHGCalCellHOffset = 0;
  static const int kHGCalCellHMask = 0xFF;
  static const int kHGCalCellTypHOffset = 8;
  static const int kHGCalCellTypHMask = 0x1;
  static const int kHGCalWaferHOffset = 9;
  static const int kHGCalWaferHMask = 0x3FF;
  static const int kHGCalLayerHOffset = 19;
  static const int kHGCalLayerHMask = 0x7F;
  static const int kHGCalZsideHOffset = 26;
  static const int kHGCalZsideHMask = 0x1;
  static const int kHGCalSubdetHOffset = 27;
  static const int kHGCalSubdetHMask = 0x7;
  HGCalTestNumbering() {}
  virtual ~HGCalTestNumbering() {}
  static uint32_t packSquareIndex(int z, int lay, int sec, int subsec, int cell);
  static uint32_t packHexagonIndex(int subdet, int z, int lay, int wafer, int celltyp, int cell);
  static void unpackSquareIndex(const uint32_t& idx, int& z, int& lay, int& sec, int& subsec, int& cell);
  static void unpackHexagonIndex(
				 const uint32_t& idx, int& subdet, int& z, int& lay, int& wafer, int& celltyp, int& cell);
  static bool isValidSquare(int z, int lay, int sec, int subsec, int cell);
  static bool isValidHexagon(int subdet, int z, int lay, int wafer, int celltyp, int cell);
};
