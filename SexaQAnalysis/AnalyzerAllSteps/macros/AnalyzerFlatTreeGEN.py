from ROOT import TFile, TH1F, TH2F, TEfficiency, TH1D, TH2D


#fIn = TFile('/user/jdeclerc/Analysis/SexaQuark/CMSSW_9_4_7/src/SexaQAnalysis/AnalyzerAllSteps/test/wihtMatchingOnHits/test_TrackMatchingOnHits.root', 'read')
fIn = TFile('/user/jdeclerc/CMSSW_8_0_30/src/SexaQAnalysis/AnalyzerAllSteps/test/FlatTreeProducerGEN/test_Looping.root', 'read')

tree = fIn.Get('FlatTreeProducerGEN/FlatTreeGENLevel') 

l_h_lxy_eta = []
l_h_vz_eta = []
l_h_lxy_n_loops = []
eta_unit = 0.2
for i in range(0,100):
	lower_eta = i*0.2
	higher_eta = (i+1)*0.2
	h_lxy_eta = TH1F('h_lxy_eta'+str(lower_eta)+'_to_'+str(higher_eta),'h_lxy_eta; lxy #bar{S} interaction vertex (cm); #Entries',100,0,100)
	l_h_lxy_eta.append(h_lxy_eta)
	h_vz_eta = TH1F('h_vz_eta'+str(lower_eta)+'_to_'+str(higher_eta),'h_vz_eta; vz #bar{S} interaction vertex (cm); #Entries',600,-300,300)
	l_h_vz_eta.append(h_vz_eta)
	h_lxy_n_loops = TH1F('h_lxy_n_loops'+str(i),'h_lxy_n_loops; lxy #bar{S} interaction vertex (cm); #Entries',100,0,100)
	l_h_lxy_n_loops.append(h_lxy_n_loops)


nEntries = tree.GetEntries()
print 'Number of entries in the tree: ', nEntries
for i in range(0,nEntries):
	tree.GetEntry(i)
	for j in range(0,len(tree._S_lxy_interaction_vertex)):
		eta_range = int(abs(tree._S_eta[j])/eta_unit)
		l_h_lxy_eta[eta_range].Fill(tree._S_lxy_interaction_vertex[j])
		l_h_vz_eta[eta_range].Fill(tree._S_vz_interaction_vertex[j])
		if(int(tree._S_n_loops[j])<100):
			l_h_lxy_n_loops[int(tree._S_n_loops[j])].Fill(tree._S_lxy_interaction_vertex[j])

fOut = TFile('AnalyzerFlatTreeProducerGEN.root','RECREATE')

lxy_eta_dir = fOut.mkdir("lxy_eta")
lxy_eta_dir.cd()
for h in l_h_lxy_eta:
	h.Write()

vz_eta_dir = fOut.mkdir("vz_eta")
vz_eta_dir.cd()
for h in l_h_vz_eta:
	h.Write()

lxy_n_loops_dir = fOut.mkdir("lxy_n_loops")
lxy_n_loops_dir.cd()
for h in l_h_lxy_n_loops:
	h.Write()

overall_dir = fOut.mkdir("overall")
overall_dir.cd()
#plot the n_loops versus eta:
h2_n_loops_vs_eta = TH2F("h2_n_loops_vs_eta","h2_n_loops_vs_eta; #eta(#bar{S}); #Loops; #Entries",80,-4,4,200,0,200)
#plot the number of interactions in each loop
h_n_interactions_vs_n_loops = TH1F("h_n_interactions_vs_n_loops","h_n_interactions_vs_n_loops; #Loops; #bar{S} doing this #Loops",200,0,200)

for i in range(0,nEntries):
	tree.GetEntry(i)
	for j in range(0,len(tree._S_lxy_interaction_vertex)):
		h2_n_loops_vs_eta.Fill(tree._S_eta[j],tree._S_n_loops[j])
		h_n_interactions_vs_n_loops.Fill(tree._S_n_loops[j])

h2_n_loops_vs_eta.Write()
h_n_interactions_vs_n_loops.Write()

fOut.Write()
fOut.Close()
