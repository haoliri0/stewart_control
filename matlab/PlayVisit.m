function [  ] = PlayVisit( theta_input )
%PLAYVISIT Summary of this function goes here
%   Detailed explanation goes here
 %u = udp('192.168.15.201',7409,'LocalPort',8411);
 u = udp('192.168.15.255',7402,'LocalPort',8410);
 fopen(u)

 rotate_pulse = theta_input*500000/360;
 
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
 
 AppWhoFiled=[ WhoAcceptCode,WhoReplyCode ];
 %=========================================================================

  %*************************************************************************
 % Udp Data: App Data Field
 %*************************************************************************
 PlayLine=[ hex2dec('00'),hex2dec('00'),hex2dec('04'),hex2dec('04')];
 
 PlayTime=[ hex2dec('00'),hex2dec('00'),hex2dec('00'),hex2dec('65')];

 modnum = [hex2dec('100') hex2dec('10000') hex2dec('100000')];

 delta3x = [rotate_pulse(1),rotate_pulse(2),rotate_pulse(3),rotate_pulse(4),rotate_pulse(5),rotate_pulse(6)];
 
    now = delta3x;

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
 
 fclose(u)
 
end
% 
