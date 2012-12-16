function [eeg gyro mrk info] = sbs2_rawdata2matlab(fname,fs,opts)
%==========================================================================
% Filename: sbs2_rawdata2matlab.m (function).
%
% Description:  Read data saved in smartphone brain scanner2 (sbs2)
%               format.
%
% Input:        fname: Filename without file-extension
%                  fs: Sampling frequency
%
%                opts: Optional parameters
%                   .noHeaderLines: (Default: 1)
%                   .flag_metaFile: Read meta file [fname '.meta']
%                                   (Default: true)
%
% Output:       eeg: EEG recordings
%               gyro: data gyroscope [X Y]
%               mrk:
%                          .y: Class labels
%                        .pos: Time position of stimuli occurring
%                 .classLabel: Event name read from meta file. Reads the
%                              name until '_', which allows for sub-
%                              classes e.g. 'motor_left' will have motor
%                              being saved as classLabel.
%                       .text: Sub-class text of event name. E.g.
%                              event_name 'motor_left' saves 'left' to
%                              mrk.text.
%
%               info:
%                       .time: Time axis with 1/fs time steps.
%                   .time_abs: Time axis - logged in .txt relative to first
%                              package.
%                         .fs: Sampling frequency
%                     .chlist: Channel list
%
% Example:
%           fname = 'DemoData'
%           [data gyro mrk info] = sbs2_rawdata2matlab(fname,128,struct('noHeaderLines',1));
%
% History:
%   - Created:  18/07/2012
%
% Author: Carsten Stahlhut
%
% Copyright (C) SBS2, DTU Informatics, 2012
%==========================================================================

if nargin<3
    opts = [];
end

if isfield(opts,'noHeaderLines'), noHeaderLines = opts.noHeaderLines; else noHeaderLines = 1; end
if isfield(opts,'flag_metaFile'), flag_metaFile = opts.flag_metaFile; else flag_metaFile = true; end

% Read header - extract channel list
fname
fid = fopen([fname '.txt'],'r');
lineno = 0;
tline = fgetl(fid);                 %Read meta file line by line
temp = regexp(tline,' ','split');
fclose(fid)
chlist = temp(5:end-2);
Nc = length(chlist);
clear fid tline lineno temp

% Read data
Nbits = 14;     %14 bits unsigned integers
[no timestamp gyroX gyroY eeg{1:Nc} counter ChBadStr] = textread([fname '.txt'],...
    '%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %s','delimiter',...
    '\t','headerlines',noHeaderLines,'emptyvalue',NaN);


ChBad.lineNo = find(~cellfun(@isempty,ChBadStr));
ChBadStr(cellfun(@isempty,ChBadStr)) = [];
ChBad.string = ChBadStr;

gyro = [gyroX gyroY];
eeg = cell2mat(eeg);
% eeg = eeg-2^(Nbits-1);

if flag_metaFile
    mrk = get_classinfo([fname '.meta'],timestamp,no,fs,opts);
else
    mrk.pos = timestamp(1);
end

% Change to a relative time axis in stead of absolute time. t=0 when
% recording starts
if log10(mrk.pos(1))>12
    mrk.pos = mrk.pos/1e3;      %tr
    disp('Transforming mrk.pos from ms to seconds')
end

if log10(timestamp(1))>12
    timestamp = timestamp/1e3;      %tr
    disp('Transforming absolut time axis from ms to seconds')
end

pos = mrk.pos-timestamp(1);

info.time_abs = timestamp-timestamp(1);
info.time = (0:length(timestamp)-1)/fs;
info.fs = fs;
info.chlist = chlist;

for i=1:length(pos)
    mrk.pos(i) = find( info.time >= pos(i),1,'first');
end



function mrk = get_classinfo(fname,timestamp,no,fs,opts)
%==========================================================================
% Function name: get_classinfo.m (function).
%
% Description:
%
% Input:        fname: Filename with extension .meta
%           timestamp: Time axis to align with - the timestamps from the
%                      associated data file.
%                  no: Line number in data file
%   fhandle_classInfo:
%
% Output:        mrk:
%                   .y: Class labels
%                   .time: Time position of stimuli occurring
%==========================================================================

