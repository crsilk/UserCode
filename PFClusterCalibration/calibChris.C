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
#include "TLine.h"
#include <string>
#include <iostream>
#include <math.h>


using namespace std;

///////////////////////////////////////////////////////////////////////////////
//Container class that holds all the coefficients for a particular Etrue bin
///////////////////////////////////////////////////////////////////////////////

class ABC
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

      double sigmaB_;  //Uncertainty in the constants
      double sigmaC_;

   public:
      ABC(double binLowEdge, double binHighEdge, bool isBarrel); 

      bool addEntry(double ETrueEnergy, double ecalEnergy, double hcalEnergy,
                    double eta);  //Adds an event to the ETrue bin
      double getBinLowEdge();
      double getBinHighEdge();
      bool isBarrel();  //Checks if it is a barrel-type constant storage
      bool isEmpty();   //Checks if its empty
      bool isEmptyInFitRange();   //Checks if its empty in eta fit range
      unsigned getSize();        //Returns the various stored variables in the
      double getETrueAverage();  //ABC object
      double getETrueRMS();
      double getA();
      double getB();
      double getC();
      double getSigmaB();
      double getSigmaC();
       
      double getETrue(unsigned i);
      double getEcal(unsigned i); //Returns b*ecal for entry i
      double getHcal(unsigned i); //Returns c*hcal for entry i
      double getEta(unsigned i);
      double getNEntries();

      void computeETrueAverage();  //Computes the various calibration constants
      void computeETrueRMS();      //and other stored elements in the object.
      void computeA(double a);     //Right now the constant "a" is not computed
      void computeB();             //but rather just set.
      void computeC();
      bool computeBC();
      void clear();
};

ABC::ABC(double binLowEdge, double binHighEdge, 
                                     bool isBarrel) 

{
   binLowEdge_ = binLowEdge;
   binHighEdge_ = binHighEdge;
   isBarrel_ = isBarrel;

   if(isBarrel_)
   {
      etaMinFit_ = 0.0;
      etaMaxFit_ = 1.0;
      etaMinEtaFit_ = 0.0;
      etaMaxEtaFit_ = 1.3;
   }
   else
   {
      etaMinFit_ = 1.6;
      etaMaxFit_ = 2.2;
      etaMinEtaFit_ = 1.6;
      etaMaxEtaFit_ = 2.8;
   }
   
   a_ = 0;
   b_ = 0;
   c_ = 0;
   ETrueAverage_ = 0;
   ETrueRMS_ = 0;
   sigmaB_ = 0;
   sigmaC_ = 0;
}
bool ABC::addEntry(double ETrue, double ecalEnergy, double hcalEnergy, double eta)
{

   
   double sigmaEcalHcal;

   if(isBarrel_) 
      sigmaEcalHcal = sqrt(0.08*0.08 + 1.04*1.04*(std::max(ecalEnergy + 
                                                           hcalEnergy, 1.0)));
   else
      sigmaEcalHcal = sqrt(0.04*0.04 + 1.80*1.80*(std::max(ecalEnergy + 
                                                           hcalEnergy, 1.0)));

   if((ecalEnergy + hcalEnergy + a_ - ETrue) < -5.0*sigmaEcalHcal || 
      (ecalEnergy + hcalEnergy) < 0.5 || ETrue < 1.0 || ETrue < binLowEdge_ || 
      ETrue> binHighEdge_ || fabs(eta) > etaMaxFit_ || fabs(eta) < etaMinFit_ )
      return false;


   ETrueEnergies_.push_back(ETrue);
   ecalEnergies_.push_back(ecalEnergy);
   hcalEnergies_.push_back(hcalEnergy);
   etas_.push_back(eta);
   sigmaEcalHcal_.push_back(sigmaEcalHcal);

   return true;
}

double ABC::getBinLowEdge() {return binLowEdge_;}
double ABC::getBinHighEdge() {return binHighEdge_;}
double ABC::getETrueAverage() {return ETrueAverage_;}
double ABC::getETrueRMS() {return ETrueRMS_;}
double ABC::getA() {return a_;}
double ABC::getB() {return b_;}
double ABC::getC() {return c_;}
double ABC::getSigmaB() {return sigmaB_;}
double ABC::getSigmaC() {return sigmaC_;}

