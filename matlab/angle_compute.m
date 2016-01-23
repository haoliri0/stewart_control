function [ theta ] = angle_compute( a,b,c,x0,y0,x1,y1 )
%ANGLE_COMPUTE Summary of this function goes here
%   Detailed explanation goes here
%   (a,b,c)�����Ӷ�ƽ̨����������ģ�
%   (x0,y0,0)�Ǽ������������꣬(x1,y1,0)�Ǽ�������ʼ״̬ʱ�¶�����ڵ�����
    r1 = sqrt((x1-x0)^2+(y1-y0)^2);   %���������ĵ��׶���������ľ���,r1 = ||(x1,y1)-(x0,y0)||   
    r2 = 400;  %���˵ĳ���
     
    % ����theta��֪����Բ�ϵĵ�����Ϊ
    % (x2,y2,z2) =��x0+cosd(theta)*(x1-x0),y0+cosd(theta)*(y1-y0),r1*sind(theta));
    % ���(a,b,c) - (x2,y2,z2)�ľ���Ϊr2
     
% %   matlab������ⷨ
     opt = optimset('TolX',1e-8);
     [theta,fv,ef,out] = fzero(@F_distance,[10,90],opt,a,b,c,x0,y0,x1,y1,r1,r2);

% %   �������ֶ����ֽⷨ��
%     ����ȷ��Բ�ϵ������������(x0,y0,r1);Ȼ��õ�����������x0-x1,y0-y1,0)�ͣ�0,0,r1)
%     ���������ת����ķ���Ϊaa*x + bb*y + c*z + dd = 0
%     ������(aa,bb,cc)��ֱ��������x0-x1,y0-y1,0)�ͣ�0,0,r1)
%     ��ˣ����ȵõ�c = 0,�����aa*(x0-x1) + bb*(y0-y1) = 0��ȡaa=1,�� bb = -(x0-x1)/(y0-y1) = (x1-x0)/(y0-y1)
%     ������һ����õ�dd
%     Ps:����Ҫ��y0-y1������0
    aa = 1;
    bb = (x1-x0)/(y0-y1);
    cc = 0;
    dd = -(aa*x0+bb*y0+cc*0); %�ѵõ���ķ���
%     ���߷���Ϊ��
%     (x-a)/aa = (y-b)/bb = (z-c)/cc = t
%     ��ƽ�淽������
    t = -(aa*a + bb*b + cc*c + dd)/(aa^2+bb^2+cc^2);
%     t
    xv = a + aa*t;
    yv = b + bb*t;
    zv = c + cc*t;
%     xv
%     yv
%     zv
%     ����������ͶӰ������(xv,yv,zv)��(x,y,0)�õ�����ʱ�Ķ˵�
    v1 = [xv-x0,yv-y0,zv];
    len1 = v1(1)^2 + v1(2)^2 + v1(3)^2;
    v2 = [x1-x0,y1-y0,0];
    len2 = v2(1)^2 + v2(2)^2 + v2(3)^2;
    l_theta = acosd(sum(v1.*v2)/sqrt(len1*len2));
%     l_theta
%     ���������õ���arcos��������ѧ�������п���������l_theta���ڣ�-pi/2,0��֮��ġ�
%     ����ʵ����ֻҪ(x1,y1,0)���ڲ����Ǹ�ˮƽ�㣨��ƽ̨�Ⱦ�ƽ̨С������ʵl_theta�϶���(0,,pi/2)֮��

%     ���������ж��� r_theta = l_theta �� pi
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
    % �ٴ�ʹ�ö��� while (rr-ll > 1e-2)
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
