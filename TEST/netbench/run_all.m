% process all performance measurements.

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

function run_all ( )

sizes  = [ 0 100 500 1000 1472 ];
% speeds = [ 10 20 50 90 100 ];
speeds = [ 1 5 9 10 11 ];

for iSize = 1:length(sizes)
    for iSpeed = 1:length(speeds)
        disp(['-- processing meas_' int2str(sizes(iSize)) '.' int2str(speeds(iSpeed)) '000000'])
        process_perfmeas(['meas_' int2str(sizes(iSize)) '.' int2str(speeds(iSpeed)) '000000'], ...
            [int2str(sizes(iSize)) ' data, ' int2str(speeds(iSpeed)) 'Mbps'])
    end
end
