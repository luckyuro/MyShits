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
startT = windowSize+1;

for strike_price = 2925:100:3325
    blsCallPrice = zeros(T,1);
    blsPutPrice = zeros(T,1);
    for t = startT:T
        window = price((t-windowSize):t);
        u = tick2ret(window, [], 'Continuous');
        s = std(u);
        N = size(window, 1);
        volatility = s / sqrt(1 / 252);
        [call, put] = blsprice(price(t), strike_price, ...
            0.06, (T + 1 - t) / 252, volatility);
        blsCallPrice(t) = call;
        blsPutPrice(t) = put;
    end
    prices = cpp(strike_price);
    callPrice = prices(:,1);
    putPrice = prices(:,2);
    figure;
    plot(blsCallPrice(1:T));
    hold on;
    plot(callPrice(1:T));
    title(sprintf('call at %d', strike_price));
    legend('Estimated price', 'Actual price');
    xlabel('Time');
    ylabel('Price');
    figure;
    plot(blsPutPrice(1:T));
    hold on;
    plot(putPrice(1:T));
    title(sprintf('put at %d', strike_price));
    legend('Estimated price', 'Actual price');
    xlabel('Time');
    ylabel('Price');
end
