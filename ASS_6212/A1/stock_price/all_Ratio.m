function allRatio = all_Ratio()
%% Import data from text file.
% Script for importing data from the following text file:
%
%    /Users/lambda-c/Documents/MATLAB/stock_price/all_Ratio.csv
%
% To extend the code to different selected data or a different text file,
% generate a function instead of a script.

% Auto-generated by MATLAB on 2017/03/05 17:48:49

%% Initialize variables.
filename = '/Users/lambda-c/Documents/MATLAB/A1/stock_price/all_Ratio.csv';
delimiter = ',';
startRow = 2;

%% Format for each line of text:
%   column2: double (%f)
%	column3: double (%f)
%   column4: double (%f)
%	column5: double (%f)
%   column6: double (%f)
%	column7: double (%f)
%   column8: double (%f)
%	column9: double (%f)
%   column10: double (%f)
%	column11: double (%f)
%   column12: double (%f)
%	column13: double (%f)
%   column14: double (%f)
%	column15: double (%f)
%   column16: double (%f)
%	column17: double (%f)
%   column18: double (%f)
%	column19: double (%f)
%   column20: double (%f)
%	column21: double (%f)
%   column22: double (%f)
%	column23: double (%f)
%   column24: double (%f)
%	column25: double (%f)
%   column26: double (%f)
%	column27: double (%f)
%   column28: double (%f)
%	column29: double (%f)
%   column30: double (%f)
%	column31: double (%f)
%   column32: double (%f)
%	column33: double (%f)
%   column34: double (%f)
%	column35: double (%f)
%   column36: double (%f)
%	column37: double (%f)
% For more information, see the TEXTSCAN documentation.
formatSpec = '%*s%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%[^\n\r]';

%% Open the text file.
fileID = fopen(filename,'r');

%% Read columns of data according to the format.
% This call is based on the structure of the file used to generate this
% code. If an error occurs for a different file, try regenerating the code
% from the Import Tool.
dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'HeaderLines' ,startRow-1, 'ReturnOnError', false, 'EndOfLine', '\r\n');

%% Close the text file.
fclose(fileID);

%% Post processing for unimportable data.
% No unimportable data rules were applied during the import, so no post
% processing code is included. To generate code which works for
% unimportable data, select unimportable cells in a file and regenerate the
% script.

%% Create output variable
allRatio = [dataArray{1:end-1}];
%% Clear temporary variables
clearvars filename delimiter startRow formatSpec fileID dataArray ans;

end