double ABC::getETrue(unsigned i) {return ETrueEnergies_[i];}
double ABC::getEcal(unsigned i) {return ecalEnergies_[i];}
double ABC::getHcal(unsigned i) {return hcalEnergies_[i];}
double ABC::getEta(unsigned i) {return etas_[i];}
double ABC::getNEntries(){return ETrueEnergies_.size();}
bool ABC::isBarrel() {return isBarrel_;}
bool ABC::isEmpty() 
{
   if(ETrueEnergies_.size() == 0) return true;
   else return false;
}
bool ABC::isEmptyInFitRange() 
{
   for(unsigned i = 0; i < etas_.size(); i++)
   {
      if(fabs(etas_[i]) < etaMaxFit_ && fabs(etas_[i]) > etaMinFit_)
         return false;
   }
   return true;
}
unsigned ABC::getSize()
{
   return ETrueEnergies_.size();
}
void ABC::computeETrueAverage()
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
void ABC::computeETrueRMS()
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
void ABC::computeA(double a) 
{
   a_ = a; 
}
void ABC::computeB() 
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
void ABC::computeC() 
{
   double totalHcalSquared = 0;
   double totalEMinusATimesHcal = 0;
   
   for (unsigned i = 0; i < ETrueEnergies_.size(); i++)
   {
      totalHcalSquared += 2*hcalEnergies_[i]*hcalEnergies_[i]/
         (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
      totalEMinusATimesHcal += 2*(ETrueEnergies_[i] - a_)*hcalEnergies_[i]/
         (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
   }

   c_ = totalEMinusATimesHcal/totalHcalSquared;
   sigmaC_ = sqrt(1/totalHcalSquared);
}
bool ABC::computeBC()
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


   if(isInverted && sqrt(coeffs(0,0)) <  100000) //Make sure it inverted successfully (i.e. det != 0)
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

void ABC::clear()
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
   delete &sigmaB_;
   delete &sigmaC_;
}

class AlphaBeta
{
   private:
      vector<double> ETrueEnergies_;
      vector<double> ecalEnergies_;
      vector<double> hcalEnergies_;
      vector<double> etas_;
      vector<double> sigmaEcalHcal_;
      vector<double> a_;
      

      double binLowEdge_;
      double binHighEdge_;
      double etaMinFit_;
      double etaMaxFit_;
      bool isBarrel_;

      double ETrueAverage_;
      double ETrueRMS_;
      double alpha_;
      double beta_;
      double sigmaAlpha_;
      double sigmaBeta_;

   public:
      AlphaBeta(double binLowEdge, double binHighEdge, bool isBarrel);
      bool addEntry(double ETrueEnergy, double ecalEnergies, double correctedHcal, double eta);  //Adds an event to the ETrue bin
      double getBinLowEdge();
      double getBinHighEdge();
      bool isBarrel();  //Checks if it is a barrel-type constant storage
      bool isEmpty();   //Checks if its empty
      bool isEmptyInFitRange();   //Checks if its empty in eta fit range
      unsigned getSize();        //Returns the various stored variables in the
      double getETrue(unsigned i);
      double getEcal(unsigned i);
      double getHcal(unsigned i);

      double getETrueAverage();  //AlphaBeta object
      double getETrueRMS();
      double getAlpha();
      double getBeta();
      double getSigmaAlpha();
      double getSigmaBeta();

      void correctEcal(unsigned i, double b);
      void correctHcal(unsigned i, double c);
      void computeSigmaEcalHcal();
      void computeETrueAverage();  //Computes the various calibration constants
      void computeETrueRMS();      //and other stored elements in the object.
      bool computeAlphaBeta();
      void clear();
      
};

AlphaBeta::AlphaBeta(double binLowEdge, double binHighEdge, bool isBarrel) 

{
   binLowEdge_ = binLowEdge;
   binHighEdge_ = binHighEdge;
   isBarrel_ = isBarrel;

   if(isBarrel_)
   {
      etaMinFit_ = 0.0;
      etaMaxFit_ = 1.3;
   }
   else
   {
      etaMinFit_ = 1.6;
      etaMaxFit_ = 2.8;
   }
   
   alpha_ = 0;
   beta_ = 0;
   ETrueAverage_ = 0;
   ETrueRMS_ = 0;
   sigmaAlpha_ = 0;
   sigmaBeta_ = 0;
}
bool AlphaBeta::addEntry(double ETrue, double ecal, 
                         double hcal, double eta)
{
   double a = 0;

   if(isBarrel_) 
   {
    
      if(ecal > 0)
         a = 3.5;
      else
         a = 2.5;
   }
   else
   {
      if(ecal > 0)
         a = 3.5;
      else
         a = 2.5;
   }
   if((ecal + hcal) < 0.5 || ETrue < 1.0 || 
      ETrue < binLowEdge_ || ETrue > binHighEdge_ || fabs(eta) > etaMaxFit_ || 
      fabs(eta) < etaMinFit_) return false;

   a_.push_back(a);
   ETrueEnergies_.push_back(ETrue);
   ecalEnergies_.push_back(ecal);
   hcalEnergies_.push_back(hcal);
   etas_.push_back(eta);

   return true;
}

double AlphaBeta::getBinLowEdge() {return binLowEdge_;}
double AlphaBeta::getBinHighEdge() {return binHighEdge_;}
double AlphaBeta::getETrueAverage() {return ETrueAverage_;}
double AlphaBeta::getETrueRMS() {return ETrueRMS_;}
double AlphaBeta::getAlpha() {return alpha_;}
double AlphaBeta::getBeta() {return beta_;}
double AlphaBeta::getSigmaAlpha() {return sigmaAlpha_;}
double AlphaBeta::getSigmaBeta() {return sigmaBeta_;}
bool AlphaBeta::isBarrel() {return isBarrel_;}
bool AlphaBeta::isEmpty() 
{
   if(ETrueEnergies_.size() == 0) return true;
   else return false;
}
bool AlphaBeta::isEmptyInFitRange() 
{
   for(unsigned i = 0; i < etas_.size(); i++)
   {
      if(fabs(etas_[i]) < etaMaxFit_ && fabs(etas_[i]) > etaMinFit_)
      {
         return false;
      }
   }
   return true;
}
unsigned AlphaBeta::getSize()
{
   return ETrueEnergies_.size();
}
double AlphaBeta::getETrue(unsigned i){return ETrueEnergies_[i];}
double AlphaBeta::getEcal(unsigned i){return ecalEnergies_[i];}
double AlphaBeta::getHcal(unsigned i){return hcalEnergies_[i];}
void AlphaBeta::correctEcal(unsigned i, double b)
{
   ecalEnergies_[i] = b* ecalEnergies_[i];
}
void AlphaBeta::correctHcal(unsigned i, double c)
{
   hcalEnergies_[i] = c* hcalEnergies_[i];
}
void AlphaBeta::computeSigmaEcalHcal()
{
   double sigmaEcalHcal;
   double ecal;
   double hcal;
   double a;
   vector<bool> erase;
   

   for(unsigned entry = 0; entry < ETrueEnergies_.size(); entry++)
   {
      ecal = ecalEnergies_[entry];
      hcal = hcalEnergies_[entry];
      a = a_[entry];
      erase.push_back(false);
      
      if(isBarrel_)
         sigmaEcalHcal = sqrt(0.08*0.08 + 1.04*1.04*
                              (std::max(ecal + hcal, 1.0)));
      else
         sigmaEcalHcal = sqrt(0.04*0.04 + 1.80*1.80*
                              (std::max(ecal + hcal, 1.0)));
      
      sigmaEcalHcal_.push_back(sigmaEcalHcal);
         
   }


}
void AlphaBeta::computeETrueAverage()
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
void AlphaBeta::computeETrueRMS()
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



bool AlphaBeta::computeAlphaBeta()
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
         if ((ecalEnergies_[i] + hcalEnergies_[i] - ETrueEnergies_[i] + a_[i]) 
             < -5.*sigmaEcalHcal_[i]) continue;   
         if(hcalEnergies_[i] == 0)continue;
         
         coeffs(0, 0) += 2.0*ecalEnergies_[i]*ecalEnergies_[i]/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         coeffs(0, 1) += 2.0*etaPow[i]*ecalEnergies_[i]*ecalEnergies_[i]/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         coeffs(1, 0) += 2.0*etaPow[i]*ecalEnergies_[i]*ecalEnergies_[i]/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         coeffs(1, 1) += 2.0*etaPow[i]*etaPow[i]*ecalEnergies_[i]*
            ecalEnergies_[i]/(sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         consts(0) += 2.0*(ETrueEnergies_[i] - a_[i] - ecalEnergies_[i] - 
             hcalEnergies_[i])*ecalEnergies_[i]/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);  
         
         consts(1) += 2.0*etaPow[i]*
            (ETrueEnergies_[i] - a_[i] - ecalEnergies_[i] - 
             hcalEnergies_[i])*ecalEnergies_[i]/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
      }
   }
   else
   {

      factor = 0.5; //A factor put in by hand to make the eta dependence agree
                    //better. Should fit for the value later.
      for(unsigned i = 0; i < etas_.size(); i++) 
         etaPow.push_back((fabs(etas_[i]) - 1.5)*(fabs(etas_[i]) - 1.5)*
                          (fabs(etas_[i]) - 1.5)*(fabs(etas_[i]) - 1.5));
      
      for(unsigned i = 0; i < etas_.size(); ++i)
      {  
         if ((ecalEnergies_[i] + hcalEnergies_[i] - ETrueEnergies_[i] + a_[i]) < -5.*sigmaEcalHcal_[i]) continue;             
         if(hcalEnergies_[i] == 0)continue;
         
         coeffs(0, 0) += 2.0*
            (factor*ecalEnergies_[i] + hcalEnergies_[i])*
            (factor*ecalEnergies_[i] + hcalEnergies_[i])/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         coeffs(0, 1) += 2.0*etaPow[i]*
            (factor*ecalEnergies_[i] + hcalEnergies_[i])*
            (factor*ecalEnergies_[i] + hcalEnergies_[i])/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         coeffs(1, 0) += 2.0*etaPow[i]*
            (factor*ecalEnergies_[i] + hcalEnergies_[i])*
            (factor*ecalEnergies_[i] + hcalEnergies_[i])/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         coeffs(1, 1) += 2.0*etaPow[i]*etaPow[i]*
            (factor*ecalEnergies_[i] + hcalEnergies_[i])*
            (factor*ecalEnergies_[i] + hcalEnergies_[i])/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
         
         consts(0) += 2.0*
            (ETrueEnergies_[i] - a_[i] - ecalEnergies_[i] - 
             hcalEnergies_[i])*
            (factor*ecalEnergies_[i] + hcalEnergies_[i])/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);  
         
         consts(1) += 2.0*etaPow[i]*
            (ETrueEnergies_[i] - a_[i] - ecalEnergies_[i] - 
             hcalEnergies_[i])*
            (factor*ecalEnergies_[i] + hcalEnergies_[i])/
            (sigmaEcalHcal_[i]*sigmaEcalHcal_[i]);
      }
   }

   //Create the matrix that will be inverted and the vector which will multiply
   //that matrix to find the alpha and beta calibration constants.


   isInverted = coeffs.Invert();
   
   if(isInverted && sqrt(coeffs(0,0)) < 100000)
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

void AlphaBeta::clear()
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
   delete &alpha_;
   delete &beta_;
   delete &sigmaAlpha_;
   delete &sigmaBeta_;
}








///////////////////////////////////////////////////////////////////////////////
//The class that holds the calibration information over all values of ETrue. 
//It takes in a collection of ABC objects and fits each calibration 
//constant to a function. These functions are then used to find the calibrated
//energy.
///////////////////////////////////////////////////////////////////////////////
class Calibration
{
   private:
      double ETrueMax_;
      bool isBarrel_;
      
      vector<double> ETrueMeansABC_;
      vector<double> ETrueRMSsABC_;
      vector<double> ETrueMeansAlphaBeta_;
      vector<double> ETrueRMSsAlphaBeta_;

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
      TF1* functionA_;
      TF1* functionB_;
      TF1* functionC_;
      TF1* functionAlpha_;
      TF1* functionBeta_;
      
