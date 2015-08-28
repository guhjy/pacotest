pacotest = function(Udata,W,pacotestOptions){
  
  pacotestOptions = pacotestset(pacotestOptions)
  Udata = as.matrix(Udata)
  W = as.matrix(W)
  
  if (pacotestOptions$TestType=='ERC')
  {
    if (pacotestOptions$Grouping=='TreeERC' && pacotestOptions$AggPvalsNumbRep > 1)
    {
      # third list element are the p-values that have been aggregated.
      Grouping = 0
      out = ERC(Udata,W,Grouping,pacotestOptions$AggPvalsNumbRep,pacotestOptions$ExpMinSampleSize,pacotestOptions$TrainingDataFraction)
      CondSetDim = ncol(W);
      out$DecisionTree = ExtractDecisionTree(CondSetDim, out$SplitVariable, out$SplitQuantile, out$SplitThreshold)
    }
    else
    {
      Grouping = which(pacotestOptions$Grouping==c('TreeERC','TreeEC','SumMedian','SumThirdsI','SumThirdsII','ProdMedian','ProdThirdsI','ProdThirdsII'),arr.ind=TRUE)
      if (Grouping > 2)
      {
        out = ERC(Udata,W,Grouping,0,1,1)
      }
      else
      {
        out = ERC(Udata,W,Grouping,0,pacotestOptions$ExpMinSampleSize,pacotestOptions$TrainingDataFraction)
      }
      if (pacotestOptions$GroupedScatterplots)
      {
        GroupedScatterplot(out$Xdata,out$Ydata)
      }
      if (Grouping<=2)
      {
        CondSetDim = ncol(W);
        out$DecisionTree = ExtractDecisionTree(CondSetDim, out$SplitVariable, out$SplitQuantile, out$SplitThreshold)
        if (pacotestOptions$DecisionTreePlot)
        {
          if (!requireNamespace("plotrix", quietly = TRUE)) {
            stop("plotrix needed to obtain decision tree plots. Please install it.",
                 call. = FALSE)
            DecisionTreePlot(out$DecisionTree)
          }
        }
      }
    }
    out[c("SplitVariable", "SplitQuantile", "SplitThreshold")] = NULL
  }
  else if (pacotestOptions$TestType=='EC')
  {
    Grouping = which(pacotestOptions$Grouping==c('TreeERC','TreeEC','SumMedian','SumThirdsI','SumThirdsII','ProdMedian','ProdThirdsI','ProdThirdsII'),arr.ind=TRUE)
    if (Grouping > 2)
    {
      out = EC(Udata,W,pacotestOptions$NumbBoot,Grouping,1,1)
    }
    else
    {
      out = EC(Udata,W,pacotestOptions$NumbBoot,Grouping,pacotestOptions$ExpMinSampleSize,pacotestOptions$TrainingDataFraction)
    }
    if (pacotestOptions$GroupedScatterplots)
    {
      GroupedScatterplot(out$Xdata,out$Ydata)
    }
    if (Grouping<=2)
    {
      CondSetDim = ncol(W);
      out$DecisionTree = ExtractDecisionTree(CondSetDim, out$SplitVariable, out$SplitQuantile, out$SplitThreshold)
      if (pacotestOptions$DecisionTreePlot)
      {
        if (!requireNamespace("plotrix", quietly = TRUE)) {
          stop("plotrix needed to obtain decision tree plots. Please install it.",
               call. = FALSE)
          DecisionTreePlot(out$DecisionTree)
        }
      }
    }
    out[c("SplitVariable", "SplitQuantile", "SplitThreshold")] = NULL
  }
  else if (pacotestOptions$TestType=='VI')
  {
    out = VI(Udata,W,pacotestOptions$NumbBoot)
  }
  else
  {
    stop('Unknown TestType')
  }
  return(out)
}

