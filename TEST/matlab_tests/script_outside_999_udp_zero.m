clear;

%Load test settings
%set working directory to the directory where the results file is saved
addpath('/home/janusz/matlab_tests');
cd '/home/janusz/matlab_tests';
values=load('res_outside_999_udp_zero.txt');%load results file
quantity=200000;%quantity of packets sent
ratio=999;%ratio packetsM for 1 packetN
repeat=20;%how many measure per bitrate

%Caculations
attacks=quantity/(ratio+1);%number of attacks
dims=size(values); %dimensions of values
nOfBitrates=dims(1,1)/repeat %how many different bitrates tested
absc=values(1:nOfBitrates,1)/1000;
for n=1:nOfBitrates
    for m=0:(repeat-1)
        detected(m+1,n)=values(n+nOfBitrates*m,3)/attacks;
        dropped(m+1,n)=values(n+nOfBitrates*m,5)/quantity;
    end;
end;
moyDet=mean(detected)';
moyDro=mean(dropped)';
stddevDet=std(detected)';
stddevDro=std(dropped)';
moydevDet=[moyDet-stddevDet moyDet+stddevDet moyDet-2*stddevDet moyDet+2*stddevDet];
moydevDro=[moyDro-stddevDro moyDro+stddevDro moyDro-2*stddevDro moyDro+2*stddevDro];

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

%----------------------------------------------------------
%plot detected mean values and detected standard deviations
%----------------------------------------------------------
hold;
subplot(2,2,2);
AXIS([0 absc(nOfBitrates,1) 0 1]);
title('Mean values and standart deviations of detected attacks %','FontWeight','bold');
xlabel('bitrate MBit/s');
ylabel('detected attacks / total attacks');
grid on;
hold all;
plot(absc,moyDet,'g');
%plot mean +- sigma:
plot(absc,moydevDet(:,1),'rx');
plot(absc,moydevDet(:,2),'rx');
%plot mean +- 2*sigma:
plot(absc,moydevDet(:,3),'mx');
plot(absc,moydevDet(:,4),'mx');

%----------------------------------
%plot dropped and their mean values
%----------------------------------
hold;
subplot(2,2,3);
AXIS([0 absc(nOfBitrates,1) 0 1]);
title('Measured dropped packets %','FontWeight','bold');
xlabel('bitrates Mbit/s');
ylabel('dropped packets / total packets');
grid on;
hold all;
for m=0:(repeat-1)
    plot(absc,dropped(m+1,:),'.'),
end;
plot(absc,moyDro,'g');

%--------------------------------------------------------
%plot dropped mean values and dropped standard deviations
%--------------------------------------------------------
hold;
subplot(2,2,4);
AXIS([0 absc(nOfBitrates,1) 0 1]);
title('Mean values and standard deviations of dropped packets %','FontWeight','bold');
xlabel('bitrate MBit/s');
ylabel('detected attacks / total attacks');
grid on;
hold all;
plot(absc,moyDro,'g');
%plot mean +- sigma:
plot(absc,moydevDro(:,1),'rx');
plot(absc,moydevDro(:,2),'rx');
%plot mean +- 2*sigma:
plot(absc,moydevDro(:,3),'mx');
plot(absc,moydevDro(:,4),'mx');

%-----------
%save figure
%-----------
saveas(VALSFIG,'outside_999_udp_zero','png');

%-----------
%quit matlab
%-----------
%quit
