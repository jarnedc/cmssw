from ROOT import TFile, TH1F, TH2F, TEfficiency, TCanvas, TH2D


fIn = TFile('/storage_mnt/storage/user/jdeclerc/CMSSW_8_0_30/src/SexaQAnalysis/AnalyzerAllSteps/test/hadd/haddStep2/combined_AnalyzerAllStep2_WithPU2016NeutrinoGun_tryToFix_8.root', 'read')
fOut = TFile('effPlots_Step2_SexaqWithPU2016NeutrinoGun_tryToFix_8.root','RECREATE')
##########################################################################################################################################################################################################################
#add to this list all the num and denom plots of which you want to make an efficiency plot
##########################################################################################################################################################################################################################
aNumAndDenomForEff = []
aNumAndDenomForEff_2D = []
standard_parameters = ["pt","eta","phi","lxy","vz","dxy"]
twoD_standard_parameters = ["vx_vy","vx_vz","vx_vy_cut_pt_0p5","vx_vy_cut_pt_0p5_and_1","vx_vy_cut_pt_1","vx_vy_cut_pz_0p5","vx_vy_cut_pz_0p5_and_1","vx_vy_cut_pz_1","vx_vz_cut_pt_0p5","vx_vz_cut_pt_0p5_and_1","vx_vz_cut_pt_1","vx_vz_cut_pz_0p5","vx_vz_cut_pz_0p5_and_1","vx_vz_cut_pz_1"]

#Non AntiS tracking efficiency:
for parameter in ["pt","eta","phi","lxy","vz","dxy","pt_cut_eta_Lxy","lxy_cut_pt_eta","lxy_cut_pt_eta_dxy_dz","lxy_cut_tight_pt_eta","lxy_cut_pt_eta_purity_tight_high","lxy_cut_pt_eta_purity_loose_tight_high","lxy_cut_pt_eta_dxy_nPVs_1","lxy_cut_pt_eta_dxy_nPVs_smaller5","lxy_cut_pt_eta_dxy_nPVs_from5to10","lxy_cut_pt_eta_dxy_nPVs_from10to20","lxy_cut_pt_eta_dxy_nPVs_from20to30","lxy_cut_pt_eta_dxy_nPVs_larger30","eta_cut_pt_lxy"]:
#for parameter in ["pt","eta","phi","lxy","vz","dxy","pt_cut_eta_Lxy","lxy_cut_pt_eta","lxy_cut_pt_eta_dxy","eta_cut_pt_lxy"]:
	aNumAndDenomForEff.append([fIn.Get("AnalyzerGEN/TrackingEff/NonAntiSTracks/RECO/h_NonAntiSTrack_RECO_"+parameter),fIn.Get("AnalyzerGEN/TrackingEff/NonAntiSTracks/All/h_NonAntiSTrack_All_"+parameter)])

#daughters from AntiS-Ks tracking efficiency, only for charged granddaughters
for parameter in standard_parameters:
	aNumAndDenomForEff.append([fIn.Get("AnalyzerGEN/TrackingEff/AntiSKsDaughterTracks/RECO/h_AntiSKsDaughterTracks_RECO_"+parameter),fIn.Get("AnalyzerGEN/TrackingEff/AntiSKsDaughterTracks/All/h_AntiSKsDaughterTracks_All_"+parameter)])

#for the 2D efficiencies
for parameter in twoD_standard_parameters:
	aNumAndDenomForEff_2D.append([fIn.Get("AnalyzerGEN/TrackingEff/AntiSKsDaughterTracks/RECO/h2_AntiSKsDaughterTracks_RECO_"+parameter),fIn.Get("AnalyzerGEN/TrackingEff/AntiSKsDaughterTracks/All/h2_AntiSKsDaughterTracks_All_"+parameter)])
	
#AntiProton daughters from AntiS-AntiL tracking efficiency, only for charged granddaughters
for parameter in standard_parameters:
	aNumAndDenomForEff.append([fIn.Get("AnalyzerGEN/TrackingEff/AntiSAntiLambdaAntiProtonTracks/RECO/h_AntiSAntiLAntiProtonDaughterTracks_RECO_"+parameter),fIn.Get("AnalyzerGEN/TrackingEff/AntiSAntiLambdaAntiProtonTracks/All/h_AntiSAntiLAntiProtonDaughterTracks_All_"+parameter)])

