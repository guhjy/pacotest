#include "SAtest_header.h"
#include "armaMex.hpp"


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    try
    {
        // Check the number of input arguments.
        if (nrhs != 6)
            mexErrMsgTxt("Incorrect number of input arguments.");
        
        // Associate inputs
        mat Udata = armaGetPr(prhs[0]);
        mat Wdata = armaGetPr(prhs[1]);
        double *grouping = mxGetPr(prhs[2]);
        int Grouping = (int) *grouping;
        double *aggPvalsNumbRep = mxGetPr(prhs[3]);
        int AggPvalsNumbRep = (int) *aggPvalsNumbRep;
        double ExpMinSampleSize;
        double TrainingDataFraction;
        if (!mxIsEmpty(prhs[4]))
        {
            double *expMinSampleSize = mxGetPr(prhs[4]);
            ExpMinSampleSize = *expMinSampleSize;
        }
        else
        {
            ExpMinSampleSize = 50;
        }
        if (!mxIsEmpty(prhs[5]))
        {
            double *trainingDataFraction = mxGetPr(prhs[5]);
            TrainingDataFraction = *trainingDataFraction;
        }
        else
        {
            TrainingDataFraction = 0.5;
        }
        
        // Check type of input.
        if ( (mxGetClassID(prhs[0]) != mxDOUBLE_CLASS) || (mxGetClassID(prhs[1]) != mxDOUBLE_CLASS) || Udata.n_rows != Wdata.n_rows || Udata.n_cols != 2 )
            mexErrMsgTxt(".");
        
        // Check if input is real.
        if ( (mxIsComplex(prhs[0])) || (mxIsComplex(prhs[1])) )
            mexErrMsgTxt(".");
        
        
        // Associate outputs
        double *pValue;
        plhs[0] = mxCreateDoubleScalar(mxREAL);
        pValue = mxGetPr(plhs[0]);
        unsigned int i=0;
        
        if (AggPvalsNumbRep == 0)
        {
            double *TestStat;
            plhs[1] = mxCreateDoubleScalar(mxREAL);
            TestStat = mxGetPr(plhs[1]);
            
            if (nlhs > 5)
            {
                arma::mat Xdata(1,2);
                arma::mat Ydata(1,2);
                arma::umat SplitVariable(1,4);
                arma::umat SplitQuantile(1,4);
                arma::mat SplitThreshold(1,3);
                SplitVariable(0,0) = 0; SplitVariable(0,1) = 0; SplitVariable(0,2) = 0; SplitVariable(0,3) = 0;
                SplitQuantile(0,0) = 0; SplitQuantile(0,1) = 0; SplitQuantile(0,2) = 0; SplitQuantile(0,3) = 0;
                SplitThreshold(0,0) = 0; SplitThreshold(0,1) = 0; SplitThreshold(0,2) = 0;
                
                EqualRankCorrTest(Udata, Wdata, Grouping, TestStat, pValue, Xdata, Ydata, ExpMinSampleSize,TrainingDataFraction, SplitVariable, SplitQuantile, SplitThreshold);
                plhs[2] = armaCreateMxMatrix(1, 4);
                armaSetPr(plhs[2], arma::conv_to<arma::mat>::from(SplitVariable));
                plhs[3] = armaCreateMxMatrix(1, 4);
                armaSetPr(plhs[3], arma::conv_to<arma::mat>::from(SplitQuantile));
                plhs[4] = armaCreateMxMatrix(1, 3);
                armaSetPr(plhs[4], arma::conv_to<arma::mat>::from(SplitThreshold));
                plhs[5] = armaCreateMxMatrix(Xdata.n_rows, 2);
                armaSetPr(plhs[5], Xdata);
                plhs[6] = armaCreateMxMatrix(Ydata.n_rows, 2);
                armaSetPr(plhs[6], Ydata);
            }
            else
            {
                arma::umat SplitVariable(1,4);
                arma::umat SplitQuantile(1,4);
                arma::mat SplitThreshold(1,3);
                SplitVariable(0,0) = 0; SplitVariable(0,1) = 0; SplitVariable(0,2) = 0; SplitVariable(0,3) = 0;
                SplitQuantile(0,0) = 0; SplitQuantile(0,1) = 0; SplitQuantile(0,2) = 0; SplitQuantile(0,3) = 0;
                SplitThreshold(0,0) = 0; SplitThreshold(0,1) = 0; SplitThreshold(0,2) = 0;
                EqualRankCorrTest(Udata, Wdata, Grouping, TestStat, pValue, ExpMinSampleSize,TrainingDataFraction, SplitVariable, SplitQuantile, SplitThreshold);
                plhs[2] = armaCreateMxMatrix(1, 4);
                armaSetPr(plhs[2], arma::conv_to<arma::mat>::from(SplitVariable));
                plhs[3] = armaCreateMxMatrix(1, 4);
                armaSetPr(plhs[3], arma::conv_to<arma::mat>::from(SplitQuantile));
                plhs[4] = armaCreateMxMatrix(1, 3);
                armaSetPr(plhs[4], arma::conv_to<arma::mat>::from(SplitThreshold));
            }
        }
        else
        {
            arma::mat pValues(AggPvalsNumbRep,1);
            arma::umat SplitVariable(AggPvalsNumbRep,4);
            arma::umat SplitQuantile(AggPvalsNumbRep,4);
            arma::mat SplitThreshold(AggPvalsNumbRep,3);
            for (i=0;i<AggPvalsNumbRep;i++)
            {
                SplitVariable(i,0) = 0; SplitVariable(i,1) = 0; SplitVariable(i,2) = 0; SplitVariable(i,3) = 0;
                SplitQuantile(i,0) = 0; SplitQuantile(i,1) = 0; SplitQuantile(i,2) = 0; SplitQuantile(i,3) = 0;
                SplitThreshold(i,0) = 0; SplitThreshold(i,1) = 0; SplitThreshold(i,2) = 0;
            }
            
            EqualRankCorrTest(Udata, Wdata, pValues, pValue, AggPvalsNumbRep, ExpMinSampleSize,TrainingDataFraction, SplitVariable, SplitQuantile, SplitThreshold);
            
            
            plhs[1] = armaCreateMxMatrix(AggPvalsNumbRep, 1);
            armaSetPr(plhs[1], pValues);
            plhs[2] = armaCreateMxMatrix(AggPvalsNumbRep, 4);
            armaSetPr(plhs[2], arma::conv_to<arma::mat>::from(SplitVariable));
            plhs[3] = armaCreateMxMatrix(AggPvalsNumbRep, 4);
            armaSetPr(plhs[3], arma::conv_to<arma::mat>::from(SplitQuantile));
            plhs[4] = armaCreateMxMatrix(AggPvalsNumbRep, 3);
            armaSetPr(plhs[4], arma::conv_to<arma::mat>::from(SplitThreshold));
        }
        
    }
    catch (std::exception& e)
    {
        std::string msg = std::string("SAtest Error: ") + e.what();
        mexErrMsgTxt(msg.c_str());
    }
    return;
}
