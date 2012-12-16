function EEG = sbs2_convert2eeglab(fpath,fname_eeglab,data,info,path_eeglabToolbox,opts)
%==========================================================================
% Filename: sbs2_convert2eeglab.m (function).
%
% Description:  Convert data to eeglab format.
%
% Input:        fpath: file path
%        fname_eeglab: Name of the eeglab-file to be saved.
%                data: Matrix with data [ samples x channels]
%                info:
%                     .fs: Sampling frequency
%                     .time: Time axis
%  path_eeglabToolbox: Path to where the eeglab toolbox is installed (used
%                      to load a template channel setup file).
%
%                opts:
%                     .eventInfo.eventTable: Array specifying the events,
%                                            e.g. with latency and type
%                                            values.
%                     .eventInfo.eventFields: Cell array with the fields,
%                                             e.g. {'latency' 'type'}
%                     .events: Events to be epoched w.r.t.
%                     .timeEpoch: Time window w.r.t. event-timing.
%                     .Chlabels: Channel labels
%                     .refChNo: Reference channel number (multiple channels
%                               allowed - will be the average)
%
% Output:       EEG: EEGLAB structure with the data and associated info.
%
% Example:
%
% History:
%   - Created:  18/07/2012
%
% Author: Carsten Stahlhut
%
% Copyright (C) SBS2, DTU Informatics, 2012
%==========================================================================


if nargin<6
    opts = [];
end


if isfield(opts,'events'), events = opts.events; else events = { }; end
if isfield(opts,'timeEpoch'), timeEpoch = opts.timeEpoch; else timeEpoch = []; end
if isfield(opts,'timeBaseline'), timeBaseline = opts.timeBaseline; else timeBaseline = []; end
if isfield(opts,'Chlabels')
    Chlabels = opts.Chlabels;
    if isfield(opts,'refChNo'), refChNo = opts.refChNo; else refChNo = []; end
else
    Chlabels = {'P7' 'FC6' 'T7' 'P8' 'O2' 'O1' 'FC5' 'F3' 'F4' 'T8' 'F7' 'F8' 'AF4' 'AF3' 'P4' 'P3'};
    refChNo = [15 16];  %P3 P4
end


fs = info.fs;
[Ns Nc] = size(data);

disp('Adding reference channel...')
data = [data zeros(Ns,2)];     %Add P3 and P4
save('data_temp','data')

eeglab


%% Import data to eeglab
EEG = pop_importdata('dataformat','matlab','nbchan',0,'data',[pwd filesep 'data_temp.mat'],'subject',fname_eeglab,'srate',fs,'pnts',0,'xmin',0);
EEG = eeg_checkset( EEG );
eeglab redraw


EEG.setname= fname_eeglab;
EEG = eeg_checkset( EEG );
eeglab redraw


%% Channel names, locations, and type
EEG = eeg_checkset( EEG );
% EEG=pop_chanedit(EEG, 'append',1,'append',2,'append',3,'append',4,'append',5,'append',6,'append',7,'append',8,'append',9,'append',10,'append',11,'append',12,'append',13,'append',14,'append',15,'changefield',{2 'datachan' 1},'changefield',{3 'datachan' 1},'changefield',{4 'datachan' 1},'changefield',{5 'datachan' 1},'changefield',{6 'datachan' 1},'changefield',{7 'datachan' 1},'changefield',{8 'datachan' 1},'changefield',{9 'datachan' 1},'changefield',{10 'datachan' 1},'changefield',{11 'datachan' 1},'changefield',{12 'datachan' 1},'changefield',{13 'datachan' 1},'changefield',{14 'datachan' 1},'changefield',{15 'datachan' 1},'changefield',{16 'datachan' 1},'changefield',{1 'labels' 'P7'},'changefield',{2 'labels' 'FC6'},'changefield',{3 'labels' 'T7'},'changefield',{4 'labels' 'P8'},'changefield',{5 'labels' 'O2'},'changefield',{6 'labels' 'O1'},'changefield',{7 'labels' 'FC5'},'changefield',{8 'labels' 'F3'},'changefield',{9 'labels' 'F4'},'changefield',{10 'labels' 'T8'},'changefield',{11 'labels' 'F7'},'changefield',{12 'labels' 'F8'},'changefield',{13 'labels' 'AF4'},'changefield',{14 'labels' 'AF3'}, 'changefield',{15 'labels' 'P4'},'changefield',{16 'labels' 'P3'},'lookup',fullfile(path_eeglabToolbox,'plugins/dipfit2.2/standard_BESA/standard-10-5-cap385.elp'));
EEG=pop_chanedit(EEG, 'append',1,'append',2,'append',3,'append',4,'append',5,'append',6,'append',7,'append',8,'append',9,'append',10,'append',11,'append',12,'append',13,'append',14,'append',15,'changefield',{2 'datachan' 1},'changefield',{3 'datachan' 1},'changefield',{4 'datachan' 1},'changefield',{5 'datachan' 1},'changefield',{6 'datachan' 1},'changefield',{7 'datachan' 1},'changefield',{8 'datachan' 1},'changefield',{9 'datachan' 1},'changefield',{10 'datachan' 1},'changefield',{11 'datachan' 1},'changefield',{12 'datachan' 1},'changefield',{13 'datachan' 1},'changefield',{14 'datachan' 1},'changefield',{15 'datachan' 1},'changefield',{16 'datachan' 1},'changefield',{1 'labels' Chlabels{1}},'changefield',{2 'labels' Chlabels{2}},'changefield',{3 'labels' Chlabels{3}},'changefield',{4 'labels' Chlabels{4}},'changefield',{5 'labels' Chlabels{5}},'changefield',{6 'labels' Chlabels{6}},'changefield',{7 'labels' Chlabels{7}},'changefield',{8 'labels' Chlabels{8}},'changefield',{9 'labels' Chlabels{9}},'changefield',{10 'labels' Chlabels{10}},'changefield',{11 'labels' Chlabels{11}},'changefield',{12 'labels' Chlabels{12}},'changefield',{13 'labels' Chlabels{13}},'changefield',{14 'labels' Chlabels{14}}, 'changefield',{15 'labels' Chlabels{15}},'changefield',{16 'labels' Chlabels{16}},'lookup',fullfile(path_eeglabToolbox,'plugins/dipfit2.2/standard_BESA/standard-10-5-cap385.elp'));
EEG = eeg_checkset( EEG );

