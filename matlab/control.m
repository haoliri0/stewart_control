pause(8) %pause(8)表示程序在此等待8秒
% 请在开始运行程序6秒后开始播放未命名.avi

for i = 1:8 %分8次运行到指定位姿(0,0,400,16,0,0)
    main_stewart(0,0,400,2*(i),0,0);
    pause(0.02); %相邻两次运行间隔为0.02秒(20ms)
end

pause(1.2)  %等待1.2秒

for i = 1:8 %逐渐恢复到初始位姿(0,0,400,0,0,0)
    main_stewart(0,0,400,2*(8-i),0,0);
    pause(0.02);
end

%%下面部分的代码类似

pause(0.4)

for i = 1:8
    main_stewart(0,0,400,0,2*(i),0);
    pause(0.02);
end

pause(0.6)

for i = 1:8
    main_stewart(0,0,400,0,2*(8-i),0);
    pause(0.02);
end
% 
% pause(0.4)
% 
% for i = 1:8
%     main_stewart(0,0,400,0,-3*i,0);
%     pause(0.01);
% end
% pause(0.1)
% for i = 1:8
%     main_stewart(0,0,400,0,-(8-i)*3,0);
%     pause(0.01);
% end
% 
% pause(0.4)
% 



% for i = 1:8
%     main_stewart(0,0,400,0,3*i,0);
%     pause(0.01);
% end
% for i = 1:8
%     main_stewart(0,0,400,0,(8-i)*3,0);
%     pause(0.01);
% end
% 
% pause(0.3)
% 
% for i = 1:8
%     main_stewart(0,0,400,0,-3*i,0);
%     pause(0.01);
% end
% 
% pause(0.2)
% 
% for i = 1:8
%     main_stewart(0,0,400,0,-(8-i)*3,0);
%     pause(0.01);
% end
% 