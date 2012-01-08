#include "TF2.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "Math/SMatrix.h"
#include "Math/SVector.h"
#include <TCanvas.h>
#include "TStyle.h"
#include "TLine.h"
#include <string>
#include <iostream>
#include <math.h>
#include <TFile.h>
#include "TLatex.h"
#include "THStack.h"

using namespace std;
void nice1DHisto(string fileName, string histoName, string histoTitle, string xAxisTitle)
{
   gStyle->SetStatColor(0);
   gStyle->SetTitleFillColor(0);
   gStyle->SetFrameBorderMode(0);
   gStyle->SetPalette(1);
   TCanvas * compareHistoCanvas = new TCanvas(histoName.c_str(),"");
   compareHistoCanvas->SetFillColor(0);

   TFile * file = new TFile(fileName.c_str());
   if(TH1F * histo = (TH1F *)file->Get(histoName.c_str()))
   {
   
      histo->SetTitle(histoTitle.c_str());
      histo->GetXaxis()->SetTitle(xAxisTitle.c_str());
      histo->SetLineColor(2);
            
      compareHistoCanvas->cd();
      
      histo->Draw("histo");
      compareHistoCanvas->Update(); 
   }
   else
   {
      cout <<"Histogram not found"<<endl;
   }
}

void nice2DHisto(string fileName, string histoName, string histoTitle, string xAxisTitle, string yAxisTitle)
{
   gStyle->SetStatColor(0);
   gStyle->SetTitleFillColor(0);
   gStyle->SetFrameBorderMode(0);
   gStyle->SetPalette(1);
   TCanvas * compareHistoCanvas = new TCanvas(histoName.c_str(),"");
   compareHistoCanvas->SetFillColor(0);

   TFile * file = new TFile(fileName.c_str());
   if(TH2F * histo = (TH2F *)file->Get(histoName.c_str()))
   {
   
      histo->SetTitle(histoTitle.c_str());
      histo->GetXaxis()->SetTitle(xAxisTitle.c_str());
      histo->GetYaxis()->SetTitle(yAxisTitle.c_str());
      histo->SetLineColor(2);
            
      compareHistoCanvas->cd();
      
      histo->Draw("colz");
      compareHistoCanvas->Update(); 
   }
   else
   {
      cout <<"Histogram not found"<<endl;
   }
}

void compare2Histos(string file1Name,string file2Name,string histoName,string histoTitle,string legend1Name, string legend2Name, string xAxisTitle)
{
   gStyle->SetOptStat(0);
   gStyle->SetStatColor(0);
   gStyle->SetTitleFillColor(0);
   gStyle->SetFrameBorderMode(0);

   TCanvas * compareHistoCanvas = new TCanvas(histoName.c_str(),"");
   compareHistoCanvas->SetFillColor(0);   

   TFile * file1 = new TFile(file1Name.c_str());
   TFile * file2 = new TFile(file2Name.c_str());

   TH1F *file1histo = (TH1F *)file1->Get(histoName.c_str());
   TH1F *file2histo = (TH1F *)file2->Get(histoName.c_str());
   file1histo->Scale(1/file1histo->GetEntries());
   file2histo->Scale(1/file2histo->GetEntries());
   file1histo->SetTitle(histoTitle.c_str());
   file1histo->GetXaxis()->SetTitle(xAxisTitle.c_str());
   file2histo->SetTitle(histoTitle.c_str());
   file1histo->SetLineColor(2);
   file2histo->SetLineColor(4);

   TLegend * legend1 = new TLegend(0.65, 0.8, 0.89, 0.89);
   legend1->SetFillColor(0);
   legend1->AddEntry(file1histo, legend1Name.c_str());
   legend1->AddEntry(file2histo, legend2Name.c_str());

   compareHistoCanvas->cd();

   file2histo->Draw("HISTO");
   file1histo->Draw("HISTO SAME");
   legend1->Draw();
   compareHistoCanvas->Update();   
}
vector<THStack *> stackHistos( vector<string> fileNames, vector<string> histoNames, vector<string> legendNames, string tag , bool saveHistos)
{
  
   gStyle->SetOptStat(0);
   gStyle->SetStatColor(0);
   gStyle->SetTitleFillColor(0);
   gStyle->SetFrameBorderMode(0);

   TH1F* histo;

   TLegend * legend = new TLegend(0.5, 0.6, 0.89, 0.89);
   legend->SetFillColor(0);

   vector<THStack *> stacks;
   vector<TCanvas *> canvases;
   size_t position;
   string tempString;

   for(unsigned j = 0; j < histoNames.size();j++)
     {
       position = histoNames[j].find("/");
       tempString = histoNames[j].substr(position + 1);
       stacks.push_back(new THStack(histoNames[j].c_str(),tempString.c_str()));

       canvases.push_back(new TCanvas(histoNames[j].c_str(),""));
       canvases[j]->SetFillColor(0);
       
       for (unsigned i = 0; i < fileNames.size(); i++)
	 {
	   TFile * file = new TFile(fileNames[i].c_str());
	   histo = (TH1F *)file->Get(histoNames[j].c_str());
	   
	   histo->SetFillColor(i + 1);
	   histo->SetMarkerStyle(22);
	   histo->SetMarkerColor(i);

	   if(j == 0) legend->AddEntry(histo, legendNames[i].c_str());
	   stacks[j]->Add(histo);
	 }
       canvases[j]->cd();
       stacks[j]->Draw();
       legend->Draw();
       if(saveHistos)
	 {
	   position = histoNames[j].find("/");
	   tempString = histoNames[j].substr(position + 1);
	   tempString = tempString + "_" + tag +".gif";
	   canvases[j]->SaveAs(tempString.c_str());
	 }
     }
   

   return stacks;
}
