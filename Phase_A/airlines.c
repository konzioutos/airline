	/***************************************************
 *                                                 *
 * file: twitter.c                                 *
 *                                                 *
 * @Author  Iacovos G. Kolokasis                   *
 * @Version 25-02-2020                             *
 * @email   kolokasis@csd.uoc.gr                   *
 *                                                 *
 * @brief   Implementation of airlines.h            *
 *                                                 *
 ***************************************************
 */

#include "airlines.h"
#include <assert.h>

_airlines_l * new_airline(int aId);
_airlines_l * find_airline(int aId);
_airplane * new_airplane(int pId, int dest, int depart_time);
_flights_l * new_flight(int pId, int depart_time);
int insert_flight_at_dest(int pId, int dest, int depart_time);
int delete_flight_at_dest(int pId, int dest);
void print_output();
int activate_delete_flight_at_dest=1; /*global metavliti gia to an tha diagrafoume kai apo ton pinaka proorismwn*/
_airlines_l *get_next_node(_airlines_l *comp, int movelevel, int direction);
int move_to_airline(_airplane *pl, _airlines_l *comp);


/*Dimiourgia kai arxikopiisi neou komvou gia kathe diaforetiki domi*/


_airlines_l * new_airline(int aId) {
	_airlines_l * node;

	node = (_airlines_l *)malloc(sizeof(_airlines_l));
	assert(node != NULL);
	node->p_root = NULL;
	node->next = NULL;
	node->prev = NULL;
	node->aId = aId;
	return node;
}

_airplane * new_airplane(int pId, int dest, int depart_time) {
	_airplane * node;
	node = (_airplane*)malloc(sizeof(_airplane));
	assert(node != NULL);
	node->pId = pId;
	node->dest = dest;
	node->depart_time = depart_time;
	node->next = NULL;
	return node;
}

_flights_l * new_flight(int pId, int depart_time) {
	_flights_l * node;
	node = (_flights_l*)malloc(sizeof(_flights_l));
	assert(node != NULL);
	node->pId = pId;
	node->depart_time = depart_time;
	node->next = NULL;
	return node;
}

/*sinartisi pou vriskei mesa sti lista me ta aeroplana to aeroplano me anagnoristiko aId*/

