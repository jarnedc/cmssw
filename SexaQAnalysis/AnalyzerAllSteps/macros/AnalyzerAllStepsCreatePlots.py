from ROOT import TFile, TH1F, TH2F, TEfficiency


#fIn = TFile('/user/jdeclerc/Analysis/SexaQuark/CMSSW_9_4_7/src/SexaQAnalysis/AnalyzerAllSteps/test/wihtMatchingOnHits/test_TrackMatchingOnHits.root', 'read')
fIn = TFile('/user/jdeclerc/CMSSW_8_0_30_bis/CMSSW_8_0_30/src/SexaQAnalysis/AnalyzerAllSteps/test/wihtMatchingOnHits/test_TrackMatchingOnHits_withSexaq.root', 'read')

aNum = [
fIn.Get("AnalyzerAllSteps/TrackingEff/NonAntiS/RECO/h_NonAntiSTrack_RECO_pt"),
fIn.Get("AnalyzerAllSteps/TrackingEff/NonAntiS/RECO/h_NonAntiSTrack_RECO_pt_cut_eta_Lxy"),
fIn.Get("AnalyzerAllSteps/TrackingEff/NonAntiS/RECO/h_NonAntiSTrack_RECO_lxy_cut_pt_eta"),
fIn.Get("AnalyzerAllSteps/TrackingEff/NonAntiS/RECO/h_NonAntiSTrack_RECO_eta_cut_pt_lxy")
]


aDeNom = [
fIn.Get("AnalyzerAllSteps/TrackingEff/NonAntiS/All/h_NonAntiSTrack_All_pt"),
fIn.Get("AnalyzerAllSteps/TrackingEff/NonAntiS/All/h_NonAntiSTrack_All_pt_cut_eta_Lxy"),
fIn.Get("AnalyzerAllSteps/TrackingEff/NonAntiS/All/h_NonAntiSTrack_All_lxy_cut_pt_eta"),
fIn.Get("AnalyzerAllSteps/TrackingEff/NonAntiS/All/h_NonAntiSTrack_All_eta_cut_pt_lxy")
]

fOut = TFile('AnalyzerAllStepsPlots.root','RECREATE')

aTeff=[]
for i in range(0,len(aNum)):
	print i
	aTeff.append(TEfficiency(aNum[i],aDeNom[i]))	
	aTeff[i].Write()
	
#teff = TEfficiency(histNum,histDeNom)
#teff.Write()
fOut.Write()
fOut.Close()
