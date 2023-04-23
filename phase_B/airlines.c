/***************************************************
*                                                 *
* file:        airlines.c                         *
* Implementation of airlines.h                    *
*                                                 *
* @Author:     Apostolos Karampelas               *
* @Version:    23-04-2020                         *
* @email:      apskar@csd.uoc.gr                  *
*                                                 *
***************************************************/

#include "airlines.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>

_airplane * new_airplane(int pid, int dest, int depart_time);
_airline * new_airline(int aid);
_flight* new_flight(int pId, int depart_time);
_airline * find_airline(int aid);
int insert_flight_at_dest(int pid, int dest, int depart_time);
void _print_airplanes(_airplane *pl, int aid, int f);
void print_airplanes(_airline *comp, int dest);
_airplane *get_successor(_airplane *root, _airplane *node);
_flight *find_parent_flight_at_dest(_flight *fl, _flight *parent_fl, int pid);
int bst_to_array(_airplane *pl, _airplane *pl_array[], int index);
int bst_count(_airplane *pl);
_airplane *array_to_bst(_airplane *pl_array[], int start, int end);
int pq_full_sum_nodes(int height);
int pq_height(int num_elements);
void _print_clients(_client *cl);
void q_enqueue(_client *cl);
_client *q_dequeue();
struct client *q_front();
int q_isempty();

_airline * new_airline(int aid) {
	_airline * node;
	node = (_airline*)malloc(sizeof(_airline));
	assert(node != NULL);
	node->aid = aid;
	node->next = NULL;
	node->pR = NULL;
	node->prev = NULL;
	return node;
}
_airplane * new_airplane(int pid, int dest, int depart_time) {
	_airplane * node;
	node = (_airplane*)malloc(sizeof(_airplane));
	assert(node != NULL);
	node->pid = pid;
	node->dest = dest;
	node->depart_time = depart_time;
	node->rc = NULL;
	node->lc = NULL;
	return node;
}
_flight* new_flight(int pid, int depart_time) {
	_flight * node;
	node = (_flight*)malloc(sizeof(_flight));
	assert(node != NULL);
	node->pid = pid;
	node->depart_time = depart_time;
	node->thread_status = 3;
	node->lc = NULL;
	node->rc = NULL;
	return node;
}

_client* _new_client(int cid, int miles) {
	_client *node;
	node = (_client*)malloc(sizeof(_client));
	if (node == NULL) {
		return NULL;
	}
	node->cid = cid;
	node->miles = miles;
	node->lc = NULL;
	node->mc = NULL;
	node->p = NULL;
	node->rc = NULL;
	return node;
}


_airline * find_airline(int aid) {
	_airline *curr = airlines->next;
	assert(airlines != NULL);
	while (curr != airlines)
	{
		if (curr->aid == aid) {
			return curr;
		}
		else if (curr->aid > aid) {
			break;
		}
		curr = curr->next;
	}
	return NULL;
}

int insert_flight_at_dest(int pid, int dest, int depart_time) {
	_flight *curr, *par, *newnode;
	assert(dest >= 0 && dest < MAX_DEST);
	curr = DESTINATIONS[dest];
	par = NULL;
	newnode = new_flight(pid, depart_time);
	if (newnode == NULL) {
		return 0;
	}

	if (curr == NULL) {
		DESTINATIONS[dest] = newnode;
		DESTINATIONS[dest]->thread_status = 3;
		return 1;
	}

	par = DESTINATIONS[dest];
	while (1) {
		if (par->depart_time > depart_time || (par->depart_time == depart_time && par->pid > pid)) {
			if (par->thread_status % 2 == 1) {
				break;
			}
			else {
				par = par->lc;
			}
		}
		else {
			if (par->thread_status < 2)
			{
				par = par->rc;
			}
			else {
				break;
			}
		}
	}
	if (par->depart_time > depart_time || (par->depart_time == depart_time && par->pid > pid)) {
		newnode->lc = par->lc;
		newnode->rc = par;
		par->lc = newnode;
		par->thread_status -= 1;
	}
	else {
		newnode->rc = par->rc;
		newnode->lc = par;
		par->rc = newnode;
		par->thread_status -= 2;
	}
	return 1;
}


