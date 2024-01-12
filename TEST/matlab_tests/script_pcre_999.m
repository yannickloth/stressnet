clear;

%Load test settings
%set working directory to the directory where the results file is saved
%addpath('/home/janusz/matlab_tests');
%cd '/home/janusz/matlab_tests';
values=load('res_smtp_pcre_999_udp_zero.txt');%load results file
quantity=200000;%quantity of packets sent
ratio=999;%ratio packetsM for 1 packetN
repeat=20;%how many measure per bitrate

%Caculations
attacks=quantity/(ratio+1);%number of attacks
dims=size(values); %dimensions of values
nOfBitrates=dims(1,1)/repeat %how many different bitrates tested
absc=values(1:nOfBitrates,1)/1000;
measures=1:1:20;
for n=1:nOfBitrates
    for m=0:(repeat-1)
        detected(m+1,n)=values(n+nOfBitrates*m,3)/attacks;
        dropped(m+1,n)=values(n+nOfBitrates*m,5)/quantity;
    end;
end;
moyDet=mean(detected)';
moyDro=mean(dropped)';


%Calculating standard devs has sense provided the distribution of values at
%a given bitrate is normal (GAUSSIAN PLOT)
%This could be established with the matlab fitting tools (Stats toolbox)

%-----------------------------------
%plot detected and their mean values
%-----------------------------------
VALSFIG=figure;
hold;
subplot(2,2,1);
AXIS([0 absc(nOfBitrates,1) 0 1]);
title('Measured detected attacks %','FontWeight','bold');
xlabel('bitrates Mbit/s');
ylabel('detected attacks / total attacks');
grid on;
hold;
for m=0:(repeat-1)
    plot(absc,detected(m+1,:),'.');
end;
plot(absc,moyDet,'g');

%------------------------
%box plot detected values
%------------------------
subplot(2,2,2);
hold all;
boxplot(detected,'labels',int2str(absc));
hold all;
title('Box plot of detected attacks %','FontWeight','bold');
xlabel('bitrate MBit/s');
ylabel('detected attacks / total attacks');
grid on;

%----------------------------------
%plot dropped and their mean values
%----------------------------------
hold;
subplot(2,2,3);
axis([0 absc(nOfBitrates,1) 0 1]);
title('Measured dropped packets %','FontWeight','bold');
xlabel('bitrates Mbit/s');
ylabel('dropped packets / total packets');
grid on;
hold all;
for m=0:(repeat-1)
    plot(absc,dropped(m+1,:),'.'),
end;
plot(absc,moyDro,'g');

%-----------------------
%box plot dropped values
%-----------------------
subplot(2,2,4);
hold all;
boxplot(dropped,'labels',int2str(absc));
hold all;
title('Box plot of dropped packets %','FontWeight','bold');
xlabel('bitrate MBit/s');
ylabel('detected attacks / total attacks');
grid on;

DRODET=figure;
hold;
plot(1-(moyDro+moyDet));
hold all;
title('Comparison of dropped packets % vs detected packets %','FontWeight','bold');
xlabel('bitrate MBit/s');
ylabel('1-(mean dropped+mean detected)');
grid on;

%-----------
%save figure
%-----------
saveas(VALSFIG,'smtp_pcre_999_udp_zero','png');
saveas(DRODET,'smtp_pcre_999_udp_zero_dordet','png');
%-----------
%quit matlab
%-----------
%quit
