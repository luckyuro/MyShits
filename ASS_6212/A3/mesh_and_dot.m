function [] = mesh_and_dot(X, Y, Z)
    figure;
%     F = TriScatteredInterp(X, Y, Z);
    F = scatteredInterpolant(X, Y, Z);
    rx = (max(X) - min(X)) / 20;
    ry = (max(Y) - min(Y)) / 20;
    [qx,qy] = meshgrid(min(X):rx:max(X), min(Y):ry:max(Y));
    qz = F(qx,qy);
    mesh(qx,qy,qz);
    hold on;
    zlim([min(Z),max(Z)]);
    scatter3(X, Y, Z,'.');
end