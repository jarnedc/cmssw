from ROOT import TFile, TH1F, TH2F, TEfficiency


#fIn = TFile('/user/jdeclerc/Analysis/SexaQuark/CMSSW_9_4_7/src/SexaQAnalysis/AnalyzerAllSteps/test/wihtMatchingOnHits/test_TrackMatchingOnHits.root', 'read')
fIn = TFile('/storage_mnt/storage/user/jdeclerc/CMSSW_8_0_30/src/SexaQAnalysis/AnalyzerAllSteps/test/hadd/haddStep2/combinedStep2_all.root', 'read')
fOut = TFile('AnalyzerAllStepsPlots.root','RECREATE')
##########################################################################################################################################################################################################################
#add to this list all the num and denom plots of which you want to make an efficiency plot
##########################################################################################################################################################################################################################
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

for particle in ["KsNonAntiS","KsAntiS","AntiLambdaNonAntiS","AntiLambdaAntiS","AntiS"]:
	for parameter in standard_parameters:
		num = "AnalyzerAllSteps/RECO/GENRECO_"+particle+"/GENRECO_RECO_"+particle+"/h_GENRECO_RECO_"+particle+"_"+parameter
		denom = "AnalyzerAllSteps/RECO/GENRECO_"+particle+"/GENRECO_All_"+particle+"/h_GENRECO_All_"+particle+"_"+parameter
		print num
		print denom
        	aNumAndDenomForEff.append([fIn.Get(num),fIn.Get(denom)])
		teff = TEfficiency(fIn.Get(num),fIn.Get(denom))

aTeff=[]
for plots in aNumAndDenomForEff:
	aTeff.append(TEfficiency(plots[0],plots[1]))	
for teff in aTeff:
	teff.Write()
	

##########################################################################################################################################################################################################################
#project 2D histos on 1D
##########################################################################################################################################################################################################################
#currently only for 1 plot: the plot with the inneficiecies for tracking
h2 = fIn.Get("AnalyzerAllSteps/TrackingEff/Global/h2_GlobalEfficiencies")
h1x = h2.ProjectionX()
h1y = h2.ProjectionY()
#h1x.Write()
#h1y.Write()


fOut.Write()
fOut.Close()
