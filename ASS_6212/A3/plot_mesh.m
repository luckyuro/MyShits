function [] = plot_mesh(X, Y, Z)
    figure;
%     F = TriScatteredInterp(X, Y, Z);
    F = scatteredInterpolant(X, Y, Z);
    rx = (max(X) - min(X)) / 40;
    ry = (max(Y) - min(Y)) / 40;
    [qx,qy] = meshgrid(min(X):rx:max(X), min(Y):ry:max(Y));
    qz = F(qx,qy);
    mesh(qx,qy,qz);
    hold on;
%     zlim([min(Z),max(Z)]);
end

