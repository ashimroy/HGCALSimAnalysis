
InputFilePath=/eos/cms/store/group/dpg_hgcal/tb_hgcal/simulation2018/HGCal_TB18_October_ConfigurationI/SinglePart_33mBeam/VtxGaus_FTFP_BERT_EMN_v3/pion_100GeV/

InputFileName=TBGenSim_pi-_100GeV_All.root

OutputFile=HGCTBSimOutput_MC_PiE100_33mBeam.root

MaxEvent=-1
ReportEvery=1000
MIPCut=0.5

echo "Running... ./HGCTB18_SIM.exe $InputFilePath$InputFileName $OutputFile $MIPCut $MaxEvent $ReportEvery"
#./HGCTB18_SIM.exe $InputFilePath$InputFileName $OutputFile $MIPCut $MaxEvent $ReportEvery