#for the 2D efficiencies
for parameter in twoD_standard_parameters:
	aNumAndDenomForEff_2D.append([fIn.Get("AnalyzerGEN/TrackingEff/AntiSAntiLambdaAntiProtonTracks/RECO/h2_AntiSAntiLAntiProtonDaughterTracks_RECO_"+parameter),fIn.Get("AnalyzerGEN/TrackingEff/AntiSAntiLambdaAntiProtonTracks/All/h2_AntiSAntiLAntiProtonDaughterTracks_All_"+parameter)])
	
#PosPion daughters from AntiS-AntiL tracking efficiency, only for charged granddaughters
for parameter in standard_parameters:
	aNumAndDenomForEff.append([fIn.Get("AnalyzerGEN/TrackingEff/AntiSAntiLambdaPosPionTracks/RECO/h_AntiSAntiLPosPionDaughterTracks_RECO_"+parameter),fIn.Get("AnalyzerGEN/TrackingEff/AntiSAntiLambdaPosPionTracks/All/h_AntiSAntiLPosPionDaughterTracks_All_"+parameter)])

#for the 2D efficiencies
for parameter in twoD_standard_parameters:
	aNumAndDenomForEff_2D.append([fIn.Get("AnalyzerGEN/TrackingEff/AntiSAntiLambdaPosPionTracks/RECO/h2_AntiSAntiLPosPionDaughterTracks_RECO_"+parameter),fIn.Get("AnalyzerGEN/TrackingEff/AntiSAntiLambdaPosPionTracks/All/h2_AntiSAntiLPosPionDaughterTracks_All_"+parameter)])
	
#for particle in ["KsNonAntiS","KsAntiS","AntiLambdaNonAntiS","AntiLambdaAntiS","AntiS"]:
for particle in ["KsNonAntiS","KsAntiS","AntiLambdaNonAntiS","AntiLambdaAntiS"]:
	print particle
	for parameter in standard_parameters:
		print parameter
		num = "AnalyzerGEN/GENRECO/GENRECO_"+particle+"/GENRECO_RECO_"+particle+"/h_GENRECO_RECO_"+particle+"_"+parameter
		denom = "AnalyzerGEN/GENRECO/GENRECO_"+particle+"/GENRECO_All_"+particle+"/h_GENRECO_All_"+particle+"_"+parameter
        	aNumAndDenomForEff.append([fIn.Get(num),fIn.Get(denom)])
		teff = TEfficiency(fIn.Get(num),fIn.Get(denom))


aTeff=[]
for plots in aNumAndDenomForEff:
	print plots[0].GetName()
	aTeff.append(TEfficiency(plots[0],plots[1]))	
for teff in aTeff:
	teff.Write()
	
#for 2D teff:
for entry in aNumAndDenomForEff_2D:
	twoD_eff = TH2D(entry[0].GetName(), "",entry[0].GetYaxis().GetNbins(), entry[0].GetYaxis().GetXmin(), entry[0].GetYaxis().GetXmax(), entry[0].GetXaxis().GetNbins(), entry[0].GetXaxis().GetXmin(), entry[0].GetXaxis().GetXmax()); 
	print entry[0].GetName()
	for i in range(0,entry[0].GetNbinsX()):
		for j in range(0,entry[0].GetNbinsY()):
			if(entry[1].GetBinContent(i,j) > 0.):
				twoD_eff.SetBinContent(j,i,entry[0].GetBinContent(i,j)/entry[1].GetBinContent(i,j))
	twoD_eff.Write()
	#c1 = TCanvas("c", "c")
	#twoD_eff.Draw("colz")
	#c1.Write()
##########################################################################################################################################################################################################################
#project 2D histos on 1D
##########################################################################################################################################################################################################################
#currently only for 1 plot: the plot with the inneficiecies for tracking
h2 = fIn.Get("AnalyzerGEN/TrackingEff/Global/h2_GlobalEfficiencies")
h1x = h2.ProjectionX()
h1y = h2.ProjectionY()
#h1x.Write()
#h1y.Write()


fOut.Write()
fOut.Close()