void _print_airplanes(_airplane *pl, int aid, int f) {
	if (pl == NULL) {
		return;
	}
	_print_airplanes(pl->lc, aid, f);
	if (f == 0) {
		printf("<%d:%d>", pl->pid, aid);
	}
	else {
		printf("<%d>", pl->pid);
	}
	_print_airplanes(pl->rc, aid, f);

}

void _print_destinations(_flight *fl, _flight *parent_fl, int f, int dep_time) {
	if (fl == NULL || (parent_fl != NULL && parent_fl->thread_status == 3)
		|| (parent_fl != NULL && fl == parent_fl->lc && parent_fl->thread_status == 1)
		|| (parent_fl != NULL && fl == parent_fl->rc && parent_fl->thread_status == 2)
		) {
		return;
	}
	_print_destinations(fl->lc, fl, f, dep_time);
	if (f == 0) {
		printf("<%d:%d>", fl->pid, fl->depart_time);
	}
	else if (f == 1 && fl->depart_time > dep_time) {
		printf("<%d:%d>", fl->pid, fl->depart_time);
	}
	_print_destinations(fl->rc, fl, f, dep_time);
}

void print_airplanes(_airline *comp, int dest) {
	printf("\nAirline <%d> = ", comp->aid);
	_print_airplanes(comp->pR, comp->aid, 0);
	if (comp->pR != NULL)
		printf("\nDestination <%d> = ", dest);
	_print_destinations(DESTINATIONS[dest], NULL, 0, 0);
	printf("\n");

}


void delete_airplane(_airline *comp, _airplane *pl, int pid) {
	int _pid;
	int _dest;
	int _depart_time;
	_airplane *temp, *curr, *prev;
	if (pl == NULL) {
		return;
	}
	prev = NULL;
	curr = pl;
	while (curr != NULL) {
		if (curr->pid > pid) {
			prev = curr;
			curr = curr->lc;
		}
		else if (curr->pid < pid) {
			prev = curr;
			curr = curr->rc;
		}
		else {
			break;
		}
	}

	assert(curr != NULL);
	;	if (curr == NULL) {
		return;
	}
	else {
		if (curr->lc == NULL && curr->rc == NULL) {
			if (curr == comp->pR) {
				free(comp->pR);
				comp->pR = NULL;
			}
			else if (curr == prev->lc) {
				prev->lc = NULL;
				free(curr);
			}
			else {
				prev->rc = NULL;
				free(curr);
			}
		}
		else if (curr->lc != NULL && curr->rc == NULL) {
			temp = curr->lc;
			curr->pid = curr->lc->pid;
			curr->depart_time = curr->lc->depart_time;
			curr->dest = curr->lc->dest;
			curr->rc = curr->lc->rc;
			curr->lc = curr->lc->lc;
			free(temp);
		}
		else if (curr->lc == NULL && curr->rc != NULL) {
			temp = curr->rc;
			curr->pid = curr->rc->pid;
			curr->depart_time = curr->rc->depart_time;
			curr->dest = curr->rc->dest;
			curr->lc = curr->rc->lc;
			curr->rc = curr->rc->rc;
			free(temp);
		}
		else if (curr->lc != NULL && curr->rc != NULL) {
			_airplane *succ = get_successor(pl, curr);
			assert(succ != NULL);
			_pid = succ->pid;
			_dest = succ->dest;
			_depart_time = succ->depart_time;
			delete_airplane(comp, pl, succ->pid);
			curr->pid = _pid;
			curr->dest = _dest;
			curr->depart_time = _depart_time;
		}
	}
}

