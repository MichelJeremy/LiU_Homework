package tddc17;



import aima.core.environment.liuvacuum.*;
import aima.core.agent.Action;
import aima.core.agent.AgentProgram;
import aima.core.agent.Percept;
import aima.core.agent.impl.*;

import java.util.ArrayList;
import java.util.Random;

class MyAgentState
{
	public int[][] world = new int[30][30];
	public int initialized = 0;
	final int UNKNOWN 	= 0;
	final int WALL 		= 1;
	final int CLEAR 	= 2;
	final int DIRT		= 3;
	final int HOME		= 4;
	final int ACTION_NONE 			= 0;
	final int ACTION_MOVE_FORWARD 	= 1;
	final int ACTION_TURN_RIGHT 	= 2;
	final int ACTION_TURN_LEFT 		= 3;
	final int ACTION_SUCK	 		= 4;
	
	public int agent_x_position = 1;
	public int agent_y_position = 1;
	public int agent_last_action = ACTION_NONE;
	
	public static final int NORTH = 0;
	public static final int EAST = 1;
	public static final int SOUTH = 2;
	public static final int WEST = 3;
	public int agent_direction = EAST;
	
	// initialize state with unknown everywhere except home position
	MyAgentState() {
		for (int i=0; i < world.length; i++) {
			for (int j=0; j < world[i].length ; j++) {
				world[i][j] = UNKNOWN;
			}
		world[1][1] = HOME;
		agent_last_action = ACTION_NONE;
		}
	}
	
	
	// Based on the last action and the received percept updates the x & y agent position
	public void updatePosition(DynamicPercept p) {
		Boolean bump = (Boolean)p.getAttribute("bump");

		if (agent_last_action==ACTION_MOVE_FORWARD && !bump)
	    {
			switch (agent_direction) {
			case MyAgentState.NORTH:
				agent_y_position--;
				break;
			case MyAgentState.EAST:
				agent_x_position++;
				break;
			case MyAgentState.SOUTH:
				agent_y_position++;
				break;
			case MyAgentState.WEST:
				agent_x_position--;
				break;
			}
	    }
		
	}
	
	public void updateWorld(int x_position, int y_position, int info) {
		world[x_position][y_position] = info;
	}
	
	public void printWorldDebug()
	{
		for (int i=0; i < world.length; i++)
		{
			for (int j=0; j < world[i].length ; j++)
			{
				if (world[j][i]==UNKNOWN)
					System.out.print(" ? ");
				if (world[j][i]==WALL)
					System.out.print(" # ");
				if (world[j][i]==CLEAR)
					System.out.print(" . ");
				if (world[j][i]==DIRT)
					System.out.print(" D ");
				if (world[j][i]==HOME)
					System.out.print(" H ");
			}
			System.out.println("");
		}
	}
}

class MyAgentProgram implements AgentProgram {

	private int initnialRandomActions = 10;
	private Random random_generator = new Random();
	
	// Here you can define your variables!
	public int iterationCounter = 100;
	public MyAgentState state = new MyAgentState();
	public int northMode = 1; // mode whose main objective is to go to the top right
	public int southMode = 0; // mode whose main objective is to go bottom right. if right-er position is discovered, goes back into north mode. (this is to avoid obstacles)
	public int initPosition = 0; // 1 as long as it is directing itself toward north
	public int best_x_achieved;
	public Boolean north_blocked = false;
	
