from ROOT import TFile, TH1F, TH2F, TEfficiency


#fIn = TFile('/user/jdeclerc/Analysis/SexaQuark/CMSSW_9_4_7/src/SexaQAnalysis/AnalyzerAllSteps/test/wihtMatchingOnHits/test_TrackMatchingOnHits.root', 'read')
fIn = TFile('/user/jdeclerc/CMSSW_8_0_30/src/SexaQAnalysis/AnalyzerAllSteps/test/wihtMatchingOnHits/test_TrackMatchingOnHits_withSexaq_Step2.root', 'read')
fOut = TFile('AnalyzerAllStepsPlots.root','RECREATE')

#add to this list all the num and denom plots of which you want to make an efficiency plot
aNumAndDenomForEff = []
standard_parameters = ["pt","eta","phi","lxy","vz","dxy"]
#Non AntiS tracking efficiency:
for parameter in ["pt","eta","phi","lxy","vz","dxy","pt_cut_eta_Lxy","lxy_cut_pt_eta","eta_cut_pt_lxy"]:
	aNumAndDenomForEff.append([fIn.Get("AnalyzerAllSteps/TrackingEff/NonAntiSTracks/RECO/h_NonAntiSTrack_RECO_"+parameter),fIn.Get("AnalyzerAllSteps/TrackingEff/NonAntiSTracks/All/h_NonAntiSTrack_All_"+parameter)])
#daughters from AntiS-Ks tracking efficiency, only for charged granddaughters
for parameter in standard_parameters:
	aNumAndDenomForEff.append([fIn.Get("AnalyzerAllSteps/TrackingEff/AntiSKsDaughterTracks/RECO/h_AntiSKsDaughterTracks_RECO_"+parameter),fIn.Get("AnalyzerAllSteps/TrackingEff/AntiSKsDaughterTracks/All/h_AntiSKsDaughterTracks_All_"+parameter)])

#AntiProton daughters from AntiS-AntiL tracking efficiency, only for charged granddaughters
for parameter in standard_parameters:
	aNumAndDenomForEff.append([fIn.Get("AnalyzerAllSteps/TrackingEff/AntiSAntiLambdaAntiProtonTracks/RECO/h_AntiSAntiLAntiProtonDaughterTracks_RECO_"+parameter),fIn.Get("AnalyzerAllSteps/TrackingEff/AntiSAntiLambdaAntiProtonTracks/All/h_AntiSAntiLAntiProtonDaughterTracks_All_"+parameter)])

#PosPion daughters from AntiS-AntiL tracking efficiency, only for charged granddaughters
for parameter in standard_parameters:
	aNumAndDenomForEff.append([fIn.Get("AnalyzerAllSteps/TrackingEff/AntiSAntiLambdaPosPionTracks/RECO/h_AntiSAntiLPosPionDaughterTracks_RECO_"+parameter),fIn.Get("AnalyzerAllSteps/TrackingEff/AntiSAntiLambdaPosPionTracks/All/h_AntiSAntiLPosPionDaughterTracks_All_"+parameter)])



aTeff=[]
for plots in aNumAndDenomForEff:
	aTeff.append(TEfficiency(plots[0],plots[1]))	
for teff in aTeff:
	teff.Write()
	
#teff = TEfficiency(histNum,histDeNom)
#teff.Write()
fOut.Write()
fOut.Close()
