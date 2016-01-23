 %u = udp('192.168.15.201',7409,'LocalPort',8411);
 u = udp('192.168.15.255',7402,'LocalPort',8410);
 fopen(u);
 
 %*************************************************************************
 % Udp Data: App Control Field
 %*************************************************************************
 ConfirmCode=[ hex2dec('55'),hex2dec('aa')];
 PassCode=[ hex2dec('00'),hex2dec('00')];
 FunctionCode=[ hex2dec('14'),hex2dec('01')];
 ChannelCode=[ hex2dec('00'),hex2dec('01')];
 
 AppControlFiled=[ ConfirmCode,PassCode,FunctionCode,ChannelCode ];
 %=========================================================================
 
  %*************************************************************************
 % Udp Data: App Who Field
 %*************************************************************************
 WhoAcceptCode=[ hex2dec('ff'),hex2dec('ff')];
 WhoReplyCode=[ hex2dec('ff'),hex2dec('ff')];
 
 AppWhoFiled=[ WhoAcceptCode,WhoReplyCode ]
 %=========================================================================

  %*************************************************************************
 % Udp Data: App Data Field
 %*************************************************************************
 PlayLine=[ hex2dec('00'),hex2dec('00'),hex2dec('04'),hex2dec('04')];
 
 PlayTime=[ hex2dec('00'),hex2dec('00'),hex2dec('00'),hex2dec('65')];

 delta1 = [hex2dec('4000'),hex2dec('3666'),hex2dec('3999'),hex2dec('5999'),hex2dec('5999'),hex2dec('5999')];
 delta2 = [-hex2dec('4000'),-hex2dec('3666'),-hex2dec('3999'),hex2dec('5999'),hex2dec('5999'),hex2dec('5999')]
 modnum = [hex2dec('100') hex2dec('10000') hex2dec('100000')];
 
delta3x = [hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('0')];
delta3y = [hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('500')]; 
delta3z = [hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('0')];
delta3u = [hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('0')];
delta3v = [hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('1'),hex2dec('0'),hex2dec('0')];
delta3w = [hex2dec('0'),hex2dec('0'),hex2dec('0'),hex2dec('1'),hex2dec('0'),hex2dec('0')];



% ×î¸ßµã£º 81000    53000    69000    76000    72000    72000

 for kk = 1:1
     initial_state = [0,0,0,0,0,0];
     delta= [1500,1500,1500,-1500,-1500,-1500];
     now = initial_state;
%     if kk<=20
%         now = now + delta1/4;
%     else 
%         now = now + delta2/4;
%     end;
    PlayXpos=[ hex2dec('00'),fix( mod(now(1),modnum(3)) / modnum(2) ),fix( mod(now(1),modnum(2)) / modnum(1) ),mod( now(1),modnum(1) )];
    PlayYpos=[ hex2dec('00'),fix( mod(now(2),modnum(3)) / modnum(2) ),fix( mod(now(2),modnum(2)) / modnum(1) ),mod( now(2),modnum(1) )];
    PlayZpos=[ hex2dec('00'),fix( mod(now(3),modnum(3)) / modnum(2) ),fix( mod(now(3),modnum(2)) / modnum(1) ),mod( now(3),modnum(1) )];
    PlayUpos=[ hex2dec('00'),fix( mod(now(4),modnum(3)) / modnum(2) ),fix( mod(now(4),modnum(2)) / modnum(1) ),mod( now(4),modnum(1) )];
    PlayVpos=[ hex2dec('00'),fix( mod(now(5),modnum(3)) / modnum(2) ),fix( mod(now(5),modnum(2)) / modnum(1) ),mod( now(5),modnum(1) )];
    PlayWpos=[ hex2dec('00'),fix( mod(now(6),modnum(3)) / modnum(2) ),fix( mod(now(6),modnum(2)) / modnum(1) ),mod( now(6),modnum(1) )];
%  
%  PlayXpos=[ hex2dec('00'),hex2dec('01'),hex2dec('40'),hex2dec('00')];
%  PlayYpos=[ hex2dec('00'),hex2dec('01'),hex2dec('10'),hex2dec('00')];
%  PlayZpos=[ hex2dec('00'),hex2dec('01'),hex2dec('20'),hex2dec('00')];
%  PlayUpos=[ hex2dec('00'),hex2dec('01'),hex2dec('c0'),hex2dec('00')];
%  PlayVpos=[ hex2dec('00'),hex2dec('01'),hex2dec('c0'),hex2dec('00')];
%  PlayWpos=[ hex2dec('00'),hex2dec('01'),hex2dec('c0'),hex2dec('00')];
%  
%  x 014000 / 5 = 4000
%  y 011000 / 5 = 3666
%  z 012000 / 5 = 3999
%  u 01c000 / 5 = 5999
%  v 01c000 / 5 = 5999
%  w 01c000 / 5 = 5999
 
 PlayDout=[ hex2dec('12'),hex2dec('34')];
 PlayDac=[ hex2dec('56'),hex2dec('78'),hex2dec('AB'),hex2dec('CD')]; 

 AppDataField=[ PlayLine,PlayTime,PlayXpos,PlayYpos,PlayZpos,PlayUpos,PlayVpos,PlayWpos,PlayDout,PlayDac];
 %=========================================================================
  
 %*************************************************************************
 % Udp Data Include: AppControlField/AppWhoField/AppDataField
 %*************************************************************************
 UdpData =[ AppControlFiled,AppWhoFiled,AppDataField ];
 %=========================================================================

 fwrite(u, UdpData)
 pause(0.1)
end;
 
 fclose(u)
 
 
