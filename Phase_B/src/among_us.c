/***************************************************
 *                                                 *
 * file: among_us.c                                *
 *                                                 *
 * @Author  Skerdi Basha                           *
 * @Version 1-12-2020                              *
 * @email   sbash@csd.uoc.gr                       *
 *                                                 *
 * @brief   Implementation of among_us.h           *
 *                                                 *
 ***************************************************
 */

#include "among_us.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>

struct Player *sent;
int a, b, p, m, count_crewmates = 0, count_aliens = 0;
int not_alien_player_count = 0;
int global_counter = 0;
struct Player *player_with_max_evidence;

#define SHOW_ERROR catch_error(__LINE__, __FILE__);

void catch_error(int line, char *file)
{
    printf("Error at line %d of file %s\n", line, file);
    assert(1 == 0);
}

unsigned int primes_g[650] = {
    179,
    181,
    191,
    193,
    197,
    199,
    211,
    223,
    227,
    229,
    233,
    239,
    241,
    251,
    257,
    263,
    269,
    271,
    277,
    281,
    283,
    293,
    307,
    311,
    313,
    317,
    331,
    337,
    347,
    349,
    353,
    359,
    367,
    373,
    379,
    383,
    389,
    397,
    401,
    409,
    419,
    421,
    431,
    433,
    439,
    443,
    449,
    457,
    461,
    463,
    467,
    479,
    487,
    491,
    499,
    503,
    509,
    521,
    523,
    541,
    547,
    557,
    563,
    569,
    571,
    577,
    587,
    593,
    599,
    601,
    607,
    613,
    617,
    619,
    631,
    641,
    643,
    647,
    653,
    659,
    661,
    673,
    677,
    683,
    691,
    701,
    709,
    719,
    727,
    733,
    739,
    743,
    751,
    757,
    761,
    769,
    773,
    787,
    797,
    809,
    811,
    821,
    823,
    827,
    829,
    839,
    853,
    857,
    859,
    863,
    877,
    881,
    883,
    887,
    907,
    911,
    919,
    929,
    937,
    941,
    947,
    953,
    967,
    971,
    977,
    983,
    991,
    997,
    1009,
    1013,
    1019,
    1021,
    1031,
    1033,
    1039,
    1049,
    1051,
    1061,
    1063,
    1069,
    1087,
    1091,
    1093,
    1097,
    1103,
    1109,
    1117,
    1123,
    1129,
    1151,
    1153,
    1163,
    1171,
    1181,
    1187,
    1193,
    1201,
    1213,
    1217,
    1223,
    1229,
    1231,
    1237,
    1249,
    1259,
    1277,
    1279,
    1283,
    1289,
    1291,
    1297,
    1301,
    1303,
    1307,
    1319,
    1321,
    1327,
    1361,
    1367,
    1373,
    1381,
    1399,
    1409,
    1423,
    1427,
    1429,
    1433,
    1439,
    1447,
    1451,
    1453,
    1459,
    1471,
    1481,
    1483,
    1487,
    1489,
    1493,
    1499,
    1511,
    1523,
    1531,
    1543,
    1549,
    1553,
    1559,
    1567,
    1571,
    1579,
    1583,
    1597,
    1601,
    1607,
    1609,
    1613,
    1619,
    1621,
    1627,
    1637,
    1657,
    1663,
    1667,
    1669,
    1693,
    1697,
    1699,
    1709,
    1721,
    1723,
    1733,
    1741,
    1747,
    1753,
    1759,
    1777,
    1783,
    1787,
    1789,
    1801,
    1811,
    1823,
    1831,
    1847,
    1861,
    1867,
    1871,
    1873,
    1877,
    1879,
    1889,
    1901,
    1907,
    1913,
    1931,
    1933,
    1949,
    1951,
    1973,
    1979,
    1987,
    1993,
    1997,
    1999,
    2003,
    2011,
    2017,
    2027,
    2029,
    2039,
    2053,
    2063,
    2069,
    2081,
    2083,
    2087,
    2089,
    2099,
    2111,
    2113,
    2129,
    2131,
    2137,
    2141,
    2143,
    2153,
    2161,
    2179,
    2203,
    2207,
    2213,
    2221,
    2237,
    2239,
    2243,
    2251,
    2267,
    2269,
    2273,
    2281,
    2287,
    2293,
    2297,
    2309,
    2311,
    2333,
    2339,
    2341,
    2347,
    2351,
    2357,
    2371,
    2377,
    2381,
    2383,
    2389,
    2393,
    2399,
    2411,
    2417,
    2423,
    2437,
    2441,
    2447,
    2459,
    2467,
    2473,
    2477,
    2503,
    2521,
    2531,
    2539,
    2543,
    2549,
    2551,
    2557,
    2579,
    2591,
    2593,
    2609,
    2617,
    2621,
    2633,
    2647,
    2657,
    2659,
    2663,
    2671,
    2677,
    2683,
    2687,
    2689,
    2693,
    2699,
    2707,
    2711,
    2713,
    2719,
    2729,
    2731,
    2741,
    2749,
    2753,
    2767,
    2777,
    2789,
    2791,
    2797,
    2801,
    2803,
    2819,
    2833,
    2837,
    2843,
    2851,
    2857,
    2861,
    2879,
    2887,
    2897,
    2903,
    2909,
    2917,
    2927,
    2939,
    2953,
    2957,
    2963,
    2969,
    2971,
    2999,
    3001,
    3011,
    3019,
    3023,
    3037,
    3041,
    3049,
    3061,
    3067,
    3079,
    3083,
    3089,
    3109,
    3119,
    3121,
    3137,
    3163,
    3167,
    3169,
    3181,
    3187,
    3191,
    3203,
    3209,
    3217,
    3221,
    3229,
    3251,
    3253,
    3257,
    3259,
    3271,
    3299,
    3301,
    3307,
    3313,
    3319,
    3323,
    3329,
    3331,
    3343,
    3347,
    3359,
    3361,
    3371,
    3373,
    3389,
    3391,
    3407,
    3413,
    3433,
    3449,
    3457,
    3461,
    3463,
    3467,
    3469,
    3491,
    3499,
    3511,
    3517,
    3527,
    3529,
    3533,
    3539,
    3541,
    3547,
    3557,
    3559,
    3571,
    3581,
    3583,
    3593,
    3607,
    3613,
    3617,
    3623,
    3631,
    3637,
    3643,
    3659,
    3671,
    3673,
    3677,
    3691,
    3697,
    3701,
    3709,
    3719,
    3727,
    3733,
    3739,
    3761,
    3767,
    3769,
    3779,
    3793,
    3797,
    3803,
    3821,
    3823,
    3833,
    3847,
    3851,
    3853,
    3863,
    3877,
    3881,
    3889,
    3907,
    3911,
    3917,
    3919,
    3923,
    3929,
    3931,
    3943,
    3947,
    3967,
    3989,
    4001,
    4003,
    4007,
    4013,
    4019,
    4021,
    4027,
    4049,
    4051,
    4057,
    4073,
    4079,
    4091,
    4093,
    4099,
    4111,
    4127,
    4129,
    4133,
    4139,
    4153,
    4157,
    4159,
    4177,
    4201,
    4211,
    4217,
    4219,
    4229,
    4231,
    4241,
    4243,
    4253,
    4259,
    4261,
    4271,
    4273,
    4283,
    4289,
    4297,
    4327,
    4337,
    4339,
    4349,
    4357,
    4363,
    4373,
    4391,
    4397,
    4409,
    4421,
    4423,
    4441,
    4447,
    4451,
    4457,
    4463,
    4481,
    4483,
    4493,
    4507,
    4513,
    4517,
    4519,
    4523,
    4547,
    4549,
    4561,
    4567,
    4583,
    4591,
    4597,
    4603,
    4621,
    4637,
    4639,
    4643,
    4649,
    4651,
    4657,
    4663,
    4673,
    4679,
    4691,
    4703,
    4721,
    4723,
    4729,
    4733,
    4751,
    4759,
    4783,
    4787,
    4789,
    4793,
    4799,
    4801,
    4813,
    4817,
    4831,
    4861,
    4871,
    4877,
    4889,
    4903,
    4909,
    4919,
    4931,
    4933,
    4937,
    4943,
    4951,
    4957,
    4967,
    4969,
    4973,
    4987,
    4993,
    4999,
    5003,
    5009,
    5011,
    5021,
    5023,
    5039,
    5051,
    5059,
    5077,
    5081,
    5087,
    5099,
    5101,
    5107,
    5113,
    5119,
    5147,
    5153,
    5167,
    5171,
    5179,
};

