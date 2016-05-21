#include <pacotest_header.h>

// [[Rcpp::export]]
Rcpp::List VI(arma::mat Udata, arma::mat Wdata, double NumbBoot) {
    try
    {
        
        int N = (int) NumbBoot;
        
        
        // Associate outputs
        double TestStat;
        double pValue;
        arma::mat S(N,1);
        Rcpp::List out;
        
        VecIndepTest(Udata, Wdata, N, &TestStat, &pValue, S);
        
        out = Rcpp::List::create(Rcpp::Named("pValue")=pValue,Rcpp::Named("TestStat")=TestStat,Rcpp::Named("S")=S);
        return out;
    }
    catch( std::exception& __ex__ ) {
        forward_exception_to_r( __ex__ );
    }
    catch(...) {
        ::Rf_error( "c++ exception" );
    }
}


// [[Rcpp::export]]
Rcpp::List EC(arma::mat Udata, arma::mat Wdata, double NumbBoot, double Grouping, double finalComparison, double ExpMinSampleSize = 50, double TrainingDataFraction = 0.5) {
    try
    {
        
        int N = (int) NumbBoot;
        int grouping = (int) Grouping;
        
        
        // Associate outputs
        double TestStat;
        double pValue;
        arma::mat S(N,1);
        arma::mat Xdata(1,2);
        arma::mat Ydata(1,2);
        arma::uvec SplitVariable(4);
        arma::uvec SplitQuantile(4);
        arma::vec SplitThreshold(3);
        SplitVariable.zeros();
        SplitQuantile.zeros();
        SplitThreshold.zeros();
        
        Rcpp::List out;
        
        EqualCopTest(Udata, Wdata, N, grouping, finalComparison, &TestStat, &pValue, S, Xdata, Ydata, ExpMinSampleSize, TrainingDataFraction, SplitVariable, SplitQuantile, SplitThreshold);
        
        out = Rcpp::List::create(Rcpp::Named("pValue")=pValue,Rcpp::Named("TestStat")=TestStat,Rcpp::Named("S")=S,Rcpp::Named("Xdata")=Xdata,Rcpp::Named("Ydata")=Ydata,Rcpp::Named("SplitVariable")=SplitVariable,Rcpp::Named("SplitQuantile")=SplitQuantile,Rcpp::Named("SplitThreshold")=SplitThreshold);
        return out;
    }
    catch( std::exception& __ex__ ) {
        forward_exception_to_r( __ex__ );
    }
    catch(...) {
        ::Rf_error( "c++ exception" );
    }
}


// [[Rcpp::export]]
Rcpp::List ECOV(arma::mat Udata, arma::mat Wdata, double Grouping, double doubleWithEstUncert,double finalComparison, arma::mat &data, Rcpp::DataFrame svcmDataFrame, Rcpp::List cPitData, double AggPvalsNumbRep=0, double ExpMinSampleSize = 50, double TrainingDataFraction = 0.5) {
    try
    {
        
        int grouping = (int) Grouping;
        int aggPvalsNumbRep = (int) AggPvalsNumbRep;
        int intWithEstUncert = (int) doubleWithEstUncert;
        
        
        // Associate outputs
        double pValue;
        Rcpp::List out;
        unsigned int i=0;
        
        if (AggPvalsNumbRep == 0)
        {
            double TestStat;
            arma::mat Xdata(1,2);
            arma::mat Ydata(1,2);
            arma::uvec SplitVariable(4);
            arma::uvec SplitQuantile(4);
            arma::vec SplitThreshold(3);
            SplitVariable.zeros();
            SplitQuantile.zeros();
            SplitThreshold.zeros();
            
            EqualCovTest(Udata, Wdata, grouping, intWithEstUncert, finalComparison, &TestStat, &pValue, Xdata, Ydata, ExpMinSampleSize, TrainingDataFraction, SplitVariable, SplitQuantile, SplitThreshold, data, svcmDataFrame, cPitData);
            
            out = Rcpp::List::create(Rcpp::Named("pValue")=pValue,Rcpp::Named("TestStat")=TestStat,Rcpp::Named("Xdata")=Xdata,Rcpp::Named("Ydata")=Ydata,Rcpp::Named("SplitVariable")=SplitVariable,Rcpp::Named("SplitQuantile")=SplitQuantile,Rcpp::Named("SplitThreshold")=SplitThreshold);
        }
        else
        {
            arma::mat pValues(AggPvalsNumbRep,1);
            arma::mat Xdata(1,2);
            arma::mat Ydata(1,2);
            arma::umat SplitVariable(4,AggPvalsNumbRep);
            arma::umat SplitQuantile(4,AggPvalsNumbRep);
            arma::mat SplitThreshold(3,AggPvalsNumbRep);
            SplitVariable.zeros();
            SplitQuantile.zeros();
            SplitThreshold.zeros();

            EqualCovTest(Udata, Wdata, grouping, intWithEstUncert, finalComparison, pValues, &pValue, AggPvalsNumbRep, ExpMinSampleSize, TrainingDataFraction, SplitVariable, SplitQuantile, SplitThreshold, data, svcmDataFrame, cPitData);
            
            out = Rcpp::List::create(Rcpp::Named("pValue")=pValue,Rcpp::Named("pValues")=pValues,Rcpp::Named("SplitVariable")=SplitVariable,Rcpp::Named("SplitQuantile")=SplitQuantile,Rcpp::Named("SplitThreshold")=SplitThreshold);
        }
        
        return out;
    }
    catch( std::exception& __ex__ ) {
        forward_exception_to_r( __ex__ );
    }
    catch(...) {
        ::Rf_error( "c++ exception" );
    }
}

