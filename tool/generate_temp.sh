#!/bin/bash

main_direc=$PWD/..
length=3000
width=1000
Particle='pion'
Pa='pi-'
Eincident=10 #GeV
Nevents=50 # number of events in each job
Njobs=30 # number of jobs
store_direc="${main_direc}/${Particle}_${Eincident}GeV_N${Nevents}_length${length}_width${width}"

if [ ! -d "${store_direc}" ]; then
   mkdir ${store_direc}
   echo "created ${store_direc}"
fi

cfg_name="${store_direc}/template.cfg"
source temp_template.sh $length $width  > ${cfg_name}
mac_name="${store_direc}/run.mac"
source temp_mac.sh $Pa $Eincident $Nevents  > ${mac_name}
jdl_name="${store_direc}/condor.jdl"
source temp_jdl.sh ${main_direc} ${store_direc} ${Particle}_${Eincident}GeV_N${Nevents} ${cfg_name} ${mac_name} ${Njobs} > ${jdl_name}
submit_name="${store_direc}/submit_condor.sh"
echo "cd ${main_direc}" > $submit_name
echo "condor_submit ${store_direc}/condor.jdl" >> $submit_name
#source ${submit_name}

