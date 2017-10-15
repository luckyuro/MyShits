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
% randStart = unidrnd(T-30-windowSize);



startT = windowSize+1;
impliedVolatilities = zeros(T, 1);
calculatedVolatilities = zeros(T, 1);
strike = 3325;
option = p3325;
endT = startT+30;
for t = startT:endT
    impliedVolatilities(t) = blsimpv(price(t), strike, 0.06, (T + 1 - t) / 252, option(t));
    window = price(t-windowSize:t);
    u = tick2ret(window, [], 'Continuous');
    s = std(u);
    N = size(window, 1);
    calculatedVolatilities(t) = s / sqrt(1 / 252);
end

startT0 = windowSize+41;
impliedVolatilities0 = zeros(T, 1);
calculatedVolatilities0 = zeros(T, 1);
strike0 = 3325;
option0 = p3325;
endT0 = startT0+30;
for t = startT0:endT0
    impliedVolatilities0(t) = blsimpv(price(t), strike0, 0.06, (T + 1 - t) / 252, option0(t));
    window = price(t-windowSize:t);
    u = tick2ret(window, [], 'Continuous');
    s = std(u);
    N = size(window, 1);
    calculatedVolatilities0(t) = s / sqrt(1 / 252);
end

startT1 = windowSize+81;
impliedVolatilities1 = zeros(T, 1);
calculatedVolatilities1 = zeros(T, 1);
strike1 = 3325;
option1 = p3325;
endT1 = startT1+30;
for t = startT1:endT1
    impliedVolatilities1(t) = blsimpv(price(t), strike1, 0.06, (T + 1 - t) / 252, option1(t));
    window = price(t-windowSize:t);
    u = tick2ret(window, [], 'Continuous');
    s = std(u);
    N = size(window, 1);
    calculatedVolatilities1(t) = s / sqrt(1 / 252);
end

startT2 = windowSize+120;
impliedVolatilities2 = zeros(T, 1);
calculatedVolatilities2 = zeros(T, 1);
strike2 = 3325;
option2 = p3325;
endT2 = startT2+30;
for t = startT2:endT2
    impliedVolatilities2(t) = blsimpv(price(t), strike2, 0.06, (T + 1 - t) / 252, option2(t));
    window = price(t-windowSize:t);
    u = tick2ret(window, [], 'Continuous');
    s = std(u);
    N = size(window, 1);
    calculatedVolatilities2(t) = s / sqrt(1 / 252);
end

% startT3 = windowSize+140;
% impliedVolatilities3 = zeros(T, 1);
% calculatedVolatilities3 = zeros(T, 1);
% strike3 = 3325;
% option3 = c3325;
% endT3 = startT3+30;
% for t = startT3:endT3
%     impliedVolatilities3(t) = blsimpv(price(t), strike3, 0.06, (T + 1 - t) / 252, option3(t));
%     window = price(t-windowSize:t);
%     u = tick2ret(window, [], 'Continuous');
%     s = std(u);
%     N = size(window, 1);
%     calculatedVolatilities3(t) = s / sqrt(1 / 252);
% end

figure;
scatter(impliedVolatilities(startT:endT), calculatedVolatilities(startT:endT),'filled','DisplayName','day 56');
hold on;
scatter(impliedVolatilities0(startT0:endT0), calculatedVolatilities0(startT0:endT0),'filled','DisplayName','day 96');
hold on;
scatter(impliedVolatilities1(startT1:endT1), calculatedVolatilities1(startT1:endT1),'filled','DisplayName','day 136');
hold on;
scatter(impliedVolatilities2(startT2:endT2), calculatedVolatilities2(startT2:endT2),'filled','DisplayName','day 175');
% scatter(impliedVolatilities3(startT3:endT3), calculatedVolatilitiesZ(startT3:endT3));
hold on;
plot(0:2,0:2,'DisplayName','y=x');
title(sprintf('Implied volatility vs estimated volatility(put at 3325)'));
minX = min([min(impliedVolatilities(startT:endT)),min(impliedVolatilities0(startT0:endT0)), ...
    min(impliedVolatilities1(startT1:endT1)),min(impliedVolatilities2(startT2:endT2))]);
maxX = max([max(impliedVolatilities),max(impliedVolatilities0),max(impliedVolatilities1),max(impliedVolatilities2)]);
minY = min([min(calculatedVolatilities(startT:endT)),min(calculatedVolatilities0(startT0:endT0)), ...
    min(calculatedVolatilities1(startT1:endT1)),min(calculatedVolatilities2(startT2:endT2))]);
maxY = max([max(calculatedVolatilities),max(calculatedVolatilities0),max(calculatedVolatilities1),max(calculatedVolatilities2)]);

xlim([min([minX,minY]), maxX]);
ylim([min([minX,minY]),maxY]);
xlabel('Implied volatility');
ylabel('Estmated volatility');
legend('show','Location','best');



strikePrices = (2925:100:3325)';
impliedVolatilities = zeros(size(strikePrices, 1), 1);

t = 170;
for n = 1:size(strikePrices, 1)
    strikePrice = strikePrices(n);
    callPut = cpp(strikePrice);
    priceData = callPut(:,2);
    T = size(priceData, 1);
    impliedVolatilities(n) = blsimpv(price(t), strikePrice, 0.06, (T + 1 - t) / 252, priceData(t));
end

figure;
scatter(strikePrices, impliedVolatilities,'filles');
hold on;
newS = 2925-strikePrices(5:-1:2)+2810;
newI = impliedVolatilities(5:-1:2);
scatter(newS,newI,'filled');
title(sprintf('Refined:Strike price - implied volatility(put) for day %d', t));
xlabel('Strike price');
ylabel('Implied volatility');
