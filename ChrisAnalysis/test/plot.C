{
  #include "TLatex.h"
  TFile f("PFAnalysis_SingleGamma.root");

  int style = 7;
  int size = 4;
  int color = 8;


  TCanvas* c1 = new TCanvas("c1","", 700,500);
  c1->SetFillColor(0);
  c1->GetFrame()->SetFillColor(0);
  pad1 = new TPad("pad1","pad1",0.02,0.52,0.48,0.98,32);
  pad2 = new TPad("pad2","pad2",0.52,0.52,.98,0.98,4);
  pad3 = new TPad("pad3","pad3",0.02,0.02,0.32,.48,4);
  pad4 = new TPad("pad4","pad4",0.34,0.02,0.64,0.48,4);
  pad5 = new TPad("pad5","pad5",0.67,0.02,0.97,0.48,4);
  pad1->SetFillColor(0);
  pad2->SetFillColor(0);
  pad3->SetFillColor(0);
  pad4->SetFillColor(0);
  pad5->SetFillColor(0);
  pad1->Draw(); 
  pad2->Draw(); 
  pad3->Draw(); 
  pad4->Draw();
  pad5->Draw();
  
  pad1->cd();
  
  /*  TH1F* h1 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConeTotPhotonPt");
  TGraph* g1 = new TGraph( h1);
  g1->SetLineColor(0);
  g1->SetMarkerColor(color);
  g1->SetMarkerStyle(style);
  g1->SetMarkerSize(size);
  g1->SetTitle("Total p_{t} from  Photons in the Cone");
  g1->GetXaxis()->SetTitle("p_{t}");
  g1->Draw("ACP");
  c1->Update();
  */
  TH2F* Hist1 = (TH2F*)f.Get("ChrisAnalyzer/Photons/inConePhotonDeltaAngle");
  Hist1->SetTitle("In Cone Photon #Delta #eta and #Delta #phi Distribution");
  Hist1->GetXaxis()->SetTitle("#Delta #eta");
  Hist1->GetYaxis()->SetTitle("#Delta #phi");
  Hist1->SetFillColor(9);
  Hist1->Draw("LEGO1");
  
  pad3->cd();
  /*
  TH1F* h2 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConeTotElectronPt");
  TGraph* g2 = new TGraph( h2);
  g2->SetLineColor(0);
  g2->SetMarkerColor(color);
  g2->SetMarkerStyle(style);
  g2->SetMarkerSize(size);
  g2->SetTitle("Total p_{t} from Electrons in the Cone");
  g2->GetXaxis()->SetTitle("p_{t}");
  g2->Draw("ACP");
  c1->Update();
  
  */

  TH2D* Hist2 = (TH2D*)f.Get("ChrisAnalyzer/Photons/inConeElectronDeltaAngle");
  Hist2->SetTitle("In Cone Electron #Delta #eta and #Delta #phi Distribution");
  Hist2->GetXaxis()->SetTitle("#Delta #eta");
  Hist2->GetYaxis()->SetTitle("#Delta #phi");
  Hist2->SetFillColor(9);
  Hist2->Draw("LEGO1");

  pad5->cd();  
  
  /*
  TH1F* h3 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConeTotPionPt");
  TGraph* g3 = new TGraph( h3);
  g3->SetLineColor(0);
  g3->SetMarkerColor(color);
  g3->SetMarkerStyle(style);
  g3->SetMarkerSize(size);
  g3->SetTitle("Total p_{t} from Pions in the Cone");
  g3->GetXaxis()->SetTitle("p_{t}");
  g3->Draw("ACP");
  c1->Update();
  */

  TH2D* Hist3 = (TH2D*)f.Get("ChrisAnalyzer/Photons/inConePionDeltaAngle");
  Hist3->SetTitle("In Cone Pion #Delta #eta and #Delta #phi Distribution");
  Hist3->GetXaxis()->SetTitle("#Delta #eta");
  Hist3->GetYaxis()->SetTitle("#Delta #phi");
  Hist3->SetFillColor(9);
  Hist3->Draw("LEGO1");

  pad4->cd();
  
  /*
  TH1F* h4 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConeTotKL0Pt");
  TGraph* g4 = new TGraph( h4);
  g4->SetLineColor(0);
  g4->SetMarkerColor(color);
  g4->SetMarkerStyle(style);
  g4->SetMarkerSize(size);
  g4->SetTitle("Total p_{t} from KL0s in the Cone");
  g4->GetXaxis()->SetTitle("p_{t}");
  g4->Draw("ACP");
  c1->Update();
  */

  TH2D* Hist4 = (TH2D*)f.Get("ChrisAnalyzer/Photons/inConeKL0DeltaAngle");
  Hist4->SetTitle("In Cone KL0 #Delta #eta and #Delta #phi Distribution");
  Hist4->GetXaxis()->SetTitle("#Delta #eta");
  Hist4->GetYaxis()->SetTitle("#Delta #phi");
  Hist4->SetFillColor(9);
  Hist4->Draw("LEGO1");

  pad2->cd();
  
  /*
  TH1F* h5 = (TH1F*)f.Get("ChrisAnalyzer/Photons/inConeTotPFParticlePt");
  TGraph* g5 = new TGraph( h5);
  g5->SetLineColor(0);
  g5->SetMarkerColor(color);
  g5->SetMarkerStyle(style);
  g5->SetMarkerSize(size);
  g5->SetTitle("Total p_{t} from PF Particles in the Cone");
  g5->GetXaxis()->SetTitle("p_{t}");
  g5->Draw("ACP");
  c1->Update();
  */
  
  TH2D* Hist5 = (TH2D*)f.Get("ChrisAnalyzer/Photons/inConePFParticleDeltaAngle");
  Hist5->SetTitle("In Cone PFParticle #Delta #eta and #Delta #phi Distribution");
  Hist5->GetXaxis()->SetTitle("#Delta #eta");
  Hist5->GetYaxis()->SetTitle("#Delta #phi");
  Hist5->SetFillColor(9);
  Hist5->Draw("LEGO1");
 

 
 
}
