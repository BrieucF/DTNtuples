#include "DTNtupleTPGSimAnalyzer.h"
#include "TVector2.h"
#include "TF1.h"

// AM
// 1,2 -> 3 hits
// 3,4 -> 4 hits
// 5 -> 3+2
// 6 -> 3+3
// 7 -> 4+2
// 8 -> 4+3
// 9 -> 4+4


DTNtupleTPGSimAnalyzer::DTNtupleTPGSimAnalyzer(const TString & inFileName,
                                               const TString & outFileName,
                                               const TString & quality = "",
                                               Int_t index = -99,
                                               Int_t maxindex = +99
                                              ):
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName), quality_(quality), index_(index), maxindex_(maxindex)
{

  m_minMuPt = 20;

  m_maxMuSegDPhi = 0.2;
  m_maxMuSegDEta = 0.3;

  m_minSegHits = 4;

  m_maxSegTrigDPhi = 0.005;
  m_maxMuTrigDPhi  = 0.2;

  //m_maxSegTrigDPhi = 8;
  //m_maxMuTrigDPhi  = 8;

}



DTNtupleTPGSimAnalyzer::~DTNtupleTPGSimAnalyzer()
{

}



void DTNtupleTPGSimAnalyzer::Loop()
{
  book();

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEvent(jentry);   nbytes += nb;

      if (jentry % 100 == 0)
  std::cout << "[DTNtupleTPGSimAnalyzer::Loop] processed : "
      << jentry << " entries\r" << std::flush;

      fill();
    }

  std::cout << std::endl;

  endJob();
}