if isfield(opts,'fhandle_classInfo'), fhandle_classInfo = opts.fhandle_classInfo; else fhandle_classInfo = []; end
if isfield(opts,'package_offset'), package_offset = opts.package_offset; else package_offset = 0; end
flag_trlOnsetDelay = false;

class_num = 1;

task = [];
class_trn = [];
time_trn = [];

trlOnset = zeros(no(end),1);
trlOnset_correction = trlOnset;
trlOnsetTime = [];
trlOnsetPackage = [];
marker_pos = [];

trlClass = cell(0);
NameList = cell(0);

fid = fopen(fname,'r');
lineno = 0;
while 1
    tline = fgetl(fid);                 %Read meta file line by line
    lineno = lineno+1;
    if ~ischar(tline), break, end
    
    string_name = [];
    switch lineno
        case 1
            temp = regexp(tline(length(string_name)+1:end),':','split');
            subjectName  = temp{1}; Experiment = temp{2}; tstartTime = temp{3}; tstartPackage = temp{4};
            
        case 2
            temp = regexp(tline(length(string_name)+1:end),':','split');
            ExpText = temp{1};  tstartTime = str2double(temp{2}); tstartPackage = temp{3};
            
        otherwise
            idx = lineno-2;
            temp = regexp(tline(length(string_name)+1:end),':','split');
            
            trlOnsetTime(idx) = str2double(temp(2));
            trlOnsetPackage(idx) = str2double(temp(3));
            
            ExpText = regexp(temp{1},'_','split');
            
            trlClass(idx) = ExpText(1);        %Get class of trial
            try NameList(lineno-2) = ExpText(2); catch NameList{idx} = []; end    %Get the name of the NameList
            
            % Check for delay in events presented relative to the eeg
            % signal.
            if length(ExpText)>2
                flag_trlOnsetDelay = true;
                
                trlOnset_TimeDelay = str2double(ExpText(3))+package_offset;
                trlOnsetPackageDelay = round( (trlOnset_TimeDelay-trlOnsetTime(idx) )*fs/1e3 );
                trlOnsetOrigPackage = trlOnsetPackage(idx);
                trlOnsetPackage(idx) = trlOnsetPackageDelay + trlOnsetOrigPackage;
                
                trlOnset_correction(idx) = timestamp(no==trlOnsetPackage(idx)) - timestamp(no==trlOnsetOrigPackage);
            end
            trlOnset(idx) = timestamp(no==trlOnsetPackage(idx));
    end
end
fclose(fid);

trlOnset = trlOnset(1:idx);
trlOnset_correction = trlOnset_correction(1:idx);
clear idx

% trlOnset = timestamp(no());
% % wtrlOnsetPackagearning('deleting the last field in trlClass')
% % trlOnset(end:end) = [];
% % trlClass(end:end) = [];
% % NameList(end) = [];


classLabel = unique(trlClass);
classLabel(cellfun(@isempty,classLabel)) = [];  %Delete empty cells

Nevents = length(trlClass);
y = zeros(Nevents,1);
for iclass=1:length(classLabel)
    classcheck = strfind(trlClass,classLabel{iclass});
    list_iclass = ~cellfun(@isempty,classcheck);
    y(list_iclass) = iclass;
end

if isempty(fhandle_classInfo)
    idx = 1:Nevents;              %Trl started
else
    [y idx] = feval(fhandle_classInfo,NameList);
end

Ntrl = length(idx);

mrk.y = y(idx);
mrk.pos = trlOnset(idx);
mrk.classLabel = classLabel;
mrk.text = NameList(idx);
mrk.pos_packageno = trlOnsetPackage(idx);
if flag_trlOnsetDelay
    mrk.pos_correction = trlOnset_correction;
end
