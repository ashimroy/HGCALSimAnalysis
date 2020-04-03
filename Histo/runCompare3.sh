BaseDir=$PWD
BaseDir1=/afs/cern.ch/work/a/asroy/public/HGCalTB2018/Analysis/CMSSW_11_0_0_pre5/src/Histo/Archive_Data-MC_Histograms/v5_2019-11-23
BaseDir2=$PWD
BaseDir3=$PWD
Par=Pion # Ele/Pion
PhyList1=FTFP_BERT_EMN
PhyList2=QGSP_FTFP_BERT_EMN
NormalTag=30mBeamv3
Tagf1=Data
Tagf1V=v14  # i.e. v13 or ...
Tagf2=MC
Tagf2V=30mBeamv3  # i.e. FullBeam_v44 or v2_withMCP_EMN_Angle or ..
Tagf3=MC
Tagf3V=30mBeamv3  # i.e. FullBeam_v44 or v2_withMCP_EMN_Angle or ..


#BeamE=( 20 30 50 80 100 120 150 200 250 300 )
BeamE=( 50 80 100 120 150 200 300)
#BeamE=( 20 50 80 100 120 )
#BeamE=( 100 )
BESize=${#BeamE[@]}

#HistoName=(NLayer SumE E1Layer E7Layer E19Layer E1byE7 E1byE19 E7byE19 E7byEAll E19byEAll )
HistoName=(SumE E1Layer E7Layer E19Layer E1byE7 E1byE19 E7byE19 E7byEAll E19byEAll )
#HistoName=(E1Layer E7Layer E19Layer )
#HistoName=(SumE )
HNSize=${#HistoName[@]}

make clean
make

for (( iBE=0; iBE<${BESize}; iBE++ ))
do
    echo ======================== Beam Energy ${BeamE[$iBE]} GeV =======================

    File1=$BaseDir1/Histograms/$Tagf1/$Tagf1V/$Par/HGCTBHisto_${Tagf1}_${Par}E${BeamE[$iBE]}_${Tagf1V}.root
    File2=$BaseDir2/Histograms/$Tagf2/${Tagf2V}_${PhyList1}/$Par/HGCTBHisto_${Tagf2}_${Par}E${BeamE[$iBE]}_${Tagf2V}_${PhyList1}.root
    File3=$BaseDir3/Histograms/$Tagf3/${Tagf3V}_${PhyList2}/$Par/HGCTBHisto_${Tagf3}_${Par}E${BeamE[$iBE]}_${Tagf3V}_${PhyList2}.root

    for (( iHN=0; iHN<${HNSize}; iHN++ ))
    do

	echo "        ================== HistoName: ${HistoName[$iHN]} ================"
	
	echo "mkdir -p Plots/${NormalTag}/${Tagf1}_$Tagf1V-${Tagf2}_$Tagf2V-${Tagf3}_$Tagf3V/$Par/${BeamE[$iBE]}/${HistoName[$iHN]}"
	mkdir -p Plots/${NormalTag}/${Tagf1}_$Tagf1V-${Tagf2}_$Tagf2V-${Tagf3}_$Tagf3V/$Par/${BeamE[$iBE]}/${HistoName[$iHN]}/
	cp CompareHisto3.exe Plots/${NormalTag}/${Tagf1}_$Tagf1V-${Tagf2}_$Tagf2V-${Tagf3}_$Tagf3V/$Par/${BeamE[$iBE]}/${HistoName[$iHN]}/
	echo "cd Plots/${NormalTag}/${Tagf1}_$Tagf1V-${Tagf2}_$Tagf2V-${Tagf3}_$Tagf3V/$Par/${BeamE[$iBE]}/${HistoName[$iHN]}/"
	cd Plots/${NormalTag}/${Tagf1}_$Tagf1V-${Tagf2}_$Tagf2V-${Tagf3}_$Tagf3V/$Par/${BeamE[$iBE]}/${HistoName[$iHN]}/
#	echo "mkdir -p PDF/ PNG/ ROOT/"
#	mkdir -p PDF/ PNG/ ROOT/
	echo "Running... ./CompareHisto3.exe ${Tagf1}_$Tagf1V $File1 ${Tagf2}_$Tagf2V $File2 ${Tagf3}_$Tagf3V $File3 ${HistoName[$iHN]} $Par ${BeamE[$iBE]}"
	./CompareHisto3.exe ${Tagf1}_$Tagf1V $File1 ${Tagf2}_$Tagf2V $File2 ${Tagf3}_$Tagf3V $File3 ${HistoName[$iHN]} $Par ${BeamE[$iBE]} $NormalTag
#	mv *pdf PDF/
#	mv *png PNG/
#	mv *root ROOT/
	cp $BaseDir/index.php .
	cd $BaseDir
    done
done
