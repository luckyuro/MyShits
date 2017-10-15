% descriptionP
m = [0.10 0.20 0.15]';
COV = [0.005 -0.010 0.004;
    -0.010 0.040 -0.002;
    0.004 -0.002 0.023];

[PRisk1, PRoR1, PWts1] = NaiveMV(m, COV, 100, 1)
[PRisk2, PRoR2, PWts2] = NaiveMV(m, COV, 100, 2)

scatter(PRisk1, PRoR1,'DisplayName','Original NavieMV');
hold on;
scatter(PRisk2, PRoR2,'x','DisplayName','NavieMV with CVX');
hold on;
legend('show')
xlabel('Risk (Variance)');
ylabel('Expected Return');