_airlines_l * find_airline(int aId) {
	_airlines_l*curr = airlines_p->next;
	if (airlines_p == NULL) {
		return NULL;
	}
	while (curr != airlines_p) {
		if (curr->aId == aId) {
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

/*sinartisi pou eisagei taksinomimena mia ptisi ston pinaka proorismon */

int insert_flight_at_dest(int pId, int dest, int depart_time) {
	_flights_l * curr_flight = destTable[dest].flight_p;
	_flights_l * prev_flight = destTable[dest].flight_p;
	_flights_l *new_node = new_flight(pId, depart_time);
	assert(dest >= 0 && dest < MAX_DEST);
	if (new_node == NULL) {
		return 0;
	}

	assert(destTable[dest].sentinel_p != NULL);
	if (curr_flight == destTable[dest].sentinel_p) { /*An to destination den exei kati mesa ara h sygkekrimeni thesi deixnei sto frouro */
		destTable[dest].flight_p = new_node; /*ara to new node to vazw sthn arxh*/
		new_node->next = destTable[dest].sentinel_p;
		return 1;
	}
	else if (curr_flight->depart_time > depart_time || /*Diaforetika an periexei ptiseis kai h kainoyria ptisi prepei na mpei san prvti gia to sygkekrimeno index toy destTable */
		(curr_flight->depart_time == depart_time && curr_flight->pId > pId)) {
		new_node->next = destTable[dest].flight_p;
		destTable[dest].flight_p = new_node;
		return 1;
	}

	while (curr_flight != destTable[dest].sentinel_p) { /*Psaxnoyme ti thesi sthn opoia prepei na mpei h pthsh*/
		if (curr_flight->depart_time < depart_time ||
			(curr_flight->depart_time == depart_time && curr_flight->pId < pId)) { /*an  exoun idio depart time proigeitai to mikrotero pId*/

			prev_flight = curr_flight;
			curr_flight = curr_flight->next;
		}
		else {
			prev_flight->next = new_node;
			new_node->next = curr_flight;
			return 1;
		}
	}
	prev_flight->next = new_node; /*periptwsh mpainei sto telos*/
	new_node->next = destTable[dest].sentinel_p;
	return 1;
}

/*diatrexei to pinaka proorismon kai diagrafei to aeroplano me pId */

int delete_flight_at_dest(int pId, int dest){
	struct flights *curr, *prev;
	curr = destTable[dest].flight_p;
	while (curr !=destTable[dest].sentinel_p) {
		if (curr == destTable[dest].flight_p && curr->pId == pId) {
			destTable[dest].flight_p = curr->next;
			free(curr);
			return 1;
		}
		if (curr->pId == pId) {
			prev->next = curr->next;
			free(curr);
			return 1;
		}
		prev = curr;

		curr = curr->next;

	}
	return 0;
}

void print_output() {
	int i = 1;
	_airlines_l * curr2 = airlines_p->next;
	_airplane * curr_plane2;
	while (curr2 != airlines_p) {
		printf("Airline-%d = ", i);
		curr_plane2 = curr2->p_root;
		while (curr_plane2 != NULL) {
			printf("<%d:%d>", curr_plane2->pId, curr2->aId);
			if (curr_plane2->next != NULL) {
				printf(", ");
			}
			curr_plane2 = curr_plane2->next;
		}
		i++;
		curr2 = curr2->next;
		printf("\n");
	}
	printf("\nDONE\n\n");


}
/*
movelevel for how many nodes next
direction: 1 for right, 0 for left
*/
_airlines_l *get_next_node(_airlines_l *comp, int movelevel, int direction) {
	_airlines_l *next_comp;
	assert(comp != NULL);
	next_comp = comp;
	while (movelevel > 0) { /*Proxwra tosous kombous oso einai h timh toy level*/
		if (direction == 0) { /* An theloyme pros ta aristera */
			next_comp = next_comp->prev;
			if (next_comp == airlines_p) { /*an vrei to header to prospername kai pame sto epomeno*/
				next_comp = next_comp->prev;
			}
		}
		else { /* An theloyme pros ta deksia */
			next_comp = next_comp->next;
			if (next_comp == airlines_p) {
				next_comp = next_comp->next;
			}
		}

		movelevel--;
	}
	return next_comp;
}


int move_to_airline(_airplane *pl, _airlines_l *comp) {
	_airplane *curr_pl, *prev_pl;
	assert(comp != NULL);
	assert(pl != NULL);

	curr_pl = comp->p_root;
	if (comp->p_root == NULL) { /*periptosi pou h etairia pou metakineitai to aeroplano den exei hdh aeroplano */
		comp->p_root = pl;
		pl->next = NULL;
		return 1;
	}
	if (curr_pl->pId > pl->pId) { /*h periptosi pou prepei na mpei sthn arxh ths listas twn aeroplanwn*/
		pl->next = comp->p_root;
		comp->p_root = pl;
		return 1;
	}

	prev_pl = comp->p_root;
	while (curr_pl != NULL) {
		if (curr_pl->pId > pl->pId) { /*taksinomisi dhladh an vrethei megaltero pId mpainei sthn prohgoymenh thesi*/
			prev_pl->next = pl;
			if (curr_pl != prev_pl) {
				pl->next = curr_pl;
			}
			else {
				pl->next = NULL;
			}
			return 1;
		}
		prev_pl = curr_pl;
		curr_pl = curr_pl->next;
	}

	prev_pl->next = pl; /*periptwsi pou mpainei sto telos*/
	pl->next = NULL;
	return 1;
}

/**
 * @brief Optional function to initialize data structures that 
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize() {	
	int dest;
	airlines_p = new_airline(0);
	airlines_p->next = airlines_p;
	airlines_p->prev = airlines_p;
	
	for (dest = 0; dest < MAX_DEST; dest++) {		/*kathe lista sto destTable arxikopoieitai vste to header thw na deixnei sto sentinel*/
		destTable[dest].sentinel_p = (_flights_l *)malloc(sizeof(struct flights));		
		assert(destTable[dest].sentinel_p != NULL);
		destTable[dest].sentinel_p->pId = -1;
		destTable[dest].sentinel_p->depart_time = -1;
		destTable[dest].sentinel_p->next = NULL;
		destTable[dest].flight_p = destTable[dest].sentinel_p;		
	}
	
    return 1;
}


/**
 * @brief Register airline
 *
 * @param aId The airline's id
 *
 * @return 1 on success
 *         0 on failure
 */
int register_airline(int aId) {
	_airlines_l * new_node = new_airline(aId);
	_airlines_l * curr;
	assert(airlines_p != NULL);
	airlines_p->next->prev = new_node;  /*panta o kainourgios komvos mpainei sthn arxh dhladh epomenos tou header*/
	new_node->prev = airlines_p;
	new_node->next = airlines_p->next;
	airlines_p->next = new_node;
	if (airlines_p == NULL) {
		return 0;
	}
	printf("Airlines=");
	curr = airlines_p->next;
	while (curr != airlines_p) {
		printf("<%d>", curr->aId);
		if (curr->next !=airlines_p) {
			printf(", ");
		}
		curr = curr->next;

	}
	printf("\n\nDONE\n\n");
    return 1;
}

/**
 * @brief Insert new airplane to the list of the certain airline with aId
 *
 * @param aId         The airlines id
 * @param pId         The airplanes id
 * @param dest        Destination Id [0-9]
 * @param depart_time Departure time
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_airplanes(int aId, int pId, int dest, int depart_time) {
	_airlines_l* curr;
	_airplane *prev_plane, *curr_plane, *new_node;
	assert(dest >= 0 && dest < MAX_DEST);
	assert(airlines_p != NULL);
	curr = find_airline(aId); /*vriskoume se pia etairia tha eisagoume aeroplana*/
	if (curr == NULL) {
		return 0;
	}
	new_node = new_airplane(pId, dest, depart_time);
	if (curr->p_root == NULL) { /*an h etairia den exei aeroplana*/
		curr->p_root = new_node;
		
		if (insert_flight_at_dest(pId, dest, depart_time) == 0) {
			free(new_node);
			return 0;
		}
		print_output();
		return 1;
	}
	prev_plane = curr->p_root;
	curr_plane = curr->p_root;
	if (pId < curr->p_root->pId) { /*an to aeroplano pou tha eisagoume einai mikrotero to pId apo to prwto aeroplano*/
		new_node->next = curr->p_root; /*tha mpei auto sthn arxh*/
		curr->p_root = new_node;
		if (insert_flight_at_dest(pId, dest, depart_time) == 0) {
			free(new_node);
			return 0;
		}
		print_output();
		return 1;
	}

	while (curr_plane != NULL) { /*diatrexoume ta aeroplana*/
		if (curr_plane->pId > pId) { /*elegxo diatirisis taksinomisis*/
			prev_plane->next = new_node;
			new_node->next = curr_plane;
			if (insert_flight_at_dest(pId, dest, depart_time) == 0) {
				free(new_node);
				return 0;
			}
			print_output();
			return 1;
		}
		prev_plane = curr_plane;
		curr_plane = curr_plane->next;
	}
	if (curr_plane == NULL) { /*exoume ftasei sto telos ara prepei to aeroplano na mpei sto telos*/
		prev_plane->next = new_node;
		if (insert_flight_at_dest(pId, dest, depart_time) == 0) {
			free(new_node);
			return 0;
		}
	}
	if (airlines_p == NULL) {
		return 0;
	}
	print_output();
	return 1;
}