void DTNtupleTPGSimAnalyzer::book()
{
  m_outFile.cd();

//   std::vector<std::string> algoTag  = {"HB",      "AM", "AM+RPC"};
  std::vector<std::string> algoTags  = {"HB", "AM"};
  std::vector<std::string> stationTags = {"MB1",     "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags    = { "Wh.-2", "Wh.-1", "Wh.0", "Wh.+1", "Wh.+2"};

  for (const auto & algo : algoTags)
  {
    for (const auto & station : stationTags)
    {
      for (const auto & wheel : whTags)
      {
      m_plots["TimingGenMu_" + algo + "_" + station + "_" + wheel] = new TH1D(("hTimingGenMu_" + algo + "_" + station + "_" + wheel).c_str(),
                                                ("Timing of TP associated to genMu pt > 20  for " + algo + "_" + station + "_" + wheel).c_str(),
                                                100, 400, 700);
      m_plots["BXGenMu_" + algo + "_" + station + "_" + wheel] = new TH1D(("hBXGenMu_" + algo + "_" + station + "_" + wheel).c_str(),
                                                ("BX of TP associated to genMu pt > 20 for " + algo + "_" + station + "_" + wheel).c_str(),
                                                17, 15, 32);
      m_plots["dTimingOffSeg_" + algo + "_" + station + "_" + wheel] = new TH1D(("hdTimingOffSeg_" + algo + "_" + station + "_" + wheel).c_str(),
                                                ("Offline t0 - TP t0 of TP associated to offline Seg for " + algo + "_" + station + "_" + wheel).c_str(),
                                                100, -50, 50);
      m_plots["dBXOffSeg_" + algo + "_" + station + "_" + wheel] = new TH1D(("hdBXOffSeg_" + algo + "_" + station + "_" + wheel).c_str(),
                                                ("Offline BX - TP BX of TP associated to offline Seg for " + algo + "_" + station + "_" + wheel).c_str(),
                                                6, -3, 3);
      m_plots["TimingOffSeg_" + algo + "_" + station + "_" + wheel] = new TH1D(("hTimingOffSeg_" + algo + "_" + station + "_" + wheel).c_str(),
                                                ("Timing of TP associated to offline Seg for " + algo + "_" + station + "_" + wheel).c_str(),
                                                40, 480, 520);
      m_plots["BXOffSeg_" + algo + "_" + station + "_" + wheel] = new TH1D(("hBXOffSeg_" + algo + "_" + station + "_" + wheel).c_str(),
                                                ("BX of TP associated to offline Seg for " + algo + "_" + station + "_" + wheel).c_str(),
                                                6, 17, 23);
      }
    }
    m_plots["TimingGenMu_" + algo ] = new TH1D(("hTimingGenMu_" + algo ).c_str(),
                                              ("Timing of TP associated to genMu pt > 20  for " + algo).c_str(), 100, 400, 700);
    m_plots["BXGenMu_" + algo ] = new TH1D(("hBXGenMu_" + algo ).c_str(),
                                              ("BX of TP associated to genMu pt > 20 for " + algo).c_str(), 17, 15, 32);
    m_plots["dPhi_GenMu_TP_" + algo ] = new TH1D(("dPhiGenMu_TP_" + algo ).c_str(),
                                                ("GenMu phi - TP phi of TP associated to genMu pt > 20 for " + algo).c_str(), 100, -0.2, 0.2);
    m_plots["dTimingOffSeg_" + algo ] = new TH1D(("hdTimingOffSeg_" + algo ).c_str(),
                                              ("Offline t0 - TP t0 of TP associated to offline Seg for " + algo).c_str(), 100, -50, 50);
    m_plots["dBXOffSeg_" + algo ] = new TH1D(("hdBXOffSeg_" + algo ).c_str(),
                                              ("Offline BX - TP BX of TP associated to offline Seg for " + algo).c_str(), 6, -3, 3);
    m_plots["TimingOffSeg_" + algo ] = new TH1D(("hTimingOffSeg_" + algo ).c_str(),
                                              ("Timing of TP associated to offline Seg for " + algo).c_str(), 40, 480, 520);
    m_plots["BXOffSeg_" + algo ] = new TH1D(("hBXOffSeg_" + algo ).c_str(),
                                                ("BX of TP associated to offline Seg for " + algo).c_str(), 6, 17, 23);
    m_plots["dPhi_Off_TP_" + algo ] = new TH1D(("dPhiOffSeg_TP_" + algo ).c_str(),
                                                ("Offline phi - TP phi of TP associated to offline Seg for " + algo).c_str(), 100, -0.02, 0.02);
  }
}



void DTNtupleTPGSimAnalyzer::fill()
{
  Int_t minQualityAM = -99;
  Int_t minQualityHB = -99;
  std::vector<Int_t> vetoedqualitiesAM; vetoedqualitiesAM.clear();
  if (quality_ == "nothreehits")
  {
    minQualityAM = 3;
    minQualityHB = 4;
  }
  else if (quality_ == "higherthanfour")
  {
    minQualityAM = 5;
    minQualityHB = 5;
  }
  else if (quality_ == "higherthanfourvetoing")
  {
    minQualityAM = 5;
    minQualityHB = 5;
    vetoedqualitiesAM.push_back(5);
    vetoedqualitiesAM.push_back(7);
  }
    
  std::vector<std::string> stationTags = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags    = { "Wh.-2", "Wh.-1", "Wh.0", "Wh.+1", "Wh.+2"};
  std::vector<std::string> secTags   = { "Sec.1", "Sec.2", "Sec.3", "Sec.4", "Sec.5", "Sec.6", "Sec.7", "Sec.8","Sec.9","Sec.10","Sec.11","Sec.12","Sec.13","Sec.14"};

  for (std::size_t iGenPart = 0; iGenPart < gen_nGenParts; ++iGenPart)
  {
    if (std::abs(gen_pdgId->at(iGenPart)) != 13 || gen_pt->at(iGenPart) < m_minMuPt) continue;

    // Match trig seg with gen mu and store t0 and bx
    for (auto stationTag = stationTags.begin(); stationTag != stationTags.end(); stationTag++) {
      for (auto whTag = whTags.begin(); whTag != whTags.end(); whTag++) {
          // ==================== VARIABLES FOR THE ANALYTICAL METHOD ALGORITHM
          Int_t bestTPAM = -1;
          Double_t bestMuTrigAMDPhi = 100000;
          Int_t    besttrigAMBX = -100000;
          Int_t    besttrigAMt0 = -100000;
          for (std::size_t iTrigAM = 0; iTrigAM < ph2TpgPhiEmuAm_nTrigs; ++iTrigAM)
          {
            Int_t trigAMWh  = ph2TpgPhiEmuAm_wheel->at(iTrigAM);
            Int_t trigAMSec = ph2TpgPhiEmuAm_sector->at(iTrigAM);
            Int_t trigAMSt  = ph2TpgPhiEmuAm_station->at(iTrigAM);
            Int_t trigAMBX  = ph2TpgPhiEmuAm_BX->at(iTrigAM);
            Int_t trigAMt0  = ph2TpgPhiEmuAm_t0->at(iTrigAM);
            Int_t trigAMRpcFlag  = ph2TpgPhiEmuAm_rpcFlag->at(iTrigAM);
            Int_t trigAMquality  = ph2TpgPhiEmuAm_quality->at(iTrigAM);
            if (trigAMRpcFlag == 5) continue; // skip all the RPC hit that were used elsewhere
            if (stationTags.at(trigAMSt - 1) == *stationTag && whTags.at(trigAMWh + 2) == *whTag) { // allow to have several match per genMuon (for different stations and wheels, no need for sector since best phi chosen

                Double_t trigAMGlbPhi = trigPhiInRad(ph2TpgPhiEmuAm_phi->at(iTrigAM), trigAMSec);
                Double_t muTrigAMDPhi = gen_phi->at(iGenPart) - trigAMGlbPhi;

                if (abs(muTrigAMDPhi) < m_maxMuTrigDPhi && abs(muTrigAMDPhi) < abs(bestMuTrigAMDPhi) && (trigAMquality >= minQualityAM || trigAMquality == -1 )) {
                    bestTPAM = iTrigAM;
                    bestMuTrigAMDPhi = muTrigAMDPhi;
                    besttrigAMt0 = trigAMt0;
                    besttrigAMBX = trigAMBX;
                }
            }
          }
          if (bestTPAM != -1) {
            m_plots["TimingGenMu_AM_" + *stationTag + "_" + *whTag]->Fill(besttrigAMt0);
            m_plots["BXGenMu_AM_" + *stationTag + "_" + *whTag]->Fill(besttrigAMBX);
            m_plots["TimingGenMu_AM"]->Fill(besttrigAMt0);
            m_plots["BXGenMu_AM"]->Fill(besttrigAMBX);
            m_plots["dPhi_GenMu_TP_AM"]->Fill(bestMuTrigAMDPhi);
          }
          // ==================== VARIABLES FOR THE HISTOGRAM BASED ALGORITHM
          Int_t bestTPHB = -1;
          Double_t bestMuTrigHBDPhi = 100000;
          Int_t    besttrigHBBX = -100000;
          Int_t    besttrigHBt0 = -100000;
          for (std::size_t iTrigHB = 0; iTrigHB < ph2TpgPhiEmuHb_nTrigs; ++iTrigHB)
          {
            Int_t trigHBWh  = ph2TpgPhiEmuHb_wheel->at(iTrigHB);
            Int_t trigHBSec = ph2TpgPhiEmuHb_sector->at(iTrigHB);
            Int_t trigHBSt  = ph2TpgPhiEmuHb_station->at(iTrigHB);
            Int_t trigHBBX  = ph2TpgPhiEmuHb_BX->at(iTrigHB);
            Int_t trigHBt0  = ph2TpgPhiEmuHb_t0->at(iTrigHB);
            Int_t trigHBRpcFlag  = ph2TpgPhiEmuHb_rpcFlag->at(iTrigHB);
            Int_t trigHBRquality  = ph2TpgPhiEmuHb_quality->at(iTrigHB);
            if (trigHBRpcFlag == 5) continue; // skip all the RPC hit that were used elsewhere
            if (stationTags.at(trigHBSt - 1) == *stationTag && whTags.at(trigHBWh + 2) == *whTag){ // allow to have several match per genMuon (for different stations and wheels, no need for sector since best phi chosen

                Double_t trigHBGlbPhi = trigPhiInRad(ph2TpgPhiEmuHb_phi->at(iTrigHB), trigHBSec);
                Double_t muTrigHBDPhi = gen_phi->at(iGenPart) - trigHBGlbPhi;

                if (abs(muTrigHBDPhi) < m_maxMuTrigDPhi && abs(muTrigHBDPhi) < abs(bestMuTrigHBDPhi) && (trigHBRquality >= minQualityHB || trigHBRquality == -1)) {
                    bestTPHB = iTrigHB;
                    bestMuTrigHBDPhi = muTrigHBDPhi;
                    besttrigHBt0 = trigHBt0;
                    besttrigHBBX = trigHBBX;
                }
            }
          }
          if (bestTPHB != -1) {
            m_plots["TimingGenMu_HB_" + *stationTag + "_" + *whTag]->Fill(besttrigHBt0);
            m_plots["BXGenMu_HB_" + *stationTag + "_" + *whTag]->Fill(besttrigHBBX);
            m_plots["TimingGenMu_HB"]->Fill(besttrigHBt0);
            m_plots["BXGenMu_HB"]->Fill(besttrigHBBX);
            m_plots["dPhi_GenMu_TP_HB"]->Fill(bestMuTrigHBDPhi);
          }
      }
    }

    // find the best offline segments
    // CB this should not be a vector ...
    std::vector<std::size_t> bestSegIndex = { 999, 999, 999, 999 };
    std::vector<Int_t> bestSegNHits       = { 0, 0, 0, 0 }; // One segment per station allowed


    for (std::size_t iSeg = 0; iSeg < seg_nSegments; ++iSeg)
    {
      Int_t segSt    = seg_station->at(iSeg);
      Int_t segNHits = seg_phi_nHits->at(iSeg);
      
      Double_t muSegDPhi = std::abs(acos(cos(gen_phi->at(iGenPart) - seg_posGlb_phi->at(iSeg))));
      Double_t muSegDEta = std::abs(gen_eta->at(iGenPart) - seg_posGlb_eta->at(iSeg));
      
      if (muSegDPhi < m_maxMuSegDPhi &&
          muSegDEta < m_maxMuSegDEta &&
          segNHits >= m_minSegHits &&
          segNHits >= bestSegNHits.at(segSt - 1))
      {
        bestSegNHits[segSt - 1] = segNHits;
        bestSegIndex[segSt - 1] = iSeg;
      }
    }


    // match all offline segment with TP's and store t0, BX and the difference w.r.t. offline
    for (const auto & iSeg : bestSegIndex)
    {
      if (iSeg == 999) continue;

      Int_t segWh  = seg_wheel->at(iSeg);
      Int_t segSec = seg_sector->at(iSeg);
      if (segSec == 13) segSec = 4;
      if (segSec == 14) segSec = 10;
      Int_t segSt  = seg_station->at(iSeg);
      
      std::string chambTag = stationTags.at(segSt - 1);
      std::string whTag    = whTags.at(segWh + 2);
      std::string secTag   = secTags.at(segSec - 1);

      // ==================== VARIABLES FOR THE HOUGH TRANSFORM BASED ALGORITHM
      Int_t    bestTPHB = -1;
      Double_t bestSegTrigHBDPhi = 1000;
      Double_t bestHBDPhi = 0;
      Int_t    besttrigHBBX = 0;
      Int_t    besttrigHBt0 = 0;
      Int_t    besttrigHBdBX = 0;
      Int_t    besttrigHBdt0 = 0;
      for (std::size_t iTrigHB = 0; iTrigHB < ph2TpgPhiEmuHb_nTrigs; ++iTrigHB)
      {
        Int_t trigHBWh  = ph2TpgPhiEmuHb_wheel->at(iTrigHB);
        Int_t trigHBSec = ph2TpgPhiEmuHb_sector->at(iTrigHB);
        Int_t trigHBSt  = ph2TpgPhiEmuHb_station->at(iTrigHB);
        Int_t trigHBBX  = ph2TpgPhiEmuHb_BX->at(iTrigHB);
        Int_t trigHBt0  = ph2TpgPhiEmuHb_t0->at(iTrigHB);
        Int_t trigHBRpcFlag  = ph2TpgPhiEmuHb_rpcFlag->at(iTrigHB);
        if (trigHBRpcFlag == 5) continue; // skip all the RPC hit that were used elsewhere
        Int_t trigHBdBX  = ph2TpgPhiEmuHb_BX->at(iTrigHB) - round((seg_phi_t0->at(iSeg) + 12.5)/25.);
        Int_t trigHBdt0  = ph2TpgPhiEmuHb_t0->at(iTrigHB) - (seg_phi_t0->at(iSeg) + 20*25);

        if (segWh  == trigHBWh && segSec == trigHBSec &&  segSt  == trigHBSt)
        {
          Double_t trigGlbPhi    = trigPhiInRad(ph2TpgPhiEmuHb_phi->at(iTrigHB),trigHBSec);
          Double_t finalHBDPhi   = seg_posGlb_phi->at(iSeg) - trigGlbPhi;
          Double_t segTrigHBDPhi = finalHBDPhi;

          //if ((abs(segTrigHBDPhi) < m_maxSegTrigDPhi) && (abs(bestSegTrigHBDPhi) > abs(segTrigHBDPhi)) && (ph2TpgPhiEmuHb_quality->at(iTrigHB) >= minQualityHB) && (((ph2TpgPhiEmuHb_index->at(iTrigHB) < maxindex_) && (index_ < 0)) || ((ph2TpgPhiEmuHb_index->at(iTrigHB) == index_) && (index_ >= 0))) )
          if ((abs(segTrigHBDPhi) < m_maxSegTrigDPhi) && (abs(bestSegTrigHBDPhi) > abs(segTrigHBDPhi)) && (ph2TpgPhiEmuHb_quality->at(iTrigHB) >= minQualityHB || ph2TpgPhiEmuHb_quality->at(iTrigHB) == -1) )
          {
            bestTPHB          = iTrigHB;
            besttrigHBBX      = trigHBBX;
            besttrigHBt0      = trigHBt0;
            besttrigHBdBX      = trigHBdBX;
            besttrigHBdt0      = trigHBdt0;
            bestSegTrigHBDPhi = segTrigHBDPhi;
            bestHBDPhi        = TVector2::Phi_mpi_pi(finalHBDPhi);
          }
        }
      }

      if (bestTPHB > -1 && seg_phi_t0->at(iSeg) > -500)
      {
        m_plots["TimingOffSeg_HB_" + stationTags.at(segSt - 1) + "_" + whTags.at(segWh + 2)]->Fill(besttrigHBt0);
        m_plots["dTimingOffSeg_HB_" + stationTags.at(segSt - 1) + "_" + whTags.at(segWh + 2)]->Fill(besttrigHBdt0);
        m_plots["BXOffSeg_HB_" + stationTags.at(segSt - 1) + "_" + whTags.at(segWh + 2)]->Fill(besttrigHBBX);
        m_plots["dBXOffSeg_HB_" + stationTags.at(segSt - 1) + "_" + whTags.at(segWh + 2)]->Fill(besttrigHBdBX);
        m_plots["TimingOffSeg_HB"]->Fill(besttrigHBt0);
        m_plots["dTimingOffSeg_HB"]->Fill(besttrigHBdt0);
        m_plots["BXOffSeg_HB"]->Fill(besttrigHBBX);
        m_plots["dBXOffSeg_HB"]->Fill(besttrigHBdBX);
        m_plots["dPhi_Off_TP_HB"]->Fill(bestSegTrigHBDPhi);
      }

      // ==================== VARIABLES FOR THE ANALYTICAL METHOD ALGORITHM
      Int_t    bestTPAM = -1;
      Double_t bestSegTrigAMDPhi = 1000;
      Double_t bestAMDPhi = 0;
      Int_t    besttrigAMBX = 0;
      Int_t    besttrigAMt0 = 0;
      Int_t    besttrigAMdBX = 0;
      Int_t    besttrigAMdt0 = 0;
      for (std::size_t iTrigAM = 0; iTrigAM < ph2TpgPhiEmuAm_nTrigs; ++iTrigAM)
      {
        Int_t trigAMWh  = ph2TpgPhiEmuAm_wheel->at(iTrigAM);
        Int_t trigAMSec = ph2TpgPhiEmuAm_sector->at(iTrigAM);
        Int_t trigAMSt  = ph2TpgPhiEmuAm_station->at(iTrigAM);
        Int_t trigAMBX  = ph2TpgPhiEmuAm_BX->at(iTrigAM);
        Int_t trigAMt0  = ph2TpgPhiEmuAm_t0->at(iTrigAM);
        Int_t trigAMRpcFlag  = ph2TpgPhiEmuAm_rpcFlag->at(iTrigAM);
        if (trigAMRpcFlag == 5) continue; // skip all the RPC hit that were used elsewhere
        Int_t trigAMdBX  = ph2TpgPhiEmuAm_BX->at(iTrigAM) - round((seg_phi_t0->at(iSeg) + 12.5)/25.);
        Int_t trigAMdt0  = ph2TpgPhiEmuAm_t0->at(iTrigAM) - (seg_phi_t0->at(iSeg) + 20*25);

        if (segWh  == trigAMWh && segSec == trigAMSec &&  segSt  == trigAMSt)
        {
          Double_t trigGlbPhi    = trigPhiInRad(ph2TpgPhiEmuAm_phi->at(iTrigAM),trigAMSec);
          Double_t finalAMDPhi   = seg_posGlb_phi->at(iSeg) - trigGlbPhi;
          Double_t segTrigAMDPhi = finalAMDPhi;

          //if ((abs(segTrigAMDPhi) < m_maxSegTrigDPhi) && (abs(bestSegTrigAMDPhi) > abs(segTrigAMDPhi)) && (ph2TpgPhiEmuAm_quality->at(iTrigAM) >= minQualityAM) && (((ph2TpgPhiEmuAm_index->at(iTrigAM) < maxindex_) && (index_ < 0)) || ((ph2TpgPhiEmuAm_index->at(iTrigAM) == index_) && (index_ >= 0))) )
          if ((abs(segTrigAMDPhi) < m_maxSegTrigDPhi) && (abs(bestSegTrigAMDPhi) > abs(segTrigAMDPhi)) && (ph2TpgPhiEmuAm_quality->at(iTrigAM) >= minQualityAM || ph2TpgPhiEmuAm_quality->at(iTrigAM) == -1))
          {
            bestTPAM          = iTrigAM;
            besttrigAMBX      = trigAMBX;
            besttrigAMt0      = trigAMt0;
            besttrigAMdBX      = trigAMdBX;
            besttrigAMdt0      = trigAMdt0;
            bestSegTrigAMDPhi = segTrigAMDPhi;
            bestAMDPhi        = TVector2::Phi_mpi_pi(finalAMDPhi);
          }
        }
      }

      if (bestTPAM > -1 && seg_phi_t0->at(iSeg) > -500)
      {
        m_plots["TimingOffSeg_AM_" + stationTags.at(segSt - 1) + "_" + whTags.at(segWh + 2)]->Fill(besttrigAMt0);
        m_plots["dTimingOffSeg_AM_" + stationTags.at(segSt - 1) + "_" + whTags.at(segWh + 2)]->Fill(besttrigAMdt0);
        m_plots["BXOffSeg_AM_" + stationTags.at(segSt - 1) + "_" + whTags.at(segWh + 2)]->Fill(besttrigAMBX);
        m_plots["dBXOffSeg_AM_" + stationTags.at(segSt - 1) + "_" + whTags.at(segWh + 2)]->Fill(besttrigAMdBX);
        m_plots["TimingOffSeg_AM"]->Fill(besttrigAMt0);
        m_plots["dTimingOffSeg_AM"]->Fill(besttrigAMdt0);
        m_plots["BXOffSeg_AM"]->Fill(besttrigAMBX);
        m_plots["dBXOffSeg_AM"]->Fill(besttrigAMdBX);
        m_plots["dPhi_Off_TP_AM"]->Fill(bestSegTrigAMDPhi);
      }
    }
  }
}



void DTNtupleTPGSimAnalyzer::endJob()
{
  m_outFile.cd();

  m_outFile.Write();
  m_outFile.Close();
}



Double_t DTNtupleTPGSimAnalyzer::trigPhiInRad(Double_t trigPhi, Int_t sector)
{
  return trigPhi / 65536. * 0.8 + TMath::Pi() / 6 * (sector - 1);
}
