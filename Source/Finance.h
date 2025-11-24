#ifndef FINANCE_H
#define FINANCE_H

#include<stdio.h>
#include<stdlib.h>

typedef struct Transactions
{
    int id;
    char type[15]; //income or expense
    char des[20]; // 2-3 words description
    float amt;
    char category[15]; //food,education,entertainment...
    char date[11]; //  (dd/mm/yyyy)
    float extraCharge; //extra charges if any

    struct Transactins *next;
    
}transaction;

typedef struct summary
{
    float total_income;
    float total_expense;
    float net_balance;
    float m_inclome; //montl=hly income
    float m_expense;  // monthly expense
}summary;

typedef struct Limits
{
    float m_limit;
    float target;
    float current;
}Limits;


#endif