/**
 * @brief Cancel flight
 *
 * @param aId         The airlines id
 * @param pId         The airplanes id
 * @param dest        Destination Id [0-9]
 *
 * @return 1 on success
 *         0 on failure
 */
int cancel_flight(int aId, int pId, int dest) {	
	_airlines_l *f;
	_airplane * curr, *prev;
	assert(dest >= 0 && dest < MAX_DEST);
	assert(airlines_p != NULL);
	f = find_airline(aId);
	curr = f->p_root;
	prev = curr;
	
	if (f == NULL) {
		return 0;
	}
	while (curr != NULL && curr->pId <= pId) {		
		if (pId == curr->pId && curr == f->p_root) { /*periptosi pou vriskoume to pId stin arxi ths listas*/
			assert(delete_flight_at_dest(pId, dest) != 0);
			f->p_root = curr->next;			
			free(curr);
			print_output();
			return 1;
		}
		
		if (curr->pId == pId) { /*kathe alli periptosi*/
			assert(delete_flight_at_dest(pId, dest) != 0);
			prev->next = curr->next;
			free(curr);
			print_output();
			return 1;
		}
		prev = curr;
		curr = curr->next;
	}
	print_output();
	return 0;
}

/**
 * @brief Delete airline
 *
 * @param aId         The airlines id
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_airline(int aId) {
	_airlines_l *temp,*curr,*prev;
	_airplane *curr_plane, *prev_plane;
	temp = find_airline(aId);
	curr_plane = temp->p_root;
	prev_plane = curr_plane;
	if (temp == NULL) {
		return 0;
	}
	/*diagrafi aeroplanou apo ti lista aeroplanwn*/
	while (curr_plane!= NULL) {
		if(curr_plane == temp->p_root){ /*an einai sthn arxh tis listas*/
			if (activate_delete_flight_at_dest == 1) { /*an h global metavliti exei th timi 1 mono tote diagrafei tis pthseis me pId apo ton pinaka proorismon*/
				assert(delete_flight_at_dest(curr_plane->pId, curr_plane->dest) != 0);
			}
			temp->p_root = curr_plane->next;
			free(curr_plane);
		}
		else { /*kathe alli periptosi*/
			if (activate_delete_flight_at_dest == 1) {
				assert(delete_flight_at_dest(curr_plane->pId, curr_plane->dest) != 0);
			}			
			prev_plane->next = curr_plane->next;
			free(curr_plane);
		}
		prev_plane = curr_plane;
		curr_plane = curr_plane->next;
	}
	/*diagrafi etairias*/
	curr = airlines_p->next;
	prev = curr;
	while (curr!= airlines_p) {
		if (curr == airlines_p->next && curr->aId==aId) { /*vrisketai stin arxi tis listas*/
			airlines_p->next = curr->next;
			free(curr);
			print_output();
			return 1;
		}
		if(curr->aId == aId) { /*kathe alli periptwsi*/
			prev->next = curr->next;
			free(curr);
			print_output();
			return 1;
		}
		prev = curr;
		curr = curr->next;
	}
	print_output();
	return 0;
}

