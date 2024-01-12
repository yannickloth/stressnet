clear;

%Load test settings
%put the working directory to the directory where the results file is
%saved!
addpath('/home/yannick/matlab_tests');
cd '/home/yannick/matlab_tests';
load res_smtp_pcre_99_udp_zero.txt
values=res_smtp_pcre_99_udp_zero;
quantity=200000;
ratio=99;
repeat=50;

%Caculations
attacks=quantity/(ratio+1);
dims=size(values); %dimensions of values
a=values(1:dims(1),3)/attacks;
b=values(1:9,1)/1000;
for n=1:9
    for m=0:(repeat-1)
        detected(m+1,n)=values(n+9*m,3);
    end;
end;
moyenne=mean(detected)'/attacks;
stddev=std(detected)'/attacks;
moydev=[moyenne-stddev moyenne+stddev moyenne-2*stddev moyenne+2*stddev];

%------------------------------
%plot all values and mean value
%------------------------------
VALSFIG=figure;
hold;
subplot(2,1,1);
%axes('FontSize',12);
TITLE('Measured detection percentage and their mean values','FontSize',16,'FontWeight','bold');
XLABEL('bitrates Mbit/s');
YLABEL('detected attacks / total attacks');
grid on;
hold all;
for m=0:(repeat-1)
    plot(b,a((1+9*m):(9*(1+m))),'.'),
end;
plot(b,moyenne,'g');
%saveas(VALSFIG,'values_smtp_pcre_99_udp_zero','png');

%----------------------------------------
%plot mean values and standard deviations
%----------------------------------------
subplot(2,1,2);
%axes('FontSize',12);
TITLE('Mean values of detection percentage and their standard deviations','FontSize',16,'FontWeight','bold');
XLABEL('bitrate MBit/s');
YLABEL('detected attacks / total attacks');
grid on;
hold all;
plot(b,moyenne,'g');
%plot mean +- sigma:
plot(b,moydev(:,1),'rx');
plot(b,moydev(:,2),'rx');
%plot mean +- 2*sigma:
plot(b,moydev(:,3),'mx');
plot(b,moydev(:,4),'mx');
saveas(VALSFIG,'smtp_pcre_99_udp_zero','png');

%logdetected=log10(detected);

%-----------
%quit matlab
%-----------
%quit
