clear
fid = fopen('training_data.txt');
A = fscanf(fid, '%f %f', [2 inf]);
fclose(fid);

fid = fopen('test_data.txt');
B = fscanf(fid, '%f %f', [2 inf]);
fclose(fid);

hold on;


totalTrainingSize = 10;
trainingSize = 7;
validationSize = 3;
testSize = 3716;

K = 6;



A = A';

B = B';

    for i=1:trainingSize
        Input(i,1) = A(i,1);
        R(i,1) = A(i,2);
    end
    
   for i = trainingSize+1:(trainingSize+validationSize)
        Validation(i-trainingSize,1) = A(i,1);
        ValidationR(i-trainingSize,1) = A(i,2);
   end
    
    
    for i = 1:totalTrainingSize
        TVInput(i,1) = A(i,1);
        TVR(i,1) = A(i,2);
    end
    
    
   for i = 1:testSize
        TestInput(i,1) = B(i,1);
        TestR(i,1) = B(i,2);
    end
   
   
   

Input = Input';
R = R';

Validation = Validation';
ValidationR = ValidationR';


TVInput = TVInput';
TVR = TVR';


TestInput = TestInput';
TestR = TestR';





    for k = 0:K
       
        for i = 1:trainingSize
           Phi(k+1,i) = Input(i)^k;
        end
  
    W = R*Phi'*(inv(Phi*Phi'));
    
    W
    
    for l=1:k+1
        Result(k+1,l) = W(l);
    end
    
    
   Error = 0;
    hypothesisValue = 0;
       
    for i=1:trainingSize
        
        for j = 0:k
            
            hypothesisValue = hypothesisValue + W(1,j+1)*Phi(j+1,i);
        end
        
        ytraining(k+1,i) = hypothesisValue;
            
        Error = Error + (R(1,i) - hypothesisValue)^2;
        
        hypothesisValue = 0;
        
    end
    
    TrainingError = (Error/trainingSize);
    
   TrainingError
    
    Errors(k+1,1) = TrainingError;
    
  
    
    Error = 0;
    
    for i = 1:validationSize
           PhiValidation(k+1,i) = Validation(i)^k;
    end
    
    
    
   for i=1:validationSize
        
        for j = 0:k
             hypothesisValue = hypothesisValue + W(1,j+1)*PhiValidation(j+1,i);
        end
        
        yvalidation(k+1,i) = hypothesisValue;
        
        Error = Error + (ValidationR(1,i) - hypothesisValue)^2;
        
        hypothesisValue = 0;
        
   end
   
   ValidationError = (Error/validationSize);
   Errors(k+1,2) = ValidationError;
   
   ValidationError
       
   
   Error = 0;
          
    for i = 1:totalTrainingSize
           PhiTVInput(k+1,i) = TVInput(i)^k;
    end
    
    
    
   for i=1:totalTrainingSize
        
        for j = 0:k
             hypothesisValue = hypothesisValue + W(1,j+1)*PhiTVInput(j+1,i);
        end
        
        Error = Error + (TVR(1,i) - hypothesisValue)^2;
        
        hypothesisValue = 0;
        
   end
   
   TVError = (Error/totalTrainingSize);
   Errors(k+1,3) = TVError;
   
   TVError
   
   
      Error = 0;
          
    for i = 1:testSize
           PhiTestInput(k+1,i) = TestInput(i)^k;
    end
    
    
    
   for i=1:testSize
        
        for j = 0:k
             hypothesisValue = hypothesisValue + W(1,j+1)*PhiTestInput(j+1,i);
        end
        
        ytest(k+1,i) = hypothesisValue;
        
        Error = Error + (TestR(1,i) - hypothesisValue)^2;
        
        hypothesisValue = 0;
        
   end
   
   TestError = (Error/testSize);
   Errors(k+1,4) = TestError;
   
   TestError
   
   
   
   
   
       
    end
   
   plot(Input,R,'o');
   plot(Validation,ValidationR,'*')
   %legend('Training data');
   
   %plot(TestInput,TestR,'r*');
   
   xlabel('x');
   ylabel('y');
   title('Training and Validation'); 
   
   x = 0:.001:1.5;
   y1 = @(x)1.2591*x.^0;
   %plot(x,y1(x),'green','LineWidth',2);
   %legend('degree 0');
   y2 =@(x)0.9595 + 0.3444*x;
   %plot(x,y2(x),'yellow','LineWidth',2);
   %legend('degree  1');
   y3 =@(x)0.8094 + 1.2336*x - 0.6244*x.^2;
   %plot(x,y3(x),'blue','LineWidth',2);
   %legend('degree  2');
   y4 =@(x)0.8552 + 0.8106*x + 0.0232*x.^2-0.2675*x.^3;
   %plot(x,y4(x),'black','LineWidth',.5);
   %legend('degree 3');
   y5 =@(x)1.1865-2.6468*x + 8.0724*x.^2-7.2140*x.^3 + 2.034*x.^4;
   %plot(x,y5(x),'black','LineWidth',2);
   %legend('degree  4');
   y6 =@(x)0.4516 + 7.9751*x-39.9080*x.^2 + 75.8405*x.^3-58.8348*x.^4 + 15.9794*x.^5 ;
   %plot(x,y6(x),'red','LineWidth',2);
   %legend('degree  5');
   y7 =@(x)-0.3356 + 22.7281*x-131.0983*x.^2 + 295.4919*x.^3-306.8257*x.^4 +148.8833*x.^5 - 27.3936*x.^6;
   %plot(x,y7(x),'cyan','LineWidth',2);
   %legend('degree  6');
   
   plot(x,y1(x),x,y2(x),x,y3(x),x,y4(x),x,y5(x),x,y6(x),x,y7(x),'LineWidth',2);
   
   legend('Training Data','Validation Data','Degree 0','Degree 1','Degree 2','Degree 3','Degree 4','Degree 5','Degree 6');
   
  
   
   

   
       
   
  
   
   
    