/**
 * @brief Acquisition airline
 *
 * @param aId1         The first airline id
 * @param aId2         The second airline id
 *
 * @return 1 on success
 *         0 on failure
 */
int acquisition_airline(int aId1, int aId2) {	
	_airlines_l *comp1, *comp2;
	_airplane *pl1, *pl2, *prev2, *temp;
	int delete_succesful = 1;
	comp1 = find_airline(aId1);
	comp2= find_airline(aId2);
	if (comp1 == NULL || comp2 == NULL) {
		return 0;
	}
	pl1 = comp1->p_root;
	pl2 = comp2->p_root;
	prev2 = pl2; /*an vrei megalytero tha mpei san epomeno ayths ths metavlitis gia ayto kai to kratame */
	while (pl1 != NULL && pl2 != NULL) { /*diatexoume tautoxrona kai tis duo listes me aeroplana*/
		if (pl1->pId > pl2->pId) { /*diatirisi taksinomisis*/
			prev2 = pl2;
			pl2 = pl2->next;
		}
		else
		{
			temp = pl1->next; /*kratame to epomeno tou pl1 epeidi to xalame*/
			
			if (pl1 == comp1->p_root) { /*an einai sthn arxi ths listas aeroplanon ths proths etairias*/
				comp1->p_root = pl1->next;
			}

			if (pl2 == comp2->p_root) { /*an einai sthn arxi ths listas aeroplanon ths deuteris etairias*/
				pl1->next = pl2;
				comp2->p_root = pl1;
				prev2 = comp2->p_root;
				pl2 = comp2->p_root;
			}
			else { /*se kathe allh periptosi*/
				pl1->next = pl2;
				prev2->next = pl1;
				prev2 = pl1;
			}
			pl1 = temp;
		}
	}
	activate_delete_flight_at_dest = 0; /*den theloume na diagrapsei apo tous proorismous opote dinoume th timi 0*/
	if (pl1 == NULL) { /*an teleiwse proti h proti lista tote apla kanoume diagrafi tis etairias*/
		delete_succesful = delete_airline(aId1);
		activate_delete_flight_at_dest = 1;
		return delete_succesful;
	}
	else { /*An h lista ton aeroplanwn thw etairiaw poy ta pairnei exei ft;asei sto telos tote ta ypoloipa aeroplana*/
		prev2->next = pl1; /* ths etaireias poy eksagorazetai prepei na metakinithoun opws einai sto telos thw listas ths etairias poy eksagorazei*/
		comp1->p_root = NULL;
		delete_succesful = delete_airline(aId1);
		activate_delete_flight_at_dest = 1;
		return delete_succesful;
	}
    return 1;
}

