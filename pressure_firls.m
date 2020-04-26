close all;
clear all;
foldername='E:\Final Year Project\my tutorials\new data\fypLogic-master\DATA\Data Set1\Healthy\H3F27\';
Pres=[foldername 'ReadingCombinedPressure.csv' ];
P=csvread(Pres);
P(P(:,2)>100,:)=[];
P(P(:,2)<50,:)=[];
p = P(:,2);
[x,n]=size(p);
X=linspace(0,1,x);
figure;
%raw data
plot(X,p);

%fir constrained least square filter
b = firls(512,[0 0.12 0.14 1],[1 1 0 0]);
fvtool(b,1);
pressure1=filter(b,1,p);
[x,n] = size(pressure1);
X = linspace(0,1,x)';
figure;
plot(X,pressure1,'b-');

%detrend data using high pass filter
[b,a]=butter(6,0.55/45,'high');
new_pressure=filter(b,a,pressure1);
figure;
plot(X,new_pressure);
%detrend data using the detrend function
y=detrend(pressure1);
figure
plot(X,y);
%detrend data using polynomial fit
opol = 6;
[p,s,mu] = polyfit(X,pressure1,opol);
f_y = polyval(p,X,[],mu);

dt_pressure = pressure1 - f_y;
figure
plot(X,dt_pressure);
