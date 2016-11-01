function [  ] = PlayVisit( theta_input )
%% ??4???

 % ????udp????u,  192.168.15.255???192.168.15.0~254??????????mdbox???192.168.15.1?192.168.15.2?
 % 7402?mdbox????8410?????????????mdbox???
 u = udp('192.168.15.255',7402,'LocalPort',8410);
 
 fopen(u)
 
 %% ??4-1??????mdbox??p29?p31?6????????????????
 %*************************************************************************
 % Udp Data: App Control Field
 %*************************************************************************
 ConfirmCode=[ hex2dec('55'),hex2dec('aa')];
 PassCode=[ hex2dec('00'),hex2dec('00')];
 FunctionCode=[ hex2dec('14'),hex2dec('01')];% ??????
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
 % delta_time——?????????
 PlayTime=[ hex2dec('00'),hex2dec('00'),hex2dec('00'),hex2dec('65')];

 modnum = [hex2dec('100') hex2dec('10000') hex2dec('100000')];

 %% ??4-2??????????????? ???50:1,10000????????????????500000
 rotate_pulse = theta_input*500000/360;
 %% ??4-2??
 
 delta3x = [rotate_pulse(1),rotate_pulse(2),rotate_pulse(3),rotate_pulse(4),rotate_pulse(5),rotate_pulse(6)];
 
 now = delta3x;
 % ??????????????????????????p32
    PlayXpos=[ hex2dec('00'),fix( mod(now(1),modnum(3)) / modnum(2) ),fix( mod(now(1),modnum(2)) / modnum(1) ),mod( now(1),modnum(1) )];
    PlayYpos=[ hex2dec('00'),fix( mod(now(2),modnum(3)) / modnum(2) ),fix( mod(now(2),modnum(2)) / modnum(1) ),mod( now(2),modnum(1) )];
    PlayZpos=[ hex2dec('00'),fix( mod(now(3),modnum(3)) / modnum(2) ),fix( mod(now(3),modnum(2)) / modnum(1) ),mod( now(3),modnum(1) )];
    PlayUpos=[ hex2dec('00'),fix( mod(now(4),modnum(3)) / modnum(2) ),fix( mod(now(4),modnum(2)) / modnum(1) ),mod( now(4),modnum(1) )];
    PlayVpos=[ hex2dec('00'),fix( mod(now(5),modnum(3)) / modnum(2) ),fix( mod(now(5),modnum(2)) / modnum(1) ),mod( now(5),modnum(1) )];
    PlayWpos=[ hex2dec('00'),fix( mod(now(6),modnum(3)) / modnum(2) ),fix( mod(now(6),modnum(2)) / modnum(1) ),mod( now(6),modnum(1) )];
 
 PlayDout=[ hex2dec('12'),hex2dec('34')];
 PlayDac=[ hex2dec('56'),hex2dec('78'),hex2dec('AB'),hex2dec('CD')]; 

 AppDataField=[ PlayLine,PlayTime,PlayXpos,PlayYpos,PlayZpos,PlayUpos,PlayVpos,PlayWpos,PlayDout,PlayDac];
 %=========================================================================
  
 %*************************************************************************
 % Udp Data Include: AppControlField/AppWhoField/AppDataField
 %*************************************************************************
 % UdpData??????????
 UdpData =[ AppControlFiled,AppWhoFiled,AppDataField ];
 %=========================================================================
 
 % ??u??
 fwrite(u, UdpData)
 
 % ?u????????pc??????????????????????????8411?
 fclose(u)
 
end
%%??4?? 