struct Player *new_player(int pid, int is_alien)
{
    struct Player *node;
    node = (struct Player *)malloc(sizeof(struct Player));
    assert(node != NULL);
    node->pid = pid;
    node->is_alien = is_alien;
    node->evidence = 0;
    node->parrent = NULL;
    node->rc = NULL;
    node->lc = NULL;
    node->tasks = NULL;
    return node;
}

struct Task *new_task(int tid, int difficulty)
{
    struct Task *node;
    node = (struct Task *)malloc(sizeof(struct Task));
    assert(node != NULL);
    node->tid = tid;
    node->difficulty = difficulty;
    node->lcnt = 0;
    node->rc = NULL;
    node->lc = NULL;
    return node;
}

struct HT_Task new_HT_Task(int tid, int difficulty)
{
    struct HT_Task node;
    node.tid = tid;
    node.difficulty = difficulty;
    node.next = NULL;
    return node;
}

int findNextPrime(int key)  /*epomenos prwtos arithmos apo to key pou sto initialize tha doume oti einai o max arithmos twn task*/
{
    int i = 0;
    while (primes_g[i] < key)
    {
        i++;
    }
    return primes_g[i];
}

int random_prime(int x, int y)
{
    srand(time(NULL));
    return (rand() % (y - x + 1)) + x;
}

void _print_players(struct Player *pl)
{
    if (pl == sent)
    {
        return;
    }
    _print_players(pl->lc);
    printf("<%d:%d>", pl->pid, pl->is_alien);
    _print_players(pl->rc);
}

void print_tasts_tree(struct Task *root)
{
    if (root != NULL)
    {
        print_tasts_tree(root->lc);
        printf("<%d:%d>", root->tid, root->difficulty);
        print_tasts_tree(root->rc);
    }
}

