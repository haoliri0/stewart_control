function [ out ] = main_stewart( xx,yy,zz,aa,bb,cc )

%% ���嶨ƽ̨�ϵ�6���㣬����������˵�

radius1 = 327.54;
distance1 = 340;
motorPoint = [distance1 / 2,    radius1,    0,  1;...   %�����������
              -distance1 / 2,   radius1,    0,  1];
c = 120;
Rc = [cosd(c),   -sind(c),  0, 0;...
      sind(c),  cosd(c),  0, 0;...
      0,         0,        1, 0;...
      0,         0,        0, 1];
pointTemp1 = [Rc * motorPoint']';       %����������
pointTemp2 = [Rc * pointTemp1']';       %�Ҳ��������
motorPoint = [motorPoint;pointTemp1;pointTemp2];

% motorPoint = [motorPoint;...
%               distance1 / 2,    radius1,    0,  1]; %�γ�һ���ջ������ڻ�ͼ
%plot3(motorPoint(:,1),motorPoint(:,2),motorPoint(:,3),'o-','Color','r');
%xlabel('x axis');
%ylabel('y axis');
%zlabel('z axis');
%hold on;
%% ����������������6������ڵ�����
y_d = 320;
% ���������ĵ��¶���������ĵľ���Ϊsqrt(150^2+44^2)
x_d = 340+(2*156.3202);
hingePoint = [x_d / 2,    y_d,    0,  1;...   %�����������
              -x_d / 2,   y_d,    0,  1];
c = 120;
Rc = [cosd(c),   -sind(c),  0, 0;...
      sind(c),  cosd(c),  0, 0;...
      0,         0,        1, 0;...
      0,         0,        0, 1];
pointTemp1 = [Rc * hingePoint']';       %����������
pointTemp2 = [Rc * pointTemp1']';       %�Ҳ��������
hingePoint = [hingePoint;pointTemp1;pointTemp2];

% motorPoint = [motorPoint;...
%               distance1 / 2,    radius1,    0,  1]; %�γ�һ���ջ������ڻ�ͼ
%plot3(hingePoint(:,1),hingePoint(:,2),hingePoint(:,3),'o-','Color','d');
%xlabel('x axis');
%ylabel('y axis');
%zlabel('z axis');
%hold on;
%% ���嶯ƽ̨�ο��� topPoint

% distance1 = 374.2641;
distance1 = 100;
radius1 = sqrt(250^2-(distance1/2)^2);

topPoint = [distance1 / 2,    radius1,    0,  1;...   %�����������
              -distance1 / 2,   radius1,    0,  1]; %(��50,244.94)
c = 120;
Rc = [cosd(c),   -sind(c),  0, 0;...
      sind(c),  cosd(c),  0, 0;...
      0,         0,        1, 0;...
      0,         0,        0, 1];
pointTemp1 = [Rc * topPoint']';       %���������� (-237.132,-79.173) (-187.1320,-165.7758)
pointTemp2 = [Rc * pointTemp1']';       %�Ҳ�������� (187.1320 -165.7758) 237.1320  -79.1732
topPoint = [topPoint;pointTemp1;pointTemp2];
 
for i = 1:length(topPoint)
   toptemp(i,:) = Inverse(xx,yy,zz,aa,bb,cc,topPoint(i,:) );
end
topPoint = toptemp;

% ������ƽ̨�Ĳο���
%plot3(topPoint(:,1),topPoint(:,2),topPoint(:,3),'o-','Color','b');

%% ��������������,������˳�
barLength = zeros(length(topPoint),1);
for i = 1:length(topPoint)
%    plot3([motorPoint(i,1);topPoint(i,1)], ...
%          [motorPoint(i,2);topPoint(i,2)], ...
%          [motorPoint(i,3);topPoint(i,3)], ...
%          'g');
      barLength(i) = sqrt((motorPoint(i,1)-topPoint(i,1))^2 ...
                       + (motorPoint(i,2)-topPoint(i,2))^2 ...
                       + (motorPoint(i,3)-topPoint(i,3))^2 );
end
% barLength
% axis equal

% (ע�⣬theta����-16.3482ʱ��ʾ��������������������ˮƽ)
% arctand(44/150) = 16.3482
[ theta_x ] = angle_compute( topPoint(2,1),topPoint(2,2),topPoint(2,3),motorPoint(2,1),motorPoint(2,2),hingePoint(2,1),hingePoint(2,2) ) - 16.3482;
[ theta_u ] = angle_compute( topPoint(3,1),topPoint(3,2),topPoint(3,3),motorPoint(3,1),motorPoint(3,2),hingePoint(3,1),hingePoint(3,2) ) - 16.3482;
[ theta_y ] = angle_compute( topPoint(6,1),topPoint(6,2),topPoint(6,3),motorPoint(6,1),motorPoint(6,2),hingePoint(6,1),hingePoint(6,2) ) - 16.3482;
[ theta_v ] = angle_compute( topPoint(1,1),topPoint(1,2),topPoint(1,3),motorPoint(1,1),motorPoint(1,2),hingePoint(1,1),hingePoint(1,2) ) - 16.3482;
[ theta_z ] = angle_compute( topPoint(4,1),topPoint(4,2),topPoint(4,3),motorPoint(4,1),motorPoint(4,2),hingePoint(4,1),hingePoint(4,2) ) - 16.3482;
[ theta_w ] = angle_compute( topPoint(5,1),topPoint(5,2),topPoint(5,3),motorPoint(5,1),motorPoint(5,2),hingePoint(5,1),hingePoint(5,2) ) - 16.3482;
% 
% theta_x
% theta_y
% theta_z
% theta_u
% theta_v
% theta_w
% �����zz = 388.25ʱ theta_x��theta_wΪ0

if min([theta_x,theta_y,theta_z,theta_u,theta_v,theta_w]) > 0
      PlayVisit([max(theta_x,0),max(theta_y,0),max(theta_z,0),max(theta_u,0),max(theta_v,0),max(theta_w,0)] );
end; 
