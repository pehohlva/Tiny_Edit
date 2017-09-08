#!/bin/bash
dir=$(pwd)
echo "$dir"
echo RUNNING COPY FILE ACTION RUNNING
echo RUNNING COPY FILE ACTION RUNNING
echo replace a file by google cld_generated_cjk_uni_prop_80.cc
rm ../google_bridge/cld2/internal/cld_generated_cjk_uni_prop_80.cc
rm ../google_bridge/cld2/internal/scoreonescriptspan.cc
cp ../google_bridge/cld_generated_cjk_uni_prop_80.cc  ../google_bridge/cld2/internal/cld_generated_cjk_uni_prop_80.cc
cp ../google_bridge/scoreonescriptspan.cc  ../google_bridge/cld2/internal/scoreonescriptspan.cc
echo end copy from $dir/cld_generated_cjk_uni_prop_80.cc
echo END COPY FILE ACTION RUNNING
echo END COPY FILE ACTION RUNNING