GroupedScatterplot = function(Xdata,Ydata)
{
  par(mfrow=c(2,2))
  plot(Xdata[,1],Xdata[,2],xlab='U',ylab='V')
  plot(Ydata[,1],Ydata[,2],xlab='U',ylab='V')
  plot(qnorm(Xdata[,1]),qnorm(Xdata[,2]),xlab=expression(paste(Phi^-1,(U))),ylab=expression(paste(Phi^-1,(V))))
  plot(qnorm(Ydata[,1]),qnorm(Ydata[,2]),xlab=expression(paste(Phi^-1,(U))),ylab=expression(paste(Phi^-1,(V))))
}


ExtractDecisionTree = function(CondSetDim, SplitVariable, SplitQuantile, SplitThreshold)
{
  
  Node = list(Variable=NULL,Quantile=NULL,Threshold=NULL)
  DecisionTree = rep(list(list(CentralNode=Node,LeftNode=NULL,RightNode=NULL,LeavesForFinalComparison=NULL)), nrow(SplitVariable))
  Quantiles = c(25,50,75)
  
  for (i in 1:nrow(SplitVariable))
  {
    
    if ((SplitVariable[1]+1) <= CondSetDim)
    {
      DecisionTree[[i]]$CentralNode$Variable = paste('W', SplitVariable[1]+1, sep = '')
    }
    else
    {
      DecisionTree[[i]]$CentralNode$Variable = 'Mean(W)';
    }
    DecisionTree[[i]]$CentralNode$Quantile = paste('Q', Quantiles[SplitQuantile[1]+1], sep = '')
    DecisionTree[[i]]$CentralNode$Threshold = SplitThreshold[1];
    
    if (SplitVariable[4] == 6) # The one split only case
    {
      DecisionTree[[i]]$LeavesForFinalComparison = 'L vs R';
    }
    else
    {
      if (SplitVariable[4] < 6)
      {
        
        
        if ((SplitVariable[2]+1) <= CondSetDim)
        {
          DecisionTree[[i]]$LeftNode$Variable = paste('W', SplitVariable[2]+1, sep = '')
        }
        else
        {
          DecisionTree[[i]]$LeftNode$Variable = 'Mean(W)';
        }
        DecisionTree[[i]]$LeftNode$Quantile = paste('Q', Quantiles[SplitQuantile[2]+1], sep = '')
        DecisionTree[[i]]$LeftNode$Threshold = SplitThreshold[2];
        
        if ((SplitVariable[3]+1) <= CondSetDim)
        {
          DecisionTree[[i]]$RightNode$Variable = paste('W', SplitVariable[3]+1, sep = '')
        }
        else
        {
          DecisionTree[[i]]$RightNode$Variable = 'Mean(W)';
        }
        DecisionTree[[i]]$RightNode$Quantile = paste('Q', Quantiles[SplitQuantile[3]+1], sep = '')
        DecisionTree[[i]]$RightNode$Threshold = SplitThreshold[3];
        
        PossibleLeaves = c('LL vs LR','LL vs RL','LL vs RR','LR vs RL','LR vs RR','RL vs RR')
        
        DecisionTree[[i]]$LeavesForFinalComparison = PossibleLeaves[SplitVariable[4]+1]
      }
      else
      {
        if (SplitVariable[4]<13)
        {
          
          if ((SplitVariable[2]+1) <= CondSetDim)
          {
            DecisionTree[[i]]$LeftNode$Variable = paste('W', SplitVariable[2]+1, sep = '')
          }
          else
          {
            DecisionTree[[i]]$LeftNode$Variable = 'Mean(W)';
          }
          DecisionTree[[i]]$LeftNode$Quantile = paste('Q', Quantiles[SplitQuantile[2]+1], sep = '')
          DecisionTree[[i]]$LeftNode$Threshold = SplitThreshold[2];
          
          PossibleLeaves = c('LL vs LR','LL vs R','LR vs R','','','')
          
          DecisionTree[[i]]$LeavesForFinalComparison = PossibleLeaves[SplitVariable[4]-10+1]
        }
        else
        {
          if ((SplitVariable[3]+1) <= CondSetDim)
          {
            DecisionTree[[i]]$RightNode$Variable = paste('W', SplitVariable[3]+1, sep = '')
          }
          else
          {
            DecisionTree[[i]]$RightNode$Variable = 'Mean(W)';
          }
          DecisionTree[[i]]$RightNode$Quantile = paste('Q', Quantiles[SplitQuantile[3]+1], sep = '')
          DecisionTree[[i]]$RightNode$Threshold = SplitThreshold[3];
          
          PossibleLeaves = c('','','','L vs RL','L vs RR','RL vs RR')
          
          DecisionTree[[i]]$LeavesForFinalComparison = PossibleLeaves[SplitVariable[4]-10+1]
        }
      }
    }
  }
  
  if (nrow(SplitVariable) == 1)
  {
    DecisionTree = DecisionTree[[1]]
  }
  
  return(DecisionTree)
}


