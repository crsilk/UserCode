{

 #include "TLatex.h"

gStyle->SetStatColor(0);

TFile f("PFAnalysis_SingleGamma.root");


TCanvas* c = new TCanvas("c","Total Particle p_{t} > 38", 700, 500);
c->SetFillColor(0);
c->GetFrame()->SetFillColor(0);

TPad* cpad1 = new TPad("cpad1","cpad1",0.02,0.52,0.48,0.98,4);
cpad1->SetFillColor(0);
cpad1->Draw();

TPad* cpad2 = new TPad("cpad2","cpad2",0.52,0.52,0.98,0.98,4);
cpad2->SetFillColor(0);
cpad2->Draw();

TPad* cpad3 = new TPad("cpad3","cpad3",0.02, 0.02,0.48,0.48,4);
cpad3->SetFillColor(0);
cpad3->Draw();

TPad* cpad4 = new TPad("cpad4","cpad4",0.52,0.02,0.98,0.48,4);
cpad4->SetFillColor(0);
cpad4->Draw();


cpad4->cd();


TH1F* nh1 = (TH1F*)f.Get("ChrisAnalyzer/Photons/photonUnderMeasured");
nh1->SetLineWidth(2);
nh1->SetTitle("Photon p_{t}");
nh1->GetXaxis()->SetTitle("p_{t}");
nh1->Draw("histo");
c->Update();


cpad1->cd();

TH1F* nh2 = (TH1F*)f.Get("ChrisAnalyzer/Photons/electronUnderMeasured");
nh2->SetLineWidth(2);
nh2->SetTitle("Electron p_{t}");
nh2->GetXaxis()->SetTitle("p_{t}");
nh2->Draw("histo");
c->Update();

cpad2->cd();

TH1F* nh3 = (TH1F*)f.Get("ChrisAnalyzer/Photons/pionUnderMeasured");
nh3->SetLineWidth(2);
nh3->SetTitle("Pion p_{t}");
nh3->GetXaxis()->SetTitle("p_{t}");
nh3->Draw("histo");
c->Update();

cpad3->cd();

TH1F* nh4 = (TH1F*)f.Get("ChrisAnalyzer/Photons/KL0UnderMeasured");
nh4->SetLineWidth(2);
nh4->SetTitle("KL0 p_{t}");
nh4->GetXaxis()->SetTitle("p_{t}");
nh4->Draw("histo");
c->Update();

 c->SaveAs("UnderMeasuredParticlePtDistributions.gif");
	  }
