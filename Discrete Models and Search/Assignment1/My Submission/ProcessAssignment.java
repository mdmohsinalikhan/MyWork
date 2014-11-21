
/*
 * This is some demo Java code for reading in a problem instance and
 * initial assignment for the first programming assignment of T-79.4101
 * Discrete Models and Search. For more information see Noppa.
 *
 * Note: the main method here just reads in the problem instance file and
 * the initial assignment and then prints both on the screen.
 */


import java.io.*;
import java.util.StringTokenizer;
import java.util.Vector;
import java.util.Collections;

class ProcessAssignment  {

    // number of resources (N_R)
    static int numResources = 0;

    // number of machines (N_M)
    static int numMachines = 0;

    // number of processes (N_P)
    static int numProcesses = 0;

    // number of services (N_S)
    static int numServices = 0;

    // number of locations (N_L)
    static int numLocations = 0;

    static Vector< Vector<Integer> > machineCapacities = new Vector< Vector< Integer > >();
    static Vector< Vector<Integer> > softMachineCapacities = new Vector< Vector< Integer > >();
    static Vector< Integer > machineLocations = new Vector< Integer >();
    static Vector< Integer > serviceMinSpreads = new Vector< Integer >();
    static Vector< Integer > processServices = new Vector< Integer >();
    static Vector< Vector<Integer> > processRequirements = new Vector< Vector< Integer > >();
    static Vector< Integer > processMovingCosts = new Vector< Integer >();

    static Vector< Integer > initialAssignment = new Vector< Integer >();

//added variables by me
    static Vector< Integer > neighborAssignment = new Vector< Integer >();
    static Vector< Integer > currentBestNeighborAssignment = new Vector< Integer >();
    static Vector< Vector<Integer> > machineUtilized = new Vector< Vector< Integer > >();
    static Vector<Vector<Integer>> serviceLocations = new Vector<Vector<Integer>>();
    static Vector< Integer > firstAssignment = new Vector< Integer >();
    static int cost,current_best;
    static int temp_best=0;

    // helper function for asserting that input values are in correct range
    // and other checks work out
    private static void assertCondition(boolean condition) {
	if(!condition) {
	    System.out.println("assert violated!");
	    System.exit(0);
	}
    }

    // reads in an instance file
    // Note: this is a very crude method of reading the input file!
    // (meaning there is only very basic checking for errors)
    private static void readInstanceFile(String fileName) {
	try{
	    FileInputStream fstream = new FileInputStream(fileName);
	    DataInputStream in = new DataInputStream(fstream);
	    BufferedReader br = new BufferedReader(new InputStreamReader(in));
	    String strLine;
	    
	    numResources = Integer.parseInt(br.readLine());
	    assertCondition(1 <= numResources && numResources <= 10);

	    numMachines = Integer.parseInt(br.readLine());
	    assertCondition(1<= numMachines && numMachines <= 500);

	    System.out.println("LOG: numResources and numMachines");

	    for(int m=0;m<numMachines;m++) {
		StringTokenizer st = new StringTokenizer(br.readLine());

		machineCapacities.add(new Vector< Integer >());
		softMachineCapacities.add(new Vector< Integer >());

		// read in location of machine m
		int location_m = Integer.parseInt(st.nextToken());
		assertCondition(0<= location_m && location_m < numMachines);

		if(numLocations < location_m + 1)
		    numLocations = location_m + 1;

		machineLocations.add(new Integer(location_m));

		// read in machine capacities (for MCCon)
		for(int r=0;r<numResources;r++) {
		    machineCapacities.get(m).add(new Integer(Integer.parseInt(st.nextToken())));
		}

		// read in soft machine capacities (for MLCost)
		for(int r=0;r<numResources;r++) {
		    softMachineCapacities.get(m).add(new Integer(Integer.parseInt(st.nextToken())));
		}
	    }

	    System.out.println("LOG: read machines");

	    numServices = Integer.parseInt(br.readLine());
	    assertCondition(1<= numServices && numServices <= 2000);

	    for(int s=0;s<numServices;s++) {
		// read in minSpread of service s
		int minSpread_s = Integer.parseInt(br.readLine());
		assertCondition(0<= minSpread_s && minSpread_s <= numLocations);

		serviceMinSpreads.add(new Integer(minSpread_s));
	    }

	    System.out.println("LOG: read services");

	    numProcesses = Integer.parseInt(br.readLine());
	    assertCondition(1<= numProcesses && numProcesses <= 2000);

	    for(int p=0;p<numProcesses;p++) {
		StringTokenizer st = new StringTokenizer(br.readLine());

		// read in service of process p
		int service_p = Integer.parseInt(st.nextToken());
		assertCondition(0<= service_p && service_p < numServices);

		processServices.add(new Integer(service_p));

		processRequirements.add(new Vector<Integer>(numResources));

		// read in process requirements
		for(int r=0;r<numResources;r++) {
		    processRequirements.get(p).add(new Integer(Integer.parseInt(st.nextToken())));
		}

		int processMovingCost_p = Integer.parseInt(st.nextToken());
		assertCondition(0<= processMovingCost_p && processMovingCost_p <= 1000);

		processMovingCosts.add(new Integer(processMovingCost_p));
	    }

	    System.out.println("LOG: read processes");
	    System.out.println("LOG: read ALL");
	    

	    //Close the input stream
	    in.close();
	} catch (Exception e){//Catch exception if any
	    System.err.println("ERROR: " + e.toString());
	}
    }

