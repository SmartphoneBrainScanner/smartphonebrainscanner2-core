function [y idx] = sbs2_demo_class(NameList)
%==========================================================================
% Filename: verb_classes.m
% 
% Description:  
%           
% Input:        NameList: List with names of events in a cell-array
%
% Output:       y:   Class labels
%               idx: Indices of events with classlabels different from zero.
%
% Created: 2012/02/08
%
% Special remarks:
%
% Author: Carsten Stahlhut
%
% Copyright (c) Technical Univeristy of Denmark, DTU Informatics
%==========================================================================


hand = {'grasp','push'};

leg = {'kick','bend'};

y = zeros(length(NameList),1);
for iname=1:length(NameList)
 
    if ~isempty(NameList{iname})
        switch NameList{iname}
            case hand
                y(iname) = 1;
            case leg
                y(iname) = 2;
            otherwise
                error('Verb not part of the active list.')
        end
    end
end

idx = find(y~=0);

end