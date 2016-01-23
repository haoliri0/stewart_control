%% 此程序用于复位

% echoudp('on',4012)
 
u = udp('192.168.15.255',7402,'LocalPort',8410);
 fopen(u) 
%  fopen(v)
 
 %*************************************************************************
 % Udp Data: App Control Field
 %*************************************************************************
 ConfirmCode=[ hex2dec('55'),hex2dec('aa')];
 PassCode=[ hex2dec('00'),hex2dec('00')];
 FunctionCode=[ hex2dec('12'),hex2dec('01')];
 ChannelCode=[ hex2dec('00'),hex2dec('02')];
 
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
 RegVisitAddress=[ hex2dec('00'),hex2dec('00')];
 RegVisitNum=[ hex2dec('00'),hex2dec('02')];
 RegVisitData=[ hex2dec('00'),hex2dec('00'),hex2dec('00'),hex2dec('00')];
 
 AppDataField=[ RegVisitAddress,RegVisitNum,RegVisitData];
 %=========================================================================
  
 %*************************************************************************
 % Udp Data Include: AppControlField/AppWhoField/AppDataField
 %*************************************************************************
 UdpData =[ AppControlFiled,AppWhoFiled,AppDataField ]
 %=========================================================================
 fwrite(u, UdpData)
 
 fclose(u)
%  fclose(v)
 
 
 