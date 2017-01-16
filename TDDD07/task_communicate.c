/**
 * @file	task_communicate.c
 * @author  Eriks Zaharans
 * @date    31 Oct 2013
 *
 * @section DESCRIPTION
 *
 * Communicate Task.
 */

/* -- Includes -- */
/* system libraries */

/* project libraries */
#include "task.h"

 /**
 * Communication (receive and send data)
 */
void task_communicate(void)
{
	// Check if task is enabled
	if(g_task_communicate.enabled == s_TRUE)
	{
		// Loacal variables
		void *data; // Void pointer for data
		int data_type; // Data type

		// UDP Packet
		char udp_packet[g_config.udp_packet_size];
		int udp_packet_len;

		// Protocol
		protocol_t packet;

		//Start the new sequence
		int seq = 0; // Massi thing
		//In principle I want to send all the data in the buffer
		int last_id = g_list_send->count; // Massi thing

		// --------------------------------------------------
		//	LAB 2 starts here
		// --------------------------------------------------
		doublylinkedlist_t* prioritized_list = doublylinkedlist_init();
		doublylinkedlist_node_t *node_act;
		int position_act;
		int position;
		int victim_count = 0;
		int location_count = 0;
		int pheromone_count = 0;
		int cmd_count = 0;
		int stream_count = 0;

		/*
		 * First, make a new send list that takes the priority into account
		 * 	priority is (lowest number is higher priority):
		 * 1: Victim report, since it is the main goal
		 * 2: Location, because it is crucial to get the current Location for next
		 									go ahead. Not as important as victim reports though
		 * 3: Pheromone map, because old pheromone maps become redundant, and missing one
		 									message is not going to break the map
		 * 4: Stream data, because it is non crucial data
		 * 5: cmd data... is only useful for reception, not emission. Can be at the very end
		 *
		 * Then, send it starting by beginning
		 */

		 // g_list contains list of messages without priority
		 while(g_list_send->count != 0) {
			 //seq is useless if we don't send data
			 switch(g_list_send->first->data_type) {
				 case s_DATA_STRUCT_TYPE_ROBOT:
				 	// priority 2
					data = (void *)malloc(sizeof(robot_t));
					position = victim_count;
					position_act = 0;
					// insert after last victim count
					if (position == 0) {
						// if 0, insert at beginning (no location msg yet)
						doublylinkedlist_remove(g_list_send, g_list_send->first ,data, &data_type);
						doublylinkedlist_insert_beginning(prioritized_list, data, data_type); // exception
					}	else {
						node_act = prioritized_list->first;
						position_act++;
						while (position_act < position) {
							node_act = node_act->next;
							position_act++;
						}
						doublylinkedlist_remove(g_list_send, g_list_send->first, data, &data_type);
						doublylinkedlist_insert_after(prioritized_list, node_act, data, data_type);
					}
					location_count += 1;
					free(data);
					break;

				 case s_DATA_STRUCT_TYPE_VICTIM:
				 	// priority 1
				 	data = (void *)malloc(sizeof(victim_t));
					doublylinkedlist_remove(g_list_send, g_list_send->first ,data, &data_type);
				 	doublylinkedlist_insert_beginning(prioritized_list, data, data_type);
					victim_count += 1;
			 		free(data);
					break;

				 case s_DATA_STRUCT_TYPE_PHEROMONE:
				 // priority 3
				 data = (void *)malloc(sizeof(pheromone_map_sector_t));
				 if (pheromone_count < 8) {
					 position = victim_count + location_count;
					 position_act = 0;
					 if (position == 0) {
						 // if 0, insert at beginning (no lower priority msg yet)
						 doublylinkedlist_remove(g_list_send, g_list_send->first ,data, &data_type);
						 doublylinkedlist_insert_beginning(prioritized_list, data, data_type); // exception
					 } else {
						 node_act = prioritized_list->first;
						 position_act++;
						 while (position_act < position) {
							 node_act = node_act->next;
							 position_act++;
						 }
						 doublylinkedlist_remove(g_list_send, g_list_send->first, data, &data_type);
						 doublylinkedlist_insert_after(prioritized_list, node_act, data, data_type);
					 }
					 pheromone_count += 1;
				 } else {
					 doublylinkedlist_remove(g_list_send, g_list_send->first, data, &data_type);
				 }
				 free(data);
				 break;

				 case s_DATA_STRUCT_TYPE_CMD:
				 data = (void *)malloc(sizeof(command_t));
				 position = victim_count + location_count + pheromone_count + stream_count;
				 position_act = 0;
				 if (position == 0) {
					 doublylinkedlist_remove(g_list_send, g_list_send->first ,data, &data_type);
					 doublylinkedlist_insert_beginning(prioritized_list, data, data_type);
				 } else {
					 node_act = prioritized_list -> first;
					 position_act++;
					 while (position_act < position) {
						 node_act = node_act->next;
						 position_act++;
					 }
					 doublylinkedlist_remove(g_list_send, g_list_send->first, data, &data_type);
					 doublylinkedlist_insert_after(prioritized_list, node_act, data, data_type);
				 }
				 cmd_count += 1;
				 free(data);
				 break;

				 case s_DATA_STRUCT_TYPE_STREAM:
				 // priority 4
				 data = (void *)malloc(sizeof(stream_t));
				 if (stream_count < 7) {
					 position = victim_count + location_count + pheromone_count;
					 position_act = 0;
					 if (position == 0) {
						 doublylinkedlist_remove(g_list_send, g_list_send->first ,data, &data_type);
						 doublylinkedlist_insert_beginning(prioritized_list, data, data_type);
					 } else {
						 // go to the right position
					 	node_act = prioritized_list->first;
						position_act++;
						while (position_act < position) {
							node_act = node_act->next;
							position_act++;
						}
						doublylinkedlist_remove(g_list_send, g_list_send->first, data, &data_type);
						doublylinkedlist_insert_after(prioritized_list, node_act, data, data_type);
					}
					stream_count += 1;
				} else {
					doublylinkedlist_remove(g_list_send, g_list_send->first, data, &data_type);
				}
				free(data);
				break;
			 }
		 }

		 last_id = prioritized_list->count;
		/* --- Send Data --- */
		while(prioritized_list->count != 0)
		{
			seq++;
			// Allocate memory for data structure
			switch(prioritized_list->first->data_type)
			{
			// Robot pose
			case s_DATA_STRUCT_TYPE_ROBOT :
				data = (void *)malloc(sizeof(robot_t));
				break;
			// Victim information
			case s_DATA_STRUCT_TYPE_VICTIM :
				data = (void *)malloc(sizeof(victim_t));
				break;
			// Pheromone map
			case s_DATA_STRUCT_TYPE_PHEROMONE :
				data = (void *)malloc(sizeof(pheromone_map_sector_t));
				break;
			// Command (for future use)
			case s_DATA_STRUCT_TYPE_CMD :
				data = (void *)malloc(sizeof(command_t));
				break;
			case s_DATA_STRUCT_TYPE_STREAM :
				data = (void *)malloc(sizeof(stream_t));
				break;
			// Other
			default :
				// Do nothing
				continue;
				break;
			}

			// Get data from the list
			doublylinkedlist_remove(prioritized_list, prioritized_list->first, data, &data_type);

			// Encode data into UDP packet
			protocol_encode(udp_packet,
					&udp_packet_len,
					s_PROTOCOL_ADDR_BROADCAST,
					g_config.robot_id,
					g_config.robot_team,
					s_PROTOCOL_TYPE_DATA,
					seq,
					g_message_sequence_id,
					last_id,
					data_type,
					data);

			// Broadcast packet
			udp_broadcast(g_udps, udp_packet, udp_packet_len);

			// Free memory
			free(data);
		}



		/* --- Receive Data --- */
		// Receive packets, decode and forward to proper process

		// Receive UDP packet
		while(udp_receive(g_udps, udp_packet, &udp_packet_len) == s_OK)
		{
			// Decode packet
			//printf("%s\n",udp_packet);
			if(protocol_decode(&packet, udp_packet, udp_packet_len, g_config.robot_id, g_config.robot_team) == s_OK)
			{
				// Now decoding depends on the type of the packet
				switch(packet.type)
				{
				// ACK
				case s_PROTOCOL_TYPE_ACK :
					// Do nothing
					break;

				//Massi: go_ahead packet
				case s_PROTOCOL_TYPE_GO_AHEAD :
				{
					// Declare go ahead command
					command_t go_ahead;
					go_ahead.cmd = s_CMD_GO_AHEAD;
					// Redirect to mission by adding it to the queue
					queue_enqueue(g_queue_mission, &go_ahead, s_DATA_STRUCT_TYPE_CMD);

					// Debuging stuff
					printf("GO_AHEAD RECEIVED for robot %d team %d\n",packet.recv_id,packet.send_team);
					// Calculate time from packet (ms and s)
					int send_time_s = floor(packet.send_time / 1000);
					int send_time_ms = packet.send_time % 1000;
					int now = floor(((long long)timelib_unix_timestamp() % 60000) / 1000);
					printf("GO_AHEAD_TIME: %d (%d)\n",send_time_ms,now);

					break;
				}
				// Data
				case s_PROTOCOL_TYPE_DATA :
					// Continue depending on the data type
					switch(packet.data_type)
					{
					// Robot pose
					case s_DATA_STRUCT_TYPE_ROBOT :
						debug_printf("received robot\n");
						// Do nothing
						break;
					// Victim information
					case s_DATA_STRUCT_TYPE_VICTIM :
						debug_printf("received victim\n");
						// Redirect to mission by adding it to the queue
						queue_enqueue(g_queue_mission, packet.data, s_DATA_STRUCT_TYPE_VICTIM);
						break;
					// Pheromone map
					case s_DATA_STRUCT_TYPE_PHEROMONE :
						debug_printf("received pheromone\n");
						// Redirect to navigate by adding it to the queue
						queue_enqueue(g_queue_navigate, packet.data, s_DATA_STRUCT_TYPE_PHEROMONE);

						break;
					// Command
					case s_DATA_STRUCT_TYPE_CMD :
						debug_printf("received CMD\n");
						// Redirect to mission by adding it to the queue
						queue_enqueue(g_queue_mission, packet.data, s_DATA_STRUCT_TYPE_CMD);
						break;
					case s_DATA_STRUCT_TYPE_STREAM :
						debug_printf("received data stream item\n");
						break;
					// Other
					default :
						// Do nothing
						break;
					}
				// Other ?
				default:
					// Do nothing
					break;
				}

				// Free memory (only if data packet was received!)
				if(packet.type == s_PROTOCOL_TYPE_DATA)
					free(packet.data);
			}
		}

		// Increase msg sequance id
		g_message_sequence_id++;
	}
}
