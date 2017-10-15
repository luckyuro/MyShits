function [PRisk, PRoR, PWts] = NaiveMV(ERet, ECov, NPts, type)

switch(type)
    case 1
        [PRisk, PRoR, PWts] = NaiveMV_Origin(ERet, ECov, NPts);
    case 2
        [PRisk, PRoR, PWts] = NaiveMV_CVX(ERet, ECov, NPts);
end

end

function [PRisk, PRoR, PWts] = NaiveMV_Origin(ERet, ECov, NPts)
ERet = ERet(:);      % makes sure it is a column vector
NAssets = length(ERet);  % get number of assets
% vector of lower bounds on weights
V0 = zeros(NAssets, 1);
% row vector of ones
V1 = ones(1, NAssets);
% set medium scale option
options = optimset('LargeScale', 'off');
% Find the maximum expected return
MaxReturnWeights = linprog(-ERet, [], [], V1, 1, V0);
MaxReturn = MaxReturnWeights' * ERet;
% Find the minimum variance return
MinVarWeights = quadprog(ECov,V0,[],[],V1,1,V0,[],[],options);
MinVarReturn = MinVarWeights' * ERet;
MinVarStd = sqrt(MinVarWeights' * ECov * MinVarWeights);
% check if there is only one efficient portfolio
if MaxReturn > MinVarReturn
   RTarget = linspace(MinVarReturn, MaxReturn, NPts);
   NumFrontPoints = NPts;
else
    RTarget = MaxReturn;
    NumFrontPoints = 1;
end
% Store first portfolio
PRoR = zeros(NumFrontPoints, 1);
PRisk = zeros(NumFrontPoints, 1);
PWts = zeros(NumFrontPoints, NAssets);
PRoR(1) = MinVarReturn;
PRisk(1) = MinVarStd;
PWts(1,:) = MinVarWeights(:)';
% trace frontier by changing target return
VConstr = ERet';
A = [V1 ; VConstr ];
B = [1 ; 0];
for point = 2:NumFrontPoints
    B(2) = RTarget(point);
    Weights = quadprog(ECov,V0,[],[],A,B,V0,[],[],options);
    PRoR(point) = dot(Weights, ERet);
    PRisk(point) = sqrt(Weights'*ECov*Weights);
    PWts(point, :) = Weights(:)';
end

end


function [PRisk, PRoR, PWts] = NaiveMV_CVX(ERet, ECov, NPts)
ERet = ERet(:);      % makes sure it is a column vector
NAssets = length(ERet);  % get number of assets
% vector of lower bounds on weights
V0 = zeros(NAssets, 1);
% row vector of ones
V1 = ones(1, NAssets);
% set medium scale option
options = optimset('LargeScale', 'off');
% Find the maximum expected return
cvx_quiet true
% CVX replace
%MaxReturnWeights = linprog(-ERet, [], [], V1, 1, V0);
cvx_begin
    variable MaxReturnWeights(NAssets)
    maximize( ERet'*MaxReturnWeights )
    subject to
         V1*MaxReturnWeights == 1
         0 <= MaxReturnWeights <= 1
cvx_end
%MaxReturnWeights = x;
%CVX replace end

MaxReturn = MaxReturnWeights' * ERet;
% Find the minimum variance return

%CVX replace
%MinVarWeights = quadprog(ECov,V0,[],[],V1,1,V0,[],[],options);
cvx_begin
    variable MinVarWeights(NAssets)
    minimize(MinVarWeights'*ECov*MinVarWeights)
    subject to
        V1*MinVarWeights == 1
        0 <= MinVarWeights <= 1
cvx_end
% MinVarWeights = x;
%CVX replace end
MinVarReturn = MinVarWeights' * ERet;
MinVarStd = sqrt(MinVarWeights' * ECov * MinVarWeights);
% check if there is only one efficient portfolio
if MaxReturn > MinVarReturn
   RTarget = linspace(MinVarReturn, MaxReturn, NPts);
   NumFrontPoints = NPts;
else
    RTarget = MaxReturn;
    NumFrontPoints = 1;
end
% Store first portfolio
PRoR = zeros(NumFrontPoints, 1);
PRisk = zeros(NumFrontPoints, 1);
PWts = zeros(NumFrontPoints, NAssets);
PRoR(1) = MinVarReturn;
PRisk(1) = MinVarStd;
PWts(1,:) = MinVarWeights(:)';
% trace frontier by changing target return
VConstr = ERet';
A = [V1 ; VConstr ];
B = [1 ; 0];
for point = 2:NumFrontPoints
    B(2) = RTarget(point);
    %CVX replace
    %Weights = quadprog(ECov,V0,[],[],A,B,V0,[],[],options);
    cvx_begin
        variable x(NAssets)
        minimize(x'*ECov*x)
        subject to
            A*x == B
            0 <= x <= 1
    cvx_end
    Weights = x;
    %CVX replace
    PRoR(point) = dot(Weights, ERet);
    PRisk(point) = sqrt(Weights'*ECov*Weights);
    PWts(point, :) = Weights(:)';
end

end