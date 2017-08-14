%% ת������Ϊ���
function t = object_2_table(obj)
    fields = fieldnames(obj);
    %disp('>>>>>>>>>>')
    [m,n] = size(fields);
    %disp(['m=' num2str(m) 'n=' num2str(n)])
    data = cell(1,m);
    for i = 1:m
        tmp = getfield(obj,fields{i});
        if ischar(tmp)
            % ������ֻ��У���ӡ���ʱ����ʾ[1x17 char]�������Ѻ�
            data{i} = strjoin(strsplit(tmp,{'\r','\n'}),'\t');
        else
            data{i} = tmp;
        end
    end
    
    %disp('<<<<<<<<<<<<')
    
    t = cell2table(data);
    t.Properties.VariableNames = fields;
end
