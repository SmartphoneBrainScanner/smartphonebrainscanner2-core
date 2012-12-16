% Demo example of how import SBS2 data (.txt & .meta) to SPM8 using fieldtrip.
clear all, close all, clc
spm('defaults', 'eeg');

path(fullfile(spm('dir'),'/external/fieldtrip'),path);



%% Data / path specification
fname = {'emotivData_2012_02_06_12_00_17_demo01'}       %Filenames (.txt & .meta)
fpath = '/home/cs/smartphonebrainscanner2/data/verbs';  %file path

path_SPMdatafiles = [fpath '/SPM8files'];                   %Path where eeglab files will be stored

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


% % Example of epoching data wrt a specific class, e.g. mrk.y == 3.
% prestim = round(0.2*info.fs);
% poststim = round(1.0*info.fs);
% 
% % Find all trials with mkr.y == 3:
% idx = mrk.y==3;
% trl_pos = mrk.pos(idx);
% 
% % trl: [begsample , endsample , prestim relativ to onset , classlabels]
% cfg = [];
% cfg.trl = [ (trl_pos-prestim) , (trl_pos+poststim) ,...
%             -prestim*ones(length(trl_pos),1) , mrk.y(idx)];    
% FTdata = ft_redefinetrial(cfg,FTdata);



%% Convert Fieldtrip format to SPM8 format
if ~isdir(path_SPMdatafiles), mkdir(path_SPMdatafiles), end
fname_spmfile = ['SPM8' fname{ifiles}];
D = spm_eeg_ft2spm(FTdata,fullfile(path_SPMdatafiles,fname_spmfile));


%% Modify SPM meeg object
% Assign default sensor positions
S.D = D;
S.task = 'defaulteegsens';
S.save = 1;
D = spm_eeg_prep(S);

S = [];
S.task = 'settype';
S.D = fullfile(path_SPMdatafiles,[fname_spmfile '.mat']);
S.ind = 1:D.nchannels;
% S.ind = [1 3 6:D.nchannels];
S.type = 'EEG';
S.save = 1;
D = spm_eeg_prep(S);

