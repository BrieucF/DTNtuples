import ROOT as r
from multiprocessing import Pool
import sys
r.gROOT.SetBatch(True)
r.gROOT.ProcessLine(".x loadTPGSimAnalysis_timing.C")

# in current situation there are 48 processes, 8 cores (6jobs) is a good compromise
if len(sys.argv) == 2: nCores = int(sys.argv[1])
else:                  nCores = 1


#path = "/pool/ciencias/userstorage/sscruz/DT_TDR_12sep/%s.root"
path = "/eos/cms/store/group/dpg_rpc/comm_rpc/Trigger/dt_phaseII_ntuples/190925_storeAllRPC/%s.root"

#def ExecuteTheThing(pu, age, rpc, qual, ind = -99, indmax = +99):
def ExecuteTheThing(tsk):
    pu, age, rpc, qual, ind, indmax = tsk
    inname  = '%s_%s_%s'%('nopu' if pu == 'nopu' else 'pu200', age, rpc)
    outname = "results_timing_%s_%s_%s_%s.root"%(pu, age, rpc, qual) * (ind == -99 and indmax == +99) + "results_timing_%s_%s_%s_%s_%s.root"%(pu, age, rpc, qual, "ind%i"%(ind) * (ind != -99) + "indmax%i"%(indmax) * (indmax != +99)) * (ind != -99 or indmax != +99)
    analysis = r.DTNtupleTPGSimAnalyzer(path%inname, outname, qual, ind, indmax)
    analysis.Loop()
    return


tasks = []
for pu in 'nopu,pu'.split(','):
    for age in 'age,noage'.split(','):
        for rpc in "withrpc,norpc".split(","):
            for qual in ['nothreehits']:
            #for qual in ',nothreehits,higherthanfour,higherthanfourvetoing'.split(','):
            #for qual in 'higherthanfour,higherthanfourvetoing'.split(','):
                tasks.append( (pu, age, rpc, qual, -99, +99) )
                if qual == "":
                    tasks.append( (pu, age, rpc, qual, 0, +99) )
                    tasks.append( (pu, age, rpc, qual, -99, 2) )

#tasks.append(("pu", "age", "withrpc", "nothreehits", -99, +99))

#tasks.append( ("nopu", "age", "withrpc", "higherthanfourvetoing", -99, +99) )

if nCores == 1:
    for task in tasks: ExecuteTheThing(task)
else:
    print "\n> Launching plotting processes..."
    pool = Pool(nCores)
    pool.map(ExecuteTheThing, tasks)
    pool.close()
    pool.join()
    del pool