_airplane *get_successor(_airplane *root, _airplane *node) {
	_airplane *curr, *prev, *succ;
	if (node->rc != NULL) {
		curr = node->rc;
		prev = curr;
		while (curr->lc != NULL) {
			prev = curr;
			curr = curr->lc;
		}
		return prev;
	}

	while (root != NULL) {
		if (node->pid < root->pid) {
			succ = root;
			root = root->lc;
		}
		else if (node->pid > root->pid) {
			root = root->rc;
		}
		else {
			break;
		}
	}
	return succ;
}

int delete_flight_at_dest(int pid, int dest) {
	_flight *fl, *parent_fl, *temp;
	parent_fl = NULL;
	assert(dest >= 0 && dest < MAX_DEST);
	fl = DESTINATIONS[dest];
	if (fl == NULL) {
		return 0;
	}
	if (fl->pid != pid) {
		parent_fl = find_parent_flight_at_dest(fl, NULL, pid);
		if (parent_fl == NULL) {
			return 0;
		}

		assert(parent_fl != NULL);

		if (parent_fl->thread_status == 1) {
			fl = parent_fl->rc;
		}
		else if (parent_fl->thread_status == 2) {
			fl = parent_fl->lc;
		}
		else if (parent_fl->thread_status == 0) {
			if (parent_fl->lc->pid == pid) {
				fl = parent_fl->lc;
			}
			else {
				fl = parent_fl->rc;
			}
		}
		else {
			/*  We should never arrive at this point. If so, we throw an error
			i.e. assert(1 == 0)
			*/
			assert(1 == 0);
		}
	}
	else {

	}
	assert(fl != NULL);

	if (fl->thread_status == 3) { /*Flight node is leaf*/
		if (fl == DESTINATIONS[dest]) {
			free(DESTINATIONS[dest]);
			DESTINATIONS[dest] = NULL;
		}
		else if (parent_fl->thread_status == 2) {
			parent_fl->thread_status = 3;
			parent_fl->rc = fl->lc;
			free(fl);
		}
		else if (parent_fl->thread_status == 1) {
			parent_fl->thread_status = 3;
			parent_fl->lc = fl->rc;
			free(fl);
		}
		else if (parent_fl->thread_status == 0) {
			if (fl == parent_fl->rc) {
				parent_fl->rc = fl->lc;
				parent_fl->thread_status = 2;
			}
			else {
				parent_fl->lc = fl->rc;
				parent_fl->thread_status = 1;
			}
			free(fl);
		}
		return 1;
	}
	else if (fl->thread_status == 1) {
		if (fl == DESTINATIONS[dest]) {
			temp = fl;
			DESTINATIONS[dest] = fl->rc;
			free(temp);
		}
		else {
			temp = fl;
			if (fl == parent_fl->lc) {
				parent_fl->lc = fl->rc;
			}
			else if (fl == parent_fl->rc) {
				parent_fl->rc = fl->rc;
			}
			free(temp);
		}
	}
	else if (fl->thread_status == 2) {
		if (fl == DESTINATIONS[dest]) {
			temp = fl;
			DESTINATIONS[dest] = fl->lc;
			free(temp);
		}
		else {
			temp = fl;
			if (fl == parent_fl->lc) {
				parent_fl->lc = fl->lc;
			}
			else if (fl == parent_fl->rc) {
				parent_fl->rc = fl->lc;
			}
			free(temp);
		}
	}
	else if (fl->thread_status == 0) {
		assert(fl->rc != NULL);
		return delete_flight_at_dest(fl->rc->pid, dest);
	}
	else {
		/*  We should never arrive at this point. If so, we throw an error
		i.e. assert(1 == 0)
		*/
		assert(1 == 0);
	}
	return 1;
}



