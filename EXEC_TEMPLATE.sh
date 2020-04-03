#!/bin/bash                                                                                                              
Object=pion
InputFilePath=/eos/cms/store/group/dpg_hgcal/tb_hgcal/simulation2018/HGCal_TB18_October_ConfigurationI/SinglePart_33mBeam/VtxGaus_#PhyList#_v3/${Object}_#Energy#GeV/
InputFileName=TBGenSim_#Par##Energy#GeV_All.root
OutputFile=HGCTBSimOutput_#DataType#_#Par#E#Energy#_#TagMC#.root

MaxEvent=-1
ReportEvery=1000
MIPCut=0.5
                                                  
echo "inside jobsubmit"
pwd
#cd /afs/cern.ch/work/a/asroy/public/HGCalTB2018/AnalysisFromSimHit/CMSSW_11_0_0_pre11/src
cd $CMSSW_BASE/src
pwd
source /cvmfs/cms.cern.ch/cmsset_default.sh
echo "sourced"
cmsenv
cd ${_CONDOR_SCRATCH_DIR}
pwd
hostname
make
echo "Running... ./HGCTB18_SIM.exe $InputFilePath$InputFileName $OutputFile $MIPCut $MaxEvent $ReportEvery"
./HGCTB18_SIM.exe $InputFilePath$InputFileName $OutputFile $MIPCut $MaxEvent $ReportEvery $MIPCut
