#!/bin/sh

#define parameters which are passed in.
mac_p=$1
mac_e=$2
mac_n=$3
cat  << EOF
/gps/energy $mac_e GeV
/gps/particle $mac_p

/gps/pos/type Plane
/gps/pos/shape Square
/gps/pos/halfx 0.5 mm
/gps/pos/halfy 0.5 mm
/gps/pos/centre 0 0 -1501 mm
/gps/direction 0 0 1

/run/beamOn $mac_n
EOF