    // reads in the initial process to machine assignment
    // note: should be called after function readInstanceFile()
    static void readInitialAssignment(String fileName) {
	try{
	    FileInputStream fstream = new FileInputStream(fileName);
	    DataInputStream in = new DataInputStream(fstream);
	    BufferedReader br = new BufferedReader(new InputStreamReader(in));
	    StringTokenizer st = new StringTokenizer(br.readLine());

	    assert(st.countTokens() == numProcesses);

	    for(int p=0;p<numProcesses;p++)
		initialAssignment.add(new Integer(Integer.parseInt(st.nextToken())));

	    //Close the input stream
	    in.close();
	} catch (Exception e){//Catch exception if any
	    System.err.println("ERROR: " + e.toString());
	}
    }

    // dumps the instance to the screen
    private static void dumpProblem() {
	System.out.println("Problem instance:");
	System.out.println("  Resources: "+numResources);
	System.out.println("  Machines: "+numMachines);
	System.out.println("  Processes: "+numProcesses);
	System.out.println("  Services: "+numServices);
	System.out.println("  Locations: "+numLocations);

	System.out.println("  MachineCapacities: (format: m(achine) capacities soft/hard)");

	for(int m=0;m<numMachines;m++) {
	    System.out.println("    m: "+m);
	    System.out.print("       ");

	    for(int r=0;r<numResources;r++) {
		System.out.print(softMachineCapacities.get(m).get(r)+"/"+machineCapacities.get(m).get(r));
		
		if(r<numResources-1)
		    System.out.print(", ");
		else
		    System.out.println("");
	    }
	}

	System.out.println("  MachineLocations: (format: m(achine) l(ocation))");
	
	for(int m=0;m<numMachines;m++)
	    System.out.println("    m: "+m+ " l: "+machineLocations.get(m));

	System.out.println("  Services spreadMin: (format: s(ervice) (spread)M(in))");

	for(int s=0;s<numServices;s++)
	    System.out.println("    s: "+s+ " M: "+serviceMinSpreads.get(s));

	System.out.println("  ProcessRequirements: (format: p(rocess) requirements)");

	for(int p=0;p<numProcesses;p++) {
	    System.out.println("    p: "+p);
	    System.out.print("       ");

	    for(int r=0;r<numResources;r++) {
		System.out.print(processRequirements.get(p).get(r));
		
		if(r<numResources-1)
		    System.out.print(", ");
		else
		    System.out.println("");
	    }
	}

	System.out.println("  ProcessServices: (format: p(rocess) s(ervice)))");

	for(int p=0;p<numProcesses;p++)
	    System.out.println("    p: "+p+" s: "+processServices.get(p));

	System.out.println("  ProcessMovingCosts: (format: p(rocess) (pm)c)");

	for(int p=0;p<numProcesses;p++)
	    System.out.println("    p: "+p+" c: "+processMovingCosts.get(p));

    }

