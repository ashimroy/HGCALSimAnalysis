# HGCTBSimAnalysis

Requiremnt: ROOT

Instruction:

Since it is not depend on the CMSSW, any CMSSW release can be used to import ROOT.

cmsrel CMSSW_11_0_0_pre5

cd CMSSW_11_0_0_pre5/src

cmsenv

git clone https://github.com/ashimroy/HGCTBSimAnalysis.git

make


That's it. Ready to run.

How to run?
./HGCTBAna.exe InputFie OutputFile TotalEvent ReportEvery MIPCut

Example:

.//HGCTB18_SIM.exe /eos/cms/store/group/dpg_hgcal/tb_hgcal/simulation2018/HGCal_TB18_October_ConfigurationI/SinglePart_33mBeam/VtxGaus_FTFP_BERT_EMN_v3/pion_100GeV/TBGenSim_pi-_100GeV_All.root HGCTBSimOutput_MC_PiE100_33mBeam.root 10000 1000 1.0

Or,

./runSIM.sh  


Please change the input parameters in the run.sh according to your requirement and then run.  


For Condor Submission: (Submit jobs for all energy points)

./runCondor.sh 