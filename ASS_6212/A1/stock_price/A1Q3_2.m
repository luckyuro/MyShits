all = all_New();
ftse = all(:,1);
assets = all(:,2:36);
format long
y = tick2ret(ftse);
R = tick2ret(assets);

[T, N] = size(R);

tau = 0.1;

cvx_begin quiet
variable w(N)
    minimize(square_pos(norm(y - R * w)) + tau * norm(w, 1));
cvx_end

w = w / norm(w, 1);

disp(norm(y - R * w) ^ 2);
y2 = R * w;
figure(1);
bar(w);
title('Index-tracking portfolio using l1-regularization algorithm');
ylabel('Weight');
xlabel('Asset number');
figure(2);
[ax, p1, p2] = plotyy(1:T, cumsum(y), 1:T, cumsum(y2));
title('Return against time');
ylabel(ax(1), 'Return on FTSE 100');
ylabel(ax(2), 'Return on selected portfolio');
xlabel('Time');
% set(ax(1), 'YLim', [median(ftse) - range(ftse), median(ftse) + range(ftse)]);
% set(ax(2), 'YLim', [median(y2) - range(y2), median(y2) + range(y2)]);