/**
 * @brief Subsidiary company
 *
 * @param aId1         The first airline id
 * @param aId2         The second airline id
 * @param dest         Destination
 *
 * @return 1 on success
 *         0 on failure
 */
int subsidiary_airiline(int aId1, int aId2, int dest) {
	_airlines_l *comp1, *comp2;
	_airplane *pl1, *pl2, *prev_pl1, *prev_pl2, *temp;
	comp1 = find_airline(aId1);
	comp2 = find_airline(aId2);
	assert(comp1 != NULL);
	assert(comp2 != NULL);
	pl1 = comp1->p_root;
	prev_pl1 = NULL; 
	pl2 = comp2->p_root;
	prev_pl2 = comp2->p_root;

	while (pl1 != NULL) /*diatrexoume ti lista pou eksagorazetai*/
	{
		temp = pl1; 
		if (temp->dest == dest) {
			if (temp == comp1->p_root) { /*periptosi pou einai stin arxi*/
				comp1->p_root = pl1->next;
				pl1 = pl1->next;
			}
			else {
				prev_pl1->next = pl1->next;
				pl1 = pl1->next;
			}
			if ((comp2->p_root == NULL)
				|| (comp2->p_root != NULL && pl2 == comp2->p_root && pl2->pId > temp->pId)) { /*h periptwsh pou to aeroplano prepei na mpei sthn arxh kai auto ginetai eite otan einai keni h lista h einai mikrotero to pId*/
				temp->next = comp2->p_root;
				comp2->p_root = temp;
			}
			else {
				while (pl2 != NULL && pl2->pId < temp->pId) { /* diatirisi taksinomisis mpainontas sto prohgoumeno */
					prev_pl2 = pl2;
					pl2 = pl2->next;
				}

				if (pl2 == NULL) { /*prepei na mpei sto telos*/
					prev_pl2->next = temp;
					temp->next = NULL; /*o epomenos tou teleutaiou komvou prepei na einai null*/
				}
				else {
					temp->next = pl2;
					prev_pl2->next = temp;
					pl2 = temp;
				}
			}
		}
		else {
			prev_pl1 = pl1;
			pl1 = pl1->next;
		}


	}
	print_output();
	return 1;
}


