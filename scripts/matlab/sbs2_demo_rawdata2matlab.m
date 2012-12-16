% Demo example of how to load SBS2 data (.txt & .meta) into matlab.

clear all, close all, clc


%% Data specification
fname = {'emotivData_2012_02_06_12_00_17_demo01'}       %Filenames (.txt & .meta)
fpath = '/home/cs/smartphonebrainscanner2/data/verbs';  %file path
fs = 128;       %Sampling frequency


%% Read data
ifiles = 1;

%--------------------------------------------------------------------------
% Example without a experiment specific grouping of event names
%--------------------------------------------------------------------------
[data gyro mrk info] = sbs2_rawdata2matlab(fullfile(fpath,fname{ifiles}),...
                fs,struct('noHeaderLines',1));
            
%--------------------------------------------------------------------------
% Example with a experiment specific grouping of event names
%--------------------------------------------------------------------------
fhandle_classInfo = @sbs2_demo_class; %func object for class organization.
[data gyro mrk2 info2] = sbs2_rawdata2matlab(fullfile(fpath,fname{ifiles}),...
                fs,struct('noHeaderLines',1,'fhandle_classInfo',fhandle_classInfo));
