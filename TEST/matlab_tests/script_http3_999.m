clear;

%Load test settings
%put the working directory to the directory where the results file is
%saved!
addpath('/home/yannick/matlab_tests');
cd '/home/yannick/matlab_tests';
values=load('res_http3_999_udp_zero.txt');
values2=load('res_http3_999_udp_zero_100150200.txt');
values3=load('res_http3_999_udp_zero_210220230.txt');
quantity=200000;
ratio=999;
repeat=50;
repeat2=50;
repeat3=20;

%Caculations
attacks=quantity/(ratio+1);
dims=size(values); %dimensions of values
a=values(1:dims(1),3)/attacks;
b=values(1:9,1)/1000;
b(10:12,1)=values2(1:3,1)/1000;
b(13:15,1)=values3(1:3,1)/1000;
for n=1:9
    for m=0:(repeat-1)
        detected(m+1,n)=values(n+9*m,3);
    end;
end;
for n=1:3
    for m=0:(repeat2-1)
        detected(m+1,n+9)=values2(n+3*m,3);
    end;
end;
for n=1:3
    for m=0:(repeat3-1)
        detected(m+1,n+12)=values3(n+3*m,3);
    end;
end;
detected=detected(1:20,:);
moyenne=mean(detected)'/attacks;
stddev=std(detected)'/attacks;
moydev=[moyenne-stddev moyenne+stddev moyenne-2*stddev moyenne+2*stddev];

%------------------------------
%plot all values and mean value
%------------------------------
VALSFIG=figure;
subplot(2,1,1);
hold;
%axes('FontSize',12);
TITLE('Measured detection percentage and their mean values','FontSize',16,'FontWeight','bold');
XLABEL('bitrates Mbit/s');
YLABEL('detected attacks / total attacks');
grid on;
hold all;
for m=0:(repeat3-1)
    %plot(b,a((1+9*m):(9*(1+m))),'.');
    plot(b,detected(m+1,:),'.');
end;
plot(b,moyenne,'g');


%----------------------------------------
%plot mean values and standard deviations
%----------------------------------------
%axes('FontSize',12);
subplot(2,1,2);
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
plot(b,moydev(:,3),'m');
plot(b,moydev(:,4),'m');
saveas(VALSFIG,'http3_999_udp_zero','png');

%-----------
%quit matlab
%-----------
%quit
