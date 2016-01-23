function [ output ] = F_distance( theta,a,b,c,x,y,x1,y1,r1,r2 )
%F_DISTANCE Summary of this function goes here
%   Detailed explanation goes here
    vec = [a, b, c] -[x+cosd(theta)*(x1-x), y+cosd(theta)*(y1-y), r1*sind(theta)];
    output = sum((vec).^2) - r2^2;
end

