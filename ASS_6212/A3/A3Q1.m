data = GET_DATA;
price = data(:,2);
c2925 = data(:,3);
c3025 = data(:,4);
c3125 = data(:,5);
c3225 = data(:,6);
c3325 = data(:,7);
p2925 = data(:,8);
p3025 = data(:,9);
p3125 = data(:,10);
p3225 = data(:,11);
p3325 = data(:,12);


strike_prices = [ones(222, 1) * 2925; ones(222, 1) * 3025; ones(222, 1) * 3125; ones(222, 1) * 3225; ones(222, 1) * 3325];
% Actual prices normalised by strike price (S/X)
actual_prices = [price/2925; price/3025; price/3125; price/3225; price/3325];
% Times (T - t)
t = linspace(223 / 252, 1/252, 222)';
times = [t; t; t; t; t];


N = size(price, 1);
volatility = std(tick2ret(price, [], 'Continuous')) / sqrt(N / 252);
[call, put] = blsprice(price, 2925, 0.06, t, volatility);
[delta2925, a] = blsdelta(price, 2925, 0.06, t, volatility);
call2925 = call / 2925;


N = size(price, 1);
volatility = std(tick2ret(price, [], 'Continuous')) / sqrt(N / 252);
[call, put] = blsprice(price, 3025, 0.06, t, volatility);
[delta3025, ~] = blsdelta(price, 3025, 0.06, t, volatility);
call3025 = call / 3025;

N = size(price, 1);
volatility = std(tick2ret(price, [], 'Continuous')) / sqrt(N / 252);
[call, put] = blsprice(price, 3125, 0.06, t, volatility);
[delta3125, ~] = blsdelta(price, 3125, 0.06, t, volatility);
call3125 = call / 3125;

N = size(price, 1);
volatility = std(tick2ret(price, [], 'Continuous')) / sqrt(N / 252);
[call, put] = blsprice(price, 3225, 0.06, t, volatility);
[delta3225, ~] = blsdelta(price, 3225, 0.06, t, volatility);
call3225 = call / 3225;

N = size(price, 1);
volatility = std(tick2ret(price, [], 'Continuous')) / sqrt(N / 252);
[call, put] = blsprice(price, 3325, 0.06, t, volatility);
[delta3325, ~] = blsdelta(price, 3325, 0.06, t, volatility);
call3325 = call / 3325;

% Black-Scholes predicted call option prices normalised by strike price
% (C/X)
bls_prices = [call2925; call3025; call3125; call3225; call3325];

real_prices = [c2925/2925; c3025/3025; c3125/3125; c3225/3225; c3325/3325];

bls_deltas = [delta2925; delta3025; delta3125; delta3225; delta3325];

% mesh_and_dot(actual_prices, times, bls_prices);
plot_mesh(actual_prices, times, bls_prices);
scatter3(actual_prices, times, bls_prices, 14, 'filled');
xlabel('S/X');
ylabel('T - t');
zlabel('C/X');
% title('Pricing and Hedging Derivatives')