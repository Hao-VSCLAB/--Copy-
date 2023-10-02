clc;close all; clear;

MeasuredData = load( 'MeasuredData.txt' ) ;

Axis = 1;

Pos =     MeasuredData( : , 1 + Axis * 0 : Axis  + Axis * 0 ) ;
Vel =     MeasuredData( : , 1 + Axis * 1 : Axis  + Axis * 1 ) ;
PosCmd =  MeasuredData( : , 1 + Axis * 2 : Axis  + Axis * 2 ) ;
VelCmd =  MeasuredData( : , 1 + Axis * 3 : Axis  + Axis * 3 ) ;
TorCtrl = MeasuredData( : , 1 + Axis * 4 : Axis  + Axis * 4 ) ;


%% Read Data
PosError = PosCmd - Pos ;
y = lowp(Vel(:,1),10,100,0.1,20,1000);
VelError = VelCmd - y ;

for i = 1 : Axis

norm_PosError( 1 , i ) = norm( PosError( : , i ) ) / norm( PosCmd( : , i ) ) ;
norm_VelError( 1 , i ) = norm( VelError( : , i ) ) / norm( VelCmd( : , i ) ) ;

end

norm_PosError , norm_VelError

%% Plot
Sampling_time = 0.001;
tf = size( MeasuredData , 1 ) * Sampling_time ;
Time = Sampling_time : Sampling_time : tf ;


subplot( 3 , 1 , 1 ) ;
plot( Time , PosCmd( : , 1 ) , '-' , Time , Pos( : , 1 ) , '--' , Time , PosError( : , i ) , ':' ,'LineWidth',2) ;
title( [ 'Pos ' , num2str(1) ] , 'FontWeight' , 'bold' , 'FontSize' , 12 ) ;
xlabel( 'Time (sec)') ; ylabel( 'Pos (rad)', 'FontSize' , 10 ) ;
legend( 'PosCmd' , 'Pos' , 'Error' ) ;
grid on ;






subplot( 3 , 1 , 2 ) ;
plot( Time , VelCmd( : , 1 ) , '-' , Time , y , '--' ,  Time , VelError( : , i ) , ':'  ,'LineWidth',2) ;
title( [ 'Vel ' , num2str(1) ] , 'FontWeight' , 'bold' , 'FontSize' , 12 ) ;
xlabel( 'Time (sec)') ; ylabel( 'Vel (rad/s)', 'FontSize' , 10 ) ;
legend( 'VelCmd' , 'Vel' , 'Error' ) ;
grid on ;






subplot( 3 , 1 , 3 ) ;
plot( Time , TorCtrl( : , 1 ) , '-' , 'LineWidth' , 2 ) ;
title( [ 'TorCtrl ' , num2str(1) ]  ) ;
xlabel( 'Time (sec)') ; ylabel( 'Tor (Nm)', 'FontSize' , 10 ) ;
grid on ;

function y=lowp(x,f1,f3,rp,rs,Fs)
%低通?波
%使用注意事?：通?或阻?的截止?率的?取范?是不能超?采?率的一半
%即，f1,f3的值都要小于 Fs/2
%x:需要?通?波的序列
% f 1：通?截止?率
% f 3：阻?截止?率
%rp：???衰?DB??置
%rs：截止?衰?DB??置
%FS：序列x的采??率
% rp=0.1;rs=30;%通??衰?DB值和阻??衰?DB值
% Fs %采?率

%   [N, Wp] = CHEB1ORD(Wp, Ws, Rp, Rs, 's') does the computation for an
%   analog filter, in which case Wp and Ws are in radians/second.
%
%   % Example 1:
%   %   For data sampled at 1000 Hz, design a lowpass filter with less than
%   %   3 dB of ripple in the passband defined from 0 to 40 Hz and at least
%   %   60 dB of ripple in the stopband defined from 150 Hz to the Nyquist
%   %   frequency (500 Hz):
%
%   Wp = 40/500; Ws = 150/500;
%   Rp = 3; Rs = 60;
%   [n,Wp] = cheb1ord(Wp,Ws,Rp,Rs)  % Gives mimimum order of filter
%   [b,a] = cheby1(n,Rp,Wp);        % Chebyshev Type I filter

wp=2*pi*f1/Fs;
ws=2*pi*f3/Fs;
% ??切比雪夫?波器；
[n,wn]=cheb1ord(wp/pi,ws/pi,rp,rs);
[bz1,az1]=cheby1(n,rp,wp/pi);

y=filter(bz1,az1,x);%?序列x?波后得到的序列y
end