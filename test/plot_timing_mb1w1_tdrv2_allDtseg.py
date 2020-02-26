import ROOT as r
import sys
import os
r.gROOT.SetBatch(True)
r.gROOT.ProcessLine(".L /afs/cern.ch/user/b/brfranco/work/public/RPC/PlottingTemplate_L1T_TDR/PlotTemplate.C")
r.gStyle.SetOptStat(000000)
r.gStyle.SetOptTitle(0)

#filename_template = "results_timing_alsoQ9_PU_AGE_RPC_QUAL.root"
#filename_template = "results_timing_tightWindow_alsoQ9_PU_AGE_RPC_QUAL.root"


histo_name = 'hTimingOffSeg_AM_MB1_Wh.+1'
#histo_name = 'hTimingOffSeg_AM_MB2_Wh.-1'
#histo_name = 'hTimingGenMu_AM'
#histo_name = 'hdTimingOffSeg_AM'
#histo_name = 'hBXOffSeg_AM'

plot_dir = 'plots_mb1wh1_alsoq9_tightWindow_tdr_v2_allDtHits_test'
plotName_tpl = 'barrel_tp_time_resolution_alsoq9_' + histo_name
LogY = True
DisplayGrid = True
Lumi = "35.9"

files_to_run_on = ["191104_output_all_pu200_noage_norpc.root", "191104_output_all_pu200_noage_withrpc.root", "191104_output_all_pu200_age_norpc_youngseg_muonage_norpcage_nofail_3000.root", "191104_output_all_pu200_age_withrpc_youngseg_muonage_norpcage_fail_3000.root"]

logY = False 
pu = 'pu' 
qual = 'all'
firstTime = True
#legend = r.TLegend(0.14, 0.75, 0.40, 0.86)
legend = r.TLegend(0.14, 0.60, 0.421, 0.89)
#sigma_text = r.TPaveText(0.14, 0.75, 0.40, 0.86)
#sigma_text = r.TPaveText(0.64, 0.75, 0.90, 0.86)
plotName = plotName_tpl
#legend.SetTextSize(0.035)
if logY :
    plotName += "_" + pu +"_logY"
    myCanvas = r.CreateCanvas(plotName, logY, DisplayGrid)
else:
    plotName += "_" + pu 
    myCanvas = r.CreateCanvas(plotName, logY, DisplayGrid)
