/* Message types used by the protocol. Do not change. */
mtype = {data, hole};

chan in_chan[3] = [1] of { mtype, byte };
chan from_user[3] = [0] of { mtype, byte };
chan to_user[3] = [0] of { mtype, byte };

/* A process modelling the sending user. Do not change. */

proctype sending_user(byte id) {
    /* Send a message randomly to one of the nodes. */
    do
        :: from_user[id] ! data, 0
        :: from_user[id] ! data, 1
        :: from_user[id] ! data, 2
    od
}

/* A process modelling the receiving user. Do not change. */

proctype receiving_user(byte id) {
    byte message_to;
    mtype message_type;
    do
        :: to_user[id] ? message_type, message_to;
           atomic {
               assert((message_type == data) && (message_to == id));
               message_to = 0;
               message_type = 0;
           }
    od
}

inline consume_message(id, message_type,message_to)
{
	
	if
	:: (message_to == id) -> 
		atomic {
		to_user[id] ! message_type, message_to;
		in_chan[(id+1)%3] !  hole, 0;
		}
	printf("Node %d has sent a data message to to_user[%d] channel and pushed a hole message to in_chan[%d]\n", id, id,(id+1)%3);
	:: else -> in_chan[(id+1)%3] ! message_type, message_to;
		printf("Node %d has sent a data message to in_chan[%d]\n", id, (id+1)%3);
	fi;
}


/* A simple ring protocol in Promela */

proctype node(byte id) {

		do
			:: from_user[id] ? data, 0;
				do
				:: in_chan[id] ? hole, 0 -> consume_message(id, data, 0); break;
				:: in_chan[id] ? data, 0 -> consume_message(id, data, 0); break;
				:: in_chan[id] ? data, 1 -> consume_message(id, data, 1); break;
				:: in_chan[id] ? data, 2 -> consume_message(id, data, 2); break;			
				od;
			:: from_user[id] ? data, 1;
				do
				:: in_chan[id] ? hole, 0 -> consume_message(id, data, 1); break;
				:: in_chan[id] ? data, 0 -> consume_message(id, data, 0); break;
				:: in_chan[id] ? data, 1 -> consume_message(id, data, 1); break;
				:: in_chan[id] ? data, 2 -> consume_message(id, data, 2); break;			
				od;
			:: from_user[id] ? data, 2;
				do
				:: in_chan[id] ? hole, 0 -> consume_message(id, data, 2); break;
				:: in_chan[id] ? data, 0 -> consume_message(id, data, 0); break;
				:: in_chan[id] ? data, 1 -> consume_message(id, data, 1); break;
				:: in_chan[id] ? data, 2 -> consume_message(id, data, 2); break;
				od;

			:: in_chan[id] ? hole, 0 -> in_chan[(id+1)%3] ! hole, 0; 

			:: in_chan[id] ? data, 0 -> consume_message(id, data, 0); 
			:: in_chan[id] ? data, 1 -> consume_message(id, data, 1);
			:: in_chan[id] ? data, 2 -> consume_message(id, data, 2);

		od;
 


}

/* The init process for starting all other processes. */

init {
    int id;
    atomic {
        id = 0;
        do
            :: (id < 3) ->
                   run node(id);
                   run receiving_user(id);
                   run sending_user(id);
                   id++
            :: (id == 3) -> break
        od;

/* The following generates the initial holes in the */
/* ring in question 2.                              */
        
        /* Send 2 holes to the channels 0,1. */
        id = 0;
        do
            :: (id < 2) -> in_chan[id] ! hole, 0; id++
            :: (id == 2) -> break
        od

    }
}
