% process a performance measurement.

% netbench
% Copyright (C) 2004   Wim Mees <Wim.Mees@rma.ac.be>
%                      All rights reserved.
%
% This program is free software; you can redistribute it and/or
% modify it under the terms of the GNU General Public License
% as published by the Free Software Foundation; either version 2
% of the License, or (at your option) any later version.
%
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with this program; if not, write to the Free Software
% Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

function process_perfmeas ( filename, projname )

% read the file
[ tx_counter_raw tx_time_raw rx_time_raw packet_length_raw packet_code ] ...
  = textread ([filename '.txt'], '%f %f %f %f %f', ...
              'headerlines', 1);

% global overview including calibration samples
% (for debugging purposes)
latency_raw = rx_time_raw - tx_time_raw;
g = plot (tx_time_raw, 1000*latency_raw, '.');
xlabel('transmit time (in s)')
ylabel('RAW latency (in ms)')
title(projname)
grid on
saveas(g, [filename '_rawlatency_all.png'])

% separate the calibration from the measurement part          
n = length(packet_code);
iMin = 1;
while packet_code(iMin)==0
    iMin = iMin+1;
end
iMax = n;
while packet_code(iMax)==0
    iMax = iMax-1;
end

tx_time_calib = tx_time_raw(1:iMin-1);
rx_time_calib = rx_time_raw(1:iMin-1);
tx_time_calib(iMin:iMin+n-iMax-1) = tx_time_raw(iMax+1:n);
rx_time_calib(iMin:iMin+n-iMax-1) = rx_time_raw(iMax+1:n);

tx_counter    = tx_counter_raw(iMin:iMax);
tx_time       = tx_time_raw(iMin:iMax);
rx_time       = rx_time_raw(iMin:iMax);
packet_length = packet_length_raw(iMin:iMax);

% perform the calibration
if (length(tx_time_calib) > 20)
    a = tx_time_calib;
    b = [ ones(size(tx_time_calib)) tx_time_calib ];
    calib_coef = b \ rx_time_calib;
    tx_time = calib_coef(1) + calib_coef(2) * tx_time;
else
    % do nothing
end

% some constants          
n = length(tx_counter);
middle = round(n/2);
zoomrange = max([1 middle-500]):min([n middle+500]);          

% compute and plot the complete latency measurement
latency = rx_time - tx_time;
g = plot (tx_time, 1000*latency, '.');
xlabel('transmit time (in s)')
ylabel('latency (in ms)')
title(projname)
grid on
saveas(g, [filename '_latency_all.png'])

% plot a part of the latency measurement
g = plot (tx_time(zoomrange), 1000.0*latency(zoomrange), '.');
xlabel('transmit time (in s)')
ylabel('latency (in ms)')
title(projname)
grid on
saveas(g, [filename '_latency_part.png'])

% compute the linearity of the transmission
reduction_factor = floor(length(tx_counter) / 1000);
i_times_reduction_factor = 1;
if (reduction_factor >1)
    for i=1:floor(length(tx_counter)/reduction_factor);
        tx_counter_reduced(i) = tx_counter(i_times_reduction_factor);
        tx_time_reduced(i) = tx_time(i_times_reduction_factor);
        i_times_reduction_factor = i_times_reduction_factor + reduction_factor;
    end
else
    tx_counter_reduced = tx_counter;
    tx_time_reduced = tx_time;
end

a = tx_counter_reduced;
b = [ ones(size(a)) a ];
tx_coef = b \ tx_time_reduced;
tx_linear_reduced = tx_coef(1) + tx_coef(2) * a;

% plot the linearity of the transmission time
g = plot (tx_time_reduced, 1000.0*(tx_time_reduced-tx_linear_reduced), '.');
xlabel('transmit time (in s)')
ylabel('timing error at transmission (in ms)')
title(projname)
grid on
saveas(g, [filename '_txerr_all.png'])

% plot the bandwidth at reception
averaging_window = 50;
if (length(packet_length) > 10 * averaging_window)
    averaging_window = round(length(packet_length)/100)+1;
end

% plot an average bandwidth at reception
sum_length = 0;
for i=1:averaging_window-1
    sum_length = sum_length + packet_length(i:length(packet_length)-(averaging_window-i));
end

time_diff  = rx_time(averaging_window:length(packet_length)) - rx_time(1:length(packet_length)-averaging_window+1);
avg_bandwidth = 8.0 * sum_length ./ time_diff;
g = plot (rx_time(1:length(packet_length)-(averaging_window-1)), avg_bandwidth/1.0e6, '.');
xlabel('receive time (in s)')
ylabel('bandwidth (in Mbps)')
title(projname)
grid on
saveas(g, [filename '_bandwidth_all.png'])

% plot a part of the bandwidth at reception
zoomrange_bandwidth = (zoomrange(1):min([length(zoomrange) length(avg_bandwidth)]))';
g = plot (rx_time(zoomrange_bandwidth), 1000.0*avg_bandwidth(zoomrange_bandwidth), '.');
xlabel('transmit time (in s)')
ylabel('bandwidth (in Mbps)')
title(projname)
grid on
saveas(g, [filename '_bandwidth_part.png'])

% plot the packet loss
rx_counter = (1:n)';
packet_loss = tx_counter-rx_counter;
diff_packet_loss = packet_loss(2:n) - packet_loss(1:n-1);
g = plot (tx_time(1:n-1), diff_packet_loss, '.');
xlabel('transmit time (in s)')
ylabel('instantaneous packet loss')
title(projname)
grid on
saveas(g, [filename '_packetloss_all.png'])