	// moves the Agent to a random start position
	// uses percepts to update the Agent position - only the position, other percepts are ignored
	// returns a random action
	private Action moveToRandomStartPosition(DynamicPercept percept) {
		int action = random_generator.nextInt(6);
		initnialRandomActions--;
		state.updatePosition(percept);
		if(action==0) {
		    state.agent_direction = ((state.agent_direction-1) % 4);
		    if (state.agent_direction<0) 
		    	state.agent_direction +=4;
		    state.agent_last_action = state.ACTION_TURN_LEFT;
			return LIUVacuumEnvironment.ACTION_TURN_LEFT;
		} else if (action==1) {
			state.agent_direction = ((state.agent_direction+1) % 4);
		    state.agent_last_action = state.ACTION_TURN_RIGHT;
		    return LIUVacuumEnvironment.ACTION_TURN_RIGHT;
		} 
		state.agent_last_action=state.ACTION_MOVE_FORWARD;
		return LIUVacuumEnvironment.ACTION_MOVE_FORWARD;
	}
	
	
	@Override
	public Action execute(Percept percept) {
		
		// DO NOT REMOVE this if condition!!!
    	if (initnialRandomActions>0) {
    		return moveToRandomStartPosition((DynamicPercept) percept);
    	} else if (initnialRandomActions==0) {
    		// process percept for the last step of the initial random actions
    		initnialRandomActions--;
    		state.updatePosition((DynamicPercept) percept);
			System.out.println("Processing percepts after the last execution of moveToRandomStartPosition()");
			state.agent_last_action=state.ACTION_SUCK;
	    	return LIUVacuumEnvironment.ACTION_SUCK;
    	}
		
    	// This example agent program will update the internal agent state while only moving forward.
    	// START HERE - code below should be modified!
    	    	
    	System.out.println("x=" + state.agent_x_position);
    	System.out.println("y=" + state.agent_y_position);
    	System.out.println("dir=" + state.agent_direction);
    	
		
	    iterationCounter--;
	    
	    if (iterationCounter==0)
	    	return NoOpAction.NO_OP;

	    // first, get where we landed and update
	    DynamicPercept p = (DynamicPercept) percept;
	    Boolean bump = (Boolean)p.getAttribute("bump");
	    Boolean dirt = (Boolean)p.getAttribute("dirt");
	    Boolean home = (Boolean)p.getAttribute("home");
	    System.out.println("percept: " + p);
	    
	    state.updatePosition((DynamicPercept) percept);
	    
	    // assuming precept
	    // first, go into a side: north preferably because we do not know the closest side. to do so, start orienting north
	    if (initPosition == 0 && state.agent_direction != 0) {
		    if (state.agent_direction == 1) {
				state.agent_direction = ((state.agent_direction-1) % 4);
				if (state.agent_direction<0) state.agent_direction +=4;
		    	return LIUVacuumEnvironment.ACTION_TURN_LEFT;
		    } else if (state.agent_direction == 2) {
					state.agent_direction = ((state.agent_direction+1) % 4);
					if (state.agent_direction>3) state.agent_direction -=4;
		    	return LIUVacuumEnvironment.ACTION_TURN_RIGHT;
		    } else if (state.agent_direction == 3) {
				state.agent_direction = ((state.agent_direction+1) % 4);
				if (state.agent_direction>3) state.agent_direction -=4;
		    	initPosition = 1;
		    	best_x_achieved = state.agent_x_position; // current best x
		    	return LIUVacuumEnvironment.ACTION_TURN_RIGHT;
		    }
	    }
	    
	    if (initPosition == 1) {
	    	if (northMode == 1) {
			    if (bump) {
			    	switch (state.agent_direction) {
						case MyAgentState.NORTH:
							north_blocked = true;
							state.updateWorld(state.agent_x_position,state.agent_y_position-1,state.WALL);
		    				state.agent_last_action=state.ACTION_TURN_RIGHT;
		    				state.agent_direction = ((state.agent_direction+1) % 4);
		    			    if (state.agent_direction<3) state.agent_direction -=4;
		    				return LIUVacuumEnvironment.ACTION_TURN_RIGHT;
		    				
		    			case MyAgentState.EAST:
							state.updateWorld(state.agent_x_position+1,state.agent_y_position,state.WALL);
							northMode = 0;
							southMode = 1;
							state.agent_direction = ((state.agent_direction-1) % 4);
							if (state.agent_direction<0) state.agent_direction +=4;
		    				return LIUVacuumEnvironment.ACTION_TURN_LEFT;
						//southmode possibilities
						//case MyAgentState.SOUTH:
						//	state.updateWorld(state.agent_x_position,state.agent_y_position+1,state.WALL);
						//	break;
						//case MyAgentState.WEST:
						//	state.updateWorld(state.agent_x_position-1,state.agent_y_position,state.WALL);
					}
			    } else if (dirt) { //if we stop on dirt, update it on the world map an suck it
			    	state.updateWorld(state.agent_x_position,state.agent_y_position,state.CLEAR);
	    			System.out.println("DIRT -> choosing SUCK action!");
			    	state.agent_last_action=state.ACTION_SUCK;
			    	return LIUVacuumEnvironment.ACTION_SUCK;
			    } else if (home) {
    				state.updateWorld(state.agent_x_position,state.agent_y_position,state.HOME);
    				switch (state.agent_direction) {
    					case MyAgentState.NORTH:
    						state.agent_last_action=state.ACTION_MOVE_FORWARD;
    						return LIUVacuumEnvironment.ACTION_MOVE_FORWARD;
    					case MyAgentState.EAST:
    						if (!north_blocked) {
    							state.agent_last_action=state.ACTION_MOVE_FORWARD;
        						return LIUVacuumEnvironment.ACTION_MOVE_FORWARD;
    						} else {
    							state.agent_last_action=state.ACTION_TURN_LEFT;
    							state.agent_direction = ((state.agent_direction-1) % 4);
    							if (state.agent_direction<0) state.agent_direction +=4;
    							return LIUVacuumEnvironment.ACTION_TURN_LEFT;
    						}
    				}
			    } else { // else, since there is neither, it must be a clear block
			    	state.updateWorld(state.agent_x_position,state.agent_y_position,state.CLEAR);
			    	switch(state.agent_direction) {
					    case MyAgentState.NORTH:
							state.agent_last_action=state.ACTION_MOVE_FORWARD;
							return LIUVacuumEnvironment.ACTION_MOVE_FORWARD;
						case MyAgentState.EAST:
							if (!north_blocked) {
								state.agent_last_action=state.ACTION_MOVE_FORWARD;
								return LIUVacuumEnvironment.ACTION_MOVE_FORWARD;
							} else {
								state.agent_last_action=state.ACTION_TURN_LEFT;
								state.agent_direction = ((state.agent_direction-1) % 4);
								if (state.agent_direction<0) state.agent_direction +=4;
								return LIUVacuumEnvironment.ACTION_TURN_LEFT;
							}
					    }
			    state.printWorldDebug(); // print world state
			    }
	    	}
	    }
	return NoOpAction.NO_OP;
	}
}



public class MyVacuumAgent extends AbstractAgent {
    public MyVacuumAgent() {
    	super(new MyAgentProgram());
	}
}
