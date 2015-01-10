/*---------------------------------------------------------------------------
 * 
 * wrr.c 
 *
 *     01/05/2015 - 
 *
 *     <sunil895@gmail.com>
 *   
 * Alorithm: Weighted Round Robin
 * Queues set Q = {Q0, Q1, …, Qn-1};
 * W(Qi) indicates the weight of Qi;
 * qindex indicates the flow selected last time, and qindex is initialized with -1;
 * cw is the current weight in scheduling, and cw is initialized with 0; 
 * max(Q) is the maximum weight of all the flows in Q;
 * gcd(Q) is the greatest common divisor of all flow weights in Q;
 *
 * Source: http://kb.linuxvirtualserver.org/wiki/Weighted_Round-Robin_Scheduling
 *---------------------------------------------------------------------------
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define QUEUES 4

/*
 * Compute the Greatest Common Divisor (GCD) of two numbers.
 * This implementation uses Euclid's algorithm:
 *    gcd(a, 0) = a
 *    gcd(a, b) = gcd(b, a mod b)
 *
 */
static inline int
get_gcd(int a, int b)
{
        int c;
     
        if (a == 0)
                return b;
        if (b == 0)
                return a;
                             
        if (a < b) {
                c = a;
                a = b;
                b = c;
        }   
                             
        while (b != 0) {
                c = a % b;
                a = b;
                b = c;
        }   
     
        return a;
}

/*
 * Compute sum of weights
 */
static inline int
get_sum(int wrr_cost[], int wsize)
{
    int i, sum = 0;
    for (i = 0; i < wsize; i++)
        sum += wrr_cost[i];
    return sum;
}

/* 
 * Caluclate max weight out of given weights
 */
static inline int
get_max(int wrr_cost[], int wsize) 
{
    int maxw = 0, i;

    for (i = 0; i < wsize; i++) {
        if (i == 0) 
            maxw = wrr_cost[i];
        else if (wrr_cost[i] > maxw) 
            maxw = wrr_cost[i];
    }
    return maxw;
}

/*
 * main routine
 */
int main(int agrc, char *agrv[]) 
{
    char *queues[] = {"Q1", "Q2", "Q3", "Q4"};
    int wi;
    int wrr_cost[QUEUES];
    int gcd, gcd1, gcd2, maxw, sum;
    int qindex = -1, cw = 0;
    int itr = 0;
    
    if (agrc < 6 || agrc > 6) {
        printf("Usage: ./wrr queue_weights weight0 weight1" 
               "weight2 weight3\n");
        exit(EXIT_FAILURE);
    }

    for (wi = 0; wi < QUEUES; wi++) {
        wrr_cost[wi] = atol(agrv[wi + 2]);
    }

    maxw = get_max(wrr_cost, QUEUES);
    sum = get_sum(wrr_cost, QUEUES);
    gcd1 = get_gcd(wrr_cost[0], wrr_cost[1]);
    gcd2 = get_gcd(wrr_cost[2], wrr_cost[3]);
    gcd = get_gcd(gcd1, gcd2);
   
    for (itr; itr < sum; itr ++) {
        while (1) {
            qindex = (qindex + 1) % QUEUES;
            if (qindex == 0) {
                cw = cw - gcd;
                if (cw <= 0) {
                    cw = maxw;
                    if (cw == 0)
                        break;
                }
            }
            if (wrr_cost[qindex] >= cw) {
                /* At this point the WRR scheduler will dequeue packets from
                 * different traffic flows. To keep it simple I am just
                 * printing the flow selected.
                 */
                printf("Queues selected: %s\n", queues[qindex]);
                break;
            }
        }
    }
    return 0;
}