_flight *find_parent_flight_at_dest(_flight *fl, _flight *parent_fl, int pid) {
	_flight *foundnode;
	if ((parent_fl != NULL && parent_fl->thread_status == 3)
		|| (parent_fl != NULL && fl == parent_fl->lc && parent_fl->thread_status == 1)
		|| (parent_fl != NULL && fl == parent_fl->rc && parent_fl->thread_status == 2)
		) {
		return NULL;
	}
	if (fl == NULL) {
		return NULL;
	}
	if (fl->pid == pid) {
		assert(parent_fl != NULL);
		return parent_fl;
	}
	else {
		foundnode = find_parent_flight_at_dest(fl->lc, fl, pid);
		if (foundnode == NULL) {
			foundnode = find_parent_flight_at_dest(fl->rc, fl, pid);
		}
		return foundnode;
	}
}

void _delete_airline(_airline *comp, _airplane *pl) {
	if (pl == NULL) {
		return;
	}
	_delete_airline(comp, pl->lc);
	_delete_airline(comp, pl->rc);
	delete_flight_at_dest(pl->pid, pl->dest);
	delete_airplane(comp, comp->pR, pl->pid);

}
void _subsidiary_airlines(_airline *comp1, _airline *comp2, _airplane *pl1, int dest) {
	if (pl1 == NULL || pl1->dest != dest) {
		return;
	}

	_subsidiary_airlines(comp1, comp2, pl1->lc, dest);
	_subsidiary_airlines(comp1, comp2, pl1->rc, dest);
	insert_airplane(pl1->pid, comp2->aid, dest, pl1->depart_time);
	delete_airplane(comp1, pl1, pl1->pid);
}

/**
* @brief Optional function to initialize data structures that
*        need initialization
*
* @return 1 on success
*         0 on failure
*/
int initialize() {
	airlines = new_airline(0);
	airlines->next = airlines;
	airlines->prev = airlines;
	PQ = (_priority_queue *)malloc(sizeof(_priority_queue));
	if (PQ == NULL) {
		return 0;
	}
	PQ->Q = NULL;
	PQ->num_elements = 0;
	FQ = (_fifo_queue *)malloc(sizeof(_fifo_queue));
	if (FQ == NULL) {
		return 0;
	}
	return 1;
}

/**
* @brief Register airline
*
* @param aid The airline's id
*
* @return 1 on success
*         0 on failure
*/
int register_airline(int aid) {
	_airline * curr, *curr1;
	_airline * new_node;
	assert(airlines != NULL);
	new_node = new_airline(aid);
	curr1 = airlines->next;

	while (curr1 != airlines) {
		if (curr1->aid > new_node->aid) {
			break;
		}
		curr1 = curr1->next;
	}

	new_node->next = curr1;
	new_node->prev = curr1->prev;
	curr1->prev->next = new_node;
	curr1->prev = new_node;

	printf("\nAirlines=");
	curr = airlines->next;
	while (curr != airlines) {
		printf("<%d>", curr->aid);
		if (curr->next != airlines) {
			printf(", ");
		}
		curr = curr->next;
	}
	printf("\n\nDONE\n\n");
	return 1;
}

/**
* @brief Insert new airplane
*
* @param pid         The airplanes id
* @param aid         The airlines id
* @param dest        Destination Id [0-9]
* @param depart_time Departure time
*
* @return 1 on success
*         0 on failure
*/
int insert_airplane(int pid, int aid, int dest, int depart_time) {
	_airline *comp;
	_airplane *curr, *new_node, *prev;
	comp = find_airline(aid);
	if (comp == NULL) {
		return 0;
	}
	curr = comp->pR;
	new_node = new_airplane(pid, dest, depart_time);
	if (new_node == NULL) {
		return 0;
	}
	if (comp->pR == NULL) {
		comp->pR = new_node;
		insert_flight_at_dest(pid, dest, depart_time);
		print_airplanes(comp, dest);
		printf("\n\nDONE\n");
		return 1;
	}
	while (curr != NULL)
	{
		prev = curr;
		if (new_node->pid > curr->pid) {
			curr = curr->rc;
		}
		else if (new_node->pid < curr->pid) {
			curr = curr->lc;
		}
	}

	if (new_node->pid > prev->pid) {
		prev->rc = new_node;
	}
	else if (new_node->pid < prev->pid) {
		prev->lc = new_node;
	}
	insert_flight_at_dest(pid, dest, depart_time);
	print_airplanes(comp, dest);
	printf("\n\nDONE\n");
	return 1;
}

