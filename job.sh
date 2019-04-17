#!/bin/bash
wkpath=/scratchfs/bes/lixr/tmp1/tes/subjob
for i in 0 100 200 300 400 500 600 700 800
do
echo "#!/bin/bash">${wkpath}/run_$i
source "/afs/ihep.ac.cn/users/w/wcdarec/wcda/env.sh">>${wkpath}/run_$i
echo "/scratchfs/bes/lixr/tmp1/tes/evtwcdacell $i">>${wkpath}/run_$i 
chmod +x ${wkpath}/run_$i
${wkpath}/run_$i >back_run_$i &
done