   public:
      Calibration();
      Calibration(double ETrueMax, bool isBarrel);
      //Adds a graph point by hand, i.e. putting in the individual values.
      void addGraphPoints(double ETrueAverage, double ETrueRMS,
                          double a, double b, double sigmaB, double C,
                          double sigmaC, double alpha, double sigmaAlpha,
                          double beta, double sigmaBeta); 
      //Adds a graph point by taking apart an ABC object.
      void addGraphPoints(ABC* abc);
      void addGraphPoints( AlphaBeta* alphabeta);
      //Creates the graphs after the points have been added.
      void initializeGraphs(string option);  
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
   ETrueMeansABC_.push_back(ETrueAverage);
   ETrueRMSsABC_.push_back(ETrueRMS);
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
void Calibration::addGraphPoints(ABC* abc)
{
   if(abc->isEmpty() || (abc->getSigmaB() == 0 && abc->getB() == 0)) return;

   ETrueMeansABC_.push_back(abc->getETrueAverage());
   ETrueRMSsABC_.push_back(abc->getETrueRMS());
   as_.push_back(abc->getA());
   bs_.push_back(abc->getB());
   cs_.push_back(abc->getC());

   sigmaBs_.push_back(abc->getSigmaB());
   sigmaCs_.push_back(abc->getSigmaC());
}

void Calibration::addGraphPoints(AlphaBeta* alphabeta)
{
   if(alphabeta->isEmpty()|| 
      (alphabeta->getSigmaBeta() == 0 && alphabeta->getBeta() == 0)) return;


   ETrueMeansAlphaBeta_.push_back(alphabeta->getETrueAverage());
   ETrueRMSsAlphaBeta_.push_back(alphabeta->getETrueRMS());
   alphas_.push_back(alphabeta->getAlpha());
   betas_.push_back(alphabeta->getBeta());
   sigmaAlphas_.push_back(alphabeta->getSigmaAlpha());
   sigmaBetas_.push_back(alphabeta->getSigmaBeta());


}
void Calibration::initializeGraphs(string option)
{
   vector<double> x;
   vector<double> sigmaX;
   vector<double> y;
   vector<double> sigmaY;
   
   if(option == "abc" || option == "ABC" || option == "all")
   {
      for(unsigned i = 0; i < ETrueMeansABC_.size(); i++)
      {
         if( bs_[i] == 0 && sigmaBs_[i] == 0.0) continue;
         
         x.push_back(ETrueMeansABC_[i]);
         sigmaX.push_back(ETrueRMSsABC_[i]);
         y.push_back(bs_[i]);
         sigmaY.push_back(sigmaBs_[i]);
         
      }
      
      graphB_ = new TGraphErrors(x.size(), &x[0], &y[0], &sigmaX[0], 
                                 &sigmaY[0]);
      
      x.clear();
      sigmaX.clear();
      y.clear();
      sigmaY.clear();
      
      for(unsigned i = 0; i < ETrueMeansABC_.size(); i++)
      {
         if( cs_[i] == 0 && sigmaCs_[i] == 0.0) continue;
         
         x.push_back(ETrueMeansABC_[i]);
         sigmaX.push_back(ETrueRMSsABC_[i]);
         y.push_back(cs_[i]);
         sigmaY.push_back(sigmaCs_[i]);
         
      }
      
      graphC_ = new TGraphErrors(x.size(), &x[0], &y[0], &sigmaX[0], 
                                 &sigmaY[0]);
      
      x.clear();
      sigmaX.clear();
      y.clear();
      sigmaY.clear();
   }
   if(option == "AlphaBeta" || option == "alphabeta" || 
      option == "alphaBeta" || option == "ALPHABETA" || option == "all")
   {      
      for(unsigned i = 0; i < ETrueMeansAlphaBeta_.size(); i++)
      {
         if( alphas_[i] == 0 && sigmaAlphas_[i] == 0.0) continue;
         
         x.push_back(ETrueMeansAlphaBeta_[i]);
         sigmaX.push_back(ETrueRMSsAlphaBeta_[i]);
         y.push_back(alphas_[i]);
         sigmaY.push_back(sigmaAlphas_[i]);
         
      }

      graphAlpha_ = new TGraphErrors(x.size(), &x[0], &y[0], &sigmaX[0], 
                                  &sigmaY[0]);
      x.clear();
      sigmaX.clear();
      y.clear();
      sigmaY.clear();
      
      for(unsigned i = 0; i < ETrueMeansAlphaBeta_.size(); i++)
      {
         if( betas_[i] == 0 && sigmaBetas_[i] == 0.0) continue;

         x.push_back(ETrueMeansAlphaBeta_[i]);
         sigmaX.push_back(ETrueRMSsAlphaBeta_[i]);
         y.push_back(betas_[i]);
         sigmaY.push_back(sigmaBetas_[i]);
         
      }
   
      graphBeta_ = new TGraphErrors(x.size(), &x[0], &y[0], &sigmaX[0], 
                                    &sigmaY[0]);
      
      x.clear();
      sigmaX.clear();
      y.clear();
      sigmaY.clear();
   }
   
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
      counterAlpha = alpha;
      counterBeta = beta;
   }
   else 
   {
      etaPow = (fabs(eta) - 1.5)*(fabs(eta) - 1.5)*(fabs(eta) - 1.5)*
         (fabs(eta) - 1.5);
      factor = 0.5;
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
   graphB_->Fit(functionB_->GetName(), "Q", "", 1.5, ETrueMax_);
   return true;
}
bool Calibration::fitBsToFunction()
{
   graphB_->Fit(functionB_->GetName(), "Q", "", 1.5, ETrueMax_);
   return true;
}
bool Calibration::fitCsToFunction(TF1 *functionC)
{
   functionC_ = functionC;
   graphC_->Fit(functionC_->GetName(), "Q", "", 1.5, ETrueMax_);
   return true;
}
bool Calibration::fitCsToFunction()
{
   graphC_->Fit(functionC_->GetName(), "Q", "", 1.5, ETrueMax_);
   return true;
}
bool Calibration::fitAlphasToFunction(TF1 *functionAlpha)
{
   functionAlpha_ = functionAlpha;
   graphAlpha_->Fit(functionAlpha_->GetName(), "Q", "", 2.0, ETrueMax_);
   return true;
}
bool Calibration::fitAlphasToFunction()
{
   graphAlpha_->Fit(functionAlpha_->GetName(), "Q", "", 1.5, ETrueMax_);
   return true;
}
bool Calibration::fitBetasToFunction(TF1 *functionBeta)
{
   functionBeta_ = functionBeta;
   graphBeta_->Fit(functionBeta_->GetName(), "Q", "", 2.0, ETrueMax_);
   return true;
}
bool Calibration::fitBetasToFunction()
{
   graphBeta_->Fit(functionBeta_->GetName(), "Q", "", 2.0, ETrueMax_);
   return true;
}
void Calibration::drawCoeffGraph(string graph)
{
  
   TCanvas* canvas = new TCanvas();
   TH2F* histo = new TH2F("histo", "", 100, 0, 1000, 100,  -2.0, 2.0); 

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
      graphB_->SetMarkerSize(1);
      graphB_->Draw("P");
      canvas->SaveAs("BCoefficient.gif");
   }
   else if(graph == "c" || graph == "C") 
   {
      histo->SetTitle("C vs True Energy");
      graphC_->SetMarkerStyle(22);
      graphC_->SetMarkerSize(1);
      graphC_->SetMarkerColor(2);
      graphC_->Draw("P");
      canvas->SaveAs("CCoefficient.gif");
   }
   else if(graph == "alpha" || graph == "Alpha") 
   {
      histo->SetTitle("Alpha vs True Energy");
      graphAlpha_->SetMarkerStyle(22);
      graphAlpha_->SetMarkerSize(1);
      graphAlpha_->SetMarkerColor(4);
      graphAlpha_->Draw("P");
      canvas->SaveAs("AlphaCoefficient.gif");
   }
   else if(graph == "beta" || graph == "Beta") 
   {
      histo->SetTitle("Beta vs True Energy");
      graphBeta_->SetMarkerStyle(22);
      graphBeta_->SetMarkerSize(1);
      graphBeta_->SetMarkerColor(3);
      graphBeta_->Draw("P");
      canvas->SaveAs("BetaCoefficient.gif");
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
      cout<<cs_[i]<<endl;
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
      cout<<"Alphas: "<<alphas_[i]<<endl;;
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
   TLine *line = new TLine(0.0,0.0,500,0.0);

   TH2F* respHisto = new TH2F("respHisto", "", 100, 0, 500, 100, -1.0, 1.0);
   TH2F* resoHisto = new TH2F("resoHisto", "", 100, 0, 500, 100, 0.0, 0.5);

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
         if(ETrueBin.back()->GetEntries() > 0)
         {
            //Fit each ETrue bin to a gaus (iteratively done to get better fit)
            ETrueBin.back()->Fit("gaus", "Q", "", -1.0, 1.0);
            gaus = ETrueBin.back()->GetFunction("gaus");
            
            ETrueBin.back()->Fit("gaus", "Q", "",
                                 gaus->GetParameter(1) - 2*gaus->
                                 GetParameter(2), 1.0);  
            gaus = ETrueBin.back()->GetFunction("gaus");

            ETrueBin.back()->Fit("gaus", "Q", "",
                                 gaus->GetParameter(1) - 2*gaus->
                                 GetParameter(2), 1.0);
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
   response.SetMarkerColor(4);

   resolution.SetMarkerStyle(22);
   resolution.SetMarkerSize(0.8);
   resolution.SetMarkerColor(4);

   averages.SetMarkerStyle(22);
   averages.SetMarkerSize(0.8);
   averages.SetMarkerColor(4);

   rmss.SetMarkerStyle(22);
   rmss.SetMarkerSize(0.8);
   rmss.SetMarkerColor(4);

   line->SetLineStyle(1);
   line->SetLineWidth(2);
   line->SetLineColor(2);



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
   line->Draw();

   canvas->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   resoHisto->SetStats(0);
   resoHisto->SetTitle("Resolution");
   resoHisto->Draw();
   resolution.Draw("P");
}

void drawEtaDependence(TH2F* inHisto, TGraph& responseEta)
{
   if(inHisto->GetEntries() == 0) return;

   vector<TH1F*> etaBin;
   TF1* gaus; 
   string name;
   char num[3];

   TCanvas* canvas;
   TCanvas* temp = new TCanvas();
   TLine* line = new TLine(-3, 0, 3, 0);

   TH2F* respHisto = new TH2F("respHisto", "", 60, -3.0, 3.00, 100, -1.0, 1.0);

   TGraph averages;
   TGraph rmss;

   vector<double> etaAverage;
   vector<double> gausMean; 
   vector<double> gausSigma;
   vector<double> average;
   vector<double> etaRms;
   
   
   temp->cd();//This TCanvas is only used since when we do the fit down below 
              //it creates an unwanted TCanvas. We will get rid of it later on 
              //in the function.

   
   for(unsigned bin = 1; bin < (unsigned)inHisto->GetNbinsX(); bin = bin + 1)
   {
      name = "hist";
      sprintf(num, "%i", bin);
      name += num;
      //Split up the TH2 into many TH1's for each eta bin.
   
      etaBin.push_back((TH1F*)inHisto->ProjectionY(name.c_str(),bin, bin + 1));
      
      if(etaBin.back()->GetEntries() > 0)
      {
         //Fit each eta bin to a gaus (iteratively done to get better fit)

         etaBin.back()->Fit("gaus", "Q", "", -1.0, 1.0);
         gaus = etaBin.back()->GetFunction("gaus");
         
         etaBin.back()->Fit("gaus", "Q", "",
                            gaus->GetParameter(1) - 2*gaus->
                            GetParameter(2), 1.0);  
         gaus = etaBin.back()->GetFunction("gaus");
         
         etaBin.back()->Fit("gaus", "Q", "",
                            gaus->GetParameter(1) - 2*gaus->
                            GetParameter(2), 1.0);
         gaus = etaBin.back()->GetFunction("gaus");
         
         etaAverage.push_back(inHisto->GetBinCenter(bin));
         etaRms.push_back(0.1);
         gausMean.push_back(etaBin.back()->GetMean());
         gausSigma.push_back(etaBin.back()->GetRMS());


      }
      
      
      //Increase bin size with increasing eta since there are fewer high 
      //energy events than low energy ones.
   }
   responseEta = TGraph(etaAverage.size(), &etaAverage[0], &gausMean[0]); 
//&etaRms[0], &gausSigma[0]); 

   responseEta.SetMarkerStyle(22);
   responseEta.SetMarkerSize(1);
   responseEta.SetMarkerColor(4);   

   line->SetLineStyle(1);
   line->SetLineWidth(2);
   line->SetLineColor(2);

   canvas = new TCanvas("canvas", "Response and Resolution", 
                        1000, 500);

   
   temp->~TCanvas();  //destroy the TCanvas 
   
   canvas->cd();
   canvas->SetFillColor(0);

   gPad->SetGridx();
   gPad->SetGridy();
   respHisto->SetStats(0);
   respHisto->SetTitle("Response");
   respHisto->Draw();
   responseEta.Draw("P");
   line->Draw();   
}

void drawCompare(TGraph& response1, TGraph& response2, TGraph& resolution1, TGraph& resolution2)
{
   

   TCanvas* Compare = new TCanvas("Compare" ,"", 1000, 500);
   TH2F * respHisto = new TH2F("respHisto", "", 100, 0, 1000, 100, -1, 1);
   TH2F * resoHisto = new TH2F("resoHisto", "", 100, 0, 1000, 100, 0, 1);
   TLegend * legend1 = new TLegend(0.75, 0.75, 0.95, 0.9);
   TLegend * legend2 = new TLegend(0.75, 0.75, 0.95, 0.9);

   response1.SetMarkerColor(4);
   response1.SetMarkerStyle(22);
   response1.SetMarkerSize(0.8);

   resolution1.SetMarkerColor(4);
   resolution1.SetMarkerStyle(22);
   resolution1.SetMarkerSize(0.8);

   response2.SetMarkerColor(2);
   response2.SetMarkerStyle(22);
   response2.SetMarkerSize(0.8);

   resolution2.SetMarkerColor(2);
   resolution2.SetMarkerStyle(22);
   resolution2.SetMarkerSize(0.8);
   
   legend1->AddEntry(&response1, "Raw");
   legend1->AddEntry(&response2, "Corrected");
   legend2->AddEntry(&resolution1, "Raw");
   legend2->AddEntry(&resolution2, "Corrected");

   Compare->Divide(2,1);
  
   Compare->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   respHisto->SetStats(0);
   respHisto->SetTitle("Response");
   respHisto->Draw();
   response1.Draw("P");
   response2.Draw("P");
   legend1->Draw();

   Compare->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   resoHisto->SetStats(0);
   resoHisto->SetTitle("Resolution");
   resoHisto->Draw();
   resolution1.Draw("P");
   resolution2.Draw("P");
   legend2->Draw();

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

vector<ABC*> barrelABCEcalHcal; //Vectors of the ABC objects
vector<ABC*> barrelABCEcal;     //which hold all the calibration 
vector<ABC*> barrelABCHcal;     //constants for an individual bin.
vector<ABC*> endcapABCEcalHcal;
vector<ABC*> endcapABCEcal;
vector<ABC*> endcapABCHcal;

vector<AlphaBeta*> barrelAlphaBeta; //Vectors of the AlphaBeta objects which 
vector<AlphaBeta*> endcapAlphaBeta; //hold all the calibration constants for 
                                    //an individual bin.

TF1* functionBarrelEcalHcalA;     //Functions that the calibration equations
TF1* functionBarrelEcalHcalB;     //are fit to
TF1* functionBarrelEcalHcalC;  
TF1* functionEndcapEcalHcalA;
TF1* functionEndcapEcalHcalB;
TF1* functionEndcapEcalHcalC;  

TF1* functionBarrelHcalA;
TF1* functionBarrelHcalB;
TF1* functionBarrelHcalC;  
TF1* functionEndcapHcalA;
TF1* functionEndcapHcalB;
TF1* functionEndcapHcalC;  

TF1* functionBarrelAlpha;
TF1* functionBarrelBeta;
TF1* functionEndcapAlpha;
TF1* functionEndcapBeta;

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
double b;
double c;
double etrueMax;
double barrelEcalHcalB;
double barrelEcalHcalC;
double barrelHcalC;
double barrelAlpha;
double barrelBeta;
double endcapEcalHcalB;
double endcapEcalHcalC;
double endcapHcalC;
double endcapAlpha;
double endcapBeta;
double correctedE;
double correctedEta;

const char* functionEndcapEcalHcalB_e;
const char* functionEndcapEcalHcalC_e;
const char* functionEndcapHcalC_e;
const char* functionEndcapAlpha_e;
const char* functionEndcapBeta_e;
const char* functionBarrelEcalHcalB_e;
const char* functionBarrelEcalHcalC_e;
const char* functionBarrelHcalC_e;
const char* functionBarrelAlpha_e;
const char* functionBarrelBeta_e;


//All the differenct types of TH2's that will be filled in order to make 
//resolution and response plots
TH2F* raw = new TH2F("raw","", 1000, 0, 1000, 150, -1.5, 1.5);
TH2F* corrEta = new TH2F("corrEta", "", 1000, 0, 1000, 150, -1.5, 1.5);


TH2F* rawBarrel = new TH2F("rawBarrel","", 1000, 0, 1000, 150, -1.5, 1.5);
TH2F* corrBarrel = new TH2F("corrBarrel", "", 1000, 0, 1000, 150, -1.5, 1.5);
TH2F* corrEtaBarrel = new TH2F("corrEtaBarrel", "", 1000, 0, 1000, 150, -1.5, 
                               1.5);
TH2F* rawBarrelEcalHcal = new TH2F("rawBarrelEcalHcal","", 1000, 0, 1000, 150, 
                                   -1.5, 1.5);
TH2F* corrBarrelEcalHcal = new TH2F("corrBarrelEcalHcal", "", 1000, 0, 1000, 
                                    150, -1.5, 1.5);
TH2F* corrEtaBarrelEcalHcal = new TH2F("corrEtaBarrelEcalHcal", "", 1000, 0, 
                                       1000, 150, -1.5, 1.5);
TH2F* rawBarrelHcal = new TH2F("rawBarrelHcal","", 1000, 0, 1000, 150, -1.5, 
                               1.5);
TH2F* corrBarrelHcal = new TH2F("corrBarrelHcal", "", 1000, 0, 1000, 150, -1.5,
                                1.5);
TH2F* corrEtaBarrelHcal = new TH2F("corrEtaBarrelHcal", "", 1000, 0, 1000, 150,
                                   -1.5, 1.5);

TH2F* rawEndcap = new TH2F("rawEndcap","", 1000, 0, 1000, 150, -1.5, 1.5);
TH2F* corrEndcap = new TH2F("corrEndcap", "", 1000, 0, 1000, 150, -1.5, 1.5);
TH2F* corrEtaEndcap = new TH2F("corrEtaEndcap", "", 1000, 0, 1000, 150, -1.5, 
                               1.5);
TH2F* rawEndcapEcalHcal = new TH2F("rawEndcapEcalHcal","", 1000, 0, 1000, 150, 
                                   -1.5, 1.5);
TH2F* corrEndcapEcalHcal = new TH2F("corrEndcapEcalHcal", "", 1000, 0, 1000, 
                                    150, -1.5, 1.5);
TH2F* corrEtaEndcapEcalHcal = new TH2F("corrEtaEndcapEcalHcal", "", 1000, 0, 
                                       1000, 150, -1.5, 1.5);
TH2F* rawEndcapHcal = new TH2F("rawEndcapHcal","", 1000, 0, 1000, 150, -1.5, 
                               1.5);
TH2F* corrEndcapHcal = new TH2F("corrEndcapHcal", "", 1000, 0, 1000, 150, -1.5,
                                1.5);
TH2F* corrEtaEndcapHcal = new TH2F("corrEtaEndcapHcal", "", 1000, 0, 1000, 150,
                                   -1.5, 1.5);

TH2F * rawEtaDependence = new TH2F("rawEtaDependence","Response vs. Eta", 300, -3.0, 3.0, 150, -1.0,1.0 );
TH2F * corrEtaDependence = new TH2F("corrEtaDependence","Response vs. Eta", 300, -3.0, 3.0, 150, -1.0,1.0 );

TH1F * trueTempHisto = new TH1F("trueTempHisto", "true", 100, 0, 100);
TH1F * ecalTempHisto = new TH1F("ecalTempHisto", "ecal", 100, 0, 100);

//Temporary TGraphs to passed drawGausFit
TGraph response;
TGraph resolution;
TGraph responseRaw;
TGraph resolutionRaw;
TGraph responseEta;

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
   //inputFile = TFile::Open("IsolatedChargedHadronsFromQCD.root");
   inputFile = TFile::Open("pfcalibTestTag_all.root");
   sTree = (TTree*)inputFile->Get("s;1");
   getValuesFromTree(sTree, ETrueEnergies, ecalEnergies, 
                     hcalEnergies, etas, phis);

   
   //Create all the ABC objects you need with increasing bin size
   //since there are fewer events at higher energies. 
   cout<<"Creating abc and alphabeta objects..."<<endl;
   for(double bin = 0.0; bin < 10.0; bin = bin + 1.0)
   {
      barrelABCEcalHcal.push_back(new ABC(bin, bin + 1.0, true));
      barrelABCEcal.push_back(new ABC(bin, bin + 1.0, true));
      barrelABCHcal.push_back(new ABC(bin, bin + 1.0, true));
      endcapABCEcalHcal.push_back(new ABC(bin, bin + 1.0, false));
      endcapABCEcal.push_back(new ABC(bin, bin + 1.0,false));
      endcapABCHcal.push_back(new ABC(bin, bin + 1.0, false));

   }
   for(double bin = 10.0; bin < 100.0 ; bin = bin + 2.0)
   {
      barrelABCEcalHcal.push_back(new ABC(bin, bin + 2.0, true));
      barrelABCEcal.push_back(new ABC(bin, bin + 2.0, true));
      barrelABCHcal.push_back(new ABC(bin, bin + 2.0, true));
      endcapABCEcalHcal.push_back(new ABC(bin, bin + 2.0, false));
      endcapABCEcal.push_back(new ABC(bin, bin + 2.0,false));
      endcapABCHcal.push_back(new ABC(bin, bin + 2.0, false));

   }
   
   for(double bin = 100.0; bin < 1000.0 ; bin = bin + 10)
   {
      barrelABCEcalHcal.push_back(new ABC(bin, bin + 10.0, true));
      barrelABCEcal.push_back(new ABC(bin, bin + 10.0, true));
      barrelABCHcal.push_back(new ABC(bin, bin + 10.0, true));
      endcapABCEcalHcal.push_back(new ABC(bin, bin + 10.0, false));
      endcapABCEcal.push_back(new ABC(bin, bin + 10.0,false));
      endcapABCHcal.push_back(new ABC(bin, bin + 10.0, false));  

   }
   for(double bin = 0.0; bin < 10.0; bin = bin + 2.0)
   {
      barrelAlphaBeta.push_back(new AlphaBeta(bin, bin + 2.0, true));
      endcapAlphaBeta.push_back(new AlphaBeta(bin, bin + 2.0, false));
   }
   for(double bin = 10.0; bin < 100.0 ; bin = bin + 10.0)
   {
      barrelAlphaBeta.push_back(new AlphaBeta(bin, bin + 10.0, true));
      endcapAlphaBeta.push_back(new AlphaBeta(bin, bin + 10.0, false));
   }
   
   for(double bin = 100.0; bin < 1000.0 ; bin = bin + 50)
   {
      barrelAlphaBeta.push_back(new AlphaBeta(bin, bin + 50.0, true));
      endcapAlphaBeta.push_back(new AlphaBeta(bin, bin + 50.0, false));
   }
   //Fill all the ABC Objects with their respective events. They are all 
   //divided up into the six possible case ( (endcap or barrel)x(ecalhcal or 
   //ecal or hcal))
   
   cout<<"Filling abc objects..."<<endl;
   for( unsigned bin = 0; bin < barrelABCEcal.size(); ++bin)
   {
      barrelABCEcalHcal[bin]->computeA(3.5);
      barrelABCEcal[bin]->computeA(3.5);
      barrelABCHcal[bin]->computeA(2.5);

      endcapABCEcalHcal[bin]->computeA(3.5);
      endcapABCEcal[bin]->computeA(3.5);
      endcapABCHcal[bin]->computeA(2.5);


      for(unsigned entry = 0; entry < ETrueEnergies.size(); entry++)
      {
         etrue = ETrueEnergies[entry];
         ecal = ecalEnergies[entry];
         hcal = hcalEnergies[entry];
         eta = etas[entry];

         if(hcal == 0.0) continue;

         if( ecal > 0.0 && hcal > 0.0)
         {
            barrelABCEcalHcal[bin]->addEntry(etrue, ecal, hcal, eta);
            endcapABCEcalHcal[bin]->addEntry(etrue, ecal, hcal, eta);
            
         }
         else if(ecal > 0.0)
         {
            barrelABCEcal[bin]->addEntry(etrue, ecal, hcal ,eta);
            endcapABCEcal[bin]->addEntry(etrue, ecal, hcal ,eta);
         }
         else if(hcal > 0.0)
         {
            barrelABCHcal[bin]->addEntry(etrue, ecal, hcal, eta);
            endcapABCHcal[bin]->addEntry(etrue, ecal, hcal, eta);
         }
         
         if(bin < barrelAlphaBeta.size())
         {
         barrelAlphaBeta[bin]->addEntry(etrue, ecal, hcal, eta);
         endcapAlphaBeta[bin]->addEntry(etrue, ecal, hcal, eta);
         }
      }
   }
   
   
   //Compute the calibration constants along with their uncertainties for each
   //ETrue bin, then add their values to a Calibration object.
   cout<<"Computing a, b, c coefficients..."<<endl;

   for(unsigned bin = 2; bin < barrelABCEcalHcal.size() - 1; ++bin)
   {
      
      if(!barrelABCEcalHcal[bin]->isEmptyInFitRange())
      { 
         barrelABCEcalHcal[bin]->computeETrueAverage();
         barrelABCEcalHcal[bin]->computeETrueRMS();
         barrelABCEcalHcal[bin]->computeA(3.5);
         barrelABCEcalHcal[bin]->computeBC();
      }
      if(!barrelABCEcal[bin]->isEmptyInFitRange())
      { 
         barrelABCEcal[bin]->computeETrueAverage();
         barrelABCEcal[bin]->computeETrueRMS();
         barrelABCEcal[bin]->computeA(3.5);
         barrelABCEcal[bin]->computeB();
      }
      if(!barrelABCHcal[bin]->isEmptyInFitRange())
      { 
         barrelABCHcal[bin]->computeETrueAverage();
         barrelABCHcal[bin]->computeETrueRMS();
         barrelABCHcal[bin]->computeA(3.0);
         barrelABCHcal[bin]->computeC();
      }
      if(!endcapABCEcalHcal[bin]->isEmptyInFitRange())
      {
         endcapABCEcalHcal[bin]->computeETrueAverage();
         endcapABCEcalHcal[bin]->computeETrueRMS();
         endcapABCEcalHcal[bin]->computeA(3.5);
         endcapABCEcalHcal[bin]->computeBC();
      }
      if(!endcapABCEcal[bin]->isEmptyInFitRange())
      {
         endcapABCEcal[bin]->computeETrueAverage();
         endcapABCEcal[bin]->computeETrueRMS();
         endcapABCEcal[bin]->computeA(3.5);
         endcapABCEcal[bin]->computeB();
      }
      if(!endcapABCHcal[bin]->isEmptyInFitRange())
      {
         endcapABCHcal[bin]->computeETrueAverage();
         endcapABCHcal[bin]->computeETrueRMS();
         endcapABCHcal[bin]->computeA(2.5);
         endcapABCHcal[bin]->computeC();
      }
      

      if(!barrelABCEcalHcal[bin]->isEmpty() && 
         barrelABCEcalHcal[bin]->getBinHighEdge() >
         barrelWithEcalHcalCalib->getETrueMax())
      {
         barrelWithEcalHcalCalib->setETrueMax(
            barrelABCEcalHcal[bin]->getBinHighEdge());
      }
      if(!barrelABCEcal[bin]->isEmpty() && 
         barrelABCEcal[bin]->getBinHighEdge() >
         barrelWithEcalCalib->getETrueMax())
      {
         barrelWithEcalCalib->setETrueMax(
            barrelABCEcal[bin]->getBinHighEdge());
      }
      if(!barrelABCHcal[bin]->isEmpty() && 
         barrelABCHcal[bin]->getBinHighEdge() >
         barrelWithHcalCalib->getETrueMax())
      {
         barrelWithHcalCalib->setETrueMax(
            barrelABCHcal[bin]->getBinHighEdge());
      }
      if(!endcapABCEcalHcal[bin]->isEmpty() && 
         endcapABCEcalHcal[bin]->getBinHighEdge() >
         endcapWithEcalHcalCalib->getETrueMax())
      {
         endcapWithEcalHcalCalib->setETrueMax(
            endcapABCEcalHcal[bin]->getBinHighEdge());
      }
      if(!endcapABCEcal[bin]->isEmpty() && 
         endcapABCEcal[bin]->getBinHighEdge() >
         endcapWithEcalCalib->getETrueMax())
      {
         endcapWithEcalCalib->setETrueMax(
            endcapABCEcal[bin]->getBinHighEdge());
      }
      if(!endcapABCHcal[bin]->isEmpty() && 
         endcapABCHcal[bin]->getBinHighEdge() >
         endcapWithHcalCalib->getETrueMax())
      {
         endcapWithHcalCalib->setETrueMax(
            endcapABCHcal[bin]->getBinHighEdge());
      }
 
/////////////////////////////////////////
      if(bin == 30)
      {
         for( unsigned event = 0; event < barrelABCEcalHcal[bin]->getNEntries(); event++)
         {            
            cout<<"ETRUE: "<<barrelABCEcalHcal[bin]->getETrue(event);
            cout<<"\tEcal: "<<barrelABCEcalHcal[bin]->getEcal(event);
            cout<<"\tHcal: "<<barrelABCEcalHcal[bin]->getHcal(event)<<endl;
         }
      }
/////////////////////////////////////////
      barrelWithEcalHcalCalib->addGraphPoints(barrelABCEcalHcal[bin]); 
      barrelWithEcalCalib->addGraphPoints(barrelABCEcal[bin]); 
      barrelWithHcalCalib->addGraphPoints(barrelABCHcal[bin]); 
      endcapWithEcalHcalCalib->addGraphPoints(endcapABCEcalHcal[bin]); 
      endcapWithEcalCalib->addGraphPoints(endcapABCEcal[bin]); 
      endcapWithHcalCalib->addGraphPoints(endcapABCHcal[bin]);                 


   }
   
   cout<<"Fitting a, b, c coefficients..."<<endl;
   //Initialize all the ABC graphs in the calibration objects.
   barrelWithEcalHcalCalib->initializeGraphs("abc");
   barrelWithEcalCalib->initializeGraphs("abc");
   barrelWithHcalCalib->initializeGraphs("abc");   
   endcapWithEcalHcalCalib->initializeGraphs("abc");
   endcapWithEcalCalib->initializeGraphs("abc");
   endcapWithHcalCalib->initializeGraphs("abc");   


   //Define the functions that you will fit your ABC calibration constants to.
   functionBarrelEcalHcalA = new TF1("functionBarrelEcalHcalA","[0]", 0, 1000);
   functionBarrelEcalHcalB = new TF1("functionBarrelEcalHcalB","[0]+([1]+[2]/sqrt(x))*exp(-x/[3])-[4]*exp(-x*x/[5])", 0, 1000);
   functionBarrelEcalHcalC = new TF1("functionBarrelEcalHcalC","[0]+([1]+[2]/sqrt(x))*exp(-x/[3])-[4]*exp(-x*x/[5])", 0, 1000);
   functionEndcapEcalHcalA = new TF1("functionEndcapEcalHcalA","[0]", 0, 1000);
   functionEndcapEcalHcalB = new TF1("functionEndcapEcalHcalB","[0]+([1]+[2]/sqrt(x))*exp(-x/[3])-[4]*exp(-x*x/[5])", 0, 1000);
   functionEndcapEcalHcalC = new TF1("functionEndcapEcalHcalC","[0]+([1]+[2]/sqrt(x))*exp(-x/[3])-[4]*exp(-x*x/[5])", 0, 1000);

   functionBarrelHcalA = new TF1("functionBarrelHcalA","[0]", 0, 1000);
   functionBarrelHcalB = new TF1("functionBarrelHcalB","[0]", 0, 1000);
   functionBarrelHcalC = new TF1("functionBarrelHcalC","[0]+([1]+[2]/sqrt(x))*exp(-x/[3])-[4]*exp(-x*x/[5])", 0, 1000);
   functionEndcapHcalA = new TF1("functionEndcapHcalA","[0]", 0, 1000);
   functionEndcapHcalB = new TF1("functionEndcapHcalB","[0]", 0, 1000);
   functionEndcapHcalC = new TF1("functionEndcapHcalC","[0]+([1]+[2]/sqrt(x))*exp(-x/[3])-[4]*exp(-x*x/[5])", 0, 1000);


   //Set the parameters of the function you just defined.
   functionBarrelEcalHcalA->FixParameter(0, 3.5);
   functionBarrelEcalHcalB->SetParameters(1.10,0.12,-0.2,119,1.3,40);
   functionBarrelEcalHcalC->SetParameters(1.04,0.27,-0.6,70,1.05,24);
   functionBarrelEcalHcalC->FixParameter(4,1.0);
   functionEndcapEcalHcalA->FixParameter(0, 3.5);
   functionEndcapEcalHcalB->SetParameters(1.10,0.12,-0.2,119,1.3,80);
   functionEndcapEcalHcalB->FixParameter(4,2.0);
   functionEndcapEcalHcalC->SetParameters(1.04,0.27,-0.6,70,1.05,50);
   functionEndcapEcalHcalC->FixParameter(4,1.2);


   functionBarrelHcalA->FixParameter(0, 3.0);
   functionBarrelHcalB->FixParameter(0, 0.0);
   functionBarrelHcalC->SetParameters(0.95, 0.04, 0.07, 76, 1.23, 25);
   functionEndcapHcalA->FixParameter(0, 2.5);
   functionEndcapHcalB->FixParameter(0, 0.0);
   functionEndcapHcalC->SetParameters(0.95, 0.04, 0.07, 76, 1.23, 25);

   //Fit the functions that were defined above to the calibration graphs that
   //are stored in the calibration objects.
   barrelWithEcalHcalCalib->fitAsToFunction(functionBarrelEcalHcalA);
   barrelWithEcalHcalCalib->fitBsToFunction(functionBarrelEcalHcalB);
   barrelWithEcalHcalCalib->fitBsToFunction();
   barrelWithEcalHcalCalib->fitCsToFunction(functionBarrelEcalHcalC);
   barrelWithEcalHcalCalib->fitCsToFunction();
   barrelWithEcalHcalCalib->fitCsToFunction();
   endcapWithEcalHcalCalib->fitAsToFunction(functionEndcapEcalHcalA);
   endcapWithEcalHcalCalib->fitBsToFunction(functionEndcapEcalHcalB);
   endcapWithEcalHcalCalib->fitBsToFunction();
   endcapWithEcalHcalCalib->fitCsToFunction(functionEndcapEcalHcalC);
   endcapWithEcalHcalCalib->fitCsToFunction();
   endcapWithEcalHcalCalib->fitCsToFunction();
   
   barrelWithHcalCalib->fitAsToFunction(functionBarrelHcalA);
   barrelWithHcalCalib->fitBsToFunction(functionBarrelHcalB);
   barrelWithHcalCalib->fitBsToFunction();
   barrelWithHcalCalib->fitCsToFunction(functionBarrelHcalC);
   barrelWithHcalCalib->fitCsToFunction();
   barrelWithHcalCalib->fitCsToFunction();
   endcapWithHcalCalib->fitAsToFunction(functionEndcapHcalA);
   endcapWithHcalCalib->fitBsToFunction(functionEndcapHcalB);
   endcapWithHcalCalib->fitBsToFunction();
   endcapWithHcalCalib->fitCsToFunction(functionEndcapHcalC);
   endcapWithHcalCalib->fitCsToFunction();
   endcapWithHcalCalib->fitCsToFunction();
      
   //Here we fill up the AlphaBeta objects, compute alpha and beta, then add 
   //them to the Calibration objects. 
   cout<<"Computing alpha and beta coefficients..."<<endl;
   for(unsigned bin = 2; bin < barrelAlphaBeta.size() - 1; bin++)
   {
      for(unsigned entry = 0; entry < barrelAlphaBeta[bin]->getSize(); entry++)
      {
         
         etrue = barrelAlphaBeta[bin]->getETrue(entry);
         ecal = barrelAlphaBeta[bin]->getEcal(entry);
         hcal = barrelAlphaBeta[bin]->getHcal(entry);
         b = 1.0;
         c = 1.0;
         

         if(ecal > 0 && hcal > 0)
         {
            b = barrelWithEcalHcalCalib->getFunctionB()->Eval(etrue);
            c = barrelWithEcalHcalCalib->getFunctionC()->Eval(etrue);
         }
         else if(ecal > 0)
            b = barrelWithEcalHcalCalib->getFunctionB()->Eval(etrue);
         else if(hcal > 0)
            c = barrelWithHcalCalib->getFunctionC()->Eval(etrue);
         
         barrelAlphaBeta[bin]->correctEcal(entry, b);
         barrelAlphaBeta[bin]->correctHcal(entry, c);
      }

      for(unsigned entry = 0; entry < endcapAlphaBeta[bin]->getSize(); entry++)
      {

         etrue = endcapAlphaBeta[bin]->getETrue(entry);
         ecal = endcapAlphaBeta[bin]->getEcal(entry);
         hcal = endcapAlphaBeta[bin]->getHcal(entry);
         b = 1.0;
         c = 1.0;
         
         if(ecal > 0 && hcal > 0)
         {
            b = endcapWithEcalHcalCalib->getFunctionB()->Eval(etrue);
            c = endcapWithEcalHcalCalib->getFunctionC()->Eval(etrue);
         }
         else if(ecal > 0)
            b = endcapWithEcalHcalCalib->getFunctionB()->Eval(etrue);
         else if(hcal > 0)
            c = endcapWithHcalCalib->getFunctionC()->Eval(etrue);
         
         endcapAlphaBeta[bin]->correctEcal(entry, b);
         endcapAlphaBeta[bin]->correctHcal(entry, c);
      }
      barrelAlphaBeta[bin]->computeSigmaEcalHcal();
      barrelAlphaBeta[bin]->computeETrueAverage();
      barrelAlphaBeta[bin]->computeETrueRMS();
      
      endcapAlphaBeta[bin]->computeSigmaEcalHcal();
      endcapAlphaBeta[bin]->computeETrueAverage();
      endcapAlphaBeta[bin]->computeETrueRMS();
      if(barrelAlphaBeta[bin]->computeAlphaBeta())
      {
         barrelWithEcalHcalCalib->addGraphPoints(barrelAlphaBeta[bin]);
         barrelWithEcalCalib->addGraphPoints(barrelAlphaBeta[bin]);
         barrelWithHcalCalib->addGraphPoints(barrelAlphaBeta[bin]);
      }

      if(endcapAlphaBeta[bin]->computeAlphaBeta())
      {
         endcapWithEcalHcalCalib->addGraphPoints(endcapAlphaBeta[bin]);
         endcapWithEcalCalib->addGraphPoints(endcapAlphaBeta[bin]);
         endcapWithHcalCalib->addGraphPoints(endcapAlphaBeta[bin]);
      }
      
   }
   
   cout<<"Fitting alpha, beta coefficients..."<<endl;
   barrelWithEcalHcalCalib->initializeGraphs("alphabeta");
   barrelWithEcalCalib->initializeGraphs("alphabeta");
   barrelWithHcalCalib->initializeGraphs("alphabeta");   
   endcapWithEcalHcalCalib->initializeGraphs("alphabeta");
   endcapWithEcalCalib->initializeGraphs("alphabeta");
   endcapWithHcalCalib->initializeGraphs("alphabeta");   

   functionBarrelAlpha = new TF1("functionBarrelAlpha","[0]+[1]*exp(-x/[2])", 0, 1000);
   functionBarrelBeta = new TF1("functionBarrelBeta","[0]+[1]*exp(-x/[2])", 0, 1000);
   functionEndcapAlpha = new TF1("functionEndcapAlpha","[0]+[1]*exp(-x/[2])", 0, 1000);
   functionEndcapBeta = new TF1("functionEndcapBeta","[0]+[1]*exp(-x/[2])+[3]*[3]*exp(-x*x/([4]*[4]))",0,1000);

   functionBarrelAlpha->SetParameters(0.02, -0.1, 200);
   functionBarrelBeta->SetParameters(-0.02, 0.4, 200);
   functionEndcapAlpha->SetParameters(0.02, -0.1, 200);
   functionEndcapBeta->SetParameters(0.07, -2.5, 6.0, 0.3, 175.0);

   barrelWithEcalHcalCalib->fitAlphasToFunction(functionBarrelAlpha);
   barrelWithEcalHcalCalib->fitAlphasToFunction();
   barrelWithEcalHcalCalib->fitBetasToFunction(functionBarrelBeta);
   barrelWithEcalHcalCalib->fitBetasToFunction();
   endcapWithEcalHcalCalib->fitAlphasToFunction(functionEndcapAlpha);
   endcapWithEcalHcalCalib->fitAlphasToFunction();
   endcapWithEcalHcalCalib->fitBetasToFunction(functionEndcapBeta);
   endcapWithEcalHcalCalib->fitBetasToFunction();

   barrelWithHcalCalib->fitAlphasToFunction(functionBarrelAlpha);
   barrelWithHcalCalib->fitAlphasToFunction();
   barrelWithHcalCalib->fitBetasToFunction(functionBarrelBeta);
   barrelWithHcalCalib->fitBetasToFunction();
   endcapWithHcalCalib->fitAlphasToFunction(functionEndcapAlpha);
   endcapWithHcalCalib->fitAlphasToFunction();
   endcapWithHcalCalib->fitBetasToFunction(functionEndcapBeta);
   endcapWithHcalCalib->fitBetasToFunction();


   
   //Fill all the TH2's that can be put into drawGausFit in order to produce 
   //response and resolution plots.
   cout<<"Making response and resolution plots..."<<endl;
   for(unsigned entry = 0; entry < ETrueEnergies.size(); ++entry)
   {

      etrue = ETrueEnergies[entry];
      ecal = ecalEnergies[entry];
      hcal = hcalEnergies[entry];
      eta = abs(etas[entry]);

      if((ecal + hcal) < 0.5) continue;
      if( etrue < 1.0) continue;
      if( hcal == 0) continue;
      

      if(fabs(eta) < 1.5) //alpha beta fit range for barrel
      {     
         raw->Fill(etrue, (ecal + hcal - etrue)/etrue);
         if(ecal > 0)
         {
            correctedEta = barrelWithEcalHcalCalib->
               getCalibratedEnergy(etrue, ecal, hcal, eta);
            
            corrEta->Fill(etrue, (correctedEta - etrue)/etrue);
            corrEtaBarrel->Fill(etrue, (correctedEta - etrue)/etrue);
            corrEtaBarrelEcalHcal->Fill(etrue, (correctedEta - etrue)/etrue);


         }
         else 
         {

            correctedEta = barrelWithHcalCalib->
               getCalibratedEnergy(etrue, ecal, hcal, eta);

            corrEta->Fill(etrue, (correctedEta - etrue)/etrue);
            corrEtaBarrel->Fill(etrue, (correctedEta - etrue)/etrue);
            corrEtaBarrelHcal->Fill(etrue, (correctedEta - etrue)/etrue);

         }
         if(fabs(eta) < 1.0) //b, c fit range
         {
            rawBarrel->Fill(etrue, (ecal + hcal - etrue)/etrue);
            
            if(ecal > 0)
            {
               correctedE = barrelWithEcalHcalCalib->
                  getCalibratedEnergy(etrue, ecal, hcal);

               rawBarrelEcalHcal->Fill(etrue, (ecal + hcal - etrue)/etrue);
               corrBarrel->Fill(etrue, (correctedE - etrue)/etrue);
               corrBarrelEcalHcal->Fill(etrue, (correctedE - etrue)/etrue);
               
               rawEtaDependence->Fill(eta, (ecal + hcal - etrue)/etrue);
               corrEtaDependence->Fill(eta, (correctedEta - etrue)/etrue);


            }
            else
            {
               correctedE = barrelWithHcalCalib->
                  getCalibratedEnergy(etrue, ecal, hcal);

               rawBarrelHcal->Fill(etrue, (ecal + hcal - etrue)/etrue);
               corrBarrel->Fill(etrue, (correctedE - etrue)/etrue);
               corrBarrelHcal->Fill(etrue, (correctedE - etrue)/etrue);

               rawEtaDependence->Fill(eta, (ecal + hcal - etrue)/etrue);
               corrEtaDependence->Fill(eta, (correctedEta - etrue)/etrue);

            }
         }
      }
      
      if(fabs(eta) < 2.8 && fabs(eta) > 1.5) //alpha beta fit range for endcap
      {
         raw->Fill(etrue, (ecal + hcal - etrue)/etrue);
         if(ecal > 0)
         {
            correctedEta = endcapWithEcalHcalCalib->
               getCalibratedEnergy(etrue, ecal, hcal, eta);

            corrEta->Fill(etrue, (correctedEta - etrue)/etrue);
            corrEtaEndcap->Fill(etrue, (correctedEta - etrue)/etrue);
            corrEtaEndcapEcalHcal->Fill(etrue, (correctedEta - etrue)/etrue);
         }
         else
         {
            correctedEta = endcapWithHcalCalib->
               getCalibratedEnergy(etrue, ecal, hcal, eta);

            corrEta->Fill(etrue, (correctedEta - etrue)/etrue);            
            corrEtaEndcap->Fill(etrue, (correctedEta - etrue)/etrue);
            corrEtaEndcapHcal->Fill(etrue, (correctedEta - etrue)/etrue);

         }
         if(fabs(eta) < 2.2) //b, c fit range
         {
            rawEndcap->Fill(etrue, (ecal + hcal - etrue)/etrue);
            
            if(ecal > 0)
            {
               correctedE = endcapWithEcalHcalCalib->
                  getCalibratedEnergy(etrue, ecal, hcal);

               rawEndcapEcalHcal->Fill(etrue, (ecal + hcal - etrue)/etrue);
               corrEndcap->Fill(etrue, (correctedE - etrue)/etrue);
               corrEndcapEcalHcal->Fill(etrue, (correctedE - etrue)/etrue);

               rawEtaDependence->Fill(eta, (ecal + hcal - etrue)/etrue);
               corrEtaDependence->Fill(eta, (correctedEta - etrue)/etrue);

            }
            else 
            {
               correctedE = endcapWithHcalCalib->
                  getCalibratedEnergy(etrue, ecal, hcal);
               
               rawEndcapHcal->Fill(etrue, (ecal + hcal - etrue)/etrue);
               corrEndcap->Fill(etrue, (correctedE - etrue)/etrue);
               corrEndcapHcal->Fill(etrue, (correctedE - etrue)/etrue);

               rawEtaDependence->Fill(eta, (ecal + hcal - etrue)/etrue);
               corrEtaDependence->Fill(eta, (correctedEta - etrue)/etrue);
            }
         }
      }  
   }

   ////////////////////////////////////////////////////////////////////////////
   //Add all the draw functions that you would like here, as well as any 
   //additional output you would like.
   ////////////////////////////////////////////////////////////////////////////
   
//   drawGausFit(raw,responseRaw,resolutionRaw);
//   drawEtaDependence(corrEtaDependence, responseEta);
//   drawGausFit(corrEta,response, resolution);
//   drawCompare(responseRaw, response, resolutionRaw, resolution);

   barrelWithEcalHcalCalib->drawCoeffGraph("B");
   barrelWithEcalHcalCalib->drawCoeffGraph("C");
   barrelWithEcalHcalCalib->drawCoeffGraph("Alpha");
   barrelWithEcalHcalCalib->drawCoeffGraph("Beta");


   
   functionBarrelEcalHcalB_e = functionBarrelEcalHcalB->GetTitle();
   functionBarrelEcalHcalC_e = functionBarrelEcalHcalC->GetTitle();
   functionBarrelHcalC_e = functionBarrelHcalC->GetTitle();
   functionBarrelAlpha_e = functionBarrelAlpha->GetTitle();
   functionBarrelBeta_e = functionBarrelBeta->GetTitle();

   functionEndcapEcalHcalB_e = functionEndcapEcalHcalB->GetTitle();
   functionEndcapEcalHcalC_e = functionEndcapEcalHcalC->GetTitle();
   functionEndcapHcalC_e = functionEndcapHcalC->GetTitle();
   functionEndcapAlpha_e = functionEndcapAlpha->GetTitle();
   functionEndcapBeta_e = functionEndcapBeta->GetTitle();
 
   cout<<"  threshE = "<<barrelABCEcalHcal[2]->getA()<<";"<<endl;
   cout<<"  threshH = "<<barrelABCHcal[2]->getA()<<";"<<endl; 
   
   cout<<"  faBarrel = new TF1(\"faBarrel\",\""<<
      functionBarrelEcalHcalB_e<<"\",1.,1000.);"<<endl;
   cout<<"  fbBarrel = new TF1(\"fbBarrel\",\""<<
      functionBarrelEcalHcalC_e<<"\",1.,1000.);"<<endl;
   cout<<"  fcBarrel = new TF1(\"fcBarrel\",\""<<
      functionBarrelHcalC_e<<"\",1.,1000.);"<<endl;
   cout<<"  faEtaBarrel = new TF1(\"faEtaBarrel\",\""<<
      functionBarrelAlpha_e<<"\",1.,1000.);"<<endl;
   cout<<"  fbEtaBarrel = new TF1(\"fbEtaBarrel\",\""<<
      functionBarrelBeta_e<<"\",1.,1000.);"<<endl;

  for ( unsigned i = 0; i < 10; ++i ) 
  { 
     barrelEcalHcalB = functionBarrelEcalHcalB->GetParameter(i);
     barrelEcalHcalC = functionBarrelEcalHcalC->GetParameter(i);
     barrelHcalC = functionBarrelHcalC->GetParameter(i);
     barrelAlpha = functionBarrelAlpha->GetParameter(i);
     barrelBeta = functionBarrelBeta->GetParameter(i);
     
     if ( barrelEcalHcalB != 0. ) 
        cout<<"  faBarrel->SetParameter("<<i<<","<<barrelEcalHcalB<<");"<<endl;
     if ( barrelEcalHcalC != 0. ) 
        cout<<"  fbBarrel->SetParameter("<<i<<","<<barrelEcalHcalC<<");"<<endl;
     if ( barrelHcalC != 0. ) 
        cout<<"  fcBarrel->SetParameter("<<i<<","<<barrelHcalC<<");"<<endl;
     if ( barrelAlpha != 0. ) 
        cout<<"  faEtaBarrel->SetParameter("<<i<<","<<barrelAlpha<<");"<<endl;
     if ( barrelBeta != 0. ) 
        cout<<"  fbEtaBarrel->SetParameter("<<i<<","<<barrelBeta<<");"<<endl;
  }

   cout<<"  faEndcap = new TF1(\"faEndcap\",\""<<
      functionEndcapEcalHcalB_e<<"\",1.,1000.);"<<endl;
   cout<<"  fbEndcap = new TF1(\"fbEndcap\",\""<<
      functionEndcapEcalHcalC_e<<"\",1.,1000.);"<<endl;
   cout<<"  fcEndcap = new TF1(\"fcEndcap\",\""<<
      functionEndcapHcalC_e<<"\",1.,1000.);"<<endl;
   cout<<"  faEtaEndcap = new TF1(\"faEtaEndcap\",\""<<
      functionEndcapAlpha_e<<"\",1.,1000.);"<<endl;
   cout<<"  fbEtaEndcap = new TF1(\"fbEtaEndcap\",\""<<
      functionEndcapBeta_e<<"\",1.,1000.);"<<endl;

  for ( unsigned i = 0; i < 10; ++i ) 
  { 
     endcapEcalHcalB = functionEndcapEcalHcalB->GetParameter(i);
     endcapEcalHcalC = functionEndcapEcalHcalC->GetParameter(i);
     endcapHcalC = functionEndcapHcalC->GetParameter(i);
     endcapAlpha = functionEndcapAlpha->GetParameter(i);
     endcapBeta = functionEndcapBeta->GetParameter(i);
     
     if ( endcapEcalHcalB != 0. ) 
        cout<<"  faEndcap->SetParameter("<<i<<","<<endcapEcalHcalB<<");"<<endl;
     if ( endcapEcalHcalC != 0. ) 
        cout<<"  fbEndcap->SetParameter("<<i<<","<<endcapEcalHcalC<<");"<<endl;
     if ( endcapHcalC != 0. ) 
        cout<<"  fcEndcap->SetParameter("<<i<<","<<endcapHcalC<<");"<<endl;
     if ( endcapAlpha != 0. ) 
        cout<<"  faEtaEndcap->SetParameter("<<i<<","<<endcapAlpha<<");"<<endl;
     if ( endcapBeta != 0. ) 
        cout<<"  fbEtaEndcap->SetParameter("<<i<<","<<endcapBeta<<");"<<endl;
  }

}

