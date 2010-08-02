{
 TFile *f = new TFile("FakePlots.root","CREATE","FakePlots");      
 TH1D* h1 = new TH1D("PhoEff","PF Photon Efficiency", 100,5,1005);
 TH1D* h2 = new TH1D("PhoFake","PF Photon Fake Rate", 100,5,1005); 
 TH1D* h3 = new TH1D("PFFake","Other PF Particle Fake Rate", 100,5,1005); 
 TH1D* h4 = new TH1D("KL0Fake","KL0 Fake Rate", 100,5,1005);             
 TH1D* h5 = new TH1D("PionFake","Pion Fake Rate", 100,5,1005);
 TH1D* h6 = new TH1D("EleFake","Electron Fake Rate", 100,5,1005); 
 f->Write();
 }
