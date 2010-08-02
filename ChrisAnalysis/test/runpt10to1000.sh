
    
    i=100
#    for i in 50 150 200 240 
#    do
    

    sed "s/QQQQQQ/$i/"<$CMSSW_BASE/src/FastSimulation/Configuration/test/SingleGammaPtQQQQQQ_cfg.py>$CMSSW_BASE/src/FastSimulation/Configuration/test/SingleGammaPt${i}_cfg.py
    sed "s/QQQQQQ/$i/"<$CMSSW_BASE/src/PhysicsTools/PatAlgos/test/SingleGammaPtQQQQQQ_PF2PAT_cfg.py>$CMSSW_BASE/src/PhysicsTools/PatAlgos/test/SingleGammaPt${i}_PF2PAT_cfg.py
    sed "s/QQQQQQ/$i/"<$CMSSW_BASE/src/PFAnalyses/ChrisAnalysis/test/ChrisQQQQQQ_cfg.py>$CMSSW_BASE/src/PFAnalyses/ChrisAnalysis/test/Chris${i}_cfg.py
    echo "created configs now running fastsim for pt $i"
    cmsRun $CMSSW_BASE/src/FastSimulation/Configuration/test/SingleGammaPt${i}_cfg.py
    echo "Ran FastSim, now running pf2pat for pt $i"
    cmsRun $CMSSW_BASE/src/PhysicsTools/PatAlgos/test/SingleGammaPt${i}_PF2PAT_cfg.py
    echo "Ran pf2pat now chrisanalyzing for pt $i"
    chrisAnalysis $CMSSW_BASE/src/PFAnalyses/ChrisAnalysis/test/Chris${i}_cfg.py
    echo "Finished with analysis for pt $i"
    rm $CMSSW_BASE/src/FastSimulation/Configuration/test/SingleGammaPt${i}_cfg.py
    rm $CMSSW_BASE/src/FastSimulation/Configuration/test/SingleGammaPt${i}.root
    rm $CMSSW_BASE/src/PhysicsTools/PatAlgos/test/SingleGammaPt${i}_PF2PAT_cfg.py
    rm $CMSSW_BASE/src/PhysicsTools/PatAlgos/test/SingleGammaPt${i}_PF2PAT.root
    rm $CMSSW_BASE/src/PFAnalyses/ChrisAnalysis/test/Chris${i}_cfg.py
    
    echo "removed all files for pt $i"
    
    


    

#    done
