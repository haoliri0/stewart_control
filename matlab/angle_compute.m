function [ theta ] = angle_compute( a,b,c,x0,y0,x1,y1 )
%ANGLE_COMPUTE Summary of this function goes here
%   Detailed explanation goes here
%   (a,b,c)是联接动平台的万向节轴心，
%   (x0,y0,0)是减速器轴心坐标，(x1,y1,0)是减速器初始状态时下端万向节的轴心
    r1 = sqrt((x1-x0)^2+(y1-y0)^2);   %减速器轴心到底端万向节轴心距离,r1 = ||(x1,y1)-(x0,y0)||   
    r2 = 400;  %定杆的长度
     
    % 假设theta已知，则圆上的点坐标为
    % (x2,y2,z2) =（x0+cosd(theta)*(x1-x0),y0+cosd(theta)*(y1-y0),r1*sind(theta));
    % 因此(a,b,c) - (x2,y2,z2)的距离为r2
     
% %   matlab工具箱解法
     opt = optimset('TolX',1e-8);
     [theta,fv,ef,out] = fzero(@F_distance,[10,90],opt,a,b,c,x0,y0,x1,y1,r1,r2);

% %   如下是手动二分解法：
%     首先确定圆上第三个点的坐标(x0,y0,r1);然后得到两个向量（x0-x1,y0-y1,0)和（0,0,r1)
%     设减速器旋转的面的方程为aa*x + bb*y + c*z + dd = 0
%     则向量(aa,bb,cc)垂直于向量（x0-x1,y0-y1,0)和（0,0,r1)
%     因此，首先得到c = 0,其次是aa*(x0-x1) + bb*(y0-y1) = 0，取aa=1,则 bb = -(x0-x1)/(y0-y1) = (x1-x0)/(y0-y1)
%     最后代入一个点得到dd
%     Ps:这里要求y0-y1不等于0
    aa = 1;
    bb = (x1-x0)/(y0-y1);
    cc = 0;
    dd = -(aa*x0+bb*y0+cc*0); %已得到面的方程
%     垂线方程为：
%     (x-a)/aa = (y-b)/bb = (z-c)/cc = t
%     和平面方程联立
    t = -(aa*a + bb*b + cc*c + dd)/(aa^2+bb^2+cc^2);
%     t
    xv = a + aa*t;
    yv = b + bb*t;
    zv = c + cc*t;
%     xv
%     yv
%     zv
%     接下来根据投影点坐标(xv,yv,zv)和(x,y,0)得到二分时的端点
    v1 = [xv-x0,yv-y0,zv];
    len1 = v1(1)^2 + v1(2)^2 + v1(3)^2;
    v2 = [x1-x0,y1-y0,0];
    len2 = v2(1)^2 + v2(2)^2 + v2(3)^2;
    l_theta = acosd(sum(v1.*v2)/sqrt(len1*len2));
%     l_theta
%     这里由于用的是arcos，所以数学意义上有可能真正的l_theta是在（-pi/2,0）之间的。
%     但是实际上只要(x1,y1,0)是内部的那个水平点（动平台比静平台小），其实l_theta肯定在(0,,pi/2)之间

%     接下来进行二分 r_theta = l_theta ± pi
    rr = l_theta;
    ll = rr - 180;
    while (rr-ll > 1e-2)
        mid = (ll+rr)/2;
        if F_distance(mid,a,b,c,x0,y0,x1,y1,r1,r2) * F_distance(rr,a,b,c,x0,y0,x1,y1,r1,r2) < 1e-10
            ll = mid;
        else
            rr = mid;
        end;
    end;
    
%     ll
    
    rr = l_theta + 180;
    ll = rr - 180;
    % 再次使用二分 while (rr-ll > 1e-2)
    while (rr-ll > 1e-2)
        mid = (ll+rr)/2;
        if F_distance(mid,a,b,c,x0,y0,x1,y1,r1,r2) * F_distance(rr,a,b,c,x0,y0,x1,y1,r1,r2) < 1e-10
            ll = mid;
        else
            rr = mid;
        end;
    end;
    
%     ll
end

