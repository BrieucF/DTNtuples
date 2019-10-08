import ROOT as r
import sys
import os
r.gROOT.SetBatch(True)
r.gROOT.ProcessLine(".L /afs/cern.ch/user/b/brfranco/work/public/RPC/PlottingTemplate_L1T_TDR/PlotTemplate.C")
r.gStyle.SetOptStat(000000)
r.gStyle.SetOptTitle(0)

filename_template = "results_timing_PU_AGE_RPC_QUAL.root"

histo_name = 'hTimingOffSeg_AM'
#histo_name = 'hTimingOffSeg_AM_MB2_Wh.-1'
#histo_name = 'hTimingGenMu_AM'
#histo_name = 'hdTimingOffSeg_AM'
#histo_name = 'hBXOffSeg_AM'

plot_dir = 'plots'
plotName_tpl = 'barrel_tp_time_resolution_' + histo_name
LogY = True
DisplayGrid = True
Lumi = "35.9"


for logY in [True, False]:
    for pu in 'nopu,pu'.split(','):
        #legend = r.TLegend(0.14, 0.75, 0.40, 0.86)
        legend = r.TLegend(0.14, 0.72, 0.47, 0.86)
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
        sigma_text.SetTextSize(0.035);
        #text_x_start_position = 0.64
        text_x_start_position = 0.59
        #text_y_start_position = 0.84
        text_y_start_position = 0.835
        y_step = 0.033
        filenames = {}
        rootfiles = {}
        th1s = {}
        firstTime = True
        for age in 'noage,age'.split(','):
            for rpc in "norpc,withrpc".split(","):
                for qual in ['nothreehits']:
                #for qual in ',nothreehits,higherthanfour,higherthanfourvetoing'.split(','):
                    filename = filename_template.replace("RPC", rpc).replace("QUAL", qual).replace("PU", pu).replace("AGE", age)
                    print filename
                    filenames[pu+age+rpc+qual] = filename
                    rootfiles[pu+age+rpc+qual] = r.TFile(filename)
                    th1s[pu+age+rpc+qual] = rootfiles[pu+age+rpc+qual].Get(histo_name)
                    th1s[pu+age+rpc+qual].Scale(1/th1s[pu+age+rpc+qual].Integral())
                    th1s[pu+age+rpc+qual].GetXaxis().SetTitle('Time of TP\'s associated to prompt muons [ns]')
                    th1s[pu+age+rpc+qual].GetYaxis().SetTitle('Arbitrary units')
                    th1s[pu+age+rpc+qual].GetYaxis().SetTitleOffset(1.5)
                    if not logY:
                        th1s[pu+age+rpc+qual].GetYaxis().SetRangeUser(0.00000001, th1s[pu+age+rpc+qual].GetMaximum()*1.6)
                    legend_string = "AM"
                    if rpc == 'withrpc':
                        legend_string += " w/ RPC"
                        if age == 'age':
                            th1s[pu+age+rpc+qual].SetMarkerStyle(28)
                            th1s[pu+age+rpc+qual].SetMarkerColor(2)
                            legend_string += " w/ aging"
                        else:
                            th1s[pu+age+rpc+qual].SetMarkerStyle(34)
                            th1s[pu+age+rpc+qual].SetMarkerColor(4)
                    else: 
                        if age == 'age':
                            th1s[pu+age+rpc+qual].SetMarkerStyle(24)
                            th1s[pu+age+rpc+qual].SetMarkerColor(2)
                            legend_string += " w/ aging"
                        else:
                            th1s[pu+age+rpc+qual].SetMarkerStyle(20)
                            th1s[pu+age+rpc+qual].SetMarkerColor(4)

                    if firstTime:
                        th1s[pu+age+rpc+qual].Draw('HISTP')
                        firstTime = False
                    else:
                        th1s[pu+age+rpc+qual].Draw("HISTPsame")
                    legend.AddEntry(th1s[pu+age+rpc+qual], legend_string)
                    if 'hTimingOffSeg_AM' in histo_name:
                        #fit = th1s[pu+age+rpc+qual].Fit('gaus', 'SQ', '', 490, 510)
                        fit = th1s[pu+age+rpc+qual].Fit('gaus', 'SQ')
                    else:
                        fit = th1s[pu+age+rpc+qual].Fit('gaus', 'SQ')
                    sigma = fit.Parameter(2)
                    sigma_text.DrawLatexNDC(text_x_start_position, text_y_start_position, "\sigma_{%s} = %s"%(legend_string, round(sigma,1)))
                    text_y_start_position -= y_step

                    #sigma_text.AddText("\sigma_{%s} = %s"%(legend_string, sigma))


        legend.Draw("HISTP")
        r.DrawPrelimLabel(myCanvas)
        r.DrawLumiLabel(myCanvas, Lumi);
        myCanvas.cd()
        sigma_text.Draw("same")
        r.SaveCanvas(myCanvas, os.path.join(plot_dir, plotName));