void _print_double_tree(struct Player *root)
{
    if (root != sent)
    {
        _print_double_tree(root->lc);
        printf("Player <%d> :", root->pid);
        if (root->tasks != NULL)
        {
            print_tasts_tree(root->tasks);
        }
        printf("\n");
        _print_double_tree(root->rc);
    }
}

void _print_double_tree_evidence(struct Player *root)
{
    if (root != sent)
    {
        _print_double_tree_evidence(root->lc);
        printf("Player <%d>,<%d> :", root->pid, root->evidence);
        if (root->tasks != NULL)
        {
            print_tasts_tree(root->tasks);
        }
        printf("\n");
        _print_double_tree_evidence(root->rc);
    }
}

struct Player *find_player(struct Player *root, int pid)
{
    if (root == sent)
    {
        return NULL;
    }
    if (root->pid == pid)
    {
        return root;
    }
    if (root->pid < pid)
    {
        return find_player(root->rc, pid);
    }
    else
    {
        return find_player(root->lc, pid);
    }
}

struct Task *find_task(struct Task *root, int tid)
{
    if (root == NULL || root->tid == tid)
    {
        return root;
    }
    if (root->tid < tid)
    {
        return find_task(root->rc, tid);
    }
    else
    {
        return find_task(root->lc, tid);
    }
}

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize()
{
    int i;
    sent = (struct Player *)malloc(sizeof(struct Player));
    sent->pid = -1;
    sent->is_alien = 0;
    sent->evidence = 0;
    sent->parrent = NULL;
    sent->tasks = NULL;
    sent->lc = NULL;
    sent->rc = NULL;

    players_tree = sent;

    p = findNextPrime(max_tid_g);
    m = max_tasks_g / 3;
    a = random_prime(0, p - 1); /* ta a,b tou hash table einai tyxaia vash tou orismou*/
    b = random_prime(1, p - 1);
    general_tasks_ht.tasks = (struct HT_Task **)malloc(sizeof(struct HT_Task *) * m);
    general_tasks_ht.count = 0;
    for (i = 0; i < m; i++)
    {
        general_tasks_ht.tasks[i] = NULL;
    }

    completed_tasks_pq.size = 0;
    completed_tasks_pq.tasks = (struct HT_Task *)malloc(sizeof(struct HT_Task) * max_tasks_g);

    return 1;
}

/**
 * @brief Register Player
 *
 * @param pid The player's id
 *
 * @param is_alien The variable that decides if he is an alien or not
 * @return 1 on success
 *         0 on failure
 */
int register_player(int pid, int is_alien)
{
    struct Player *new_node, *curr, *prev;
    new_node = new_player(pid, is_alien);
    if (players_tree == sent)
    {
        players_tree = new_node;
        new_node->lc = sent;
        new_node->rc = sent;
        print_players();
        if (new_node->is_alien == 0)
        {
            count_crewmates++;
        }
        else
        {
            count_aliens++;
        }
        return 1;
    }
    curr = players_tree;
    prev = curr;
    while (curr != sent)
    {
        prev = curr;
        if (pid < curr->pid)
        {
            curr = curr->lc;
        }
        else
        {
            curr = curr->rc;
        }
    }

    if (new_node->pid > prev->pid)
    {
        prev->rc = new_node;
    }
    else if (new_node->pid < prev->pid)
    {
        prev->lc = new_node;
    }
    else
    {
        SHOW_ERROR
    }
    new_node->parrent = prev;
    new_node->lc = sent;
    new_node->rc = sent;
    print_players();
    if (new_node->is_alien == 0)
    {
        count_crewmates++;
    }
    else
    {
        count_aliens++;
    }
    return 1;
}

int hash(int tid)
{
    return ((a * tid + b) % p) % m; /*synarthsh tou katholikou katakermatismou symfwna me tis diafanies*/
}

/**
 * @brief Insert Task in the general task hash table
 *
 * @param tid The task id
 *
 * @param difficulty The difficulty of the task
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_task(int tid, int difficulty)
{
    struct HT_Task *new_node = (struct HT_Task *)malloc(sizeof(struct HT_Task));
    int pos = hash(tid);
    new_node->tid = tid;
    new_node->difficulty = difficulty;
    if (new_node == NULL)
    {
        return 0;
    }
    if (general_tasks_ht.tasks[pos] == NULL)
    {
        general_tasks_ht.tasks[pos] = new_node;
        new_node->next = NULL;
    }
    else
    {
        new_node->next = general_tasks_ht.tasks[pos];
        general_tasks_ht.tasks[pos] = new_node;
    }
    general_tasks_ht.count++;
    print_tasks();
    return 1;
}


/*-------------------- Distribute-------------------*/



void insert_task_players(struct Player *pl, struct Task *new_node)
{
    struct Task *curr, *prev;
    assert(pl != NULL);
    curr = pl->tasks;
    prev = curr;

    if (curr == NULL)
    {
        pl->tasks = new_node;
        return;
    }

    while (curr != NULL)
    {
        prev = curr;
        if (new_node->tid < curr->tid)
        {
            curr->lcnt++;
            curr = curr->lc; /*paw sto aristero paidi ara auksanw to lcnt*/
        }
        else if (new_node->tid > curr->tid)
        {
            curr = curr->rc; /*paw deksia den auksanw to lcnt*/
        }
        else
        {
            return; /*yparxei hdh*/
        }
    }

    if (new_node->tid < prev->tid)
    {
        prev->lc = new_node;
    }
    else if (new_node->tid > prev->tid)
    {
        prev->rc = new_node;
    }
}