sigma_text = r.TLatex()
#sigma_text.SetTextSize(0.025);
sigma_text.SetTextSize(0.031);
#text_x_start_position = 0.64
text_x_start_position = 0.57
#text_y_start_position = 0.84
text_y_start_position = 0.81
y_step = 0.033
filenames = {}
rootfiles = {}
th1s = {}
for file_to_run_on in files_to_run_on:
        if file_to_run_on == '191104_output_all_pu200_noage_withrpc.root':
            age = 'noage'
            rpc = 'withrpc'
        if file_to_run_on == "191104_output_all_pu200_noage_norpc.root":
            age = 'noage'
            rpc = 'norpc'
        if file_to_run_on == "191104_output_all_pu200_age_norpc_youngseg_muonage_norpcage_nofail_3000.root":
            age = 'age'
            rpc = 'norpc'
        if file_to_run_on == "191104_output_all_pu200_age_withrpc_youngseg_muonage_norpcage_fail_3000.root":
            age = 'age'
            rpc = 'withrpc'
                #for qual in ',all,higherthanfour,higherthanfourvetoing'.split(','):
        filename = file_to_run_on
        print filename
        filenames[pu+age+rpc+qual] = filename
        rootfiles[pu+age+rpc+qual] = r.TFile(filename)
        th1s[pu+age+rpc+qual] = rootfiles[pu+age+rpc+qual].Get(histo_name)
        th1s[pu+age+rpc+qual].Scale(1/th1s[pu+age+rpc+qual].Integral())
        th1s[pu+age+rpc+qual].GetXaxis().SetTitle('Time of TPs associated to prompt muons [ns]')
        th1s[pu+age+rpc+qual].GetYaxis().SetTitle('Fraction of TPs')
        th1s[pu+age+rpc+qual].GetYaxis().SetTitleOffset(1.5)
        th1s[pu+age+rpc+qual].GetXaxis().SetRangeUser(-10, 10)
        th1s[pu+age+rpc+qual].SetLineWidth(3)
        #th1s[pu+age+rpc+qual].SetMarkerSize(1.3)
        #th1s[pu+age+rpc+qual].SetLineColor(0) # avoid seeing the line in the legend
        if not logY:
            th1s[pu+age+rpc+qual].GetYaxis().SetRangeUser(0.00000001, th1s[pu+age+rpc+qual].GetMaximum()*1.3)
        legend_string = "DT AM"
        if age == 'age':
            legend_string += " w/ ageing"
            if rpc == 'withrpc':
                th1s[pu+age+rpc+qual].SetLineColor(28)
                th1s[pu+age+rpc+qual].SetLineStyle(3)
                #legend_string += " w/ RPC w/ failures"
                #th1s[pu+age+rpc+qual].SetMarkerStyle(26)
                #th1s[pu+age+rpc+qual].SetMarkerColor(46)
            else:
                th1s[pu+age+rpc+qual].SetLineColor(31)
                th1s[pu+age+rpc+qual].SetLineStyle(2)
                #th1s[pu+age+rpc+qual].SetMarkerStyle(24)
                #th1s[pu+age+rpc+qual].SetMarkerColor(46)
        else:
            if rpc == 'withrpc':
                legend_string += " w/ RPC"
                #th1s[pu+age+rpc+qual].SetLineColor(4)
                th1s[pu+age+rpc+qual].SetLineStyle(1)
                #th1s[pu+age+rpc+qual].SetMarkerStyle(22)
                #th1s[pu+age+rpc+qual].SetMarkerColor(9)
            else: 
                th1s[pu+age+rpc+qual].SetLineColor(46)
                th1s[pu+age+rpc+qual].SetLineStyle(9)
                #th1s[pu+age+rpc+qual].SetMarkerStyle(20)
                #th1s[pu+age+rpc+qual].SetMarkerColor(9)

        if firstTime:
            th1s[pu+age+rpc+qual].Draw('HIST')
            firstTime = False
        else:
            th1s[pu+age+rpc+qual].Draw("HISTsame")
        legend.AddEntry(th1s[pu+age+rpc+qual], legend_string)
        if age == 'age' and rpc == 'withrpc' : legend.AddEntry(None,  "w/ RPC w/ failures", "")
        if 'hTimingOffSeg_AM' in histo_name:
            fit = th1s[pu+age+rpc+qual].Fit('gaus', 'SQ', '', -10, 10)
            #fit = th1s[pu+age+rpc+qual].Fit('gaus', 'SQ')
        else:
            fit = th1s[pu+age+rpc+qual].Fit('gaus', 'SQ')
        sigma = fit.Parameter(2)
        mean = fit.Parameter(1)
        print pu+age+rpc+qual, " ", mean
        #sigma_text.DrawLatexNDC(text_x_start_position, text_y_start_position, "#sigma_{%s} = %s"%(legend_string, round(sigma,1)))
        text_y_start_position -= y_step
        legend.AddEntry(None,  '#sigma = ' + str(round(sigma, 1)) + " ns", "")



        legend.Draw("")
        r.DrawPrelimLabel(myCanvas)
        r.DrawLumiLabel(myCanvas, Lumi)
        myCanvas.cd()
        #sigma_text.Draw("same")
        wheel_text = r.TLatex()
        wheel_text.SetTextSize(0.035)
        wheel_text.DrawLatexNDC(0.755, 0.87, "MB1 W+1")
        wheel_text.DrawLatexNDC(0.708, 0.83, "Single Muon")
        wheel_text.DrawLatexNDC(0.718, 0.79, "P_{T} > 20 GeV")
        #prelim_text = r.TLatex()
        #prelim_text.SetTextSize(0.03)
        ##prelim_text.DrawLatexNDC(0.14, 0.85, "Preliminary") #align with TLegend
        #prelim_text.DrawLatexNDC(0.208, 0.865, "Preliminary")
        r.SaveCanvas(myCanvas, os.path.join(plot_dir, plotName))
