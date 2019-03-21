source /afs/ihep.ac.cn/users/w/wcdarec/wcda/env.sh
propath=/workfs/bes/lixr/match/trigger
inpath_big=/eos/lhaaso/experiment/wcda/decoded/2019/0302/
inroot_big=ES.10001.TimedFrag.20190302033320.032.root
inpath_sm=/eos/lhaaso/experiment/wcdaplus/root/
inroot_sm=ES.10012.TimedFrag.20190302033147.017.dat.root
${propath}/muonmatch $inpath_big $inroot_big $inpath_sm $inroot_sm
