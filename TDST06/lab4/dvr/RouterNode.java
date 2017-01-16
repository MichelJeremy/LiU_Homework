import javax.swing.*;

public class RouterNode {
  public static final boolean POISONED_REVERSE = false;
  private int myID;
  private GuiTextArea myGUI;
  private RouterSimulator sim;
  private int[] costs = new int[RouterSimulator.NUM_NODES];
  private int[] route = new int[RouterSimulator.NUM_NODES];
  private RouterPacket rpkt;
  private int updated = 0;
  private int neighCount = 0;
  private int neighIndex = 0;
  private int[] neighbours;
  private int[][] routingTable = new int[RouterSimulator.NUM_NODES][RouterSimulator.NUM_NODES];
  private int[] directLinks = new int[RouterSimulator.NUM_NODES];

  //--------------------------------------------------
  public RouterNode(int ID, RouterSimulator sim, int[] costs) {
    myID = ID;
    this.sim = sim;
    myGUI = new GuiTextArea("  Output window for Router #"+ ID + "  ");
    System.arraycopy(costs, 0, this.costs, 0, RouterSimulator.NUM_NODES);
    // initialize route values for printing
    for (int i = 0; i < route.length; i++) {
      route[i] = i;
    }
    /*save neighbours in a separate array for easier coding and understanding*/
    // run over the costs and find out my neighbours
    for (int node = 0; node < costs.length; node++) {
      if (((costs[node]) != 999) && ((costs[node]) != 0)) {
        neighCount+=1;
      }
    }



    neighbours = new int[neighCount]; // init array


    /* send update to neighbors */
    // send rpkt to all neighbors using sendUpdate
    // dangerous because object is not yet fully initialized but whatever
    for (int node = 0; node < costs.length; node++) {
      if (((costs[node]) != 999) && ((costs[node]) != 0)) {
        directLinks[node] = costs[node];
        System.out.print(directLinks[node] + " (" + myID + "), ");
        neighbours[neighIndex] = node;
        for (int node2 = 0; node2 < RouterSimulator.NUM_NODES; node2++) {
          routingTable[node][node2] = 999;
        }
        neighIndex+=1;
        rpkt = new RouterPacket(myID, node, costs);
        sendUpdate(rpkt);
      }
    }
  }

  //--------------------------------------------------
  // lowest on the layer
  public void recvUpdate(RouterPacket pkt) {
    for (int j = 0; j < pkt.mincost.length; j++) {
      routingTable[pkt.sourceid][j] = pkt.mincost[j];
    }
    // look at sourceid and cost, replace costs array if needed
    System.out.println("Update received from :" + pkt.sourceid);
    System.out.println("Update sent to :" + pkt.destid);
    System.out.print("Vector sent: ");
    for (int i = 0; i < pkt.mincost.length; i++) {
      System.out.print(pkt.mincost[i] + ", ");
    }
    System.out.print("\n");

    // for every costs in received update, compare to my DV table
    // and update if shorter path found
    // if shorter path found, send to neighbours according to bellman ford
    for (int node = 0; node < pkt.mincost.length; node++) {
      System.out.println("costs: " + costs[node] + ", mincosts: " + pkt.mincost[node]);
      // if a direct link becomes better, use it
      if ((costs[pkt.sourceid] + pkt.mincost[node]) < costs[node]) {
        routingTable[pkt.sourceid][node] = pkt.mincost[node];
        costs[node] = costs[pkt.sourceid] + pkt.mincost[node];
        route[node] = pkt.sourceid;
        updated = 1;
      }
    }

    // so, this seem to make the convergence succesfull
    // BUT there is an infinite loop
    // so ... works ?
    for (int node = 0; node < pkt.mincost.length; node++) {
      if ((directLinks[node] < pkt.mincost[node] + costs[pkt.sourceid]) && (node != myID) && (updated == 0)) {
        costs[node] = directLinks[node];
        route[node] = node; // DC
        updated = 1;
      }
    }


    // updates if more expensive but same as currently routing through
    for (int node = 0; node < pkt.mincost.length; node++) {
      if ((updated == 0) && (route[node] == pkt.sourceid)) {
        System.out.println("procs");
        if (pkt.mincost[node] + costs[pkt.sourceid] > costs[node]) {
          costs[node] = pkt.mincost[node] + costs[pkt.sourceid];
          routingTable[pkt.sourceid][node] = pkt.mincost[node];
          updated = 1;
        }
      }
    }




    if (updated == 1) {
      // send updated vector to neighbours
      System.out.println("DV for " + myID + " updated !");
      for (int neighbour = 0; neighbour < neighbours.length; neighbour++) {
        rpkt = new RouterPacket(myID, neighbours[neighbour], costs);
        sendUpdate(rpkt);
      }
      updated = 0;
    }

    System.out.print("\n\n");
  }