void inorder_players(struct Player *root, struct Task *task, int assign_player_count)
{
    if (root == sent)
    {
        return;
    }

    inorder_players(root->lc, task, assign_player_count);

    if (root->is_alien == 0)
    {
        not_alien_player_count++; /*se poio mi alien paixth vrisketai h diasxisi*/
    }

    if (root->is_alien == 0 && assign_player_count == not_alien_player_count) /*otan ftasei sto paixth pou theloume kanoume eisagwgh*/
    {

        insert_task_players(root, task);
        return;
    }

    inorder_players(root->rc, task, assign_player_count);
}

/**
 * @brief Distribute Tasks to the players
 * @return 1 on success
 *         0 on failure
 */
int distribute_tasks()
{
    struct HT_Task *curr;
    struct Task *task;
    int assign_player_count = 0; /* poios paixths tha parei to task */
    int i, task_counter;
    task_counter = 0;

    for (i = 0; i < m; i++)
    {
        curr = general_tasks_ht.tasks[i]; /*diasxizw thn alysida*/
        while (curr != NULL)
        {
            task = new_task(curr->tid, curr->difficulty);
            not_alien_player_count = 0;
            inorder_players(players_tree, task, assign_player_count + 1);

            curr = curr->next;
            task_counter++;
            assign_player_count++;
            if (assign_player_count % count_crewmates == 0) /*otan toys perasei olous to mhdenizw kai paw sthn arxh*/
            {
                assign_player_count = 0;
            }
        }
    }

    print_double_tree();

    return 1;
}



/*--------------------IMPLEMENT-----------------------*/



struct Task *find_smallest_task_in_right_subtree(struct Task *node)
{
    struct Task *curr = node;
    while (curr != NULL && curr->lc != NULL)
    {
        curr->lcnt--;
        curr = curr->lc;
    }
    return curr;
}

void insert_PQ(struct HT_Task tsk) /*eisaw=gwgh sth pq*/
{
    int parent_index, size = completed_tasks_pq.size;
    assert(completed_tasks_pq.size < max_tasks_g);
    parent_index = (size - 1) / 2; /*to megethos tou patera vash theorias*/

    while (size > 0 && completed_tasks_pq.tasks[parent_index].difficulty < tsk.difficulty) /*elegxos gia diatirisi tis taksinomisis*/
    {

        completed_tasks_pq.tasks[size] = completed_tasks_pq.tasks[parent_index];
        size = (size - 1) / 2; 
        parent_index = (size - 1) / 2;
    }
    completed_tasks_pq.tasks[size] = tsk;
    completed_tasks_pq.size++;
    print_pq();
}

struct Task *delete_task(struct Task *root, struct Task *node) /*anadromiki diagrafi tou task apo to dentro kai eisagwgh tou sthn oura*/
{
    struct Task *tmp;
    struct HT_Task pq_task;
    if (root == NULL)
    {
        return root;
    }
    if (node->tid < root->tid)
    {
        root->lc = delete_task(root->lc, node);
    }
    else if (node->tid > root->tid)
    {
        root->rc = delete_task(root->rc, node);
    }
    else
    {
        if (root->lc == NULL)
        {
            tmp = root->rc;
            pq_task = new_HT_Task(root->tid, root->difficulty); /*metatrepw se HT_Task apo task gia na ginei swsra h eisagwgh*/
            insert_PQ(pq_task);
            return tmp;
        }
        else if (root->rc == NULL)
        {
            tmp = root->lc;
            pq_task = new_HT_Task(root->tid, root->difficulty);
            insert_PQ(pq_task);
            return tmp;
        }
        else
        {
            tmp = find_smallest_task_in_right_subtree(root->rc); /*an exei dyo paidia tha psaksei to mikrotero sto deksi upodentro pou einai o amesws epomenos*/
            assert(tmp != NULL);
            tmp->lcnt = root->lcnt;
            root = tmp;
            root->rc = delete_task(root->rc, tmp);
        }
    }
    return root;
}

/**
 * @brief Implement Task
 *
 * @param pid The player's id
 *
 * @param tid The task's tid
 *
 * @return 1 on success
 *         0 on failure
 */
int implement_task(int pid, int tid)
{
    struct Player *pl = find_player(players_tree, pid);
    struct Task *tsk;
    if (pl == NULL)
    {
        return 1;
    }
    tsk = find_task(pl->tasks, tid);
    if (pl->tasks == NULL || tsk == NULL)
    {
        return 1;
    }
    assert(pl != NULL);
    assert(pl->tasks != NULL);
    assert(tsk != NULL);

    pl->tasks = delete_task(pl->tasks, tsk);
    print_double_tree();
    return 1;
}


/*---------------EJECT---------------*/


