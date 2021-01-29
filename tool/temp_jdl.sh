#!/bin/sh

#define parameters which are passed in.
jdl_main=$1
jdl_store=$2
jdl_name=$3
jdl_cfg=$4
jdl_mac=$5
jdl_njob=$6
cat  << EOF
universe = vanilla
getenv=True
Executable = ${jdl_main}/Calo 
should_transfer_files = NO
Requirements = TARGET.FileSystemDomain == "privnet"
Output = ${jdl_store}/${jdl_name}_\$(ProcId).out
Error =  ${jdl_store}/${jdl_name}_\$(ProcId).err
Log =    ${jdl_store}/${jdl_name}_\$(ProcId).log
Arguments = -c ${jdl_cfg} -m ${jdl_mac} -o ${jdl_store}/${jdl_name}_\$(ProcId)
Queue ${jdl_njob}
EOF