// [[Rcpp::export]]
Rcpp::List ECOVwithPenalty(arma::mat Udata, arma::mat Wdata, double Grouping, double doubleWithEstUncert,double finalComparison, arma::mat &data, Rcpp::DataFrame svcmDataFrame, Rcpp::List cPitData, double ExpMinSampleSize = 50, double penaltyLevel = 1, double penaltyPower = 0.5, double Gamma0Partition = 4) {
    try
    {
        
        int grouping = (int) Grouping;
        int intWithEstUncert = (int) doubleWithEstUncert;
        int gamma0Partition = (int) Gamma0Partition;
        
        // Associate outputs
        double pValue;
        Rcpp::List out;
        
        double TestStat;
        arma::mat Xdata(1,2);
        arma::mat Ydata(1,2);
        arma::uvec SplitVariable(4);
        arma::uvec SplitQuantile(4);
        arma::vec SplitThreshold(3);
        SplitVariable.zeros();
        SplitQuantile.zeros();
        SplitThreshold.zeros();
        
        EqualCovTestWithPenalty(Udata, Wdata, grouping, intWithEstUncert, finalComparison, &TestStat, &pValue, Xdata, Ydata, ExpMinSampleSize, penaltyLevel, penaltyPower, gamma0Partition, SplitVariable, SplitQuantile, SplitThreshold, data, svcmDataFrame, cPitData);
        
        out = Rcpp::List::create(Rcpp::Named("pValue")=pValue,Rcpp::Named("TestStat")=TestStat,Rcpp::Named("Xdata")=Xdata,Rcpp::Named("Ydata")=Ydata,Rcpp::Named("SplitVariable")=SplitVariable,Rcpp::Named("SplitQuantile")=SplitQuantile,Rcpp::Named("SplitThreshold")=SplitThreshold);
        
        
        return out;
    }
    catch( std::exception& __ex__ ) {
        forward_exception_to_r( __ex__ );
    }
    catch(...) {
        ::Rf_error( "c++ exception" );
    }
}


// [[Rcpp::export]]
Rcpp::List ECORR(arma::mat Udata, arma::mat Wdata, double Grouping, double doubleWithEstUncert,double finalComparison, arma::mat &data, Rcpp::DataFrame svcmDataFrame, Rcpp::List cPitData, double AggPvalsNumbRep=0, double ExpMinSampleSize = 50, double TrainingDataFraction = 0.5) {
    try
    {
        
        int grouping = (int) Grouping;
        int aggPvalsNumbRep = (int) AggPvalsNumbRep;
        int intWithEstUncert = (int) doubleWithEstUncert;
        
        
        // Associate outputs
        double pValue;
        Rcpp::List out;
        unsigned int i=0;
        
        if (AggPvalsNumbRep == 0)
        {
            double TestStat;
            arma::mat Xdata(1,2);
            arma::mat Ydata(1,2);
            arma::uvec SplitVariable(4);
            arma::uvec SplitQuantile(4);
            arma::vec SplitThreshold(3);
            SplitVariable.zeros();
            SplitQuantile.zeros();
            SplitThreshold.zeros();
            
            EqualCorrTest(Udata, Wdata, grouping, intWithEstUncert, finalComparison, &TestStat, &pValue, Xdata, Ydata, ExpMinSampleSize, TrainingDataFraction, SplitVariable, SplitQuantile, SplitThreshold, data, svcmDataFrame, cPitData);
            
            out = Rcpp::List::create(Rcpp::Named("pValue")=pValue,Rcpp::Named("TestStat")=TestStat,Rcpp::Named("Xdata")=Xdata,Rcpp::Named("Ydata")=Ydata,Rcpp::Named("SplitVariable")=SplitVariable,Rcpp::Named("SplitQuantile")=SplitQuantile,Rcpp::Named("SplitThreshold")=SplitThreshold);
        }
        else
        {
            arma::mat pValues(AggPvalsNumbRep,1);
            arma::mat Xdata(1,2);
            arma::mat Ydata(1,2);
            arma::umat SplitVariable(4,AggPvalsNumbRep);
            arma::umat SplitQuantile(4,AggPvalsNumbRep);
            arma::mat SplitThreshold(3,AggPvalsNumbRep);
            SplitVariable.zeros();
            SplitQuantile.zeros();
            SplitThreshold.zeros();

            EqualCorrTest(Udata, Wdata, grouping, intWithEstUncert, finalComparison, pValues, &pValue, AggPvalsNumbRep, ExpMinSampleSize, TrainingDataFraction, SplitVariable, SplitQuantile, SplitThreshold, data, svcmDataFrame, cPitData);
            
            out = Rcpp::List::create(Rcpp::Named("pValue")=pValue,Rcpp::Named("pValues")=pValues,Rcpp::Named("SplitVariable")=SplitVariable,Rcpp::Named("SplitQuantile")=SplitQuantile,Rcpp::Named("SplitThreshold")=SplitThreshold);
        }
        
        return out;
    }
    catch( std::exception& __ex__ ) {
        forward_exception_to_r( __ex__ );
    }
    catch(...) {
        ::Rf_error( "c++ exception" );
    }
}

