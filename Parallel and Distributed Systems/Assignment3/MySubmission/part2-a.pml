#define N 4 /* number of cores */
#define M 2 /* number of FPUs */
#define any M
byte fpusg[M];
/* Message types used by the protocol. Do not change. */
mtype = {req_fpu, grant_fpu, free_fpu};

/* Message channels used by the protocol. Do not change. */
chan from_core[N] = [1] of { mtype, byte };
chan to_core[N] = [1] of { mtype, byte };

/* A process modelling the FPU use of a core. 
   Do not change functionality, you may add something for part 2 of the assignment. */
proctype core(byte id) {
    byte my_fpu = M;
    do
        :: from_core[id] ! req_fpu, any;
           to_core[id] ? grant_fpu, my_fpu;
	
		fpusg[my_fpu]++;
		assert(fpusg[my_fpu]==1);

           atomic {
               printf("MSC: Core %d using FPU %d\n", id, my_fpu);
               from_core[id] ! free_fpu, my_fpu;
		fpusg[my_fpu]--;
               my_fpu = M
           }
    od
}

active proctype fpu_controller() {
	bit fpus[M];
	byte currentfpu = 0;
	byte outerchannelcounter=0;
	byte channelcounter=0;
	byte madefree = 0;
	
	printf("MSC: fpu controller has been called\n");
	
	do
		:: from_core[outerchannelcounter] ? req_fpu, any ->
			printf("MSC: Reading Request Message from From_Core %d Channel\n",outerchannelcounter);
			do
				:: (currentfpu < M) ->
					if			
					:: (fpus[currentfpu] == 0) -> 
						printf("MSC: Core %d is given with fpu %d	 grant direct\n",outerchannelcounter,currentfpu);
						to_core[outerchannelcounter] ! grant_fpu, currentfpu;
						fpus[currentfpu]=1;
						outerchannelcounter = (outerchannelcounter+1)%N;
						currentfpu=M+1;
					:: else -> currentfpu = currentfpu+1;
					fi;
				:: (currentfpu==M) ->
					do
					:: from_core[channelcounter] ? free_fpu, madefree ->
						printf("MSC: Core %d Freing FPU %d\n",channelcounter,madefree);
						printf("MSC: Core %d is given with fpu %d		grant after waiting\n",outerchannelcounter,madefree);
						fpus[madefree]=1;
						to_core[outerchannelcounter] ! grant_fpu, madefree;
						currentfpu=M+1;
						break;
					:: channelcounter = (channelcounter+1)%N;
					od;
				:: (currentfpu == M+1) -> currentfpu=0; break;	
			od

		:: from_core[outerchannelcounter] ? free_fpu, madefree ->
			fpus[madefree] = 0;
			printf("MSC: Core %d Freing FPU %d\n",outerchannelcounter,madefree);
			outerchannelcounter = (outerchannelcounter+1)%N; 
	od;
}

/* The init process for starting all other processes. */

init {
    int id;
    atomic {
        id = 0;
        do
            :: (id < N) -> run core(id); id++
            :: (id == N) -> break
        od;
        id = 0
    }
}
