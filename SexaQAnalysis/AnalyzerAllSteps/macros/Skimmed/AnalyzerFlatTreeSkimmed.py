from ROOT import TFile, TH1F, TH2F, TEfficiency, TH1D, TH2D, TCanvas

#fIn = TFile('/user/jdeclerc/Analysis/SexaQuark/CMSSW_9_4_7/src/SexaQAnalysis/AnalyzerAllSteps/test/wihtMatchingOnHits/test_TrackMatchingOnHits.root', 'read')
fIn = TFile('/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Skimmed/CRAB_SimSexaqWithPU2016NeutrinoGun_tryToFix_8_10072019_v1/crab_SkimmingSexaqWithPU2016NeutrinoGun_tryToFix_8_10072019_v1/190710_054705/FlatTree_WithPU2016NeutrinoGun_tryToFix_8_10072019_v1.root', 'read')
tree = fIn.Get('FlatTreeProducer/FlatTree') 

fOut = TFile('macroFlatTree.root','RECREATE')

h_antiS_mass = TH1F('h_antiS_mass','; #bar{S} mass (GeV); #Entries',200,0,20)
h_antiS_pt = TH1F('h_antiS_pt','; #bar{S} p_{t} (GeV); #Entries',200,0,20)
h_antiS_pz = TH1F('h_antiS_pz','; #bar{S} |p_{z}| (GeV); #Entries',100,0,100)
h_antiS_eta = TH1F('h_antiS_eta','; #bar{S} #eta ; #Entries',160,-8,8)
h_antiS_lxy = TH1F('h_antiS_lxy','; #bar{S} interaction vertex l_{0} (cm) ; #Entries',100,0,10)
h_antiS_vz = TH1F('h_antiS_vz','; #bar{S} interaction vertex |v_{z}| (cm); #Entries',50,0,200)

h_antiS_eta_pt = TH2F('h_antiS_eta_pt',';#bar{S} #eta; #bar{S} p_{t} (GeV); #Entries',160,-8,8,200,0,20)
h_antiS_eta_pz = TH2F('h_antiS_eta_pz',';#bar{S} #eta; #bar{S} |p_{z}| (GeV); #Entries',160,-8,8,200,0,100)

h_antiS_vx_vy = TH2F('h_antiS_vx_vy',';#bar{S} interaction vertex v_{x} (cm); #bar{S} interaction vertex v_{y} (cm); #Entries',240,-120,120,240,-120,120)
h_antiS_vz_vx = TH2F('h_antiS_vz_vx',';#bar{S} interaction vertex v_{z} (cm); #bar{S} interaction vertex v_{x} (cm); #Entries',240,-120,120,240,-120,120)
h_antiS_vz_lxy = TH2F('h_antiS_vz_lxy',';#bar{S} interaction vertex v_{z} (cm); #bar{S} interaction vertex l_{0} (cm); #Entries',240,-120,120,120,0,120)

nEntries = tree.GetEntries()
print 'Number of entries in the tree: ', nEntries
for i in range(0,nEntries):
	tree.GetEntry(i)
	h_antiS_mass.Fill(tree._S_mass[0])
	h_antiS_pt.Fill(tree._S_pt[0])
	h_antiS_pz.Fill(tree._S_pz[0])
	h_antiS_eta.Fill(tree._S_eta[0])
	h_antiS_lxy.Fill(tree._S_lxy_interaction_vertex[0])	
	h_antiS_vz.Fill(abs(tree._S_vz[0]))	

	h_antiS_eta_pt.Fill(tree._S_eta[0],tree._S_pt[0])
	h_antiS_eta_pz.Fill(tree._S_eta[0],abs(tree._S_pz[0]))
	h_antiS_vx_vy.Fill(tree._S_vx[0],tree._S_vy[0])
	h_antiS_vz_vx.Fill(tree._S_vz[0],tree._S_vx[0])
	h_antiS_vz_lxy.Fill(tree._S_vz[0],tree._S_lxy_interaction_vertex[0])

c_antiS_mass = TCanvas("c_antiS_mass","");
h_antiS_mass.DrawNormalized()
h_antiS_mass.Fit("gaus")
h_antiS_mass.DrawNormalized()
c_antiS_mass.Write()

c_antiS_pt = TCanvas("c_antiS_pt","");
h_antiS_pt.DrawNormalized()
c_antiS_pt.Write()

c_antiS_pz = TCanvas("c_antiS_pz","");
h_antiS_pz.DrawNormalized()
c_antiS_pz.Write()

c_antiS_eta = TCanvas("c_antiS_eta","");
h_antiS_eta.DrawNormalized()
c_antiS_eta.Write()

c_antiS_lxy = TCanvas("c_antiS_lxy","");
h_antiS_lxy.DrawNormalized()
c_antiS_lxy.Write()

c_antiS_vz = TCanvas("c_antiS_vz","");
h_antiS_vz.DrawNormalized()
c_antiS_vz.Write()

c_antiS_eta_pt = TCanvas("c_antiS_eta_pt","")
h_antiS_eta_pt.DrawNormalized()
c_antiS_eta_pt.Write()

c_antiS_eta_pz = TCanvas("c_antiS_eta_pz","")
h_antiS_eta_pz.DrawNormalized()
c_antiS_eta_pz.Write()

fOut.Write()
fOut.Close()
