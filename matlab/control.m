pause(8) %pause(8)��ʾ�����ڴ˵ȴ�8��
% ���ڿ�ʼ���г���6���ʼ����δ����.avi

for i = 1:8 %��8�����е�ָ��λ��(0,0,400,16,0,0)
    main_stewart(0,0,400,2*(i),0,0);
    pause(0.02); %�����������м��Ϊ0.02��(20ms)
end

pause(1.2)  %�ȴ�1.2��

for i = 1:8 %�𽥻ָ�����ʼλ��(0,0,400,0,0,0)
    main_stewart(0,0,400,2*(8-i),0,0);
    pause(0.02);
end

%%���沿�ֵĴ�������

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