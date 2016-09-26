/* Message types used by the protocol. Do not change. */
mtype = {data};

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

/* A simple ring protocol in Promela */

proctype node(byte id) {

byte message_to;
mtype message_type;

do
	:: from_user[id] ? message_type, message_to;
    		if
			:: (message_to == id) -> to_user[id]!message_type, message_to;
			:: else -> in_chan[(id+1)%3]!message_type, message_to;
		fi;

	:: in_chan[id] ? message_type, message_to;
		if
			:: (message_to == id) -> to_user[id] ! message_type, message_to;
			:: else -> in_chan[(id+1)%3] ! message_type, message_to;

		fi;
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

    }
}
