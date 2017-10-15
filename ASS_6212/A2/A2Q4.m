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
cpp = containers.Map({2925,3025,3125,3225,3325},{[c2925,p2925], ...
    [c3025,p3025],[c3125,p3125],[c3225,p3225],[c3325,p3325]});

T = size(price,1);
windowSize = floor(T/4);

t = 75;


option = c3125;
optionPrice = 3125;
flag = 1;

window = option((t-windowSize):t);
u = tick2ret(window, [], 'Continuous');
s = std(u);
N = size(window, 1);
volatility = s / sqrt(1 / 252);
 

diffs = zeros(100, 1);
deltaTs = linspace(0.01, 0.2, 100);
for n = 1:100
    [call, put] = blsprice(option(t), optionPrice, 0.06, (T + 1 - t) / 252, volatility);
    [AssetPrice, OptionValue] = binprice(option(t), optionPrice, 0.06, (T + 1 - t) / 252, deltaTs(n), volatility, flag);
    diffs(n) = abs(call - OptionValue(1, 1));
end
scatter(deltaTs, diffs);
xlabel('Delta T');
ylabel('Black-Scholes estimate - Binomial Lattice estimate');
