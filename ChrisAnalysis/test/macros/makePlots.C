
{
 
#include "TLatex"


 TFile *f = new TFile("delRdelP.root","UPDATE","delRdelP");      

 TH2D* PhoEff = (TH2D*)f->Get("PhoEff");
 TH2D* PhoPure = (TH2D*)f->Get("PhoPure"); 
 TH2D* PhoQual = (TH2D*)f->Get("PhoQual");
 /* TH1D* PFFake = (TH1D*)f->Get("PFFake");
 TH1D* KL0Fake = (TH1D*)f->Get("KL0Fake");
 TH1D* PionFake = (TH1D*)f->Get("PionFake");
 TH1D* EleFake = (TH1D*)f->Get("EleFake");

 TGraph* PhoEffG = new TGraph(PhoEff);
 TGraph* PhoFakeG = new TGraph(PhoFake);
 TGraph* PFFakeG = new TGraph(PFFake);
 TGraph* KL0FakeG = new TGraph(KL0Fake);
 TGraph* PionFakeG = new TGraph(PionFake);
 TGraph* EleFakeG = new TGraph(EleFake);
 */
 TCanvas *c1 = new TCanvas("c1","Photons",800,400);
 c1->SetFillColor(0);
 c1->GetFrame()->SetFillColor(6);
 c1->Divide(2,2); 
 
 c1->cd(1);
 PhoEff->SetTitle("Photon Efficiency vs. #Delta R and #Delta p_{t}");
 PhoEff->GetXaxis()->SetTitle("#Delta R");
 PhoEff->GetYaxis()->SetTitle("#Delta p_{t}");
 PhoEff->SetFillColor(9);
 PhoEff->Draw("LEGO1");
 c1->Update();

 c1->cd(2);
 PhoPure->SetTitle("Photon 'Purity' vs. #Delta R and #Delta p_{t}");
 PhoPure->GetXaxis()->SetTitle("#Delta R");
 PhoPure->GetYaxis()->SetTitle("#Delta p_{t}");
 PhoEff->SetFillColor(2);
 PhoPure->SetFillColor(9);
 PhoPure->Draw("LEGO1");
 c1->Update();

 c1->cd(3);
 PhoQual->SetTitle("Photon 'Quality' vs. #Delta R and #Delta p_{t}");
 PhoQual->GetXaxis()->SetTitle("#Delta R");
 PhoQual->GetYaxis()->SetTitle("#Delta p_{t}");
 PhoEff->SetFillColor(2); 
 PhoQual->SetFillColor(9);
 PhoQual->Draw("LEGO1");
 c1->Update();
 
 /*PhoEffG->SetLineColor(0);
 PhoEffG->SetMarkerColor(4);
 PhoEffG->SetMarkerStyle(2);
 PhoEffG->SetTitle("PF Photon Efficiency");
 PhoEffG->GetXaxis()->SetTitle("Generated #gamma p_{t}");
 PhoEffG->GetYaxis()->SetTitle("$n_{R#gamma}/n_{T#gamma}$ ");
 PhoEffG->Draw("ACP");
 
 c1->Update();
 c1->cd(2);
 
 PhoFakeG->SetLineColor(0);
 PhoFakeG->SetMarkerColor(4);
 PhoFakeG->SetMarkerStyle(2);
 PhoFakeG->SetTitle("Fake Photon Rate");
 PhoFakeG->GetXaxis()->SetTitle("Generated #gamma p_{t} ");
 PhoFakeG->GetYaxis()->SetTitle("n_{F#gamma}/n{T#gamma}"); 
 PhoFakeG->Draw("ACP");
 
 c1->Update();

 TCanvas *c2 = new TCanvas("c2","Other Fake Particles",800,800);
 c2->SetFillColor(0);
 c2->GetFrame()->SetFillColor(7);
 c2->Divide(2,2);
 c2->cd(1);

 PFFakeG->SetLineColor(0);
 PFFakeG->SetMarkerColor(3);
 PFFakeG->SetMarkerStyle(5);
 PFFakeG->SetTitle("Incorrectly Matched PF Particles");
 PFFakeG->GetXaxis()->SetTitle("Generated #gamma p_{t} ");
 PFFakeG->GetYaxis()->SetTitle("n_{FPF}/n_{T#gamma}");
 PFFakeG->Draw("ACP");
 
 c2->Update();
 c2->cd(2);

 KL0FakeG->SetLineColor(0);
 KL0FakeG->SetMarkerColor(3);
 KL0FakeG->SetMarkerStyle(5);
 KL0FakeG->SetTitle("Fake K_{L}^{0}");
 KL0FakeG->GetXaxis()->SetTitle("Generatred #gamma p_{t} ");
 KL0FakeG->GetYaxis()->SetTitle("n_{FK_{L}^{0}}/n_{TP}");
 KL0FakeG->Draw("ACP");
 
 c2->Update();
 c2->cd(3);

 PionFakeG->SetLineColor(0);
 PionFakeG->SetMarkerColor(3);
 PionFakeG->SetMarkerStyle(5);
 PionFakeG->SetTitle("Fake Pions");
 PionFakeG->GetXaxis()->SetTitle("Generatred #gamma p_{t} ");
 PionFakeG->GetYaxis()->SetTitle("n_{F#pi}");
 PionFakeG->Draw("ACP");

 c2->Update();
 c2->cd(4);

 EleFakeG->SetLineColor(0);
 EleFakeG->SetMarkerColor(3);
 EleFakeG->SetMarkerStyle(5);
 EleFakeG->SetTitle("Fake Electrons");
 EleFakeG->GetXaxis()->SetTitle("Generated #gamma p_{t} ");
 EleFakeG->GetYaxis()->SetTitle("n_{Fe}");
 EleFakeG->Draw("ACP");

  c2->Update(); 
 */

 }
