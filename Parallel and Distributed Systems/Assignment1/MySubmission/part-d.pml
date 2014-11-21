/* Partial Promela model of an elevator. */

mtype = { call_0, call_1, call_2, call_3,
          go_0, go_1, go_2, go_3,
          open, close,
          up, down}

chan floor_buttons = [1] of { mtype };
chan elevator_buttons = [1] of { mtype };
chan commands = [0] of { mtype };

active proctype elevator() {
    do
        :: commands ? open -> printf("Elevator: opened doors.\n");
        :: commands ? close -> printf("Elevator: closed doors.\n");
        :: commands ? up -> printf("Elevator: moved up one floor.\n");
        :: commands ? down -> printf("Elevator: moved down one floor.\n");
    od
}

/* Simulates random pushing of call buttons. */
active proctype floor_button_input() {
    do
        :: floor_buttons ! call_0;
        :: floor_buttons ! call_1;
        :: floor_buttons ! call_2;
        :: floor_buttons ! call_3;
    od
}

/* Simulates random pushing of elevator buttons. */
active proctype elevator_button_input() {
    do
        :: elevator_buttons ! go_0;
        :: elevator_buttons ! go_1;
        :: elevator_buttons ! go_2;
        :: elevator_buttons ! go_3;
    od
}

active proctype controller() {
    int at = 0;
    int go_to;
    mtype input_message;
    bool closed = true;
    bool take_input_from_floors = true;

 
do ::(5==5) ->  


   if
   ::(take_input_from_floors==true) -> 
   				 floor_buttons ? input_message;
				 take_input_from_floors=false;
   ::(take_input_from_floors==false) -> 
   				 elevator_buttons ? input_message;
				 take_input_from_floors=true;
   fi;


   commands ! close;


   if
	:: (input_message == call_0) -> go_to = 0; 
	:: (input_message == call_1) -> go_to = 1; 
	:: (input_message == call_3) -> go_to = 3;
	:: (input_message == go_0) -> go_to = 0;
	:: (input_message == go_1) -> go_to = 1;
	:: (input_message == go_2) -> go_to = 2;
	:: (input_message == go_3) -> go_to = 3;
	:: (input_message == call_2) -> go_to = 2;
   fi;

 

	do
		::(at == go_to) -> 
		      	 	commands ! open;	
				break;
		::(at > go_to) ->
			        assert(closed == true); 
				commands ! down; 
				at--;
		:: (at < go_to ) -> 
		       	       	assert(closed == true);
				commands ! up; 
				at++;
	od;

od;

}