/**
* @brief Cancel flight
*
* @param aid         The airlines id
* @param pid         The airplanes id
* @param dest        Destination Id [0-9]
*
* @return 1 on success
*         0 on failure
*/
int cancel_flight(int aid, int pid, int dest) {
	_airline *f;
	_airplane *curr;
	assert(dest >= 0 && dest < MAX_DEST);
	assert(airlines != NULL);
	f = find_airline(aid);
	if (f == NULL) {
		return 0;
	}
	curr = f->pR;
	if (curr == NULL) {
		return 0;
	}
	delete_airplane(f, curr, pid);
	delete_flight_at_dest(pid, dest);
	print_airplanes(f, dest);
	printf("\n\nDONE\n");
	return 1;
}

/**
* @brief Delete airline
*
* @param aid         The airlines id
*
* @return 1 on success
*         0 on failure
*/
int delete_airline(int aid) {
	_airline *temp;
	temp = find_airline(aid);

	if (temp == NULL) {
		return 0;
	}

	_delete_airline(temp, temp->pR);
	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;
	free(temp);
	print_airlines();
	print_destinations();
	return 1;
}




/**
* @brief Acquisition airline
*
* @param aid1         The first airline id
* @param aid2         The second airline id
*
* @return 1 on success
*         0 on failure
*/
int acquisition_airline(int aid1, int aid2) {
	/*int num_of_planes1, num_of_planes2;
	int cnt1, cnt2, cnt3;
	_airline *comp1, *comp2;
	_airline *temp;
	_airplane *planes1[num_of_planes1];
	_airplane *planes2[num_of_planes2];
	_airplane *merged_planes[num_of_planes1 + num_of_planes2];
	comp1 = find_airline(aid1);
	comp2 = find_airline(aid2);

	if (comp1 == NULL || comp2 == NULL) {
		return 0;
	}

	num_of_planes1 = bst_to_array(comp1->pR, planes1, 0);
	num_of_planes2 = bst_to_array(comp2->pR, planes2, 0);

	for (cnt3 = 0, cnt1 = 0, cnt2 = 0; cnt1 < num_of_planes1 && cnt2 < num_of_planes2; cnt3++) {
		if (planes1[cnt1]->pid < planes2[cnt2]->pid) {
			merged_planes[cnt3] = planes1[cnt1];
			cnt1++;
		}
		else if (planes2[cnt2]->pid < planes1[cnt1]->pid) {
			merged_planes[cnt3] = planes2[cnt2];
			cnt2++;
		}
		else {
			 Not a valid case 
			assert(1 == 0);
		}
	}

	if (cnt1 < num_of_planes1) {
		while (cnt3 < num_of_planes1 + num_of_planes2) {
			merged_planes[cnt3] = planes1[cnt1];
			cnt3++;
			cnt1++;
		}
	}
	else if (cnt2 < num_of_planes2) {
		while (cnt3 < num_of_planes1 + num_of_planes2) {
			merged_planes[cnt3] = planes2[cnt2];
			cnt3++;
			cnt2++;
		}
	}

	for (cnt3 = 0; cnt3 < num_of_planes1 + num_of_planes2; cnt3++) {
		printf("%d, ", merged_planes[cnt3]->pid);
	}
	printf("\n");

	comp2->pR = array_to_bst(merged_planes, 0, num_of_planes1 + num_of_planes2 - 1);
	comp1->pR = NULL;
	temp = comp1;

	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;
	free(temp);

	assert(comp2->pR);
	print_airlines();
	*/
	return 1;
}
_airplane *array_to_bst(_airplane *pl_array[], int start, int end) {
	int middle = (int)(end + start) / 2;
	if (start - end > 0) {
		return NULL;
	}

	pl_array[middle]->lc = NULL;
	pl_array[middle]->rc = NULL;
	assert(pl_array[middle] != NULL);
	pl_array[middle]->lc = array_to_bst(pl_array, start, middle - 1);
	pl_array[middle]->rc = array_to_bst(pl_array, middle + 1, end);

	return pl_array[middle];
}

