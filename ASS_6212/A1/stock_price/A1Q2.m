% all = all_Ratio();
all_new = all_New();
all = tick2ret(all_new);
ftse = all(:,1);
sample = all(1:380,[5,6,9]);
test = all(380:783,[5,6,9]);
format long
% m = mean(all);
% COV = cov(all);
sample_m = mean(sample);
sample_cov = cov(sample);

sample_p = Portfolio;
sample_p = setAssetMoments(sample_p, sample_m, sample_cov);
sample_p = setDefaultConstraints(sample_p);
% plotFrontier(sample_p);

% num_samples = 100;
% num_assets = 3;
% pc = zeros(num_samples,num_assets);
% m = sample_m';
% COV = sample_cov;
% 
% for n = 1:num_samples
%     x = rand(1,num_assets);
%     pc(n,:) = x./sum(x);
% end
% eReturn = pc*m;
% Variance = ones(num_samples,1);
% for n = 1:num_samples
%     Xs = pc(n,:);
%     PostCOV = [Xs;Xs;Xs].*COV; %COV==COV'
%     v = Xs*PostCOV*ones(num_assets,1);
%     Variance(n,:)=v;
% end
% SD = sqrt(Variance);
% figure(1)
% scatter(SD, eReturn,'filled')
% hold on

opted_weights = estimateFrontier(sample_p,1000);

% cvx_begin quiet
% variable w(3)
%     maximize(w' * sample_m')
%     subject to
%         w' * sample_cov * w <= 0.008 ^ 2;
%         w' * ones(3, 1) == 1;
%         w >= 0;
% cvx_end
eqWeight = [1,1,1]';
eqWeight = eqWeight ./ sum(eqWeight);

% sharpe_1 = sharpe(test * w);
% sharpe_2 = sharpe(test * eqWeight);
% total_1 = sum(test * w);
% total_2 = sum(test * eqWeight);
ncol = size(opted_weights,2);
max_sharpe = 0
for k = 1:ncol
    w = opted_weights(:,k);
    sharpe_tmp = sharpe(sample * w);
    if sharpe_tmp >max_sharpe
        max_sharpe = sharpe_tmp;
        max = k;
        max_p = w;
    end
end

plot(cumsum(test*max_p),'DisplayName',sprintf('Efficient portfolio'));
hold on;
% plot(cumsum(test * w));
% hold on;
plot(cumsum(test * eqWeight),'DisplayName','1/N portfolio');

title('Cumulative return over time for efficient and 1/N portfolios');
ylabel('Return');
xlabel('Time');
legend('show')
