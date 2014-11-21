%load Data
[T_Value, T_Name, Data, Fea_Name] = load_hs_vaalikone('data_vk_training.csv');
d = length(Fea_Name);
n = length(Data(:,1));
%Principal components analysis
%test data size
test_size = 800;
[COEFF,SCORE,latent] = princomp(Data);




%checks how many principle component should be considered
for i=1:n
    if(sum(latent(1:i,1)) > 0.8*sum(latent))
        break;
    end
end
d = i; % d is being assigned with the number of principal components that we will consider


TrainData = SCORE(1:test_size,1:d); % TrainData is assigned with the data taken from the pricipal component space

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%mean of i=1 elected

M=zeros(2,d); % declaring a new matrix which will contain the mean of different dimensions for both the class of "elected" and "not elected"





%Vaild 
elected_correct = 0;
elected = 0;
not_elected_correct = 0;
not_elected = 0;


this_many_nearest = 3;

result = zeros(n-test_size,3);
KnearestTracker = zeros(this_many_nearest,2);
KnearestTracker = -100000 + KnearestTracker;


%KNN = zeros(5,1);
nearest = -1000000;


not_elected_majority = 0; elected_majority = 0;

for i=test_size+1:n
    not_elected_majority = 0; elected_majority = 0;
    KnearestTracker = -100000 + 0*KnearestTracker;
    X = SCORE(i,:);
        for s = 1:test_size
            Y = SCORE(s,:);
            k1 = 0;
            for j=1:d
               k1 = k1 + power((X(j) - Y(j)),2); 
            end
            g1 = -0.1 * k1;
            
            for r = 1:this_many_nearest
                if (g1 > KnearestTracker(r,2))
                    KnearestTracker(r,1) = T_Value{s,1}-48;
                    KnearestTracker(r,2) = g1;
                    break;
                end
            end
        end
         
        for r = 1:this_many_nearest
                if (KnearestTracker(r,1) == 0)
                    not_elected_majority = not_elected_majority + 1;
                else
                    elected_majority = elected_majority + 1;
                end
        end
        
    if (elected_majority/not_elected_majority >= .5)
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

% From here it starts the classification of parties


unique_parties = length(unique(T_Value(1:test_size,2))); % counting the no of parties available in the training data

party_names = unique(unique(T_Value(1:end,2))); % preserving the unique name of the parties in this matrix named party_names

party_details = zeros(unique_parties,d+2); % creating a matrix to keep the detail info about the parties


this_many_nearest = 8;
avg_dist = -1000000;

result = zeros(n-test_size,3);
KnearestTracker = zeros(this_many_nearest,2);
KnearestTracker = -100000 + KnearestTracker;


KnearestCountTracker = zeros(unique_parties,4);




result_party = zeros(n-test_size,3);

for i=test_size+1:n
    for w = 1:unique_parties
        KnearestCountTracker(w,1) = w;
        KnearestCountTracker(w,2) = 0;
        KnearestCountTracker(w,3) = 0;
    end
    not_elected_majority = 0; elected_majority = 0;
    KnearestTracker = -100000 + 0*KnearestTracker;
    X = SCORE(i,:);
        for s = 1:test_size
            Y = SCORE(s,:);
            k1 = 0;
            for j=1:d
               k1 = k1 + power((X(j) - Y(j)),2); 
            end
            g1 = -0.1 * k1;
            
            for r = 1:this_many_nearest
                if (g1 > KnearestTracker(r,2))
                    
                    
                        for u = 1:unique_parties
                            if (strcmp(party_names(u,1),T_Value{s,2}) == 1)
                                KnearestTracker(r,1) = u;
                            end
                        end
                    
                        KnearestTracker(r,2) = g1;
                        break;
                end
            end
        end
        
              
        for r = 1:this_many_nearest
                for v = 1:unique_parties
                    if(KnearestTracker(r,1) == KnearestCountTracker(v,1))
                        KnearestCountTracker(v,2) = KnearestCountTracker(v,2) + 1;
                        KnearestCountTracker(v,3) = KnearestCountTracker(v,3) + KnearestTracker(r,2);
                    end
                end
        end
        
        max = 0;
        avg_dist = -1000000;
        
        for v = 1:unique_parties
             
                    if( max < KnearestCountTracker(v,2))
                        max = KnearestCountTracker(v,2);
                        party = v;
                        avg_dist = KnearestCountTracker(v,3)/KnearestCountTracker(v,2);
                    elseif (max == KnearestCountTracker(v,2))
                            if (KnearestCountTracker(v,3)/KnearestCountTracker(v,2) > avg_dist )
                                max = KnearestCountTracker(v,2);
                                party = v;
                                avg_dist = KnearestCountTracker(v,3)/KnearestCountTracker(v,2);
                            end
                    else
                    end
        end
        
        result_party(i-test_size,1) = i;
        result_party(i-test_size,2) = party;
        
        for x = 1:unique_parties
          if(strcmp(T_Value{i,2},party_names(x,1)) == 1)
              result_party(i-test_size,3) = x; % finding out the actual party label of the candidate
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




fprintf('\n\n PCA and K Nearest Neighbour is used. \n Here is the evaluation: \n F_elected = %1.5f \n F_not_elected = %1.5f \n F_party = %1.5f \n',F_elected,F_not_elected,F_party);