  //--------------------------------------------------
  // will trigger recvUpdate on other nodes (in router simulator)
  private void sendUpdate(RouterPacket pkt) {
    sim.toLayer2(pkt);
  }


  //--------------------------------------------------
  // value printing
  public void printDistanceTable() {
	  myGUI.println("Current table for " + myID +
			"  at time " + sim.getClocktime());

    myGUI.print("My neighbours: ");
    for (int i = 0; i < neighbours.length; i++) {
      myGUI.print(neighbours[i] + " ");
      if (i == neighbours.length - 1) myGUI.print("\n");
    }

    myGUI.println("Distance vector and routes: ");
    myGUI.print("DST        | ");
    for (int i = 0; i < costs.length; i++) {
        myGUI.print("\t" + i);
    }

    myGUI.print("\n-------------");
    for (int i = 0; i < costs.length; i++) {
      myGUI.print("--------------------");
    }

    myGUI.print("\nCOST       |");
    for (int i = 0; i < costs.length; i++) {
      myGUI.print("\t" + costs[i]);
    }

    myGUI.print("\nROUTE     |");
    for (int i = 0; i < costs.length; i++) {
      if (costs[i] == 999) {
        myGUI.print("\t-");
      } else {
        myGUI.print("\t" + route[i]);
      }
    }

    myGUI.print("\n\n");

    myGUI.println("Neighbor distance");
    myGUI.print("DST        |");
    for (int i = 0; i < RouterSimulator.NUM_NODES; i++) {
      myGUI.print("\t" + i);
    }

    myGUI.print("\n-------------");
    for (int i = 0; i < RouterSimulator.NUM_NODES; i++) {
      myGUI.print("--------------------");
    }

    for (int i = 0; i < neighbours.length; i++) {
      myGUI.print("\nFrom " + neighbours[i] + "     |");
      for (int j = 0; j < RouterSimulator.NUM_NODES; j++) {
        myGUI.print("\t" + routingTable[i][j]);
      }
    }

    myGUI.print("\n\n\n");
  }

  //--------------------------------------------------
  // will be called on sent packet on a node and will trigger sendUpdate
  public void updateLinkCost(int dest, int newcost) {
    int min = 999;

    if (POISONED_REVERSE == true) {
      // seems like it works, except for the infinite loop
      costs[dest] = newcost;
      routingTable[dest][myID] = newcost;
      costs[dest] = newcost;
      directLinks[dest] = newcost;
      for (int neighbour = 0; neighbour < neighbours.length; neighbour++) {
          if ((routingTable[neighbours[neighbour]][dest] + costs[neighbours[neighbour]]) < newcost) {
            if ((routingTable[neighbours[neighbour]][dest] + costs[neighbours[neighbour]]) < min) {
              min = routingTable[neighbours[neighbour]][dest] + costs[neighbours[neighbour]];
              route[dest] = neighbours[neighbour]; // update next hop
            }
          }
      }
      if (route[dest] != dest) {
        costs[dest] = 999; // INFINITY
      } else {
        costs[dest] = min;
      }
    } else {

      // look at my neighbours' costs to go to other nodes
      routingTable[dest][myID] = newcost;
      costs[dest] = newcost;
      directLinks[dest] = newcost;
      for (int neighbour = 0; neighbour < neighbours.length; neighbour++) {
          if ((routingTable[neighbours[neighbour]][dest] + costs[neighbours[neighbour]]) < newcost) {
            if ((routingTable[neighbours[neighbour]][dest] + costs[neighbours[neighbour]]) < min) {
              min = routingTable[neighbours[neighbour]][dest] + costs[neighbours[neighbour]];
              route[dest] = neighbours[neighbour]; // update next hop
            }
          }
      }

      // here, we have a new min cost
      costs[dest] = min;
      System.out.println(min);
    }
    for (int neighbour = 0; neighbour < neighbours.length; neighbour++) {
      System.out.println("Sending to " + neighbours[neighbour]);
      rpkt = new RouterPacket(myID, neighbours[neighbour], costs);
      sendUpdate(rpkt);
    }
  }

}
