%load Data
[T_Value, T_Name, Data, Fea_Name] = load_hs_vaalikone('data_vk_training.csv');
d = length(Fea_Name);
n = length(Data(:,1));
%Principal components analysis
%test data size
test_size = 800;
%[COEFF,SCORE,latent] = princomp(Data);



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%mean of i=1 elected

M=zeros(2,d); % declaring a new matrix which will contain the mean of different dimensions for both the class of "elected" and "not elected"



for j=1:d
   k1 = 0; k0 = 0;
   for i=1:test_size
      if(T_Value{i,1} == '1')
         k1 = k1 +  Data(i,j);
      else
         k0 = k0 + Data(i,j);
      end
   end
   M(1,j) = k1 / test_size; % mean of jth dimension for the class "elected" is being assigned to M(1,J)
   M(2,j) = k0 / test_size; % mean of jth dimension for the class "not elected" is being assigned to M(2,J)
end


result = zeros(n-test_size,3);

%Vaild 
elected_correct = 0;
elected = 0;
not_elected_correct = 0;
not_elected = 0;
for i=test_size+1:n
    X = Data(i,:);
    k1 = 0; k0 = 0;
    for j=1:d
       k1 = k1 + power((X(j) - M(1,j)),2); 
       k0 = k0 + power((X(j) - M(2,j)),2);
    end
    g1 = -1 * k1;
    g0 = -1 * k0;
    if g1 >= g0
        result(i-test_size,1) = i;
        result(i-test_size,2) = 1;
        result(i-test_size,3) = T_Value{i,1}-48;
    else
        result(i-test_size,1) = i;
        result(i-test_size,2) = 0;
        result(i-test_size,3) = T_Value{i,1}-48;
    end
end

TP = 0;
FP = 0;
FN = 0;
TN = 0;
for i = 1:length(result)
    
    if(result(i,2) == result(i,3))
        if(result(i,2) == 1)
            TP = TP + 1;
        else
            TN = TN + 1;
        end
    else
        if(result(i,2) == 1)
            FP = FP + 1;
        else
            FN = FN +1;
        end
    end
end


if(TP > 0)
    Precision = TP/(TP+FP);
    Recall = TP / (TP+FN);

    F_elected = 2 * Precision * Recall / (Precision + Recall);
else
    F_elected = 0;
end

if(TN > 0)
    Precision = TN/(TN+FP);
    Recall = TN / (TN+FN);

    F_not_elected = 2 * Precision * Recall / (Precision + Recall);
else
    F_not_elected = 0;
end


% From here it starts the classification of parties


unique_parties = length(unique(T_Value(1:test_size,2))); % counting the no of parties available in the training data

party_names = unique(unique(T_Value(1:end,2))); % preserving the unique name of the parties in this matrix named party_names

party_details = zeros(unique_parties,d+2); % creating a matrix to keep the detail info about the parties

for i = 1:test_size
    for j = 1:unique_parties
       if (strcmp(T_Value(i,2),party_names(j,1)) == 1)
           party_details(j,1) = party_details(j,1) + 1; % counting the number of candidates for each party and storing it in the first column of the party_detail matrix
       end
    end 
end

for j = 1:unique_parties
       
           party_details(j,2) = party_details(j,1)/test_size; % calculating the probability of a certain party and storing it in the second column of the party_details matrix
end



for j=1:d
   for i=1:test_size
       for r = 1:unique_parties
           if(strcmp(T_Value{i,2},party_names(r,1)) == 1)
                party_details(r,j+2) = party_details(r,j+2) +  Data(i,j); % summing up the values of a certain dimension for all the samples under the same party and storing them in party_details matrix
           end
       end  
   end
  for s = 1:unique_parties
    party_details(s,j+2) = party_details(s,j+2)/party_details(s,1); % calculating the mean value of a certain dimension under a certain party and storing them in party_details matrix
  end
  
end



result_party = zeros(n-test_size,3); % result will be stored in this matrix
dvalues = zeros(unique_parties,1); % this matrix will temporarily hold the values of discrimator function of each class for a certain sample
%Vaild 
for i=test_size+1:n %iterate over each sample
    X = Data(i,:);
    for j=1:d % iterate over each dimension
        for p = 1:unique_parties %iterate over each party
            dvalues(p,1) = dvalues(p,1) + power((X(j) - party_details(p,j+2)),2); %calculate the value of the discriminator function
        end
    end
    
    for p = 1:unique_parties
            dvalues(p,1) = -1 * dvalues(p,1); %complete the calculation of the discriminator functions associated with each party
    end
    
    max = -10000000000; % arbitrarily set a very very small value
    
    for p = 1:unique_parties
            if (max < dvalues(p,1)) 
                max = dvalues(p,1);
                result_party(i-test_size,1) = i;
                result_party(i-test_size,2) = p;    % party is being predicted here
            end
    end
    
    for u = 1:unique_parties
          if(strcmp(T_Value{i,2},party_names(u,1)) == 1)
              result_party(i-test_size,3) = u; % finding out the actual party label of the candidate
              break;
          end
    end   
    

end


% now F-score for each party and their average will be calculated

F_party_sum = 0;
Party_TP = 0;
Party_TN = 0;
Party_FP = 0;
Party_FN = 0;

correct = 0;
incorrect =0;
for i = 1:length(result_party)
         if(result_party(i,2) == result_party(i,3))
            correct = correct+1;
        else 
            incorrect = incorrect+1;
         end
end

for s = 1:unique_parties

Party_TP = 0;
Party_TN = 0;
Party_FP = 0;
Party_FN = 0;

    for i = 1:length(result_party)
       
        %fprintf('\nI am here when predicted value is: %1f  and actual label is:%1f  and S = %1f\n',result_party(i,2),result_party(i,3),s);
        if(result_party(i,2) == s)
            %fprintf('I have found match\n');
            if(result_party(i,2) == result_party(i,3))
                Party_TP = Party_TP + 1;
             %   fprintf('I am counting TP = %1f \n',Party_TP);
            else
                Party_FP = Party_FP + 1;
              %  fprintf('I am counting FP = %1f  \n',Party_FP);
            end
        else
            %fprintf('I have found mismatch \n');
            if(result_party(i,3) == s)
                Party_FN = Party_FN + 1;
              %  fprintf('I am counting FN = %1f  \n',Party_FN);
            else
                Party_TN = Party_TN + 1;
               % fprintf('I am counting TN = %1f  \n',Party_TN);
            end
        end
    end
    
    fprintf('\n\n Checking Party: %1f \n Party_TP = %1f \n Party_FP = %1f \n Party_TN = %1f \n Party_FN = %1f', s, Party_TP, Party_FP, Party_TN, Party_FN);
    
    if(Party_TP > 0)
        Precision = Party_TP/(Party_TP+Party_FP);
        Recall = Party_TP / (Party_TP+Party_FN);

        F_party = 2 * Precision * Recall / (Precision + Recall);
        fprintf('\n F_party(%1f): %1.5f',s,F_party);
    else
        F_party = 0;
        fprintf('\n F_party(%1f): %1.5f',s,F_party);
    end
%fprintf('\nFor the party %1.0f F-score is is %1.20f',s,F_party);
F_party_sum = F_party_sum + F_party;
end

F_party = F_party_sum / length(unique(result_party(1:end,3)));

fprintf('\n\n Nearest Mean Classifier is used. \n Here is the evaluation: \n F_elected = %1.5f \n F_not_elected = %1.5f \n F_party = %1.5f \n',F_elected,F_not_elected,F_party);

