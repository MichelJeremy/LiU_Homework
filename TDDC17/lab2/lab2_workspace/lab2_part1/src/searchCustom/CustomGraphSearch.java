package searchCustom;

import java.util.ArrayList;
import java.util.HashSet;

import searchShared.NodeQueue;
import searchShared.Problem;
import searchShared.SearchObject;
import searchShared.SearchNode;

import world.GridPos;

public class CustomGraphSearch implements SearchObject {

	private HashSet<SearchNode> explored;
	private NodeQueue frontier;
	protected ArrayList<SearchNode> path;
	private boolean insertFront;

	/**
	 * The constructor tells graph search whether it should insert nodes to front or back of the frontier 
	 */
    public CustomGraphSearch(boolean bInsertFront) {
		insertFront = bInsertFront;
    }

	/**
	 * Implements "graph search", which is the foundation of many search algorithms
	 */
	public ArrayList<SearchNode> search(Problem p) {
		// The frontier is a queue of expanded SearchNodes not processed yet
		frontier = new NodeQueue();
		/// The explored set is a set of nodes that have been processed 
		explored = new HashSet<SearchNode>();
		// The start state is given
		GridPos startState = (GridPos) p.getInitialState();
		// Initialize the frontier with the start state  
		frontier.addNodeToFront(new SearchNode(startState));

		// Path will be empty until we find the goal.
		path = new ArrayList<SearchNode>();
		
		SearchNode sn;
		GridPos node;
		
		while (true) { // loops infinitely until something is returned
			if (frontier.isEmpty()) return path; // return path, will be empty because there are no more path to be explored
			if (insertFront) {
				sn = frontier.peekAtBack(); // case insertFront true: choose node at the end, breadthFirst graph
				frontier.removeLast(); // remove it from frontier
			} else {
				sn = frontier.peekAtFront(); // case insertFront false: choose node at the beginning, depthFirst search
				frontier.removeFirst(); // remove it from frontier
			}
			node = sn.getState(); // cast peeked node into state to read it
			if (p.isGoalState(node)) return path = sn.getPathFromRoot(); // if node contains goal, return path
			else {
				explored.add(sn); // add node to explored list
				ArrayList<GridPos> childStates = p.getReachableStatesFrom(node); // get the nodes 
				for (int i = 0; i <= childStates.size()-1; i++) { // loops every element of the reachable states because they may be more than one resulting node (different paths)
					SearchNode childNode = new SearchNode(childStates.get(i), sn); // create a node out of a state with the parent node as added parameter
					if (((frontier.contains(childNode)) == false) && ((explored.contains(childNode)) == false)) {
						// if both are false, node is neither in explored nor frontier
						frontier.addNodeToFront(childNode); // add it to front, boolean insertFront will decide which side to pick itself
					}
				}
			}

		}
		
		
		/* Some hints:
		 * -Read early part of chapter 3 in the book!
		 * -You are free to change anything how you wish as long as the program runs, but some structure is given to help you.
		 * -You can Google for "javadoc <class>" if you are uncertain of what you can do with a particular Java type.
		 * 
		 * -SearchNodes are the nodes of the search tree and contains the relevant problem state, in this case x,y position (GridPos) of the agent 
		 * --You can create a new search node from a state by: SearchNode childNode = new SearchNode(childState, currentNode);
		 * --You can also extract the state by .getState() method
		 * --All search structures use search nodes, but the problem object only speaks in state, so you may need to convert between them 
		 * 
		 * -The frontier is a queue of search nodes, open this class to find out what you can do with it! 
		 * 
		 * -If you are unfamiliar with Java, the "HashSet<SearchNode>" used for the explored set means a set of SearchNode objects.
		 * --You can add nodes to the explored set, or check if it contains a node!
		 * 
		 * -To get the child states (adjacent grid positions that are not walls) of a particular search node, do: ArrayList<GridPos> childStates = p.getReachableStatesFrom(currentState);
		 * 
		 * -Depending on the addNodesToFront boolean variable, you may need to do something with the frontier... (see book)
		 * 
		 * -You can check if you have reached the goal with p.isGoalState(NodeState)
		 * 
		 *  When the goal is found, the path to be returned can be found by: path = node.getPathFromRoot();
		 */
		/* Note: Returning an empty path signals that no path exists */
	}

	/*
	 * Functions below are just getters used externally by the program 
	 */
	public ArrayList<SearchNode> getPath() {
		return path;
	}

	public ArrayList<SearchNode> getFrontierNodes() {
		return new ArrayList<SearchNode>(frontier.toList());
	}
	public ArrayList<SearchNode> getExploredNodes() {
		return new ArrayList<SearchNode>(explored);
	}
	public ArrayList<SearchNode> getAllExpandedNodes() {
		ArrayList<SearchNode> allNodes = new ArrayList<SearchNode>();
		allNodes.addAll(getFrontierNodes());
		allNodes.addAll(getExploredNodes());
		return allNodes;
	}

}