int bst_count(_airplane *pl) {
	int counter = 1;
	if (pl == NULL) {
		return 0;
	}
	counter = counter + bst_count(pl->lc);
	counter = counter + bst_count(pl->rc);

	return counter;
}

int bst_to_array(_airplane *pl, _airplane **pl_array, int index) {
	if (pl == NULL) {
		return index;
	}


	index = bst_to_array(pl->lc, pl_array, index);
	pl_array[index] = pl;
	index++;
	index = bst_to_array(pl->rc, pl_array, index);
	return index;
}

/**
* @brief Subsidiary company
*
* @param aid1         The first airline id
* @param aid2         The second airline id
* @param dest         Destination
*
* @return 1 on success
*         0 on failure
*/
int subsidiary_airiline(int aid1, int aid2, int dest) {
	_airline *comp1, *comp2;

	comp1 = find_airline(aid1);
	comp2 = find_airline(aid2);

	if (comp1 == NULL || comp2 == NULL) {
		return 0;
	}
	_subsidiary_airlines(comp1, comp2, comp1->pR, dest);
	print_airlines();
	return 1;
}



/**
* @brief Find flight
*
* @param dest         Destination
* @param ts           Timestamp
*
* @return 1 on success
*         0 on failure
*/
int find_flight(int dest, int ts) {
	printf("\nDestination <%d> = ", dest);
	_print_destinations(DESTINATIONS[dest], NULL, 1, ts);
	printf("\n");
	printf("\n\nDONE\n");
	return 1;
}

/**
* @brief New client
*
* @param aid         The client's id
*
* @return 1 on success
*         0 on failure
*/
int new_client(int cid) {
		int node_counter = 0;
		int height;
		_client *curr_cl;
		_client *cl = _new_client(cid, 0);

		if (cl == NULL) {
			return 0;
		}
		assert(PQ);
		if (PQ->Q == NULL) {
			PQ->Q = cl;
			PQ->num_elements = 1;
			print_clients();

			return 1;
		}
		FQ->Q = malloc(PQ->num_elements * sizeof(struct client *));
		FQ->front = 0;
		FQ->length = 0;
		height = pq_height(PQ->num_elements);
		q_enqueue(PQ->Q);

		while (!q_isempty()) {
			curr_cl = q_dequeue();
			node_counter++;
			if (node_counter * 3 - 1 == PQ->num_elements + 1) {
				curr_cl->lc = cl;
			}
			else if (node_counter * 3 == PQ->num_elements + 1) {
				curr_cl->mc = cl;
			}
			else if (node_counter * 3 + 1 == PQ->num_elements + 1) {
				curr_cl->rc = cl;
			}

			if (curr_cl->lc != NULL) {
				q_enqueue(curr_cl->lc);
			}
			if (curr_cl->mc != NULL) {
				q_enqueue(curr_cl->mc);
			}
			if (curr_cl->rc != NULL) {
				q_enqueue(curr_cl->rc);
			}
		}
		PQ->num_elements++;
		free(FQ->Q);
		print_clients();
		printf("\n\n");
		printf("\n\nDONE\n");
		return 1;
}

/* Gives the total number of nodes if the PQ of height was full */
int pq_full_sum_nodes(int height) {
	int sum_nodes = 0;
	int curr_height = 0;

	while (curr_height <= height) {
		sum_nodes += pow(3, curr_height);
		curr_height++;
	}
	return sum_nodes;
}