// [[Rcpp::export]]
Rcpp::List ECORRwithPenalty(arma::mat Udata, arma::mat Wdata, double Grouping, double doubleWithEstUncert,double finalComparison, arma::mat &data, Rcpp::DataFrame svcmDataFrame, Rcpp::List cPitData, double ExpMinSampleSize = 50, double penaltyLevel = 1, double penaltyPower = 0.5, double Gamma0Partition = 4) {
    try
    {
        
        int grouping = (int) Grouping;
        int intWithEstUncert = (int) doubleWithEstUncert;
        int gamma0Partition = (int) Gamma0Partition;
        
        // Associate outputs
        double pValue;
        Rcpp::List out;
        
        double TestStat;
        arma::mat Xdata(1,2);
        arma::mat Ydata(1,2);
        arma::uvec SplitVariable(4);
        arma::uvec SplitQuantile(4);
        arma::vec SplitThreshold(3);
        SplitVariable.zeros();
        SplitQuantile.zeros();
        SplitThreshold.zeros();
        
        EqualCorrTestWithPenalty(Udata, Wdata, grouping, intWithEstUncert, finalComparison, &TestStat, &pValue, Xdata, Ydata, ExpMinSampleSize, penaltyLevel, penaltyPower, gamma0Partition, SplitVariable, SplitQuantile, SplitThreshold, data, svcmDataFrame, cPitData);
        
        out = Rcpp::List::create(Rcpp::Named("pValue")=pValue,Rcpp::Named("TestStat")=TestStat,Rcpp::Named("Xdata")=Xdata,Rcpp::Named("Ydata")=Ydata,Rcpp::Named("SplitVariable")=SplitVariable,Rcpp::Named("SplitQuantile")=SplitQuantile,Rcpp::Named("SplitThreshold")=SplitThreshold);
        
        
        return out;
    }
    catch( std::exception& __ex__ ) {
        forward_exception_to_r( __ex__ );
    }
    catch(...) {
        ::Rf_error( "c++ exception" );
    }
}


// [[Rcpp::export]]
SEXP covOfCorrelationsWithEstimationFromCpp(arma::mat &data, Rcpp::DataFrame svcmDataFrame, arma::umat &indexVectors, arma::uvec &nObsPerVector, Rcpp::List cPitData, arma::vec &theta, arma::mat &sigma)
{
  int nGroups = nObsPerVector.n_elem;
  
  Rcpp::List indListForR;
  
  arma::uvec indInFirstGroup = indexVectors.submat(0,0,nObsPerVector(0)-1,0);
  indInFirstGroup++;
  arma::uvec indInSecondGroup = indexVectors.submat(0,1,nObsPerVector(1)-1,1);
  indInSecondGroup++;
  
  if (nGroups == 2)
  {
    indListForR = Rcpp::List::create(indInFirstGroup,indInSecondGroup);
  }
  else
  {
    if (nGroups > 2)
    {
      arma::uvec indInThirdGroup = indexVectors.submat(0,2,nObsPerVector(2)-1,2);
      indInThirdGroup++;
      if (nGroups == 3)
      {
      indListForR = Rcpp::List::create(indInFirstGroup,indInSecondGroup,indInThirdGroup);
      }
      else
      {
      arma::uvec indInFourthGroup = indexVectors.submat(0,3,nObsPerVector(3)-1,3);
      indInFourthGroup++;
      indListForR = Rcpp::List::create(indInFirstGroup,indInSecondGroup,indInThirdGroup,indInFourthGroup);
      }
    }
  }
  
  Rcpp::Environment pacotest = Rcpp::Environment::namespace_env("pacotest");
  Rcpp::Function covOfCorrelationsWithEstimation = pacotest["covOfCorrelationsWithEstimation"];
  
  SEXP outFromR = covOfCorrelationsWithEstimation(Rcpp::Named("data", data), Rcpp::Named("svcmDataFrame", svcmDataFrame), Rcpp::Named("indList", indListForR), Rcpp::Named("cPitData", cPitData), Rcpp::Named("theta", theta));
  
  sigma = Rcpp::as<arma::mat>(outFromR);
  
}



