#!/bin/sh

#define parameters which are passed in.
cfg_l=$1
cfg_w=$2
cat  << EOF
####################
# general parameters
seed = -1
printModulo = 1
checkOverlaps = true
gps_instructions_file = gps.mac
physname = 1 # 1) FTFP_BERT_EMV; 2) QGSP_BERT
B_field_intensity = 0.    # in Tesla

####################
# set optic simu
switchOnScintillation = 0
propagateScintillation = 0
switchOnCerenkov = 0
propagateCerenkov = 0

####################
# set Ecal parameters
pointingAngle  = 0   # degree
world_material = 1   # 0) Vacuum; 1) Air
ecal_material  = 14     # 1) for quartz; 14) for PWO; 15) for Acrylic; 16) for copper; 17) BGO
ecal_length     = $cfg_l # mm
ecal_face       = $cfg_w # mm
EOF