    // dumps the given assignment to the screen
    private static void dumpAssignment(Vector< Integer > assignment) {
	for(int p=0;p<assignment.size();p++)
	    System.out.println("  " + p + " -> " + assignment.get(p));
    }






// Calculate the cost of an assignment
private static int calculateCost(Vector< Integer > assignment, Vector<Integer> assignment_s) {

	int temp=0,MLCost=0;

	//the following for loop initializes machineUtilized vector of machines. where each machine is also a vector of resources
	for(int m=0;m<numMachines;m++){
		for(int r=0;r<2*numResources;r++) {
		machineUtilized.get(m).set(r,0);
		}
	}

	//the following for loop iterates over all the processes. checks the machine it is assigned to. checks the requirement of each resources for that process. updates the current utilization of those resources at those machines in machineUtilized vector
	for(int p=0;p<assignment.size();p++){
		for(int r=0;r<numResources;r++) {
		temp = machineUtilized.get(assignment.get(p)).get(r);
		machineUtilized.get(assignment.get(p)).set(r,temp+processRequirements.get(p).get(r));
		}
	}

	//the follwoing for loop iterates over all the machines in machineUtilized vector. compares the utilization of each resources with the softcapacity of that machine for that resource. and if utilization is greater than softcapacity then cost is considered.
	for(int m=0;m<numMachines;m++){
				
		for(int r=0;r<numResources;r++) {
		temp = machineUtilized.get(m).get(r) - softMachineCapacities.get(m).get(r);
			if(temp > 0)
			{
				machineUtilized.get(m).set(r+numResources,temp);
			}
		}
	}
	

	//summing up all the cost incurred at each machines
	for(int m=0;m<numMachines;m++){
		for(int r=0;r<numResources;r++) {
			MLCost = MLCost + machineUtilized.get(m).get(r+numResources);
		}
	}


int cost=MLCost;
int O_machine;
int I_machine;
int pmcost;


	//if the second parameter of this mehthod is of size 0 then it means, it is calculating the cost of the given initial assignment of the problem instance and process moving cost is irrelevant. but if it is greater than zero, that means the process moving cost has to be considered.
    	if(assignment_s.size() > 0)
	{
		for(int p = 0;p<numProcesses;p++)
		{
			O_machine = assignment.get(p);
			I_machine = assignment_s.get(p);
			if(O_machine == I_machine) // if in both of the assignment, process p is assigned to the same machine then do nothing.
			{
			}
			else //if the assigned machine to process p is different in the two assignment, then add the process moving cost
			{
				pmcost = processMovingCosts.get(p);
				cost = cost + pmcost;
			}
		}
	}

return cost;    	
}






// MCCon checking. This is needed if we want to check the constraint for the first initialassingment given with the problem instance. once it is validated, I will use some other function to check the constraint. because this function's complexity is higher
private static boolean mcconChecking(Vector< Integer > assignment) {
	Vector< Vector<Integer> > machineUtilized = new Vector< Vector< Integer > >();
	int temp=0,MLCost=0;

	//the following for loop initializes machineUtilized vector of machines. where each machine is also a vector of resources
	for(int m=0;m<numMachines;m++){
		machineUtilized.add(new Vector< Integer >());
		for(int r=0;r<2*numResources;r++) {
		machineUtilized.get(m).add(0);
		}
	}

	//the following for loop iterates over all the processes. checks the machine it is assigned to. checks the requirement of each resources for that process. updates the current utilization of those resources at those machines in machineUtilized vector
	for(int p=0;p<assignment.size();p++){
		for(int r=0;r<numResources;r++) {
		temp = machineUtilized.get(assignment.get(p)).get(r);
		machineUtilized.get(assignment.get(p)).set(r,temp+processRequirements.get(p).get(r));
		}
	}


	//the following for loop iterates over all the machines in machineutilized vector and for each machine it checks if there is any resource in that machine for which the utilization has exceeded the cpacity of the machine for that resource
	for(int m=0;m<numMachines;m++){
		for(int r=0;r<numResources;r++) {
			if(machineUtilized.get(m).get(r) > machineCapacities.get(m).get(r))
			{
				return false;
			}
		}
	}
	
return true;	
}







//SCCon Checking. This is needed if we want to check the constraint for the first initialassingment given with the problem instance. once it is validated, I will use some other function to check the constraint. because this function's complexity is higher
public static boolean scconChecking(Vector< Integer > assignment)
{
Vector< Vector<Integer> > machineServices = new Vector< Vector< Integer > >();
int temp=0,temp1=0;

//the follwoing for loop initializes a vector of machines which are also a vector to contain the services.
	for(int m = 0;m<numMachines;m++)
	{
		machineServices.add(new Vector<Integer>());

		for(int s=0;s<numServices;s++)
		{
			machineServices.get(m).add(0);	
		}
	}

//the following for loop iterates over all the processes. finds out the machine and service the process is assigned to. increment the value of the corresponding service of the machine by 1 in machineServices vector

	for(int p = 0;p<numProcesses;p++)
	{
		temp = processServices.get(p);
		temp1=machineServices.get(assignment.get(p)).get(temp);
		machineServices.get(assignment.get(p)).set(temp,temp1+1);			
	}

//if any service under any machine have count more than 1, that means that machine has at least 2 processes assigned to the same service
	for(int m = 0;m<numMachines;m++)
	{
		for(int s=0;s<numServices;s++)
		{
			if(machineServices.get(m).get(s) > 1)
			{
				return false;
			}
	
		}
	}

return true;

}




//SSCon checking.This is needed if we want to check the constraint for the first initialassingment given with the problem instance. once it is validated, I will use some other function to check the constraint. because this function's complexity is higher
public static boolean ssconChecking(Vector< Integer > assignment)
{
Vector< Vector<Integer> > serviceSpreaded = new Vector< Vector< Integer > >();
int temp=0;

//the follwoing for loop initializes a vector named "serviceSpreaded" of services where each service is also vector of locations.
	for(int s = 0;s<numServices;s++)
	{
		serviceSpreaded.add(new Vector<Integer>());

		for(int l=0;l<numLocations;l++)
		{
			serviceSpreaded.get(s).add(0);	
		}
	}

//the follwoing for loop iterates over each process. Finds out the machine it is assigned to and the location of that machine. Then the location of that machine is updated by 1 in the servicesSpreaded vector.

	for(int p = 0;p<numProcesses;p++)
	{
		temp = machineLocations.get(assignment.get(p));
		serviceSpreaded.get(processServices.get(p)).set(temp,1);			
	}
	

//the following for loop iterates over each services in the serviceSpreaded vector. for each service it checks how many locations under that service has been updated by 1. It indicates in how many locations that service has been spreaded over. If the count is less than its minimum spread count then it returns false
	int temp_loc = 0;
	for(int s = 0;s<numServices;s++)
	{
		temp = 0;
		for(int l=0;l<numLocations;l++)
		{
			temp_loc = serviceSpreaded.get(s).get(l);
			if (temp_loc == 1){
				temp++;
			}
		}
		if(temp < serviceMinSpreads.get(s))
		{
			return false;
		}

	}

return true;  //if the code reaches here, it means the constraint is passed.
}



//it check mccon. takes only the process i and machine j to which process i is going to be re-assigned. it doesnt take initialAssignment as a parameter as initialAssignment is static. This method considers the current initialAssignment is feasible.
public static boolean mcconChecking_mini(int i,int j)
{
			//iteates over all the resources of machine j in machineutilized vector. add the requirement of of the process i for that resource with the current utilization of that resource in that machine. and if it is greater than the capacity then the constraint is violated
			for(int r=0;r<numResources;r++) {
				if ( (machineUtilized.get(j).get(r) + processRequirements.get(i).get(r)) > machineCapacities.get(j).get(r))
				return false;
			}

return true; // the code reaches here only if the the constraint is passed
}






//it check sccon. takes only the process i and machine j to which process i is going to be re-assigned. it doesnt take initialAssignment as a parameter as initialAssignment is static. This method considers the current initialAssignment is feasible.
public static boolean scconChecking_mini(int i,int j)
{
			int service = -1,service1 = -1,temp = 0;
			//iterates over all the processes of initialAssignment
			for(int p=0;p<numProcesses;p++) 
			{
				temp = initialAssignment.get(p);
				if( temp == j) 	
				{ //the code reach here if p is running on the same machine where i is going to be re-assigned(that is, to machine j)
				        service = processServices.get(i); //check the service of process i
					service1 = processServices.get(p); // check the service of process p
					if( service == service1) // if they are same then the constraint is violated
					{
						return false;
					}
				}
			}
return true; // the code can reach here only if the constraint is passed
}






//it checks if ssconChecking is passed or not. returns true if passed, otherwise false. it takes only the process i and machine j to which process i is being-reassigned as input. the initialAssignment is a static vector of this class, so initialAssignment passing is not required. This method considers the current initialAssignment is feasible.
/*public static boolean ssconChecking_mini(int i,int j)
{
 			Vector<Integer> machineLocations_temp = new Vector<Integer>();
			int service = processServices.get(i);
			int service1 =0;
			int minspread = serviceMinSpreads.get(service);
			int count = 0;

			for(int l=0;l<numLocations;l++)
			{
				machineLocations_temp.add(0); //it makes a vector that will inidicate in which location the processes of the service of process i are located
			}
			
			//this for loop iterates over all the processes of initialAssignment and find out after reassigning of process i to machine j, which locations will have the processes of the service of process i.
			for(int p=0;p<numProcesses;p++) 
			{
				if(p != i)
				{
					service1 = processServices.get(p);
					if( service1 == service )	
					{
						machineLocations_temp.set(machineLocations.get(initialAssignment.get(p)),1);
					}
				}
				else
				{
					machineLocations_temp.set(machineLocations.get(j),1);

				}
			}

			int temp =0;
			//ths for loop iterates over all the location and check if there is a process of service of process i in that location
			for(int l=0;l<numLocations;l++)
			{

				temp = machineLocations_temp.get(l);
				if( temp == 1)
					count++;
			}
			
			if(count >= minspread)
				return true;
			else
				return false;
}*/







//it checks if ssconChecking is passed or not. returns true if passed, otherwise false. it takes only the process i and machine j to which process i is being-reassigned as input. the initialAssignment is a static vector of this class, so initialAssignment passing is not required. This method considers the current initialAssignment is feasible.
public static boolean ssconChecking_mini(int i,int j)
{
int temp = 0,count = 0,location_i=0,location_j=0;
int minspread = serviceMinSpreads.get(processServices.get(i));

			for(int l=0;l<numLocations;l++)
			{
				location_i = machineLocations.get(initialAssignment.get(i));
				location_j = machineLocations.get(j);
				if(l == location_i)
				  temp = serviceLocations.get(processServices.get(i)).get(l) - 1;
				else if(l == location_j)
				  temp = serviceLocations.get(processServices.get(i)).get(l) + 1;
				else 
				  temp = serviceLocations.get(processServices.get(i)).get(l);

				if( temp > 0)
					count++;
			}

			if(count >= minspread)
				return true;
			else
				return false;


}



















//it updates the machineUtilized vector. it does it considering that the static vector initialAssignment is holding the current_best assigment.
public static void updateMachineUtilization(int i, int j)
{

int current_utilization=0,will_reduce=0,will_increase=0;
	for(int r = 0;r < numResources ; r++)
	{
		current_utilization = machineUtilized.get(initialAssignment.get(i)).get(r);
		will_reduce = processRequirements.get(i).get(r);
		machineUtilized.get(initialAssignment.get(i)).set(r,current_utilization-will_reduce);

		
		current_utilization = machineUtilized.get(j).get(r);
		will_increase = will_reduce;
		machineUtilized.get(j).set(r,current_utilization+will_increase);
	}
}



public static void findLocalBest(String outputfile)
{

	Vector<Integer> v = new Vector<Integer>();
	Vector<Integer> u = new Vector<Integer>();
	Vector<Integer> emptyAssignment = new Vector<Integer>();
	int best_neighbor_process = -1;
	int best_neighbor_machine = -1;
	int i=0,current_utilization=0,softcapacity=0,will_reduce = 0, will_increase = 0;
	int j=0;
	int firstMachine = 0,currentMachine = 0, targetMachine = 0;
	

	for(int p=0;p<numProcesses;p++)
	{
		neighborAssignment.add(-1);
	}

	current_best = calculateCost(initialAssignment,emptyAssignment); //this is important. the machineUtilized vector will be initialized here
	temp_best = current_best; //current_best is transfered to temp_best. because temp_best will hold the cost of current solution whereas current_best will hold the cost of the best feasble neighbor of the current soltuion

	for(int k=0;k>-1;k++) // this loop iterates for each solution we consider is feasible and better. Starts with the given initial assignment
	{
		    System.out.println("Going to search steepest descent neighbor from :"+ k +" th solution");
		    best_neighbor_process = -1;
		    best_neighbor_machine = -1;
		    for(i=0;i<numProcesses;i++) // this loops iterates over each process of a feasible solution.
		    {
			for(j=0;j<numMachines;j++) // this loops iterates over all machines to check if a process of a solution can be assigned there
			{
			    if(initialAssignment.get(i) != j) //this if condition ensures that it dont do anything for a process for the machine which it is already assigned to
			    {
				if(mcconChecking_mini(i,j) == true) //mccon
				{					
					if(ssconChecking_mini(i,j) == true) //sscon
					{
						if(scconChecking_mini(i,j) == true) //sccon
						{
							cost = temp_best;
							for(int r = 0;r < numResources ; r++) // if the code here is reached, that means the assignment of process i to machine j is feasible. now this loop iterates over all the resources of the machine to which process i is currently assigned and and of machine j to which i is going to be assigned and calculate the cost. then check if the new assignment is better than the current assignment.
							{	
								//checking the machine to which i is currently assigned	to calculate the cost of the new assignment		
								current_utilization = machineUtilized.get(initialAssignment.get(i)).get(r);
								will_reduce = processRequirements.get(i).get(r);
								softcapacity = softMachineCapacities.get(initialAssignment.get(i)).get(r);

								if(current_utilization > softcapacity)
								{
									if((current_utilization - will_reduce) > softcapacity)
									{
										cost = cost - will_reduce;
									}
									else
									{
										cost = cost - (current_utilization - softcapacity);
									}
								}

								//checking machine j, to which process i is going to be assigned to calculate the cost of the new assignment
								current_utilization = machineUtilized.get(j).get(r);
								will_increase = will_reduce;
								softcapacity = softMachineCapacities.get(j).get(r);

								if(current_utilization > softcapacity)
								{
									cost = cost + will_increase;
								}
								else
								{
									if((current_utilization + will_increase) > softcapacity)
									{
										cost = cost + ((current_utilization + will_increase) - softcapacity);
									}

								}
								
							}

								//adding process moving cost
								firstMachine = firstAssignment.get(i);
								currentMachine = initialAssignment.get(i);
								targetMachine = j;
								
								if(firstMachine == currentMachine && targetMachine != firstMachine)
								{
									cost = cost + processMovingCosts.get(i); //adding the process moving cost
								}
								else if(firstMachine != currentMachine && targetMachine == firstMachine)
								{
									
									cost = cost - processMovingCosts.get(i); //deducting the process moving cost
								}
								else if(firstMachine != currentMachine && targetMachine != firstMachine)
								{

								}

							if(cost < current_best) //checking if the new assignment is better 
							{
								best_neighbor_process = i; //recording which process needs to be re-assigned
								best_neighbor_machine = j; //recording to which machine it needs to be re-assigned
								current_best = cost;
							}
						}
					}
				}
			
    
			     }
			
			}

			 
		    }
			    

		    if(best_neighbor_process == -1 && best_neighbor_machine == -1)
		    {
			break; //if the code reaches here that means it has reached a local minima
		    }
		    else
		    {	//if the code reaches here that means, we have got a feasible and better soltuion
			updateMachineUtilization(best_neighbor_process,best_neighbor_machine); //update the machine utilization vector
			updateServiceLocations(best_neighbor_process,best_neighbor_machine);
			initialAssignment.set(best_neighbor_process,best_neighbor_machine); //update the initialAssignment to the new feasible and better soltuion and this new one becomes initialAssignment
			System.out.println("I am changing the initial assignment: best_neighbor_process: "+best_neighbor_process+" best_neighbor_machine :"+best_neighbor_machine+" Better cost: "+current_best);
			writeoutput(outputfile);
			temp_best = current_best; //temp_best is updated with the cost of this new initalAssignment. this temp_best is used to calculate the cost of a neighbor from this initialAssignment that it just has found
		    }
	}


}