void tree_to_array(struct Task *node, struct Task *arr[]) /*pairnei ena struct dentro kai to kanei pinaka struct*/
{
    if (node == NULL)
    {
        return;
    }
    tree_to_array(node->lc, arr);
    arr[global_counter++] = node; /*megethos pinaka dinetai apo global metavliti pou arxikopoieitai sto event kai auksanetai analoga to megethos tou pinaka*/
    tree_to_array(node->rc, arr);
}

int count_nodes(struct Task *root) /*metraei tous komvous toy dentroy*/
{
    int count = 0;
    if (root->lc != NULL)
        count += count_nodes(root->lc);
    if (root->rc != NULL)
        count += count_nodes(root->rc);
    return count + 1;
}

struct Task *array_to_bst(struct Task *arr[], int start, int end) /*metatrepei to pinaka se dentro*/ 
{
    int middle = (int)(end + start) / 2;

    if (start - end > 0)
    {
        return NULL;
    }
    arr[middle]->lc = NULL; /*pairnei to mesaio stoixeio kai to kanei riza*/
    arr[middle]->rc = NULL;
    assert(arr[middle] != NULL);

    arr[middle]->lc = array_to_bst(arr, start, middle - 1); /*kanei anadromika thn idia diakasia gia to aristero kai to deksi paidi tou*/
    if (arr[middle]->lc != NULL)
    {
        arr[middle]->lc->lcnt = middle - start; /*se periptwsh pou yparxei aristero paidi ftiaxnw to lcnt*/
    }
    arr[middle]->rc = array_to_bst(arr, middle + 1, end);

    return arr[middle];
}

struct Task *merge(struct Task *root1, struct Task *root2)
{
     /*syghwneysh twn pinakwn taksinomimena se ena trito pinaka pou ton dinoume sth parapanw synarthsh mazi me thn arxh to 0 
     kai to telos pou einai to megethos -1*/

    int n1 = count_nodes(root1);
    int n2 = count_nodes(root2);
    int i = 0, j = 0, k = 0;

    struct Task *arr1[n1];
    struct Task *arr2[n2];
    struct Task *arr3[n1 + n2];
    global_counter = 0;
    tree_to_array(root1, arr1);
    global_counter = 0;
    tree_to_array(root2, arr2);

