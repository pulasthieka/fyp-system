clear all;
close all;
folderName = 'E:\Final Year Project\my tutorials\new data\fypLogic-master\DATA\Data Set1\Healthy\H3F27\';
data = [folderName 'ReadingCombinedIMP.csv'];
P = csvread(data);
p=P(:,15);
[x,n] = size(p);
X=linspace(0,1,x);
figure;
plot(X,p);
[b,a] = cheby1(8,0.01,0.02);
fvtool(b,a);
imp=filter(b,a,p);
x=length(imp);
X = linspace(0,1,x)';
plot(X,imp);

%differentiation
Nf = 50; 
Fpass = 0.1; 
Fstop = 0.5;
Fs=10;

d = designfilt('differentiatorfir','FilterOrder',Nf, ...
    'PassbandFrequency',Fpass,'StopbandFrequency',Fstop, ...
    'SampleRate',Fs);

fvtool(d,'MagnitudeDisplay','zero-phase','Fs',Fs)

dX = X(2)-X(1);
dimp = filter(d,imp)/dX;
delay = mean(grpdelay(d));
tt = X(1:end-delay);
vd = dimp;
vd(1:delay) = [];

tt(1:delay) = [];
vd(1:delay) = [];
%[pkp,lcp] = findpeaks(imp);
%zcp = zeros(size(lcp));

%[pkm,lcm] = findpeaks(-imp);
%zcm = zeros(size(lcm));

subplot(2,1,1)
plot(X,imp)
xlabel('Time')
ylabel('Temperature')
grid

subplot(2,1,2)
plot(tt,vd)
xlabel('Time)')
ylabel('Differentiated Temperature')
grid

