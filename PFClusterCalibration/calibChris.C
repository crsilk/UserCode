#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TF1.h"
#include "TF2.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "Math/SMatrix.h"
#include "Math/SVector.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <string>
#include <iostream>
#include <math.h>


using namespace std;

///////////////////////////////////////////////////////////////////////////////
//Container class that holds all the coefficients for a particular Etrue bin
///////////////////////////////////////////////////////////////////////////////

class ABCAlphaBeta
{
   private:

      vector<double> ETrueEnergies_; //Variables that fall in this ETrue bin, 
      vector<double> ecalEnergies_;  //which is defined by binLowEdge and 
      vector<double> hcalEnergies_;  //binHighEdge. sigmaEcalHcal is simply the
      vector<double> etas_;          //uncertainty from the ecal and hcal(not
      vector<double> sigmaEcalHcal_; //uncertainty in the calibration consts)
      double binLowEdge_;
      double binHighEdge_;
      double etaMinFit_;     //From what etas to fit the B and C constants.
      double etaMaxFit_;     
      double etaMinEtaFit_;  //From what etas to fit the Alpha and Beta 
      double etaMaxEtaFit_;  //constants.
      bool isBarrel_;
      

      double ETrueAverage_; //Average and RMS of the ETrue values in the bin.
      double ETrueRMS_;
      double a_;  //Constant values.
      double b_;
      double c_;
      double alpha_;
      double beta_;

      double sigmaB_;  //Uncertainty in the constants
      double sigmaC_;
      double sigmaAlpha_;
      double sigmaBeta_;

   public:
      ABCAlphaBeta(double binLowEdge, double binHighEdge, bool isBarrel); 

      bool addEntry(double ETrueEnergy, double ecalEnergy, double hcalEnergy,
                    double eta);  //Adds an event to the ETrue bin
      double getBinLowEdge();
      double getBinHighEdge();
      bool isBarrel();  //Checks if it is a barrel-type constant storage
      bool isEmpty();   //Checks if its empty
      bool isEmptyInFit();   //Checks if its empty in eta fit range
      unsigned getSize();        //Returns the various stored variables in the
      double getETrueAverage();  //ABCAlphaBeta object
      double getETrueRMS();
      double getA();
      double getB();
      double getC();
      double getAlpha();
      double getBeta();
      double getSigmaB();
      double getSigmaC();
      double getSigmaAlpha();
      double getSigmaBeta();


      void computeETrueAverage();  //Computes the various calibration constants
      void computeETrueRMS();      //and other stored elements in the object.
      void computeA(double a);     //Right now the constant "a" is not computed
      void computeB();             //but rather just set.
      void computeC();
      bool computeBC();
      bool computeAlphaBeta();
      void clear();
};

ABCAlphaBeta::ABCAlphaBeta(double binLowEdge, double binHighEdge, 
                                     bool isBarrel) 

{
   binLowEdge_ = binLowEdge;
   binHighEdge_ = binHighEdge;
   isBarrel_ = isBarrel;

   if(isBarrel_)
   {
      etaMinFit_ = 0.0;
      etaMaxFit_ = 1.0;
      etaMaxEtaFit_ = 0.0;
      etaMaxEtaFit_ = 1.3;
   }
   else
   {
      etaMinFit_ = 1.6;
      etaMaxFit_ = 2.2;
      etaMaxEtaFit_ = 1.6;
      etaMaxEtaFit_ = 2.8;
   }
   
   a_ = 0;
   b_ = 0;
   c_ = 0;
   alpha_ = 0;
   beta_ = 0;
   ETrueAverage_ = 0;
   ETrueRMS_ = 0;
   sigmaB_ = 0;
   sigmaC_ = 0;
   sigmaAlpha_ = 0;
   sigmaBeta_ = 0;
}
bool ABCAlphaBeta::addEntry(double ETrue, double ecalEnergy, double hcalEnergy, double eta)
{
   double sigmaEcalHcal;

   if(isBarrel_) 
      sigmaEcalHcal = sqrt(0.08*0.08 + 1.04*1.04*(std::max(ecalEnergy + 
                                                           hcalEnergy, 1.0)));
   else
      sigmaEcalHcal = sqrt(0.04*0.04 + 1.80*1.80*(std::max(ecalEnergy + 
                                                           hcalEnergy, 1.0)));

   if((ecalEnergy + hcalEnergy + a_ - ETrue) < -5.0*sigmaEcalHcal) return false;

   ETrueEnergies_.push_back(ETrue);
   ecalEnergies_.push_back(ecalEnergy);
   hcalEnergies_.push_back(hcalEnergy);
   etas_.push_back(eta);
   sigmaEcalHcal_.push_back(sigmaEcalHcal);

   return true;
}

