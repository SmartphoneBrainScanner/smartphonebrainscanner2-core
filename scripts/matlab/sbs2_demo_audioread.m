% Demo example of how import SBS2 data (.txt & .meta) to eeglab.

clear all, close all, clc


%% Data / path specification
fname = {'emotivData_ivan_in_2012_07_20_10_30_29_'}       %Filenames (.txt & .meta)
fpath = '/home/cs/smartphonebrainscanner2/data/audio';  %file path
path_eeglab = [fpath '/eeglabFiles'];                   %Path where eeglab files will be stored
path_eeglabToolbox = '/media/FreeAgent GoFlex Drive/cs/Toolboxes/NeuroTools/eeglab'; %Path to where eeglab is installed

fs = 128;       %Sampling frequency


%% Read data into matlab
ifiles = 1;

%--------------------------------------------------------------------------
% Example without a experiment specific grouping of event names
%--------------------------------------------------------------------------            
const_delay = 93;   % 93ms
[data gyro mrk info] = sbs2_rawdata2matlab(fullfile(fpath,fname{ifiles}),...
                fs,struct('noHeaderLines',1,'package_offset',const_delay));



            
break
% %--------------------------------------------------------------------------
% % Example with a experiment specific grouping of event names
% %--------------------------------------------------------------------------
% fhandle_classInfo = @sbs2_demo_class; %func object for class organization.
% [data gyro mrk info] = sbs2_rawdata2matlab(fullfile(fpath,fname{ifiles}),...
%     fs,struct('noHeaderLines',1,'fhandle_classInfo',fhandle_classInfo));


%% Convert data to eeglab
fpath = path_eeglab;

eventInfo.eventTable = [info.time(mrk.pos)' mrk.y];     
eventInfo.eventFields = { 'latency', 'type'};

fname_eeglab = 'Subject01';
opts.eventInfo = eventInfo;

%Note EEGLAB has problem with updating the GUI inside the
%sbs2_convert2eeglab function - GUI is updated below
if ~isdir(path_eeglab), mkdir(path_eeglab); end
EEG = sbs2_convert2eeglab(path_eeglab,fname_eeglab,data,info,path_eeglabToolbox,opts);

for itext=1:length(mrk.text)
    EEG.event(itext).text = mrk.text{itext};
end
EEG = pop_saveset( EEG, 'savemode','resave');
EEG = eeg_checkset( EEG );

eeglab redraw    % Update GUI