// [[Rcpp::export]]
SEXP covOfCovariancesWithEstimationFromCpp(arma::mat &data, Rcpp::DataFrame svcmDataFrame, arma::umat &indexVectors, arma::uvec &nObsPerVector, Rcpp::List cPitData, arma::vec &theta, arma::mat &sigma)
{
  int nGroups = nObsPerVector.n_elem;
  
  Rcpp::List indListForR;
  
  arma::uvec indInFirstGroup = indexVectors.submat(0,0,nObsPerVector(0)-1,0);
  indInFirstGroup++;
  arma::uvec indInSecondGroup = indexVectors.submat(0,1,nObsPerVector(1)-1,1);
  indInSecondGroup++;
  
  if (nGroups == 2)
  {
    indListForR = Rcpp::List::create(indInFirstGroup,indInSecondGroup);
  }
  else
  {
    if (nGroups > 2)
    {
      arma::uvec indInThirdGroup = indexVectors.submat(0,2,nObsPerVector(2)-1,2);
      indInThirdGroup++;
      if (nGroups == 3)
      {
      indListForR = Rcpp::List::create(indInFirstGroup,indInSecondGroup,indInThirdGroup);
      }
      else
      {
      arma::uvec indInFourthGroup = indexVectors.submat(0,3,nObsPerVector(3)-1,3);
      indInFourthGroup++;
      indListForR = Rcpp::List::create(indInFirstGroup,indInSecondGroup,indInThirdGroup,indInFourthGroup);
      }
    }
  }
  
  Rcpp::Environment pacotest = Rcpp::Environment::namespace_env("pacotest");
  Rcpp::Function covOfCovariancesWithEstimation = pacotest["covOfCovariancesWithEstimation"];
  
  SEXP outFromR = covOfCovariancesWithEstimation(Rcpp::Named("data", data), Rcpp::Named("svcmDataFrame", svcmDataFrame), Rcpp::Named("indList", indListForR), Rcpp::Named("cPitData", cPitData), Rcpp::Named("theta", theta));
  
  sigma = Rcpp::as<arma::mat>(outFromR);
  
}


// [[Rcpp::export]]
Rcpp::List testStatEqualCorrWithoutEstimationCpp(arma::umat indexVectors, arma::uvec nObsPerVector, arma::mat Udata)
{
  try
  {
    // Associate outputs
    Rcpp::List out;
    double testStat;
    arma::mat sigma;
    arma::vec theta;
    
    EqualCorrChi2TestStat(Udata, indexVectors, nObsPerVector, &testStat, sigma, theta);
    
    out = Rcpp::List::create(Rcpp::Named("testStat")=testStat,Rcpp::Named("sigma")=sigma,Rcpp::Named("theta")=theta);
    
    return out;
  }
  catch( std::exception& __ex__ ) {
    forward_exception_to_r( __ex__ );
  }
  catch(...) {
    ::Rf_error( "c++ exception" );
  }
}

// [[Rcpp::export]]
Rcpp::List testStatEqualCorrWithEstimationCpp(arma::umat indexVectors, arma::uvec nObsPerVector, arma::mat Udata, arma::mat data, Rcpp::DataFrame svcmDataFrame, Rcpp::List cPitData)
{
  try
  {
    // Associate outputs
    Rcpp::List out;
    double testStat;
    arma::mat sigma;
    arma::vec theta;
    
    EqualCorrChi2WithEstimationTestStat(Udata, indexVectors, nObsPerVector, &testStat, sigma, theta, data, svcmDataFrame, cPitData);
    
    out = Rcpp::List::create(Rcpp::Named("testStat")=testStat,Rcpp::Named("sigma")=sigma,Rcpp::Named("theta")=theta);
    
    return out;
  }
  catch( std::exception& __ex__ ) {
    forward_exception_to_r( __ex__ );
  }
  catch(...) {
    ::Rf_error( "c++ exception" );
  }
}