EEG = eeg_checkset( EEG );
EEG=pop_chanedit(EEG, 'settype',{'1:16' 'eeg'});
EEG = eeg_checkset( EEG );


%% Re-reference
if ~isempty(refChNo)
    EEG = pop_reref( EEG, refChNo );
    EEG = eeg_checkset( EEG );
end


%         %Re-referencing to average channels and keep P3 info.
%         fname_eeglab = ['AvRef_' fname_eeglab];
%         EEG = pop_reref( EEG, [],'refloc',struct('labels',{'P3'},'type',{''},'theta',{-140.053},'radius',{0.34459},'X',{-57.5511},'Y',{48.2004},'Z',{39.8697},'sph_theta',{140.053},'sph_phi',{27.973},'sph_radius',{85},'urchan',{15},'ref',{'P3'}));
%         EEG = pop_saveset( EEG, 'filename',fname_eeglab,'filepath',[fpath filesep]);

EEG = pop_rmbase( EEG, [0   EEG.xmax*1e3]);
EEG = pop_saveset( EEG, 'filename',fname_eeglab,'filepath',[fpath filesep]);



Nc = size(data,1);  %Channels or components depending on if ICA has been carried out

%Add time axis to eeglab structure
EEG.times = info.time;

%% Event info
%Extract event info
if isfield(opts,'eventInfo'),
    eventTable = opts.eventInfo.eventTable;
    eventFields = opts.eventInfo.eventFields;
else
    eventTable = [];
    eventFields = {};
    warning('Ingen event info - fix dette!!')
    eeglab redraw
    EEG = pop_saveset( EEG, 'filename',fname_eeglab,'filepath',[fpath filesep]);
    return
end

%Add eventinfo to eeglab structure
eeglab redraw
EEG = eeg_checkset( EEG );
eventTable = mattocell(eventTable);     %convert to cell format
EEG = pop_importevent( EEG, 'event',eventTable,'fields',eventFields,'timeunit',1);
EEG = eeg_checkset( EEG );

pop_eegplot( EEG, 1, 1, 1);
eeglab redraw

fname_eeglab = ['E_' fname_eeglab];
EEG = pop_saveset( EEG, 'filename',fname_eeglab,'filepath',[fpath filesep]);


%% Do epoching
if ~isempty(timeEpoch)
    EEG = pop_epoch( EEG, events , timeEpoch, 'newname', [EEG.setname '_epoch'], 'epochinfo', 'yes');
    eeglab redraw
else
    disp('No epoching performed as opts.timeEpoch not specified.')
    EEG = pop_saveset( EEG, 'filename',fname_eeglab,'filepath',[fpath filesep]);
    return
end

fname_eeglab = ['epoch' fname_eeglab];
EEG = pop_saveset( EEG, 'filename',fname_eeglab,'filepath',[fpath filesep]);

if ~isempty(timeBaseline)
    fname_eeglab = ['BC' fname_eeglab];
    EEG = pop_rmbase( EEG, timeBaseline);
    EEG = pop_saveset( EEG, 'filename',fname_eeglab,'filepath',[fpath filesep]);
else
    disp('No baseline correction will be carried out as opts.timeBaseline not specified.')
end

