{
  
  #include "TLatex.h"


  gStyle->SetStatColor(0);
  

  TFile f("PFAnalysis_SingleGamma.root");

  //Number of Particle Histograms
  TCanvas* c = new TCanvas("c","", 700, 500);
  c->SetFillColor(0);
  c->GetFrame()->SetFillColor(0);

  TPad* cpad1 = new TPad("cpad1","cpad1",0.02,0.52,0.48,0.98,4);
  cpad1->SetFillColor(0);
  cpad1->Draw(); 

  TPad* cpad2 = new TPad("cpad2","cpad2",0.52,0.52,0.98,0.98,4);
  cpad2->SetFillColor(0);
  cpad2->Draw(); 

  TPad* cpad3 = new TPad("cpad3","cpad3",0.02, 0.02,0.32,0.48,4);
  cpad3->SetFillColor(0);
  cpad3->Draw(); 

  TPad* cpad4 = new TPad("cpad4","cpad4",0.34,0.02,0.64,0.48,4);
  cpad4->SetFillColor(0);
  cpad4->Draw(); 

  TPad* cpad5 = new TPad("cpad5","cpad5",0.66 ,0.02,.98,0.48,4);
  cpad5->SetFillColor(0);
  cpad5->Draw(); 

  cpad2->cd();

  TH1F* nh1 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConePhotons");
  nh1->SetLineWidth(2);
  nh1->SetTitle("Number of Photons in the Cone");
  nh1->GetXaxis()->SetTitle("Number");
  nh1->Draw("histo");
  c->Update();

  cpad5->cd();
  
  TH1F* nh2 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConeElectrons");
  nh2->SetLineWidth(2);
  nh2->SetTitle("Number of Electrons in the Cone");
  nh2->GetXaxis()->SetTitle("Number");
  nh2->Draw("histo");
  c->Update();

  cpad3->cd();
  
  TH1F* nh3 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConePions");
  nh3->SetLineWidth(2);
  nh3->SetTitle("Number of Pions in the Cone");
  nh3->GetXaxis()->SetTitle("Number");
  nh3->Draw("histo");
  c->Update();
  
  cpad4->cd();

  TH1F* nh4 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConeKL0s");
  nh4->SetLineWidth(2);
  nh4->SetTitle("Number of KL0s in the Cone");
  nh4->GetXaxis()->SetTitle("Number");
  nh4->Draw("histo");
  c->Update();

  cpad1->cd();
  
  TH1F* nh5 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConePFParticles");
  nh5->SetLineWidth(2);
  nh5->SetTitle("Number of PFParticles in the Cone");
  nh5->GetXaxis()->SetTitle("Number");
  nh5->Draw("histo");
  c->Update();
  c->SaveAs("ParticlesInCone.gif");

  //Lin and Log Histograms
  TCanvas* c1 = new TCanvas("c1","", 700,500);
  c1->SetFillColor(0);
  c1->GetFrame()->SetFillColor(0);

  TPad* c1pad1 = new TPad("c1pad1","c1pad1",0.02,0.02,0.48,0.98,4);
  c1pad1->SetFillColor(0);
  c1pad1->Draw(); 

  TPad* c1pad2 = new TPad("c1pad2","c1pad2",0.52,0.02,.98,0.98,4);
  c1pad2->SetFillColor(0);
  c1pad2->Draw(); 

  c1pad1->cd();
  
  TH1F* h1 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConeTotPhotonPt");
  h1->SetLineWidth(2);
  h1->SetTitle("Total p_{t} from  Photons in the Cone");
  h1->GetXaxis()->SetTitle("p_{t}");
  h1->Draw("histo");
  c1->Update();
  
  c1pad2->SetLogy();
  c1pad2->cd();
    
  h1->Draw("histo");
  c1->Update();
  c1->SaveAs("LinAndLogPhoPt.gif");
  
  TCanvas* c2 = new TCanvas("c2","", 700,500);
  c2->SetFillColor(0);
  c2->GetFrame()->SetFillColor(0);
  
  TPad* c2pad1 = new TPad("c2pad1","c2pad1",0.02,0.02,0.48,0.98,4);
  c2pad1->SetFillColor(0);
  c2pad1->Draw(); 

  TPad* c2pad2 = new TPad("c2pad2","c2pad2",0.52,0.02,.98,0.98,4);
  c2pad2->SetFillColor(0);
  c2pad2->Draw();   

  c2pad1->cd();

  TH1F* h2 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConeTotElectronPt");
  h2->SetLineWidth(2);
  h2->SetTitle("Total p_{t} from Electrons in the Cone");
  h2->GetXaxis()->SetTitle("p_{t}");
  h2->Draw("histo");
  c2->Update();
  
  c2pad2->cd();
  c2pad2->SetLogy();  

  h2->Draw("histo");
  c2->Update();
  c2->SaveAs("LinAndLogElePt.gif");
  
  TCanvas* c3 = new TCanvas("c3","", 700,500);
  c3->SetFillColor(0);
  c3->GetFrame()->SetFillColor(0);

  TPad* c3pad1 = new TPad("c3pad1","c3pad1",0.02,0.02,0.48,0.98,4);
  c3pad1->SetFillColor(0);
  c3pad1->Draw(); 

  TPad* c3pad2 = new TPad("c3pad2","c3pad2",0.52,0.02,.98,0.98,4);
  c3pad2->SetFillColor(0);
  c3pad2->Draw(); 
  
  c3pad1->cd();
  
  TH1F* h3 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConeTotPionPt");
  h3->SetLineWidth(2);
  h3->SetTitle("Total p_{t} from Pions in the Cone");
  h3->GetXaxis()->SetTitle("p_{t}");
  h3->Draw("histo");
  c3->Update();

  c3pad2->cd();
  c3pad2->SetLogy();

  h3->Draw("histo");
  c3->Update();
  c3->SaveAs("LinAndLogPioPt.gif");
  
  TCanvas* c4 = new TCanvas("c4","", 700,500);
  c4->SetFillColor(0);
  c4->GetFrame()->SetFillColor(0);

  TPad* c4pad1 = new TPad("c4pad1","c4pad1",0.02,0.02,0.48,0.98,4);
  c4pad1->SetFillColor(0);
  c4pad1->Draw(); 

  TPad* c4pad2 = new TPad("c4pad2","c4pad2",0.52,0.02,.98,0.98,4);
  c4pad2->SetFillColor(0);
  c4pad2->Draw(); 

  c4pad1->cd();  

  TH1F* h4 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConeTotKL0Pt");
  h4->SetLineWidth(2);
  h4->SetTitle("Total p_{t} from KL0s in the Cone");
  h4->GetXaxis()->SetTitle("p_{t}");
  h4->Draw("histo");
  c4->Update();

  c4pad2->cd();
  c4pad2->SetLogy();

  h4->Draw("histo");
  c4->Update();
  c4->SaveAs("LinAndLogKL0Pt.gif");

  TCanvas* c5 = new TCanvas("c5","", 700,500);
  c5->SetFillColor(0);
  c5->GetFrame()->SetFillColor(0);

  TPad* c5pad1 = new TPad("c5pad1","c5pad1",0.02,0.02,0.48,0.98,4);
  c5pad1->SetFillColor(0);
  c5pad1->Draw(); 

  TPad* c5pad2 = new TPad("c5pad2","c5pad2",0.52,0.02,.98,0.98,4);
  c5pad2->SetFillColor(0);
  c5pad2->Draw(); 

  c5pad1->cd();
  
  TH1F* h5 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConeTotPFParticlePt");
  TGraph* g5 = new TGraph( h5);
  h5->SetLineWidth(2);
  h5->SetTitle("Total p_{t} from PF Particles in the Cone");
  h5->GetXaxis()->SetTitle("p_{t}");
  h5->Draw("histo");
  c5->Update();

  c5pad2->cd();
  c5pad2->SetLogy();

  h5->Draw("histo");
  c5->Update();
  c5->SaveAs("LinAndLogPFsPt.gif");


  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);

  //Delta Angle Histograms
  TCanvas* cH1 = new TCanvas("cH1","", 700,500);
  cH1->SetFillColor(0);
  cH1->GetFrame()->SetFillColor(0);

  cH1->cd();
  
  TH2F* Hist1 = (TH2F*)f.Get("ChrisAnalyzer/Photons/inConePhotonDeltaAngle");
  Hist1->SetTitle("In Cone Photon #Delta #eta and #Delta #phi Distribution");
  Hist1->GetXaxis()->SetTitle("#Delta #eta");
  Hist1->GetYaxis()->SetTitle("#Delta #phi");
  Hist1->SetFillColor(9);
  Hist1->Draw("COLZ");
  cH1->Update();
  cH1->SaveAs("PhoDeltaAngle.gif");

  TCanvas* cH2 = new TCanvas("cH2","", 700,500);
  cH2->SetFillColor(0);
  cH2->GetFrame()->SetFillColor(0);

  cH2->cd();

  TH2D* Hist2 = (TH2D*)f.Get("ChrisAnalyzer/Photons/inConeElectronDeltaAngle");
  Hist2->SetTitle("In Cone Electron #Delta #eta and #Delta #phi Distribution");
  Hist2->GetXaxis()->SetTitle("#Delta #eta");
  Hist2->GetYaxis()->SetTitle("#Delta #phi");
  Hist2->SetFillColor(9);
  Hist2->Draw("COLZ");
  cH2->Update();
  cH2->SaveAs("EleDeltaAngle.gif");
  
  TCanvas* cH3 = new TCanvas("cH3","", 700,500);
  cH3->SetFillColor(0);
  cH3->GetFrame()->SetFillColor(0);

  cH3->cd();

  TH2D* Hist3 = (TH2D*)f.Get("ChrisAnalyzer/Photons/inConePionDeltaAngle");
  Hist3->SetTitle("In Cone Pion #Delta #eta and #Delta #phi Distribution");
  Hist3->GetXaxis()->SetTitle("#Delta #eta");
  Hist3->GetYaxis()->SetTitle("#Delta #phi");
  Hist3->SetFillColor(9);
  Hist3->Draw("COLZ");
  cH3->Update();
  cH3->SaveAs("PioDeltaAngle.gif");

  TCanvas* cH4 = new TCanvas("cH4","", 700,500);
  cH4->SetFillColor(0);
  cH4->GetFrame()->SetFillColor(0);

  cH4->cd();

  TH2D* Hist4 = (TH2D*)f.Get("ChrisAnalyzer/Photons/inConeKL0DeltaAngle");
  Hist4->SetTitle("In Cone KL0 #Delta #eta and #Delta #phi Distribution");
  Hist4->GetXaxis()->SetTitle("#Delta #eta");
  Hist4->GetYaxis()->SetTitle("#Delta #phi");
  Hist4->SetFillColor(9);
  Hist4->Draw("COLZ"); 
  cH4->Update();
  cH4->SaveAs("KL0DeltaAngle.gif");

  TCanvas* cH5 = new TCanvas("cH5","", 700,500);
  cH5->SetFillColor(0);
  cH5->GetFrame()->SetFillColor(0);

  cH5->cd();

  TH2D* Hist5 = (TH2D*)f.Get("ChrisAnalyzer/Photons/inConePFParticleDeltaAngle");
  Hist5->SetTitle("In Cone PFParticle #Delta #eta and #Delta #phi Distribution");
  Hist5->GetXaxis()->SetTitle("#Delta #eta");
  Hist5->GetYaxis()->SetTitle("#Delta #phi");
  Hist5->SetFillColor(9);
  Hist5->Draw("COLZ");
  cH5->Update();
  cH5->SaveAs("PFsDeltaAngle.gif");

 
 
}
