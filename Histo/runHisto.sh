#BaseDir=/afs/cern.ch/work/a/asroy/public/HGCalTB2018/Analysis/CMSSW_11_0_0_pre5/src/
BaseDir=$CMSSW_BASE/src
Par=Pion # Ele/Pion
DataType=MC  # Data/MC
#DataType=Data  # Data/MC
#PhyList=FTFP_BERT_EMN
PhyList=QGSP_FTFP_BERT_EMN
Tag=30mBeamv3 # i.e. v14/ FullBeam_v44
InTag=${Tag}_$PhyList 
OutTag=${Tag}_$PhyList 

MaxEvent=-1
ReportEvery=10000

cmsenv
make clean
make
InputFilePath=${BaseDir}/${DataType}/${InTag}/${Par}/

#BeamE=100
#BeamE=( 20 30 50 80 100 120 150 200 250 300)
BeamE=( 20 30 50 80 100 120 200 250 300)
#BeamE=( 20 30 50 80 100 120 200)
#BeamE=( 100 )
BESize=${#BeamE[@]}

echo "mkdir -p Histograms/$DataType/$OutTag/$Par"
mkdir -p Histograms/$DataType/$OutTag/$Par
echo "cp HGCTBHisto.exe Histograms/$DataType/$OutTag/$Par/"
cp HGCTBHisto.exe Histograms/$DataType/$OutTag/$Par/
echo "cd Histograms/$DataType/$OutTag/$Par"
cd Histograms/$DataType/$OutTag/$Par

for (( iBE=0; iBE<${BESize}; iBE++ ))
do
    echo ======================== Beam BEergy ${BeamE[$iBE]} GeV ====================

    InputFileName=HGCTBSimOutput_${DataType}_${Par}E${BeamE[$iBE]}_${InTag}.root
    OutputFile=HGCTBHisto_${DataType}_${Par}E${BeamE[$iBE]}_${OutTag}.root
    
    echo "Running... ./HGCTBHisto.exe $InputFilePath$InputFileName $OutputFile $MaxEvent $ReportEvery ${BeamE[$iBE]}"
    ./HGCTBHisto.exe $InputFilePath$InputFileName $OutputFile $MaxEvent $ReportEvery ${BeamE[$iBE]}
done