double ABCAlphaBeta::getBinLowEdge() {return binLowEdge_;}
double ABCAlphaBeta::getBinHighEdge() {return binHighEdge_;}
double ABCAlphaBeta::getETrueAverage() {return ETrueAverage_;}
double ABCAlphaBeta::getETrueRMS() {return ETrueRMS_;}
double ABCAlphaBeta::getA() {return a_;}
double ABCAlphaBeta::getB() {return b_;}
double ABCAlphaBeta::getC() {return c_;}
double ABCAlphaBeta::getAlpha() {return alpha_;}
double ABCAlphaBeta::getBeta() {return beta_;}
double ABCAlphaBeta::getSigmaB() {return sigmaB_;}
double ABCAlphaBeta::getSigmaC() {return sigmaC_;}
double ABCAlphaBeta::getSigmaAlpha() {return sigmaAlpha_;}
double ABCAlphaBeta::getSigmaBeta() {return sigmaBeta_;}
bool ABCAlphaBeta::isBarrel() {return isBarrel_;}
bool ABCAlphaBeta::isEmpty() 
{
   if(ETrueEnergies_.size() == 0) return true;
   else return false;
}
bool ABCAlphaBeta::isEmptyInFit() 
{
   for(unsigned i = 0; i < etas_.size(); i++)
   {
      if(fabs(etas_[i]) < etaMaxFit_ && fabs(etas_[i]) > etaMinFit_)
         return false;
   }
   return true;
}
unsigned ABCAlphaBeta::getSize()
{
   return ETrueEnergies_.size();
}
void ABCAlphaBeta::computeETrueAverage()
{
   double totalETrue = 0;
   int numberSkipped = 0;

   for(unsigned i = 0; i < ETrueEnergies_.size(); i++)
   {
     if(fabs(etas_[i]) > etaMaxFit_ || fabs(etas_[i]) < etaMinFit_ ) 
      {
         numberSkipped++;
         continue;
      }   

     totalETrue += ETrueEnergies_[i];
   }
   
   ETrueAverage_ = totalETrue/(ETrueEnergies_.size() - numberSkipped);
} 
void ABCAlphaBeta::computeETrueRMS()
{
   double totalETrueSquared = 0;
   int numberSkipped = 0;

   for(unsigned i = 0; i<ETrueEnergies_.size(); i++)
   {
      if(fabs(etas_[i]) > etaMaxFit_ || fabs(etas_[i]) < etaMinFit_ ) 
      {
         numberSkipped++;
         continue;
      }
       
      totalETrueSquared += ETrueEnergies_[i]*ETrueEnergies_[i];
   }
   
   ETrueRMS_ = sqrt(
      (totalETrueSquared/(ETrueEnergies_.size() - numberSkipped) -
       ETrueAverage_*ETrueAverage_)/(ETrueEnergies_.size() - numberSkipped)); 
} 
void ABCAlphaBeta::computeA(double a) 
{
   a_ = a; 
}
void ABCAlphaBeta::computeB() 
{
   double totalEcalSquared = 0;
   double totalEMinusATimesEcal = 0;
   
   for (unsigned i = 0; i < ETrueEnergies_.size(); i++)
   {
      if(fabs(etas_[i]) > etaMaxFit_ || fabs(etas_[i]) < etaMinFit_ ) continue;

      totalEcalSquared += 2* ecalEnergies_[i]*ecalEnergies_[i]/
         (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
      totalEMinusATimesEcal += 2*(ETrueEnergies_[i] - a_)*ecalEnergies_[i]/
         (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
   }
   
   b_ = totalEMinusATimesEcal/totalEcalSquared;
   sigmaB_ = sqrt(1/totalEcalSquared);
}
void ABCAlphaBeta::computeC() 
{
   double totalHcalSquared = 0;
   double totalEMinusATimesHcal = 0;
   
   for (unsigned i = 0; i < ETrueEnergies_.size(); i++)
   {
      if(fabs(etas_[i]) > etaMaxFit_ || fabs(etas_[i]) < etaMinFit_ ) continue;

      totalHcalSquared += 2*hcalEnergies_[i]*hcalEnergies_[i]/
         (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
      totalEMinusATimesHcal += 2*(ETrueEnergies_[i] - a_)*hcalEnergies_[i]/
         (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
   }

   c_ = totalEMinusATimesHcal/totalHcalSquared;
   sigmaC_ = sqrt(1/totalHcalSquared);
}
bool ABCAlphaBeta::computeBC()
{
   ROOT::Math::SMatrix<double,2, 2, ROOT::Math::MatRepStd<double,2> > coeffs;
   ROOT::Math::SVector<double, 2> consts;
   ROOT::Math::SVector<double, 2> values;
   bool isInverted;
 
   coeffs(0, 0) = 0;
   coeffs(0, 1) = 0;
   coeffs(1, 0) = 0;
   coeffs(1, 1) = 0;
   consts(0) = 0;
   consts(1) = 0;
   //Create the matrix that will be inverted and the vector which will multiply
   //that matrix to find the b and c calibration constants.
   for(unsigned i = 0; i < ETrueEnergies_.size(); ++i)
   {
      if(fabs(etas_[i]) > etaMaxFit_ || fabs(etas_[i]) < etaMinFit_ ) continue;
      
      coeffs(0, 0) += 2*ecalEnergies_[i]*ecalEnergies_[i]/
         (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
      
      coeffs(0, 1) += 2*ecalEnergies_[i]*hcalEnergies_[i]/
         (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
      
      coeffs(1, 0) += 2*ecalEnergies_[i]*hcalEnergies_[i]/
         (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
      
      coeffs(1, 1) += 2*hcalEnergies_[i]*hcalEnergies_[i]/
         (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
      
      consts(0) += 2*(ETrueEnergies_[i] - a_)*ecalEnergies_[i] /
         (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
      
      consts(1) += 2*(ETrueEnergies_[i] - a_)*hcalEnergies_[i]/
         (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);

   }

   isInverted = coeffs.Invert();


   if(isInverted) //Make sure it inverted successfully (i.e. det != 0)
   {
      values = coeffs*consts;
      
      b_ = values(0);
      c_ = values(1);
      sigmaB_ = sqrt(coeffs(0,0));
      sigmaC_ = sqrt(coeffs(1,1));
      return true;
   }
   else return false;
}

bool ABCAlphaBeta::computeAlphaBeta()
{
   ROOT::Math::SMatrix<double,2, 2, ROOT::Math::MatRepStd<double,2> > coeffs;
   ROOT::Math::SVector<double, 2> consts;
   ROOT::Math::SVector<double, 2> values;
   bool isInverted;
   vector<double> etaPow;
   double factor;

   coeffs(0, 0) = 0;
   coeffs(0, 1) = 0;
   coeffs(1, 0) = 0;
   coeffs(1, 1) = 0;
   consts(0) = 0;
   consts(1) = 0;

   if(isBarrel_) 
   {
      for(unsigned i = 0; i < etas_.size(); i++) 
         etaPow.push_back(etas_[i]*etas_[i]);
      

      for(unsigned i = 0; i < etas_.size(); ++i)
      {   
         if(fabs(etas_[i]) < etaMinEtaFit_ || fabs(etas_[i]) > etaMaxEtaFit_)
            continue;
         
         coeffs(0, 0) += 2*b_*ecalEnergies_[i]*b_*ecalEnergies_[i]/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         coeffs(0, 1) += 2*etaPow[i]*b_*ecalEnergies_[i]*b_*ecalEnergies_[i]/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         coeffs(1, 0) += 2*etaPow[i]*b_*ecalEnergies_[i]*b_*ecalEnergies_[i]/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         coeffs(1, 1) += 2*etaPow[i]*etaPow[i]*b_*ecalEnergies_[i]*
            b_*ecalEnergies_[i]/(sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         consts(0) += 2*(ETrueEnergies_[i] - a_ - b_*ecalEnergies_[i] - 
             c_*hcalEnergies_[i])*b_*ecalEnergies_[i]/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);  
         
         consts(1) += 2*etaPow[i]*
            (ETrueEnergies_[i] - a_ - b_*ecalEnergies_[i] - 
             c_*hcalEnergies_[i])*b_*ecalEnergies_[i]/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
      }
   }
   else
   {
      factor = 0.5; //A factor put in my hand to make the eta dependence agree
                    //better. Should fit for the value later.
      for(unsigned i = 0; i < etas_.size(); i++) 
         etaPow.push_back((fabs(etas_[i]) - 1.5)*(fabs(etas_[i]) - 1.5)*
                          (fabs(etas_[i]) - 1.5)*(fabs(etas_[i]) - 1.5));
      
      for(unsigned i = 0; i < etas_.size(); ++i)
      {   
         if(fabs(etas_[i]) < etaMinEtaFit_ || fabs(etas_[i]) > etaMaxEtaFit_)
            continue;
         
         coeffs(0, 0) += 2*
            (factor*b_*ecalEnergies_[i] + c_*hcalEnergies_[i])*
            (factor*b_*ecalEnergies_[i] + c_*hcalEnergies_[i])/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         coeffs(0, 1) += 2*etaPow[i]*
            (factor*b_*ecalEnergies_[i] + c_*hcalEnergies_[i])*
            (factor*b_*ecalEnergies_[i] + c_*hcalEnergies_[i])/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         coeffs(1, 0) += 2*etaPow[i]*
            (factor*b_*ecalEnergies_[i] + c_*hcalEnergies_[i])*
            (factor*b_*ecalEnergies_[i] + c_*hcalEnergies_[i])/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         coeffs(1, 1) += 2*etaPow[i]*etaPow[i]*
            (factor*b_*ecalEnergies_[i] + c_*hcalEnergies_[i])*
            (factor*b_*ecalEnergies_[i] + c_*hcalEnergies_[i])/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         consts(0) += 2*
            (ETrueEnergies_[i] - a_ - b_*ecalEnergies_[i] - 
             c_*hcalEnergies_[i])*
            (factor*b_*ecalEnergies_[i] + c_*hcalEnergies_[i])/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);  
         
         consts(1) += 2*etaPow[i]*
            (ETrueEnergies_[i] - a_ - b_*ecalEnergies_[i] - 
             c_*hcalEnergies_[i])*
            (factor*b_*ecalEnergies_[i] + c_*hcalEnergies_[i])/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
      }
   }

   //Create the matrix that will be inverted and the vector which will multiply
   //that matrix to find the alpha and beta calibration constants.


   isInverted = coeffs.Invert();
   
   if(isInverted)
   {
      values = coeffs*consts;
      
      alpha_ = values(0);
      beta_ = values(1);
      sigmaAlpha_ = sqrt(coeffs(0,0));
      sigmaBeta_ = sqrt(coeffs(1,1));

      return true;
   }
   else
   {
      return false;
   }
} 

void ABCAlphaBeta::clear()
{
   ETrueEnergies_.clear();
   ecalEnergies_.clear();
   hcalEnergies_.clear();
   etas_.clear();
   
   delete &binLowEdge_;
   delete &binHighEdge_;
   delete &isBarrel_;
   delete &ETrueAverage_;
   delete &ETrueRMS_;
   delete &a_;
   delete &b_;
   delete &c_;
   delete &alpha_;
   delete &beta_;
   delete &sigmaB_;
   delete &sigmaC_;
   delete &sigmaAlpha_;
   delete &sigmaBeta_;
}


///////////////////////////////////////////////////////////////////////////////
//The class that holds the calibration information over all values of ETrue. 
//It takes in a collection of ABCAlphaBeta objects and fits each calibration 
//constant to a function. These functions are then used to find the calibrated
//energy.
///////////////////////////////////////////////////////////////////////////////
class Calibration
{
   private:
      double ETrueMax_;
      bool isBarrel_;
      
      vector<double> ETrueAverages_;
      vector<double> ETrueRMSs_;

      vector<double> as_;
      vector<double> bs_;
      vector<double> cs_;
      vector<double> alphas_;
      vector<double> betas_;
 
      vector<double> sigmaBs_;
      vector<double> sigmaCs_;
      vector<double> sigmaAlphas_;
      vector<double> sigmaBetas_;
      
      TGraph *graphA_ ;
      TGraphErrors *graphB_;
      TGraphErrors *graphC_;
      TGraphErrors *graphAlpha_;
      TGraphErrors *graphBeta_ ;

      TGraph *graphBError_;
      TGraph *graphCError_;
      TGraph *graphAlphaError_;
      TGraph *graphBetaError_ ;
      TF1 *functionA_;
      TF1 *functionB_;
      TF1 *functionC_;
      TF1 *functionAlpha_;
      TF1 *functionBeta_;
      
   public:
      Calibration();
      Calibration(double ETrueMax, bool isBarrel);
      //Adds a graph point by hand, i.e. putting in the individual values.
      void addGraphPoints(double ETrueAverage, double ETrueRMS,
                          double a, double b, double sigmaB, double C,
                          double sigmaC, double alpha, double sigmaAlpha,
                          double beta, double sigmaBeta); 
      //Adds a graph point by taking apart an ABCAlphaBeta object.
      void addGraphPoints(ABCAlphaBeta* abcalphabeta);
      //Creates the graphs after the points have been added.
      void initializeGraphs();  
      double getETrueMax();      //Returns the various objects  that the 
      TGraphErrors* getGraph();  //calibration class holds.
      TF1* getFunctionA();
      TF1* getFunctionB();
      TF1* getFunctionC();
      TF1* getFunctionAlpha();
      TF1* getFunctionBeta();      
      //Returns calibrated energy without any eta dependence.
      double getCalibratedEnergy(double ETrue, double ecalEnergy, 
                                 double hcalEnergy); 
      //Returns calibrated energy with eta dependence.
      double getCalibratedEnergy(double ETrue, double ecalEnergy, 
                                 double hcalEnergy, double eta);
      void setETrueMax(double ETrueMax);
      bool fitAsToFunction(TF1 *functionA);  //Fits the functions to their 
      bool fitAsToFunction();                //graph points. One that takes in
      bool fitBsToFunction(TF1 *functionB);  //a function which then fits it to
      bool fitBsToFunction();                //that function, and one that is
      bool fitCsToFunction(TF1 *functionC);  //used to simply improve the fit.
      bool fitCsToFunction();
      bool fitAlphasToFunction(TF1 *functionAlpha);
      bool fitAlphasToFunction();
      bool fitBetasToFunction(TF1 *functionBetas);
      bool fitBetasToFunction();
      void drawCoeffGraph(string graph);  //Makes and draws a graph of a 
      void drawSigmaGraph(string graph);  //coefficient. Takes in as an 
                                          //argument a, b, c, alpha, or beta.
      void printBs();
      void printCs();
      void printBetas();
      void printAlphas();
};

Calibration::Calibration()
{
   ETrueMax_ = 1000;
   isBarrel_ = true;
}
Calibration::Calibration(double ETrueMax, bool isBarrel) 
          
{
   ETrueMax_ = ETrueMax;
   isBarrel_ = isBarrel;
}
void Calibration::addGraphPoints(double ETrueAverage, double ETrueRMS,
                                 double a, double b, double sigmaB, double c,
                                 double sigmaC, double alpha, 
                                 double sigmaAlpha, double beta, 
                                 double sigmaBeta)
{
   ETrueAverages_.push_back(ETrueAverage);
   ETrueRMSs_.push_back(ETrueRMS);
   as_.push_back(a);
   bs_.push_back(b);
   cs_.push_back(c);
   alphas_.push_back(alpha);
   betas_.push_back(beta);
   

   sigmaBs_.push_back(sigmaB);
   sigmaCs_.push_back(sigmaC);
   sigmaAlphas_.push_back(sigmaAlpha);
   sigmaBetas_.push_back(sigmaBeta);

}
void Calibration::addGraphPoints(ABCAlphaBeta* coeffs)
{
   if(coeffs->isEmptyInFit()) return;

   ETrueAverages_.push_back(coeffs->getETrueAverage());
   ETrueRMSs_.push_back(coeffs->getETrueRMS());
   as_.push_back(coeffs->getA());
   bs_.push_back(coeffs->getB());
   cs_.push_back(coeffs->getC());
   alphas_.push_back(coeffs->getAlpha());
   betas_.push_back(coeffs->getBeta());

   sigmaBs_.push_back(coeffs->getSigmaB());
   sigmaCs_.push_back(coeffs->getSigmaC());
   sigmaAlphas_.push_back(coeffs->getSigmaAlpha());
   sigmaBetas_.push_back(coeffs->getSigmaBeta());

}
void Calibration::initializeGraphs()
{
   vector<double> x;
   vector<double> sigmaX;
   vector<double> y;
   vector<double> sigmaY;
   
   for(unsigned i = 0; i < ETrueAverages_.size(); i++)
   {
      if( bs_[i] == 0 && sigmaBs_[i] == 0.0) continue;

      x.push_back(ETrueAverages_[i]);
      sigmaX.push_back(ETrueRMSs_[i]);
      y.push_back(bs_[i]);
      sigmaY.push_back(sigmaBs_[i]);
      
   }
   
   graphB_ = new TGraphErrors(x.size(), &x[0], &y[0], &sigmaX[0], &sigmaY[0]);

   x.clear();
   sigmaX.clear();
   y.clear();
   sigmaY.clear();

   for(unsigned i = 0; i < ETrueAverages_.size(); i++)
   {
      if( cs_[i] == 0 && sigmaCs_[i] == 0.0) continue;

      x.push_back(ETrueAverages_[i]);
      sigmaX.push_back(ETrueRMSs_[i]);
      y.push_back(cs_[i]);
      sigmaY.push_back(sigmaCs_[i]);
      
   }
   
   graphC_ = new TGraphErrors(x.size(), &x[0], &y[0], &sigmaX[0], &sigmaY[0]);

   x.clear();
   sigmaX.clear();
   y.clear();
   sigmaY.clear();

   for(unsigned i = 0; i < ETrueAverages_.size(); i++)
   {
      if( alphas_[i] == 0 && sigmaAlphas_[i] == 0.0) continue;

      x.push_back(ETrueAverages_[i]);
      sigmaX.push_back(ETrueRMSs_[i]);
      y.push_back(alphas_[i]);
      sigmaY.push_back(sigmaAlphas_[i]);
      
   }
   
   graphAlpha_ = new TGraphErrors(x.size(), &x[0], &y[0], &sigmaX[0], 
                                  &sigmaY[0]);
   x.clear();
   sigmaX.clear();
   y.clear();
   sigmaY.clear();

   for(unsigned i = 0; i < ETrueAverages_.size(); i++)
   {
      if( betas_[i] == 0 && sigmaBetas_[i] == 0.0) continue;

      x.push_back(ETrueAverages_[i]);
      sigmaX.push_back(ETrueRMSs_[i]);
      y.push_back(betas_[i]);
      sigmaY.push_back(sigmaBetas_[i]);
      
   }
   
   graphBeta_ = new TGraphErrors(x.size(), &x[0], &y[0], &sigmaX[0], 
                                 &sigmaY[0]);
   x.clear();
   sigmaX.clear();
   y.clear();
   sigmaY.clear();

   graphBError_ = new TGraph(ETrueAverages_.size(), &ETrueAverages_[0], 
                             &sigmaBs_[0]);
   graphCError_ = new TGraph(ETrueAverages_.size(), &ETrueAverages_[0], 
                             &sigmaCs_[0]);
   graphAlphaError_ = new TGraph(ETrueAverages_.size(), &ETrueAverages_[0],  
                                 &sigmaAlphas_[0]);
   graphBetaError_ = new TGraph(ETrueAverages_.size(), &ETrueAverages_[0], 
                                &sigmaBetas_[0]);
   
}
double Calibration::getETrueMax() {return ETrueMax_;}
TGraphErrors* Calibration::getGraph() {return graphB_;}
TF1* Calibration::getFunctionA() {return functionA_;}
TF1* Calibration::getFunctionB() {return functionB_;}
TF1* Calibration::getFunctionC() {return functionC_;}
TF1* Calibration::getFunctionAlpha() {return functionAlpha_;}
TF1* Calibration::getFunctionBeta() {return functionBeta_;}
double Calibration::getCalibratedEnergy(double ETrue, double ecalEnergy, 
                                        double hcalEnergy)
{
   double a = functionA_->Eval(ETrue);
   double b = functionB_->Eval(ETrue);
   double c = functionC_->Eval(ETrue);
   
   return a + b*ecalEnergy + c*hcalEnergy;
}
double Calibration::getCalibratedEnergy(double ETrue, double ecalEnergy, 
                                        double hcalEnergy, double eta)
{
   double etaPow;
   double factor;
   double a = functionA_->Eval(ETrue);
   double b = functionB_->Eval(ETrue);
   double c = functionC_->Eval(ETrue);
   double alpha = functionAlpha_->Eval(ETrue);
   double beta = functionBeta_->Eval(ETrue);
   double counterAlpha = 0;
   double counterBeta = 0;

   if(isBarrel_) 
   {
      etaPow = eta*eta;
      factor = 1.0;
      beta = 0;
      counterAlpha = alpha;
      counterBeta = beta;
   }
   else 
   {
      etaPow = (fabs(eta) - 1.5)*(fabs(eta) - 1.5)*(fabs(eta) - 1.5)*
         (fabs(eta) - 1.5);
      factor = 0.5;
      if( hcalEnergy == 0) factor = 1.0;
   }
   
   return a + (1.0 + alpha + factor*beta*etaPow)*b*ecalEnergy + 
      (1.0 + alpha + beta*etaPow - counterAlpha - counterBeta*etaPow)*
      c*hcalEnergy;
}
void Calibration::setETrueMax(double ETrueMax){ETrueMax_ = ETrueMax;}


bool Calibration::fitAsToFunction(TF1 *functionA)
{
   functionA_ = functionA;
//   graphA_->Fit(functionA_->GetName(), "Q", "", 0, ETrueMax_);
   return true;
}
bool Calibration::fitAsToFunction()
{
   graphA_->Fit(functionA_->GetName(), "Q", "", 0, ETrueMax_);
   return true;
}
bool Calibration::fitBsToFunction(TF1 *functionB)
{
   functionB_ = functionB;
   graphB_->Fit(functionB_->GetName(), "Q", "", 0, ETrueMax_);
   return true;
}
bool Calibration::fitBsToFunction()
{
   graphB_->Fit(functionB_->GetName(), "Q", "", 0, ETrueMax_);
   return true;
}
bool Calibration::fitCsToFunction(TF1 *functionC)
{
   functionC_ = functionC;
   graphC_->Fit(functionC_->GetName(), "Q", "", 0, ETrueMax_);
   return true;
}
bool Calibration::fitCsToFunction()
{
   graphC_->Fit(functionC_->GetName(), "Q", "", 0, ETrueMax_);
   return true;
}
bool Calibration::fitAlphasToFunction(TF1 *functionAlpha)
{
   functionAlpha_ = functionAlpha;
   graphAlpha_->Fit(functionAlpha_->GetName(), "Q", "", 0, ETrueMax_);
   return true;
}
bool Calibration::fitAlphasToFunction()
{
   graphAlpha_->Fit(functionAlpha_->GetName(), "Q", "", 0, ETrueMax_);
   return true;
}
bool Calibration::fitBetasToFunction(TF1 *functionBeta)
{
   functionBeta_ = functionBeta;
   graphBeta_->Fit(functionBeta_->GetName(), "Q", "", 0, ETrueMax_);
   return true;
}
bool Calibration::fitBetasToFunction()
{
   graphBeta_->Fit(functionBeta_->GetName(), "Q", "", 0, ETrueMax_);
   return true;
}
void Calibration::drawCoeffGraph(string graph)
{
  
   TCanvas* canvas = new TCanvas();
   TH2F* histo = new TH2F("histo", "", 100, 0, 1000, 100,  -3.0, 3.0); 

   canvas->cd();
   histo->SetStats(0);
   histo->GetXaxis()->SetTitle("True Energy(GeV)");
   histo->GetYaxis()->SetTitle("Value");
   histo->Draw();
   
   gPad->SetGridx();
   gPad->SetGridy();
   

  if(graph == "b" || graph == "B") 
   {
      histo->SetTitle("B vs True Energy");
      graphB_->SetMarkerStyle(22);
      graphB_->SetMarkerSize(.5);
      graphB_->Draw("P");
   }
   else if(graph == "c" || graph == "C") 
   {
      histo->SetTitle("C vs True Energy");
      graphC_->SetMarkerStyle(22);
      graphC_->SetMarkerSize(.5);
      graphC_->SetMarkerColor(2);
      graphC_->Draw("P");
   }
   else if(graph == "alpha" || graph == "Alpha") 
   {
      histo->SetTitle("Alpha vs True Energy");
      graphAlpha_->SetMarkerStyle(22);
      graphAlpha_->SetMarkerSize(.5);
      graphAlpha_->SetMarkerColor(4);
      graphAlpha_->Draw("P");
   }
   else if(graph == "beta" || graph == "Beta") 
   {
      histo->SetTitle("Beta vs True Energy");
      graphBeta_->SetMarkerStyle(22);
      graphBeta_->SetMarkerSize(.5);
      graphBeta_->SetMarkerColor(3);
      graphBeta_->Draw("P");
   }  
   else cout << "No graph with that name" <<endl;
 


}
void Calibration::drawSigmaGraph(string graph)
{
  
   TCanvas* canvas2 = new TCanvas();
   TH2F* histo2 = new TH2F("histo", "", 100, 0, ETrueMax_, 100,  0, .1); 
   
   canvas2->cd();
   histo2->SetStats(0);
   histo2->Draw();
   
   gPad->SetGridx();
   gPad->SetGridy();

   if(graph == "b" || graph =="B")
   {
      graphBError_->SetMarkerStyle(22);
      graphBError_->SetMarkerColor(2);
      graphBError_->SetMarkerSize(.5);
      graphBError_->Draw("P");

   }
   else if(graph == "c" || graph =="C")
   {
      graphCError_->SetMarkerStyle(22);
      graphCError_->SetMarkerColor(4);
      graphCError_->SetMarkerSize(.5);
      graphCError_->Draw("P");
   }
   else if(graph == "alpha" || graph =="Alpha")
   {
      graphAlphaError_->SetMarkerStyle(22);
      graphAlphaError_->SetMarkerColor(5);
      graphAlphaError_->SetMarkerSize(.5);
      graphAlphaError_->Draw("P");
   }
   else if(graph == "beta" || graph =="Beta")
   {
      graphBetaError_->SetMarkerStyle(22);
      graphBetaError_->SetMarkerColor(7);
      graphBetaError_->SetMarkerSize(.5);
      graphBetaError_->Draw("P");
   }
   else cout<<"No graph with that name"<<endl;
   
   delete histo2;

}
void Calibration::printBs()
{
   
   for(unsigned i = 0; i < as_.size(); i++)
   {
     cout<<bs_[i];
   }
}

void Calibration::printCs()
{
   
   for(unsigned i = 0; i < as_.size(); i++)
   {
      cout<<cs_[i];
   }
}
void Calibration::printBetas()
{
   
   for(unsigned i = 0; i < betas_.size(); i++)
   {
      cout<<betas_[i];
   }
}
void Calibration::printAlphas()
{
   
   for(unsigned i = 0; i < alphas_.size(); i++)
   {
      cout<<alphas_[i];
   }
}
///////////////////////////////////////////////////////////////////////////////
//Global functions used in the main of the code.
///////////////////////////////////////////////////////////////////////////////


//Takes apart a TH2 and creates response and resolution plots from it. Note: 
//the TGraphs will not draw correctly without passing the TGraphs as references
//to the function.
void drawGausFit(TH2F* inHisto, TGraph& response, TGraph& resolution)
{
   if(inHisto->GetEntries() == 0) return;

   vector<TH1F*> ETrueBin;
   TF1* gaus; 
   string name;
   char num[3];
   int rebin = 1;
   TCanvas* canvas;
   TCanvas* temp = new TCanvas();

   TH2F* respHisto = new TH2F("respHisto", "", 100, 0, 1000, 100, -1.0, 1.0);
   TH2F* resoHisto = new TH2F("resoHisto", "", 100, 0, 1000, 100, 0.0, 0.5);

   TGraph averages;
   TGraph rmss;

   vector<double> ETrue;
   vector<double> gausMean; 
   vector<double> gausSigma;
   vector<double> average;
   vector<double> rms;
   
   
   temp->cd();//This TCanvas is only used since when we do the fit down below 
              //it creates an unwanted TCanvas. We will get rid of it later on 
              //in the function.


   for(unsigned bin = 2; bin < 1000; )
   {
      name = "hist";
      sprintf(num, "%i", bin);
      name += num;
      //Split up the TH2 into many TH1's for each ETrue bin.
      ETrueBin.push_back((TH1F*)inHisto->ProjectionY(name.c_str(),bin, 
                                                     bin + 4*rebin));

      //Fit each ETrue bin to a gaus (iteratively done just get better fit)
      if(ETrueBin.back()->GetEntries() != 0)
      {
         ETrueBin.back()->Fit("gaus", "Q", "", -1.0, 1.0);
         gaus = ETrueBin.back()->GetFunction("gaus");
         
         ETrueBin.back()->Fit("gaus", "Q", "",
                              gaus->GetParameter(1) - 2*gaus->GetParameter(2), 
                              1.0);     
         gaus = ETrueBin.back()->GetFunction("gaus");
         ETrueBin.back()->Fit("gaus", "Q", "",
                              gaus->GetParameter(1) - 2*gaus->GetParameter(2), 
                           1.0);
         gaus = ETrueBin.back()->GetFunction("gaus");
         
         ETrue.push_back(bin + 2.0*rebin);
         gausMean.push_back(gaus->GetParameter(1));
         gausSigma.push_back(gaus->GetParameter(2)/
                             (1.0 + min(0.0, gaus->GetParameter(1))));
         average.push_back(ETrueBin.back()->GetMean());
         rms.push_back(ETrueBin.back()->GetRMS());
      }
      
      bin += 4*rebin;
      
      //Increase bin size with increasing ETrue since there are fewer high 
      //energy events than low energy ones.
      if(bin > 10) rebin = 2.0;
      if(bin > 100) rebin = 20.0;
      if(bin > 1000) rebin = 50.0;
   }
   response = TGraph(ETrue.size(), &ETrue[0], &gausMean[0]); //Fill the graphs
   resolution = TGraph(ETrue.size(),&ETrue[0], &gausSigma[0]);
   averages =  TGraph(ETrue.size(), &ETrue[0], &average[0]);
   rmss = TGraph(ETrue.size(), &ETrue[0], &rms[0]);

   //Set up the graphs to look how you want them to.
   response.SetMarkerStyle(22);
   response.SetMarkerSize(0.8);
   response.SetMarkerColor(2);

   resolution.SetMarkerStyle(22);
   resolution.SetMarkerSize(0.8);
   resolution.SetMarkerColor(2);

   averages.SetMarkerStyle(22);
   averages.SetMarkerSize(0.8);
   averages.SetMarkerColor(4);

   rmss.SetMarkerStyle(22);
   rmss.SetMarkerSize(0.8);
   rmss.SetMarkerColor(4);

   canvas = new TCanvas("canvas", "Response and Resolution", 
                                 1000, 500);
   canvas->Divide(2, 1);
   temp->~TCanvas();  //destroy the TCanvas 

   canvas->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   respHisto->SetStats(0);
   respHisto->SetTitle("Response");
   respHisto->Draw();
   response.Draw("P");

   canvas->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   resoHisto->SetStats(0);
   resoHisto->SetTitle("Resolution");
   resoHisto->Draw();
   resolution.Draw("P");

   
}

//Takes apart a TTree from a root file and puts the wanted information into 
//vectors. 
void getValuesFromTree(TTree* tree, vector<double>& ETrueEnergies, 
                       vector<double>& ecalEnergies, 
                       vector<double>& hcalEnergies, vector<double>& etas, 
                       vector<double>& phis)
{
   Float_t         true_;
   Float_t         p_;
   Float_t         ecal_;
   Float_t         hcal_;
   Float_t         eta_;
   Float_t         phi_;
   TBranch        *b_true; 
   TBranch        *b_p;   
   TBranch        *b_ecal;   
   TBranch        *b_hcal;   
   TBranch        *b_eta;    
   TBranch        *b_phi;    

   tree->SetMakeClass(1);
   
   if(tree->GetBranchStatus("true"))
      tree->SetBranchAddress("true", &true_, &b_true);
   tree->SetBranchAddress("p", &p_, &b_p);
   tree->SetBranchAddress("ecal", &ecal_, &b_ecal);
   tree->SetBranchAddress("hcal", &hcal_, &b_hcal);
   tree->SetBranchAddress("eta", &eta_, &b_eta);
   tree->SetBranchAddress("phi", &phi_, &b_phi);

   for( unsigned entry = 0; entry < tree->GetEntriesFast(); entry++)
   {
      tree->GetEntry(entry);

      if(tree->GetBranchStatus("true")) ETrueEnergies.push_back(true_);
      else ETrueEnergies.push_back(p_);
      ecalEnergies.push_back(ecal_);
      hcalEnergies.push_back(hcal_);
      etas.push_back(eta_);
      phis.push_back(phi_);
   }
}


///////////////////////////////////////////////////////////////////////////////
//All the needed variables for the main (calibChris) function.
///////////////////////////////////////////////////////////////////////////////

TFile* inputFile;
TTree* sTree;

vector<double> ETrueEnergies;  //The values that are taken from the root file
vector<double> ecalEnergies;
vector<double> hcalEnergies;
vector<double> etas;
vector<double> phis;

vector<ABCAlphaBeta*> barrelWithEcalHcal; //Vectors of the ABCAlphaBeta objects
vector<ABCAlphaBeta*> barrelWithEcal;     //which hold all the calibration 
vector<ABCAlphaBeta*> barrelWithHcal;     //constants for an individual bin.
vector<ABCAlphaBeta*> endcapWithEcalHcal;
vector<ABCAlphaBeta*> endcapWithEcal;
vector<ABCAlphaBeta*> endcapWithHcal;

TF1* functionBarrelEcalHcalA;     //Functions that the calibration equations
TF1* functionBarrelEcalHcalB;     //are fit to
TF1* functionBarrelEcalHcalC;  
TF1* functionBarrelEcalHcalAlpha;
TF1* functionBarrelEcalHcalBeta;
TF1* functionEndcapEcalHcalA;
TF1* functionEndcapEcalHcalB;
TF1* functionEndcapEcalHcalC;  
TF1* functionEndcapEcalHcalAlpha;
TF1* functionEndcapEcalHcalBeta;

TF1* functionBarrelHcalA;
TF1* functionBarrelHcalB;
TF1* functionBarrelHcalC;  
TF1* functionBarrelHcalAlpha;
TF1* functionBarrelHcalBeta;
TF1* functionEndcapHcalA;
TF1* functionEndcapHcalB;
TF1* functionEndcapHcalC;  
TF1* functionEndcapHcalAlpha;
TF1* functionEndcapHcalBeta;

//Calibration objects which hold the all the calibration costants as functions
//of ETrue. 
Calibration* barrelWithEcalHcalCalib = new Calibration(1000, true);
Calibration* barrelWithEcalCalib = new Calibration(1000, true);
Calibration* barrelWithHcalCalib = new Calibration(1000, true);
Calibration* endcapWithEcalHcalCalib = new Calibration(1000, false);
Calibration* endcapWithEcalCalib = new Calibration(1000, false);
Calibration* endcapWithHcalCalib = new Calibration(1000, false);

//Temporary varibles that will be used in for loops just to cut down on the 
//length of the lines of code.
double etrue;
double ecal;
double hcal;
double eta;
double correctedE;
double correctedEEta;

double etrueMax;

//All the differenct types of TH2's that will be filled in order to make 
//resolution and response plots.
TH2F* uncorrectedBarrel = new TH2F("uncorrectedBarrel","", 1000, 0, 1000, 150, 
                                   -1.5, 1.5);
TH2F* correctedBarrel = new TH2F("correctedBarrel", "", 1000, 0, 1000, 150, 
                                 -1.5, 1.5);
TH2F* correctedBarrelEta = new TH2F("correctedBarrelEta", "", 1000, 0, 1000, 
                                    150, -1.5, 1.5);
TH2F* uncorrectedBarrelEcalHcal = new TH2F("uncorrectedBarrelEcalHcal","", 
                                           1000, 0, 1000, 150, -1.5, 1.5);
TH2F* correctedBarrelEcalHcal = new TH2F("correctedBarrelEcalHcal", "", 1000, 
                                         0, 1000, 150, -1.5, 1.5);
TH2F* correctedBarrelEtaEcalHcal = new TH2F("correctedBarrelEtaEcalHcal", 
                                            "", 1000, 0, 1000, 150, -1.5, 1.5);
TH2F* uncorrectedBarrelHcal = new TH2F("uncorrectedBarrelHcal","", 1000, 0, 
                                       1000, 150, -1.5, 1.5);
TH2F* correctedBarrelHcal = new TH2F("correctedBarrelHcal", "", 1000, 0, 
                                     1000, 150, -1.5, 1.5);
TH2F* correctedBarrelEtaHcal = new TH2F("correctedBarrelEtaHcal", "", 1000, 
                                        0, 1000, 150, -1.5, 1.5);

TH2F* uncorrectedEndcap = new TH2F("uncorrectedEndcap","", 1000, 0, 1000, 150, 
                                   -1.5, 1.5);
TH2F* correctedEndcap = new TH2F("correctedEndcap", "", 1000, 0, 1000, 150, 
                                 -1.5, 1.5);
TH2F* correctedEndcapEta = new TH2F("correctedEndcapEta", "", 1000, 0, 1000, 
                                    150, -1.5, 1.5);
TH2F* uncorrectedEndcapEcalHcal = new TH2F("uncorrectedEndcapEcalHcal","", 
                                           1000, 0, 1000, 150, -1.5, 1.5);
TH2F* correctedEndcapEcalHcal = new TH2F("correctedEndcapEcalHcal", "", 1000, 
                                         0, 1000, 150, -1.5, 1.5);
TH2F* correctedEndcapEtaEcalHcal = new TH2F("correctedEndcapEtaEcalHcal", 
                                            "", 1000, 0, 1000, 150, -1.5, 1.5);
TH2F* uncorrectedEndcapHcal = new TH2F("uncorrectedEndcapHcal","", 1000, 0, 
                                       1000, 150, -1.5, 1.5);
TH2F* correctedEndcapHcal = new TH2F("correctedEndcapHcal", "", 1000, 0, 
                                     1000, 150, -1.5, 1.5);
TH2F* correctedEndcapEtaHcal = new TH2F("correctedEndcapEtaHcal", "", 1000, 
                                        0, 1000, 150, -1.5, 1.5);
//Temporary TGraphs to passed drawGausFit
TGraph response;
TGraph resolution;

///////////////////////////////////////////////////////////////////////////////
//This is the main of the macro. Everything that you want output must be added 
//in here. I have it so that all the variables that I use were defined above 
//since it looks neater.
///////////////////////////////////////////////////////////////////////////////
void calibChris() 
{
   gROOT->Reset();
   gStyle->SetCanvasColor(0);

   //Open the file, get the tree and fill of the vectors of values you need.
   inputFile = TFile::Open("pfcalib_all.root");
   sTree = (TTree*)inputFile->Get("s");
   getValuesFromTree(sTree, ETrueEnergies, ecalEnergies, 
                     hcalEnergies, etas, phis);



   //Create all the ABCAlphaBeta objects you need with increasing bin size
   //since there are fewer events at higher energies. 
   
   for(double bin = 0.0; bin < 10.0; bin = bin + 1.0)
   {
      barrelWithEcalHcal.push_back(new ABCAlphaBeta(bin, bin + 1.0, true));
      barrelWithEcal.push_back(new ABCAlphaBeta(bin, bin + 1.0, true));
      barrelWithHcal.push_back(new ABCAlphaBeta(bin, bin + 1.0, true));
      endcapWithEcalHcal.push_back(new ABCAlphaBeta(bin, bin + 1.0, false));
      endcapWithEcal.push_back(new ABCAlphaBeta(bin, bin + 1.0,false));
      endcapWithHcal.push_back(new ABCAlphaBeta(bin, bin + 1.0, false));
                                                                
   }
   for(double bin = 10.0; bin < 100.0 ; bin = bin + 2.0)
   {
      barrelWithEcalHcal.push_back(new ABCAlphaBeta(bin, bin + 2.0, true));
      barrelWithEcal.push_back(new ABCAlphaBeta(bin, bin + 2.0, true));
      barrelWithHcal.push_back(new ABCAlphaBeta(bin, bin + 2.0, true));
      endcapWithEcalHcal.push_back(new ABCAlphaBeta(bin, bin + 2.0, false));
      endcapWithEcal.push_back(new ABCAlphaBeta(bin, bin + 2.0,false));
      endcapWithHcal.push_back(new ABCAlphaBeta(bin, bin + 2.0, false));
   }
   
   for(double bin = 100.0; bin < 1000.0 ; bin = bin + 10)
   {
      barrelWithEcalHcal.push_back(new ABCAlphaBeta(bin, bin + 10.0, true));
      barrelWithEcal.push_back(new ABCAlphaBeta(bin, bin + 10.0, true));
      barrelWithHcal.push_back(new ABCAlphaBeta(bin, bin + 10.0, true));
      endcapWithEcalHcal.push_back(new ABCAlphaBeta(bin, bin + 10.0, false));
      endcapWithEcal.push_back(new ABCAlphaBeta(bin, bin + 10.0,false));
      endcapWithHcal.push_back(new ABCAlphaBeta(bin, bin + 10.0, false));  
   }

   //Fill all the ABCAlphaObjects with their respective events. They are all 
   //divided up into the six possible case ( (endcap or barrel)x(ecalhcal or 
   //ecal or hcal))
   for(unsigned entry = 0; entry < ETrueEnergies.size() ; ++entry)
   {
      etrue = ETrueEnergies[entry];
      ecal = ecalEnergies[entry];
      hcal = hcalEnergies[entry];
      eta = etas[entry];

      if((ecal + hcal) < 0.5 || etrue < 1.0) continue;
      
      for( unsigned bin = 0; bin < barrelWithEcal.size(); ++bin)
      {
         if( fabs(eta) < 1.6)
         {
            if( ecal > 0 && hcal > 0)
            {
               if(etrue < barrelWithEcalHcal[bin]->getBinHighEdge() &&
                  etrue > barrelWithEcalHcal[bin]->getBinLowEdge())
               {
                  barrelWithEcalHcal[bin]->addEntry(etrue, ecal, hcal, eta);

               }
            }
            else if(ecal > 0)
            {
               if(etrue < barrelWithEcal[bin]->getBinHighEdge() &&
                  etrue > barrelWithEcal[bin]->getBinLowEdge())
                  barrelWithEcal[bin]->addEntry(etrue, ecal, hcal ,eta);
            }
            else if(hcal > 0)
            {
               if(etrue <barrelWithHcal[bin]->getBinHighEdge() &&
                  etrue > barrelWithHcal[bin]->getBinLowEdge())
                  barrelWithHcal[bin]->addEntry(etrue, ecal, hcal, eta);
            }
         }
         if(fabs(eta) > 1.5 && fabs(eta) < 3.0)
         {
            if( ecal > 0 && hcal > 0)
            {
               if(etrue < endcapWithEcalHcal[bin]->getBinHighEdge() &&
                  etrue > endcapWithEcalHcal[bin]->getBinLowEdge())
                  endcapWithEcalHcal[bin]->addEntry(etrue, ecal, hcal, eta);


            }
            else if(ecal > 0)
            {
               if(etrue < endcapWithEcal[bin]->getBinHighEdge() &&
                  etrue > endcapWithEcal[bin]->getBinLowEdge())
                  endcapWithEcal[bin]->addEntry(etrue, ecal, hcal ,eta);

            }
            else if(hcal > 0)
            {
               if(etrue <endcapWithHcal[bin]->getBinHighEdge() &&
                  etrue > endcapWithHcal[bin]->getBinLowEdge())
                  endcapWithHcal[bin]->addEntry(etrue, ecal, hcal, eta);

            }  
         }
      }
   }
   //Compute the calibration constants along with their uncertainties for each
   //ETrue bin, then add their values to a Calibration object.
   

   for(unsigned bin = 1; bin < barrelWithEcalHcal.size() - 1; ++bin)
   {
      if(!barrelWithEcalHcal[bin]->isEmptyInFit())
      { 
         barrelWithEcalHcal[bin]->computeETrueAverage();
         barrelWithEcalHcal[bin]->computeETrueRMS();
         barrelWithEcalHcal[bin]->computeA(3.5);
         barrelWithEcalHcal[bin]->computeBC();
         barrelWithEcalHcal[bin]->computeAlphaBeta();
      }
      if(!barrelWithEcal[bin]->isEmptyInFit())
      { 
         barrelWithEcal[bin]->computeETrueAverage();
         barrelWithEcal[bin]->computeETrueRMS();
         barrelWithEcal[bin]->computeA(3.5);
         barrelWithEcal[bin]->computeB();
         barrelWithEcal[bin]->computeAlphaBeta();
      }
      if(!barrelWithHcal[bin]->isEmptyInFit())
      { 
         barrelWithHcal[bin]->computeETrueAverage();
         barrelWithHcal[bin]->computeETrueRMS();
         barrelWithHcal[bin]->computeA(3.0);
         barrelWithHcal[bin]->computeC();
         barrelWithHcal[bin]->computeAlphaBeta();
      }
      if(!endcapWithEcalHcal[bin]->isEmptyInFit())
      {
         endcapWithEcalHcal[bin]->computeETrueAverage();
         endcapWithEcalHcal[bin]->computeETrueRMS();
         endcapWithEcalHcal[bin]->computeA(3.5);
         endcapWithEcalHcal[bin]->computeBC();
         endcapWithEcalHcal[bin]->computeAlphaBeta();
      }
      if(!endcapWithEcal[bin]->isEmptyInFit())
      {
         endcapWithEcal[bin]->computeETrueAverage();
         endcapWithEcal[bin]->computeETrueRMS();
         endcapWithEcal[bin]->computeA(3.5);
         endcapWithEcal[bin]->computeB();
         endcapWithEcal[bin]->computeAlphaBeta();
      }
      if(!endcapWithHcal[bin]->isEmptyInFit())
      {
         endcapWithHcal[bin]->computeETrueAverage();
         endcapWithHcal[bin]->computeETrueRMS();
         endcapWithHcal[bin]->computeA(2.5);
         endcapWithHcal[bin]->computeC();
         endcapWithHcal[bin]->computeAlphaBeta();
      }

      if(barrelWithEcalHcal[bin]->isEmptyInFit() && bin > 10 && 
         barrelWithEcalHcalCalib->getETrueMax() == 1000)
      {
         barrelWithEcalHcalCalib->setETrueMax(
            barrelWithEcalHcal[bin]->getBinLowEdge());
      }
      if(barrelWithEcal[bin]->isEmpty() && bin > 10 &&
         barrelWithEcalCalib->getETrueMax() == 1000)
      {
         barrelWithEcalCalib->setETrueMax(
            barrelWithEcal[bin]->getBinLowEdge());
      }
      if(barrelWithHcal[bin]->isEmpty() && bin > 10 &&
         barrelWithHcalCalib->getETrueMax() == 1000)
      {
         barrelWithHcalCalib->setETrueMax(
            barrelWithHcal[bin]->getBinLowEdge());
      }
      if(endcapWithEcalHcal[bin]->isEmpty() && bin > 10 && 
         endcapWithEcalHcalCalib->getETrueMax() == 1000)
      {
         endcapWithEcalHcalCalib->setETrueMax(
            endcapWithEcalHcal[bin]->getBinLowEdge());
      }
      if(endcapWithEcal[bin]->isEmpty() && bin > 10 &&
         endcapWithEcalCalib->getETrueMax() == 1000)
      {
         endcapWithEcalCalib->setETrueMax(
            endcapWithEcal[bin]->getBinLowEdge());
      }
      if(endcapWithHcal[bin]->isEmpty() && bin > 10 &&
         endcapWithHcalCalib->getETrueMax() == 1000)
      {
         endcapWithHcalCalib->setETrueMax(
            endcapWithHcal[bin]->getBinLowEdge());
      }

      barrelWithEcalHcalCalib->addGraphPoints(barrelWithEcalHcal[bin]);
      barrelWithEcalCalib->addGraphPoints(barrelWithEcal[bin]);
      barrelWithHcalCalib->addGraphPoints(barrelWithHcal[bin]);
      endcapWithEcalHcalCalib->addGraphPoints(endcapWithEcalHcal[bin]);
      endcapWithEcalCalib->addGraphPoints(endcapWithEcal[bin]);
      endcapWithHcalCalib->addGraphPoints(endcapWithHcal[bin]);
   }
   
   //Initialize all the graphs in the calibration objects.
   barrelWithEcalHcalCalib->initializeGraphs();
   barrelWithEcalCalib->initializeGraphs();
   barrelWithHcalCalib->initializeGraphs();   
   endcapWithEcalHcalCalib->initializeGraphs();
   endcapWithEcalCalib->initializeGraphs();
   endcapWithHcalCalib->initializeGraphs();   

   //Define the functions that you will fit your calibration constants to.
   functionBarrelEcalHcalA = new TF1("functionBarrelEcalHcalA","[0]",0,1000);
   functionBarrelEcalHcalB = new TF1("functionBarrelEcalHcalB","[0]+([1]+[2]/sqrt(x))*exp(-x/[3])-[4]*exp(-x*x/[5])",0,1000);
   functionBarrelEcalHcalC = new TF1("functionBarrelEcalHcalC","[0]+([1]+[2]/sqrt(x))*exp(-x/[3])-[4]*exp(-x*x/[5])",0,1000);
   functionBarrelEcalHcalAlpha = new TF1("functionBarrelEcalHcalAlpha","[0]+[1]*exp(-x/[2])",0,1000);
   functionBarrelEcalHcalBeta = new TF1("functionBarrelEcalHcalBeta","[0]+[1]*exp(-x/[2])+[3]*[3]*exp(-x*x/([4]*[4]))",0,1000);
   functionEndcapEcalHcalA = new TF1("functionEndcapEcalHcalA","[0]",0,1000);
   functionEndcapEcalHcalB = new TF1("functionEndcapEcalHcalB","[0]+([1]+[2]/sqrt(x))*exp(-x/[3])-[4]*exp(-x*x/[5])",0,1000);
   functionEndcapEcalHcalC = new TF1("functionEndcapEcalHcalC","[0]+([1]+[2]/sqrt(x))*exp(-x/[3])-[4]*exp(-x*x/[5])",0,1000);
   functionEndcapEcalHcalAlpha = new TF1("functionEndcapEcalHcalAlpha","[0]+[1]*exp(-x/[2])",0,1000);
   functionEndcapEcalHcalBeta = new TF1("functionEndcapEcalHcalBeta","[0]+[1]*exp(-x/[2])+[3]*[3]*exp(-x*x/([4]*[4]))",0,1000);

   functionBarrelHcalA = new TF1("functionBarrelHcalA","[0]",0,1000);
   functionBarrelHcalB = new TF1("functionBarrelHcalB","[0]",0,1000);
   functionBarrelHcalC = new TF1("functionBarrelHcalC","[0]+([1]+[2]/sqrt(x))*exp(-x/[3])-[4]*exp(-x*x/[5])",0,1000);
   functionBarrelHcalAlpha = new TF1("functionBarrelHcalAlpha","[0]+[1]*exp(-x/[2])",0,1000);
   functionBarrelHcalBeta = new TF1("functionBarrelHcalBeta","[0]+[1]*exp(-x/[2])+[3]*[3]*exp(-x*x/([4]*[4]))",0,1000);
   functionEndcapHcalA = new TF1("functionEndcapHcalA","[0]",0,1000);
   functionEndcapHcalB = new TF1("functionEndcapHcalB","[0]",0,1000);
   functionEndcapHcalC = new TF1("functionEndcapHcalC","[0]+([1]+[2]/sqrt(x))*exp(-x/[3])-[4]*exp(-x*x/[5])",0,1000);
   functionEndcapHcalAlpha = new TF1("functionEndcapHcalAlpha","[0]+[1]*exp(-x/[2])",0,1000);
   functionEndcapHcalBeta = new TF1("functionEndcapHcalBeta","[0]+[1]*exp(-x/[2])+[3]*[3]*exp(-x*x/([4]*[4]))",0,1000);

   //Set the parameters of the function you just defined.
   functionBarrelEcalHcalA->FixParameter(0, 3.5);
   functionBarrelEcalHcalB->SetParameters(1.10,0.12,-0.2,119,1.3,80);
   functionBarrelEcalHcalC->SetParameters(1.04,0.27,-0.6,70,1.05,50);
   functionBarrelEcalHcalC->FixParameter(4,1.0);
   functionBarrelEcalHcalAlpha->SetParameters(0.02, -0.1, 200);
   functionBarrelEcalHcalBeta->SetParameters(-0.02, 0.4, 200, 0.0, 0.0);
   functionEndcapEcalHcalA->FixParameter(0, 3.5);
   functionEndcapEcalHcalB->SetParameters(1.10,0.12,-0.2,119,1.3,80);
   functionEndcapEcalHcalB->FixParameter(4,2.0);
   functionEndcapEcalHcalC->SetParameters(1.04,0.27,-0.6,70,1.05,50);
   functionEndcapEcalHcalC->FixParameter(4,1.2);
   functionEndcapEcalHcalAlpha->SetParameters(0.02, -0.1, 200);
   functionEndcapEcalHcalBeta->SetParameters(-0.02, 0.4, 200, 0.0, 0.0);
   functionEndcapEcalHcalBeta->FixParameter(3, 0.0);
   functionEndcapEcalHcalBeta->FixParameter(4, 0.0);

   functionBarrelHcalA->FixParameter(0, 3.0);
   functionBarrelHcalB->FixParameter(0, 0.0);
   functionBarrelHcalC->SetParameters(.95, 0.01, 0.7, 76, 1.23, 25);
   functionBarrelHcalAlpha->SetParameters(0.02, -0.1, 200);
   functionBarrelHcalBeta->SetParameters(-0.02, 0.4, 200, 0.0, 0.0);
   functionEndcapHcalA->FixParameter(0, 2.5);
   functionEndcapHcalB->FixParameter(0, 0.0);
   functionEndcapHcalC->SetParameters(.95, 0.01, 0.7, 76, 1.23, 25);
   functionEndcapHcalC->FixParameter(4,1.2);
   functionEndcapHcalAlpha->SetParameters(0.02, -0.1, 200);
   functionEndcapHcalBeta->SetParameters(-0.07, -2.5, 6.0, 0.3, 175);

   //Fit the functions that were defined above to the calibration graphs that
   //are stored in the calibration objects.
   barrelWithEcalHcalCalib->fitAsToFunction(functionBarrelEcalHcalA);
   barrelWithEcalHcalCalib->fitBsToFunction(functionBarrelEcalHcalB);
   barrelWithEcalHcalCalib->fitBsToFunction();
   barrelWithEcalHcalCalib->fitCsToFunction(functionBarrelEcalHcalC);
   barrelWithEcalHcalCalib->fitCsToFunction();
   barrelWithEcalHcalCalib->fitCsToFunction();
   barrelWithEcalHcalCalib->fitAlphasToFunction(functionBarrelEcalHcalAlpha);
   barrelWithEcalHcalCalib->fitAlphasToFunction();
   barrelWithEcalHcalCalib->fitBetasToFunction(functionBarrelEcalHcalBeta);
   barrelWithEcalHcalCalib->fitBetasToFunction();
   endcapWithEcalHcalCalib->fitAsToFunction(functionEndcapEcalHcalA);
   endcapWithEcalHcalCalib->fitBsToFunction(functionEndcapEcalHcalB);
   endcapWithEcalHcalCalib->fitBsToFunction();
   endcapWithEcalHcalCalib->fitCsToFunction(functionEndcapEcalHcalC);
   endcapWithEcalHcalCalib->fitCsToFunction();
   endcapWithEcalHcalCalib->fitCsToFunction();
   endcapWithEcalHcalCalib->fitAlphasToFunction(functionEndcapEcalHcalAlpha);
   endcapWithEcalHcalCalib->fitAlphasToFunction();
   endcapWithEcalHcalCalib->fitBetasToFunction(functionEndcapEcalHcalBeta);
   endcapWithEcalHcalCalib->fitBetasToFunction();
   
   barrelWithHcalCalib->fitAsToFunction(functionBarrelHcalA);
   barrelWithHcalCalib->fitBsToFunction(functionBarrelHcalB);
   barrelWithHcalCalib->fitBsToFunction();
   barrelWithHcalCalib->fitCsToFunction(functionBarrelHcalC);
   barrelWithHcalCalib->fitCsToFunction();
   barrelWithHcalCalib->fitCsToFunction();
   barrelWithHcalCalib->fitAlphasToFunction(functionBarrelHcalAlpha);
   barrelWithHcalCalib->fitAlphasToFunction();
   barrelWithHcalCalib->fitBetasToFunction(functionBarrelHcalBeta);
   barrelWithHcalCalib->fitBetasToFunction();
   endcapWithHcalCalib->fitAsToFunction(functionEndcapHcalA);
   endcapWithHcalCalib->fitBsToFunction(functionEndcapHcalB);
   endcapWithHcalCalib->fitBsToFunction();
   endcapWithHcalCalib->fitCsToFunction(functionEndcapHcalC);
   endcapWithHcalCalib->fitCsToFunction();
   endcapWithHcalCalib->fitCsToFunction();
   endcapWithHcalCalib->fitAlphasToFunction(functionEndcapHcalAlpha);
   endcapWithHcalCalib->fitAlphasToFunction();
   endcapWithHcalCalib->fitBetasToFunction(functionEndcapHcalBeta);
   endcapWithHcalCalib->fitBetasToFunction();



   //Fill all the TH2's that can be put into drawGausFit in order to produce 
   //response and resolution plots.
   for(unsigned entry = 0; entry < ETrueEnergies.size(); ++entry)
   {

      etrue = ETrueEnergies[entry];
      ecal = ecalEnergies[entry];
      hcal = hcalEnergies[entry];
      eta = etas[entry];

      if(etrue < 1.0 || ecal + hcal < 0.5) continue;
      
      if( fabs(eta) < 1.3)
      {
         uncorrectedBarrel->Fill(etrue, (ecal + hcal - etrue)/etrue);
         if( ecal > 0 && hcal > 0) 
         {
            correctedE = barrelWithEcalHcalCalib->
               getCalibratedEnergy(etrue, ecal, hcal);
            correctedEEta = barrelWithEcalHcalCalib->
               getCalibratedEnergy(etrue, ecal, hcal, eta);
            
            
            uncorrectedBarrelEcalHcal->Fill(etrue, (ecal + hcal - etrue)/
                                            etrue);
            correctedBarrelEtaEcalHcal->Fill(etrue, (correctedEEta - etrue)/
                                             etrue);
            correctedBarrelEta->Fill(etrue, (correctedEEta - etrue)/etrue);
            if(fabs(eta) > 1.0) continue;
            correctedBarrelEcalHcal->Fill(etrue, (correctedE - etrue)/etrue);

            correctedBarrel->Fill(etrue, (correctedE - etrue)/etrue);

            
      }
      else if( ecal > 0)
      {
         correctedE = barrelWithEcalHcalCalib->
            getCalibratedEnergy(etrue, ecal, hcal);
         correctedEEta = barrelWithEcalHcalCalib->
            getCalibratedEnergy(etrue, ecal, hcal, eta);

         correctedBarrelEta->Fill(etrue, (correctedEEta - etrue)/etrue);

         correctedBarrel->Fill(etrue, (correctedE - etrue)/etrue);
      } 
      else if( hcal > 0)
      {            
         correctedE = barrelWithHcalCalib->
            getCalibratedEnergy(etrue, ecal, hcal);
         correctedEEta = barrelWithHcalCalib->
            getCalibratedEnergy(etrue, ecal, hcal, eta);
         

         uncorrectedBarrelHcal->Fill(etrue, (ecal + hcal - etrue)/etrue);
         correctedBarrelEta->Fill(etrue, (correctedEEta - etrue)/etrue);
         correctedBarrelEtaHcal->Fill(etrue, (correctedEEta - etrue)/etrue);  
         if(fabs(eta) > 1.0) continue;
         correctedBarrelHcal->Fill(etrue, (correctedE - etrue)/etrue);
         correctedBarrel->Fill(etrue, (correctedE - etrue)/etrue);

         
         }
      }

      if(fabs(eta) > 1.6 && fabs(eta) < 2.8)
      {
         uncorrectedEndcap->Fill(etrue, (ecal + hcal - etrue)/etrue);

         if( ecal > 0 && hcal > 0)
         {
            
            correctedE = endcapWithEcalHcalCalib->
               getCalibratedEnergy(etrue, ecal, hcal);
            correctedEEta = endcapWithEcalHcalCalib->
               getCalibratedEnergy(etrue, ecal, hcal, eta);

            uncorrectedEndcapEcalHcal->Fill(etrue, (ecal + hcal - etrue)/
                                            etrue);
            correctedEndcapEtaEcalHcal->Fill(etrue, (correctedEEta - etrue)/
                                             etrue);    
            correctedEndcapEta->Fill(etrue, (correctedEEta - etrue)/etrue);

            if(fabs(eta) > 2.2) continue;

            correctedEndcapEcalHcal->Fill(etrue, (correctedE - etrue)/etrue);
            correctedEndcap->Fill(etrue, (correctedE - etrue)/etrue);

         }
         else if( ecal > 0)
         {
         correctedE = endcapWithEcalHcalCalib->
            getCalibratedEnergy(etrue, ecal, hcal);
         correctedEEta = endcapWithEcalHcalCalib->
            getCalibratedEnergy(etrue, ecal, hcal, eta);            

         correctedEndcapEta->Fill(etrue, (correctedEEta - etrue)/etrue);     
         if(fabs(eta) > 2.2) continue;
         correctedEndcap->Fill(etrue, (correctedE - etrue)/etrue);

         } 
         else if( hcal > 0)
         {            
         correctedE = endcapWithHcalCalib->
            getCalibratedEnergy(etrue, ecal, hcal);
         correctedEEta = endcapWithHcalCalib->
            getCalibratedEnergy(etrue, ecal, hcal, eta);
         
         uncorrectedEndcapHcal->Fill(etrue, (ecal + hcal - etrue)/etrue);
         correctedEndcapEtaHcal->Fill(etrue, (correctedEEta - etrue)/etrue);
         correctedEndcapEta->Fill(etrue, (correctedEEta - etrue)/etrue);     
         if(fabs(eta) > 2.2) continue;
         correctedEndcapHcal->Fill(etrue, (correctedE - etrue)/etrue);
         correctedEndcap->Fill(etrue, (correctedE - etrue)/etrue);
         
         }

      }

   }

   ////////////////////////////////////////////////////////////////////////////
   //Add all the draw functions that you would like here, as well as any 
   //additional output you would like.
   ////////////////////////////////////////////////////////////////////////////

}