DecisionTreePlot = function(DecisionTree)
{
  plot.new()
  par(mfrow=c(1,1))
  
  PossibleLeaves = c('LL vs LR','LL vs RL','LL vs RR','LR vs RL','LR vs RR','RL vs RR','L vs RL','L vs RR','LL vs R','LR vs R')
  
  plotrix::textbox(c(0.425,0.575), 1, DecisionTree$CentralNode$Variable,col="blue",lwd=2,justify='c')
  
  plotrix::textbox(c(0.1,0.3), 0.8, paste('<=',format(DecisionTree$CentralNode$Threshold,digits=4),sep=''),box=FALSE,justify='r')
  
  plotrix::textbox(c(0.7,0.9), 0.8, paste('>',format(DecisionTree$CentralNode$Threshold,digits=4),sep=''),box=FALSE,justify='l')
  
  
  if (!is.null(DecisionTree$LeftNode))
  {
    if (grepl('LL',DecisionTree$LeavesForFinalComparison) || grepl('LR',DecisionTree$LeavesForFinalComparison))
    {
      plotrix::textbox(c(0.125,0.275), 0.5, DecisionTree$LeftNode$Variable,col="blue",lwd=2,justify='c')
      lines(c(0.5,0.2),c(0.95,0.5),lwd=2)
    }
    else
    {
      plotrix::textbox(c(0.125,0.275), 0.6, DecisionTree$LeftNode$Variable,justify='c')
      lines(c(0.5,0.2),c(0.95,0.5))
    }
    
    plotrix::textbox(c(-0.025,0.125), 0.3, paste('<=',format(DecisionTree$LeftNode$Threshold,digits=4),sep=''),box=FALSE,justify='r')
    
    plotrix::textbox(c(0.275,0.475), 0.3, paste('>',format(DecisionTree$LeftNode$Threshold,digits=4),sep=''),box=FALSE,justify='l')
    
    if (grepl('LL',DecisionTree$LeavesForFinalComparison))
    {
      plotrix::textbox(c(-0.025,0.125), 0.05, 'Group1',col="blue",lwd=2,justify='c')
      lines(c(0.2,0.05),c(0.45,0.05),lwd=2)
    }
    else
    {
      plotrix::textbox(c(-0.025,0.125), 0.05, 'Group1',justify='c')
      lines(c(0.2,0.05),c(0.45,0.05))
    }
    
    if (grepl('LR',DecisionTree$LeavesForFinalComparison))
    {
      plotrix::textbox(c(0.275,0.425), 0.05, 'Group2',col="blue",lwd=2,justify='c')
      lines(c(0.2,0.35),c(0.45,0.05),lwd=2)
    }
    else
    {
      plotrix::textbox(c(0.275,0.425), 0.05, 'Group2',justify='c')
      lines(c(0.2,0.35),c(0.45,0.05))
    }
  }
  else
  {
    if (grepl('L',DecisionTree$LeavesForFinalComparison))
    {
      plotrix::textbox(c(0.125,0.275), 0.5, 'Group1',col="blue",lwd=2,justify='c')
      lines(c(0.5,0.2),c(0.95,0.5),lwd=2)
    }
    else
    {
      plotrix::textbox(c(0.125,0.275), 0.5, 'Group1',justify='c')
      lines(c(0.5,0.2),c(0.95,0.5))
    }
  }
  
  if (!is.null(DecisionTree$RightNode))
  {
    if (grepl('RL',DecisionTree$LeavesForFinalComparison) || grepl('RR',DecisionTree$LeavesForFinalComparison))
    {
      plotrix::textbox(c(0.725,0.875), 0.5, DecisionTree$RightNode$Variable,col="blue",lwd=2,justify='c')
      lines(c(0.5,0.8),c(0.95,0.5),lwd=2)
    }
    else
    {
      plotrix::textbox(c(0.725,0.875), 0.5, DecisionTree$RightNode$Variable,justify='c')
      lines(c(0.5,0.8),c(0.95,0.5))
    }
    
    plotrix::textbox(c(0.525,0.725), 0.3, paste('<=',format(DecisionTree$RightNode$Threshold,digits=4),sep=''),box=FALSE,justify='r')
    
    plotrix::textbox(c(0.875,1.075), 0.3, paste('>',format(DecisionTree$RightNode$Threshold,digits=4),sep=''),box=FALSE,justify='l')
    
    if (!is.null(DecisionTree$RightNode))
    {
      if (grepl('RL',DecisionTree$LeavesForFinalComparison))
      {
        plotrix::textbox(c(0.575,0.725), 0.05, 'Group3',col="blue",lwd=2,justify='c')
        lines(c(0.8,0.65),c(0.45,0.05),lwd=2)
      }
      else
      {
        plotrix::textbox(c(0.575,0.725), 0.05, 'Group3',justify='c')
        lines(c(0.8,0.65),c(0.45,0.05))
      }
      
      if (grepl('RR',DecisionTree$LeavesForFinalComparison))
      {
        plotrix::textbox(c(0.875,1.025), 0.05, 'Group4',col="blue",lwd=2,justify='c')
        lines(c(0.8,0.95),c(0.45,0.05),lwd=2)
      }
      else
      {
        plotrix::textbox(c(0.875,1.025), 0.05, 'Group4',justify='c')
        lines(c(0.8,0.95),c(0.45,0.05))
      }
    }
    else
    {
      if (grepl('RL',DecisionTree$LeavesForFinalComparison))
      {
        plotrix::textbox(c(0.575,0.725), 0.05, 'Group2',col="blue",lwd=2,justify='c')
        lines(c(0.8,0.65),c(0.45,0.05),lwd=2)
      }
      else
      {
        plotrix::textbox(c(0.575,0.725), 0.05, 'Group2',justify='c')
        lines(c(0.8,0.65),c(0.45,0.05))
      }
      
      if (grepl('RR',DecisionTree$LeavesForFinalComparison))
      {
        plotrix::textbox(c(0.875,1.025), 0.05, 'Group3',col="blue",lwd=2,justify='c')
        lines(c(0.8,0.95),c(0.45,0.05),lwd=2)
      }
      else
      {
        plotrix::textbox(c(0.875,1.025), 0.05, 'Group3',justify='c')
        lines(c(0.8,0.95),c(0.45,0.05))
      }
    }
  }
  
  else
  {
    if (!is.null(DecisionTree$LeftNode))
    {
      if (grepl('R',DecisionTree$LeavesForFinalComparison))
      {
        plotrix::textbox(c(0.725,0.875), 0.5, 'Group3',col="blue",lwd=2,justify='c')
        lines(c(0.5,0.8),c(0.95,0.5),lwd=2)
      }
      else
      {
        plotrix::textbox(c(0.725,0.875), 0.5, 'Group3',justify='c')
        lines(c(0.5,0.8),c(0.95,0.5))
      }
    }
    
    else
    {
      if (grepl('R',DecisionTree$LeavesForFinalComparison))
      {
        plotrix::textbox(c(0.725,0.875), 0.5, 'Group2',col="blue",lwd=2,justify='c')
        lines(c(0.5,0.8),c(0.95,0.5),lwd=2)
      }
      else
      {
        plotrix::textbox(c(0.725,0.875), 0.5, 'Group2',justify='c')
        lines(c(0.5,0.8),c(0.95,0.5))
      }
      
    }
    
  }
  
}

  
  
  