    while (i < n1 && j < n2)
    {

        if (arr1[i]->tid < arr2[j]->tid)
        {
            arr3[k] = arr1[i];
            i++;
        }
        else
        {
            arr3[k] = arr2[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr3[k] = arr1[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr3[k] = arr2[j];
        j++;
        k++;
    }

    return array_to_bst(arr3, 0, n1 + n2 - 1);
}

struct Player *find_smallest_in_right_subtree(struct Player *node)
{
    struct Player *curr = node;
    while (curr != sent && curr->lc != sent)
    {
        curr = curr->lc;
    }
    return curr;
}

void delete_player(struct Player *root, struct Player *node) /*Diagrafi tou paixth anadromika*/
{
    struct Player *tmp;
    if (root == sent)
    {
        return;
    }
    if (node->pid < root->pid)
    {
        delete_player(root->lc, node);
    }
    else if (node->pid > root->pid)
    {
        delete_player(root->rc, node);
    }
    else /*ginontai oi kataliles syndeseis se kathe periptwsh*/
    {
        if (root == players_tree && root->rc == sent) /*periptwsh poy den exei deksi paidi*/
        {
            players_tree = root->lc;
            free(root);
            return;
        }
        if (root->rc == sent && root->lc == sent) /*an den exei paidia*/
        {
            if (root == root->parrent->lc)
            {
                root->parrent->lc = sent;
            }
            else if (root == root->parrent->rc)
            {
                root->parrent->rc = sent;
            }

            free(root);
            return;
        }
        else if (root->lc == sent) /* an exei mono deksi*/
        {

            if (root == root->parrent->lc)
            {
                root->parrent->lc = root->rc;
            }
            else if (root == root->parrent->rc)
            {
                root->parrent->rc = root->rc;
            }
            root->rc->parrent = root->parrent;

            free(root);
            return;
        }
        else if (root->rc == sent) /*mono aristero*/
        {
            if (root == root->parrent->lc)
            {
                root->parrent->lc = root->lc;
            }
            else if (root == root->parrent->rc)
            {
                root->parrent->rc = root->lc;
            }
            root->lc->parrent = root->parrent;

            free(root);
            return;
        }
        else /*kai ta duo ara prepei na brei to epomeno pou tha parei th thesi toy */
        {

            tmp = find_smallest_in_right_subtree(root->rc);
            root = tmp;
            delete_player(tmp->rc, node);
            return;
        }
    }
    return;
}

/**
 * @brief Eject Player
 *
 * @param pid_1 The ejected player's id
 *
 * @param pid_2 The crewmates id
 *
 * @return 1 on success
 *         0 on failure
 */

int eject_player(int pid_1, int pid_2)
{
    struct Player *ej = find_player(players_tree, pid_1);
    struct Player *crewmate = find_player(players_tree, pid_2);
    if (ej != NULL)
    {
        print_double_tree();
        return 1;
    }
    if (crewmate != NULL)
    {
        print_double_tree();
        return 1;
    }

    if (ej->tasks == NULL)
    {
        if (ej->is_alien == 0)
        {
            count_crewmates--;
        }
        else
        {
            count_aliens--;
        }
        delete_player(players_tree, ej);
        print_double_tree();
        return 1;
    }
    if (crewmate->tasks == NULL)
    {
        if (ej->is_alien == 0)
        {
            count_crewmates--;
        }
        else
        {
            count_aliens--;
        }
        crewmate->tasks = ej->tasks;
        delete_player(players_tree, ej);
        print_double_tree();
        return 1;
    }
    if (ej->is_alien == 0)
    {
        count_crewmates--;
    }
    else
    {
        count_aliens--;
    }
    crewmate->tasks = merge(ej->tasks, crewmate->tasks);
    ej->tasks = NULL;
    delete_player(players_tree, ej);
    print_double_tree();

    return 1;
}
/**
 * @brief Witness Eject Player
 *
 * @param pid_1 The ejected player's id
 *
 * @param pid_2 The crewmate's pid
 *
 * @param pid_a The alien's pid
 *
 * @param number_of_witnesses The number of witnesses
 *
 * @return 1 on success
 *         0 on failure
 */
int witness_eject(int pid_1, int pid_2, int pid_a, int number_of_witnesses)
{
    struct Player *ej = find_player(players_tree, pid_1);
    struct Player *crewmate = find_player(players_tree, pid_2);
    struct Player *allien = find_player(players_tree, pid_a);

    if (ej == NULL)
    {
        print_double_tree();
        return 1;
    }

    if (crewmate == NULL)
    {
        print_double_tree();
        return 1;
    }

    if (ej->is_alien == 0)
    {
        count_crewmates--;
    }
    else
    {
        count_aliens--;
    }

    if (ej->tasks == NULL)
    {
        delete_player(players_tree, ej);
        print_double_tree();
        return 1;
    }
    if (crewmate->tasks == NULL)
    {
        crewmate->tasks = ej->tasks;
        delete_player(players_tree, ej);
        print_double_tree();
        return 1;
    }
    crewmate->tasks = merge(ej->tasks, crewmate->tasks);
    ej->tasks = NULL;
    delete_player(players_tree, ej);
    allien->evidence += number_of_witnesses;
    _print_double_tree_evidence(players_tree);
    printf("\n\nDONE\n");
    return 1;
}



/*-------------------SABOTAGE-----------------*/



struct Player *find_right_most_player() /*deksioteros paixths*/
{
    struct Player *curr = players_tree;
    while (curr != sent)
    {
        if (curr->rc == sent)
        {
            return curr;
        }
        curr = curr->rc;
    }
    return curr;
}

struct Player *find_left_most_player() /*aristeroteros paixths*/
{
    struct Player *curr = players_tree;
    while (curr != sent)
    {
        if (curr->lc == sent)
        {
            return curr;
        }
        curr = curr->lc;
    }
    return curr;
}

struct Player *FindInorderPredecessor(struct Player *node) /*prohgoumenos komvos endodiatetagmeni*/
{
    struct Player *temp, *par;
    assert(node != NULL);

    if (node == find_left_most_player())
    {
        return find_right_most_player();
    }
    par = node->parrent;
    if (node->lc != sent)
    {
        temp = node->lc;
        while (temp->rc != sent)
        {
            temp = temp->rc;
        }
        return temp;
    }
    while (par != sent && par->lc == node)
    {
        node = par;
        par = par->parrent;
    }
    return par;
}

struct Player *FindInorderSuccessor(struct Player *node) /*epomenos komvos endodiatetagmeni*/
{
    struct Player *temp, *par;
    assert(node != NULL);
    par = node->parrent;
    if (node == find_right_most_player())
    {
        return find_left_most_player();
    }

    if (node->rc != sent)
    {
        temp = node->rc;
        while (temp->lc != sent)
        {
            temp = temp->lc;
        }
        return temp;
    }
    while (par != sent && par->rc == node)
    {
        node = par;
        par = par->parrent;
    }
    return par;
}

struct Task *max_priority()
{
    int size = completed_tasks_pq.size, i = 0, j = 0;
    struct HT_Task *temp;
    struct Task *max;
    if (completed_tasks_pq.size == 0)
    {
        return NULL;
    }
    max = new_task(completed_tasks_pq.tasks[0].tid, completed_tasks_pq.tasks[0].difficulty); /*pairnoume to stoixeio sth thesh 0 pou exei panta th megalhterh proteraiothta*/
    temp = &completed_tasks_pq.tasks[size - 1];
    size--;
    if (size == 1)
    {
        completed_tasks_pq.size--;
        return max;
    }
    while ((2 * j + 1 < size && temp->difficulty < completed_tasks_pq.tasks[2 * j + 1].difficulty) || /*ftiaxnoume ksana th diataksi sto pinaka kai tha prepei na elegxw anadromika ta paidia wste na exoun tis swstes theseis */
           (2 * j + 2 < size && temp->difficulty < completed_tasks_pq.tasks[2 * j + 2].difficulty))    /* den arkei ena aplo shift aristera logw ths domhs ths heap priority*/
    {
        if (2 * j + 2 < size)
        {
            if (completed_tasks_pq.tasks[2 * j + 1].difficulty > completed_tasks_pq.tasks[2 * j + 2].difficulty)
            {
                i = 2 * j + 1;
            }
            else
            {
                i = 2 * j + 2;
            }
        }
        else
        {
            i = size - 1;
        }
        completed_tasks_pq.tasks[j] = completed_tasks_pq.tasks[i];
        j = i;
    }
    completed_tasks_pq.tasks[j] = *temp;

    completed_tasks_pq.size--;
    return max;
}

/**
 * @brief Sabotage
 *
 * @param number_of_tasks The number of tasks to be sabotaged
 *
 * @param pid The player's id
 *
 * @return 1 on success
 *         0 on failure
 */
int sabotage(int number_of_tasks, int pid)
{
    struct Player *pl = find_player(players_tree, pid);
    struct Player *pred, *suc, *temp;
    struct Task *tsk;
    int i, j;
    assert(pl != NULL);
    print_pq();
    print_double_tree();
    if (number_of_tasks > completed_tasks_pq.size)
    {
        number_of_tasks = completed_tasks_pq.size;
    }
    i = number_of_tasks / 2;
    j = number_of_tasks - 1;
    pred = pl;
    while (i > 0)
    {
        pred = FindInorderPredecessor(pred); /*paei pisw osous predecessor xreiazoamste*/
        i--;
    }
    temp = pred;
    while (pred->is_alien == 1) /*ginetai elegxos gia alien*/
    {
        pred = FindInorderPredecessor(pred);
        if (pred == temp)
        {
            return 1;
        }
    }

    tsk = max_priority(); /*pairnei apo thn oura to stoixeio me megisti proteraiothta kai t0 eisagei sto paixti*/

    insert_task_players(pred, tsk);
    suc = pl;
    while (j > 0) /*meta vazei ta upoloipa pou prepei stous successors*/
    {
        suc = FindInorderSuccessor(suc);
        if (suc->is_alien == 0)
        {
            tsk = max_priority();

            insert_task_players(suc, tsk);
        }
        else
        {
            temp = suc;
            while (suc->is_alien == 1)
            {
                suc = FindInorderSuccessor(suc);
                if (suc == temp)
                {
                    return 1;
                }
            }
            tsk = max_priority();
            insert_task_players(suc, tsk);
        }
        j--;
    }
    print_pq();
    print_double_tree();

    return 1;
}


/*----------------VOTE------------*/


int find_max_evidence(struct Player *root) /*briskei poio einai to max evidence sth lista twn paixtvn*/
{
    int max;
    if (root == sent)
    {
        return 0;
    }
    max = root->evidence;
    if (find_max_evidence(root->lc) > max)
    {
        max = find_max_evidence(root->lc);
    }

    if (find_max_evidence(root->rc) > root->evidence)
    {
        max = find_max_evidence(root->rc);
    }
    return max;
}

struct Player *find_player_max_evidence(struct Player *pl, int max) /*pairnontas dedomeno to max vriskei to paixti me auto to evidence*/
{
    struct Player *tmp;
    if (pl == sent)
    {
        return sent;
    }

    if (pl->evidence == max)
    {
        return pl;
    }
    else
    {
        tmp = find_player_max_evidence(pl->lc, max);
        if (tmp != sent)
        {
            return tmp;
        }
        return find_player_max_evidence(pl->rc, max);
    }
}
/**
 * @brief Vote
 *
 * @param pid_1 The suspicious player's id
 *
 * @param pid_2 The crewmate's pid
 *
 * @param vote_evidence The vote's evidence
 *
 * @return 1 on success
 *         0 on failure
 */
int vote(int pid_1, int pid_2, int vote_evidence)
{
    struct Player *sus = find_player(players_tree, pid_1);
    struct Player *crewmate = find_player(players_tree, pid_2);
    struct Player *max_evid;
    int max;

    if (sus == NULL)
    {
        _print_double_tree_evidence(players_tree);
        return 1;
    }
    if (crewmate == NULL)
    {
        _print_double_tree_evidence(players_tree);
        return 1;
    }
    max_evid = sent;
    sus->evidence += vote_evidence;
    _print_double_tree_evidence(players_tree);
    max = find_max_evidence(players_tree);

    max_evid = find_player_max_evidence(players_tree, max);
    assert(max_evid != NULL);
    if (crewmate->tasks == NULL)
    {
        crewmate->tasks = max_evid->tasks;
    }
    else if (max_evid->tasks == NULL)
    {
        if (max_evid->is_alien == 0)
        {
            count_crewmates--;
        }
        else
        {
            count_aliens--;
        }
        delete_player(players_tree, max_evid);
        _print_double_tree_evidence(players_tree);
        printf("\n\nDONE\n");
        return 1;
    }
    else if (crewmate->tasks != NULL)
    {
        crewmate->tasks = merge(max_evid->tasks, crewmate->tasks);
    }
    max_evid->tasks = NULL;
    if (max_evid->is_alien == 0)
    {
        count_crewmates--;
    }
    else
    {
        count_aliens--;
    }
    delete_player(players_tree, max_evid);
    _print_double_tree_evidence(players_tree);
    printf("\n\nDONE\n");
    return 1;
}



/*------------SPLIT------------*/



int count_tasks_nodes(struct Task *root) /*metraei toys komvous tou dentrou task*/
{
    struct Task *curr = root;
    int counter = 0;
    while (curr != NULL)
    {
        counter += curr->lcnt + 1;
        curr = curr->rc;
    }
    return counter;
}



void Split(struct Player *pl1, struct Player *pl2, int k)
{
    struct Task *curr, *prev, *p1;
    int broken_left, cnt;
    assert(pl1 != NULL);
    assert(pl2 != NULL);
    assert(k >= 0);
    curr = pl1->tasks;
    prev = curr;
    p1 = pl1->tasks;

    broken_left = 0; /*gia na kseroume an exoume stamathsei otan exoume kinhtei teleftaia fora deksia h aristera */
    cnt = curr->lcnt + 1; /* to cnt exei to plhthos twn komvwn tou aristerou ypodentrou mazi me to trexwn komvo*/

    if (k == 0)
    {
        return;
    }

    while (cnt != k) /* sygkrinoume mexri ton arithmo twn tasks pou prepei na xwrisoume*/
    {
        prev = curr;
        if (cnt > k) /*prepei na pame aristera*/
        {
            cnt = cnt - 1; /*ousiastika afaireitai o trexwn komvos*/

            curr = curr->lc;

            broken_left = 1;
        }
        else if (cnt < k) /*pame deksia*/
        {
            curr = curr->rc;
            cnt = cnt + curr->lcnt + 1; /* ginetai oso htan prin syn ton trexon komvo syn to aristero ypodedntro tou trexwn komvou*/
            broken_left = 0;
            if (cnt > k)
            {
                prev->rc = curr->rc;
                break;
            }
        }
    }

    if (broken_left == 0) /*ti ginetai an spaei deksia*/
    {
        pl2->tasks = prev->rc;
        prev->rc = NULL;
    }
    else /*aristera*/
    {
        pl2->tasks = prev->lc;
        prev->lc = NULL;
    }
}

/**
 * @brief Give Away Work
 *
 * @param pid_1 The existing crewmate's id
 *
 * @param pid_2 The new crewmate's pid
 *
 * @return 1 on success
 *         0 on failure
 */
int give_work(int pid_1, int pid_2)
{
    struct Player *new_node, *curr, *prev, *giver;
    int sum_nodes;
    new_node = new_player(pid_2, 0);
    curr = players_tree;
    prev = curr;
    while (curr != sent)
    {
        prev = curr;
        if (pid_2 < curr->pid)
        {
            curr = curr->lc;
        }
        else
        {
            curr = curr->rc;
        }
    }

    if (new_node->pid > prev->pid)
    {
        prev->rc = new_node;
    }
    else if (new_node->pid < prev->pid)
    {
        prev->lc = new_node;
    }
    new_node->parrent = prev; /*dhmiourgw to neo komvo*/
    new_node->lc = sent;
    new_node->rc = sent;
    count_crewmates++;
    print_double_tree();
    giver = find_player(players_tree, pid_1);
    if (giver == NULL)
    {
        print_double_tree();
        return 1;
    }
    sum_nodes = count_tasks_nodes(giver->tasks);
    Split(giver, new_node, sum_nodes / 2);
    print_double_tree();
    return 1;
}

/**
 * @brief Terminate
 *
 * @return 1 on success
 *         0 on failure
 */
int terminate()
{
    if (count_aliens > count_crewmates)
    {
        printf("\n Aliens win \n");
    }
    else if (count_aliens == 0 || completed_tasks_pq.size == max_tasks_g)
    {
        printf("\n Crewmates win \n");
    }
    printf("\n\nDONE\n");
    return 1;
}

/**
 * @brief Print Players
 *
 * @return 1 on success
 *         0 on failure
 */
int print_players()
{
    struct Player *pl = players_tree;
    if (pl == sent)
    {
        return 0;
    }
    printf("\nPlayers = ");
    _print_players(pl);
    printf("\n\nDONE\n");

    return 1;
}

/**
 * @brief Print Tasks
 *
 * @return 1 on success
 *         0 on failure
 */
int print_tasks()
{
    int i = 0;
    struct HT_Task *temp;
    if (general_tasks_ht.tasks == NULL)
    {
        return 0;
    }
    for (i = 0; i < m; i++)
    {
        temp = general_tasks_ht.tasks[i];
        printf("\tIndex %d : ", i);
        while (temp != NULL)
        {
            printf("<%d:%d>", temp->tid, temp->difficulty);
            temp = temp->next;
        }
        printf("\n");
    }
    printf("\nDONE\n");

    return 1;
}

/**
 * @brief Print Priority Queue
 *
 * @return 1 on success
 *         0 on failure
 */
int print_pq()
{
    int i;
    printf("\t Completed Tasks = : ");
    for (i = 0; i < completed_tasks_pq.size; i++)
    {
        printf("<%d,%d> ", completed_tasks_pq.tasks[i].tid, completed_tasks_pq.tasks[i].difficulty);
    }

    printf("\nDONE\n");

    return 1;
}

/**
 * @brief Print Players & Task tree
 *
 * @return 1 on success
 *         0 on failure
 */
int print_double_tree()
{
    struct Player *temp = players_tree;
    _print_double_tree(temp);
    printf("\n\nDONE\n");
    return 1;
}

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */

int free_all(void)
{
    return 1;
}
