% description
m = [0.10 0.20 0.15]';
COV = [0.005 -0.010 0.004;
    -0.010 0.040 -0.002;
    0.004 -0.002 0.023];

% question 1
num_samples = 100;
num_assets = 3;
pc = zeros(num_samples,num_assets);
for n = 1:num_samples
    x = rand(1,num_assets);
    pc(n,:) = x./sum(x);
end
eReturn = pc*m;
Variance = ones(num_samples,1);
for n = 1:num_samples
    Xs = pc(n,:);
    PostCOV = [Xs;Xs;Xs].*COV; %COV==COV'
    v = Xs*PostCOV*ones(num_assets,1);
    Variance(n,:)=v;
end
SD = sqrt(Variance);
figure(1)
% scatter(eReturn, Variance,'filled')
% ylabel('Risk (Variance)');
% xlabel('Expected Return');
% title('100 random portfolios');



%question 2
p = Portfolio;
p = setAssetMoments(p, m, COV);
p = setDefaultConstraints(p);
% asset12
m12 = [m(1) m(2)]';
COV12 = [COV(1,1) COV(1,2);COV(2,1) COV(2,2)];
pc12 = zeros(num_samples,2);
for n = 1:num_samples
    x = rand(1,2);
    pc12(n,:) = [x(1)/sum(x) x(2)/sum(x)];
end
eReturn12 = pc12*m12;
Variance12 = ones(num_samples,1);
for n = 1:num_samples
    Xs = pc12(n,:);
    PostCOV = [Xs;Xs].*COV12; %COV==COV'
    v = Xs*PostCOV*ones(2,1);
    Variance12(n,:)=v;
end
SD12 = sqrt(Variance12);
%asset23
m23 = [m(2) m(3)]';
COV23 = [COV(2,2) COV(2,3);COV(3,2) COV(3,3)];
pc23 = zeros(num_samples,2);
for n = 1:num_samples
    x = rand(1,2);
    pc23(n,:) = [x(1)/sum(x) x(2)/sum(x)];
end
eReturn23 = pc23*m23;
Variance23 = ones(num_samples,1);
for n = 1:num_samples
    Xs = pc23(n,:);
    PostCOV = [Xs;Xs].*COV23; %COV==COV'
    v = Xs*PostCOV*ones(2,1);
    Variance23(n,:)=v;
end
SD23 = sqrt(Variance23);
%asset13
m13 = [m(1) m(3)]';
COV13 = [COV(1,1) COV(1,3);COV(3,1) COV(3,3)];
pc13 = zeros(num_samples,2);
for n = 1:num_samples
    x = rand(1,2);
    pc13(n,:) = [x(1)/sum(x) x(2)/sum(x)];
end
eReturn13 = pc13*m13;
Variance13 = ones(num_samples,1);
for n = 1:num_samples
    Xs = pc13(n,:);
    PostCOV = [Xs;Xs].*COV13; %COV==COV'
    v = Xs*PostCOV*ones(2,1);
    Variance13(n,:)=v;
end
SD13 = sqrt(Variance13);


%plots
%plotmatrix(Variance, eReturn);

scatter(SD, eReturn, 'filled');
hold on;

plotFrontier(p);
hold on
figure(2)
% plotFrontier(p);
% hold on

p12 = Portfolio;
p12 = setAssetMoments(p12, m12, COV12);
p12 = setDefaultConstraints(p12);

p23 = Portfolio;
p23 = setAssetMoments(p23, m23, COV23);
p23 = setDefaultConstraints(p23);

p13 = Portfolio;
p13 = setAssetMoments(p13, m13, COV13);
p13 = setDefaultConstraints(p13);



% 
scatter(SD, eReturn, 'filled');
hold on;
scatter(SD12, eReturn12);
hold on;
scatter(SD23, eReturn23);
hold on;
scatter(SD13, eReturn13);

plotFrontier(p12);
hold on
plotFrontier(p23);
hold on
plotFrontier(p13);
hold on;

%question3



%question 4

