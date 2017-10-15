all = all_New();
ftse = all(:,1);
assets = all(:,2:36);
format long
y = tick2ret(ftse);
R = tick2ret(assets);

[T, N] = size(R);

w = zeros(35, 1);
% for n = 1:7
%     best_norm = Inf;
%     best_w = w;
%     for i = 1:35
%         if best_w(i) > 0
%            continue;
%         end
%         new_w = w;
%         new_w(i) = 1;
%         new_norm = norm(y - R * new_w) ^ 2;
%         if new_norm < best_norm
%             best_norm = new_norm;
%             best_w = new_w;
%         end
%     end
%     w = best_w;
% end

cvx_begin quiet
variable w(N)
    minimize(square_pos(norm(y - R * w)));
cvx_end

w = w / norm(w, 1);

disp(norm(y - R * w) ^ 2);
y2 = R * w;
figure(1);
bar(w);
title('Index-tracking portfolio using greedy search algorithm');
ylabel('Weight');
xlabel('Asset number');
figure(2);
[ax, p1, p2] = plotyy(1:T, cumsum(y), 1:T, cumsum(y2));
title('Index-tracking portfolio using greedy search algorithm');
ylabel(ax(1), 'Return on FTSE 100');
ylabel(ax(2), 'Return on selected portfolio');
xlabel('Time');
set(ax(1), 'YLim', [median(cumsum(y)) - range(cumsum(y)), median(cumsum(y)) + range(cumsum(y))]);
set(ax(2), 'YLim', [median(cumsum(y2)) - range(cumsum(y2)), median(cumsum(y2)) + range(cumsum(y2))]);