/**
 * @brief Partition airplanes
 *
 * @param aId         The airline id
 *
 * @return 1 on success
 *         0 on failure
 */
int partition_airplanes(int aId1) {
	int counter;
	int moving_right;
	int delete_succesful = 1;
	_airlines_l *comp, *to_comp;
	_airplane *pl;
	comp = find_airline(aId1);	
	assert(comp != NULL);
	
	if (comp->p_root == NULL) { /*an den exei aeroplana apla th svinoume*/
		activate_delete_flight_at_dest = 0;
		delete_succesful = delete_airline(aId1);
		activate_delete_flight_at_dest = 1;
		return delete_succesful;
	}
	counter = 1;
	moving_right = 1; /*1 deksia 0 aristera*/
	while (comp->p_root != NULL) {
		pl = comp->p_root; /*deixnei sthn arxi*/
		comp->p_root = pl->next;

		to_comp = get_next_node(comp, counter, moving_right); /*pairnei ton epomeno komvo pou einai counter theseis makria se kateuthinsi moving right*/
		assert(to_comp != NULL);
		assert(move_to_airline(pl, to_comp) == 1); /*metakinoume to aeroplano pl sthn etairia to_comp*/
		moving_right = (moving_right == 1) ? 0 : 1; /*an h metavliti einai 1 tha ginei 0 alliws tha ginei 1*/
		if (moving_right == 1) {
			counter++;
		}
	}
	activate_delete_flight_at_dest = 0;
	delete_succesful = delete_airline(aId1);
	activate_delete_flight_at_dest = 1;
	return delete_succesful;
}




/**
 * @brief Travel
 *
 * @param dest         Destination
 * @param timestamp    Departure time
 *
 * @return 1 on success
 *         0 on failure
 */
int travel(int dest, int timestamp) {
	struct flights *temp;
	assert(dest >= 0 && dest < MAX_DEST);
	temp = destTable[dest].flight_p;
	printf("Destination = ");
	while (temp != destTable[dest].sentinel_p) {
		if (temp->depart_time >= timestamp) {
			printf("<%d:%d>,", temp->pId, temp->depart_time);
		}
		temp = temp->next;
	}
	printf("\n\nDONE\n\n");
    return 1;
}


/**
 * @brief Print airlines
 *
 * @return 1 on success
 *         0 on failure
 */
int print_airlines() {
	int i = 1;
	_airlines_l * curr = airlines_p->next;
	_airplane * curr_plane;
	if (airlines_p == NULL) {
		return 0;
	}
	while (curr != airlines_p) {
		printf("Airline-%d = %d\n", i,curr->aId);
		curr_plane = curr->p_root;
		printf("     Airplanes-%d=", i);
		while (curr_plane!=NULL) {
			printf("<%d>", curr_plane->pId);
			if (curr_plane->next != NULL) {
				printf(", ");
			}
			curr_plane = curr_plane->next;
		}
		i++;
		curr = curr->next;
		printf("\n");
	}
	printf("\nDONE\n\n");

    return 1;
}

/**
 * @brief Print destinations
 *
 * @return 1 on success
 *         0 on failure
 */
int print_destinations() {
	int i = 0;
	_flights_l *curr_flight;

	while (i < MAX_DEST) {
		printf("Destination-%d = ", i);
		curr_flight= destTable[i].flight_p;
		while (curr_flight != destTable[i].sentinel_p) {
			printf("<%d:%d>", curr_flight->pId, curr_flight->depart_time);
			if (curr_flight->next != destTable[i].sentinel_p) {
				printf(", ");
			}
			curr_flight=curr_flight->next;
		}
		printf("\n");
		i++;
	}
	printf("\nDONE\n\n");

    return 1;
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
