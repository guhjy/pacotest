%% SAtest toolbox %%

%% Testing the simplifying assumption in vine copulas

%% Description:
% The SAtest package provides functions, which allow to test the
% simplifying assumption in vine copulas. The package consists of three
% different test types, the Equal Copula (EC) test, the Equal Rank
% Correlation (ERC) test and the Vectorial Independence (VI) test.
% The function SAtestSet can be used to create / alter SAtest options
% structures and the function SAtest can be used to test the
% simplifying assumption for a single bivariate conditional copula.

%% Author:
% Malte Kurz

%% References :
% [1] Hobaek-Haff, I., K. Aas and A. Frigessi (2010), "On the simplified
% pair-copula construction -- Simply useful or too simplistic?", Journal
% of Multivariate Analysis 101(5), pp. 1296-1310.

% [2] Kojadinovic, I. and M. Holmes (2009), "Tests of independence among
% continuous random vectors based on Cramér-von Mises functionals of the
% empirical copula process", Journal of Multivariate Analysis 100(6), pp.
% 1137-1154.

% [3] Kurz, M. and F. Spanhel (2015), "Testing the simplifying assumption
% in vine copulas", Unpublished Working Paper.

% [4] Meinshausen, N., L. Meier, and P. Bühlmann (2009), “p-Values for
% High-Dimensional Regression”, Journal of the American Statistical
% Association 104(488), pp. 1671–1681.

% [5] Omelka, M. and M. Pauly (2012), "Testing equality of correlation
% coefficients in two populations via permutation methods, Journal of
% Statistical Planning and Inference 142, pp. 1396-1406.

% [6] Quessy, J.-F. (2010), "Applications and asymptotic power of marginal-
% free tests of stochastic vectorial independence", Journal of Statistical
% Planning and Inference 140(11), pp. 3058-3075.

% [7] Rémillard, B. and O. Scaillet (2009), "Testing for equality between
% two copulas", Journal of Multivariate Analysis 100(3), pp. 377-386.

% [8] Spanhel, F. and M. Kurz (2015), "Simplified vine copula models:
% Approximations based on the simplifying assumption", Unpublished Working
% Paper.

% [9] van de Wiel, M. A., J. Berkhof, and W. N. van Wieringen (2009),
% “Testing the prediction error difference between 2 predictors”,
% Biostatistics 10(3), pp. 550–560.


%% See also:
% Development for SAtest toolbox can be followed via the GitHub repository at http://github.com/MalteKurz/SAtest.


%% Examples:
%%%%%%%%%%%%%%%%%%%%%
% Generate an option file, e.g., the equal rank correlation (ERC)
% test with default options
SAtestOptions=SAtestSet('TestType','ERC','AggPvalsNumbRep',1,'GroupedScatterplots',true)

%%%%%%%%%%%%%%%%%%%%%
% Use the specified options to test the simplifying assumption

%%%%% Example 1: Non-simplified three-dim. C-Vine %%%%%
% Simulate from a three-dimensional C-Vine copula with C_12 and C_13
% being product copulas and C_23|1 being a Frank copula with
% functional parameter theta(x_{1}) = (4x_{1}-2)^3
N = 500;
X = rand(N,3);
theta = (4.*X(:,1)-2).^3;

etheta = expm1(-theta);
X(:,3) = -1./theta.*log(1+etheta./(exp(-theta.*X(:,2)).*(1./X(:,3)-1)+1));

Result = SAtest(X(:,2:3),X(:,1),SAtestOptions)


%%%%% Example 2: Non-simplified three-dim. C-Vine %%%%%
% Simulate from a three-dimensional C-Vine copula with C_12 and C_13
% being product copulas and C_23|1 being a Frank copula with
% functional parameter theta(x_{1}) = 12 + 8*sin(0.4(3x_{1}+2)^2)
X = rand(N,3);
theta = 12 + 8.*sin(0.4.*(3.*X(:,1)+2).^2);

etheta = expm1(-theta);
X(:,3) = -1./theta.*log(1+etheta./(exp(-theta.*X(:,2)).*(1./X(:,3)-1)+1));

Result = SAtest(X(:,2:3),X(:,1),SAtestOptions)


%%%%% Example 3: Simplified three-dim. C-Vine %%%%%
% Simulate from a three-dimensional C-Vine copula with C_12 and C_13
% being Clayton copulas with prameter theta and C_23|1 being a Clayton copula with
% functional parameter theta(x_{1}) = theta / (1+theta)
W = rand(N,3);
X = zeros(N,3);
theta = 2;

X(:,1) = W(:,1);
X(:,2) = (W(:,1).^(-theta).*(W(:,2).^((-theta)./(1+theta))-1)+1).^(-1./theta);
theta_23_1 = theta ./(1+theta);
X(:,3) = (W(:,2).^(-theta_23_1).*(W(:,3).^((-theta_23_1)./(1+theta_23_1))-1)+1).^(-1./theta_23_1);
X(:,3) = (W(:,1).^(-theta).*(X(:,3).^((-theta)./(1+theta))-1)+1).^(-1./theta);

% Get Pseudo-Obs from the conditional copula C_23|1
U = zeros(N,2);
U(:,1) = (X(:,1).^theta.*(X(:,2).^(-theta)-1)+1).^(-(1+theta)./theta);
U(:,2) = (X(:,1).^theta.*(X(:,3).^(-theta)-1)+1).^(-(1+theta)./theta);
Result = SAtest(U,X(:,1),SAtestOptions)

