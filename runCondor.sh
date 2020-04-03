#!/bin/bash

Par="Pion" ## Ele/Pion
#DataType="Data"
DataType="MC"
#PhyList="FTFP_BERT_EMN"
PhyList="QGSP_FTFP_BERT_EMN"
#TagMC="FullBeam_v44_FTFP_BERT_EMN"
TagMC="30mBeamv3_${PhyList}"
#TagData="v14"


#En=( 20 30 50 80 100 120 150 200 250 300 )
En=( 20 30 50 80 100 120 200 250 300 )
#En=( 120 250 )
#En=(100)

EnSize=${#En[@]}

if [ "$DataType" == "Data" ]
then 
    echo "running on Data..."
    echo "mkdir -p ${DataType}/${TagData}"
    mkdir -p ${DataType}/${TagData}
    TagDir="$TagData"
else 
    echo "running on MC..."
    echo "mkdir -p ${DataType}/${TagMC}"
    mkdir -p ${DataType}/${TagMC}
    TagDir="$TagMC"
fi

mkdir $DataType/$TagDir/$Par
cp HGCTB18_SIM.h HGCTB18_SIM.C HGCalTBTopology.C  main.C Makefile $DataType/$TagDir/$Par
cp *TEMPLATE* $DataType/$TagDir/$Par
cd $DataType/$TagDir/$Par


for (( iEn=0; iEn<${EnSize}; iEn++ ))
do
    echo ======================== Beam Energy ${En[$iEn]} GeV ====================

    sed s/'#Energy#'/${En[$iEn]}/ < EXEC_TEMPLATE.sh |
    sed s/'#DataType#'/${DataType}/ |
    sed s/'#PhyList#'/${PhyList}/ |
    sed s/'#Par#'/${Par}/ |
    sed s/'#TagData#'/${TagData}/ |
    sed s/'#TagMC#'/${TagMC}/ > exec_${Par}${En[$iEn]}GeV.sh
    echo created exec_${Par}${En[$iEn]}GeV.sh
    sed s/'#Energy#'/${En[$iEn]}/ < CONDOR_TEMPLATE.sub |
    sed s/'#Par#'/${Par}/ > condor_${Par}${En[$iEn]}GeV.sub
    echo created condor_${Par}${En[$iEn]}GeV.sub
    condor_submit condor_${Par}${En[$iEn]}GeV.sub
done
