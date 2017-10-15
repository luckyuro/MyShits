% function [ output_args ] = cal_2_asset( input_args )
%CAL_2_ASSET Summary of this function goes here
%   Detailed explanation goes here
function [expReturn, Variance, StdDeviation] = cal_2_asset(Mean, CoVariance, num_samples)

pc_ = zeros(num_samples,2);
for n = 1:num_samples
    x = rand(1,2);
    pc_(n,:) = [x(1)/sum(x) x(2)/sum(x)];
end
eReturn_ = pc*Mean;
Variance_ = ones(num_samples,1);
for n = 1:num_samples
    Xs = pc_(n,:);
    PostCOV = [Xs;Xs].*CoVariance; %COV==COV'
    v = Xs*PostCOV*ones(2,1);
    Variance_(n,:)=v;
end
SD_ = sqrt(Variance_);
expReturn = eReturn_;
Variance = Variance_;
StdDeviation = SD_;

end

%end