    // main function
    public static void main(String args[]) {
	long startTime = System.nanoTime();
	int x=0,y=0;
			
	if(args.length != 3) {
	    System.out.println("ERROR: usage: ");
	    System.out.println("java ProcessAssignment <instance_file> <initial_solution_file> <output_file>");
	} else {
	    readInstanceFile(args[0]);
	    dumpProblem();
	    readInitialAssignment(args[1]);
	    System.out.println("InitialAssignment: (format: process -> machine)");
	    dumpAssignment(initialAssignment);
	    String outputfile = new String(args[2]);



//keeping backup of the given initialAssignment. Through out the code the vector firstAssignment will hold the given initialAssignment and each time a better assignment is found using 1-opt steepest descent, it will be kept in the vector initialAssignment
	    	for(int p=0;p<numProcesses;p++){
			x = initialAssignment.get(p);		
			firstAssignment.add(x);
		}


		//the following for loop initializes machineUtilized vector of machines. where each machine is also a vector of resources. this machineUtilized vector is used to calculate the cost and feasibility of an initalAssignment for which this machine
		for(int m=0;m<numMachines;m++){
			machineUtilized.add(new Vector< Integer >());
			for(int r=0;r<2*numResources;r++) {
			machineUtilized.get(m).add(0);
			}
		}


		for(int s = 0;s<numServices;s++)
		{
		serviceLocations.add(new Vector<Integer>());
			for(int l=0;l<numLocations;l++)
			{
				serviceLocations.get(s).add(0);
			}
	    	}
	    
	    	int current_count = 0;
	    	for(int p=0;p<numProcesses;p++)
	    	{
			current_count = serviceLocations.get(processServices.get(p)).get(machineLocations.get(initialAssignment.get(p)));
			serviceLocations.get(processServices.get(p)).set(machineLocations.get(initialAssignment.get(p)),current_count+1);
	    	}

	    findLocalBest(outputfile);	

	    if(mcconChecking(initialAssignment) == false) 
		System.out.println("The assignment is not feasible: mccon violated");
	    else if (ssconChecking(initialAssignment) == false )
		System.out.println("The assignment is not feasible: sscon violated");
	    else if (scconChecking(initialAssignment) == false)
		System.out.println("The assignment is not feasible: sccon violated");
	    else
		System.out.println("The assignment is feasible" );

	    
	}

	long endTime = System.nanoTime();
	System.out.println("Time spent "+(endTime - startTime)/1000000000 + " seconds");
    }




//updates ServiceLocations when process i is assined to machine j
public static void updateServiceLocations(int i,int j)
{

	int service = processServices.get(i);
	int location = machineLocations.get(initialAssignment.get(i));
	int location_count = serviceLocations.get(service).get(location);
	serviceLocations.get(service).set(location,location_count-1);
	location = machineLocations.get(j);
	location_count = serviceLocations.get(service).get(location);
	serviceLocations.get(service).set(location,location_count+1);



}


//write the the steepest descent neighbour in the outputfile which was provided as the third command line arguement
	public static void writeoutput(String outputfile)
	{
		//System.out.println("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		try {
 
			//String content = "";
			StringBuilder content = new StringBuilder();

			for(int p = 0;p<numProcesses;p++)
			{
				if(p != 0)				
				content.append(" ").append(initialAssignment.get(p));	
				else
				content.append(initialAssignment.get(p));
				
				
			}
			content.append("\n");

			//System.out.println("+++++++" + content.toString());
 
			File file = new File(outputfile);
 
			// if file doesnt exists, then create it
			if (!file.exists()) {
				file.createNewFile();
			}
 
			FileWriter fw = new FileWriter(file.getAbsoluteFile(),true);
			BufferedWriter bw = new BufferedWriter(fw);
			bw.write(content.toString());
			bw.close();
 
			//System.out.println("Done");
 
		} catch (IOException e) {
			e.printStackTrace();
		}

	}




}
