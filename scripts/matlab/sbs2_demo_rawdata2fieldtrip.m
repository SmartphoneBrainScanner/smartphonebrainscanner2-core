% Demo example of how import SBS2 data (.txt & .meta) to fieldtrip.

clear all, close all, clc


%% Data / path specification
fname = {'emotivData_2012_02_06_12_00_17_demo01'}       %Filenames (.txt & .meta)
fpath = '/home/cs/smartphonebrainscanner2/data/verbs';  %file path
path_FT = [fpath '/FTfiles'];                   %Path where eeglab files will be stored

fs = 128;       %Sampling frequency


%% Read data into matlab
ifiles = 1;

%--------------------------------------------------------------------------
% Example without a experiment specific grouping of event names
%--------------------------------------------------------------------------
[data gyro mrk info] = sbs2_rawdata2matlab(fullfile(fpath,fname{ifiles}),...
                fs,struct('noHeaderLines',1));

% %--------------------------------------------------------------------------
% % Example with a experiment specific grouping of event names
% %--------------------------------------------------------------------------
% fhandle_classInfo = @sbs2_demo_class; %func object for class organization.
% [data gyro mrk info] = sbs2_rawdata2matlab(fullfile(fpath,fname{ifiles}),...
%     fs,struct('noHeaderLines',1,'fhandle_classInfo',fhandle_classInfo));



%% Convert data to FieldTrip format
fname_FT = ['FT' fname{ifiles}];

FTdata.label = info.chlist;
FTdata.trial = {data'};
FTdata.time = {info.time};
FTdata.fsample = info.fs;

[Nc Ns] = size(FTdata.trial{1});    % Nc: Number of channels, Ns: Number of samples
FTdata.sampleinfo = [1 Ns];          % array containing [startsample endsample] of data

if ~isdir(path_FT), mkdir(path_FT), end
save(fullfile(path_FT,fname_FT),'FTdata','gyro','mrk','info')

%% Pre-processing of continuous data shoul be done here before epoching data.




%% Example of how to do epoching using FieldTrip
% Example of epoching data wrt a specific class, e.g. mrk.y == 3.
prestim = round(0.2*info.fs);
poststim = round(1.0*info.fs);

% Find all trials with mkr.y == 3:
idx = mrk.y==3;
trl_pos = mrk.pos(idx);

% trl: [begsample , endsample , prestim relativ to onset , classlabels]
cfg = [];
cfg.trl = [ (trl_pos-prestim) , (trl_pos+poststim) ,...
            -prestim*ones(length(trl_pos),1) , mrk.y(idx)];    
       
FTdata_new = ft_redefinetrial(cfg,FTdata);