int pq_height(int num_elements) {

	int height = 0;
	int sum_nodes = 0;

	while (sum_nodes < num_elements) {
		sum_nodes = sum_nodes + pow(3, height);
		height++;
	}
	return height - 1;
}

/**
* @brief Erase client
*
* @return 1 on success
*         0 on failure
*/
int erase_client() {
	return 1;
}

/**
* @brief Travel client
*
* @param cid          The client's id
* @param dest         Destination
*
* @return 1 on success
*         0 on failure
*/
int travel_client(int cid, int dest) {
	/*lient *curr = PQ->Q;

	_travel_client(curr, cid, ind);
	*/
	return 1;
}

/*
int _travel_client(_client *cl, int cid,int ind) {
ind++;
if (cl == NULL) {
return 0;
}
_travel_client(cl->lc,cid);
if (cl->cid == cid) {
cl->miles = ind * 100;
}
_travel_client(cl->rc, cid);

} */


/**
* @brief Print airlines
*
* @return 1 on success
*         0 on failure
*/
int print_airlines() {
	_airline * curr;
	if (airlines == NULL) {
		return 0;
	}
	curr = airlines->next;
	while (curr != airlines) {
		printf("Airline <%d> = ", curr->aid);
		_print_airplanes(curr->pR, curr->aid, 1);
		curr = curr->next;
		printf("\n");
	}
	printf("\nDONE\n");
	return 1;
}

/**
* @brief Print destinations
*
* @return 1 on success
*         0 on failure
*/
int print_destinations() {
	int i;
	for (i = 0; i < MAX_DEST; i++) {
		printf("\nDestination <%d> = ", i);
		_print_destinations(DESTINATIONS[i], NULL, 0, 0);
		printf("\n");
	}
	printf("\n\nDONE\n");
	return 1;
}

/**
* @brief Print clients
*
* @return 1 on success
*         0 on failure
*/
int print_clients() {
	_client *cl;
	assert(PQ != NULL);

	FQ->Q = malloc(PQ->num_elements * sizeof(struct client *));
	FQ->front = 0;
	FQ->length = 0;

	q_enqueue(PQ->Q);

	while (!q_isempty()) {
		cl = q_dequeue();
		printf("%d:%d,", cl->cid, cl->miles);
		if (cl->lc != NULL) {
			q_enqueue(cl->lc);
		}
		if (cl->mc != NULL) {
			q_enqueue(cl->mc);
		}
		if (cl->rc != NULL) {
			q_enqueue(cl->rc);
		}
	}

	return 1;
}

void q_enqueue(_client *cl) {
	assert(FQ != NULL);
	assert(cl != NULL);

	if (FQ->length == PQ->num_elements) {
		printf("Queue Error 1\n");
		exit(1);
	}
	else {
		FQ->length++;
		FQ->Q[(FQ->front + FQ->length - 1) % PQ->num_elements] = cl;
	}
}

_client *q_dequeue() {
	_client *cl = q_front(FQ);
	assert(FQ != NULL);

	if (q_isempty(FQ)) {
		printf("Queue Error 2");
		return NULL;
	}

	FQ->front = (FQ->front + 1) % PQ->num_elements;
	FQ->length--;
	return cl;
}

struct client *q_front() {
	if (q_isempty(FQ)) {
		printf("Queue Error 3");
		return NULL;
	}
	return FQ->Q[FQ->front];
}

int q_isempty() {
	assert(FQ != NULL);
	if (FQ->length == 0) {
		return 1;
	}
	return 0;
}

void _print_clients(_client *cl) {
	if (cl == NULL) {
		return;
	}

	printf("%d:%d,", cl->cid, cl->miles);
	_print_clients(cl->lc);
	_print_clients(cl->mc);
	_print_clients(cl->rc);
	printf("\n\nDONE\n");
}
/**
* @brief Free resources
*
* @return 1 on success
*         0 on failure
*/

int free_all(void) {
	return 1;
}
